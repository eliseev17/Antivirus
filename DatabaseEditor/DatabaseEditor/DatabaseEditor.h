#include <cliext/vector>

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
	using namespace cliext;

	/// <summary>
	/// ������ ��� MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//
		}
	private: System::Windows::Forms::Button^ btnAdd;
	private: System::Windows::Forms::TextBox^ txt1;
	private: System::Windows::Forms::TextBox^ txt2;
	private: System::Windows::Forms::TextBox^ txt3;
	private: System::Windows::Forms::TextBox^ txt4;
	private: System::Windows::Forms::TextBox^ txt5;
	private: System::Windows::Forms::TextBox^ txt6;
	private: System::Windows::Forms::TextBox^ txt7;
	private: System::Windows::Forms::TextBox^ txt8;
	private: System::Windows::Forms::Button^ btnRemove;
	private: System::Windows::Forms::OpenFileDialog^ openFD;
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::Button^ btnViewDatabase;

	public:
		SQLiteConnection^ database;
	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			database->Close();
		}

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->btnViewDatabase = (gcnew System::Windows::Forms::Button());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->txt3 = (gcnew System::Windows::Forms::TextBox());
			this->txt5 = (gcnew System::Windows::Forms::TextBox());
			this->txt7 = (gcnew System::Windows::Forms::TextBox());
			this->txt8 = (gcnew System::Windows::Forms::TextBox());
			this->btnRemove = (gcnew System::Windows::Forms::Button());
			this->txt4 = (gcnew System::Windows::Forms::TextBox());
			this->txt6 = (gcnew System::Windows::Forms::TextBox());
			this->txt2 = (gcnew System::Windows::Forms::TextBox());
			this->txt1 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// openFD
			// 
			this->openFD->FileName = L"AntimalwareDatabase.db";
			this->openFD->Filter = L"����� �� (*.db)|*.db";
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
			this->dataGridView1->Location = System::Drawing::Point(12, 66);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersWidth = 51;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(947, 565);
			this->dataGridView1->TabIndex = 4;
			// 
			// btnViewDatabase
			// 
			this->btnViewDatabase->BackColor = System::Drawing::Color::Lavender;
			this->btnViewDatabase->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnViewDatabase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnViewDatabase->Location = System::Drawing::Point(12, 12);
			this->btnViewDatabase->Name = L"btnViewDatabase";
			this->btnViewDatabase->Size = System::Drawing::Size(311, 46);
			this->btnViewDatabase->TabIndex = 1;
			this->btnViewDatabase->Text = L"��������";
			this->btnViewDatabase->UseVisualStyleBackColor = false;
			this->btnViewDatabase->Click += gcnew System::EventHandler(this, &MyForm::btnViewDatabase_Click);
			// 
			// btnAdd
			// 
			this->btnAdd->BackColor = System::Drawing::Color::Honeydew;
			this->btnAdd->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnAdd->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnAdd->Location = System::Drawing::Point(329, 12);
			this->btnAdd->Name = L"btnAdd";
			this->btnAdd->Size = System::Drawing::Size(313, 46);
			this->btnAdd->TabIndex = 2;
			this->btnAdd->Text = L"��������";
			this->btnAdd->UseVisualStyleBackColor = false;
			this->btnAdd->Click += gcnew System::EventHandler(this, &MyForm::btnAdd_Click);
			// 
			// txt3
			// 
			this->txt3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txt3->Location = System::Drawing::Point(237, 42);
			this->txt3->Multiline = true;
			this->txt3->Name = L"txt3";
			this->txt3->Size = System::Drawing::Size(122, 24);
			this->txt3->TabIndex = 7;
			// 
			// txt5
			// 
			this->txt5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txt5->Location = System::Drawing::Point(488, 42);
			this->txt5->MaxLength = 8;
			this->txt5->Multiline = true;
			this->txt5->Name = L"txt5";
			this->txt5->Size = System::Drawing::Size(123, 24);
			this->txt5->TabIndex = 9;
			// 
			// txt7
			// 
			this->txt7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txt7->Location = System::Drawing::Point(695, 42);
			this->txt7->Multiline = true;
			this->txt7->Name = L"txt7";
			this->txt7->Size = System::Drawing::Size(138, 24);
			this->txt7->TabIndex = 11;
			// 
			// txt8
			// 
			this->txt8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txt8->Location = System::Drawing::Point(832, 42);
			this->txt8->Multiline = true;
			this->txt8->Name = L"txt8";
			this->txt8->Size = System::Drawing::Size(126, 24);
			this->txt8->TabIndex = 12;
			// 
			// btnRemove
			// 
			this->btnRemove->BackColor = System::Drawing::Color::MistyRose;
			this->btnRemove->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnRemove->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnRemove->Location = System::Drawing::Point(648, 12);
			this->btnRemove->Name = L"btnRemove";
			this->btnRemove->Size = System::Drawing::Size(311, 46);
			this->btnRemove->TabIndex = 3;
			this->btnRemove->Text = L"�������";
			this->btnRemove->UseVisualStyleBackColor = false;
			this->btnRemove->Click += gcnew System::EventHandler(this, &MyForm::btnRemove_Click);
			// 
			// txt4
			// 
			this->txt4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txt4->Location = System::Drawing::Point(358, 42);
			this->txt4->MaxLength = 8;
			this->txt4->Multiline = true;
			this->txt4->Name = L"txt4";
			this->txt4->Size = System::Drawing::Size(131, 24);
			this->txt4->TabIndex = 8;
			// 
			// txt6
			// 
			this->txt6->Location = System::Drawing::Point(610, 42);
			this->txt6->Multiline = true;
			this->txt6->Name = L"txt6";
			this->txt6->Size = System::Drawing::Size(86, 24);
			this->txt6->TabIndex = 10;
			// 
			// txt2
			// 
			this->txt2->Location = System::Drawing::Point(115, 42);
			this->txt2->Multiline = true;
			this->txt2->Name = L"txt2";
			this->txt2->ReadOnly = true;
			this->txt2->Size = System::Drawing::Size(123, 24);
			this->txt2->TabIndex = 6;
			// 
			// txt1
			// 
			this->txt1->Location = System::Drawing::Point(63, 42);
			this->txt1->Multiline = true;
			this->txt1->Name = L"txt1";
			this->txt1->ReadOnly = true;
			this->txt1->Size = System::Drawing::Size(53, 24);
			this->txt1->TabIndex = 5;
			this->txt1->Text = L"eliseev";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(971, 643);
			this->Controls->Add(this->txt1);
			this->Controls->Add(this->txt2);
			this->Controls->Add(this->txt6);
			this->Controls->Add(this->txt4);
			this->Controls->Add(this->txt8);
			this->Controls->Add(this->txt7);
			this->Controls->Add(this->txt5);
			this->Controls->Add(this->txt3);
			this->Controls->Add(this->btnAdd);
			this->Controls->Add(this->btnViewDatabase);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->btnRemove);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MyForm";
			this->Text = L"DatabaseEditor";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		private: DataTable^ fillDataTable() {
			DataTable^ table;
			try
			{
				SQLiteCommand^ cmdSelect = database->CreateCommand();
				//�������� ��������, ��� SQL ������ ��������� ��� ������� �������
				cmdSelect->CommandText = "SELECT * FROM	ENTRIES;";
				SQLiteDataReader^ reader = cmdSelect->ExecuteReader();

				//����������
				DataColumn^ column; //������� �������
				DataRow^ row; //������ �������

				//������� ������� ������
				table = gcnew DataTable();

				//������ �������� ��������
				vector<String^>^ nameColumns = gcnew vector<String^>();

				//�������� ������ � ��������
				for (int i = 0; i < reader->FieldCount; i++) {
					nameColumns->push_back(reader->GetName(i));
					column = gcnew DataColumn(nameColumns->at(i), String::typeid);
					table->Columns->Add(column);
				}

				//��������� �� ������ ������
				while (reader->Read()) {
					//��������� ������� �������
					row = table->NewRow();
					//� ������ ������ ��������� �� ���� ��������
					for (int i = 0; i < reader->FieldCount; i++) {
						//��������� �������� ������� � row
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

	private: System::Void btnViewDatabase_Click(System::Object^ sender, System::EventArgs^ e) {
		if (openFD->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			String^ fileName = openFD->FileName;
			database = gcnew SQLiteConnection();
			try
			{
				database->ConnectionString = "Data Source=\"" + fileName + "\"";
				database->Open();

				DataTable^ table = fillDataTable();

				dataGridView1->DataSource = table;
			}
			catch (Exception^ e)
			{
				MessageBox::Show("Error Working SQL: " + e->ToString(), "Exception ...");
			}
		}
	}

private: System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
	if (txt3->Text == "" || txt4->Text == "" || txt5->Text == "" || txt6->Text == "" || txt7->Text == "" || txt8->Text == "") {
		MessageBox::Show("��� ���������� ������, ��������� ����", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	try
	{
		SQLiteCommand^ cmdInsertValue = database->CreateCommand();
		cmdInsertValue->CommandText = "INSERT INTO ENTRIES (MW_LENGTH, MW_NAME, SIGN_LENGTH, SIGN_PREFIX, SHA256, OFFSET_BEGIN, OFFSET_END)" + 
			" VALUES(" + txt3->Text->Length + ", '" + txt3->Text +  "', " + txt4->Text + ", " + txt5->Text + ", '" + txt6->Text + "', " + txt7->Text +  ", " + txt8->Text + ");";
		cmdInsertValue->ExecuteNonQuery();

		DataTable^ table = fillDataTable();

		dataGridView1->DataSource = table;
		ChangeTxtLocation(24);
	}
	catch (Exception^ e)
	{
		MessageBox::Show("Error Executing SQL: " + e->ToString(), "Exception ...");
	}
	txt3->Clear();
	txt4->Clear();
	txt5->Clear();
	txt6->Clear();
	txt7->Clear();
	txt8->Clear();
}

private: System::Void btnRemove_Click(System::Object^ sender, System::EventArgs^ e) {
	//����� ���������� ������
	int index = dataGridView1->CurrentCell->RowIndex;
	//��������� _id � ���������� ������
	String^ MW_NAME = dataGridView1->Rows[index]->Cells["MW_NAME"]->Value->ToString();

	try
	{
		SQLiteCommand^ cmdInsertValue = database->CreateCommand();
		cmdInsertValue->CommandText = "DELETE FROM ENTRIES WHERE MW_NAME = '" + MW_NAME + "';";
		cmdInsertValue->ExecuteNonQuery();

		DataTable^ table = fillDataTable();

		dataGridView1->DataSource = table;
		ChangeTxtLocation(-24);
	}
	catch (Exception^ e)
	{
		MessageBox::Show("Error Executing SQL: " + e->ToString(), "Exception ...");
	}
}

private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	//if (openFD->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		String^ fileName = "C:\\Users\\elisi\\source\\repos\\Antivirus\\AntimalwareDatabase.db";
		database = gcnew SQLiteConnection();
		try
		{
			database->ConnectionString = "Data Source=\"" + fileName + "\"";
			database->Open();

			DataTable^ table = fillDataTable();

			dataGridView1->DataSource = table;
			ChangeTxtLocation((dataGridView1->RowCount + 1) * 24);
		}
		catch (Exception^ e)
		{
			MessageBox::Show("Error Working SQL: " + e->ToString(), "Exception ...");
		}
	//}
}

private: void ChangeTxtLocation(int offset) {
	txt1->Location = System::Drawing::Point(63, txt1->Location.Y + offset);
	txt2->Location = System::Drawing::Point(115, txt2->Location.Y + offset);
	txt3->Location = System::Drawing::Point(237, txt3->Location.Y + offset);
	txt4->Location = System::Drawing::Point(358, txt4->Location.Y + offset);
	txt5->Location = System::Drawing::Point(488, txt5->Location.Y + offset);
	txt6->Location = System::Drawing::Point(610, txt6->Location.Y + offset);
	txt7->Location = System::Drawing::Point(695, txt7->Location.Y + offset);
	txt8->Location = System::Drawing::Point(832, txt8->Location.Y + offset);
}
};
}