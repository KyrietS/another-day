#pragma once

#include "Settings.hpp"
#include <wx/wx.h>

namespace another_day
{
class SettingsWindow : public wxDialog
{
public:
    SettingsWindow(wxWindow* parent, Settings& settings);
    void EndModal(int code) override;

private:
    Settings& settings;

    // General
    wxCheckBox* alwaysOnTopCheckBox{};
    wxCheckBox* autoStartBreakCheckBox{};
    wxCheckBox* autoStartSessionCheckBox{};
    wxCheckBox* enableWorkLogCheckBox{};

    // Duration
    wxTextCtrl* breakDurationCtrl{};
    wxTextCtrl* sessionDurationCtrl{};
    wxTextCtrl* workDurationCtrl{};
    wxTextCtrl* postponeDurationCtrl{};

    // Notification
    wxCheckBox* useAudioNotificationCheckBox{};
    wxCheckBox* useSystemNotificationCheckBox{};
};
} // namespace another_day
