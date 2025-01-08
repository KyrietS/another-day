#pragma once
#include "CustomProgressBar.hpp"

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

			m_progressBarBreak = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
			m_progressBarEnd = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
			m_progressBarBreak->SetValue(50);
			m_progressBarEnd->SetValue(50);

			firstRowSizer->Add(iconTea, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
			firstRowSizer->Add(m_progressBarBreak, 0, wxALL, 0);

			secondRowSizer->Add(iconDoor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
			secondRowSizer->Add(m_progressBarEnd, 0, wxALL, 0);

			mainSizer->Add(firstRowSizer, 0, wxALIGN_LEFT | wxALL, 0);
			mainSizer->AddSpacer(FromDIP(2));
			mainSizer->Add(secondRowSizer, 0, wxALIGN_LEFT | wxALL, 0);

			paddingSizer->Add(mainSizer, 1, wxEXPAND | wxALL, FromDIP(2));

			this->SetSizer(paddingSizer);
			this->Layout();
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
		CustomProgressBar* m_progressBarBreak;
		CustomProgressBar* m_progressBarEnd;
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