#pragma once
#include "SettingsWindow.hpp"

namespace another_day
{
SettingsWindow::SettingsWindow(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    {
        wxStaticBoxSizer* generalSizer = new wxStaticBoxSizer(wxVERTICAL, this, "General Settings");
        {
            autoStartBreakCheckBox = new wxCheckBox(this, wxID_ANY, "Auto-start break");
            autoStartBreakCheckBox->SetValue(false);
            autoStartBreakCheckBox->SetToolTip("Start the break automatically when the session ends");
            generalSizer->Add(autoStartBreakCheckBox, 0, wxALL, 5);

            autoStartSessionCheckBox = new wxCheckBox(this, wxID_ANY, "Auto-start session");
            autoStartSessionCheckBox->SetValue(true);
            autoStartSessionCheckBox->SetToolTip("Start the session automatically when the break ends");
            generalSizer->Add(autoStartSessionCheckBox, 0, wxALL, 5);
        }

        wxStaticBoxSizer* durationSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Duration Settings");
        {
            wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 0));
            {
                gridSizer->AddGrowableCol(1, 1);

                gridSizer->Add(new wxStaticText(this, wxID_ANY, "Break:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
                breakDurationCtrl = new wxTextCtrl(this, wxID_ANY);
                breakDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
                breakDurationCtrl->SetValue("5m");
                gridSizer->Add(breakDurationCtrl, 1, wxALL | wxEXPAND, 1);

                gridSizer->Add(new wxStaticText(this, wxID_ANY, "Session:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
                sessionDurationCtrl = new wxTextCtrl(this, wxID_ANY);
                sessionDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
                sessionDurationCtrl->SetValue("55m");
                gridSizer->Add(sessionDurationCtrl, 1, wxALL | wxEXPAND, 1);

                gridSizer->Add(new wxStaticText(this, wxID_ANY, "Work:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
                workDurationCtrl = new wxTextCtrl(this, wxID_ANY);
                workDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
                workDurationCtrl->SetValue("8h");
                gridSizer->Add(workDurationCtrl, 1, wxALL | wxEXPAND, 1);

                gridSizer->Add(new wxStaticText(this, wxID_ANY, "Postpone:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
                postponekDurationCtrl = new wxTextCtrl(this, wxID_ANY);
                postponekDurationCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
                gridSizer->Add(postponekDurationCtrl, 1, wxALL | wxEXPAND, 1);
            }

            durationSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
        }

        wxStaticBoxSizer* notificationSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Notification Settings");
        {
            useAudioNotificationCheckBox = new wxCheckBox(this, wxID_ANY, "Audio notification");
            useAudioNotificationCheckBox->SetValue(true);
            useAudioNotificationCheckBox->SetToolTip("Play a sound when the break or session ends");
            notificationSizer->Add(useAudioNotificationCheckBox, 0, wxALL, 5);

            useSystemNotificationCheckBox = new wxCheckBox(this, wxID_ANY, "System notification");
            useSystemNotificationCheckBox->SetValue(false);
            useSystemNotificationCheckBox->SetToolTip("Show a system notification when the break or session ends");
            useSystemNotificationCheckBox->Enable(false);
            notificationSizer->Add(useSystemNotificationCheckBox, 0, wxALL, 5);

            wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 0));
            {
                gridSizer->AddGrowableCol(1, 1);

                gridSizer->Add(new wxStaticText(this, wxID_ANY, "Notification interval:"), 0,
                               wxALL | wxALIGN_CENTER_VERTICAL, 5);
                wxTextCtrl* notificationIntervalCtrl = new wxTextCtrl(this, wxID_ANY);
                notificationIntervalCtrl->SetToolTip("Syntax: 7s, 7m, 7h");
                notificationIntervalCtrl->SetValue("60s");
                gridSizer->Add(notificationIntervalCtrl, 1, wxALL | wxEXPAND, 1);
            }
            notificationSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
        }

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        {
            auto applyButton = new wxButton(this, wxID_APPLY, "Apply");
            auto cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
            applyButton->Enable(false);

            buttonSizer->Add(applyButton, 0, wxALL, 5);
            buttonSizer->Add(cancelButton, 0, wxALL, 5);
        }

        mainSizer->Add(generalSizer, 0, wxEXPAND | wxALL, 5);
        mainSizer->Add(durationSizer, 0, wxEXPAND | wxALL, 5);
        mainSizer->Add(notificationSizer, 0, wxEXPAND | wxALL, 5);
        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    }

    SetSizerAndFit(mainSizer);
}
} // namespace another_day
