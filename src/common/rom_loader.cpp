#include "rom_loader.h"

namespace common
{

const char* ToCString(LoadRomsetError error)
{
    switch (error)
    {
    case LoadRomsetError::InvalidRomsetName:
        return "Invalid romset name";
    case LoadRomsetError::DetectionFailed:
        return "Failed to detect romsets";
    case LoadRomsetError::NoCompleteRomsets:
        return "No complete romsets";
    case LoadRomsetError::IncompleteRomset:
        return "Requested romset is incomplete";
    case LoadRomsetError::RomLoadFailed:
        return "Failed to load roms";
    }

    if (error == LoadRomsetError{})
    {
        return "No error";
    }
    else
    {
        return "Unknown error";
    }
}

LoadRomsetError LoadRomset(EMU_AllRomsetInfo&           romset_info,
                           const std::filesystem::path& rom_directory,
                           std::string_view             desired_romset,
                           bool                         legacy_loader,
                           const RomOverrides&          overrides,
                           LoadRomsetResult&            result)
{
    if (desired_romset.size())
    {
        if (!EMU_ParseRomsetName(desired_romset, result.romset))
        {
            return LoadRomsetError::InvalidRomsetName;
        }

        // When the user specifies a romset, we can speed up the loading process a bit.
        EMU_RomMapLocationSet desired{};
        desired[(size_t)result.romset] = true;

        if (legacy_loader)
        {
            if (!EMU_DetectRomsetsByFilename(rom_directory, romset_info, &desired))
            {
                return LoadRomsetError::DetectionFailed;
            }
        }
        else
        {
            if (!EMU_DetectRomsetsByHash(rom_directory, romset_info, &desired))
            {
                return LoadRomsetError::DetectionFailed;
            }
        }
    }
    else
    {
        // No user-specified romset; we'll use whatever romset we can find.
        if (legacy_loader)
        {
            if (!EMU_DetectRomsetsByFilename(rom_directory, romset_info))
            {
                return LoadRomsetError::DetectionFailed;
            }
        }
        else
        {
            if (!EMU_DetectRomsetsByHash(rom_directory, romset_info))
            {
                return LoadRomsetError::DetectionFailed;
            }
        }

        if (!EMU_PickCompleteRomset(romset_info, result.romset))
        {
            return LoadRomsetError::NoCompleteRomsets;
        }
    }

    for (size_t i = 0; i < ROMSET_COUNT; ++i)
    {
        for (size_t j = 0; j < EMU_ROMMAPLOCATION_COUNT; ++j)
        {
            if (!overrides[j].empty())
            {
                romset_info.romsets[i].rom_paths[j] = overrides[j];
                romset_info.romsets[i].rom_data[j].clear();
            }
        }
    }

    if (!EMU_IsCompleteRomset(romset_info, result.romset, &result.completion))
    {
        return LoadRomsetError::IncompleteRomset;
    }

    if (!EMU_LoadRomset(result.romset, romset_info, &result.loaded))
    {
        return LoadRomsetError::RomLoadFailed;
    }

    return LoadRomsetError{};
}

void PrintRomsets(FILE* output)
{
    fprintf(output, "Accepted romset names:\n");
    fprintf(output, "  ");
    for (const char* name : EMU_GetParsableRomsetNames())
    {
        fprintf(output, "%s ", name);
    }
    fprintf(output, "\n\n");
}

void PrintLoadRomsetDiagnostics(FILE*                    output,
                                LoadRomsetError          error,
                                const LoadRomsetResult&  result,
                                const EMU_AllRomsetInfo& info)
{
    switch (error)
    {
    case LoadRomsetError::DetectionFailed:
        // EMU_Detect* will print its own diagnostics
        break;
    case LoadRomsetError::InvalidRomsetName:
        fprintf(output, "error: %s\n", ToCString(error));
        PrintRomsets(output);
        break;
    case LoadRomsetError::NoCompleteRomsets:
        fprintf(output, "error: %s\n", ToCString(error));
        break;
    case LoadRomsetError::IncompleteRomset:
        fprintf(output, "Romset %s is incomplete:\n", EMU_RomsetName(result.romset));
        for (size_t i = 0; i < EMU_ROMMAPLOCATION_COUNT; ++i)
        {
            if (result.completion[i] != EMU_RomCompletionStatus::Unused)
            {
                fprintf(output,
                        "  * %7s: %-12s",
                        ToCString(result.completion[i]),
                        ToCString((EMU_RomMapLocation)i));

                if (result.completion[i] == EMU_RomCompletionStatus::Present)
                {
                    fprintf(output, "%s\n", info.romsets[(size_t)result.romset].rom_paths[i].generic_string().c_str());
                }
                else
                {
                    fprintf(output, "\n");
                }
            }
        }
        break;
    case LoadRomsetError::RomLoadFailed:
        fprintf(output, "Failed to load some %s roms:\n", EMU_RomsetName(result.romset));
        for (size_t i = 0; i < EMU_ROMMAPLOCATION_COUNT; ++i)
        {
            if (result.loaded[i] != EMU_RomLoadStatus::Unused)
            {
                fprintf(output,
                        "  * %s: %-12s %s\n",
                        ToCString(result.loaded[i]),
                        ToCString((EMU_RomMapLocation)i),
                        info.romsets[(size_t)result.romset].rom_paths[i].generic_string().c_str());
            }
        }
        break;
    }

    if (error == LoadRomsetError{})
    {
        fprintf(output, "Using %s romset:\n", EMU_RomsetName(result.romset));
        for (size_t i = 0; i < EMU_ROMMAPLOCATION_COUNT; ++i)
        {
            if (result.loaded[i] == EMU_RomLoadStatus::Loaded)
            {
                fprintf(output,
                        "  * %-12s %s\n",
                        ToCString((EMU_RomMapLocation)i),
                        info.romsets[(size_t)result.romset].rom_paths[i].generic_string().c_str());
            }
        }
    }
}

} // namespace common
