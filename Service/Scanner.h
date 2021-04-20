#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <Windows.h>
#include "IPC.h"
#include "Database.h"
#include <sqlite3.h>
#include <iostream>
#include "InformationStorage.h"
#include <algorithm>
#include <zip.h>
#include <filesystem>
#include <fstream>

#define MZHEADER 0x5a4d
#define ZIPHEADER 0x04034b50

class Scanner
{
	static bool shouldStop;
public:
	Scanner(std::shared_ptr<InformationStorage> infoStorage);
	void startScan(const std::string& path, HANDLE pipe);
	void stopScan();
protected:
	std::string unzip(const std::string& zipPath);
	size_t scanExe(std::string path, Database db);
	size_t scanZip(std::string inputPath, Database db);
	bool scanFile(const std::string& path, HANDLE pipe, Database db);
	message scanDirectory(const std::string& path, HANDLE pipe);
private:
	bool isSchedule = false;
	std::shared_ptr<InformationStorage> infoStorage;
};