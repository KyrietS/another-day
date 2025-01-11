#pragma once
#include "CustomProgressBar.hpp"
#include <chrono>

constexpr auto FRAME_STYLE = wxSTAY_ON_TOP;
//constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP;
// constexpr auto FRAME_STYLE = wxDEFAULT_FRAME_STYLE;

namespace minutea
{
	class MainWindow : public wxFrame
	{
	public:
		MainWindow(const wxString& title);

	private:
		bool m_dragging = false;
		wxPoint m_dragStartPos;
		CustomProgressBar* m_progressBarSession;
		CustomProgressBar* m_progressBarWork;
		wxStaticBitmap* iconTea;
		wxStaticBitmap* iconDoor;
		std::chrono::minutes sessionDuration = std::chrono::minutes(1);
		std::chrono::hours workDuration = std::chrono::hours(8);
		std::chrono::time_point<std::chrono::steady_clock> sessionStartTime;
		std::chrono::time_point<std::chrono::steady_clock> workStartTime;

		void UpdateProgressBarValues();
		void UpdateProgressBarLabels();

		void setEvents(wxEvtHandler* handler);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnLeftMouseUp(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnRightMouseDown(wxMouseEvent& event);
		void OnHello(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);
		void OnHide(wxCommandEvent& event);
		void OnResetSession(wxCommandEvent& event);

		enum Events
		{
			ID_RESET_SESSION = wxID_HIGHEST + 1
		};
	};

}