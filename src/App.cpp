#include "pch.hpp"
#include "App.hpp"
#include "MainWindow.hpp"

namespace minutea
{
	bool App::OnInit()
	{
		if (!wxApp::OnInit())
			return false;
		MainWindow* window = new MainWindow("Minutea App");
		window->Show(true);
		return true;
	}
}