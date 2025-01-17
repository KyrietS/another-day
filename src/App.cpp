#include "App.hpp"
#include "MainWindow.hpp"
#include "pch.hpp"
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

    settings = std::make_unique<Settings>(*wxConfig::Get());

    MainWindow* window = new MainWindow(*settings);
    window->Show(true);
    return true;
}
} // namespace another_day