#pragma once
#include "IPC.h"
#include "InformationStorage.h"


class FilesWorker
{
public:
	static void moveToQuarantine(message newMessage, InformationStorage& infoStorage);
	static void deleteFromQuarantine(message newMessage, InformationStorage& infoStorage);
	static void deleteThreat(message newMessage, InformationStorage& infoStorage);
};

