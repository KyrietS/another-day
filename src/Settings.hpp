#pragma once
#include "Duration.hpp"

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

    // Duration
    DurationWithUnit breakDuration = std::chrono::minutes{10};
    DurationWithUnit sessionDuration = std::chrono::minutes{50};
    DurationWithUnit workDuration = std::chrono::hours{8};
    DurationWithUnit postponeDuration = {};

    // Notification
    bool useAudioNotification = true;
    bool useSystemNotification = false;
    DurationWithUnit notificationInterval = std::chrono::seconds{60};
};
} // namespace another_day