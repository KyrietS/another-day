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
    database = std::make_shared<Database>(GetUserDataDir() / "history.db");

    MainWindow* window = new MainWindow(*settings, *database);
    window->Show(true);
    return true;
}
catch (const std::exception& e)
{
    wxLogError("Error during app startup: %s", e.what());
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
        wxLogError("Unhandled exception: %s", e.what());
        return EXIT_FAILURE;
    }
}

std::filesystem::path App::GetUserDataDir()
{
#if defined(__WINDOWS__)
    std::filesystem::path userDataDir = wxStandardPaths::Get().GetUserDataDir().ToStdString();
#else
    wxString xdgDataHome = wxGetenv("XDG_DATA_HOME");
    if (xdgDataHome.empty())
    {
        xdgDataHome = wxString(wxGetenv("HOME")) + "/.local/share";
    }
    std::filesystem::path userDataDir = xdgDataHome.ToStdString();
    userDataDir /= GetAppName().ToStdString();
#endif

    if (not std::filesystem::is_directory(userDataDir))
    {
        if (not std::filesystem::create_directories(userDataDir))
        {
            throw std::runtime_error("Failed to create user data directory: " + userDataDir.string());
        }
        wxLogDebug("Created user data directory: %s", userDataDir.string());
    }

    return userDataDir;
}

} // namespace another_day