// Copyright 2023-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1. Modified for the Whistler board, 2026-06-20 and 2026-10-23 (see firmware/README.md).

// This file contains defines relating to the USB build only.
// WHISTLER: 16-ch USB microphone (UAC2), bus-powered from the USB host, input-only.

#ifndef _XUA_CONF_H_
#define _XUA_CONF_H_

#define NUM_USB_CHAN_OUT 0
#define NUM_USB_CHAN_IN 16
#define I2S_CHANS_DAC 0
#define I2S_CHANS_ADC 0
#define MCLK_441 (512 * 44100)
#define MCLK_48 (512 * 48000)   // 24.576MHz (app-PLL)
#define MIN_FREQ 48000
#define MAX_FREQ 48000

#define EXCLUDE_USB_AUDIO_MAIN

// WHISTLER: derive the audio feedback / IN-packet pacing (g_speed) from the
// always-live 100 MHz reference clock instead of the MCLK-count port. The app-PLL
// 24.576 MHz loopback is PER-TILE (only on the tile that ran device_pll_init =
// the mic tile), so PORT_MCLK_COUNT on the USB tile (tile[0], X0D11) sees a DEAD
// clock -> g_speed collapses -> mis-paced iso IN packets -> arecord EIO. The ref
// clock is exact for this fixed single-rate (48 kHz) input-only device and removes
// the cross-tile-MCLK dependency entirely (no 2nd PLL / no PORT_1D double-enable).
#define FB_USE_REF_CLOCK 1

#define VENDOR_STR "Whistler"
#define VENDOR_ID 0x20B1
#define PRODUCT_STR_A2 "Whistler 16ch Mic"
#define PRODUCT_STR_A1 "Whistler 16ch Mic"
#define PID_AUDIO_1 0x4015
#define PID_AUDIO_2 0x4016
#define AUDIO_CLASS 2
#define UAC_FORCE_FEEDBACK_EP 0          // input-only UAC2: NO feedback EP (matches lib_xua's AN00248 PDM-mic ref). Forcing it inserts an unadvertised/never-armed EP 0x81 that diverges from the descriptor and breaks the host stream-start.
#define AUDIO_CLASS_FALLBACK 0
#define BCD_DEVICE 0x0100
// WHISTLER: USB DFU firmware updates. DFUHandler runs on tile[0] next to
// Endpoint 0 (see ep0_dfu.xc) and writes the QSPI boot flash (W25Q128JW). The
// flash is opened with XTC's generic SFDP spec, as on XMOS's own xcore.ai boards.
#define XUA_DFU_EN 1
#define QUAD_SPI_FLASH 1
#include <QuadSpecMacros.h>
#define DFU_FLASH_DEVICE FL_QUADDEVICE_DEFAULT
#define FLASH_MAX_UPGRADE_SIZE (512 * 1024)   // room reserved for one upgrade image
// No host-side volume controls (matches lib_xua's AN00248 input-only mic ref).
// BOTH must be 0: lib_xua's volume block (xua_ep0_uacreqs.xc:108) compiles if
// EITHER is 1, and its FU_USBIN path references multIn[] (only declared when
// INPUT_VOLUME_CONTROL==1) -> setting only INPUT to 0 with OUTPUT defaulting to 1
// caused the earlier build error. Both 0 also removes the FeatureUnit (id 11)
// that was failing -71 in dmesg and wedging EP0's clock-validity response.
#define INPUT_VOLUME_CONTROL 0
#define OUTPUT_VOLUME_CONTROL 0
#define XUA_POWERMODE XUA_POWERMODE_BUS  // bus-powered from the USB host VBUS (matches lib_xua default)
// CODEC_MASTER removed: Whistler has no audio codec.

#endif
