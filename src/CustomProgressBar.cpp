#include "pch.hpp"

#include "CustomProgressBar.hpp"

namespace another_day
{
CustomProgressBar::CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, id, pos, size), m_value(0), m_range(range)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &CustomProgressBar::OnPaint, this);
}

void CustomProgressBar::SetRange(int range)
{
    if (m_range == range)
        return;

    m_range = range;
    Refresh();
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

void CustomProgressBar::SetText(const wxString& text)
{
    m_text = text;
    Refresh();
}

wxString CustomProgressBar::GetText() const
{
    return m_text;
}

void CustomProgressBar::SetFilledColor(const wxBrush& brush)
{
    m_filledBrush = brush;
    Refresh();
}

void CustomProgressBar::SetEmptyColor(const wxBrush& brush)
{
    m_emptyBrush = brush;
    Refresh();
}

void CustomProgressBar::SetTextColor(const wxColor& color)
{
    m_textColor = color;
    Refresh();
}

bool CustomProgressBar::IsFilled()
{
    return m_value >= m_range;
}

void CustomProgressBar::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(m_emptyBrush);
    dc.Clear();

    wxSize size = GetClientSize();
    dc.SetPen(*wxMEDIUM_GREY_PEN);

    // Empty progress bar
    dc.SetBrush(m_emptyBrush);
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    // Filled progress bar
    dc.SetBrush(m_filledBrush);
    int width = (m_value * size.GetWidth()) / std::max(1, m_range);
    width = std::max(0, std::min(size.GetWidth(), width));
    dc.DrawRectangle(0, 0, width, size.GetHeight());

    // Text on top of progress bar
    dc.SetTextForeground(m_textColor);
    dc.DrawText(m_text, (size.GetWidth() - dc.GetTextExtent(m_text).GetWidth()) / 2,
                (size.GetHeight() - dc.GetTextExtent(m_text).GetHeight()) / 2);
}
} // namespace another_day