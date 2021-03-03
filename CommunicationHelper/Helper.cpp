#include "Helper.hpp"
using namespace std;

string Messenger::toByteArray(message& msg)
{
	uint32_t msgSize = 0, offset = 0;
	msgSize += sizeof(msg.cmd) + sizeof(uint32_t) + sizeof(uint32_t) * msg.nArr.size();
	msgSize += sizeof(uint32_t);
	for (size_t i = 0; i < msg.sArr.size(); i++)
	{
		msgSize += sizeof(uint32_t);
		msgSize += msg.sArr.at(i).size() + 1;
	}
	string byteArr;
	byteArr.resize(msgSize);
	memcpy(&byteArr[offset], &msg.cmd, sizeof(msg.cmd));
	offset += sizeof(msg.cmd);
	uint32_t size = msg.nArr.size();
	memcpy(&byteArr[offset], &size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&byteArr[offset], msg.nArr.data(), size * sizeof(uint32_t));
	offset += size * sizeof(uint32_t);
	size = msg.sArr.size();
	memcpy(&byteArr[offset], &size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	for (size_t i = 0; i < size; i++)
	{
		uint32_t elSize = msg.sArr.at(i).size() + 1;
		memcpy(&byteArr[offset], &elSize, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(&byteArr[offset], msg.sArr.at(i).data(), elSize);
		offset += elSize;
	}


	return byteArr;
}

message Messenger::fromByteArray(string str)
{
	message tempMsg;
	uint32_t offset = 0, size = 0;
	memcpy(&tempMsg.cmd, &str[offset], sizeof(tempMsg.cmd));
	offset += sizeof(tempMsg.cmd);
	memcpy(&size, &str[offset], sizeof(tempMsg.cmd));
	offset += sizeof(size);
	tempMsg.nArr.resize(size);

	memcpy(tempMsg.nArr.data(), &str[offset], size * sizeof(uint32_t));
	offset += size * sizeof(uint32_t);

	memcpy(&size, &str[offset], sizeof(tempMsg.cmd));
	offset += sizeof(size);
	tempMsg.sArr.resize(size);
	for (size_t i = 0; i < size; i++)
	{
		uint32_t elSize;
		memcpy(&elSize, &str[offset], sizeof(uint32_t));
		offset += sizeof(uint32_t);
		tempMsg.sArr.at(i).resize(elSize);
		memcpy(tempMsg.sArr.at(i).data(), &str[offset], elSize);
		offset += elSize;
	}
	return tempMsg;
}

void Messenger::sendMsg(HANDLE pipe, size_t size, message& msg)
{
	string str = toByteArray(msg);
	uint32_t strSize = str.size();
	string header;
	header.resize(sizeof(strSize));
	memcpy(header.data(), &strSize, sizeof(strSize));
	str = header + str;
	strSize = str.size();
	uint32_t offset = 0;
	size_t lim = (strSize + strSize % size) / size;
	lim = lim == 0 ? 1 : lim;
	for (size_t i = 0; i < lim - 1; ++i)
	{
		WriteFile(pipe, &str[offset], size, NULL, 0);
		offset += size;
	}
}

message Messenger::readMsg(HANDLE pipe, size_t size)
{
	string msg;
	msg.resize(size);
	ReadFile(pipe, &msg[0], size, NULL, 0);
	uint32_t strSize;
	memcpy(&strSize, msg.data(), sizeof(strSize));
	msg.resize(strSize + sizeof(uint32_t));
	uint32_t offset = size;
	size_t lim = (strSize + strSize % size) / size;
	lim = lim == 0 ? 1 : lim;
	for (size_t i = 0; i < lim - 1; ++i)
	{
		WriteFile(pipe, &msg[offset], size, NULL, 0);
		offset += size;
	}

	return fromByteArray(msg.substr(sizeof(strSize)));
}