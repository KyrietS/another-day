#pragma once

#include <wx/wx.h>

namespace another_day
{
class SettingsWindow : public wxDialog
{
public:
    SettingsWindow(wxWindow* parent);

private:
    wxTextCtrl* breakDurationCtrl;
    wxTextCtrl* sessionDurationCtrl;
    wxTextCtrl* workDurationCtrl;
    wxTextCtrl* postponekDurationCtrl;
    wxCheckBox* useAudioNotificationCheckBox;
    wxCheckBox* useSystemNotificationCheckBox;
    wxCheckBox* autoStartBreakCheckBox;
    wxCheckBox* autoStartSessionCheckBox;
};
} // namespace another_day
