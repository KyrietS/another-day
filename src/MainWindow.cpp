#include "pch.hpp"
#include "MainWindow.hpp"

namespace another_day
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
		timer->Start(1001); // clock refresh rate
		Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
		Bind(wxEVT_MENU, &MainWindow::OnHello, this, wxID_PRINT);
		Bind(wxEVT_MENU, &MainWindow::OnClose, this, wxID_CLOSE);
		Bind(wxEVT_MENU, &MainWindow::OnResetSession, this, ID_RESET_SESSION);
		Bind(wxEVT_MENU, &MainWindow::OnHide, this, wxID_ICONIZE_FRAME);
		Bind(wxEVT_MENU, &MainWindow::OnStartBreak, this, ID_START_BREAK);

		if (not notificationSound.Create(wxT("notification.wav")))
		{
			wxLogError("Failed to load notification.wav");
		}

		sessionStartTime = std::chrono::steady_clock::now();
		workStartTime = std::chrono::steady_clock::now();

		m_progressBarSession->SetRange(std::chrono::duration_cast<std::chrono::seconds>(sessionDuration).count());
		m_progressBarWork->SetRange(std::chrono::duration_cast<std::chrono::seconds>(workDuration).count());

		UpdateBars();
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
		CaptureMouse();
		const wxWindow* win = dynamic_cast<wxWindow*>(event.GetEventObject());
		const wxPoint mouseOnScreenPos = win->ClientToScreen(event.GetPosition());
		const wxPoint windowOnScreenPos = GetPosition();
		int xOffset = mouseOnScreenPos.x - windowOnScreenPos.x;
		int yOffset = mouseOnScreenPos.y - windowOnScreenPos.y;
		m_dragStartOffset = { xOffset, yOffset };
	}

	void MainWindow::OnLeftMouseUp(wxMouseEvent& event)
	{
		if (HasCapture())
		{
			ReleaseMouse();
		}
	}

	void MainWindow::OnMouseMove(wxMouseEvent& event)
	{
		if (event.Dragging() && event.LeftIsDown())
		{
			const wxWindow* win = dynamic_cast<wxWindow*>(event.GetEventObject());
			wxPoint mouseOnScreenPos = win->ClientToScreen(event.GetPosition());
			Move(mouseOnScreenPos.x - m_dragStartOffset.x, mouseOnScreenPos.y - m_dragStartOffset.y);
		}
	}

	static void SetProgressBarValue(CustomProgressBar* progressBar, std::chrono::steady_clock::time_point startPoint)
	{
		auto timePassed = std::chrono::steady_clock::now() - startPoint;
		progressBar->SetValue(std::chrono::duration_cast<std::chrono::seconds>(timePassed).count());
	}

	template<typename Duration>
	void SetProgressBarText(CustomProgressBar* progressBar, std::chrono::steady_clock::time_point startPoint, Duration duration)
	{
		auto timePassed = std::chrono::steady_clock::now() - startPoint;
		auto timePassedInSeconds = std::chrono::duration_cast<std::chrono::seconds>(timePassed);
		auto timeLeft = duration - timePassedInSeconds;

		if (timeLeft.count() < 0)
		{
			timeLeft = {};
		}

		int hours = std::chrono::duration_cast<std::chrono::hours>(timeLeft).count();
		int minutes = std::chrono::duration_cast<std::chrono::minutes>(timeLeft).count() % 60;
		int seconds = std::chrono::duration_cast<std::chrono::seconds>(timeLeft).count() % 60;
		wxString text = wxString::Format(wxT("%02d:%02d:%02d"), hours, minutes, seconds);
		progressBar->SetText(text);
	}

	void MainWindow::UpdateBars()
	{
		if (not breakInProgress)
		{
			SetProgressBarText(m_progressBarSession, sessionStartTime, sessionDuration);
			SetProgressBarValue(m_progressBarSession, sessionStartTime);

			if (m_progressBarSession->IsFilled())
			{
				PlayNotificationSound();
				m_progressBarSession->SetFilledColor(*wxRED_BRUSH);
			}
		}
		else // break in progress
		{
			SetProgressBarText(m_progressBarSession, breakStartTime, breakDuration);
			SetProgressBarValue(m_progressBarSession, breakStartTime);

			if (m_progressBarSession->IsFilled())
			{
				PlayNotificationSound();
				OnResetSession(wxCommandEvent {});
			}
		}

		SetProgressBarText(m_progressBarWork, workStartTime, workDuration);
		SetProgressBarValue(m_progressBarWork, workStartTime);

		if (m_progressBarWork->IsFilled())
			m_progressBarWork->SetFilledColor(*wxRED_BRUSH);
	}

	void MainWindow::PlayNotificationSound()
	{
		if (not lastNotificationTime.has_value() or 
			std::chrono::steady_clock::now() - lastNotificationTime.value() > notificationInterval)
		{
			notificationSound.Play();
			lastNotificationTime = std::chrono::steady_clock::now();
		}
	}

	void MainWindow::OnTimer(wxTimerEvent& event)
	{
		UpdateBars();
	}

	void MainWindow::OnRightMouseDown(wxMouseEvent& event)
	{
		wxPoint positionInWindow = this->ScreenToClient(wxGetMousePosition());

		wxMenu contextMenu;

#ifdef __WINDOWS__
		wxFont font = GetFont();
		font.SetWeight(wxFONTWEIGHT_BOLD);
		wxMenuItem* item = new wxMenuItem(&contextMenu, ID_START_BREAK, wxT("Start break"));
		item->SetFont(font);
		contextMenu.Append(item);
#else
		contextMenu.Append(ID_START_BREAK, wxT("Start break"));
#endif

		contextMenu.Append(ID_RESET_SESSION, wxT("Reset session"));
		contextMenu.Append(wxID_ICONIZE_FRAME, wxT("Hide"));
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

	void MainWindow::OnHide(wxCommandEvent& event)
	{
		Iconize(true);
	}

	void MainWindow::OnStartBreak(wxCommandEvent& event)
	{
		breakInProgress = true;
		breakStartTime = std::chrono::steady_clock::now();
		m_progressBarSession->SetFilledColor(wxBrush(wxColor("#00A5FF")));
		m_progressBarSession->SetRange(std::chrono::duration_cast<std::chrono::seconds>(breakDuration).count());
		lastNotificationTime.reset();

		UpdateBars();
	}

	void MainWindow::OnResetSession(const wxCommandEvent& event)
	{
		breakInProgress = false;
		sessionStartTime = std::chrono::steady_clock::now();
		m_progressBarSession->SetFilledColor(*wxGREEN_BRUSH);
		m_progressBarSession->SetRange(std::chrono::duration_cast<std::chrono::seconds>(sessionDuration).count());
		lastNotificationTime.reset();

		UpdateBars();
	}

}