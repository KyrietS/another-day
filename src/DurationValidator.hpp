#pragma once
#include "Duration.hpp"

namespace another_day
{
class DurationValidator : public wxGenericValidator
{
public:
    DurationValidator(const DurationValidator&) = default;

    DurationValidator(DurationSetting& duration) : wxGenericValidator(&durationString), duration(duration)
    {
    }

    wxObject* Clone() const override
    {
        return new DurationValidator(duration);
    }

    bool TransferToWindow() override;

    bool TransferFromWindow() override;

private:
    wxString durationString;
    DurationSetting& duration;
};
} // namespace another_day
