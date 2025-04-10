#pragma once
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

struct sqlite3;

namespace another_day
{

namespace events::descriptions
{
constexpr std::string_view UNKNOWN = "unknown";
constexpr std::string_view BEGIN_WORK = "begin work";
constexpr std::string_view BEGIN_BREAK = "begin break";
constexpr std::string_view BEGIN_HALT = "begin halt";
constexpr std::string_view END_HALT = "end halt";
constexpr std::string_view END = "end";
} // namespace events::descriptions

struct Event
{
    int Id;
    std::chrono::system_clock::time_point Timestamp;
    std::string Description;
};

class DatabaseError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Database
{

public:
    Database(std::filesystem::path path);
    ~Database();

    void Open();
    bool IsOpen() const noexcept;
    void Close() noexcept;

    // Events
    void InsertEvent(std::chrono::system_clock::time_point timestamp, std::string_view description);
    std::vector<Event> GetEventsFromOneDay(std::chrono::system_clock::time_point date);

private:
    void AssertOpen() const;
    void CreateEventTable();
    void CheckDatabaseVersion();

    std::filesystem::path path = {};
    sqlite3* db = nullptr;

    const int DB_VERSION = 1;
};
} // namespace another_day