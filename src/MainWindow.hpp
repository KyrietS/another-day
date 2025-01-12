#pragma once
#include "CustomProgressBar.hpp"
#include <chrono>
#include <optional>

namespace another_day
{
	constexpr auto FRAME_STYLE = wxSTAY_ON_TOP;
	//constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP;
	// constexpr auto FRAME_STYLE = wxDEFAULT_FRAME_STYLE;

	using Duration = std::common_type<std::chrono::seconds, std::common_type<std::chrono::minutes, std::chrono::hours>::type>::type;

	class MainWindow : public wxFrame
	{
	public:
		MainWindow(const wxString& title);

	private:
		bool m_dragging = false;
		wxPoint m_dragStartOffset;
		CustomProgressBar* m_progressBarSession;
		CustomProgressBar* m_progressBarWork;
		wxStaticBitmap* iconTea;
		wxStaticBitmap* iconDoor;
		Duration breakDuration { std::chrono::seconds{wxConfig::Get()->Read("BreakDuration", 10) } };
		Duration sessionDuration { std::chrono::seconds{ wxConfig::Get()->Read("SessionDuration", 5) } };
		Duration workDuration { std::chrono::hours{ wxConfig::Get()->Read("WorkDuration", 8) } };
		std::chrono::steady_clock::time_point breakStartTime;
		std::chrono::steady_clock::time_point sessionStartTime;
		std::chrono::steady_clock::time_point workStartTime;
		bool breakInProgress = false;

		std::optional<std::chrono::steady_clock::time_point> lastNotificationTime;
		Duration notificationInterval { std::chrono::seconds{wxConfig::Get()->Read("NotificationInterval", 60) } };
		wxSound notificationSound;

		void UpdateBars();
		void PlayNotificationSound();

		void setEvents(wxEvtHandler* handler);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnLeftMouseUp(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnRightMouseDown(wxMouseEvent& event);
		void OnHello(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);
		void OnHide(wxCommandEvent& event);
		void OnResetSession(const wxCommandEvent& event);
		void OnStartBreak(wxCommandEvent& event);

		enum Events
		{
			ID_RESET_SESSION = wxID_HIGHEST + 1,
			ID_START_BREAK
		};
	};

}