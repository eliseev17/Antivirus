#include "IPC.h"
#include <utility>
#include <limits.h>

std::string Messenger::toByteArray(message& msg)
{
	uint32_t msgSize = 0, offset = 0;
	msgSize += sizeof(msg.cmd) + sizeof(uint32_t) + sizeof(uint32_t) * (uint32_t)msg.nArr.size();
	msgSize += sizeof(uint32_t);
	for (size_t i = 0; i < msg.sArr.size(); i++)
	{
		msgSize += sizeof(uint32_t);
		msgSize += (uint32_t)msg.sArr.at(i).size(); // +1;
	}
	std::string byteArr;
	byteArr.resize(msgSize);
	memcpy(&byteArr[offset], &msg.cmd, sizeof(msg.cmd));
	offset += sizeof(msg.cmd);
	uint32_t size = (uint32_t)msg.nArr.size();
	memcpy(&byteArr[offset], &size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&byteArr[offset], msg.nArr.data(), size * sizeof(uint32_t));
	offset += size * sizeof(uint32_t);
	size = (uint32_t)msg.sArr.size();
	memcpy(&byteArr[offset], &size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	for (size_t i = 0; i < size; i++)
	{
		uint32_t elSize = (uint32_t)msg.sArr.at(i).size(); //+ 1;
		memcpy(&byteArr[offset], &elSize, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(&byteArr[offset], msg.sArr.at(i).data(), elSize);
		offset += elSize;
	}


	return byteArr;
}

message Messenger::fromByteArray(std::string str)
{
	message tempMsg;
	if (str == "")
		return tempMsg;
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

void Messenger::sendMessage(HANDLE pipe, size_t size, message& msg)
{
	std::string str = toByteArray(msg);
	uint32_t strSize = (uint32_t)str.size();
	std::string header;
	header.resize(sizeof(strSize));
	memcpy(header.data(), &strSize, sizeof(strSize));
	str = header + str;
	strSize = (uint32_t)str.size();
	uint32_t offset = 0;
	size_t lim = (strSize + strSize % size) / size;
	lim = lim == 0 ? 1 : lim;
	for (size_t i = 0; i < lim; ++i)
	{
		WriteFile(pipe, &str[offset], (DWORD)size, NULL, 0);
		offset += (uint32_t)size;
	}
}

message Messenger::readMessage(HANDLE pipe, size_t size)
{
	DWORD readenBytes = 0;
	std::string msg;
	msg.resize(size);
	bool readRes = ReadFile(pipe, &msg[0], (DWORD)size, &readenBytes, 0);
	if (readenBytes == 0)
	{
		return{};
	}
	uint32_t strSize;
	memcpy(&strSize, msg.data(), sizeof(strSize));
	msg.resize(strSize + sizeof(uint32_t));
	uint32_t offset = (uint32_t)size;
	size_t lim = (strSize + strSize % size) / size;
	lim = lim == 0 ? 1 : lim;
	for (size_t i = 0; i < lim - 1; ++i)
	{
		WriteFile(pipe, &msg[offset], (DWORD)size, NULL, 0);
		offset += (uint32_t)size;
	}
	return fromByteArray(msg.substr(sizeof(strSize)));
}