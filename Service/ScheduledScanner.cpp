#include "ScheduledScanner.h"
#include "PipeNamesBuffsizesPaths.h"

bool ScheduledScanner::shouldCancel = false;

ScheduledScanner::ScheduledScanner(std::shared_ptr<InformationStorage> infoStorage) : Scanner(infoStorage)
{
	this->infoStorage = infoStorage;
}

void ScheduledScanner::cancelScheduledScan()
{
	shouldCancel = true;
}

void ScheduledScanner::startScheduledScan(HANDLE pipe, message scanMsg)
{
	shouldCancel = false;
	std::string path = scanMsg.sArr.at(0);
	uint32_t hour = scanMsg.nArr.at(0);
	uint32_t minute = scanMsg.nArr.at(1);
	message scanResult;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(30));
		if (shouldCancel)
		{
			scanResult.cmd = COMMAND::RESET_SCHEDULE;
			Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
			return;
		}
		scanMutex.lock();
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		if (timeinfo->tm_min == minute && timeinfo->tm_hour == hour)
		{
			scanResult = scanDirectory(path, pipe);
			scanMutex.unlock();
			Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
			break;
		}
		else if (timeinfo->tm_min > minute && timeinfo->tm_hour > hour)
		{
			scanMutex.unlock();
			return;
		}
		scanMutex.unlock();
	}
}

message ScheduledScanner::scanDirectory(const std::string& path, HANDLE pipe)
{
	message result;
	namespace fs = std::filesystem;
	uint64_t numberOfFiles = std::count_if(fs::recursive_directory_iterator(path.data()),
		fs::recursive_directory_iterator{}, [](auto p) {return fs::is_regular_file(p); });
	result.nArr.emplace_back(numberOfFiles);
	result.cmd = COMMAND::SCHEDULE_START;
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
	result.cmd = COMMAND::SCHEDULE_SCAN_RESULT;
	result.sArr.clear();
	result.nArr.emplace_back(filesCounter);
	result.nArr.emplace_back(virusCounter);
	return result;
}

bool ScheduledScanner::scanFile(const std::string& path, HANDLE pipe, Database db)
{
	message result;
	result.cmd = COMMAND::SCHEDULE_START;
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