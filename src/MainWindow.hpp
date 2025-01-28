#pragma once
#include "CustomProgressBar.hpp"
#include "Duration.hpp"
#include "Settings.hpp"
#include <chrono>
#include <optional>

namespace another_day
{
constexpr auto FRAME_STYLE = wxSTAY_ON_TOP;
// constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP;
//  constexpr auto FRAME_STYLE = wxDEFAULT_FRAME_STYLE;

class MainWindow : public wxFrame
{
public:
    MainWindow(Settings&);

private:
    Settings& settings;

    bool m_dragging = false;
    wxPoint m_dragStartOffset;
    CustomProgressBar* m_progressBarSession;
    CustomProgressBar* m_progressBarWork;
    wxStaticBitmap* iconTea;
    wxStaticBitmap* iconDoor;
    std::unique_ptr<wxTimer> timer;
    std::chrono::steady_clock::time_point breakStartTime;
    std::chrono::steady_clock::time_point sessionStartTime;
    std::chrono::steady_clock::time_point workStartTime;
    bool breakInProgress = false;

    std::optional<std::chrono::steady_clock::time_point> lastNotificationTime;
    Duration notificationInterval{std::chrono::seconds{wxConfig::Get()->ReadLongLong("NotificationInterval", 60)}};
    wxSound notificationSound;

    std::unique_ptr<wxTaskBarIcon> m_taskBarIcon;
    void CreateTrayIcon();

    void UpdateBars();
    void PlayNotificationSound();

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
    void OnResetSession(const wxCommandEvent& event);
    void OnStartBreak(wxCommandEvent& event);
    void OnOpenSettings(wxCommandEvent& event);

    // --- DEBUG ---
    void AddDebugOptions(wxMenu&);
    void OnDebugFinishSession(wxCommandEvent& event);
    void OnDebugFinishBreak(wxCommandEvent& event);
    void OnDebugFinishWork(wxCommandEvent& event);

    enum Events
    {
        ID_RESET_SESSION = wxID_HIGHEST + 1,
        ID_START_BREAK,
        ID_HIDE_TO_TRAY,
        ID_REVEAL,

        ID_DEBUG_FINISH_SESSION,
        ID_DEBUG_FINISH_BREAK,
        ID_DEBUG_FINISH_WORK
    };
};

} // namespace another_day