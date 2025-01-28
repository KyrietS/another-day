#include "pch.hpp"

#include "DurationValidator.hpp"
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
        autoStartBreakCheckBox->SetValidator(wxGenericValidator(&settings.autoStartBreak));
        autoStartBreakCheckBox->SetToolTip("Start the break automatically when the session ends");
        generalSizer->Add(autoStartBreakCheckBox, 0, wxALL, 5);

        autoStartSessionCheckBox = new wxCheckBox(generalPanel, wxID_ANY, "Auto-start session");
        autoStartSessionCheckBox->SetValidator(wxGenericValidator(&settings.autoStartSession));
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
            breakDurationCtrl->SetValidator(DurationValidator(settings.breakDuration));
            breakDurationCtrl->SetToolTip("Number of seconds");
            gridSizer->Add(breakDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Session:"), 0, wxALL | wxALIGN_CENTER_VERTICAL,
                           5);
            sessionDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            sessionDurationCtrl->SetValidator(DurationValidator(settings.sessionDuration));
            sessionDurationCtrl->SetToolTip("Number of seconds");
            gridSizer->Add(sessionDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Work:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
            workDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            workDurationCtrl->SetValidator(DurationValidator(settings.workDuration));
            workDurationCtrl->SetToolTip("Number of seconds");
            gridSizer->Add(workDurationCtrl, 1, wxALL | wxEXPAND, 1);

            gridSizer->Add(new wxStaticText(durationPanel, wxID_ANY, "Postpone:"), 0, wxALL | wxALIGN_CENTER_VERTICAL,
                           5);
            postponeDurationCtrl = new wxTextCtrl(durationPanel, wxID_ANY);
            postponeDurationCtrl->SetValidator(DurationValidator(settings.postponeDuration));
            postponeDurationCtrl->SetToolTip("Number of second");
            postponeDurationCtrl->Enable(false);
            gridSizer->Add(postponeDurationCtrl, 1, wxALL | wxEXPAND, 1);
        }
        durationSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
    }
    durationPanel->SetSizerAndFit(durationSizer);
    notebook->AddPage(durationPanel, "Duration");

    wxPanel* notificationPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* notificationSizer = new wxBoxSizer(wxVERTICAL);
    {
        useAudioNotificationCheckBox = new wxCheckBox(notificationPanel, wxID_ANY, "Audio notification");
        useAudioNotificationCheckBox->SetValidator(wxGenericValidator(&settings.useAudioNotification));
        useAudioNotificationCheckBox->SetToolTip("Play a sound when the break or session ends");
        notificationSizer->Add(useAudioNotificationCheckBox, 0, wxALL, 5);

        useSystemNotificationCheckBox = new wxCheckBox(notificationPanel, wxID_ANY, "System notification");
        useSystemNotificationCheckBox->SetValidator(wxGenericValidator(&settings.useSystemNotification));
        useSystemNotificationCheckBox->SetToolTip("Show a system notification when the break or session ends");
        useSystemNotificationCheckBox->Enable(false);
        notificationSizer->Add(useSystemNotificationCheckBox, 0, wxALL, 5);

        wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 0));
        {
            gridSizer->AddGrowableCol(1, 1);

            gridSizer->Add(new wxStaticText(notificationPanel, wxID_ANY, "Notification interval:"), 0,
                           wxALL | wxALIGN_CENTER_VERTICAL, 5);
            wxTextCtrl* notificationIntervalCtrl = new wxTextCtrl(notificationPanel, wxID_ANY);
            notificationIntervalCtrl->SetValidator(DurationValidator(settings.notificationInterval));
            notificationIntervalCtrl->SetToolTip("Number of second");
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
        applyButton->SetDefault();

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
