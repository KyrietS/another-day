#pragma once
#include "Duration.hpp"

namespace another_day
{
class DurationValidator : public wxGenericValidator
{
public:
    DurationValidator(const DurationValidator&) = default;

    DurationValidator(Duration& duration) : wxGenericValidator(&durationString), duration(duration)
    {
    }

    wxObject* Clone() const override
    {
        return new DurationValidator(duration);
    }

    bool TransferToWindow() override;

    bool TransferFromWindow() override;

    wxString durationString;
    Duration& duration;
};
} // namespace another_day
