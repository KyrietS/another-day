#pragma once
#include "Database.hpp"
#include "Settings.hpp"

namespace another_day
{
class WorkLog
{
public:
    WorkLog(const Settings&, Database&);

    void BeginWork();
    void BeginBreak();
    void BeginSuspend();
    void EndSuspend();
    void End();
    void ManualEdit();

private:
    void InsertEvent(std::string_view description);

    bool Enabled() const;
    bool ShouldSkip() const;
    void HandleError(const DatabaseError&);

    const Settings& settings;
    Database& database;
};
} // namespace another_day