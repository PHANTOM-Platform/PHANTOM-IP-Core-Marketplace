// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2017.4
// Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

// CONTROL_BUS
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x10 : Data signal of imageIn
//        bit 31~0 - imageIn[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of imageOut0
//        bit 31~0 - imageOut0[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of imageOut1
//        bit 31~0 - imageOut1[31:0] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of imageOut2
//        bit 31~0 - imageOut2[31:0] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of imageOut3
//        bit 31~0 - imageOut3[31:0] (Read/Write)
// 0x34 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XDWT_CONTROL_BUS_ADDR_AP_CTRL        0x00
#define XDWT_CONTROL_BUS_ADDR_GIE            0x04
#define XDWT_CONTROL_BUS_ADDR_IER            0x08
#define XDWT_CONTROL_BUS_ADDR_ISR            0x0c
#define XDWT_CONTROL_BUS_ADDR_IMAGEIN_DATA   0x10
#define XDWT_CONTROL_BUS_BITS_IMAGEIN_DATA   32
#define XDWT_CONTROL_BUS_ADDR_IMAGEOUT0_DATA 0x18
#define XDWT_CONTROL_BUS_BITS_IMAGEOUT0_DATA 32
#define XDWT_CONTROL_BUS_ADDR_IMAGEOUT1_DATA 0x20
#define XDWT_CONTROL_BUS_BITS_IMAGEOUT1_DATA 32
#define XDWT_CONTROL_BUS_ADDR_IMAGEOUT2_DATA 0x28
#define XDWT_CONTROL_BUS_BITS_IMAGEOUT2_DATA 32
#define XDWT_CONTROL_BUS_ADDR_IMAGEOUT3_DATA 0x30
#define XDWT_CONTROL_BUS_BITS_IMAGEOUT3_DATA 32
