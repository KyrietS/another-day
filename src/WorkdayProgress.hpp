#pragma once
#include "Duration.hpp"

namespace another_day
{

class WorkdayProgress
{
public:
    WorkdayProgress();
    void Save(Duration);
    Duration Restore();
    Duration TimeSinceLastSave();

private:
    std::chrono::steady_clock::time_point lastSaveTime;
};

} // namespace another_day
