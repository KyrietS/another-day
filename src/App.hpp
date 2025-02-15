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

private:
    std::filesystem::path GetUserDataDir();

    std::unique_ptr<Settings> settings;
    std::shared_ptr<Database> database;
};
} // namespace another_day