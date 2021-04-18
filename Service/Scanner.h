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
#define BUFSIZE 8128
#define PIPE_BUFSIZE 1024

class Scanner
{
	static bool shouldStop;
public:
	void startScan(const std::string& path, HANDLE pipe, InformationStorage& infoStorage);
	void stopScan();
protected:
	std::string unzip(const std::string& zipPath);
	size_t scanExe(std::string path, Database db);
	size_t scanZip(std::string inputPath, Database db);
	bool scanFile(const std::string& path, HANDLE pipe, InformationStorage& infoStorage, Database db);
	message scanDirectory(const std::string& path, HANDLE pipe, InformationStorage& infoStorage);
private:
	bool isSchedule = false;
};