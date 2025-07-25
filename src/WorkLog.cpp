#include "pch.hpp"

#include "WorkLog.hpp"
#include <chrono>

namespace another_day
{
WorkLog::WorkLog(const Settings& settings, Database& database) : settings(settings), database(database)
{
    try
    {
        if (Enabled() and not database.IsOpen())
        {
            database.Open();
        }
    }
    catch (const DatabaseError& e)
    {
        HandleError(e);
    }
}

void WorkLog::BeginWork()
{
    InsertEvent(events::descriptions::BEGIN_WORK);
}

void WorkLog::BeginBreak()
{
    InsertEvent(events::descriptions::BEGIN_BREAK);
}

void WorkLog::BeginSuspend()
{
    InsertEvent(events::descriptions::BEGIN_SUSPEND);
}

void WorkLog::EndSuspend()
{
    InsertEvent(events::descriptions::END_SUSPEND);
}

void WorkLog::End()
{
    InsertEvent(events::descriptions::END);
}
void WorkLog::ManualEdit()
{
    InsertEvent(events::descriptions::MANUAL_EDIT);
}

void WorkLog::InsertEvent(std::string_view description)
{
    if (ShouldSkip())
        return;

    try
    {
        database.InsertEvent(std::chrono::system_clock::now(), description);
    }
    catch (const DatabaseError& e)
    {
        HandleError(e);
    }
}

bool WorkLog::Enabled() const
{
    return settings.enableWorkLog;
}

bool WorkLog::ShouldSkip() const
{
    return not Enabled() or not database.IsOpen();
}

void WorkLog::HandleError(const DatabaseError& e)
{
    wxLogError("Error: \"%s\".\nWork log will be disabled.", e.what());
    database.Close();
}

} // namespace another_day