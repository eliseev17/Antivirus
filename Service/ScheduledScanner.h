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
#include <memory>
#include <thread>
#include <time.h>
#include "InformationStorage.h"
#include "Database.h"
#include "Scanner.h"

class ScheduledScanner : public Scanner
{
	static bool shouldCancel;
public:
	ScheduledScanner(std::shared_ptr<InformationStorage> infoStorage);
	void cancelScheduledScan();
	void startScheduledScan(HANDLE pipe, message scanMsg);
private:
	message scanDirectory(const std::string& path, HANDLE pipe);
	bool scanFile(const std::string& path, HANDLE pipe, Database db);
	std::mutex scanMutex;
	std::shared_ptr<InformationStorage> infoStorage;
};
