#include <msclr/marshal_cppstd.h>
#include <cliext/vector>
#include "SHA256.h"

#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Data::SQLite;
using namespace cliext;

ref class DatabaseEditorLogic
{
	public: static void MarshalString(String^ s, std::string& os);

	public: static DataTable^ fillDataTable(SQLiteConnection^ database);

	public: static void addRecord(SQLiteConnection^ database, String^ malwareName, String^ inputSignature, String^ offsetBegin, String^ offsetEnd);

	public: static void deleteRecord(SQLiteConnection^ database, String^ malwareName);

	public: static void createDatabase(SQLiteConnection^ database, String^ fileName);

	public: static void openDatabase(SQLiteConnection^ database);

	public: static void openDatabase(SQLiteConnection^ database, String^ fileName);
};

