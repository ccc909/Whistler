# Whistler KiCad library

This library holds the symbols, footprints and 3D models for both Whistler
boards. They come from KiCad's official libraries and SnapEDA, each under its
own licence, and are not covered by the repository's CERN-OHL-S v2 licence.

## KiCad official libraries (KiCad 10.0)

**Contents:** 26 symbols, 17 footprints and 13 3D models, covering everything
not listed in the SnapEDA table below.

**Licence:** [CC BY-SA 4.0 with KiCad's design exception](https://www.kicad.org/libraries/license/).
The exception means designs and generated files that use these parts are not
bound by the share-alike terms.

**Changes:**
- The symbols are the copies embedded in the Whistler schematics, and their
  default footprint links point at this library.
- The footprints have the same content as KiCad 10.0.1, apart from their
  3D-model paths, which point at `Whistler.3dshapes/`.
- The 3D models are unchanged.

## SnapEDA (SnapMagic)

| Part | Symbol | Footprint | 3D model |
|---|---|---|---|
| Infineon IM72D128 MEMS mic ([part page](https://www.snapeda.com/parts/IM72D128/Infineon/view-part/?ref=snap)) | `IM72D128V01XTMA1` | `MIC_IM72D128V01XTMA1` | `IM72D128V01XTMA1.step` |
| XMOS XU316-1024-TQ128-I24 ([part page](https://www.snapeda.com/parts/XU316-1024-TQ128-I24/XMOS/view-part/?ref=snap)) | `XU316-1024-TQ128-I24` | `QFP40P1600X1600X120-129N` | `XU316-1024-TQ128-I24.step` |
| Murata DFE201612E-2R2M 2.2 µH inductor | (uses KiCad `L_Small`) | `INDC2016X120N` | `DFE201612E-2R2M_P2.step` |

**Licence:** [CC BY-SA 4.0 with SnapMagic's Design Exception 1.0](https://www.snapeda.com/about/terms/),
section 4. The exception lets you design, manufacture and distribute boards
that use these files, on terms of your choice.

**Sharing limit:** SnapMagic's terms (§5.1(g)) don't allow more than **10
design files** to be shared from one location without their permission. This
repository holds 8 of them, which leaves room for 2 more.

**Changes:**
- The footprints were converted from SnapEDA's KiCad 5 format to KiCad 10.
- Each footprint carries the 3D-model offset and rotation used on the boards.
- The symbols are the copies embedded in the schematics.
- The XU316 3D model is a FreeCAD re-export found alongside the SnapEDA download.
- The inductor model is Murata's `DFE201612R-H-1R0M=P2.STEP` for the same
  2016-size package, renamed to `DFE201612E-2R2M_P2.step`.
