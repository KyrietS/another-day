#pragma once
#include "Settings.hpp"
#include "Duration.hpp"


namespace another_day
{
class DurationEvent : public wxCommandEvent {
public:
    DurationEvent(wxEventType eventType, Duration duration)
        : wxCommandEvent(eventType), duration(duration) {}
    DurationEvent(const DurationEvent& other)
        : wxCommandEvent(other), duration(other.duration) {}
    virtual wxEvent* Clone() const override { return new DurationEvent(*this); }
    Duration duration;
};

wxDECLARE_EVENT(EVT_ADD_WORK_TIME, DurationEvent);
wxDECLARE_EVENT(EVT_SUBTRACT_WORK_TIME, DurationEvent);
wxDECLARE_EVENT(EVT_RESET_DAY_PROGRESS, wxCommandEvent);

class EditProgressWindow : public wxDialog
{
public:
    EditProgressWindow(wxWindow* parent, Settings& settings);

private:
    Settings& settings;
    DurationSetting durationValue{};

    wxTextCtrl* durationCtrl{};
    wxButton* addButton{};
    wxButton* subtractButton{};
    wxButton* resetButton{};

    void OnAdd(wxCommandEvent& event);
    void OnSubtract(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);

    void ResetDayProgress();
};
}