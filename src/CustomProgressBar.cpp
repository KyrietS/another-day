#include "CustomProgressBar.hpp"

CustomProgressBar::CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, id, pos, size), m_value(0), m_range(range)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &CustomProgressBar::OnPaint, this);
}

void CustomProgressBar::SetValue(int value)
{
    m_value = value;
    Refresh();
}

int CustomProgressBar::GetValue() const
{
    return m_value;
}

void CustomProgressBar::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize size = GetClientSize();
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);

    int width = (m_value * size.GetWidth()) / m_range;
    dc.DrawRectangle(0, 0, width, size.GetHeight());
}
