#include "pch.hpp"

#include "MainWindow.hpp"
#include "SettingsWindow.hpp"
#include <cassert>

namespace another_day
{
MainWindow::MainWindow(Settings& settings)
    : wxFrame(nullptr, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, settings.GetFrameStyle()), settings(settings)
{
    this->SetBackgroundColour(*wxWHITE);

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

    m_progressBarSession = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
    m_progressBarWork = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));

    firstRowSizer->Add(iconTea, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    firstRowSizer->Add(m_progressBarSession, 0, wxALL, 0);

    secondRowSizer->Add(iconDoor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    secondRowSizer->Add(m_progressBarWork, 0, wxALL, 0);

    mainSizer->Add(firstRowSizer, 0, wxALIGN_LEFT | wxALL, 0);
    mainSizer->AddSpacer(FromDIP(2));
    mainSizer->Add(secondRowSizer, 0, wxALIGN_LEFT | wxALL, 0);

    paddingSizer->Add(mainSizer, 1, wxEXPAND | wxALL, FromDIP(2));

    this->SetSizer(paddingSizer);
    this->Layout();
    this->Fit();

    setEvents(iconTea);
    setEvents(iconDoor);
    setEvents(m_progressBarSession);
    setEvents(m_progressBarWork);
    setEvents(this);

    timer = std::make_unique<wxTimer>(this);
    timer->Start(1001); // clock refresh rate
    Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    Bind(wxEVT_MENU, &MainWindow::OnHello, this, wxID_PRINT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainWindow::OnResetSession, this, ID_RESET_SESSION);
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

    sessionStartTime = std::chrono::steady_clock::now();
    workStartTime = std::chrono::steady_clock::now();

    UpdateBars();
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
    m_dragStartOffset = {xOffset, yOffset};
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
        Move(mouseOnScreenPos.x - m_dragStartOffset.x, mouseOnScreenPos.y - m_dragStartOffset.y);
    }
}

static void SetProgressBarValue(CustomProgressBar* progressBar, std::chrono::steady_clock::time_point startPoint,
                                Duration duration)
{
    auto timePassed = std::chrono::steady_clock::now() - startPoint;
    progressBar->SetRange(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
    progressBar->SetValue(std::chrono::duration_cast<std::chrono::seconds>(timePassed).count());
}

void SetProgressBarText(CustomProgressBar* progressBar, std::chrono::steady_clock::time_point startPoint,
                        Duration duration)
{
    auto timePassed = std::chrono::steady_clock::now() - startPoint;
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
        SetProgressBarText(m_progressBarSession, sessionStartTime, settings.sessionDuration);
        SetProgressBarValue(m_progressBarSession, sessionStartTime, settings.sessionDuration);

        if (m_progressBarSession->IsFilled()) // session is over
        {
            PlayNotificationSound();
            m_progressBarSession->SetFilledColor(*wxRED_BRUSH);
            m_progressBarSession->SetTextColor(*wxWHITE);
            if (settings.autoStartBreak)
            {
                OnStartBreak(wxCommandEvent{});
            }
        }
    }
    else // break in progress
    {
        SetProgressBarText(m_progressBarSession, breakStartTime, settings.breakDuration);
        SetProgressBarValue(m_progressBarSession, breakStartTime, settings.breakDuration);

        if (m_progressBarSession->IsFilled()) // break is over
        {
            PlayNotificationSound();
            if (settings.autoStartSession)
            {
                OnResetSession(wxCommandEvent{});
            }
        }
    }

    SetProgressBarText(m_progressBarWork, workStartTime, settings.workDuration);
    SetProgressBarValue(m_progressBarWork, workStartTime, settings.workDuration);

    if (m_progressBarWork->IsFilled())
    {
        m_progressBarWork->SetFilledColor(*wxRED_BRUSH);
        m_progressBarWork->SetTextColor(*wxWHITE);
    }
}

void MainWindow::PlayNotificationSound()
{
    if (not lastNotificationTime.has_value() or
        std::chrono::steady_clock::now() - lastNotificationTime.value() > settings.notificationInterval)
    {
        if (settings.useAudioNotification)
            notificationSound.Play();

        lastNotificationTime = std::chrono::steady_clock::now();
        Show(); // show the window if it's hidden in the tray
    }
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
    UpdateBars();
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
    auto now = std::chrono::steady_clock::now();
    auto newSessionStart = now - settings.sessionDuration + std::chrono::seconds{3};
    sessionStartTime = newSessionStart;
}

void MainWindow::OnDebugFinishBreak(wxCommandEvent& event)
{
    auto now = std::chrono::steady_clock::now();
    auto newBreakStart = now - settings.breakDuration + std::chrono::seconds{3};
    breakStartTime = newBreakStart;
}

void MainWindow::OnDebugFinishWork(wxCommandEvent& event)
{
    auto now = std::chrono::steady_clock::now();
    auto newWorkStart = now - settings.workDuration + std::chrono::seconds{3};
    workStartTime = newWorkStart;
}

void MainWindow::OnRightMouseDown(wxMouseEvent& event)
{
    wxPoint positionInWindow = this->ScreenToClient(wxGetMousePosition());

    wxMenu contextMenu;

#ifdef __WINDOWS__
    wxFont font = GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    wxMenuItem* item = new wxMenuItem(&contextMenu, ID_START_BREAK, wxT("Start break"));
    item->SetFont(font);
    contextMenu.Append(item);
#else
    contextMenu.Append(ID_START_BREAK, wxT("Start break"));
#endif

    contextMenu.Append(ID_RESET_SESSION, wxT("Start session"));
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
        if (wxMessageBox("Are you sure you want to quit?\nAll progress will be lost.", "Exit another-day",
                         wxICON_WARNING | wxYES_NO) != wxYES)
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
    breakInProgress = true;
    breakStartTime = std::chrono::steady_clock::now();
    m_progressBarSession->SetFilledColor(wxBrush(wxColor("#00A5FF")));
    m_progressBarSession->SetTextColor(*wxBLACK);
    lastNotificationTime.reset();

    UpdateBars();
}

void MainWindow::OnResetSession(const wxCommandEvent& event)
{
    breakInProgress = false;
    sessionStartTime = std::chrono::steady_clock::now();
    m_progressBarSession->SetFilledColor(*wxGREEN_BRUSH);
    m_progressBarSession->SetTextColor(*wxBLACK);
    lastNotificationTime.reset();

    UpdateBars();
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
    m_taskBarIcon = std::make_unique<wxTaskBarIcon>();
    wxIcon icon;
    icon.LoadFile("door-open-out.png", wxBITMAP_TYPE_PNG);
    m_taskBarIcon->SetIcon(icon, "Another Day");

    m_taskBarIcon->Bind(wxEVT_TASKBAR_LEFT_UP, [this](wxTaskBarIconEvent&) { this->OnReveal(wxCommandEvent{}); });

    m_taskBarIcon->Bind(wxEVT_TASKBAR_CLICK, [this](wxTaskBarIconEvent&) {
        wxMenu menu;
        menu.SetEventHandler(this);
        menu.Append(ID_REVEAL, "Reveal");
        menu.AppendSeparator();
        menu.Append(wxID_EXIT, "Exit");
        m_taskBarIcon->PopupMenu(&menu);
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