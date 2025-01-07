#pragma once

constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW;

namespace minutea
{
	class MainWindow : public wxFrame
	{
	public:
		MainWindow(const wxString& title)
			: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, FRAME_STYLE)
		{
			this->SetSize(FromDIP(120), FromDIP(50));
			this->SetBackgroundColour(*wxWHITE);

			wxImage::AddHandler(new wxPNGHandler());
			wxBitmap iconBitmapTea(wxT("cup-tea.png"), wxBITMAP_TYPE_PNG);
			wxBitmap iconBitmapDoor(wxT("door-open-out.png"), wxBITMAP_TYPE_PNG);


			iconTea = new wxStaticBitmap(this, wxID_ANY, iconBitmapTea, FromDIP(wxPoint(5, 5)), FromDIP(wxSize(20, 20)));
			iconDoor = new wxStaticBitmap(this, wxID_ANY, iconBitmapDoor, FromDIP(wxPoint(5, 25)), FromDIP(wxSize(20, 20)));

			m_progressBarBreak = new wxGauge(this, wxID_ANY, 100, FromDIP(wxPoint(30, 5)), FromDIP(wxSize(85, 20)));
			m_progressBarEnd = new wxGauge(this, wxID_ANY, 100, FromDIP(wxPoint(30, 25)), FromDIP(wxSize(85, 20)));
			m_progressBarBreak->SetValue(50);
			m_progressBarEnd->SetValue(50);

			sizer = new wxBoxSizer(wxVERTICAL);
			this->SetSizer(sizer);
			this->Layout();

			setDragEvents(this);
			setDragEvents(iconTea);
			setDragEvents(iconDoor);
			setDragEvents(m_progressBarBreak);
			setDragEvents(m_progressBarEnd);
		}

	private:
		bool m_dragging = false;
		wxPoint m_dragStartPos;
		wxBoxSizer* sizer;
		wxGauge* m_progressBarBreak;
		wxGauge* m_progressBarEnd;
		wxStaticBitmap* iconTea;
		wxStaticBitmap* iconDoor;

		void setDragEvents(wxEvtHandler* handler)
		{
			handler->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnLeftMouseDown, this);
			handler->Bind(wxEVT_LEFT_UP, &MainWindow::OnLeftMouseUp, this);
			handler->Bind(wxEVT_MOTION, &MainWindow::OnMouseMove, this);
		}

		void OnLeftMouseDown(wxMouseEvent& event)
		{
			m_dragging = true;
			wxPoint pos = event.GetPosition();

			// If child element, calculate position relative to MainWindow
			wxWindow* win = dynamic_cast<wxWindow*>(event.GetEventObject());
			if (win && win != this)
			{
				pos = win->ClientToScreen(pos);
				pos = ScreenToClient(pos);
			}
			m_dragStartPos = pos;
			CaptureMouse();
		}

		void OnLeftMouseUp(wxMouseEvent& event)
		{
			if (m_dragging)
			{
				m_dragging = false;
				ReleaseMouse();
			}
		}

		void OnMouseMove(wxMouseEvent& event)
		{
			if (m_dragging)
			{
				wxPoint pos = ClientToScreen(event.GetPosition());
				wxPoint origin = pos - m_dragStartPos;
				Move(origin);
			}
		}
	};

}