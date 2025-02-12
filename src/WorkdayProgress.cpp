#include "pch.hpp"

#include "WorkdayProgress.hpp"

namespace another_day
{
namespace
{
std::string GetTimestamp()
{
    std::chrono::zoned_time zonedTime{std::chrono::current_zone(), std::chrono::system_clock::now()};
    const auto ymd = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(zonedTime.get_local_time()));
    return std::format("{}", ymd);
}
} // namespace

WorkdayProgress::WorkdayProgress()
{
    auto currentTimestamp = GetTimestamp();
    auto savedTimestamp = wxConfig::Get()->Read("WorkdayProgressTimestamp", "");

    if (currentTimestamp != savedTimestamp) // New day, reset progress
    {
        wxConfig::Get()->Write("WorkdayProgressDuration", 0LL);
    }

    wxConfig::Get()->Write("WorkdayProgressTimestamp", wxString(currentTimestamp));
}

void WorkdayProgress::Save(Duration duration)
{
    wxConfig::Get()->Write("WorkdayProgressDuration", duration.count());
    lastSaveTime = std::chrono::steady_clock::now();
}

Duration WorkdayProgress::Restore()
{
    return Duration{wxConfig::Get()->ReadLongLong("WorkdayProgressDuration", 0)};
}

Duration WorkdayProgress::TimeSinceLastSave()
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastSave = now - lastSaveTime;
    return std::chrono::duration_cast<Duration>(timeSinceLastSave);
}

} // namespace another_day