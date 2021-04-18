#pragma once
#include <string>
#include <windows.h>
#include "Scanner.h"
#include "InformationStorage.h"

class Monitoring : public Scanner
{
	static bool shouldStop;

public:	
	Monitoring();
	void start(std::string path, HANDLE pipe, InformationStorage& infoStorage);
	void stop();

private:
	void run(std::string path, HANDLE pipe, InformationStorage& infoStorage);
	void startScan(std::string path, HANDLE pipe, InformationStorage& infoStorage);
	message scanDirectory(const std::string& path, HANDLE pipe, InformationStorage& infoStorage);
	bool scanFile(const std::string& path, HANDLE pipe, InformationStorage& infoStorage, Database db);

private:
	HANDLE changeHandle = INVALID_HANDLE_VALUE;
};