#pragma once
#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <Windows.h>
#include "IPC.h"
#include "Database.h"
#include <sqlite3.h>
#include <iostream>
#include <algorithm>
#include <zip.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include <time.h>
#include "InformationStorage.h"
#include "Database.h"
#include "Scanner.h"

class ScheduledScanner : public Scanner
{
	static bool shouldCancel;
public:
	void cancelScheduledScan();
	void startScheduledScan(HANDLE pipe, message scanMsg, InformationStorage& infoStorage);
private:
	message scanDirectory(const std::string& path, HANDLE pipe, InformationStorage& infoStorage);
	bool scanFile(const std::string& path, HANDLE pipe, InformationStorage& infoStorage, Database db);
	std::mutex scanMutex;
};
