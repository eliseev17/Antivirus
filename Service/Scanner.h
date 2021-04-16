#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <Windows.h>
#include "ScanObject.h"
#include "Library.h"
#include "Database.h"

class Scanner
{
	static bool shouldStop;
public:
	void startScan(const std::string& path, HANDLE pipe);
	void stopScan();
	message scanDirectory(const std::string& path, HANDLE pipe);
	//inline bool scanStopped() { return stopped; }
	void scanFile(const std::string& path, HANDLE pipe);
private:
	bool unzip(const std::string& zipPath);
	size_t scanExe(std::string path, Database db);
	message scanZip(std::string path, HANDLE pipe);

private:
	//bool stopped = false;
};