#include "pch.hpp"

#include "CustomProgressBar.hpp"

namespace another_day
{
CustomProgressBar::CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, id, pos, size), value(0), range(range)
{
    wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &CustomProgressBar::CustomOnPaint, this);
}

void CustomProgressBar::SetRange(int newRange)
{
    if (newRange == range)
        return;

    range = newRange;
    Refresh();
}

void CustomProgressBar::SetValue(int newValue)
{
    value = newValue;
    Refresh();
}

int CustomProgressBar::GetValue() const
{
    return value;
}

void CustomProgressBar::SetText(const wxString& newText)
{
    text = newText;
    Refresh();
}

wxString CustomProgressBar::GetText() const
{
    return text;
}

void CustomProgressBar::SetFilledColor(const wxBrush& brush)
{
    filledBrush = brush;
    Refresh();
}

void CustomProgressBar::SetEmptyColor(const wxBrush& brush)
{
    emptyBrush = brush;
    Refresh();
}

void CustomProgressBar::SetTextColor(const wxColor& color)
{
    textColor = color;
    Refresh();
}

void CustomProgressBar::SetHatched(bool hatched)
{
    isHatched = hatched;
    Refresh();
}

bool CustomProgressBar::IsFilled() const
{
    return value >= range;
}

void CustomProgressBar::CustomOnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(emptyBrush);
    dc.Clear();

    const wxSize size = GetClientSize();
    dc.SetPen(*wxMEDIUM_GREY_PEN);

    // Empty progress bar
    dc.SetBrush(emptyBrush);
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    // Filled progress bar
    dc.SetBrush(filledBrush);
    int width = (value * size.GetWidth()) / std::max(1, range);
    width = std::max(0, std::min(size.GetWidth(), width));
    dc.DrawRectangle(0, 0, width, size.GetHeight());

    if (isHatched)
    {
        dc.SetBrush(wxBrush(wxColour(120, 120, 120), wxBRUSHSTYLE_BDIAGONAL_HATCH));
        dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
    }

    // Text on top of progress bar
    dc.SetTextForeground(textColor);
    dc.DrawText(text, (size.GetWidth() - dc.GetTextExtent(text).GetWidth()) / 2,
                (size.GetHeight() - dc.GetTextExtent(text).GetHeight()) / 2);
}
} // namespace another_day