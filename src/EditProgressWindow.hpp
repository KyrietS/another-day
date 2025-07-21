#pragma once
#include "Settings.hpp"

namespace another_day
{
class EditProgressWindow : public wxDialog
{
public:
    EditProgressWindow(wxWindow* parent, Settings& settings);


private:
    Settings& settings;

};
}