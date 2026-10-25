# Manufacturing notes

Each board's ordering files are in `mic-array/production/` and
`motherboard/production/`.

| File | Use |
|---|---|
| `*-gerbers.zip` | PCB: Gerber X2 + Excellon drills (PTH/NPTH) |
| `*-bom.csv` | BOM with LCSC part numbers, plus manufacturer and MPN for the mics and XU316 |
| `*-cpl.csv` | Pick-and-place, JLCPCB format |
| `*-schematic.pdf`, `*.step` | Reference: schematic and 3D model |

## PCB options

| | Mic array | Motherboard |
|---|---|---|
| Layers | 2 | 4 |
| Size | 200 × 200 mm (round) | 28.8 × 60.2 mm |
| Thickness | 2.0 mm | 1.6 mm |
| Min track / drill | 0.2 / 0.3 mm | 0.2 / 0.3 mm |
| Assembly | Top side only | Both sides |

## Before you order

- **Motherboard J19 via-in-pad.** Each of the FFC connector's two mounting tabs
  is grounded through four vias in its pad. JLC's DFM check may query them.
- **Stock (checked 2026-10-25).** The XU316 (C6938291) is out of stock at both
  LCSC and JLCPCB. Check before ordering, or source it elsewhere and have JLC
  place it from your own supply. The W25Q128JW flash (C2763561) is running low
  at JLCPCB (66 left), and everything else is in stock there.

## Assembly

Rotations in the CPL follow the `kicad-jlcpcb-tools` conventions, so check JLC's
placement preview before you confirm the order. The U5 flash ships blank and is
programmed after assembly, as described in
[`firmware/README.md`](../firmware/README.md).

## Interconnect

The two boards connect through **J19** on each (Hirose FH12-24S-0.5SH, 24-pin
0.5 mm FFC), wired pin-for-pin:

| Pins | Signal |
|---|---|
| 1–2 | +5 V to the array |
| 4 | PDM clock |
| 6, 8, …, 20 (even) | PDM data, two mics per line |
| All other pins | GND |
