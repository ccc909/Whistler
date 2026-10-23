// Copyright 2023-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1. Modified for the Whistler board, 2026-06-20 (see firmware/README.md).
//
// WHISTLER adaptation of mic_aggregator: 16 PDM mics -> 16-ch USB audio (UAC2).
// TDM and I2C-control paths removed (USB-only board, no codec). Tile layout is
// INVERTED vs the stock example: mics + app-PLL on tile[1], hub + USB on tile[0]
// (see top_level.xc + WHISTLER-USB.xn; USB_TILE=tile[0] in mic_aggregator.cmake).

#include <stdio.h>
#include <limits.h>

#include <xcore/channel.h>
#include <xcore/channel_streaming.h>
#include <xcore/parallel.h>
#include <xcore/select.h>
#include <xcore/hwtimer.h>
#include <print.h>

#include "app_main.h"
#include "mic_array.h"
#include "device_pll_ctrl.h"
#include "mic_array_wrapper.h"

#include "xua_wrapper.h"
#include "xua_conf.h"


DECLARE_JOB(pdm_mic_16, (chanend_t));
void pdm_mic_16(chanend_t c_mic_array) {
    printf("pdm_mic_16 running: %d threads total\n", MIC_ARRAY_PDM_RX_OWN_THREAD + MIC_ARRAY_NUM_DECIMATOR_TASKS);

    app_mic_array_init();
    app_mic_array_assertion_disable();  // don't trap tile[1] if hub stops draining (test-tone diag)
    app_mic_array_task(c_mic_array);
}

DECLARE_JOB(pdm_mic_16_front_end, (void));
void pdm_mic_16_front_end(void) {
    printf("pdm_mic_16_front_end\n");

    if(MIC_ARRAY_PDM_RX_OWN_THREAD){
        app_pdm_rx_task();
    }
}

static inline int32_t scalar_gain(int32_t samp, int32_t gain){
    int64_t accum = (int64_t)samp * (int32_t)gain;
    accum = accum > INT_MAX ? INT_MAX : accum;
    accum = accum < INT_MIN ? INT_MIN : accum;

    return (int32_t)accum;
}


// DIAGNOSTIC TOGGLE. 1 = stream a synthetic sawtooth (bypasses the mics) to test
// the USB output path in isolation; 0 = real mic audio. With this = 1:
//   * arecord captures a ramp  -> USB output path is GOOD; the bug is mic->hub.
//   * arecord still hangs       -> USB output path (XUA_Buffer/exchange) is the bug.
#define HUB_TEST_TONE 0

DECLARE_JOB(hub, (chanend_t, chanend_t));
void hub(chanend_t c_mic_array, chanend_t c_aud) {
    printf("hub\n");

    unsigned write_buffer_idx = 0;
    audio_frame_t audio_frames[NUM_AUDIO_BUFFERS] = {{{{0}}}};
    const uint16_t gain = MIC_GAIN_INIT;

#if HUB_TEST_TONE
    (void) c_mic_array; (void) gain;
    int32_t phase = 0;
    while(1){
        // Synthetic ~750 Hz sawtooth, a distinct offset per channel. Paced by
        // xua_exchange (the USB side), so no ma_frame_rx / no mics involved.
        phase += 0x04000000;
        for(int ch = 0; ch < MIC_ARRAY_CONFIG_MIC_COUNT; ch++){
            audio_frames[write_buffer_idx].data[ch][0] = phase + (ch << 20);
        }
        xua_exchange(c_aud, &audio_frames[write_buffer_idx].data[0][0]);
        write_buffer_idx++;
        if(write_buffer_idx == NUM_AUDIO_BUFFERS) write_buffer_idx = 0;
    }
#else
    while(1){
        ma_frame_rx((int32_t*)&audio_frames[write_buffer_idx], c_mic_array, MIC_ARRAY_CONFIG_MIC_COUNT, MIC_ARRAY_CONFIG_SAMPLES_PER_FRAME);
        for(int ch = 0; ch < MIC_ARRAY_CONFIG_MIC_COUNT; ch++){
            audio_frames[write_buffer_idx].data[ch][0] = scalar_gain(audio_frames[write_buffer_idx].data[ch][0], gain);
        }
        xua_exchange(c_aud, &audio_frames[write_buffer_idx].data[0][0]);
        write_buffer_idx++;
        if(write_buffer_idx == NUM_AUDIO_BUFFERS) write_buffer_idx = 0;
    }
#endif
}


///////// Tile main functions where we par off the threads ///////////

// Runs on tile[1] (see top_level.xc): PDM mics + the 24.576 MHz app-PLL.
void main_tile_0(chanend_t c_cross_tile[2]){
    // Bring up the app-PLL -> PORT_MCLK (PORT_1D) internal loopback BEFORE
    // mic_array_init, exactly as the proven app_mic16 does. (Stock placed this on
    // the USB tile; Whistler co-locates it with the mics so the mic clock block
    // sees MCLK on its own tile.)
    port_t p_app_pll_out = MIC_ARRAY_CONFIG_PORT_MCLK;
    port_enable(p_app_pll_out);
    set_pad_drive_strength(p_app_pll_out, DRIVE_12MA);
    device_pll_init();

    PAR_JOBS(
        PJOB(pdm_mic_16, (c_cross_tile[0])), // spawns MIC_ARRAY_NUM_DECIMATOR_TASKS threads
        PJOB(pdm_mic_16_front_end, ())
    );
}

// Runs on tile[0] (see top_level.xc): hub (mic frames -> USB) + the USB stack.
void main_tile_1(chanend_t c_cross_tile[2]){
    // The app-PLL is programmed ONCE, on the mic tile (main_tile_0), exactly like
    // the proven app_mic16. It's a single chip-level block whose 24.576 MHz output
    // reaches both tiles' PORT_1D (stock mic_aggregator clocks its mics from the
    // other tile this way), so XUA_Buffer's MCLK count here is fed without a second
    // device_pll_init() on this tile (which would race the mic tile's PLL setup).
    channel_t c_aud = chan_alloc();

    PAR_JOBS(
        PJOB(hub, (c_cross_tile[0], c_aud.end_b)),
        PJOB(xua_wrapper, (c_aud.end_a)) // spawns 4 USB threads
    );
}
