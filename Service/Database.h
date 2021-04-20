#include <sqlite3.h>
#include "InformationStorage.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "SHA256.h"

#pragma once
class Database
{
public:
    Database(const char* filePath);
    Database(const char* filePath, std::shared_ptr<InformationStorage> infoStorage);

private:
    char* filePath;
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    SHA256 shaGen;
    void dropTable();
    void createTablesForInfo();
    std::shared_ptr<InformationStorage> infoStorage;

public:
    std::unordered_map<uint64_t, uint64_t> data;
    std::vector<std::string> ID;
    std::vector<std::string> MW_NAME;
    std::vector<uint32_t> SIGN_LENGTH;
    std::vector<uint64_t> SIGN_PREFIX;
    std::vector<std::string> SHA256;
    std::vector<uint32_t> OFFSET_BEGIN;
    std::vector<uint32_t> OFFSET_END;
    void update();
    size_t check(std::string buffer, uint32_t offset);
    uint32_t getMinOFFSET_BEGIN();
    bool findSign(std::string signature);
    bool findSign(uint64_t signature);
    void doSELECTforExample();
    size_t findSignInPartOfBuf(std::string buf, size_t& rowNumber);
    static void getInfo(const char* filePath, std::shared_ptr <InformationStorage> infoStorage);
};

