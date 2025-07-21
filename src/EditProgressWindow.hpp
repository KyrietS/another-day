#pragma once
#include "Settings.hpp"

namespace another_day
{
class EditProgressWindow : public wxDialog
{
public:
    EditProgressWindow(wxWindow* parent, Settings& settings);

private:
    Settings& settings;

    wxTextCtrl* durationCtrl{};
    wxButton* addButton{};
    wxButton* subtractButton{};
    wxButton* resetButton{};

    void OnAdd(wxCommandEvent& event);
    void OnSubtract(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);

    void ApplyProgressChange(int sign); // sign: +1 for add, -1 for subtract
    void ResetDayProgress();
};
}