#pragma once
#include <string>
#include <handleapi.h>
#include "Scanner.h"

class Monitoring
{
public:

	Monitoring(const std::string& path);

	std::string start(bool& safe, std::string& virusName);
	void resume();
	void pause();
	void stop();

	inline std::string getPath() { return path; }

private:
	void run(bool& safe, std::string& virusName);
private:
	Scanner scanner;
	std::string path;
	HANDLE changeHandle = INVALID_HANDLE_VALUE;
	bool shouldStop = false;
	bool shouldPause = false;
};