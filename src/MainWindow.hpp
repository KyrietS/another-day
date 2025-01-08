#pragma once

constexpr auto FRAME_STYLE = wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP;
// constexpr auto FRAME_STYLE = wxDEFAULT_FRAME_STYLE;

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

			this->Fit();

			setDragEvents(iconTea);
			setDragEvents(iconDoor);
			setDragEvents(m_progressBarBreak);
			setDragEvents(m_progressBarEnd);
			setDragEvents(this);
		}

	private:
		bool m_dragging = false;
		wxPoint m_dragStartPos;
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
			m_dragStartPos = wxGetMousePosition();;
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
				wxPoint newMousePos = wxGetMousePosition();
				wxPoint dragDelta = newMousePos - m_dragStartPos;
				wxPoint newWindowPos = GetScreenPosition() + dragDelta;
				SetPosition(newWindowPos);

				m_dragStartPos = newMousePos;
			}
		}
	};

}