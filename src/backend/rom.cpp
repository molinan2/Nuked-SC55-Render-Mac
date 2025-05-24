#include "rom.h"

const char* rs_name[ROMSET_COUNT] = {
    "SC-55mk2",
    "SC-55st",
    "SC-55mk1",
    "CM-300/SCC-1",
    "JV-880",
    "SCB-55",
    "RLP-3237",
    "SC-155",
    "SC-155mk2"
};

const char* rs_name_simple[ROMSET_COUNT] = {
    "mk2",
    "st",
    "mk1",
    "cm300",
    "jv880",
    "scb55",
    "rlp3237",
    "sc155",
    "sc155mk2"
};

const char* EMU_RomsetName(Romset romset)
{
    return rs_name[(size_t)romset];
}

bool EMU_ParseRomsetName(std::string_view name, Romset& romset)
{
    for (size_t i = 0; i < ROMSET_COUNT; ++i)
    {
        if (rs_name_simple[i] == name)
        {
            romset = (Romset)i;
            return true;
        }
    }
    return false;
}

std::span<const char*> EMU_GetParsableRomsetNames()
{
    return rs_name_simple;
}

bool EMU_IsWaverom(EMU_RomLocation location)
{
    switch (location)
    {
    case EMU_RomLocation::WAVEROM1:
    case EMU_RomLocation::WAVEROM2:
    case EMU_RomLocation::WAVEROM3:
    case EMU_RomLocation::WAVEROM_CARD:
    case EMU_RomLocation::WAVEROM_EXP:
        return true;
    default:
        return false;
    }
}

const char* ToCString(EMU_RomLocation location)
{
    switch (location)
    {
    case EMU_RomLocation::ROM1:
        return "ROM1";
    case EMU_RomLocation::ROM2:
        return "ROM2";
    case EMU_RomLocation::SMROM:
        return "SMROM";
    case EMU_RomLocation::WAVEROM1:
        return "WAVEROM1";
    case EMU_RomLocation::WAVEROM2:
        return "WAVEROM2";
    case EMU_RomLocation::WAVEROM3:
        return "WAVEROM3";
    case EMU_RomLocation::WAVEROM_CARD:
        return "WAVEROM_CARD";
    case EMU_RomLocation::WAVEROM_EXP:
        return "WAVEROM_EXP";
    }
    return "invalid location";
}

bool EMU_IsOptionalRom(Romset romset, EMU_RomLocation location)
{
    return romset == Romset::JV880 &&
           (location == EMU_RomLocation::WAVEROM_CARD || location == EMU_RomLocation::WAVEROM_EXP);
}
