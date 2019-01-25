// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.4
// Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

/***************************** Include Files *********************************/
#include "xdwt.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XDwt_CfgInitialize(XDwt *InstancePtr, XDwt_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_bus_BaseAddress = ConfigPtr->Control_bus_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XDwt_Start(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL) & 0x80;
    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL, Data | 0x01);
}

u32 XDwt_IsDone(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XDwt_IsIdle(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XDwt_IsReady(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XDwt_EnableAutoRestart(XDwt *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL, 0x80);
}

void XDwt_DisableAutoRestart(XDwt *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_AP_CTRL, 0);
}

void XDwt_Set_imageIn(XDwt *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEIN_DATA, Data);
}

u32 XDwt_Get_imageIn(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEIN_DATA);
    return Data;
}

void XDwt_Set_imageOut0(XDwt *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT0_DATA, Data);
}

u32 XDwt_Get_imageOut0(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT0_DATA);
    return Data;
}

void XDwt_Set_imageOut1(XDwt *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT1_DATA, Data);
}

u32 XDwt_Get_imageOut1(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT1_DATA);
    return Data;
}

void XDwt_Set_imageOut2(XDwt *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT2_DATA, Data);
}

u32 XDwt_Get_imageOut2(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT2_DATA);
    return Data;
}

void XDwt_Set_imageOut3(XDwt *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT3_DATA, Data);
}

u32 XDwt_Get_imageOut3(XDwt *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IMAGEOUT3_DATA);
    return Data;
}

void XDwt_InterruptGlobalEnable(XDwt *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_GIE, 1);
}

void XDwt_InterruptGlobalDisable(XDwt *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_GIE, 0);
}

void XDwt_InterruptEnable(XDwt *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IER);
    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IER, Register | Mask);
}

void XDwt_InterruptDisable(XDwt *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IER);
    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IER, Register & (~Mask));
}

void XDwt_InterruptClear(XDwt *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDwt_WriteReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_ISR, Mask);
}

u32 XDwt_InterruptGetEnabled(XDwt *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_IER);
}

u32 XDwt_InterruptGetStatus(XDwt *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XDwt_ReadReg(InstancePtr->Control_bus_BaseAddress, XDWT_CONTROL_BUS_ADDR_ISR);
}

