#pragma once
#include <string>
#include <windows.h>
#include "Scanner.h"
#include "InformationStorage.h"

class Monitoring : public Scanner
{
	static bool shouldStop;

public:	
	Monitoring(std::shared_ptr<InformationStorage> infoStorage);
	void start(std::string path, HANDLE pipe);
	void stop();

private:
	void run(std::string path, HANDLE pipe);
	void startScan(std::string path, HANDLE pipe);
	message scanDirectory(const std::string& path, HANDLE pipe);
	bool scanFile(const std::string& path, HANDLE pipe, Database db);

private:
	HANDLE changeHandle = INVALID_HANDLE_VALUE;
	std::shared_ptr<InformationStorage> infoStorage;
};