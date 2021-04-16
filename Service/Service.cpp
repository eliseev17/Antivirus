#include <windows.h>
#include <iostream>
#include "Library.h"
#include "Scanner.h"
//#include "Database.h"
//#include <sqlite3.h>
#include <thread>

using namespace std;

#define BUFSIZE 2048
#define PIPE_BUFSIZE 1024
#define MAIN_THREAD "Main Thread: "

DWORD WINAPI InstanceThread(LPVOID);
void listenPipe(HANDLE hPipe);
size_t i = 0;
HANDLE secondPipe = NULL;

int main()
{
    setlocale(LC_ALL, "ru"); // Подключаем русский язык

    // Флаг, показывающий, есть ли соединение
    BOOL   fConnected = FALSE;

    // Идентификатор потока
    DWORD  dwThreadId = 0;

    // Обьявление канала и потока
    HANDLE firstPipe = INVALID_HANDLE_VALUE, hThread = NULL;

    // Имя канала
    LPCTSTR firstPipeName = TEXT("\\\\.\\pipe\\firstPipe");
    LPCTSTR secondPipeName = TEXT("\\\\.\\pipe\\secondPipe");

    // Выводим базовую информацию о приложении:
    cout << "_______________________________________________________________________________________________________________________" << endl;
    cout << "\n                                                      S E R V E R                                                    " << endl;
    cout << "_______________________________________________________________________________________________________________________" << endl << endl;

    cout << "Лог сервера:" << endl << endl;

    // Основной цикл создает экземпляр именованного канала, а затем ждет, пока клиент подключится к нему.
    // Когда клиент подключается, создается поток для обработки взаимодействия с этим клиентом,
    // и этот цикл (который основной) может ждать следующего запроса на подключение клиента. Это бесконечный цикл. 
    for (;;++i)
    {
        cout << MAIN_THREAD << "Основной поток ожидает подключения клиента на канал с именем " << "\\\\.\\pipe\\firstPipe" << endl;
        firstPipe = CreateNamedPipe(
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

        if (firstPipe == INVALID_HANDLE_VALUE)
        {
            cout << MAIN_THREAD << "Создание именованного канала не удалось, код ошибки: " << GetLastError() << endl;
            return -1;
        }
        // Break if the pipe handle is valid. 
        if (firstPipe == INVALID_HANDLE_VALUE)
        {
            CloseHandle(firstPipe);
            cout << "Thread#" << i << ": " << "INVALID_HANDLE_VALUE" << endl;
            secondPipe = NULL;
            return -1;
        }

        // Exit if an error other than ERROR_PIPE_BUSY occurs.
        if (GetLastError() == ERROR_PIPE_BUSY)
        {
            CloseHandle(firstPipe);
            cout << "Thread#" << i << ": " << "ERROR_PIPE_BUSY" << endl;
            secondPipe = NULL;
            return -1;
        }

        if (GetLastError() == ERROR_BROKEN_PIPE)
        {
            CloseHandle(firstPipe);
            cout << "Thread#" << i << ": " << "ERROR_BROKEN_PIPE" << endl;
            secondPipe = NULL;
            return -1;
        }

        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            CloseHandle(firstPipe);
            cout << "Thread#" << i << ": " << "ERROR_FILE_NOT_FOUND" << endl;
            secondPipe = NULL;
            return -1;
        }
        // Подождите, пока клиент подключится; если это удастся,
        // функция возвращает ненулевое значение. Если функция
        // возвращает ноль, GetLastError возвращает ERROR_PIPE_CONNECTED.
        fConnected = ConnectNamedPipe(firstPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
       
        if (fConnected)
        {
            cout << MAIN_THREAD << "Клиент подключён, создаётся поток для него..." << endl;

            // печатаем сообщение
            cout << MAIN_THREAD << "Поток создан, получение и обработка сообщений..." << endl;
            // Параметр потока - это дескриптор экземпляра объекта канала.

            // Создание потока для клиента
            hThread = CreateThread(
                NULL,              // нет атрибута безопасности
                0,                 // размер стека по умолчанию
                InstanceThread,    // процесс потока
                (LPVOID)firstPipe,     // параметр потока
                0,                 // не приостановлено
                &dwThreadId);      // возвращает идентификатор потока

            if (hThread == NULL)
            {
                cout << MAIN_THREAD << "Создание потока не удалось, код ошибки: " << GetLastError() << endl;
                return -1;
            }
            else CloseHandle(hThread);
        }
        else
            // Клиент не смог подключиться, поэтому закрываем канал.
        {
            cout << MAIN_THREAD << "Создание потока не удалось, код ошибки: " << GetLastError() << endl;
            CloseHandle(firstPipe);
        }
    }
    return 0;
}

DWORD WINAPI InstanceThread(LPVOID lpvParam)
// Эта процедура представляет собой функцию обработки потока для чтения и ответа клиенту
// через соединение открытого канала, переданное из основного цикла. Обратите внимание, что это позволяет
// основному циклу продолжать выполнение, потенциально создавая больше потоков этой процедуры
// для одновременного выполнения, в зависимости от количества входящих клиентских подключений.
{
    HANDLE hHeap = GetProcessHeap();
    TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
    TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));

    //DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
    BOOL fSuccess = FALSE;
    HANDLE hPipe = NULL;

    // Выполняем дополнительную проверку ошибок, поскольку
    // приложение будет продолжать работать, даже если этот
    // поток выходит из строя.

    if (lpvParam == NULL)
    {
        cout << "Thread#" << i << ": "<< ": ";
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    if (pchRequest == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        return (DWORD)-1;
    }

    if (pchReply == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    hPipe = (HANDLE)lpvParam;              // point OSTANOVA
    //HANDLE mutex;

    TCHAR pipename[22] = L"\\\\.\\pipe\\secondPipe";    
    secondPipe = CreateFile(
        pipename,   // pipe name 
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
        cout << "Thread#" << i << ": "<< "INVALID_HANDLE_VALUE" << endl;
        secondPipe = NULL;
        return -1;
    }

    // Exit if an error other than ERROR_PIPE_BUSY occurs.
    if (GetLastError() == ERROR_PIPE_BUSY)
    {
        CloseHandle(secondPipe);
        cout << "Thread#" << i << ": "<< "ERROR_PIPE_BUSY" << endl;
        secondPipe = NULL;
        return -1;
    }

    if (GetLastError() == ERROR_BROKEN_PIPE)
    {
        CloseHandle(secondPipe);
        cout << "Thread#" << i << ": "<< "ERROR_BROKEN_PIPE" << endl;
        secondPipe = NULL;
        return -1;
    }

    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        CloseHandle(secondPipe);
        cout << "Thread#" << i << ": "<< "ERROR_FILE_NOT_FOUND" << endl;
        secondPipe = NULL;
        return -1;
    }

    cout.flush();
    cout << endl;
    cout << "Thread#";
    cout << i << ": Результат работы:" << endl;
    listenPipe(hPipe);

    // "Чистим" канал, чтобы клиент мог прочитать его содержимое
    // перед отключением. Затем отключаем канал и закрываем
    // дескриптор этого экземпляра канала.

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    CloseHandle(secondPipe);

    HeapFree(hHeap, 0, pchRequest);
    HeapFree(hHeap, 0, pchReply);

    cout << "Thread#" << i << ": "<< "Поток завершил работу" << endl << endl;
    return 1;
}

void processingMessage(message newMessage)
{
    Scanner scanner;
    string filePath = "";
    switch (newMessage.cmd)
    {
        case COMMAND::DEL:
            break;
        case COMMAND::START: {
            filePath = newMessage.sArr.at(0);
            scanner.startScan(filePath, secondPipe);
            break;
        }
        case COMMAND::STOP: {
            scanner.stopScan();
            break;
        }
    }
    cout << "Thread#" << i << ": " << "Запрос клиента обработан" << endl << endl;
}

void listenPipe(HANDLE hPipe)
{
    //mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));
    while (GetLastError() != ERROR_BROKEN_PIPE)//GetLastError() != ERROR_BROKEN_PIPE || mutex != NULL  //GetLastError() == ERROR_ALREADY_EXISTS
    { 
        message newMessage;
        newMessage = Messenger::readMessage(hPipe, PIPE_BUFSIZE);
        if (newMessage.cmd == COMMAND::UNKNOWN)
        {
            break;
        }
        thread scanThread(processingMessage, newMessage);
        scanThread.detach();
        //Messenger::sendMessage(hPipe, PIPE_BUFSIZE, newmessage);
        //if (!newmessage.sArr.empty())
        //    cout << "Сервер получил от клиента: " << newmessage.sArr.at(0) << endl;
        //mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));
    }
}