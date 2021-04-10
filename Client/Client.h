﻿#pragma once

#include <windows.h>
#include "Library.h"
#include <sstream>

HANDLE hPipe = NULL;
//HANDLE mutex = NULL;

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для ClientForm
	/// </summary>
	public ref class ClientForm : public System::Windows::Forms::Form
	{
	public:
		ClientForm(void)
		{
			//mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));

			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ClientForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::OldLace;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(12, 45);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(490, 40);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Отправка сообщения серверу";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &ClientForm::button1_Click);
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(12, 88);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(490, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Ответ сервера:";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox1->Location = System::Drawing::Point(12, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(490, 27);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(12, 108);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox2->Size = System::Drawing::Size(956, 480);
			this->textBox2->TabIndex = 3;
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(980, 600);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Name = L"ClientForm";
			this->Text = L"Лучший в мире антивирус (но это не точно)";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &ClientForm::ClientForm_FormClosed);
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

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
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
			CloseHandle(hPipe);
			textBox2->Text = GetLastError().ToString();
			hPipe = NULL;
			return;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			CloseHandle(hPipe);
			textBox2->Text = GetLastError().ToString();
			hPipe = NULL;
			return;
		}

		if (GetLastError() == ERROR_BROKEN_PIPE)
		{
			CloseHandle(hPipe);
			textBox2->Text = GetLastError().ToString();
			hPipe = NULL;
			return;
		}

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			CloseHandle(hPipe);
			textBox2->Text = "ERROR_FILE_NOT_FOUND";
			hPipe = NULL;
			return;
		}
		
		std::stringstream ss;
		ss << GetLastError();
		//String^ s = gcnew String("");
		//textBox2->Text = gcnew String(ss.str());

		if(textBox2->Text == textBox2->Text->Empty)
			textBox2->Text = "Соединение установлено\r\n";

		std::string text;
		MarshalString(textBox1->Text->ToString(), text);
		textBox1->Clear();
		message newMessage;
		newMessage.cmd = COMMAND::START;
		newMessage.sArr.push_back(text);
		Messenger::sendMessage(hPipe, sizeof(message), newMessage);
		message newMessage2 = Messenger::readMessage(hPipe, sizeof(message));
		std::string s = newMessage2.sArr.at(0);
		String^ textbox2 = gcnew String(s.c_str());
		textBox2->Text += textbox2 + "\r\n";
	}

private: System::Void ClientForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
	CloseHandle(hPipe);
	//CloseHandle(mutex);
}
};
}
