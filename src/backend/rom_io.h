#pragma once

#include "rom.h"
#include <filesystem>
#include <vector>

enum class EMU_RomLoadStatus
{
    // rom loaded successfully
    Loaded,
    // rom could not be loaded - likely IO failure
    Failed,
    // rom not used by romset
    Unused,
};

const char* ToCString(EMU_RomLoadStatus status);

// Set of load statuses. Indexed by EMU_RomLocation.
using EMU_RomLoadStatusSet = std::array<EMU_RomLoadStatus, EMU_ROMLOCATION_COUNT>;

enum class EMU_RomCompletionStatus
{
    // rom is present
    Present,
    // rom is missing
    Missing,
    // rom is not used in this romset
    Unused,
};

const char* ToCString(EMU_RomCompletionStatus status);

// Set of completion statuses. Indexed by EMU_RomLocation.
using EMU_RomCompletionStatusSet = std::array<EMU_RomCompletionStatus, EMU_ROMLOCATION_COUNT>;

// For a single romset, this structure maps each rom in the set to a filename on disk and that file's contents.
struct EMU_RomsetInfo
{
    // Array indexed by EMU_RomLocation
    std::filesystem::path rom_paths[EMU_ROMLOCATION_COUNT]{};
    std::vector<uint8_t>  rom_data[EMU_ROMLOCATION_COUNT]{};

    // Release all rom_data for all roms in this romset.
    void PurgeRomData();

    // Returns true if at least one of `rom_path` or `rom_data` is populated for `location`.
    bool HasRom(EMU_RomLocation location) const;
};

// Contains EMU_RomsetInfo for all supported romsets.
struct EMU_AllRomsetInfo
{
    // Array indexed by Romset
    EMU_RomsetInfo romsets[ROMSET_COUNT]{};

    // Release all rom_data for all romsets.
    void PurgeRomData();
};

// Scans files in `base_path` for specific rom filenames. Consult the `legacy_rom_names` constant in `emu.cpp` for the
// exact filename requirements.
//
// If `desired` is non-null, this function will use it as a hint to determine what filenames to examine. This function
// may also load `rom_data` for desired roms.
bool EMU_DetectRomsetsByFilename(const std::filesystem::path& base_path,
                                 EMU_AllRomsetInfo&           all_info,
                                 EMU_RomLocationSet*          desired = nullptr);

// Scans files in `base_path` for roms by hashing them. The locations of each rom will be made available in `info`. This
// will return *all* romsets in `base_path`.
//
// If any of the rom locations in `all_info` are already populated with a path or data, this function will not overwrite
// them.
//
// If `desired` is non-null, this function will use it as a hint to determine what hashes to consider. This function may
// also load `rom_data` for desired roms.
bool EMU_DetectRomsetsByHash(const std::filesystem::path& base_path,
                             EMU_AllRomsetInfo&           all_info,
                             EMU_RomLocationSet*          desired = nullptr);

// Returns true if `all_info` contains all the files required to load `romset`. Missing roms will be reported in
// `missing`.
bool EMU_IsCompleteRomset(const EMU_AllRomsetInfo&    all_info,
                          Romset                      romset,
                          EMU_RomCompletionStatusSet* status = nullptr);

// Picks the first complete romset in `all_info` and writes it to `out_romset`. If multiple romsets are present, the one
// returned is unspecified. Returns true if successful, or false if there are no complete romsets.
bool EMU_PickCompleteRomset(const EMU_AllRomsetInfo& all_info, Romset& out_romset);

// For each `rom` in `romset`, this function loads the file referenced by `all_info.romsets[romset].rom_paths[rom]` into
// the corresponding `rom_data`. Waveroms will be unscrambled at this point.
//
// `rom` will only be loaded when `rom_data` is empty and `rom_path` is non-empty.
//
// To automatically determine rom_paths, call `EMU_DetectRomsetsByHash` with a directory containing roms.
//
// Roms that were loaded successfully will be marked as true in `loaded`.
bool EMU_LoadRomset(Romset romset, EMU_AllRomsetInfo& all_info, EMU_RomLoadStatusSet* loaded = nullptr);

