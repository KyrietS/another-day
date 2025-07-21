#include "EditProgressWindow.hpp"
#include <wx/wx.h>

namespace another_day
{
EditProgressWindow::EditProgressWindow(wxWindow* parent, Settings& settings)
    : wxDialog(parent, wxID_ANY, "Edit Progress", wxDefaultPosition, wxDefaultSize), settings(settings)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add/Subtract group
    wxStaticBoxSizer* addSubBox = new wxStaticBoxSizer(wxVERTICAL, this, "Alter work time");

    // Duration label and input in a horizontal sizer
    wxBoxSizer* durationSizer = new wxBoxSizer(wxHORIZONTAL);
    durationSizer->Add(new wxStaticText(this, wxID_ANY, "Duration:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

    durationCtrl = new wxTextCtrl(this, wxID_ANY);
    durationCtrl->SetToolTip("Enter duration to add/subtract (units: s, m, h)");
    durationSizer->Add(durationCtrl, 1, wxEXPAND);

    addSubBox->Add(durationSizer, 0, wxEXPAND | wxALL, 10);

    // Add/Subtract buttons in a horizontal sizer below durationCtrl
    wxBoxSizer* buttonRowSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(this, wxID_ANY, "Add");
    subtractButton = new wxButton(this, wxID_ANY, "Subtract");
    buttonRowSizer->Add(addButton, 1, wxEXPAND | wxRIGHT, 5);
    buttonRowSizer->Add(subtractButton, 1, wxEXPAND);

    // Make buttonRowSizer take same width as durationCtrl
    addSubBox->Add(buttonRowSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    mainSizer->Add(addSubBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Reset group
    // wxStaticBoxSizer* resetBox = new wxStaticBoxSizer(wxVERTICAL, this, "Reset whole day");
    wxStaticBoxSizer* resetBox = new wxStaticBoxSizer(wxVERTICAL, this, "Total reset");
    resetButton = new wxButton(this, wxID_ANY, "Reset day progress");
    resetBox->Add(resetButton, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM | wxTOP, 5);
    mainSizer->Add(resetBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // OK/Cancel buttons
    wxBoxSizer* okCancelSizer = new wxBoxSizer(wxHORIZONTAL);
    auto okButton = new wxButton(this, wxID_OK, "Close");
    okButton->SetDefault();
    okCancelSizer->Add(okButton, 0, wxALL, 5);
    mainSizer->Add(okCancelSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizerAndFit(mainSizer);

    // Bind events
    addButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnAdd, this);
    subtractButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnSubtract, this);
    resetButton->Bind(wxEVT_BUTTON, &EditProgressWindow::OnReset, this);
}

void EditProgressWindow::OnAdd(wxCommandEvent& event)
{
    if (wxMessageBox("Are you sure you want to add this duration to the current progress?", "Confirm Add", wxICON_QUESTION | wxYES_NO) == wxYES)
    {
        ApplyProgressChange(+1);
    }
}

void EditProgressWindow::OnSubtract(wxCommandEvent& event)
{
    if (wxMessageBox("Are you sure you want to subtract this duration from the current progress?", "Confirm Subtract", wxICON_QUESTION | wxYES_NO) == wxYES)
    {
        ApplyProgressChange(-1);
    }
}

void EditProgressWindow::OnReset(wxCommandEvent& event)
{
    if (wxMessageBox("Are you sure you want to reset the day's progress to 0h?", "Confirm Reset", wxICON_QUESTION | wxYES_NO) == wxYES)
    {
        ResetDayProgress();
    }
}

void EditProgressWindow::ApplyProgressChange(int sign)
{
    // TODO: Implement logic to add/subtract duration from current progress
}

void EditProgressWindow::ResetDayProgress()
{
    // TODO: Implement logic to reset the day's progress to 0h
}

} // namespace another_day