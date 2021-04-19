#include <windows.h>
#include "IPC.h"
#include "Scanner.h"
#include "ScheduledScanner.h"
#include "InformationStorage.h"
#include "Monitoring.h"
#include <thread>
#include "FilesWorker.h"

using namespace std;

#define BUFSIZE 2048
#define PIPE_BUFSIZE 1024

int clientMessagesProcessing(HANDLE firstPipe, HANDLE secondPipe, InformationStorage& infoStorage);
void processingMessage(message newMessage, HANDLE secondPipe, InformationStorage& infoStorage);
void listenPipe(HANDLE firstPipe, HANDLE secondPipe, InformationStorage& infoStorage);

int main()
{
    // Флаг, показывающий, есть ли соединение
    BOOL   fConnected = FALSE;

    // Обьявление канала и потока
    HANDLE pipeClientToService = INVALID_HANDLE_VALUE, pipeServiceToClient = NULL;

    // Имя канала
    LPCTSTR firstPipeName = TEXT("\\\\.\\pipe\\firstPipe");

    InformationStorage infoStorage = Database::getInfo("./../AntimalwareDatabase.db");

    // Основной цикл создает экземпляр именованного канала, а затем ждет, пока клиент подключится к нему.
    // Когда клиент подключается, создается поток для обработки взаимодействия с этим клиентом,
    // и этот цикл (который основной) может ждать следующего запроса на подключение клиента. Это бесконечный цикл. 
    while (true)
    {
        cout << "Main thread waiting for connect on pipe " << "\\\\.\\pipe\\firstPipe" << endl;
        pipeClientToService = CreateNamedPipe(
            firstPipeName,             // имя канала 
            PIPE_ACCESS_DUPLEX,       // чтение/запись доступ 
            PIPE_TYPE_MESSAGE |       // тип сообщения каналу 
            PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
            PIPE_WAIT,                // режим блокирования
            PIPE_UNLIMITED_INSTANCES, // максимальное количество экземпляров 
            PIPE_BUFSIZE,                  // размер буфера на выход 
            PIPE_BUFSIZE,                  // размер буфера на вход 
            0,                        // клиент время 
            NULL);                    // атрибут безопасности по умолчанию 

        if (pipeClientToService == INVALID_HANDLE_VALUE)
        {
            return -1; //Создание именованного канала не удалось, код ошибки
        }
        // Break if the pipe handle is valid. 
        if (pipeClientToService == INVALID_HANDLE_VALUE)
        {
            CloseHandle(pipeClientToService);
            pipeServiceToClient = NULL;
            return -1;
        }

        // Exit if an error other than ERROR_PIPE_BUSY occurs.
        if (GetLastError() == ERROR_PIPE_BUSY)
        {
            CloseHandle(pipeClientToService);
            pipeServiceToClient = NULL;
            return -1;
        }

        if (GetLastError() == ERROR_BROKEN_PIPE)
        {
            CloseHandle(pipeClientToService);
            pipeServiceToClient = NULL;
            return -1;
        }

        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            CloseHandle(pipeClientToService);
            pipeServiceToClient = NULL;
            return -1;
        }
        // Подождите, пока клиент подключится; если это удастся,
        // функция возвращает ненулевое значение. Если функция
        // возвращает ноль, GetLastError возвращает ERROR_PIPE_CONNECTED.
        fConnected = ConnectNamedPipe(pipeClientToService, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
       
        if (fConnected)
        {
            // Клиент подключён, создаётся поток для него...
            // Создание потока для клиента  
            cout << "Client connected" << endl;
            thread clientThread (clientMessagesProcessing, pipeClientToService, pipeServiceToClient, std::ref(infoStorage));
            clientThread.detach();
            // Поток создан, получение и обработка сообщений...
        }
        else  // Клиент не смог подключиться, поэтому закрываем канал.
        {
            CloseHandle(pipeClientToService); //Создание потока не удалось, код ошибки
        }
    }
    return 0;
}

int clientMessagesProcessing(HANDLE firstPipe, HANDLE secondPipe, InformationStorage &infoStorage)
{
    // Выполняем дополнительную проверку ошибок, поскольку
    // приложение будет продолжать работать, даже если этот
    // поток выходит из строя.
    if (firstPipe == NULL)
    {
        return -1; //InstanceThread got an unexpected NULL value in lpvParam
    }

    LPCTSTR secondPipeName = TEXT("\\\\.\\pipe\\secondPipe");
    secondPipe = CreateFile(
        secondPipeName,   // pipe name 
        GENERIC_READ |  // read and write access 
        GENERIC_WRITE,
        0,              // no sharing 
        NULL,           // default security attributes
        OPEN_EXISTING,  // opens existing pipe 
        0,              // default attributes 
        NULL);          // no template file 

        // Break if the pipe handle is valid. 
    if (secondPipe == INVALID_HANDLE_VALUE)
    {
        CloseHandle(secondPipe);
        secondPipe = NULL;
        return -1;
    }

    // Exit if an error other than ERROR_PIPE_BUSY occurs.
    if (GetLastError() == ERROR_PIPE_BUSY)
    {
        CloseHandle(secondPipe);
        secondPipe = NULL;
        return -1;
    }

    if (GetLastError() == ERROR_BROKEN_PIPE)
    {
        CloseHandle(secondPipe);
        secondPipe = NULL;
        return -1;
    }

    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        CloseHandle(secondPipe);
        secondPipe = NULL;
        return -1;
    }

    // Thread: Результат работы
    message threatsInfo;
    threatsInfo.cmd = COMMAND::INFO;
    threatsInfo.nArr.push_back(0);
    for (size_t i = 0; i < infoStorage.threatPaths.size(); i++)
    {
        threatsInfo.sArr.push_back(infoStorage.threatPaths.at(i));
        threatsInfo.sArr.push_back(infoStorage.threatNames.at(i));
    }
    Messenger::sendMessage(secondPipe, PIPE_BUFSIZE, threatsInfo);
    threatsInfo.nArr.clear();
    threatsInfo.sArr.clear();

    threatsInfo.nArr.push_back(1);
    for (size_t i = 0; i < infoStorage.quarPaths.size(); i++)
    {
        threatsInfo.sArr.push_back(infoStorage.quarPaths.at(i));
        threatsInfo.sArr.push_back(infoStorage.quarNames.at(i));
    }
    Messenger::sendMessage(secondPipe, PIPE_BUFSIZE, threatsInfo);
    threatsInfo.nArr.clear();
    threatsInfo.sArr.clear();

    
    listenPipe(firstPipe, secondPipe, infoStorage);

    // "Чистим" канал, чтобы клиент мог прочитать его содержимое
    // перед отключением. Затем отключаем канал и закрываем
    // дескриптор этого экземпляра канала.
    FlushFileBuffers(firstPipe);
    DisconnectNamedPipe(firstPipe);
    CloseHandle(firstPipe);
    CloseHandle(secondPipe);

    Database outputInfo = Database("./../AntimalwareDatabase.db", infoStorage);
    cout << "Thread поток завершил работу" << endl << endl;
    return 0;
}

void processingMessage(message newMessage, HANDLE secondPipe, InformationStorage& infoStorage)
{
    Scanner scanner;
    Monitoring monitoring;
    ScheduledScanner scheduledScanner;
    switch (newMessage.cmd)
    {
        case COMMAND::DEL: {            
            FilesWorker::deleteThreat(newMessage, infoStorage);
            break;
        }           
        case COMMAND::START: {
            std::string filePath = newMessage.sArr.at(0);
            scanner.startScan(filePath, secondPipe, infoStorage);
            break;
        }
        case COMMAND::STOP: {
            scanner.stopScan();
            break;
        }
        case COMMAND::MOVE_TO_QUARANTINE: {
            FilesWorker::moveToQuarantine(newMessage, infoStorage);
            break;
        }
        case COMMAND::DELETE_FROM_QUARANTINE: {
            FilesWorker::deleteFromQuarantine(newMessage, infoStorage);
            break;
        }
        case COMMAND::SET_SCHEDULE: {
            scheduledScanner.startScheduledScan(secondPipe, newMessage, infoStorage);
            break;
        }
        case COMMAND::RESET_SCHEDULE: {
            scheduledScanner.cancelScheduledScan();
            break;
        }
        case COMMAND::SET_MONITORING: {
            std::string monitoringDirectory = newMessage.sArr.at(0);
            monitoring.start(monitoringDirectory, secondPipe, infoStorage);
            break;
        }
        case COMMAND::STOP_MONITORING: {
            monitoring.stop();
            break;
        }
    }
    cout << "Thread: Запрос клиента обработан" << endl << endl;
}

void listenPipe(HANDLE firstPipe, HANDLE secondPipe, InformationStorage &infoStorage)
{
    while (GetLastError() != ERROR_BROKEN_PIPE)
    { 
        message newMessage = Messenger::readMessage(firstPipe, PIPE_BUFSIZE);
        if (newMessage.cmd == COMMAND::UNKNOWN)
        {
            break;
        } 
        thread scanThread(processingMessage, newMessage, secondPipe, std::ref(infoStorage));
        scanThread.detach();
    }
}
