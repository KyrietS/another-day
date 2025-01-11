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
		std::chrono::seconds breakDuration { 10 };
		std::chrono::seconds sessionDuration { 5 };
		std::chrono::hours workDuration { 8 };
		std::chrono::steady_clock::time_point breakStartTime;
		std::chrono::steady_clock::time_point sessionStartTime;
		std::chrono::steady_clock::time_point workStartTime;
		bool breakInProgress = false;

		void UpdateBars();

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