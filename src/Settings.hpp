#pragma once
#include "Duration.hpp"

class wxConfigBase;

namespace another_day
{
struct Settings
{
    Settings(const wxConfigBase& config);
    void SaveToConfig(wxConfigBase& config);

    // General
    bool alwaysOnTop = false;
    bool autoStartBreak = false;
    bool autoStartSession = true;

    // Duration
    Duration breakDuration = std::chrono::minutes{10};
    Duration sessionDuration = std::chrono::minutes{50};
    Duration workDuration = std::chrono::hours{8};
    Duration postponeDuration = {};

    // Notification
    bool useAudioNotification = true;
    bool useSystemNotification = true;
    Duration notificationInterval = std::chrono::seconds{60};
};
} // namespace another_day