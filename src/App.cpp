#include "App.hpp"
#include "Database.hpp"
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
try
{
    // Fix needed until https://github.com/wxWidgets/wxWidgets/pull/25067 is merged.
    signal(SIGINT, OnSigInt);

    if (!wxApp::OnInit())
        return false;

    SetAppName("another-day");
    wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout_XDG);

    settings = std::make_unique<Settings>(*wxConfig::Get());
    database = std::make_unique<Database>(settings->appDataPath / "data.db");

    MainWindow* window = new MainWindow(*settings, *database);
    window->Show(true);
    return true;
}
catch (const std::exception& e)
{
    wxLogError("Error during app startup: \"%s\"", e.what());
    return false;
}

int App::OnRun()
{
    try
    {
        return wxApp::OnRun();
    }
    catch (const std::runtime_error& e)
    {
        wxLogError("Unhandled exception: \"%s\"", e.what());
        return EXIT_FAILURE;
    }
}

} // namespace another_day