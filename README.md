# Whistler: 16-Channel USB MEMS Microphone Array (Open Hardware)

Whistler is an open-source 16-channel circular MEMS microphone array. It plugs
into a computer over USB-C and works as a standard USB Audio Class 2 (UAC2)
microphone, with no drivers to install. An XMOS XU316 (xcore.ai) captures the
16 PDM microphones and streams them at 48 kHz. This repository has the KiCad
PCB design files, a BOM and JLCPCB production files, and the firmware.

| Mic array | Motherboard (top) | Motherboard (bottom) |
|---|---|---|
| ![Whistler 16-channel circular MEMS microphone array PCB, top view](docs/mic-array-top.png) | ![Whistler XMOS XU316 USB audio motherboard PCB, top view](docs/motherboard-top.png) | ![Whistler XMOS XU316 USB audio motherboard PCB, bottom view](docs/motherboard-bottom.png) |

## Key features

- **16 microphones:** Infineon IM72D128 PDM MEMS mics on a 188 mm circle with
  36.7 mm spacing. The array is designed for beamforming from about 1 kHz up to
  4.7 kHz, its spatial-aliasing limit.
- **USB audio:** an XMOS XU316 (xcore.ai) streams all 16 channels as a
  48 kHz USB Audio Class 2 microphone over USB-C.
- **Standalone:** boots from on-board QSPI flash, runs on USB bus power, and
  takes firmware updates over USB (DFU).
- **Two boards:** a 2-layer mic array and a 4-layer, 28.8 × 60.2 mm
  motherboard, linked by a 24-pin FFC.
- **Ready to order:** gerbers, a BOM with LCSC numbers, and a JLCPCB
  pick-and-place file for each board.

## Project structure

| Folder | Contents |
|---|---|
| [`mic-array/`](mic-array), [`motherboard/`](motherboard) | KiCad 10 projects; ordering files are in each `production/` folder |
| [`lib/`](lib) | Symbols, footprints and 3D models for both boards |
| [`firmware/`](firmware) | XU316 firmware: a prebuilt image and its source |
| [`docs/`](docs) | Renders and [manufacturing notes](docs/manufacturing.md) |

## Quick start

- **Edit:** open either `*.kicad_pro` in KiCad 10. All libraries are included.
- **Order:** upload a board's `production/` files to JLCPCB. Read
  [docs/manufacturing.md](docs/manufacturing.md) first.
- **Flash:** program a new motherboard once over an xTAG on J2. After that,
  updates go over USB (DFU); see [firmware/README.md](firmware/README.md).

```bash
xflash --factory firmware/bin/whistler-mic-aggregator.xe
```

## Licence

Copyright 2026 the Whistler authors.

- **Hardware:** [CERN-OHL-S v2](LICENSE).
- **Firmware:** XMOS Public Licence v1 ([`firmware/LICENSE-XMOS.rst`](firmware/LICENSE-XMOS.rst)).
- **Library parts:** keep their own licences ([`lib/README.md`](lib/README.md)).
