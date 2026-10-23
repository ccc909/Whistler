# Whistler firmware (XU316)

This firmware turns the motherboard into a 16-channel, 48 kHz UAC2 USB
microphone that enumerates as "Whistler 16ch Mic" (VID `0x20B1`, PID `0x4016`).
It is based on the `mic_aggregator` example from XMOS
[`sln_voice`](https://github.com/xmos/sln_voice).

## Flashing

The first flash of a blank board needs an XMOS xTAG on **J2** (2 × 10,
1.27 mm). Power the board over USB-C, then run:

```bash
xflash --factory bin/whistler-mic-aggregator.xe
```

## Updating over USB (DFU)

Once a board has been flashed, later updates go over USB and no xTAG is needed.
Turn the new `.xe` into an upgrade image, then send it with `xmosdfu`, XMOS's
host tool from `sln_voice/modules/xua/lib_xua/host/xmosdfu`:

```bash
xflash --factory-version 15.3 --upgrade 1 bin/whistler-mic-aggregator.xe -o upgrade.bin
xmosdfu 0x4016 --download upgrade.bin
```

The factory image stays in flash as a fallback; `xmosdfu 0x4016 --revertfactory`
switches back to it. On Windows, `xmosdfu` needs a WinUSB driver for the DFU
interface, which you can install with Zadig. DFU hasn't been tested on a
Whistler board yet.

## Channel order

The DDR de-interleave puts the odd mics on the first eight USB channels:

| USB channels | Mics |
|---|---|
| 1–8 | MK1, MK3, MK5, …, MK15 |
| 9–16 | MK2, MK4, MK6, …, MK16 |

## Building

You need XTC tools 15.3.1, CMake and make. Apply this folder to `sln_voice` at
commit `a24cd57` and build. Changes from upstream are marked `WHISTLER:` in the
source.

```bash
git clone https://github.com/xmos/sln_voice.git && cd sln_voice
git checkout a24cd5799660c4dce1b59605619da2ad5814bc1c && git submodule update --init --recursive
rm -r examples/mic_aggregator && cp -r <this-folder>/mic_aggregator examples/ && git apply <this-folder>/sln_voice-build.patch
cmake -G "Unix Makefiles" -B build --toolchain xmos_cmake_toolchain/xs3a.cmake && cmake --build build --target example_mic_aggregator_usb
```

## Licence

This firmware is derived from commit `a24cd57` of XMOS
[`sln_voice`](https://github.com/xmos/sln_voice), whose source code is available
under the XMOS Public Licence v1. This folder is distributed under the same
licence; see
[`LICENSE-XMOS.rst`](LICENSE-XMOS.rst). Commercial use is permitted only on XMOS
devices.
