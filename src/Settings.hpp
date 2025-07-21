#pragma once
#include "Duration.hpp"
#include "Paths.hpp"
#include <filesystem>

class wxConfigBase;

namespace another_day
{
struct Settings
{
    Settings(const wxConfigBase& config);
    void SaveToConfig(wxConfigBase& config);

    long GetFrameStyle();

    // General
    bool alwaysOnTop = false;
    bool autoStartBreak = false;
    bool autoStartSession = true;
    bool enableWorkLog = true;

    // Duration
    DurationSetting breakDuration = std::chrono::minutes{10};
    DurationSetting sessionDuration = std::chrono::minutes{50};
    DurationSetting workDuration = std::chrono::hours{8};
    DurationSetting postponeDuration = {};

    // Notification
    bool useAudioNotification = true;
    bool useSystemNotification = false;
    DurationSetting notificationInterval = std::chrono::seconds{60};

    // Window settings
    wxPoint windowPosition = wxDefaultPosition;

    // Hidden
    std::filesystem::path appDataPath = paths::GetAppDataPath();
};
} // namespace another_day