#include "pch.hpp"

#include "DurationValidator.hpp"
#include <optional>
#include <sstream>

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
        wxLogWarning("Invalid duration: '%s'.\nAvailable units are: s, m, h", durationString);
        return false;
    }
}

} // namespace another_day