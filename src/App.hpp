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
    int OnRun() override;

    void CleanUp() override;

private:
    std::unique_ptr<Settings> settings;
    std::unique_ptr<Database> database;
};
} // namespace another_day