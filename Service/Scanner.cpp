#include "Scanner.h"
#include "PipeNamesBuffsizesPaths.h"

bool Scanner::shouldStop = false;

Scanner::Scanner(std::shared_ptr<InformationStorage> infoStorage)
{
	this->infoStorage = infoStorage;
}

message Scanner::scanDirectory(const std::string& path, HANDLE pipe)
{
	message result;
	namespace fs = std::filesystem;
	uint64_t numberOfFiles = std::count_if(fs::recursive_directory_iterator(path.data()),
		fs::recursive_directory_iterator{}, [](auto p) {return fs::is_regular_file(p); });
	result.nArr.emplace_back(numberOfFiles);
	result.cmd = COMMAND::START;
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.nArr.clear();
	Database db = Database(DATABASE_PATH);
	size_t virusCounter = 0;
	size_t filesCounter = 0;
	for (auto& p : fs::recursive_directory_iterator(path.data()))
	{
		if (shouldStop)
		{
			result.cmd = COMMAND::STOP;
			result.sArr.clear();
			result.nArr.emplace_back(filesCounter);
			result.nArr.emplace_back(virusCounter);
			return result;
		}

		if (std::filesystem::is_directory(p.path()))
			continue;

		filesCounter++;

		if (scanFile(p.path().string(), pipe, db))
			virusCounter++;
	}
	result.cmd = COMMAND::SCAN_RESULT;
	result.sArr.clear();
	result.nArr.emplace_back(filesCounter);
	result.nArr.emplace_back(virusCounter);
	return result;
}

bool Scanner::scanFile(const std::string& path, HANDLE pipe, Database db)
{
	message result;	
	result.cmd = COMMAND::START;
	bool isVirus = false;
	std::ifstream fileStream(path.c_str(), std::ios::binary);
	uint16_t mzHeader = 0;
	uint32_t zipHeader = 0;
	fileStream.read((char*)&mzHeader, sizeof(uint16_t));
	fileStream.seekg(0);
	fileStream.read((char*)&zipHeader, sizeof(uint32_t));
	fileStream.close();
	if (!result.sArr.empty())
		result.sArr.at(0) = path;
	else
		result.sArr.emplace_back(path);
	if (mzHeader == MZHEADER)
	{
		size_t res = scanExe(path, db);
		if (res)
		{
			isVirus = true;
			infoStorage->addThreat(path, "Найден вредонос: " + db.MW_NAME.at(res));
			result.sArr.emplace_back("Найден вредонос: " + db.MW_NAME.at(res));
		}
		else
			result.sArr.emplace_back("Не опасен");
	}
	else if (zipHeader == ZIPHEADER)
	{
		size_t res = scanZip(path, db);
		if (res)
		{
			isVirus = true;
			infoStorage->addThreat(path, "Найден вредонос: опасный ZIP");
			result.sArr.emplace_back("Найден вредонос: опасный ZIP");
		}
		else
			result.sArr.emplace_back("Не опасен");
	}
	else
		result.sArr.emplace_back("Не исполняемый");
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.sArr.clear();
	//std::cout << path << '\n';	
	return isVirus;
}

size_t Scanner::scanZip(std::string inputPath, Database db)
{
	//std::cout << "Распаковка: " << inputPath << std::endl;
	std::string unzipPath = unzip(inputPath);
	size_t virusCounter = 0, zipVirCounter = 0;
	namespace fs = std::filesystem;
	for (auto& p : fs::recursive_directory_iterator(unzipPath.data()))
	{
		if (std::filesystem::is_directory(p.path()))
			continue;
		std::string path = p.path().string();
		std::ifstream fileStream(path.c_str(), std::ios::binary);
		uint16_t mzHeader = 0;
		uint32_t zipHeader = 0;
		fileStream.read((char*)&mzHeader, sizeof(uint16_t));
		fileStream.seekg(0);
		fileStream.read((char*)&zipHeader, sizeof(uint32_t));
		fileStream.close();
		if (mzHeader == MZHEADER)
		{
			size_t res = scanExe(path, db);
			if (res)
				virusCounter++;
		}
		else if (zipHeader == ZIPHEADER)
		{
			zipVirCounter = scanZip(path, db);
		}
	}
	std::filesystem::remove_all(unzipPath);
	if (virusCounter > 0)
		zipVirCounter++;
	return zipVirCounter;
}

size_t Scanner::scanExe(std::string path, Database db)
{
	uint64_t fSize = 0;
	std::ifstream stream(path, std::ios::binary);
	fSize = std::filesystem::file_size(path);
	size_t resultPosition = 0;
	uint32_t offset = db.getMinOFFSET_BEGIN();
	stream.seekg(offset);
	while (!stream.eof() && offset < fSize)
	{
		std::string buf(BUFSIZE, 0);
		stream.read(buf.data(), BUFSIZE);
		resultPosition = db.check(buf, offset);
		if (resultPosition)
			break;
		offset += BUFSIZE;
	}
	return resultPosition;
}

void Scanner::startScan(const std::string& path, HANDLE pipe)
{
	shouldStop = false;
	if (std::filesystem::is_directory(path))
	{
		message scanResult = scanDirectory(path, pipe);
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
	}
	else
	{
		message scanRes;
		scanRes.cmd = COMMAND::START;
		scanRes.nArr.emplace_back(1);
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanRes);
		Database db = Database(DATABASE_PATH);
		scanRes.cmd = COMMAND::SCAN_RESULT;
		if (scanFile(path, pipe, db))
			scanRes.nArr.emplace_back(1);
		else
			scanRes.nArr.emplace_back(0);
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanRes);
	}
}

void Scanner::stopScan()
{
	shouldStop = true;
}

std::string Scanner::unzip(const std::string& zipPath)
{
	const std::string inputFolder = UNPACKING_PATH;
	std::filesystem::path zipFileName(zipPath);
	zipFileName = zipFileName.replace_extension("").filename();
	std::string pathToZip = inputFolder + zipFileName.string();
	std::filesystem::create_directory(pathToZip);
	int err;
	struct zip* hZip = zip_open(zipPath.c_str(), 0, &err);
	if (hZip)
	{
		size_t totalIndex = zip_get_num_entries(hZip, 0);
		for (size_t i = 0; i < totalIndex; i++)
		{
			struct zip_stat st;
			zip_stat_init(&st);
			zip_stat_index(hZip, i, 0, &st);
			struct zip_file* zf = zip_fopen_index(hZip, i, 0);
			if (!zf)
			{
				zip_close(hZip);
				return {};
			}
			std::vector<char> buffer;
			buffer.resize(st.size);
			zip_fread(zf, buffer.data(), st.size);
			zip_fclose(zf);
			std::string destinationPath = pathToZip + "\\" + st.name;
			std::ofstream outfile(destinationPath, std::ios::binary | std::ios::trunc);
			outfile.write(buffer.data(), st.size);
			outfile.close();
		}
		zip_close(hZip);
	}
	return pathToZip;
}