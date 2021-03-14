/*#include "windows.h"
#include <iostream>
#include <sqlite3.h>

using namespace std;

//! ��������� �������
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

// - ��������� �������
PSAVRecordCollection AVRCollection = NULL;

void processPath(PCSTR Path);
void getMD5(const void* pData, size_t nDataSize, PDWORD RetHash);



if (argc 0) {
	// - �������� ���������
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
//��������� ����, �������� ������ ��� ������, ����� ���� ������ � ��� ���������� �� �����.
//���� �� ������ ������, �� ����� ������� ������� processPath, ������� ��������� ����������� �������� �� ���������� ����.
//
//��� ��� �������� ��� ������� :
//
//�������: ������� �������� �����
//----------------------------------------------------------------------------------------------------------


void processPath(PCSTR Path) {
	string SrcPath = Path;
	string File;
	File = Path;
	File += "*.*";

	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA(File.c_str(), &FindData);

	do {
		// - ���������� ����� . � ..
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, "..")) continue;

		File = Path;
		File += "";
		File += FindData.cFileName;

		// - ���� �����, �� ��������� ����������
		if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			processPath(File.c_str());
			// - ����� ��������� �� ������
		}
		else {
			checkFile(File.c_str());
		}

	} while (FindNextFileA(hFind, &FindData));


}
//----------------------------------------------------------------------------------------------------------
//�������� ������ ������ � �����(�� ����������� ����� "." � ".."), ��� ���� ���� ��� �������� �����, �� �������� ����������� ��������, � ���� ����, ��������� ��� �������� checkFile.
//
//���� ������� ������� ������� checkFile
//
//������� : ������� �������� �����
//----------------------------------------------------------------------------------------------------------


void checkFile(PCSTR FileName) {
	cout << FileName << "t";
	// - ��������� ����
	HANDLE hFile = CreateFileA(FileName, FILE_READ_ACCESS, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Error" << endl;
		return;
	}
	// - �������� ������ �����
	DWORD FileSize = GetFileSize(hFile, NULL);

	// - ���������� ���� � ������
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

	// - ����� �� ����������
	bool Detected = false;
	for (DWORD RecID = 0; RecID RecordCount; RecID++) {
		PSAVRecord Record = &AVRCollection->Record[RecID];
		// - ���� ���� ������� ���������, �� ��������� ������
		if (FileSize Signature.Offset + Record->Signature.Lenght)) continue;
		// - ��������� ��������� ����������� ����� ��� ���������
		DWORD Hash[4];
		getMD5((PBYTE)((DWORD)File + Record->Signature.Offset), Record->Signature.Lenght, Hash);

		// - ��������
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