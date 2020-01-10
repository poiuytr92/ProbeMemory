#ifndef _SYS_MODULE_H
#define _SYS_MODULE_H

#include "public.h"

typedef struct _DRIVER_INFO_
{
    ULONG_PTR LodeOrder;
    ULONG_PTR Base;
    ULONG_PTR Size;
    ULONG_PTR DriverObject;
    ULONG_PTR DirverStartAddress;
    WCHAR     wzDriverPath[MAX_PATH];
    WCHAR     wzKeyName[MAX_PATH];
}DRIVER_INFO, *PDRIVER_INFO;

typedef VOID (NTAPI* PFN_EnumDriverCallBack)(PDRIVER_INFO lpDriverInfo, PVOID lpContext);

NTSTATUS EnumDriverByWalkDriverObjectLdr(PDRIVER_OBJECT lpDriverObject, PFN_EnumDriverCallBack Callback, PVOID lpContext);

#endif
