#include "pch.hpp"

#include "DurationValidator.hpp"

namespace another_day
{

bool DurationValidator::TransferToWindow()
{
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    durationString = seconds ? wxString::Format("%lld", seconds) : wxString("");
    return wxGenericValidator::TransferToWindow();
}

bool DurationValidator::TransferFromWindow()
{
    if (not wxGenericValidator::TransferFromWindow())
        return false;

    long long value = 0;
    if (not durationString.IsEmpty() and not durationString.ToLongLong(&value))
    {
        wxLogWarning("Failed to parse duration: '%s'", durationString);
        return false;
    }
    duration = std::chrono::seconds{value};
    return true;
}

} // namespace another_day