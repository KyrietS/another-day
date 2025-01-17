#pragma once
#include "MainWindow.hpp"
#include "Settings.hpp"
#include <memory>

namespace another_day
{
class App : public wxApp
{
public:
    bool OnInit() override;

private:
    std::unique_ptr<Settings> settings;
};
} // namespace another_day