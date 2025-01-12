#include "pch.hpp"
#include "App.hpp"
#include "MainWindow.hpp"
#include <csignal>

extern "C" void OnSigInt(int sig)
{
	exit(sig);
}

namespace another_day
{
	bool App::OnInit()
	{
		// Fix needed until https://github.com/wxWidgets/wxWidgets/pull/25067 is merged.
		signal(SIGINT, OnSigInt);

		if (!wxApp::OnInit())
			return false;

		SetAppName("another-day");
		wxConfig::Get()->SetRecordDefaults(true);

		MainWindow* window = new MainWindow("");
		window->Show(true);
		return true;
	}
}