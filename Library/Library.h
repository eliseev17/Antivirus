#pragma once
#include <vector>
#include <Windows.h>
#include <string>
#include <cstdint>

enum class COMMAND : uint32_t { START, STOP, MOVE_TO_QUARANTINE, DEL,
	DELETE_FROM_QUARANTINE, SET_MONITORING, STOP_MONITORING,
	INT_MESSAGE, STRING_MESSAGE, SCAN_RESULT, UNKNOWN

};

struct message
{
	COMMAND cmd = COMMAND::UNKNOWN;
	std::vector<uint32_t> nArr;
	std::vector<std::string> sArr;
};

class Messenger
{
private:
	static std::string toByteArray (message& msg);
	static message fromByteArray (std::string str);
public:
	static void sendMessage(HANDLE pipe, size_t size, message& msg);
	static message readMessage(HANDLE pipe, size_t size);
};