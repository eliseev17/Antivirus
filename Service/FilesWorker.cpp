#include "FilesWorker.h"
#include "PipeNamesBuffsizesPaths.h"
#include <filesystem>
#include <fstream>

FilesWorker::FilesWorker(std::shared_ptr<InformationStorage> infoStorage)
{
    this->infoStorage = infoStorage;
}

void FilesWorker::moveToQuarantine(message newMessage)
{
    std::string filePath = newMessage.sArr.at(0);
    std::string quarName = newMessage.sArr.at(1);
    infoStorage->addQuar(filePath, quarName);
    std::filesystem::path filename = filePath;
    std::string destPath = QUARANTINE_PATH + filename.filename().string();
    std::filesystem::copy_file(filePath, destPath);
    std::filesystem::remove(filePath);
    std::ifstream fs(destPath, std::ios::binary);
    uint64_t header;
    fs.read((char*)&header, sizeof(uint64_t));
    fs.close();
    header = ~header;
    std::fstream fi(destPath, std::ios::in | std::ios::out | std::ios::binary);
    fi.seekp(0);
    fi.write((char*)&header, sizeof(uint64_t));
    fi.close();
}

void FilesWorker::deleteFromQuarantine(message newMessage)
{
    std::string filePath = newMessage.sArr.at(0);
    std::string quarName = newMessage.sArr.at(1);
    infoStorage->deleteFromQuar(filePath, quarName);
    std::filesystem::path filename = filePath;
    std::string destPath = QUARANTINE_PATH + filename.filename().string();
    std::filesystem::copy_file(destPath, filePath);
    std::filesystem::remove(destPath);
    std::ifstream fs(filePath, std::ios::binary);
    uint64_t header;
    fs.read((char*)&header, sizeof(uint64_t));
    fs.close();
    header = ~header;
    std::fstream fi(filePath, std::ios::in | std::ios::out | std::ios::binary);
    fi.seekp(0);
    fi.write((char*)&header, sizeof(uint64_t));
    fi.close();
}

void FilesWorker::deleteThreat(message newMessage)
{
    std::string filePath = newMessage.sArr.at(0);
    std::string threatName = newMessage.sArr.at(1);
    for (size_t i = 0; i < infoStorage->threatPaths.size(); i++)
    {
        if (infoStorage->threatPaths.at(i) == filePath)
        {
            infoStorage->deleteThreat(filePath, threatName);
            std::filesystem::remove(filePath);
            return;
        }
    }
}