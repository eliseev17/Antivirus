#include <cliext/vector>
#include "SHA256.h"
#include <string>

#pragma once
namespace DatabaseEditor {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Data::SQLite;
	using namespace System::Text; 
	using namespace System::Threading;
	using namespace System::IO;
	using namespace cliext;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			Control::CheckForIllegalCrossThreadCalls = false;
		}
	private: System::Windows::Forms::Button^ btnRemove;
	private: System::Windows::Forms::OpenFileDialog^ openFD;
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::Button^ btnViewDatabase;
	private: System::Windows::Forms::Button^ btnCreateDatabase;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::TextBox^ txtDatabaseFilePath;
	private: System::Windows::Forms::TextBox^ txtNewDBName;
	private: System::Windows::Forms::TextBox^ txtOffsetEnd;
	private: System::Windows::Forms::Button^ btnAdd;
	private: System::Windows::Forms::TextBox^ txtSignature;
	private: System::Windows::Forms::TextBox^ txtOffsetBegin;
	private: System::Windows::Forms::TextBox^ txtName;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	public:
		SQLiteConnection^ database;
	protected:

		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			database->Close();
		}

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->openFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->btnViewDatabase = (gcnew System::Windows::Forms::Button());
			this->btnRemove = (gcnew System::Windows::Forms::Button());
			this->btnCreateDatabase = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->txtNewDBName = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->txtDatabaseFilePath = (gcnew System::Windows::Forms::TextBox());
			this->txtOffsetEnd = (gcnew System::Windows::Forms::TextBox());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->txtSignature = (gcnew System::Windows::Forms::TextBox());
			this->txtOffsetBegin = (gcnew System::Windows::Forms::TextBox());
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFD
			// 
			this->openFD->FileName = L"AntimalwareDatabase.db";
			this->openFD->Filter = L"Файлы БД (*.db)|*.db";
			this->openFD->InitialDirectory = L"C:\\Users\\elisi\\source\\repos\\Antivirus";
			// 
			// dataGridView1
			// 
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->BackgroundColor = System::Drawing::Color::WhiteSmoke;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(12, 83);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersWidth = 51;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(1193, 400);
			this->dataGridView1->TabIndex = 4;
			// 
			// btnViewDatabase
			// 
			this->btnViewDatabase->BackColor = System::Drawing::Color::Lavender;
			this->btnViewDatabase->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnViewDatabase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnViewDatabase->Location = System::Drawing::Point(6, 25);
			this->btnViewDatabase->Name = L"btnViewDatabase";
			this->btnViewDatabase->Size = System::Drawing::Size(313, 30);
			this->btnViewDatabase->TabIndex = 1;
			this->btnViewDatabase->Text = L"Открыть файл базы";
			this->btnViewDatabase->UseVisualStyleBackColor = false;
			this->btnViewDatabase->Click += gcnew System::EventHandler(this, &MyForm::btnViewDatabase_Click);
			// 
			// btnRemove
			// 
			this->btnRemove->BackColor = System::Drawing::Color::MistyRose;
			this->btnRemove->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnRemove->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnRemove->Location = System::Drawing::Point(935, 513);
			this->btnRemove->Name = L"btnRemove";
			this->btnRemove->Size = System::Drawing::Size(270, 33);
			this->btnRemove->TabIndex = 3;
			this->btnRemove->Text = L"Удалить запись";
			this->btnRemove->UseVisualStyleBackColor = false;
			this->btnRemove->Click += gcnew System::EventHandler(this, &MyForm::btnRemove_Click);
			// 
			// btnCreateDatabase
			// 
			this->btnCreateDatabase->BackColor = System::Drawing::Color::Linen;
			this->btnCreateDatabase->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnCreateDatabase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnCreateDatabase->Location = System::Drawing::Point(6, 25);
			this->btnCreateDatabase->Name = L"btnCreateDatabase";
			this->btnCreateDatabase->Size = System::Drawing::Size(210, 30);
			this->btnCreateDatabase->TabIndex = 11;
			this->btnCreateDatabase->Text = L"Создать базу";
			this->btnCreateDatabase->UseVisualStyleBackColor = false;
			this->btnCreateDatabase->Click += gcnew System::EventHandler(this, &MyForm::btnCreateDatabase_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->btnCreateDatabase);
			this->groupBox1->Controls->Add(this->txtNewDBName);
			this->groupBox1->Location = System::Drawing::Point(773, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(438, 65);
			this->groupBox1->TabIndex = 13;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Создание базы (в папке проекта)";
			// 
			// txtNewDBName
			// 
			this->txtNewDBName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtNewDBName->Location = System::Drawing::Point(222, 25);
			this->txtNewDBName->Name = L"txtNewDBName";
			this->txtNewDBName->Size = System::Drawing::Size(210, 30);
			this->txtNewDBName->TabIndex = 12;
			this->txtNewDBName->Text = L"DefaultFileName";
			this->txtNewDBName->Click += gcnew System::EventHandler(this, &MyForm::txtNewDBName_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->txtDatabaseFilePath);
			this->groupBox2->Controls->Add(this->btnViewDatabase);
			this->groupBox2->Location = System::Drawing::Point(12, 12);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(755, 65);
			this->groupBox2->TabIndex = 14;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Открытие базы";
			// 
			// txtDatabaseFilePath
			// 
			this->txtDatabaseFilePath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->txtDatabaseFilePath->Location = System::Drawing::Point(325, 25);
			this->txtDatabaseFilePath->Name = L"txtDatabaseFilePath";
			this->txtDatabaseFilePath->Size = System::Drawing::Size(424, 30);
			this->txtDatabaseFilePath->TabIndex = 12;
			// 
			// txtOffsetEnd
			// 
			this->txtOffsetEnd->Location = System::Drawing::Point(524, 519);
			this->txtOffsetEnd->Multiline = true;
			this->txtOffsetEnd->Name = L"txtOffsetEnd";
			this->txtOffsetEnd->Size = System::Drawing::Size(112, 24);
			this->txtOffsetEnd->TabIndex = 10;
			// 
			// btnAdd
			// 
			this->btnAdd->BackColor = System::Drawing::Color::Honeydew;
			this->btnAdd->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnAdd->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnAdd->Location = System::Drawing::Point(642, 513);
			this->btnAdd->Name = L"btnAdd";
			this->btnAdd->Size = System::Drawing::Size(287, 33);
			this->btnAdd->TabIndex = 2;
			this->btnAdd->Text = L"Добавить запись";
			this->btnAdd->UseVisualStyleBackColor = false;
			this->btnAdd->Click += gcnew System::EventHandler(this, &MyForm::btnAdd_Click);
			// 
			// txtSignature
			// 
			this->txtSignature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtSignature->Location = System::Drawing::Point(140, 519);
			this->txtSignature->Multiline = true;
			this->txtSignature->Name = L"txtSignature";
			this->txtSignature->Size = System::Drawing::Size(260, 24);
			this->txtSignature->TabIndex = 8;
			// 
			// txtOffsetBegin
			// 
			this->txtOffsetBegin->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtOffsetBegin->Location = System::Drawing::Point(406, 519);
			this->txtOffsetBegin->Multiline = true;
			this->txtOffsetBegin->Name = L"txtOffsetBegin";
			this->txtOffsetBegin->Size = System::Drawing::Size(112, 24);
			this->txtOffsetBegin->TabIndex = 9;
			// 
			// txtName
			// 
			this->txtName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtName->Location = System::Drawing::Point(12, 519);
			this->txtName->Multiline = true;
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(122, 24);
			this->txtName->TabIndex = 7;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(403, 499);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(112, 17);
			this->label1->TabIndex = 11;
			this->label1->Text = L"OFFSET_BEGIN";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(521, 499);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(99, 17);
			this->label2->TabIndex = 12;
			this->label2->Text = L"OFFSET_END";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(137, 499);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(88, 17);
			this->label3->TabIndex = 13;
			this->label3->Text = L"SIGNATURE";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(9, 499);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(124, 17);
			this->label4->TabIndex = 14;
			this->label4->Text = L"MALWARE_NAME";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1217, 558);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->txtName);
			this->Controls->Add(this->btnRemove);
			this->Controls->Add(this->txtOffsetBegin);
			this->Controls->Add(this->txtSignature);
			this->Controls->Add(this->txtOffsetEnd);
			this->Controls->Add(this->btnAdd);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"DatabaseEditor";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		void MarshalString(String^ s, std::string& os) {
			using namespace Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}

		private: DataTable^ fillDataTable() {
			DataTable^ table;
			try
			{
				SQLiteCommand^ cmdSelect = database->CreateCommand();
				//Обратите внимание, что SQL запрос оформляем как обычную строчку
				cmdSelect->CommandText = "SELECT * FROM	ENTRIES;";
				SQLiteDataReader^ reader = cmdSelect->ExecuteReader();

				//Переменные
				DataColumn^ column; //Столбец таблицы
				DataRow^ row; //Строка таблицы

				//Создаем таблицу данных
				table = gcnew DataTable();

				//Вектор названий столбцов
				vector<String^>^ nameColumns = gcnew vector<String^>();

				//Заполним данные о столбцах
				for (int i = 0; i < reader->FieldCount; i++) {
					nameColumns->push_back(reader->GetName(i));
					column = gcnew DataColumn(nameColumns->at(i), String::typeid);
					table->Columns->Add(column);
				}

				//Пробегаем по каждой записи
				while (reader->Read()) {
					//Заполняем строчку таблицы
					row = table->NewRow();
					//В каждой записи пробегаем по всем столбцам
					for (int i = 0; i < reader->FieldCount; i++) {
						//Добавляем значение столбца в row
						row[nameColumns->at(i)] = reader->GetValue(i)->ToString();
						reader->GetValue(i)->ToString();
					}
					table->Rows->Add(row);
				}
			}
			catch (Exception^ e)
			{
				MessageBox::Show("Error Executing SQL: " + e->ToString(), "Exception While Displaying MyTable ...");
			}
			return table;
		}

		   private: void openDatabase() {
				
				try
				{
			   dataGridView1->DataSource = fillDataTable();
				}
				catch (Exception^ e)
				{
					MessageBox::Show("Error Working SQL: " + e->ToString(), "Exception ...");
				}
		   }
		  private: void firstOpenDatabase() {
			  String^ fileName = "./../AntimalwareDatabase.db";
			  database = gcnew SQLiteConnection();
			  try
			  {
				  database->ConnectionString = "Data Source=\"" + fileName + "\"";
				  database->Open();
				  txtDatabaseFilePath->Text = database->FileName;
				  DataTable^ table = fillDataTable();
				  dataGridView1->DataSource = table;
			  }
			  catch (Exception^ e)
			  {
				  MessageBox::Show("Error Working SQL: " + e->ToString(), "Exception ...");
			  }
		  }

	private: System::Void btnViewDatabase_Click(System::Object^ sender, System::EventArgs^ e) {
		if (openFD->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			database = gcnew SQLiteConnection();
			database->ConnectionString = "Data Source=\"" + openFD->FileName + "\"";
			database->Open();
			txtDatabaseFilePath->Text = database->FileName;
			Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::openDatabase));
			thread->IsBackground = true;
			thread->Start();
		}
	}



private: System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
	SHA256 sha256;
	std::string signaturePrefix, signature;
	uint64_t intSignature;

	if (txtName->Text == "" || txtSignature->Text == "" || txtOffsetBegin->Text == "" || txtOffsetEnd->Text == "") {
		MessageBox::Show("Для добавления записи, заполните поля", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	MarshalString(txtSignature->Text, signature);
	MarshalString(txtSignature->Text->Substring(0, 8), signaturePrefix);
	memcpy(&intSignature, signaturePrefix.c_str(), sizeof(uint64_t));
	//std::string ABOBA(sizeof(uint64_t), 0);
	//memcpy(ABOBA.data(), &intSignature, sizeof(uint64_t));
	try
	{
		SQLiteCommand^ cmdInsertValue = database->CreateCommand();
		cmdInsertValue->CommandText = "INSERT INTO ENTRIES (MW_NAME, SIGN_LENGTH, SIGN_PREFIX, SHA256, OFFSET_BEGIN, OFFSET_END)" + 
			" VALUES('" + txtName->Text +  "', " + txtSignature->Text->Trim()->Length + ", " + intSignature + ", '" + gcnew String(sha256.getSHA(signature).c_str()) + "', " + txtOffsetBegin->Text +  ", " + txtOffsetEnd->Text + ");";
		cmdInsertValue->ExecuteNonQuery();

		DataTable^ table = fillDataTable();

		dataGridView1->DataSource = table;
	}
	catch (Exception^ e)
	{
		MessageBox::Show("Error Executing SQL: " + e->ToString(), "Exception ...");
	}
	txtName->Clear();
	txtSignature->Clear();
	txtOffsetBegin->Clear();
	txtOffsetEnd->Clear();
}

private: System::Void btnRemove_Click(System::Object^ sender, System::EventArgs^ e) {
	//Номер выделенной строки
	int index = dataGridView1->CurrentCell->RowIndex;
	//Определим _id в выделенной строке
	String^ MW_NAME = dataGridView1->Rows[index]->Cells["MW_NAME"]->Value->ToString();

	try
	{
		SQLiteCommand^ cmdInsertValue = database->CreateCommand();
		cmdInsertValue->CommandText = "DELETE FROM ENTRIES WHERE MW_NAME = '" + MW_NAME + "';";
		cmdInsertValue->ExecuteNonQuery();

		DataTable^ table = fillDataTable();

		dataGridView1->DataSource = table;
	}
	catch (Exception^ e)
	{
		MessageBox::Show("Error Executing SQL: " + e->ToString(), "Exception ...");
	}
}

private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::firstOpenDatabase));
	thread->IsBackground = true;
	thread->Start();
}
private: System::Void btnCreateDatabase_Click(System::Object^ sender, System::EventArgs^ e) {	
	txtDatabaseFilePath->Clear();
	String^ fileName = "./../" + txtNewDBName->Text + ".db";
	if (File::Exists(fileName))
	{
		MessageBox::Show("Файл " + fileName + " уже Существует", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	database = gcnew SQLiteConnection();
	try
	{
		database->ConnectionString = "Data Source=\"" + fileName + "\"";
		database->Open();
		txtDatabaseFilePath->Text = database->FileName;
		SQLiteCommand^ cmdInsertValue = database->CreateCommand();
		cmdInsertValue->CommandText = "CREATE TABLE \"ENTRIES\" (\"ID\" TEXT NOT NULL DEFAULT 'eliseev',\
			\"MW_NAME\"	TEXT NOT NULL,\
			\"SIGN_LENGTH\"	INTEGER NOT NULL,\
			\"SIGN_PREFIX\"	NUMERIC NOT NULL,\
			\"SHA256\"	TEXT,\
			\"OFFSET_BEGIN\"	INTEGER NOT NULL,\
			\"OFFSET_END\"	INTEGER NOT NULL\
			);";
		cmdInsertValue->ExecuteNonQuery();
		DataTable^ table = fillDataTable();
		dataGridView1->DataSource = table;
	}
	catch (Exception^ e)
	{
		MessageBox::Show("Error Working SQL: " + e->ToString(), "Exception ...");
	}
}
private: System::Void txtNewDBName_Click(System::Object^ sender, System::EventArgs^ e) {
	txtNewDBName->SelectAll();
}
};
}