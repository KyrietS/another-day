#include "pch.hpp"

#include "SettingsWindow.hpp"

namespace another_day
{
SettingsWindow::SettingsWindow(wxWindow* parent, Settings& settings)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize), settings(settings)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    wxPanel* generalPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* generalSizer = new wxBoxSizer(wxVERTICAL);
    {
        alwaysOnTopCheckBox = new wxCheckBox(generalPanel, wxID_ANY, "Always on top");
        alwaysOnTopCheckBox->SetValidator(wxGenericValidator(&settings.alwaysOnTop));
        alwaysOnTopCheckBox->SetToolTip("Keep the window on top of other windows");
        generalSizer->Add(alwaysOnTopCheckBox, 0, wxALL, 5);

        autoStartBreakCheckBox = new wxCheckBox(generalPanel, wxID_ANY, "Auto-start break");
        autoStartBreakCheckBox->SetValue(false);
        autoStartBreakCheckBox->SetToolTip("Start the break automatically when the session ends");
        generalSizer->Add(autoStartBreakCheckBox, 0, wxALL, 5);

        autoStartSessionCheckBox = new wxCheckBox(generalPanel, wxID_ANY, "Auto-start session");
        autoStartSessionCheckBox->SetValue(true);
        autoStartSessionCheckBox->SetToolTip("Start the session automatically when the break ends");
        generalSizer->Add(autoStartSessionCheckBox, 0, wxALL, 5);
    }
    generalPanel->SetSizerAndFit(generalSizer);
    notebook->AddPage(generalPanel, "General");

    wxPanel* durationPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* durationSizer = new wxBoxSizer(wxVERTICAL);
    {
        wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 0));
        {
            gridSizer->AddGrowableCol(1, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Break:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
            breakDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            breakDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
            breakDurationCtrl->SetValue("5m");
            gridSizer->Add(breakDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Session:"), 0, wxALL | wxALIGN_CENTER_VERTICAL,
                           5);
            sessionDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            sessionDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
            sessionDurationCtrl->SetValue("55m");
            gridSizer->Add(sessionDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Work:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
            workDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            workDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
            workDurationCtrl->SetValue("8h");
            gridSizer->Add(workDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Postpone:"), 0, wxALL | wxALIGN_CENTER_VERTICAL,
                           5);
            postponekDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            postponekDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
            gridSizer->Add(postponekDurationCtrl, 1, wxALL | wxEXPAND, 1);
        }
        durationSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
    }
    durationPanel->SetSizerAndFit(durationSizer);
    notebook->AddPage(durationPanel, "Duration");

    wxPanel* notificationPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* notificationSizer = new wxBoxSizer(wxVERTICAL);
    {
        useAudioNotificationCheckBox = new wxCheckBox(notificationPanel, wxID_ANY, "Audio notification");
        useAudioNotificationCheckBox->SetValue(true);
        useAudioNotificationCheckBox->SetToolTip("Play a sound when the break or session ends");
        notificationSizer->Add(useAudioNotificationCheckBox, 0, wxALL, 5);

        useSystemNotificationCheckBox = new wxCheckBox(notificationPanel, wxID_ANY, "System notification");
        useSystemNotificationCheckBox->SetValue(false);
        useSystemNotificationCheckBox->SetToolTip("Show a system notification when the break or session ends");
        useSystemNotificationCheckBox->Enable(false);
        notificationSizer->Add(useSystemNotificationCheckBox, 0, wxALL, 5);

        wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 0));
        {
            gridSizer->AddGrowableCol(1, 1);

            gridSizer->Add(new wxStaticText(notificationPanel, wxID_ANY, "Notification interval:"), 0,
                           wxALL | wxALIGN_CENTER_VERTICAL, 5);
            wxTextCtrl* notificationIntervalCtrl = new wxTextCtrl(notificationPanel, wxID_ANY);
            notificationIntervalCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
            notificationIntervalCtrl->SetValue("60s");
            gridSizer->Add(notificationIntervalCtrl, 1, wxALL | wxEXPAND, 1);
        }
        notificationSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
    }
    notificationPanel->SetSizerAndFit(notificationSizer);
    notebook->AddPage(notificationPanel, "Notification");

    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    {
        // wxWidgets will automatically bind these events and handle the dialog closing
        auto applyButton = new wxButton(this, wxID_OK, "Apply");
        auto cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

        buttonSizer->Add(applyButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);
    }

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

    SetSizerAndFit(mainSizer);
}

void SettingsWindow::EndModal(int code)
{
    if (code == wxID_OK and Validate())
    {
        settings.SaveToConfig(*wxConfig::Get());
    }

    wxDialog::EndModal(code);
}

} // namespace another_day
