#pragma once
#include <chrono>
#include <format>
#include <optional>
#include <string>

namespace another_day
{
using Duration = std::common_type_t<std::chrono::seconds, std::common_type_t<std::chrono::minutes, std::chrono::hours>>;

struct DurationSyntaxError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct DurationSetting
{
    static std::optional<DurationSetting> FromString(const std::string&);

    DurationSetting() = default;

    DurationSetting(const std::string& valueString);

    DurationSetting(std::chrono::seconds value) : value(value), valueString(std::format("{}s", value.count()))
    {
    }

    DurationSetting(std::chrono::minutes value) : value(value), valueString(std::format("{}m", value.count()))
    {
    }

    DurationSetting(std::chrono::hours value) : value(value), valueString(std::format("{}h", value.count()))
    {
    }

    operator Duration() const
    {
        return value;
    }

    Duration value{};
    std::string valueString{};
};

} // namespace another_day