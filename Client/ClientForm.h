#pragma once
#include <windows.h>
//#include <string>
#include "../Helper.hpp"

HANDLE hPipe = NULL;

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
			HANDLE mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));
			// если он уже существует, CreateMutex вернет дескриптор существующего
			// объекта, а GetLastError вернет ERROR_ALREADY_EXISTS
			// в течение 2 секунд пытаемся захватить объект
			//if (mutex != NULL)
			//	res = WaitForSingleObject(mutex, 2000);

			//if (res == WAIT_TIMEOUT) // если захват не удался
			//	return 0;            // закрываем приложение
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
			this->button1->Location = System::Drawing::Point(35, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ClientForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(316, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(54, 72);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(319, 61);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(261, 232);
			this->textBox2->TabIndex = 3;
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(635, 362);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Name = L"ClientForm";
			this->Text = L"ClientForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
		//BOOL   fSuccess = FALSE;
		//DWORD  cbRead, cbToWrite, cbWritten, dwMode;

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
			//std::cout << "Не могу открыть канал, код ошибки = " << "INVALID_HANDLE_VALUE\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			//std::cout << "Не могу открыть канал, код ошибки = " << "ERROR_PIPE_BUSY\n" << endl;
			CloseHandle(hPipe);
			hPipe = NULL;
		}
		if(textBox2->Text == textBox2->Text->Empty)
			textBox2->Text = "Соединение установлено\r\n";

		std::string text;
		MarshalString(textBox1->Text->ToString(), text);
		textBox1->Clear();
		message newMessage;
		newMessage.cmd = COMMAND::START;
		newMessage.sArr.push_back(text);
		Messenger::sendMsg(hPipe, sizeof(message), newMessage);
		message newMessage2 = Messenger::readMsg(hPipe, sizeof(message));
		std::string s = newMessage2.sArr.at(0);
		String^ textbox2 = gcnew String(s.c_str());
		textBox2->Text += textbox2 + "\r\n";
	}
};
}
