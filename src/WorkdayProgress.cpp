#include "pch.hpp"

#include "WorkdayProgress.hpp"

namespace another_day
{
namespace
{
std::string GetTimestamp()
{
    const auto ymd =
        std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()));
    return std::format("{}", ymd);
}
} // namespace

WorkdayProgress::WorkdayProgress()
{
    wxConfig::Get()->Write("WorkdayProgressTimestamp", wxString(GetTimestamp()));
}

void WorkdayProgress::Update(Duration duration)
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastUpdateSave = now - lastUpdateSaveTime;

    // Save progress every 5 minutes
    if (timeSinceLastUpdateSave >= std::chrono::minutes{5})
    {
        lastUpdateSaveTime = now;
        SaveProgress(duration);
    }
}

void WorkdayProgress::SaveProgress(Duration duration)
{
    wxConfig::Get()->Write("WorkdayProgressDuration", duration.count());
}

Duration WorkdayProgress::Restore()
{
    auto currentTimestamp = GetTimestamp();
    auto savedTimestamp = wxConfig::Get()->Read("WorkdayProgressTimestamp", "");

    if (currentTimestamp == savedTimestamp) // Same day, restore progress
    {
        return Duration{wxConfig::Get()->ReadLongLong("WorkdayProgressDuration", 0)};
    }

    return Duration{0};
}

} // namespace another_day