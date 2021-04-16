#pragma once
#include <string>

enum class OBJTYPE
{
	EXE, ZIP
};

class ScanObject {
public:
	std::string filePath;
	OBJTYPE objectType;
};
