#pragma once
#include "pch.hpp"

#include "Settings.hpp"

namespace another_day
{
Settings::Settings(const wxConfigBase& config)
{
    alwaysOnTop = config.ReadBool("AlwaysOnTop", alwaysOnTop);
    autoStartBreak = config.ReadBool("AutoStartBreak", autoStartBreak);
    autoStartSession = config.ReadBool("AutoStartSession", autoStartSession);

    breakDuration = std::chrono::seconds{config.ReadLongLong("BreakDuration", breakDuration.count())};
    sessionDuration = std::chrono::seconds{config.ReadLongLong("SessionDuration", sessionDuration.count())};
    workDuration = std::chrono::seconds{config.ReadLongLong("WorkDuration", workDuration.count())};
    postponeDuration = std::chrono::seconds{config.ReadLongLong("PostponeDuration", postponeDuration.count())};

    useAudioNotification = config.ReadBool("UseAudioNotification", useAudioNotification);
    useSystemNotification = config.ReadBool("UseSystemNotification", useSystemNotification);
    notificationInterval =
        std::chrono::seconds{config.ReadLongLong("NotificationInterval", notificationInterval.count())};
}

void Settings::SaveToConfig(wxConfigBase& config)
{
    config.Write("AlwaysOnTop", alwaysOnTop);
    config.Write("AutoStartBreak", autoStartBreak);
    config.Write("AutoStartSession", autoStartSession);
    config.Write("BreakDuration", breakDuration.count());
    config.Write("SessionDuration", sessionDuration.count());
    config.Write("WorkDuration", workDuration.count());
    config.Write("PostponeDuration", postponeDuration.count());
    config.Write("UseAudioNotification", useAudioNotification);
    config.Write("UseSystemNotification", useSystemNotification);
    config.Write("NotificationInterval", notificationInterval.count());
    config.Flush();
}
} // namespace another_day