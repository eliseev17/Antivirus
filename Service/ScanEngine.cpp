/*#include "windows.h"
#include <iostream>
#include <sqlite3.h>

using namespace std;

//! Коллекция записей
typedef struct SAVRecordCollection {
	SAVRecordCollection(DWORD RecordCount) {
		this->RecordCount = RecordCount;
		this->Record = new SAVRecord[this->RecordCount];
	}
	~SAVRecordCollection() {
		[] this->Record;
	}
	DWORD RecordCount;
	PSAVRecord Record;
} *PSAVRecordCollection;

// - Коллекция записей
PSAVRecordCollection AVRCollection = NULL;

void processPath(PCSTR Path);
void getMD5(const void* pData, size_t nDataSize, PDWORD RetHash);



if (argc 0) {
	// - Создание коллекции
	AVRCollection = new SAVRecordCollection(hAVBFile.getRecordCount());
	for (DWORD RecID = 0; RecID RecordCount; RecID++) {
		if (!hAVBFile.readNextRecord(&AVRCollection->Record[RecID])) {
			cout << "> Error loading record #" << RecID << endl;
		}
	}
	hAVBFile.close();
}
else {
	hAVBFile.close();
	cout << "> Empty AV Base. Stop." << endl;
	return 0;
}
cout << "t" << AVRCollection->RecordCount << " records loaded." << endl;

//
cout << endl;
cout << "Starting scan for viruses" << endl;
cout << endl;

processPath(SrcPath);

//----------------------------------------------------------------------------------------------------------
//Открываем файл, выделяем память под записи, после чего читаем в них информацию из файла.
//Если всё прошло хорошо, то будет вызвана функция processPath, которая выполняет рекурсивную проверку по указанному пути.
//
//Вот так выглядит эта функция :
//
//Листинг: Функция проверки папки
//----------------------------------------------------------------------------------------------------------


void processPath(PCSTR Path) {
	string SrcPath = Path;
	string File;
	File = Path;
	File += "*.*";

	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA(File.c_str(), &FindData);

	do {
		// - Пропускаем папки . и ..
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, "..")) continue;

		File = Path;
		File += "";
		File += FindData.cFileName;

		// - Если папка, то сканируем рекурсивно
		if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			processPath(File.c_str());
			// - Иначе проверяем на вирусы
		}
		else {
			checkFile(File.c_str());
		}

	} while (FindNextFileA(hFind, &FindData));


}
//----------------------------------------------------------------------------------------------------------
//Получаем список файлов и папок(за исключением папок "." и ".."), при этом если нам попалась папка, то проводим рекурсивный просмотр, а если файл, проверяем его функцией checkFile.
//
//Ниже приведён листинг функции checkFile
//
//Листинг : Функция проверки файла
//----------------------------------------------------------------------------------------------------------


void checkFile(PCSTR FileName) {
	cout << FileName << "t";
	// - Открываем файл
	HANDLE hFile = CreateFileA(FileName, FILE_READ_ACCESS, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Error" << endl;
		return;
	}
	// - Получаем размер файла
	DWORD FileSize = GetFileSize(hFile, NULL);

	// - Отображаем файл в память
	HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, NULL, FileSize, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Error" << endl;
		CloseHandle(hFile);
		return;
	}
	LPVOID File = MapViewOfFile(hMap, FILE_MAP_READ, NULL, NULL, FileSize);
	if (File == NULL) {
		cout << "Error" << endl;
		CloseHandle(hMap);
		CloseHandle(hFile);
		return;
	}

	// - Поиск по сигнатурам
	bool Detected = false;
	for (DWORD RecID = 0; RecID RecordCount; RecID++) {
		PSAVRecord Record = &AVRCollection->Record[RecID];
		// - Если файл слишком маленький, то пропускам запись
		if (FileSize Signature.Offset + Record->Signature.Lenght)) continue;
		// - Переходим вычисляем контрольную сумму для сигнатуры
		DWORD Hash[4];
		getMD5((PBYTE)((DWORD)File + Record->Signature.Offset), Record->Signature.Lenght, Hash);

		// - Детектим
		if (!memcmp(Hash, Record->Signature.Hash, 4 * sizeof(DWORD))) {
			cout << " DETECTEDt" << Record->Name << endl;
			Detected = true;
			break;
		}
	}

	UnmapViewOfFile(File);
	CloseHandle(hMap);
	CloseHandle(hFile);

	if (!Detected) cout << "OK" << endl;
}*/