// Copyright 2026 the Whistler authors.
// Part of a Derivative of XMOS sln_voice; subject to the terms of the XMOS Public Licence: Version 1. Added 2026-10-23 (see firmware/README.md).

// WHISTLER: Endpoint 0 with lib_xua's DFU handler. i_dfu is an XC interface, so
// the C wrapper (xua_wrapper.c) calls this instead of XUA_Endpoint0 directly.
// DFUHandler is distributable (it shares Endpoint 0's thread) and needs the
// QSPI boot-flash ports, which are on tile[0] with the USB stack.

#include <xs1.h>
#include "xua.h"
#include "dfu_interface.h"

[[distributable]] void DFUHandler(server interface i_dfu i, chanend ?c_user_cmd);

void XUA_Endpoint0_dfu(chanend c_ep0_out, chanend c_ep0_in, chanend c_audioCtrl)
{
    interface i_dfu dfuInterface;

    par
    {
        [[distribute]] DFUHandler(dfuInterface, null);
        XUA_Endpoint0(c_ep0_out, c_ep0_in, c_audioCtrl, null, null, null, dfuInterface);
    }
}
