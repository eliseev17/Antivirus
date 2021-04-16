#include "Database.h"

Database::Database(const char* filePath)
{
    this->filePath = (char*)filePath;
    update();
}

int Database::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void Database::update()
{
    this->ID.clear();
    this->MW_NAME.clear();
    this->SIGN_LENGTH.clear();
    this->SIGN_PREFIX.clear();
    this->SHA256.clear();
    this->OFFSET_BEGIN.clear();
    this->OFFSET_END.clear();
    this->data.clear();
    sqlite3_stmt* stmt;
    sqlite3* db = 0; // ����� ������� ���������� � ��
    const char* SQL = "SELECT * FROM ENTRIES;";
    if (sqlite3_open(filePath, &db) == SQLITE_OK)
    {
        if (sqlite3_prepare_v2(db, SQL, -1, &stmt, 0) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW) //���� � �� ���� ������ ���������� ����������
            {
                this->ID.push_back((char*)sqlite3_column_text(stmt, 0));
                this->MW_NAME.push_back((char*)sqlite3_column_text(stmt, 1));
                this->SIGN_LENGTH.push_back(sqlite3_column_int(stmt, 2));
                this->SIGN_PREFIX.push_back(sqlite3_column_int64(stmt, 3));
                this->SHA256.push_back((char*)sqlite3_column_text(stmt, 4));
                this->OFFSET_BEGIN.push_back(sqlite3_column_int(stmt, 5));
                this->OFFSET_END.push_back(sqlite3_column_int(stmt, 6));
                this->data[SIGN_PREFIX.back()] = SIGN_PREFIX.size() - 1;
            }
        }
    }
    sqlite3_close(db);
}

bool Database::findSign(uint64_t intSignature)
{
    update();
    for (size_t i = 0; i < SIGN_PREFIX.size(); i++)
    {
        if (intSignature == SIGN_PREFIX.at(i))
            return true;
    }
    return false;
}

uint32_t Database::getMinOFFSET_BEGIN()
{
    uint32_t minValue = UINT32_MAX;
    for (size_t i = 0; i < OFFSET_BEGIN.size(); i++)
    {
        if (OFFSET_BEGIN.at(i) < minValue)
            minValue = OFFSET_BEGIN.at(i);
    }
    return minValue;
}

bool Database::findSign(std::string signature)
{
    uint64_t intSignature;
    memcpy(&intSignature, signature.data(), sizeof(uint64_t));
    update();
    for (size_t i = 0; i < SIGN_PREFIX.size(); i++)
    {
        if (intSignature == SIGN_PREFIX.at(i))
            return true;
    }
    return false;
}

size_t Database::findSignInPartOfBuf(std::string buf, size_t &rowNumber)
{
    std::string temp (sizeof(uint64_t), 0);
    for (auto &e: data)
    {       
        memcpy(temp.data(), &SIGN_PREFIX.at(e.second), sizeof(uint64_t));
        if (buf.find(temp) != std::string::npos)
        {
            rowNumber = e.second;
            return buf.find(temp);
        }
    }
    return -1;
}

size_t Database::check(std::string buffer, uint32_t offset) // input - FILE BUFFER
{
    size_t rowNumber = 0;
    size_t signPos = findSignInPartOfBuf(buffer, rowNumber);
    if (signPos == -1)
        return 0;
    if (!(buffer.length() >= SIGN_LENGTH.at(rowNumber)))
        return 0;
    if ((offset + signPos) < OFFSET_BEGIN.at(rowNumber) || (offset + signPos) > OFFSET_END.at(rowNumber))
        return 0;
    std::string sign = buffer.substr(signPos, SIGN_LENGTH.at(rowNumber));
    if (shaGen.getSHA(sign) == SHA256.at(rowNumber))
        return rowNumber;
    else
        return 0;
}

void Database::doSELECTforExample()
{
    const char* SQL = "SELECT * FROM ENTRIES;";
    sqlite3* db = 0; // ����� ������� ���������� � ��
    int rc = 0;
    char* err;
    sqlite3_stmt* result;
    // ��������� ����������
    if (sqlite3_open(filePath, &db))
        fprintf(stderr, "������ ��������/�������� ��: %s\n", sqlite3_errmsg(db));
    // ��������� SQL
    std::cout << "� � � � � � � � � �   � � � � � � �   S Q L :\n" << std::endl;
    rc = sqlite3_exec(db, SQL, callback, &result, &err);
    // Compruebo que no hay error
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error: %s.\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
}

//bool Database::findSign(std::string signature)
//{
//    uint64_t intSignature;
//    memcpy(&intSignature, signature.data(), sizeof(uint64_t));
//    sqlite3_stmt* stmt;
//    sqlite3* db = 0;
//    std::string SQL = "SELECT SIGN_PREFIX FROM ENTRIES WHERE SIGN_PREFIX=" + std::to_string(intSignature) + ";";
//    if (sqlite3_open(filePath, &db) == SQLITE_OK)
//    {
//        if (sqlite3_prepare_v2(db, SQL.c_str(), -1, &stmt, 0) == SQLITE_OK)
//        {
//            while (sqlite3_step(stmt) == SQLITE_ROW)
//            {
//                uint64_t sig = sqlite3_column_int64(stmt, 0);
//                if (sig == intSignature)
//                {
//                    sqlite3_close(db);
//                    return true;
//                }
//                else
//                {
//                    sqlite3_close(db);
//                    return false;
//                }
//            }
//        }
//    }
//    sqlite3_close(db);
//    return false;
//}