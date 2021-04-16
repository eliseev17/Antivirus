#pragma once

#include <windows.h>
#include "Library.h"
#include <sstream>
#include <msclr/marshal_cppstd.h>

//msclr::interop::marshal_as<std::string>(mPathBox->Text);
//msclr::interop::marshal_as<String^>(el));

#define PIPE_BUFSIZE 1024

HANDLE pipeForSend = NULL;
HANDLE pipeForRead = NULL;
//HANDLE mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));

namespace Client {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	public ref class ClientForm : public System::Windows::Forms::Form
	{
	public:
		ClientForm(void)
		{
			//mutex = CreateMutex(NULL, FALSE, TEXT("mutex1"));
			InitializeComponent();
			Control::CheckForIllegalCrossThreadCalls = false;
		}

	protected:
		~ClientForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ btnStartScan;
	private: System::Windows::Forms::TextBox^ txtScanPath;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ btnBrowseFolder;
	private: System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog;
	private: System::Windows::Forms::TabControl^ tabControl1;
	private: System::Windows::Forms::TabPage^ tabPage1;
	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::TabPage^ tabPage3;
	private: System::Windows::Forms::Button^ btnDeleteFromQuarantine;
	private: System::Windows::Forms::Button^ btnMoveToQuarantine;
	private: System::Windows::Forms::Button^ btnDelete;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::ListBox^ reportListBox;
	private: System::Windows::Forms::TextBox^ txtMonitoringPath;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ btnSetMonitoring;
	private: System::Windows::Forms::Button^ btnBrowse2;
	private: System::Windows::Forms::ListBox^ monitoringListBox;
	private: System::Windows::Forms::ListBox^ scheduleListBox;
	private: System::Windows::Forms::TextBox^ txtSchedulePath;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ btnScheduleScan;
	private: System::Windows::Forms::Button^ btnBrowse3;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker;
	private: System::Windows::Forms::ProgressBar^ progressBar1;
	private: System::Windows::Forms::Button^ btnDelMonitoring;
	private: System::Windows::Forms::Button^ btnStopSchedule;
	private: System::Windows::Forms::Button^ btnStopScan;
	private: System::Windows::Forms::RichTextBox^ txtScanRes;
	private: System::Windows::Forms::Button^ btnBrowseFile;
	private: System::Windows::Forms::OpenFileDialog^ openFD;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::ListBox^ QuarantineListBox;
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
			this->btnStartScan = (gcnew System::Windows::Forms::Button());
			this->txtScanPath = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnBrowseFolder = (gcnew System::Windows::Forms::Button());
			this->folderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->QuarantineListBox = (gcnew System::Windows::Forms::ListBox());
			this->btnBrowseFile = (gcnew System::Windows::Forms::Button());
			this->txtScanRes = (gcnew System::Windows::Forms::RichTextBox());
			this->btnStopScan = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->reportListBox = (gcnew System::Windows::Forms::ListBox());
			this->btnDeleteFromQuarantine = (gcnew System::Windows::Forms::Button());
			this->btnMoveToQuarantine = (gcnew System::Windows::Forms::Button());
			this->btnDelete = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->btnDelMonitoring = (gcnew System::Windows::Forms::Button());
			this->monitoringListBox = (gcnew System::Windows::Forms::ListBox());
			this->txtMonitoringPath = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->btnSetMonitoring = (gcnew System::Windows::Forms::Button());
			this->btnBrowse2 = (gcnew System::Windows::Forms::Button());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->btnStopSchedule = (gcnew System::Windows::Forms::Button());
			this->dateTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->scheduleListBox = (gcnew System::Windows::Forms::ListBox());
			this->txtSchedulePath = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->btnScheduleScan = (gcnew System::Windows::Forms::Button());
			this->btnBrowse3 = (gcnew System::Windows::Forms::Button());
			this->openFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnStartScan
			// 
			this->btnStartScan->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnStartScan->BackColor = System::Drawing::Color::MintCream;
			this->btnStartScan->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnStartScan->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnStartScan->Location = System::Drawing::Point(670, 24);
			this->btnStartScan->Name = L"btnStartScan";
			this->btnStartScan->Size = System::Drawing::Size(205, 27);
			this->btnStartScan->TabIndex = 0;
			this->btnStartScan->Text = L"Начать сканирование";
			this->btnStartScan->UseVisualStyleBackColor = false;
			this->btnStartScan->Click += gcnew System::EventHandler(this, &ClientForm::btnStartScan_Click);
			// 
			// txtScanPath
			// 
			this->txtScanPath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtScanPath->Location = System::Drawing::Point(6, 23);
			this->txtScanPath->Name = L"txtScanPath";
			this->txtScanPath->ReadOnly = true;
			this->txtScanPath->Size = System::Drawing::Size(409, 27);
			this->txtScanPath->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(3, 3);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(140, 17);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Путь сканирования:";
			// 
			// btnBrowseFolder
			// 
			this->btnBrowseFolder->BackColor = System::Drawing::Color::Ivory;
			this->btnBrowseFolder->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnBrowseFolder->Location = System::Drawing::Point(544, 24);
			this->btnBrowseFolder->Name = L"btnBrowseFolder";
			this->btnBrowseFolder->Size = System::Drawing::Size(117, 27);
			this->btnBrowseFolder->TabIndex = 5;
			this->btnBrowseFolder->Text = L"Выбрать папку";
			this->btnBrowseFolder->UseVisualStyleBackColor = false;
			this->btnBrowseFolder->Click += gcnew System::EventHandler(this, &ClientForm::btnBrowse_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1100, 546);
			this->tabControl1->TabIndex = 6;
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::Color::White;
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->QuarantineListBox);
			this->tabPage1->Controls->Add(this->btnBrowseFile);
			this->tabPage1->Controls->Add(this->txtScanRes);
			this->tabPage1->Controls->Add(this->btnStopScan);
			this->tabPage1->Controls->Add(this->progressBar1);
			this->tabPage1->Controls->Add(this->reportListBox);
			this->tabPage1->Controls->Add(this->btnDeleteFromQuarantine);
			this->tabPage1->Controls->Add(this->btnMoveToQuarantine);
			this->tabPage1->Controls->Add(this->btnDelete);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->txtScanPath);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->btnStartScan);
			this->tabPage1->Controls->Add(this->btnBrowseFolder);
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(1092, 517);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Сканирование";
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(667, 276);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(76, 17);
			this->label5->TabIndex = 18;
			this->label5->Text = L"Карантин:";
			// 
			// QuarantineListBox
			// 
			this->QuarantineListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->QuarantineListBox->FormattingEnabled = true;
			this->QuarantineListBox->ItemHeight = 16;
			this->QuarantineListBox->Location = System::Drawing::Point(670, 296);
			this->QuarantineListBox->Name = L"QuarantineListBox";
			this->QuarantineListBox->Size = System::Drawing::Size(416, 180);
			this->QuarantineListBox->TabIndex = 17;
			// 
			// btnBrowseFile
			// 
			this->btnBrowseFile->BackColor = System::Drawing::Color::Ivory;
			this->btnBrowseFile->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnBrowseFile->Location = System::Drawing::Point(421, 24);
			this->btnBrowseFile->Name = L"btnBrowseFile";
			this->btnBrowseFile->Size = System::Drawing::Size(117, 27);
			this->btnBrowseFile->TabIndex = 16;
			this->btnBrowseFile->Text = L"Выбрать файл";
			this->btnBrowseFile->UseVisualStyleBackColor = false;
			this->btnBrowseFile->Click += gcnew System::EventHandler(this, &ClientForm::btnBrowseFile_Click);
			// 
			// txtScanRes
			// 
			this->txtScanRes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtScanRes->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtScanRes->Location = System::Drawing::Point(6, 57);
			this->txtScanRes->Name = L"txtScanRes";
			this->txtScanRes->ReadOnly = true;
			this->txtScanRes->Size = System::Drawing::Size(655, 419);
			this->txtScanRes->TabIndex = 15;
			this->txtScanRes->Text = L"";
			this->txtScanRes->AutoScrollOffset = System::Drawing::Point(655, 419);
			// 
			// btnStopScan
			// 
			this->btnStopScan->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnStopScan->BackColor = System::Drawing::Color::MistyRose;
			this->btnStopScan->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnStopScan->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnStopScan->Location = System::Drawing::Point(881, 24);
			this->btnStopScan->Name = L"btnStopScan";
			this->btnStopScan->Size = System::Drawing::Size(205, 27);
			this->btnStopScan->TabIndex = 14;
			this->btnStopScan->Text = L"Остановить сканирование";
			this->btnStopScan->UseVisualStyleBackColor = false;
			this->btnStopScan->Click += gcnew System::EventHandler(this, &ClientForm::btnStopScan_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->progressBar1->Location = System::Drawing::Point(6, 482);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(655, 28);
			this->progressBar1->TabIndex = 13;
			// 
			// reportListBox
			// 
			this->reportListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->reportListBox->FormattingEnabled = true;
			this->reportListBox->ItemHeight = 16;
			this->reportListBox->Location = System::Drawing::Point(670, 88);
			this->reportListBox->Name = L"reportListBox";
			this->reportListBox->Size = System::Drawing::Size(416, 180);
			this->reportListBox->TabIndex = 12;
			// 
			// btnDeleteFromQuarantine
			// 
			this->btnDeleteFromQuarantine->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnDeleteFromQuarantine->BackColor = System::Drawing::Color::LavenderBlush;
			this->btnDeleteFromQuarantine->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnDeleteFromQuarantine->Location = System::Drawing::Point(964, 482);
			this->btnDeleteFromQuarantine->Name = L"btnDeleteFromQuarantine";
			this->btnDeleteFromQuarantine->Size = System::Drawing::Size(122, 28);
			this->btnDeleteFromQuarantine->TabIndex = 10;
			this->btnDeleteFromQuarantine->Text = L"Из карантина";
			this->btnDeleteFromQuarantine->UseVisualStyleBackColor = false;
			this->btnDeleteFromQuarantine->Click += gcnew System::EventHandler(this, &ClientForm::btnDeleteFromQuarantine_Click);
			// 
			// btnMoveToQuarantine
			// 
			this->btnMoveToQuarantine->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnMoveToQuarantine->BackColor = System::Drawing::Color::Lavender;
			this->btnMoveToQuarantine->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnMoveToQuarantine->Location = System::Drawing::Point(817, 482);
			this->btnMoveToQuarantine->Name = L"btnMoveToQuarantine";
			this->btnMoveToQuarantine->Size = System::Drawing::Size(141, 28);
			this->btnMoveToQuarantine->TabIndex = 9;
			this->btnMoveToQuarantine->Text = L"Карантин";
			this->btnMoveToQuarantine->UseVisualStyleBackColor = false;
			this->btnMoveToQuarantine->Click += gcnew System::EventHandler(this, &ClientForm::btnMoveToQuarantine_Click);
			// 
			// btnDelete
			// 
			this->btnDelete->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnDelete->BackColor = System::Drawing::Color::MistyRose;
			this->btnDelete->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnDelete->Location = System::Drawing::Point(670, 482);
			this->btnDelete->Name = L"btnDelete";
			this->btnDelete->Size = System::Drawing::Size(141, 28);
			this->btnDelete->TabIndex = 8;
			this->btnDelete->Text = L"Удалить";
			this->btnDelete->UseVisualStyleBackColor = false;
			this->btnDelete->Click += gcnew System::EventHandler(this, &ClientForm::btnDelete_Click);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(667, 68);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 17);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Угрозы:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->btnDelMonitoring);
			this->tabPage2->Controls->Add(this->monitoringListBox);
			this->tabPage2->Controls->Add(this->txtMonitoringPath);
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->btnSetMonitoring);
			this->tabPage2->Controls->Add(this->btnBrowse2);
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(1092, 517);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Мониторинг папки";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// btnDelMonitoring
			// 
			this->btnDelMonitoring->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnDelMonitoring->BackColor = System::Drawing::Color::MistyRose;
			this->btnDelMonitoring->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnDelMonitoring->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnDelMonitoring->Location = System::Drawing::Point(848, 23);
			this->btnDelMonitoring->Name = L"btnDelMonitoring";
			this->btnDelMonitoring->Size = System::Drawing::Size(238, 27);
			this->btnDelMonitoring->TabIndex = 13;
			this->btnDelMonitoring->Text = L"Остановить мониторинг";
			this->btnDelMonitoring->UseVisualStyleBackColor = false;
			// 
			// monitoringListBox
			// 
			this->monitoringListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->monitoringListBox->FormattingEnabled = true;
			this->monitoringListBox->ItemHeight = 16;
			this->monitoringListBox->Location = System::Drawing::Point(6, 56);
			this->monitoringListBox->Name = L"monitoringListBox";
			this->monitoringListBox->Size = System::Drawing::Size(1080, 452);
			this->monitoringListBox->TabIndex = 12;
			// 
			// txtMonitoringPath
			// 
			this->txtMonitoringPath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->txtMonitoringPath->Location = System::Drawing::Point(6, 23);
			this->txtMonitoringPath->Name = L"txtMonitoringPath";
			this->txtMonitoringPath->ReadOnly = true;
			this->txtMonitoringPath->Size = System::Drawing::Size(494, 27);
			this->txtMonitoringPath->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(3, 3);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(182, 17);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Директория мониторинга:";
			// 
			// btnSetMonitoring
			// 
			this->btnSetMonitoring->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnSetMonitoring->BackColor = System::Drawing::Color::MintCream;
			this->btnSetMonitoring->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnSetMonitoring->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnSetMonitoring->Location = System::Drawing::Point(604, 23);
			this->btnSetMonitoring->Name = L"btnSetMonitoring";
			this->btnSetMonitoring->Size = System::Drawing::Size(238, 27);
			this->btnSetMonitoring->TabIndex = 6;
			this->btnSetMonitoring->Text = L"Установить мониторинг";
			this->btnSetMonitoring->UseVisualStyleBackColor = false;
			this->btnSetMonitoring->Click += gcnew System::EventHandler(this, &ClientForm::btnSetMonitoring_Click);
			// 
			// btnBrowse2
			// 
			this->btnBrowse2->BackColor = System::Drawing::Color::Ivory;
			this->btnBrowse2->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnBrowse2->Location = System::Drawing::Point(506, 23);
			this->btnBrowse2->Name = L"btnBrowse2";
			this->btnBrowse2->Size = System::Drawing::Size(92, 27);
			this->btnBrowse2->TabIndex = 9;
			this->btnBrowse2->Text = L"Обзор";
			this->btnBrowse2->UseVisualStyleBackColor = false;
			this->btnBrowse2->Click += gcnew System::EventHandler(this, &ClientForm::btnBrowse2_Click);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->btnStopSchedule);
			this->tabPage3->Controls->Add(this->dateTimePicker);
			this->tabPage3->Controls->Add(this->scheduleListBox);
			this->tabPage3->Controls->Add(this->txtSchedulePath);
			this->tabPage3->Controls->Add(this->label4);
			this->tabPage3->Controls->Add(this->btnScheduleScan);
			this->tabPage3->Controls->Add(this->btnBrowse3);
			this->tabPage3->Location = System::Drawing::Point(4, 25);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(1092, 517);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Сканирование по расписанию";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// btnStopSchedule
			// 
			this->btnStopSchedule->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnStopSchedule->BackColor = System::Drawing::Color::MistyRose;
			this->btnStopSchedule->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnStopSchedule->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnStopSchedule->Location = System::Drawing::Point(900, 23);
			this->btnStopSchedule->Name = L"btnStopSchedule";
			this->btnStopSchedule->Size = System::Drawing::Size(186, 27);
			this->btnStopSchedule->TabIndex = 20;
			this->btnStopSchedule->Text = L"Отменить";
			this->btnStopSchedule->UseVisualStyleBackColor = false;
			// 
			// dateTimePicker
			// 
			this->dateTimePicker->CalendarFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->dateTimePicker->CustomFormat = L"HH:mm";
			this->dateTimePicker->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->dateTimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker->Location = System::Drawing::Point(604, 23);
			this->dateTimePicker->Name = L"dateTimePicker";
			this->dateTimePicker->ShowUpDown = true;
			this->dateTimePicker->Size = System::Drawing::Size(97, 27);
			this->dateTimePicker->TabIndex = 19;
			// 
			// scheduleListBox
			// 
			this->scheduleListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->scheduleListBox->FormattingEnabled = true;
			this->scheduleListBox->ItemHeight = 16;
			this->scheduleListBox->Location = System::Drawing::Point(6, 56);
			this->scheduleListBox->Name = L"scheduleListBox";
			this->scheduleListBox->Size = System::Drawing::Size(1080, 452);
			this->scheduleListBox->TabIndex = 17;
			// 
			// txtSchedulePath
			// 
			this->txtSchedulePath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtSchedulePath->Location = System::Drawing::Point(6, 23);
			this->txtSchedulePath->Name = L"txtSchedulePath";
			this->txtSchedulePath->ReadOnly = true;
			this->txtSchedulePath->Size = System::Drawing::Size(494, 27);
			this->txtSchedulePath->TabIndex = 14;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(3, 3);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(190, 17);
			this->label4->TabIndex = 15;
			this->label4->Text = L"Директория сканирования:";
			// 
			// btnScheduleScan
			// 
			this->btnScheduleScan->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnScheduleScan->BackColor = System::Drawing::Color::MintCream;
			this->btnScheduleScan->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnScheduleScan->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnScheduleScan->Location = System::Drawing::Point(707, 23);
			this->btnScheduleScan->Name = L"btnScheduleScan";
			this->btnScheduleScan->Size = System::Drawing::Size(187, 27);
			this->btnScheduleScan->TabIndex = 13;
			this->btnScheduleScan->Text = L"Запланировать";
			this->btnScheduleScan->UseVisualStyleBackColor = false;
			this->btnScheduleScan->Click += gcnew System::EventHandler(this, &ClientForm::btnScheduleScan_Click);
			// 
			// btnBrowse3
			// 
			this->btnBrowse3->BackColor = System::Drawing::Color::Ivory;
			this->btnBrowse3->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnBrowse3->Location = System::Drawing::Point(506, 23);
			this->btnBrowse3->Name = L"btnBrowse3";
			this->btnBrowse3->Size = System::Drawing::Size(92, 27);
			this->btnBrowse3->TabIndex = 16;
			this->btnBrowse3->Text = L"Обзор";
			this->btnBrowse3->UseVisualStyleBackColor = false;
			this->btnBrowse3->Click += gcnew System::EventHandler(this, &ClientForm::btnBrowse3_Click);
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1098, 545);
			this->Controls->Add(this->tabControl1);
			this->MinimumSize = System::Drawing::Size(1050, 300);
			this->Name = L"ClientForm";
			this->Text = L"Лучший в мире антивирус (но это не точно)";
			this->Load += gcnew System::EventHandler(this, &ClientForm::ClientForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	void messageProcessing(message msg)
	{
		switch (msg.cmd)
		{
		case COMMAND::START: {
			if (!msg.nArr.empty())
			{
				progressBar1->Maximum = msg.nArr.at(0);
			}
			else
			{
				//size_t nProb = 80 - msg.sArr.at(0).length();
				txtScanRes->AppendText(msclr::interop::marshal_as<String^>(msg.sArr.at(0)));
				//for (size_t i = 0; i < nProb; i++)
				txtScanRes->AppendText("\t\t\t");
				txtScanRes->AppendText(msclr::interop::marshal_as<String^>(msg.sArr.at(1)));
				txtScanRes->AppendText("\r\n");
				progressBar1->Increment(1);
				if (msclr::interop::marshal_as<String^>(msg.sArr.at(1))->Contains("Virus"))
					reportListBox->Items->Add(msclr::interop::marshal_as<String^>(msg.sArr.at(0))); // +"    " + msclr::interop::marshal_as<String^>(msg.sArr.at(1)) + "\r\n");
			}			
			break;
		}
		case COMMAND::STOP: {
			progressBar1->Value = 0;
			txtScanRes->Clear();
			MessageBox::Show("Сканирование остановлено", "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
			break;
		}
		case COMMAND::SCAN_RESULT: {
			btnBrowseFile->Enabled = true;
			btnBrowseFolder->Enabled = true;
			btnMoveToQuarantine->Enabled = true;
			btnDelete->Enabled = true;
			btnDeleteFromQuarantine->Enabled = true;
			txtScanRes->Text +=  "\r\nКоличество просканированных файлов: " + msg.nArr.at(0);
			txtScanRes->Text += "\r\nУгроз: " + msg.nArr.at(1);
			MessageBox::Show("Сканирование успешно завершилось!\r\nКоличество просканированных файлов: " + msg.nArr.at(0) + "\r\nУгроз: " + msg.nArr.at(1), "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
			break;
		}
		}
	}
	private: bool createPipeForRead()
	{
		pipeForRead = CreateNamedPipe(
			L"\\\\.\\pipe\\secondPipe",             // имя канала 
			PIPE_ACCESS_DUPLEX,       // чтение/запись доступ 
			PIPE_TYPE_MESSAGE |       // тип сообщения каналу 
			PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
			PIPE_WAIT,                // режим блокирования
			PIPE_UNLIMITED_INSTANCES, // максимальное количество экземпляров 
			PIPE_BUFSIZE,                  // размер буфера на выход 
			PIPE_BUFSIZE,                  // размер буфера на вход 
			0,                        // клиент время 
			NULL);                    // атрибут безопасности по умолчанию 

		if (pipeForRead == INVALID_HANDLE_VALUE)
		{
			MessageBox::Show("Нет соединения с сервером\r\nКод ошибки: " + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			FlushFileBuffers(pipeForRead);
			DisconnectNamedPipe(pipeForRead);
			CloseHandle(pipeForRead);
			createPipeForSend();
			return false;
		}
		return true;
	}
	void createPipeAndListening()
	{
		bool fConnected1 = false;
		while (!fConnected1)
		{
			fConnected1 = createPipeForSend();			
			
		}
		createPipeForRead();

		// Подождите, пока клиент подключится; если это удастся,
		// функция возвращает ненулевое значение. Если функция
		// возвращает ноль, GetLastError возвращает ERROR_PIPE_CONNECTED.
		BOOL fConnected = ConnectNamedPipe(pipeForRead, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		message messagefromService;
		while (fConnected)
		{
			fConnected = ConnectNamedPipe(pipeForRead, NULL) ?
				TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
			if (!fConnected)
			{
				MessageBox::Show("Нет соединения с сервером\r\n" + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				CloseHandle(pipeForRead);
			}
			messagefromService = Messenger::readMessage(pipeForRead, PIPE_BUFSIZE);
			if (messagefromService.cmd == COMMAND::UNKNOWN)
			{				
				MessageBox::Show("Сервер был закрыт", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				progressBar1->Value = 0;
				bool fConnected2 = false, fConnected3 = false;
				while (!fConnected2)
				{
					fConnected2 = createPipeForSend();

				}
				while (!fConnected3)
				{
					fConnected3 = createPipeForRead();

				}
			}
			messageProcessing(messagefromService);
		}
		
		//progressBar1->Value = 0;
		//FlushFileBuffers(pipeForRead);
		//DisconnectNamedPipe(pipeForRead);
		//CloseHandle(pipeForRead);
		//createPipeForSend();
		//Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &ClientForm::createPipeAndListening));
		//thread->IsBackground = true;
		//thread->Start();
		//createPipeAndListening();
}		

	private: System::Void btnBrowse_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtScanPath->Text = folderBrowserDialog->SelectedPath;
		}
	}

	private: System::Void btnBrowse2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtMonitoringPath->Text = folderBrowserDialog->SelectedPath;
		}
	}

	private: System::Void btnBrowse3_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtSchedulePath->Text = folderBrowserDialog->SelectedPath;
		}
	}

	private: System::Void btnScheduleScan_Click(System::Object^ sender, System::EventArgs^ e)
	{
		scheduleListBox->Items->Add(dateTimePicker->Value.ToShortTimeString() + "\t" + txtSchedulePath->Text);
		txtSchedulePath->Clear();
	}

	private: System::Void btnSetMonitoring_Click(System::Object^ sender, System::EventArgs^ e)
	{
		txtMonitoringPath->Clear();
		monitoringListBox->Items->Add(txtMonitoringPath->Text);
		message monitoringMessage;
		monitoringMessage.cmd = COMMAND::SET_MONITORING;
		Messenger::sendMessage(pipeForSend, PIPE_BUFSIZE, monitoringMessage);		
	}

	private: System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
	{
		reportListBox->Items->Remove(reportListBox->SelectedItem);
	}

	private: System::Void btnMoveToQuarantine_Click(System::Object^ sender, System::EventArgs^ e)
	{
		QuarantineListBox->Items->Add(reportListBox->SelectedItem);
		reportListBox->Items->Remove(reportListBox->SelectedItem);
	}

	private: System::Void btnDeleteFromQuarantine_Click(System::Object^ sender, System::EventArgs^ e)
	{
		reportListBox->Items->Add(QuarantineListBox->SelectedItem);
		QuarantineListBox->Items->Remove(QuarantineListBox->SelectedItem);
	}

	private: System::Void ClientForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &ClientForm::createPipeAndListening));
		thread->IsBackground = true;
		thread->Start();
	}

	private: bool createPipeForSend()
	{
		pipeForSend = NULL;
		CloseHandle(pipeForSend);
		TCHAR pipename[22] = L"\\\\.\\pipe\\firstPipe";
		if (pipeForSend == NULL)
			pipeForSend = CreateFile(
				pipename,   // pipe name 
				GENERIC_READ |  // read and write access 
				GENERIC_WRITE,
				0,              // no sharing 
				NULL,           // default security attributes
				OPEN_EXISTING,  // opens existing pipe 
				0,              // default attributes 
				NULL);          // no template file 

		  // Break if the pipe handle is valid.
		if (pipeForSend == INVALID_HANDLE_VALUE)
		{
			CloseHandle(pipeForSend);
			MessageBox::Show("Нет соединения с сервером\r\n" + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			pipeForSend = NULL;
			return false;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			CloseHandle(pipeForSend);
			MessageBox::Show("Нет соединения с сервером\r\n" + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			pipeForSend = NULL;
			return false;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() == ERROR_BROKEN_PIPE)
		{
			CloseHandle(pipeForSend);
			MessageBox::Show("Нет соединения с сервером\r\n" + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			pipeForSend = NULL;
			return false;
		}

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			CloseHandle(pipeForSend);
			MessageBox::Show("Нет соединения с сервером\r\n" + GetLastError().ToString(), "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			pipeForSend = NULL;
			return false;
		}
		if (!GetLastError())
			return true;
	}

	private: System::Void btnStartScan_Click(System::Object^ sender, System::EventArgs^ e)
	{
		btnBrowseFile->Enabled = false;
		btnBrowseFolder->Enabled = false;
		btnMoveToQuarantine->Enabled = false;
		btnDelete->Enabled = false;
		btnDeleteFromQuarantine->Enabled = false;
		txtScanRes->Clear();
		progressBar1->Value = 0;
		message startScanMsg;
		std::string path = msclr::interop::marshal_as<std::string>(txtScanPath->Text);
		startScanMsg.cmd = COMMAND::START;
		startScanMsg.sArr.push_back(path);
		Messenger::sendMessage(pipeForSend, PIPE_BUFSIZE, startScanMsg);		
	}

	private: System::Void btnBrowseFile_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (openFD->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtScanPath->Text = openFD->FileName;
		}
	}

	private: System::Void btnStopScan_Click(System::Object^ sender, System::EventArgs^ e)
	{
		//txtScanRes->Clear();
		message startScanMsg;
		//std::string path = msclr::interop::marshal_as<std::string>(txtScanPath->Text);
		startScanMsg.cmd = COMMAND::STOP;
		//startScanMsg.sArr.push_back(path);
		Messenger::sendMessage(pipeForSend, PIPE_BUFSIZE, startScanMsg);
	}
};
}
