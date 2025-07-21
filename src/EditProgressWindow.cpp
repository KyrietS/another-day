#include "EditProgressWindow.hpp"

namespace another_day
{
EditProgressWindow::EditProgressWindow(wxWindow* parent, Settings& settings)
    : wxDialog(parent, wxID_ANY, "Edit Progress", wxDefaultPosition, wxDefaultSize), settings(settings)
{
}


} // namespace another_day