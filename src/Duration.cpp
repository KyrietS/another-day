#include "Duration.hpp"
#include <list>
#include <utility>

namespace another_day
{
namespace
{
using Token = std::pair<long long, char>;

std::list<Token> ReadTokens(const std::string& durationString)
{
    std::list<Token> tokens;
    std::stringstream ss{durationString};

    long long value = 0;
    char unit = 0;

    while (ss >> value)
    {
        if (ss >> unit)
        {
            tokens.emplace_back(value, unit);
        }
        else
        {
            throw DurationSyntaxError{"Invalid unit in duration value"};
        }
    }

    if (not ss.eof())
        throw DurationSyntaxError{"Invalid duration value"};

    return tokens;
}

template <typename TimeUnit> TimeUnit Read(std::list<Token>& tokens, char unit)
{
    if (not tokens.empty() and tokens.front().second == unit)
    {
        TimeUnit hours{tokens.front().first};
        tokens.pop_front();
        return hours;
    }

    return TimeUnit{0};
}
} // namespace

std::optional<DurationSetting> DurationSetting::FromString(const std::string& durationString)
{
    if (durationString.empty())
        return std::nullopt;

    try
    {
        return DurationSetting{durationString};
    }
    catch (const DurationSyntaxError&)
    {
        return std::nullopt;
    }
}

DurationSetting::DurationSetting(const std::string& valueString) : value(0), valueString(valueString)
{
    auto tokens = ReadTokens(valueString);

    value += Read<std::chrono::hours>(tokens, 'h');
    value += Read<std::chrono::minutes>(tokens, 'm');
    value += Read<std::chrono::seconds>(tokens, 's');

    if (not tokens.empty()) // some tokens were not consumed
        throw DurationSyntaxError{"Invalid duration value"};
}

} // namespace another_day