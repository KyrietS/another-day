#pragma once

class CustomProgressBar : public wxPanel
{
public:
    CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void SetValue(int value);
    int GetValue() const;

private:
    void OnPaint(wxPaintEvent& event);

    int m_value;
    int m_range;
};
