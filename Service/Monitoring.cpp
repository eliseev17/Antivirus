#include "Monitoring.h"

bool Monitoring::shouldStop = false;

Monitoring::Monitoring()
{
}

void Monitoring::start(std::string path, HANDLE pipe, InformationStorage& infoStorage)
{
	shouldStop = false;
	std::wstring wstr(path.begin(), path.end());
	LPWSTR lpath = const_cast<LPWSTR>((wstr.c_str()));
	changeHandle = FindFirstChangeNotification(lpath, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	if (changeHandle == INVALID_HANDLE_VALUE) //���� ������� ��������� �� �������, ������ ��������� ���� �� ����������
	{
		return;
	}
	run(path, pipe, infoStorage);
}

void Monitoring::stop()
{
	shouldStop = true;
}

void Monitoring::run(std::string path, HANDLE pipe, InformationStorage &infoStorage)
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

		startScan(path, pipe, infoStorage);

		CloseHandle(changeHandle);
		std::wstring wstr(path.begin(), path.end());
		LPWSTR lpath = const_cast<LPWSTR>((wstr.c_str()));
		changeHandle = FindFirstChangeNotification(lpath, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	}
}

void Monitoring::startScan(std::string path, HANDLE pipe, InformationStorage& infoStorage)
{
	message scanResult = scanDirectory(path, pipe, infoStorage);
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
}

message Monitoring::scanDirectory(const std::string& path, HANDLE pipe, InformationStorage& infoStorage)
{
	message result;
	namespace fs = std::filesystem;
	uint64_t numberOfFiles = std::count_if(fs::recursive_directory_iterator(path.data()),
		fs::recursive_directory_iterator{}, [](auto p) {return fs::is_regular_file(p); });
	result.nArr.emplace_back(numberOfFiles);
	result.cmd = COMMAND::SET_MONITORING;
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.nArr.clear();
	Database db = Database("./../AntimalwareDatabase.db");
	size_t virusCounter = 0;
	size_t filesCounter = 0;
	for (auto& p : fs::recursive_directory_iterator(path.data()))
	{
		if (std::filesystem::is_directory(p.path()))
			continue;
		filesCounter++;

		if (scanFile(p.path().string(), pipe, infoStorage, db))
			virusCounter++;
	}
	result.cmd = COMMAND::MONITORING_RESULT;
	result.sArr.clear();
	result.nArr.emplace_back(filesCounter);
	result.nArr.emplace_back(virusCounter);
	return result;
}

bool Monitoring::scanFile(const std::string& path, HANDLE pipe, InformationStorage& infoStorage, Database db)
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
			infoStorage.addThreat(path, "������ ��������: " + db.MW_NAME.at(res));
			result.sArr.emplace_back("������ ��������: " + db.MW_NAME.at(res));
		}
		else
			result.sArr.emplace_back("�� ������");
	}
	else if (zipHeader == ZIPHEADER)
	{
		size_t res = scanZip(path, db);
		if (res)
		{
			isVirus = true;
			infoStorage.addThreat(path, "������ ��������: ������� ZIP");
			result.sArr.emplace_back("������ ��������: ������� ZIP");
		}
		else
			result.sArr.emplace_back("�� ������");
	}
	else
		result.sArr.emplace_back("�� �����������");
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.sArr.clear();
	std::cout << path << '\n';
	return isVirus;
}