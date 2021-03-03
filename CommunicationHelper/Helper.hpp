#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>
#include <cstdint>

using namespace std;

enum class COMMAND : uint32_t { START, STOP, MOVE, DEL };

struct message
{
	COMMAND cmd;
	vector<uint32_t> nArr;
	vector<string> sArr;
};

class Messenger
{
private:
	static string toByteArray(message& msg);
	static message fromByteArray(string str);
public:
	static void sendMsg(HANDLE pipe, size_t size, message& msg);
	static message readMsg(HANDLE pipe, size_t size);
};