#pragma once
#include "IPC.h"
#include "InformationStorage.h"
#include <memory>


class FilesWorker
{
public:
	FilesWorker(std::shared_ptr<InformationStorage> infoStorage);
	void moveToQuarantine(message newMessage);
	void deleteFromQuarantine(message newMessage);
	void deleteThreat(message newMessage);
private:
	std::shared_ptr<InformationStorage> infoStorage;
};

