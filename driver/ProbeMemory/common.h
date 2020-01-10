/**************************************************************************************
* AUTHOR : FaEry
* DATE   : 2019-12-30
* MODULE : common.h
*
* Command: 
*	IOCTRL Common Header
*
* Description:
*	Common data for the IoCtrl driver and application
*
****************************************************************************************
* Copyright (C) 2010 FaEry.
****************************************************************************************/

#pragma once 

//#######################################################################################
// D E F I N E S
//#######################################################################################

#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif

//不支持符号链接用户相关性
#define NT_DEVICE_NAME                  L"\\Device\\devProbeMemory"             // Driver Name
#define SYMBOLIC_LINK_NAME              L"\\DosDevices\\ProbeMemory"            // Symbolic Link Name
#define WIN32_LINK_NAME                 "\\\\.\\ProbeMemory"                    // Win32 Link Name


//
// Device IO Control Codes
//
#define IOCTL_BASE          0x800
#define MY_CTL_CODE(i)        \
    CTL_CODE                  \
    (                         \
        FILE_DEVICE_UNKNOWN,  \
        IOCTL_BASE + i,       \
        METHOD_BUFFERED,      \
        FILE_ANY_ACCESS       \
    )

//
// TODO: Add your IOCTL define here
//

#define  IOCTL_ENUM_DRIVER    MY_CTL_CODE(0x20)

//
// TODO: Add your struct,enum(public) define here
//


/* EOF */

