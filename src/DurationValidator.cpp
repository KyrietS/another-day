#include "pch.hpp"

#include "DurationValidator.hpp"
#include <optional>

namespace another_day
{
bool DurationValidator::TransferToWindow()
{
    durationString = duration.valueString;
    return wxGenericValidator::TransferToWindow();
}

bool DurationValidator::TransferFromWindow()
{
    if (not wxGenericValidator::TransferFromWindow())
        return false;

    try
    {
        duration = DurationSetting{durationString.ToStdString()};
        return true;
    }
    catch (const DurationSyntaxError&)
    {
        wxLogWarning("Invalid duration: '%s'.\nExample: 7h 30m 15s", durationString);
        return false;
    }
}

} // namespace another_day