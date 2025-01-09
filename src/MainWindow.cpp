#include "pch.hpp"
#include "MainWindow.hpp"

namespace minutea
{

	MainWindow::MainWindow(const wxString& title)
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
		iconTea->SetToolTip("Time for break");
		iconDoor->SetToolTip("Time to go home");

		m_progressBarBreak = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
		m_progressBarEnd = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));

		wxString counterString = wxString::Format(wxT("break in %d"), counter);

		m_progressBarBreak->SetValue(50);
		m_progressBarEnd->SetValue(50);
		m_progressBarBreak->SetText(counterString);
		m_progressBarEnd->SetText("End");

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

		wxTimer* timer = new wxTimer(this);
		timer->Start(1000); // 1000 milliseconds = 1 second
		Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
	}

	void MainWindow::setDragEvents(wxEvtHandler* handler)
	{
		handler->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnLeftMouseDown, this);
		handler->Bind(wxEVT_LEFT_UP, &MainWindow::OnLeftMouseUp, this);
		handler->Bind(wxEVT_MOTION, &MainWindow::OnMouseMove, this);
	}

	void MainWindow::OnLeftMouseDown(wxMouseEvent& event)
	{
		m_dragging = true;
		m_dragStartPos = wxGetMousePosition();;
		CaptureMouse();
	}

	void MainWindow::OnLeftMouseUp(wxMouseEvent& event)
	{
		if (m_dragging)
		{
			m_dragging = false;
			ReleaseMouse();
		}
	}

	void MainWindow::OnMouseMove(wxMouseEvent& event)
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

	void MainWindow::OnTimer(wxTimerEvent& event)
	{
		counter++;
		wxString counterString = wxString::Format(wxT("break in %d"), counter);
		m_progressBarBreak->SetText(counterString);
		m_progressBarBreak->SetValue(50 + counter);
	}
}