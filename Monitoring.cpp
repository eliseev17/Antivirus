//#include "Monitoring.h"
//#include <Windows.h>
//#include <thread>
//
//extern HANDLE mutex;
//
//Monitoring::Monitoring(const std::string& path)
//{
//	this->path = path;
//}
//
//std::string Monitoring::start(bool& safe, std::string& virusName)
//{
//	shouldStop = false;
//	shouldPause = false;
//
//	changeHandle = FindFirstChangeNotification((wchar_t*)path.c_str(), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
//
//	run(safe, virusName);
//}
//
//void Monitoring::resume()
//{
//	shouldPause = false;
//}
//
//void Monitoring::pause()
//{
//	shouldPause = true;
//}
//
//void Monitoring::stop()
//{
//	shouldStop = true;
//}
//
//void Monitoring::run(bool& safe, std::string& virusName)
//{
//	while (TRUE)
//	{
//		WaitForSingleObject(changeHandle, INFINITE);
//
//		while (shouldPause)
//			Sleep(1);
//
//		if (shouldStop)
//		{
//			CloseHandle(changeHandle);
//			return;
//		}
//
//		Sleep(100);
//
//		scanner.startScan(path, safe, virusName, false);
//
//		CloseHandle(changeHandle);
//		changeHandle = FindFirstChangeNotification((wchar_t*)path.c_str(), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
//	}
//}