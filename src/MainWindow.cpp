#include "pch.hpp"

#include "MainWindow.hpp"
#include "SettingsWindow.hpp"
#include <cassert>

namespace another_day
{
MainWindow::MainWindow(Settings& settings, Database& database)
    : wxFrame(nullptr, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, settings.GetFrameStyle()), settings(settings),
      workLog(settings, database)
{
    wxWindowBase::SetBackgroundColour(*wxWHITE);

    wxImage::AddHandler(new wxPNGHandler());
    wxBitmap iconBitmapTea(wxT("cup-tea.png"), wxBITMAP_TYPE_PNG);
    wxBitmap iconBitmapDoor(wxT("door-open-out.png"), wxBITMAP_TYPE_PNG);

    wxSizer* paddingSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* firstRowSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer* secondRowSizer = new wxBoxSizer(wxHORIZONTAL);

    iconTea = new wxStaticBitmap(this, wxID_ANY, iconBitmapTea, wxDefaultPosition, FromDIP(wxSize(20, 20)));
    iconDoor = new wxStaticBitmap(this, wxID_ANY, iconBitmapDoor, wxDefaultPosition, FromDIP(wxSize(20, 20)));
    iconTea->SetToolTip("Session time");
    iconDoor->SetToolTip("Working time");

    progressBarSession = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
    progressBarWork = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));

    firstRowSizer->Add(iconTea, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    firstRowSizer->Add(progressBarSession, 0, wxALL, 0);

    secondRowSizer->Add(iconDoor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    secondRowSizer->Add(progressBarWork, 0, wxALL, 0);

    mainSizer->Add(firstRowSizer, 0, wxALIGN_LEFT | wxALL, 0);
    mainSizer->AddSpacer(FromDIP(2));
    mainSizer->Add(secondRowSizer, 0, wxALIGN_LEFT | wxALL, 0);

    paddingSizer->Add(mainSizer, 1, wxEXPAND | wxALL, FromDIP(2));

    this->SetSizer(paddingSizer);
    this->wxTopLevelWindowBase::Layout();
    this->wxWindowBase::Fit();

    setEvents(iconTea);
    setEvents(iconDoor);
    setEvents(progressBarSession);
    setEvents(progressBarWork);
    setEvents(this);

    timer = std::make_unique<wxTimer>(this);
    timer->Start(1001); // clock refresh rate
    Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    Bind(wxEVT_MENU, &MainWindow::OnHello, this, wxID_PRINT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainWindow::OnResetSession, this, ID_RESET_SESSION);
    Bind(wxEVT_MENU, &MainWindow::OnToggleHalt, this, ID_TOGGLE_HALT);
    Bind(wxEVT_MENU, &MainWindow::OnHideToTray, this, ID_HIDE_TO_TRAY);
    Bind(wxEVT_MENU, &MainWindow::OnHide, this, wxID_ICONIZE_FRAME);
    Bind(wxEVT_MENU, &MainWindow::OnStartBreak, this, ID_START_BREAK);
    Bind(wxEVT_MENU, &MainWindow::OnReveal, this, ID_REVEAL);
    Bind(wxEVT_MENU, &MainWindow::OnOpenSettings, this, wxID_PREFERENCES);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
    Bind(wxEVT_MENU, &MainWindow::OnDebugFinishSession, this, ID_DEBUG_FINISH_SESSION);
    Bind(wxEVT_MENU, &MainWindow::OnDebugFinishBreak, this, ID_DEBUG_FINISH_BREAK);
    Bind(wxEVT_MENU, &MainWindow::OnDebugFinishWork, this, ID_DEBUG_FINISH_WORK);

    if (not notificationSound.Create(wxT("notification.wav")))
    {
        wxLogError("Failed to load notification.wav");
    }

    sessionStartTime = Now();
    workStartTime = Now() - workdayProgress.Restore();

    workLog.BeginBreak();

    UpdateBars();
}

MainWindow::~MainWindow()
{
    workLog.End();
    SaveProgress();
}

void MainWindow::setEvents(wxEvtHandler* handler)
{
    handler->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnLeftMouseDown, this);
    handler->Bind(wxEVT_LEFT_UP, &MainWindow::OnLeftMouseUp, this);
    handler->Bind(wxEVT_MOTION, &MainWindow::OnMouseMove, this);
    handler->Bind(wxEVT_RIGHT_DOWN, &MainWindow::OnRightMouseDown, this);
}

void MainWindow::OnLeftMouseDown(wxMouseEvent& event)
{
    CaptureMouse();
    const wxWindow* win = dynamic_cast<wxWindow*>(event.GetEventObject());
    const wxPoint mouseOnScreenPos = win->ClientToScreen(event.GetPosition());
    const wxPoint windowOnScreenPos = GetPosition();
    int xOffset = mouseOnScreenPos.x - windowOnScreenPos.x;
    int yOffset = mouseOnScreenPos.y - windowOnScreenPos.y;
    dragStartOffset = {xOffset, yOffset};
}

void MainWindow::OnLeftMouseUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void MainWindow::OnMouseMove(wxMouseEvent& event)
{
    if (event.Dragging() && event.LeftIsDown())
    {
        const wxWindow* win = dynamic_cast<wxWindow*>(event.GetEventObject());
        wxPoint mouseOnScreenPos = win->ClientToScreen(event.GetPosition());
        Move(mouseOnScreenPos.x - dragStartOffset.x, mouseOnScreenPos.y - dragStartOffset.y);
    }
}

void MainWindow::UpdateProgressBarValue(CustomProgressBar* progressBar,
                                        std::chrono::steady_clock::time_point startPoint, Duration duration)
{
    auto timePassed = Now() - startPoint;
    progressBar->SetRange(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
    progressBar->SetValue(std::chrono::duration_cast<std::chrono::seconds>(timePassed).count());
}

void MainWindow::UpdateProgressBarText(CustomProgressBar* progressBar, std::chrono::steady_clock::time_point startPoint,
                                       Duration duration)
{
    auto timePassed = Now() - startPoint;
    auto timePassedInSeconds = std::chrono::duration_cast<std::chrono::seconds>(timePassed);
    auto timeLeft = duration - timePassedInSeconds;
    bool isOvertime = timeLeft.count() < 0;

    if (isOvertime)
    {
        timeLeft = -timeLeft;
    }

    int hours = std::chrono::duration_cast<std::chrono::hours>(timeLeft).count();
    int minutes = std::chrono::duration_cast<std::chrono::minutes>(timeLeft).count() % 60;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(timeLeft).count() % 60;
    auto overtimePrefix = isOvertime ? "+" : "";
    wxString text = wxString::Format(wxT("%s%02d:%02d:%02d"), overtimePrefix, hours, minutes, seconds);
    progressBar->SetText(text);
}

void MainWindow::UpdateBars()
{
    if (not breakInProgress)
    {
        UpdateProgressBarText(progressBarSession, sessionStartTime, settings.sessionDuration);
        UpdateProgressBarValue(progressBarSession, sessionStartTime, settings.sessionDuration);

        if (progressBarSession->IsFilled()) // session is over
        {
            PlayNotificationSound();
            progressBarSession->SetFilledColor(*wxRED_BRUSH);
            progressBarSession->SetTextColor(*wxWHITE);
            if (settings.autoStartBreak)
            {
                OnStartBreak(wxCommandEvent{});
            }
        }
    }
    else // break in progress
    {
        UpdateProgressBarText(progressBarSession, breakStartTime, settings.breakDuration);
        UpdateProgressBarValue(progressBarSession, breakStartTime, settings.breakDuration);

        if (progressBarSession->IsFilled()) // break is over
        {
            PlayNotificationSound();
            if (settings.autoStartSession)
            {
                OnResetSession(wxCommandEvent{});
            }
        }
    }

    UpdateProgressBarText(progressBarWork, workStartTime, settings.workDuration);
    UpdateProgressBarValue(progressBarWork, workStartTime, settings.workDuration);

    if (progressBarWork->IsFilled())
    {
        progressBarWork->SetFilledColor(*wxRED_BRUSH);
        progressBarWork->SetTextColor(*wxWHITE);
    }
}

void MainWindow::PlayNotificationSound()
{
    if (not lastNotificationTime.has_value() or
        Now() - lastNotificationTime.value() > settings.notificationInterval.value)
    {
        if (settings.useAudioNotification)
            if (not notificationSound.Play())
                wxLogDebug("Failed to play notification sound");

        lastNotificationTime = Now();
        Show(); // show the window if it's hidden in the tray
    }
}

void MainWindow::SaveProgress(Duration interval) noexcept
{
    if (workdayProgress.TimeSinceLastSave() < interval)
        return;
    if (not timer->IsRunning())
        return;

    Duration workdayDuration = std::chrono::duration_cast<Duration>(Now() - workStartTime);
    workdayProgress.Save(workdayDuration);
}

std::chrono::steady_clock::time_point MainWindow::Now() const
{
    return haltStartTime ? haltStartTime.value() : std::chrono::steady_clock::now();
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
    UpdateBars();
    SaveProgress(std::chrono::minutes{5}); // save progress every 5 minutes
}

void MainWindow::AddDebugOptions(wxMenu& contextMenu)
{
    contextMenu.AppendSeparator();
    contextMenu.Append(ID_DEBUG_FINISH_SESSION, "[DEBUG] Finish session");
    contextMenu.Append(ID_DEBUG_FINISH_BREAK, "[DEBUG] Finish break");
    contextMenu.Append(ID_DEBUG_FINISH_WORK, "[DEBUG] Finish work");
}

void MainWindow::OnDebugFinishSession(wxCommandEvent& event)
{
    auto newSessionStart = Now() - settings.sessionDuration.value + std::chrono::seconds{3};
    sessionStartTime = newSessionStart;
}

void MainWindow::OnDebugFinishBreak(wxCommandEvent& event)
{
    auto newBreakStart = Now() - settings.breakDuration.value + std::chrono::seconds{3};
    breakStartTime = newBreakStart;
}

void MainWindow::OnDebugFinishWork(wxCommandEvent& event)
{
    auto newWorkStart = Now() - settings.workDuration.value + std::chrono::seconds{3};
    workStartTime = newWorkStart;
}

void MainWindow::OnRightMouseDown(wxMouseEvent& event)
{
    wxPoint positionInWindow = this->ScreenToClient(wxGetMousePosition());

    wxMenu contextMenu;

#ifdef __WINDOWS__
    wxFont font = GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    wxMenuItem* startBreakItem = new wxMenuItem(&contextMenu, ID_START_BREAK, wxT("Start break"));
    startBreakItem->SetFont(font);
    contextMenu.Append(startBreakItem);
#else
    contextMenu.Append(ID_START_BREAK, wxT("Start break"));
#endif

    contextMenu.Append(ID_RESET_SESSION, wxT("Start session"));
    contextMenu.Append(ID_TOGGLE_HALT, timer->IsRunning() ? wxT("Halt") : wxT("Resume"));

#ifndef NDEBUG
    AddDebugOptions(contextMenu);
#endif

    contextMenu.AppendSeparator();
    if (wxTaskBarIcon::IsAvailable())
        contextMenu.Append(ID_HIDE_TO_TRAY, wxT("Hide (tray)"));
    contextMenu.Append(wxID_ICONIZE_FRAME, wxT("Hide (minimize)"));
    contextMenu.AppendSeparator();
    {
        wxMenuItem* settingsItem = contextMenu.Append(wxID_PREFERENCES, wxT("Settings"));
        settingsItem->Enable(true);
    }
    contextMenu.AppendSeparator();
    contextMenu.Append(wxID_EXIT, wxT("Exit"));
    PopupMenu(&contextMenu, positionInWindow);
}

void MainWindow::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world");
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close();
}

void MainWindow::OnClose(wxCloseEvent& event)
{
    if (event.CanVeto())
    {
        if (wxMessageBox("Are you sure you want to quit?", "Exit another-day", wxICON_QUESTION | wxYES_NO) != wxYES)
        {
            event.Veto();
            return;
        }
    }
    event.Skip(); // use the default handler
}

void MainWindow::OnHide(wxCommandEvent& event)
{
    Iconize(true);
}

void MainWindow::OnReveal(const wxCommandEvent& event)
{
    Show(true);
    Restore();
}

void MainWindow::OnStartBreak(const wxCommandEvent& event)
{
    workLog.BeginBreak();

    breakInProgress = true;
    breakStartTime = Now();
    progressBarSession->SetFilledColor(wxBrush(wxColor("#00A5FF")));
    progressBarSession->SetTextColor(*wxBLACK);
    lastNotificationTime.reset();

    UpdateBars();
}

void MainWindow::OnResetSession(const wxCommandEvent& event)
{
    workLog.BeginWork();

    breakInProgress = false;
    sessionStartTime = Now();
    progressBarSession->SetFilledColor(*wxGREEN_BRUSH);
    progressBarSession->SetTextColor(*wxBLACK);
    lastNotificationTime.reset();

    UpdateBars();
}

void MainWindow::OnToggleHalt(const wxCommandEvent& event)
{
    if (timer->IsRunning())
    {
        SaveProgress();
        haltStartTime = std::chrono::steady_clock::now();
        timer->Stop();
        progressBarSession->SetHatched(true);
        progressBarWork->SetHatched(true);
        UpdateBars();
    }
    else if (haltStartTime.has_value())
    {
        auto haltDuration = std::chrono::steady_clock::now() - haltStartTime.value();
        haltStartTime = std::nullopt;
        workStartTime += haltDuration;
        sessionStartTime += haltDuration;
        breakStartTime += haltDuration;
        progressBarSession->SetHatched(false);
        progressBarWork->SetHatched(false);
        timer->Start();
    }
}

void MainWindow::OnHideToTray(wxCommandEvent& event)
{
    if (not wxTaskBarIcon::IsAvailable())
        return;

    Hide();
    CreateTrayIcon();
}

void MainWindow::CreateTrayIcon()
{
    taskBarIcon = std::make_unique<wxTaskBarIcon>();
    wxIcon icon;
    icon.LoadFile("door-open-out.png", wxBITMAP_TYPE_PNG);
    taskBarIcon->SetIcon(icon, "Another Day");

    taskBarIcon->Bind(wxEVT_TASKBAR_LEFT_UP, [this](wxTaskBarIconEvent&) { this->OnReveal(wxCommandEvent{}); });

    taskBarIcon->Bind(wxEVT_TASKBAR_CLICK, [this](wxTaskBarIconEvent&) {
        wxMenu menu;
        menu.SetEventHandler(this);
        menu.Append(ID_REVEAL, "Reveal");
        menu.AppendSeparator();
        menu.Append(wxID_EXIT, "Exit");
        taskBarIcon->PopupMenu(&menu);
    });
}

void MainWindow::OnOpenSettings(wxCommandEvent& event)
{
    SettingsWindow settingsWindow(this, settings);
    int status = settingsWindow.ShowModal();
    if (status == wxID_OK)
    {
        UpdateBars();
        SetWindowStyle(settings.GetFrameStyle());
    }
}

} // namespace another_day