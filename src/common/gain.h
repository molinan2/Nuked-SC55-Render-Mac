#pragma once

#include <string_view>

namespace common
{

struct GainScalar
{
    float scalar;
};

struct GainDb
{
    float decibels;
};

enum class ParseGainResult
{
    TooShort = 1,
    NoSuffix,
    ParseFailed,
};

ParseGainResult ParseGain(std::string_view str, float& out_gain);

float DbToScalar(float db_gain);
float ScalarToDb(float scalar);

} // namespace common
