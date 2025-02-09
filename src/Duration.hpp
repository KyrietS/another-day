#pragma once
#include <chrono>
#include <optional>
#include <string>

namespace another_day
{
using Duration =
    std::common_type<std::chrono::seconds, std::common_type<std::chrono::minutes, std::chrono::hours>::type>::type;

enum class DurationUnit
{
    Seconds,
    Minutes,
    Hours
};

struct DurationWithUnit
{
    DurationWithUnit() = default;

    DurationWithUnit(std::chrono::seconds value) : value(value), unit(DurationUnit::Seconds)
    {
    }

    DurationWithUnit(std::chrono::minutes value) : value(value), unit(DurationUnit::Minutes)
    {
    }

    DurationWithUnit(std::chrono::hours value) : value(value), unit(DurationUnit::Hours)
    {
    }

    operator Duration() const
    {
        return value;
    }

    long long CountInUnits() const;
    std::string ToString() const;

    static std::optional<DurationWithUnit> FromString(const std::string&);

    Duration value{};
    DurationUnit unit{};
};

} // namespace another_day