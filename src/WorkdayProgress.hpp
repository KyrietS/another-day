#pragma once
#include "Duration.hpp"

namespace another_day
{

class WorkdayProgress
{
public:
    WorkdayProgress();
    void Update(Duration);
    Duration Restore();

private:
    std::chrono::steady_clock::time_point lastUpdateSaveTime;
};

} // namespace another_day
