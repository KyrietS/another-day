#include "pch.hpp"

#include "Database.hpp"
#include <ctime>
#include <format>
#include <iomanip>
#include <sqlite3.h>
#include <sstream>
#include <utility>

namespace another_day
{
namespace
{
std::chrono::system_clock::time_point StringToTimePoint(const std::string& datetime)
{
    std::tm tm = {};
    std::istringstream ss(datetime);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail())
    {
        throw std::runtime_error("Failed to parse datetime string");
    }
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}
} // namespace

Database::Database(std::filesystem::path path) : path(std::move(path))
{
}

Database::~Database()
{
    Close();
}

void Database::Open()
{
    if (IsOpen())
    {
        return;
    }

    if (sqlite3_open(path.string().c_str(), &db) != SQLITE_OK)
    {
        auto msg = std::format("Failed to open database: {} ({})", path.string(), sqlite3_errmsg(db));
        Close();
        throw DatabaseError(msg);
    }

    CheckDatabaseVersion();
    CreateEventTable();
}

bool Database::IsOpen() const noexcept
{
    return db != nullptr;
}

void Database::Close() noexcept
{
    if (IsOpen())
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

void Database::InsertEvent(std::chrono::system_clock::time_point timestamp, std::string_view description)
{
    AssertOpen();

    const char* sql = R"(
        INSERT INTO events (timestamp, description)
        VALUES (?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        auto msg = std::format("Failed to prepare statement: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseError(msg);
    }

    std::chrono::zoned_time zonedTime{std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(timestamp)};
    std::string ts_str = std::format("{:%Y-%m-%d %H:%M:%S}", zonedTime);

    sqlite3_bind_text(stmt, 1, ts_str.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.data(), static_cast<int>(description.size()), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        auto msg = std::format("Failed to insert event: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseError(msg);
    }

    sqlite3_finalize(stmt);
}

std::vector<Event> Database::GetEventsFromOneDay(std::chrono::system_clock::time_point date)
{
    AssertOpen();

    const char* sql = R"(
        SELECT id, timestamp, description
        FROM events
        WHERE date(timestamp) = ?;
    )";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        auto msg = std::format("Failed to prepare statement: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseError(msg);
    }

    auto ymd = std::format("{:%Y-%m-%d}", std::chrono::floor<std::chrono::days>(date));
    sqlite3_bind_text(stmt, 1, ymd.c_str(), -1, SQLITE_STATIC);
    std::vector<Event> events;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);

        std::string timestamp(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::chrono::system_clock::time_point timePoint = StringToTimePoint(timestamp);

        auto description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (description == nullptr)
        {
            auto msg = std::format("Failed to get description: {}", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseError(msg);
        }

        Event event{.Id = id, .Timestamp = timePoint, .Description = description};

        events.push_back(event);
    }
    sqlite3_finalize(stmt);
    return events;
}

void Database::AssertOpen() const
{
    if (not IsOpen())
    {
        throw DatabaseError("Database is not open");
    }
}

void Database::CreateEventTable()
{
    AssertOpen();

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS events (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            description TEXT NOT NULL
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        auto msg = std::format("Failed to create 'event' table: {}", errMsg ? errMsg : "");
        sqlite3_free(errMsg);
        throw DatabaseError(msg);
    }
}

void Database::CheckDatabaseVersion()
{
    AssertOpen();

    const char* getVersionSql = "PRAGMA user_version;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, getVersionSql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        auto msg = std::format("Failed to prepare statement: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseError(msg);
    }

    int userVersion = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        userVersion = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (userVersion == 0)
    {
        std::string setVersionSql = std::format("PRAGMA user_version = {};", DB_VERSION);
        if (sqlite3_exec(db, setVersionSql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
        {
            auto msg = std::format("Failed to set database version");
            throw DatabaseError(msg);
        }
    }
    else if (userVersion != DB_VERSION)
    {
        auto msg =
            std::format("Database version mismatch: present {}, expected {}\nRemove the old database manually at:\n{}",
                        userVersion, DB_VERSION, path.string());
        throw DatabaseError(msg);
    }
}

} // namespace another_day