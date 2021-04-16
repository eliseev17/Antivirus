#include <filesystem>
#include <fstream>
#include "Scanner.h"
#include <sqlite3.h>
#include <iostream>
#include <algorithm>
#include <zip.h>

#define MZHEADER 0x5a4d
#define ZIPHEADER 0x04034b50
#define BUFSIZE 8128
#define PIPE_BUFSIZE 1024

bool Scanner::shouldStop = false;

message Scanner::scanDirectory(const std::string& path, HANDLE pipe)
{
	message result;
	result.cmd = COMMAND::START;
	namespace fs = std::filesystem;
	uint64_t numberOfFiles = std::count_if(fs::recursive_directory_iterator(path.data()),
		fs::recursive_directory_iterator{}, [](auto p) {return fs::is_regular_file(p); });
	result.nArr.emplace_back(numberOfFiles);
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.nArr.clear();
	Database db = Database("./../AntimalwareDatabase.db");
	size_t virusCounter = 0;
	size_t filesCounter = 0;
	for (auto& p : fs::recursive_directory_iterator(path.data()))
	{
		if (shouldStop)
		{
			result.cmd = COMMAND::STOP;
			Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
			break;
		}

		if (std::filesystem::is_directory(p.path()))
			continue;
		filesCounter++;
		std::string path = p.path().string();
		std::ifstream fileStream(path.c_str(), std::ios::binary);
		uint16_t mzHeader = 0;
		uint32_t zipHeader = 0;
		fileStream.read((char*)&mzHeader, sizeof(uint16_t));
		fileStream.seekg(0);
		fileStream.read((char*)&zipHeader, sizeof(uint32_t));
		fileStream.close();
		if (!result.sArr.empty())
			result.sArr.at(0) = path;
		else
			result.sArr.emplace_back(path);		
		if (mzHeader == MZHEADER)
		{		
			size_t res = scanExe(path, db);
			if (res) {
				virusCounter++;
				result.sArr.emplace_back("Virus founded: " + db.MW_NAME.at(res));
			}				
			else
				result.sArr.emplace_back("Is safely");
		}
		else if (zipHeader == ZIPHEADER)
		{
			message scanZipResult = scanZip(path, pipe);
			filesCounter += scanZipResult.nArr.at(0);
			virusCounter += scanZipResult.nArr.at(1);
			result.nArr.clear();
			continue;
		}
		else
		{
			result.sArr.emplace_back("Not PE");		
		}	
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
		std::cout << p.path() << '\n';
		result.sArr.clear();
	}
	result.cmd = COMMAND::SCAN_RESULT;
	result.sArr.clear();
	result.nArr.emplace_back(filesCounter);
	result.nArr.emplace_back(virusCounter);
	return result;
}

size_t Scanner::scanExe(std::string path, Database db)
{
	uint64_t fSize = 0;
	std::ifstream stream(path, std::ios::binary);
	fSize = std::filesystem::file_size(path);
	size_t resultPosition = 0;
	uint32_t offset = db.getMinOFFSET_BEGIN();
	stream.seekg(offset);
	while (!stream.eof() && offset < fSize)
	{
		std::string buf(BUFSIZE, 0);
		stream.read(buf.data(), BUFSIZE);
		resultPosition = db.check(buf, offset);
		if (resultPosition)
			break;
		offset += BUFSIZE;
	}
	return resultPosition;
}

void Scanner::startScan(const std::string& path, HANDLE pipe)
{
	shouldStop = false;
	std::filesystem::remove_all("D:\\unpackedFiles");
	if (std::filesystem::is_directory(path))
	{
		message scanResult = scanDirectory(path, pipe);
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, scanResult);
	}
	else
	{
		scanFile(path, pipe);
	}
}

void Scanner::stopScan()
{
	shouldStop = true;
}

void Scanner::scanFile(const std::string& path, HANDLE pipe)
{
	message result;
	if (shouldStop)
	{
		result.cmd = COMMAND::STOP;
		Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
		return;
	}
	size_t virusCounter = 0;
	result.cmd = COMMAND::START;
	uint64_t numberOfFiles = 1;	
	std::ifstream fileStream(path.c_str(), std::ios::binary);

	uint16_t mzHeader = 0;
	uint32_t zipHeader = 0;

	fileStream.read((char*)&mzHeader, sizeof(uint16_t));
	fileStream.seekg(0);
	fileStream.read((char*)&zipHeader, sizeof(uint32_t));
	fileStream.close();

	result.nArr.emplace_back(numberOfFiles);
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.nArr.clear();
	Database db = Database("./../AntimalwareDatabase.db");
	if (!result.sArr.empty())
		result.sArr.at(0) = path;
	else
		result.sArr.emplace_back(path);
	fileStream.close();
	if (mzHeader == MZHEADER)
	{
		size_t res = scanExe(path, db);
		if (res) {
			virusCounter++;
			result.sArr.emplace_back("Virus founded: " + db.MW_NAME.at(res));
		}
		else
			result.sArr.emplace_back("Is safely");
	}
	else if (zipHeader == ZIPHEADER)
	{
		scanZip(path, pipe);
	}
	else
	{
		result.sArr.emplace_back("Not PE");
	}
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	result.cmd = COMMAND::SCAN_RESULT;
	result.nArr.clear();
	result.sArr.clear();
	result.nArr.emplace_back(numberOfFiles);
	result.nArr.emplace_back(virusCounter);
	Messenger::sendMessage(pipe, PIPE_BUFSIZE, result);
	std::cout << path << '\n';	
}

bool Scanner::unzip(const std::string& zipPath)
{
	std::filesystem::create_directory("D:\\unpackedFiles");
	int err;
	struct zip* hZip = zip_open(zipPath.c_str(), 0, &err);
	if (hZip)
	{
		size_t totalIndex = zip_get_num_entries(hZip, 0);
		for (size_t i = 0; i < totalIndex; i++)
		{
			struct zip_stat st;
			zip_stat_init(&st);
			zip_stat_index(hZip, i, 0, &st);

			struct zip_file* zf = zip_fopen_index(hZip, i, 0);
			if (!zf)
			{
				zip_close(hZip);
				return false;
			}

			std::vector<char> buffer;
			buffer.resize(st.size);
			zip_fread(zf, buffer.data(), st.size);
			zip_fclose(zf);
			std::string destinationPath = "D:\\unpackedFiles\\";
			destinationPath += st.name;
			std::ofstream outfile(destinationPath);
			outfile.write(buffer.data(), st.size);
			outfile.close();
		}
		zip_close(hZip);
	}
	return true;
}

message Scanner::scanZip(std::string path, HANDLE pipe)
{
	unzip(path);
	return scanDirectory("D:\\unpackedFiles", pipe);
}