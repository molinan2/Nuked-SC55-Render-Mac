#include "gain.h"

#include <charconv>
#include <cmath>

namespace common
{

float DbToScalar(float db_gain)
{
    return std::powf(10.f, db_gain / 20.f);
}

float ScalarToDb(float scalar)
{
    return 20.f * std::log10f(scalar);
}

ParseGainResult ParseGain(std::string_view str, float& out_gain)
{
    if (str.length() < 3)
    {
        return ParseGainResult::TooShort;
    }

    if (str.substr(str.length() - 2) != "db")
    {
        return ParseGainResult::NoSuffix;
    }

    const char* n_first = str.data();
    const char* n_last  = str.data() + str.size() - 2;

    float db_gain = 0.0f;

    auto fc_result = std::from_chars(n_first, n_last, db_gain);

    if (fc_result.ec != std::errc{})
    {
        return ParseGainResult::ParseFailed;
    }

    out_gain = DbToScalar(db_gain);

    return ParseGainResult{};
}

} // namespace common
