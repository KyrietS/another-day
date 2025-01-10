#pragma once

class CustomProgressBar : public wxPanel
{
public:
    CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	void SetRange(int range);
    void SetValue(int value);
    int GetValue() const;

	void SetText(const wxString& text);
	wxString GetText() const;

    void SetFilledColor(const wxBrush& brush);
	void SetEmptyColor(const wxBrush& brush);
	void SetTextColor(const wxColor& color);

private:
    void OnPaint(wxPaintEvent& event);

    int m_value;
    int m_range;
	wxString m_text;

	wxBrush m_filledBrush = *wxGREEN_BRUSH;
	wxBrush m_emptyBrush = *wxLIGHT_GREY_BRUSH;
	wxColor m_textColor = *wxBLACK;
};
