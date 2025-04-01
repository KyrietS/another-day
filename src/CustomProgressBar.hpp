#pragma once

namespace another_day
{
class CustomProgressBar : public wxPanel
{
public:
    CustomProgressBar(wxWindow* parent, wxWindowID id, int range, const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize);

    void SetRange(int range);
    void SetValue(int value);
    int GetValue() const;

    void SetText(const wxString& text);
    wxString GetText() const;

    void SetFilledColor(const wxBrush& brush);
    void SetEmptyColor(const wxBrush& brush);
    void SetTextColor(const wxColor& color);
    void SetHatched(bool hatched);

    bool IsFilled() const;

private:
    void CustomOnPaint(wxPaintEvent& event);

    int value;
    int range;
    wxString text;

    wxBrush filledBrush = *wxGREEN_BRUSH;
    wxBrush emptyBrush = *wxLIGHT_GREY_BRUSH;
    wxColor textColor = *wxBLACK;
    bool isHatched = false;
};
} // namespace another_day