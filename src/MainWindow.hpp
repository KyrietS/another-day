#pragma once
#include "CustomProgressBar.hpp"
#include "Duration.hpp"
#include "Settings.hpp"
#include "WorkLog.hpp"
#include "WorkdayProgress.hpp"
#include <chrono>
#include <optional>

namespace another_day
{
constexpr auto FRAME_STYLE = wxSTAY_ON_TOP;

class MainWindow : public wxFrame
{
public:
    MainWindow(Settings&, Database&);
    ~MainWindow() override;

private:
    Settings& settings;
    WorkLog workLog;
    WorkdayProgress workdayProgress;

    bool dragging = false;
    wxPoint dragStartOffset;
    CustomProgressBar* progressBarSession;
    CustomProgressBar* progressBarWork;
    wxStaticBitmap* iconTea;
    wxStaticBitmap* iconDoor;
    std::unique_ptr<wxTimer> timer;
    std::chrono::steady_clock::time_point breakStartTime;
    std::chrono::steady_clock::time_point sessionStartTime;
    std::chrono::steady_clock::time_point workStartTime;
    std::optional<std::chrono::steady_clock::time_point> haltStartTime;
    bool breakInProgress = false;

    std::optional<std::chrono::steady_clock::time_point> lastNotificationTime;
    wxSound notificationSound;

    std::unique_ptr<wxTaskBarIcon> taskBarIcon;
    void CreateTrayIcon();

    void UpdateBars();
    void UpdateProgressBarValue(CustomProgressBar*, std::chrono::steady_clock::time_point, Duration);
    void UpdateProgressBarText(CustomProgressBar*, std::chrono::steady_clock::time_point, Duration);

    void PlayNotificationSound();
    void SaveProgress(Duration interval = {}) noexcept;

    void setEvents(wxEvtHandler* handler);
    void OnLeftMouseDown(wxMouseEvent& event);
    void OnLeftMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnRightMouseDown(wxMouseEvent& event);
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnHideToTray(wxCommandEvent& event);
    void OnHide(wxCommandEvent& event);
    void OnReveal(const wxCommandEvent& event);
    void OnStartBreak(const wxCommandEvent& event);
    void OnResetSession(const wxCommandEvent& event);
    void OnToggleHalt(const wxCommandEvent& event);
    void OnOpenSettings(wxCommandEvent& event);

    std::chrono::steady_clock::time_point Now() const;

    // --- DEBUG ---
    void AddDebugOptions(wxMenu&);
    void OnDebugFinishSession(wxCommandEvent& event);
    void OnDebugFinishBreak(wxCommandEvent& event);
    void OnDebugFinishWork(wxCommandEvent& event);

    enum Events
    {
        ID_RESET_SESSION = wxID_HIGHEST + 1,
        ID_START_BREAK,
        ID_TOGGLE_HALT,
        ID_HIDE_TO_TRAY,
        ID_REVEAL,

        ID_DEBUG_FINISH_SESSION,
        ID_DEBUG_FINISH_BREAK,
        ID_DEBUG_FINISH_WORK
    };
};

} // namespace another_day