#include "pch.hpp"
#include "App.hpp"
#include "MainWindow.hpp"

namespace minutea
{
	bool App::OnInit()
	{
		if (!wxApp::OnInit())
			return false;

		SetAppName("Minutea");
		wxConfig::Get()->SetRecordDefaults(true);

		MainWindow* window = new MainWindow("");
		window->Show(true);
		return true;
	}
}