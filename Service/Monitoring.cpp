#include "Monitoring.h"
#include "PipeNamesBuffsizesPaths.h"

bool Monitoring::shouldStop = false;

Monitoring::Monitoring(std::shared_ptr<InformationStorage> infoStorage) : Scanner(infoStorage)
{
	this->infoStorage = infoStorage;
}

void Monitoring::start(std::string path, HANDLE pipe)
{
	shouldStop = false;
	changeHandle = FindFirstChangeNotificationA(path.c_str(), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	if (changeHandle == INVALID_HANDLE_VALUE) //Если создать описатель не удалось, значит заданного пути не существует
	{
		return;
	}
	run(path, pipe);
}

void Monitoring::stop()
{
	shouldStop = true;
}

void Monitoring::run(std::string path, HANDLE pipe)
{
	while (TRUE)
	{
		WaitForSingleObject(changeHandle, INFINITE);
		if (shouldStop)
		{
			CloseHandle(changeHandle);
			return;
		}
		Sleep(100);
		startScan(path, pipe);
		CloseHandle(changeHandle);
		changeHandle = FindFirstChangeNotificationA(path.c_str(), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	}
}

void Monitoring::startScan(std::string path, HANDLE pipe)
{
	message scanResult = scanDirectory(path, pipe);
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
}

message Monitoring::scanDirectory(const std::string& path, HANDLE pipe)
{
	message result;
	namespace fs = std::filesystem;
	uint64_t numberOfFiles = std::count_if(fs::recursive_directory_iterator(path.data()),
		fs::recursive_directory_iterator{}, [](auto p) {return fs::is_regular_file(p); });
	result.nArr.emplace_back(numberOfFiles);
	result.cmd = COMMAND::SET_MONITORING;
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.nArr.clear();
	Database db = Database(DATABASE_PATH);
	size_t virusCounter = 0;
	size_t filesCounter = 0;
	for (auto& p : fs::recursive_directory_iterator(path.data()))
	{
		if (std::filesystem::is_directory(p.path()))
			continue;
		filesCounter++;

		if (scanFile(p.path().string(), pipe, db))
			virusCounter++;
	}
	result.cmd = COMMAND::MONITORING_RESULT;
	result.sArr.clear();
	result.nArr.emplace_back(filesCounter);
	result.nArr.emplace_back(virusCounter);
	return result;
}

bool Monitoring::scanFile(const std::string& path, HANDLE pipe, Database db)
{
	message result;
	result.cmd = COMMAND::SET_MONITORING;
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