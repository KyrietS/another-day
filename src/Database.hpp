#pragma once
#include <chrono>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

struct sqlite3;

namespace another_day
{

namespace events
{
constexpr std::string_view UNKNOWN = "unknown";
constexpr std::string_view BEGIN_WORK = "begin work";
constexpr std::string_view BEGIN_BREAK = "begin break";
constexpr std::string_view END = "end";
} // namespace events

struct Event
{
    int Id;
    std::chrono::system_clock::time_point Timestamp;
    std::string Type;
};

class DatabaseError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Database
{

public:
    Database(const std::filesystem::path& path);
    ~Database();

    // Events
    void InsertEvent(std::string_view data);
    void InsertEvent(std::chrono::system_clock::time_point timestamp, std::string_view data);
    std::vector<Event> GetEventsFromOneDay(std::chrono::system_clock::time_point timestamp);

private:
    void Close() noexcept;
    void AssertOpen() const;
    void CreateEventTable();
    void CheckDatabaseVersion();

    std::filesystem::path path = {};
    sqlite3* db = nullptr;

    const int DB_VERSION = 1;
};
} // namespace another_day