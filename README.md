# Nuked SC-55 Render Mac

Fork of [jcmoyer/Nuked-SC55](https://github.com/jcmoyer/Nuked-SC55). Motivation is to make it compilable under MacOS 15.5, Xcode 16.4 and Apple clang version 17.0.0.

This is an experimental fork for learning purposes only. Any changes here could (and will certainly) be breaking, so please don't use it.

# Nuked SC-55

Fork of [nukeykt/Nuked-SC55](https://github.com/nukeykt/Nuked-SC55) with the
goal of extracting the emulator backend so that it can be used in other
programs. This fork aims to be 100% behavior-compatible with upstream
(including bugs). For bugs that occur in both this fork and upstream, do not
open an issue here; report it upstream instead.

Differences from upstream:

- Produces a library for the emulator.
- Standard frontend supports routing to multiple emulators to raise polyphony
  limits.
- Includes a MIDI-to-WAVE rendererer.
- Adds tests so that the backend can be modified without worrying about
  breaking things.
- Command line is handled slightly differently. Pass `--help` to a binary to
  see what arguments it accepts.
- Improved performance without sacrificing accuracy.
- Support for ASIO output on Windows when building from source.
- Loads roms based on their hashes instead of requiring specific filenames.

## Building

See [BUILDING.md](BUILDING.md).

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

Nuked SC-55 can be distributed and used under the original MAME license (see
LICENSE file). Non-commercial license was chosen to prevent making and selling
SC-55 emulation boxes using (or around) this code, as well as preventing from
using it in the commercial music production.

