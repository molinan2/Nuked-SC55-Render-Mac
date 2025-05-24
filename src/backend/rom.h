#pragma once

#include <array>
#include <span>
#include <string_view>

enum class Romset {
    MK2,
    ST,
    MK1,
    CM300,
    JV880,
    SCB55,
    RLP3237,
    SC155,
    SC155MK2,
};

constexpr size_t ROMSET_COUNT = 9;

const char* EMU_RomsetName(Romset romset);

bool EMU_ParseRomsetName(std::string_view name, Romset& romset);

std::span<const char*> EMU_GetParsableRomsetNames();

// Symbolic name for the various roms used by the emulator.
enum class EMU_RomLocation
{
    // MCU roms
    ROM1,
    ROM2,

    // Sub-MCU roms
    SMROM,

    // PCM roms
    WAVEROM1,
    WAVEROM2,
    WAVEROM3,
    WAVEROM_CARD,
    WAVEROM_EXP,
};

constexpr size_t EMU_ROMLOCATION_COUNT = 8;

const char* ToCString(EMU_RomLocation location);

// Set of rom locations. Indexed by EMU_RomLocation.
using EMU_RomLocationSet = std::array<bool, EMU_ROMLOCATION_COUNT>;

// Returns true if `location` represents a waverom location.
bool EMU_IsWaverom(EMU_RomLocation location);

bool EMU_IsOptionalRom(Romset romset, EMU_RomLocation location);
