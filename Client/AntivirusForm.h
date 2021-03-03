#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "../CommunicationHelper/Helper.hpp"

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	/// <summary>
	/// Сводка для AntivirusForm
	/// </summary>
	public ref class AntivirusForm : public System::Windows::Forms::Form
	{
	public:
		AntivirusForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~AntivirusForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(278, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(129, 17);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Ответ от сервера:";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(270, 40);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(572, 264);
			this->textBox2->TabIndex = 6;
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox1->Location = System::Drawing::Point(14, 17);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(236, 38);
			this->textBox1->TabIndex = 5;
			// 
			// button1
			// 
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Location = System::Drawing::Point(14, 61);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(236, 35);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Отправить сообщение серверу";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &AntivirusForm::button1_Click_1);
			// 
			// AntivirusForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(868, 488);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"AntivirusForm";
			this->Text = L"AntivirusForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		HANDLE hPipe = NULL;
		BOOL   fSuccess = FALSE;
		DWORD  cbRead, cbToWrite, cbWritten, dwMode;

		   void MarshalString(String^ s, string& os) {
			   using namespace Runtime::InteropServices;
			   const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			   os = chars;
			   Marshal::FreeHGlobal(IntPtr((void*)chars));
		   }

	private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
		TCHAR pipename[22] = L"\\\\.\\pipe\\antivirus";
		if (hPipe == NULL)
			hPipe = CreateFile(
				pipename,   // pipe name 
				GENERIC_READ |  // read and write access 
				GENERIC_WRITE,
				0,              // no sharing 
				NULL,           // default security attributes
				OPEN_EXISTING,  // opens existing pipe 
				0,              // default attributes 
				NULL);          // no template file 

		  // Break if the pipe handle is valid. 

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			cout << "Не могу открыть канал, код ошибки = " << "INVALID_HANDLE_VALUE\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			cout << "Не могу открыть канал, код ошибки = " << "ERROR_PIPE_BUSY\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}
		textBox2->Text == "Соединение установлено\n";

		string text;
		MarshalString(textBox1->Text->ToString(), text);
		textBox1->Clear();
		//message newMessage;
		//newMessage.cmd = COMMAND::START;
		//newMessage.sArr.push_back(text);
		//Messenger::sendMsg(hPipe, text.length(), newMessage);
		//message newMessage2 = Messenger::readMsg(hPipe, text.length());
		//String^ textbox2 = gcnew String(newMessage2.sArr.data()->c_str());
		//textBox2->Text += textbox2;
	}
};
}