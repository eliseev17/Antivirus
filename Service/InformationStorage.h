#pragma once
#include <vector>
#include <string>

class InformationStorage
{
public:
	InformationStorage();
	void addThreat(std::string threatPath, std::string threatName);
	void addQuar(std::string quarPath, std::string quarName);
	void deleteThreat(std::string threatPath, std::string threatName);
	void deleteFromQuar(std::string quarPath, std::string quarName);
	std::vector<std::string> threatPaths;
	std::vector<std::string> threatNames;
	std::vector<std::string> quarPaths;
	std::vector<std::string> quarNames;
};

