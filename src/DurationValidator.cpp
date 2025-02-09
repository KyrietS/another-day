#include "pch.hpp"

#include "DurationValidator.hpp"
#include <optional>
#include <sstream>

namespace another_day
{
bool DurationValidator::TransferToWindow()
{
    durationString = duration.ToString();
    return wxGenericValidator::TransferToWindow();
}

bool DurationValidator::TransferFromWindow()
{
    if (not wxGenericValidator::TransferFromWindow())
        return false;

    auto parsedDuration = DurationWithUnit::FromString(durationString.ToStdString());
    if (not durationString.IsEmpty() and not parsedDuration.has_value())
    {
        wxLogWarning("Invalid duration value: '%s'.\nAvailable units are: s, m, h", durationString);
        return false;
    }

    duration = parsedDuration.value_or(DurationWithUnit{});

    return true;
}

} // namespace another_day