#pragma once
#include "CustomProgressBar.hpp"

constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP;
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
		CustomProgressBar* m_progressBarBreak;
		CustomProgressBar* m_progressBarEnd;
		wxStaticBitmap* iconTea;
		wxStaticBitmap* iconDoor;
		int counter = 0;

		void setEvents(wxEvtHandler* handler);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnLeftMouseUp(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnRightMouseDown(wxMouseEvent& event);
		void OnHello(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);
	};

}