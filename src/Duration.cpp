#include "pch.hpp"

#include "Duration.hpp"

namespace another_day
{
long long DurationWithUnit::CountInUnits() const
{
    switch (unit)
    {
    case DurationUnit::Seconds:
        return std::chrono::duration_cast<std::chrono::seconds>(value).count();
    case DurationUnit::Minutes:
        return std::chrono::duration_cast<std::chrono::minutes>(value).count();
    case DurationUnit::Hours:
        return std::chrono::duration_cast<std::chrono::hours>(value).count();
    default:
        throw std::runtime_error("Invalid duration unit");
    }
}
std::string DurationWithUnit::ToString() const
{
    std::stringstream ss;
    ss << CountInUnits();
    switch (unit)
    {
    case DurationUnit::Seconds:
        ss << "s";
        break;
    case DurationUnit::Minutes:
        ss << "m";
        break;
    case DurationUnit::Hours:
        ss << "h";
        break;
    default:
        throw std::runtime_error("Invalid duration unit");
    }
    return ss.str();
}

std::optional<DurationWithUnit> DurationWithUnit::FromString(const std::string& durationString)
{
    if (durationString.empty())
        return std::nullopt;

    std::stringstream ss{durationString};
    long long value = 0;
    std::string unit = "";

    ss >> value >> unit;

    if (unit == "")
        return std::chrono::seconds{value};
    else if (unit == "s")
        return std::chrono::seconds{value};
    else if (unit == "m")
        return std::chrono::minutes{value};
    else if (unit == "h")
        return std::chrono::hours{value};

    return std::nullopt;
}

} // namespace another_day