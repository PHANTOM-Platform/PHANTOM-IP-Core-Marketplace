// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2017.4
// Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

#ifndef XDWT_H
#define XDWT_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xdwt_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XDwt_Config;
#endif

typedef struct {
    u32 Control_bus_BaseAddress;
    u32 IsReady;
} XDwt;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XDwt_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XDwt_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XDwt_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XDwt_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XDwt_Initialize(XDwt *InstancePtr, u16 DeviceId);
XDwt_Config* XDwt_LookupConfig(u16 DeviceId);
int XDwt_CfgInitialize(XDwt *InstancePtr, XDwt_Config *ConfigPtr);
#else
int XDwt_Initialize(XDwt *InstancePtr, const char* InstanceName);
int XDwt_Release(XDwt *InstancePtr);
#endif

void XDwt_Start(XDwt *InstancePtr);
u32 XDwt_IsDone(XDwt *InstancePtr);
u32 XDwt_IsIdle(XDwt *InstancePtr);
u32 XDwt_IsReady(XDwt *InstancePtr);
void XDwt_EnableAutoRestart(XDwt *InstancePtr);
void XDwt_DisableAutoRestart(XDwt *InstancePtr);

void XDwt_Set_imageIn(XDwt *InstancePtr, u32 Data);
u32 XDwt_Get_imageIn(XDwt *InstancePtr);
void XDwt_Set_imageOut0(XDwt *InstancePtr, u32 Data);
u32 XDwt_Get_imageOut0(XDwt *InstancePtr);
void XDwt_Set_imageOut1(XDwt *InstancePtr, u32 Data);
u32 XDwt_Get_imageOut1(XDwt *InstancePtr);
void XDwt_Set_imageOut2(XDwt *InstancePtr, u32 Data);
u32 XDwt_Get_imageOut2(XDwt *InstancePtr);
void XDwt_Set_imageOut3(XDwt *InstancePtr, u32 Data);
u32 XDwt_Get_imageOut3(XDwt *InstancePtr);

void XDwt_InterruptGlobalEnable(XDwt *InstancePtr);
void XDwt_InterruptGlobalDisable(XDwt *InstancePtr);
void XDwt_InterruptEnable(XDwt *InstancePtr, u32 Mask);
void XDwt_InterruptDisable(XDwt *InstancePtr, u32 Mask);
void XDwt_InterruptClear(XDwt *InstancePtr, u32 Mask);
u32 XDwt_InterruptGetEnabled(XDwt *InstancePtr);
u32 XDwt_InterruptGetStatus(XDwt *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif