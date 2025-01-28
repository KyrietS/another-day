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

    wxTextCtrl* breakDurationCtrl;
    wxTextCtrl* sessionDurationCtrl;
    wxTextCtrl* workDurationCtrl;
    wxTextCtrl* postponeDurationCtrl;
    wxCheckBox* useAudioNotificationCheckBox;
    wxCheckBox* useSystemNotificationCheckBox;
    wxCheckBox* alwaysOnTopCheckBox;
    wxCheckBox* autoStartBreakCheckBox;
    wxCheckBox* autoStartSessionCheckBox;
};
} // namespace another_day
