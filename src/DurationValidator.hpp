#pragma once
#include "Duration.hpp"

namespace another_day
{
class DurationValidator : public wxGenericValidator
{
public:
    DurationValidator(const DurationValidator&) = default;

    DurationValidator(DurationWithUnit& duration) : wxGenericValidator(&durationString), duration(duration)
    {
    }

    wxObject* Clone() const override
    {
        return new DurationValidator(duration);
    }

    bool TransferToWindow() override;

    bool TransferFromWindow() override;

    wxString durationString;
    DurationWithUnit& duration;
};
} // namespace another_day
