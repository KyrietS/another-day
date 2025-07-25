#include "pch.hpp"

#include "Settings.hpp"

namespace another_day
{
Settings::Settings(const wxConfigBase& config)
{
    // clang-format off
    alwaysOnTop = config.ReadBool("AlwaysOnTop", alwaysOnTop);
    autoStartBreak = config.ReadBool("AutoStartBreak", autoStartBreak);
    autoStartSession = config.ReadBool("AutoStartSession", autoStartSession);
    enableWorkLog = config.ReadBool("EnableWorkLog", enableWorkLog);

    windowPosition.x = config.ReadLong("WindowPositionX", windowPosition.x);
    windowPosition.y = config.ReadLong("WindowPositionY", windowPosition.y);

    breakDuration = DurationSetting::FromString(config.Read("BreakDuration").ToStdString()).value_or(breakDuration);
    sessionDuration = DurationSetting::FromString(config.Read("SessionDuration").ToStdString()).value_or(sessionDuration);
    workDuration = DurationSetting::FromString(config.Read("WorkDuration").ToStdString()).value_or(workDuration);
    postponeDuration = DurationSetting::FromString(config.Read("PostponeDuration").ToStdString()).value_or(postponeDuration);

    useAudioNotification = config.ReadBool("UseAudioNotification", useAudioNotification);
    useSystemNotification = config.ReadBool("UseSystemNotification", useSystemNotification);
    notificationInterval = DurationSetting::FromString(config.Read("NotificationInterval").ToStdString()).value_or(notificationInterval);

    appDataPath = config.Read("AppDataPath", appDataPath.string()).ToStdString();
    // clang-format on
}

void Settings::SaveToConfig(wxConfigBase& config)
{
    config.Write("AlwaysOnTop", alwaysOnTop);
    config.Write("AutoStartBreak", autoStartBreak);
    config.Write("AutoStartSession", autoStartSession);
    config.Write("EnableWorkLog", enableWorkLog);
    config.Write("BreakDuration", wxString(breakDuration.valueString));
    config.Write("SessionDuration", wxString(sessionDuration.valueString));
    config.Write("WorkDuration", wxString(workDuration.valueString));
    config.Write("PostponeDuration", wxString(postponeDuration.valueString));
    config.Write("UseAudioNotification", useAudioNotification);
    config.Write("UseSystemNotification", useSystemNotification);
    config.Write("NotificationInterval", wxString(notificationInterval.valueString));
    config.Write("AppDataPath", wxString(appDataPath.string()));
    config.Write("WindowPositionX", windowPosition.x);
    config.Write("WindowPositionY", windowPosition.y);
    config.Flush();
}

long Settings::GetFrameStyle()
{
    return alwaysOnTop ? wxSTAY_ON_TOP : 0x00;
}
} // namespace another_day