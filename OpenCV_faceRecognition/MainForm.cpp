#include "MainForm.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
void Main()
{
	Application::EnableVisualStyles;
	Application::SetCompatibleTextRenderingDefault;
	OpenCV_faceRecognition::MainForm form;
	Application::Run(%form);
}

