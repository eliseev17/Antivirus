#include "InformationStorage.h"

InformationStorage::InformationStorage()
{
}

void InformationStorage::addThreat(std::string threatPath, std::string threatName)
{
	bool isExist = false;
	for (size_t i = 0; i < threatPaths.size(); i++)
	{
		if (threatPaths.at(i) == threatPath)
			isExist = true;
	}
	if (!isExist)
	{
		threatPaths.push_back(threatPath);
		threatNames.push_back(threatName);
	}		
}

void InformationStorage::addQuar(std::string quarPath, std::string quarName)
{
	bool isExist = false;
	for (size_t i = 0; i < quarPaths.size(); i++)
	{
		if (quarPaths.at(i) == quarPath)
			isExist = true;
	}
	if (!isExist)
	{
		quarPaths.push_back(quarPath);
		quarNames.push_back(quarName);
		threatPaths.erase(std::find_if(threatPaths.begin(), threatPaths.end(), [&](std::string& el) {return el == quarPath; }));
		threatNames.erase(std::find_if(threatNames.begin(), threatNames.end(), [&](std::string& el) {return el == quarName; }));
	}
}

void InformationStorage::deleteThreat(std::string threatPath, std::string threatName)
{
	threatPaths.erase(std::find_if(threatPaths.begin(), threatPaths.end(), [&](std::string& el) {return el == threatPath; }));
	threatNames.erase(std::find_if(threatNames.begin(), threatNames.end(), [&](std::string& el) {return el == threatName; }));
}

void InformationStorage::deleteFromQuar(std::string quarPath, std::string quarName)
{
	bool isExist = false;
	for (size_t i = 0; i < threatPaths.size(); i++)
	{
		if (threatPaths.at(i) == quarPath)
			isExist = true;
	}
	if (!isExist)
	{
		threatPaths.push_back(quarPath);
		threatNames.push_back(quarName);
		quarPaths.erase(std::find_if(quarPaths.begin(), quarPaths.end(), [&](std::string& el) {return el == quarPath; }));
		quarNames.erase(std::find_if(quarNames.begin(), quarNames.end(), [&](std::string& el) {return el == quarName; }));
	}
}
