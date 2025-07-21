#include "EditProgressWindow.hpp"
#include "DurationValidator.hpp"

namespace another_day
{
wxDEFINE_EVENT(EVT_ADD_WORK_TIME, DurationEvent);
wxDEFINE_EVENT(EVT_SUBTRACT_WORK_TIME, DurationEvent);
wxDEFINE_EVENT(EVT_RESET_DAY_PROGRESS, wxCommandEvent);

EditProgressWindow::EditProgressWindow(wxWindow* parent, Settings& settings)
    : wxDialog(parent, wxID_ANY, "Edit Progress", wxDefaultPosition, wxDefaultSize), settings(settings)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Alter work time
    {
        wxStaticBoxSizer* addSubBox = new wxStaticBoxSizer(wxVERTICAL, this, "Alter work time");

        wxBoxSizer* durationSizer = new wxBoxSizer(wxHORIZONTAL);
        durationSizer->Add(new wxStaticText(this, wxID_ANY, "Duration:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        durationCtrl = new wxTextCtrl(this, wxID_ANY);
        durationCtrl->SetValidator(DurationValidator(durationValue));
        durationCtrl->SetToolTip("Enter duration to add/subtract (units: s, m, h)");
        durationSizer->Add(durationCtrl, 1, wxEXPAND);

        addSubBox->Add(durationSizer, 0, wxEXPAND | wxALL, 10);

        wxBoxSizer* addSubButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
        addButton = new wxButton(this, wxID_ANY, "Add");
        subtractButton = new wxButton(this, wxID_ANY, "Subtract");
        addSubButtonsSizer->Add(addButton, 1, wxEXPAND | wxRIGHT, 5);
        addSubButtonsSizer->Add(subtractButton, 1, wxEXPAND);
        addSubBox->Add(addSubButtonsSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

        mainSizer->Add(addSubBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    }

    // Total reset
    {
        wxStaticBoxSizer* resetBox = new wxStaticBoxSizer(wxVERTICAL, this, "Total reset");
        resetButton = new wxButton(this, wxID_ANY, "Reset day");
        resetBox->Add(resetButton, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM | wxTOP, 5);
        mainSizer->Add(resetBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

        // OK/Cancel buttons
        wxBoxSizer* okCancelSizer = new wxBoxSizer(wxHORIZONTAL);
        auto closeButton = new wxButton(this, wxID_CANCEL, "Close");
        closeButton->SetDefault();
        okCancelSizer->Add(closeButton, 0, wxALL, 5);
        mainSizer->Add(okCancelSizer, 0, wxALIGN_RIGHT | wxALL, 5);
    }

    SetSizerAndFit(mainSizer);

    addButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnAdd, this);
    subtractButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnSubtract, this);
    resetButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnReset, this);
}

void EditProgressWindow::OnAdd(wxCommandEvent& event)
{
    if (not Validate() or not TransferDataFromWindow()) return;
    DurationEvent evt(EVT_ADD_WORK_TIME, durationValue.value);
    wxPostEvent(GetParent(), evt);
}

void EditProgressWindow::OnSubtract(wxCommandEvent& event)
{
    if (not Validate() or not TransferDataFromWindow()) return;
    DurationEvent evt(EVT_SUBTRACT_WORK_TIME, durationValue.value);
    wxPostEvent(GetParent(), evt);
}

void EditProgressWindow::OnReset(wxCommandEvent& event)
{
    if (wxMessageBox("This will annihilate your progress from today.\nAre you sure?", "Confirm Reset",
                     wxICON_QUESTION | wxYES_NO) == wxYES)
    {
        ResetDayProgress();
    }
}


void EditProgressWindow::ResetDayProgress()
{
    wxCommandEvent evt(EVT_RESET_DAY_PROGRESS);
    wxPostEvent(GetParent(), evt);
}

} // namespace another_day