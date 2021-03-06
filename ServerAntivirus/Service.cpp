#include <windows.h>
#include <iostream>
#include "Library.h"

using namespace std;

#define BUFSIZE 512

// Обьявление функций
DWORD WINAPI InstanceThread(LPVOID);

int main()
{
    setlocale(LC_ALL, "ru"); // Подключаем русский язык

    // Флаг, показывающий, есть ли соединение
    BOOL   fConnected = FALSE;

    // Идентификатор потока
    DWORD  dwThreadId = 0;

    // Обьявление канала и потока
    HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;

    // Имя канала
    LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\antivirus");

    // Выводим базовую информацию о приложении:
    cout << "_______________________________________________________________________________________________________________________" << endl;
    cout << "\n                                                      S E R V E R                                                    " << endl;
    cout << "_______________________________________________________________________________________________________________________" << endl << endl;
    
    cout << "Лог сервера:" << endl;

    // Основной цикл создает экземпляр именованного канала, а затем ждет, пока клиент подключится к нему.
    // Когда клиент подключается, создается поток для обработки взаимодействия с этим клиентом,
    // и этот цикл (который основной) может ждать следующего запроса на подключение клиента. Это бесконечный цикл. 
    for (int counter = 0;; counter++)
    {
        if (counter == 0)
            cout << "\nОсновной поток ожидает подключения клиента на канал с именем " << "\\\\.\\pipe\\antivirus" << endl;
        hPipe = CreateNamedPipe(
            lpszPipename,             // имя канала 
            PIPE_ACCESS_DUPLEX,       // чтение/запись доступ 
            PIPE_TYPE_MESSAGE |       // тип сообщения каналу 
            PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
            PIPE_WAIT,                // режим блокирования
            PIPE_UNLIMITED_INSTANCES, // максимальное количество экземпляров 
            BUFSIZE,                  // размер буфера на выход 
            BUFSIZE,                  // размер буфера на вход 
            0,                        // клиент время 
            NULL);                    // атрибут безопасности по умолчанию 

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            cout << "Создание именованного канала не удалось, код ошибки: " << GetLastError() << endl;
            return -1;
        }

        // Подождите, пока клиент подключится; если это удастся,
        // функция возвращает ненулевое значение. Если функция
        // возвращает ноль, GetLastError возвращает ERROR_PIPE_CONNECTED.
        fConnected = ConnectNamedPipe(hPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (fConnected)
        {
            cout << "\nКлиент подключён, создаётся поток для него..." << endl;

            // Создание потока для клиента
            hThread = CreateThread(
                NULL,              // нет атрибута безопасности
                0,                 // размер стека по умолчанию
                InstanceThread,    // процесс потока
                (LPVOID)hPipe,     // параметр потока
                0,                 // не приостановлено
                &dwThreadId);      // возвращает идентификатор потока

            if (hThread == NULL)
            {
                cout << "Создание потока не удалось, код ошибки: " << GetLastError() << endl;
                return -1;
            }
            else CloseHandle(hThread);
        }
        else
            // Клиент не смог подключиться, поэтому закрываем канал.
        {
            counter = 0;
            CloseHandle(hPipe);
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

    // печатаем сообщение
    cout << "Поток создан, получение и обработка сообщений...";

    // Параметр потока - это дескриптор экземпляра объекта канала.
    hPipe = (HANDLE)lpvParam;
    HANDLE mutex;
    while (1) { //GetLastError() != ERROR_BROKEN_PIPE || mutex != NULL
        mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            message newmessage = Messenger::readMessage(hPipe, sizeof(message));
            Messenger::sendMessage(hPipe, sizeof(message), newmessage);
        }
        else
            break;
    }

    // "Чистим" канал, чтобы клиент мог прочитать его содержимое
    // перед отключением. Затем отключаем канал и закрываем
    // дескриптор этого экземпляра канала.

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    HeapFree(hHeap, 0, pchRequest);
    HeapFree(hHeap, 0, pchReply);

    cout << "Экземпляр потока завершил работу" << endl;
    return 1;
}