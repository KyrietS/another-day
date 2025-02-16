#include "pch.hpp"

#include "Paths.hpp"

namespace another_day::paths
{
namespace
{
void CreateDirIfNotExists(const std::filesystem::path& dir)
{
    if (not std::filesystem::is_directory(dir))
    {
        if (not std::filesystem::create_directories(dir))
        {
            throw std::runtime_error("Failed to create directory: " + dir.string());
        }
        wxLogDebug("Created directory: %s", dir.string());
    }
}
} // namespace

std::filesystem::path GetAppDataPath()
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
    userDataDir /= wxTheApp->GetAppName().ToStdString();
#endif

    try
    {
        CreateDirIfNotExists(userDataDir);
        return userDataDir;
    }
    catch (const std::exception& e)
    {
        wxLogError("%s", e.what());
        return {};
    }
}
} // namespace another_day::paths