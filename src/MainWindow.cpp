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
		iconTea->SetToolTip("Session time");
		iconDoor->SetToolTip("Working time");

		m_progressBarSession = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));
		m_progressBarWork = new CustomProgressBar(this, wxID_ANY, 100, wxDefaultPosition, FromDIP(wxSize(85, 20)));

		firstRowSizer->Add(iconTea, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
		firstRowSizer->Add(m_progressBarSession, 0, wxALL, 0);

		secondRowSizer->Add(iconDoor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
		secondRowSizer->Add(m_progressBarWork, 0, wxALL, 0);

		mainSizer->Add(firstRowSizer, 0, wxALIGN_LEFT | wxALL, 0);
		mainSizer->AddSpacer(FromDIP(2));
		mainSizer->Add(secondRowSizer, 0, wxALIGN_LEFT | wxALL, 0);

		paddingSizer->Add(mainSizer, 1, wxEXPAND | wxALL, FromDIP(2));

		this->SetSizer(paddingSizer);
		this->Layout();
		this->Fit();

		setEvents(iconTea);
		setEvents(iconDoor);
		setEvents(m_progressBarSession);
		setEvents(m_progressBarWork);
		setEvents(this);

		wxTimer* timer = new wxTimer(this);
		timer->Start(1000); // clock refresh rate
		Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
		Bind(wxEVT_MENU, &MainWindow::OnHello, this, wxID_PRINT);
		Bind(wxEVT_MENU, &MainWindow::OnClose, this, wxID_CLOSE);
		Bind(wxEVT_MENU, &MainWindow::OnResetSession, this, ID_RESET_SESSION);

		sessionStartTime = std::chrono::steady_clock::now();
		workStartTime = std::chrono::steady_clock::now();

		m_progressBarSession->SetRange(std::chrono::duration_cast<std::chrono::seconds>(sessionDuration).count());
		m_progressBarWork->SetRange(std::chrono::duration_cast<std::chrono::seconds>(workDuration).count());

		UpdateProgressBarLabels();
		UpdateProgressBarValues();
	}

	void MainWindow::setEvents(wxEvtHandler* handler)
	{
		handler->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnLeftMouseDown, this);
		handler->Bind(wxEVT_LEFT_UP, &MainWindow::OnLeftMouseUp, this);
		handler->Bind(wxEVT_MOTION, &MainWindow::OnMouseMove, this);
		handler->Bind(wxEVT_RIGHT_DOWN, &MainWindow::OnRightMouseDown, this);
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

	void MainWindow::UpdateProgressBarValues()
	{
		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();

		auto sessionTime = now - sessionStartTime;
		auto workingTime = now - workStartTime;

		m_progressBarSession->SetValue(std::chrono::duration_cast<std::chrono::seconds>(sessionTime).count());
		m_progressBarWork->SetValue(std::chrono::duration_cast<std::chrono::seconds>(workingTime).count());

		if (m_progressBarSession->IsFilled())
		{
			m_progressBarSession->SetFilledColor(wxBrush(wxColor(0xFFA500)));
		}
	}

	void MainWindow::UpdateProgressBarLabels()
	{
		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();

		{
			auto sessionTime = now - sessionStartTime;
			auto sessionLeft = sessionDuration - sessionTime;
			int hoursLeft = std::chrono::duration_cast<std::chrono::hours>(sessionLeft).count();
			int minutesLeft = std::chrono::duration_cast<std::chrono::minutes>(sessionLeft).count() % 60;
			int secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(sessionLeft).count() % 60;
			wxString sessionLeftString = wxString::Format(wxT("%02d:%02d:%02d"), hoursLeft, minutesLeft, secondsLeft);
			m_progressBarSession->SetText(sessionLeftString);
		}

		{
			auto workingTime = now - workStartTime;
			auto workLeft = workDuration - workingTime;
			int hoursLeft = std::chrono::duration_cast<std::chrono::hours>(workLeft).count();
			int minutesLeft = std::chrono::duration_cast<std::chrono::minutes>(workLeft).count() % 60;
			int secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(workLeft).count() % 60;
			wxString workLeftString = wxString::Format(wxT("%02d:%02d:%02d"), hoursLeft, minutesLeft, secondsLeft);
			m_progressBarWork->SetText(workLeftString);
		}
	}

	void MainWindow::OnTimer(wxTimerEvent& event)
	{
		UpdateProgressBarLabels();
		UpdateProgressBarValues();
	}

	void MainWindow::OnRightMouseDown(wxMouseEvent& event)
	{
		wxPoint positionInWindow = this->ScreenToClient(wxGetMousePosition());

		wxMenu contextMenu;
		contextMenu.Append(ID_RESET_SESSION, wxT("Reset session"));
		contextMenu.AppendSeparator();
		contextMenu.Append(wxID_CLOSE, wxT("Close"));
		PopupMenu(&contextMenu, positionInWindow);
	}

	void MainWindow::OnHello(wxCommandEvent& event)
	{
		wxLogMessage("Hello world");
	}

	void MainWindow::OnClose(wxCommandEvent& event)
	{
		Close(true);
	}

	void MainWindow::OnResetSession(wxCommandEvent& event)
	{
		sessionStartTime = std::chrono::steady_clock::now();
		m_progressBarSession->SetFilledColor(*wxGREEN_BRUSH);
		UpdateProgressBarLabels();
		UpdateProgressBarValues();
	}

}