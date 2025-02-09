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

    breakDuration = DurationWithUnit::FromString(config.Read("BreakDuration").ToStdString()).value_or(breakDuration);
    sessionDuration =
        DurationWithUnit::FromString(config.Read("SessionDuration").ToStdString()).value_or(sessionDuration);
    workDuration = DurationWithUnit::FromString(config.Read("WorkDuration").ToStdString()).value_or(workDuration);
    postponeDuration =
        DurationWithUnit::FromString(config.Read("PostponeDuration").ToStdString()).value_or(postponeDuration);

    useAudioNotification = config.ReadBool("UseAudioNotification", useAudioNotification);
    useSystemNotification = config.ReadBool("UseSystemNotification", useSystemNotification);

    notificationInterval =
        DurationWithUnit::FromString(config.Read("NotificationInterval").ToStdString()).value_or(notificationInterval);
}

void Settings::SaveToConfig(wxConfigBase& config)
{
    config.Write("AlwaysOnTop", alwaysOnTop);
    config.Write("AutoStartBreak", autoStartBreak);
    config.Write("AutoStartSession", autoStartSession);
    config.Write("BreakDuration", wxString(breakDuration.ToString()));
    config.Write("SessionDuration", wxString(sessionDuration.ToString()));
    config.Write("WorkDuration", wxString(workDuration.ToString()));
    config.Write("PostponeDuration", wxString(postponeDuration.ToString()));
    config.Write("UseAudioNotification", useAudioNotification);
    config.Write("UseSystemNotification", useSystemNotification);
    config.Write("NotificationInterval", wxString(notificationInterval.ToString()));
    config.Flush();
}

long Settings::GetFrameStyle()
{
    return alwaysOnTop ? wxSTAY_ON_TOP : 0x00;
}
} // namespace another_day