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
	/// <summary>
	/// ������ ��� AntivirusForm
	/// </summary>
	public ref class AntivirusForm : public System::Windows::Forms::Form
	{
	public:
		AntivirusForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~AntivirusForm()
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

	protected:




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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(38, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(380, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(38, 52);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(383, 51);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 22);
			this->textBox2->TabIndex = 3;
			// 
			// AntivirusForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(868, 488);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
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

		   void MarshalString(String^ s, std::string& os) {
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
			//std::cout << "�� ���� ������� �����, ��� ������ = " << "INVALID_HANDLE_VALUE\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			//std::cout << "�� ���� ������� �����, ��� ������ = " << "ERROR_PIPE_BUSY\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}
		textBox2->Text == "���������� �����������\n";

		std::string text;
		MarshalString(textBox1->Text->ToString(), text);
		textBox1->Clear();
		message newMessage;
		newMessage.cmd = COMMAND::START;
		newMessage.sArr.push_back(text);
		Messenger::sendMsg(hPipe, text.length(), newMessage);
		message newMessage2 = Messenger::readMsg(hPipe, text.length());
		String^ textbox2 = gcnew String(newMessage2.sArr.data()->c_str());
		textBox2->Text += textbox2;
	}
};
}