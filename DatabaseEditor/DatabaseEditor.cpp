#include "DatabaseEditorForm.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	DatabaseEditor::MyForm Form;
	Application::Run(%Form);
	return 0;
}
