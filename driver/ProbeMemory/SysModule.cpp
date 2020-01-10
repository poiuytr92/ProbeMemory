#include "SysModule.h"

NTSTATUS EnumDriverByWalkDriverObjectLdr(PDRIVER_OBJECT pDriverObject, PFN_EnumDriverCallBack Callback, PVOID lpContext)
{
    NTSTATUS              Status        = STATUS_SUCCESS;
    PLIST_ENTRY           pListHeader   = NULL;
    PLIST_ENTRY           pListEntry    = NULL;
    PLDR_DATA_TABLE_ENTRY pLdrEntry     = NULL;
    ULONG                 ulCount       = 0;
    ULONG                 ulMaxCount    = 500;
    PMDL                  lpMDL         = NULL;

    if (!pDriverObject || !Callback)
    {
        return STATUS_INVALID_PARAMETER;
    }

    pListHeader = (PLIST_ENTRY)pDriverObject->DriverSection;
    pLdrEntry   = CONTAINING_RECORD(pListHeader, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks.Flink);

    pListEntry  = (PLIST_ENTRY)pLdrEntry->InLoadOrderLinks.Flink;

    while (TRUE)
    {
        pLdrEntry   = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks.Flink);
        if (!MmIsAddressValid(pLdrEntry))
        {
            break;
        }

#ifdef _WIN64
        if (IsUnicodeStringValid(&pLdrEntry->BaseDllName, KernelMode, &lpMDL))
#else
        if (IsUnicodeString32Valid(&pLdrEntry->BaseDllName, KernelMode, &lpMDL))
#endif
        {
            if (lpMDL)
            {
                KdPrint(("[%s] LockMemory BaseDllName:%wZ\r\n", __FUNCTION__, &pLdrEntry->BaseDllName));
                UnLockMemory(lpMDL);
                lpMDL = NULL;
            }
        }

#ifdef _WIN64
        if (IsUnicodeStringValid(&pLdrEntry->FullDllName, KernelMode, &lpMDL))
#else
        if (IsUnicodeString32Valid(&pLdrEntry->FullDllName, KernelMode, &lpMDL))
#endif
        {
            if (lpMDL)
            {
                KdPrint(("[%s] LockMemory FullDllName:%wZ\r\n", __FUNCTION__, &pLdrEntry->BaseDllName));
                UnLockMemory(lpMDL);
                lpMDL = NULL;
            }
        }

#ifdef _WIN64
        KdPrint(("[%s] Ldr DllBase:%I64x, SizeOfImage:%x\r\n", __FUNCTION__, pLdrEntry->DllBase, pLdrEntry->SizeOfImage));
#else
        KdPrint(("[%s] Ldr DllBase:%x, SizeOfImage:%x\r\n", __FUNCTION__, pLdrEntry->DllBase, pLdrEntry->SizeOfImage));
#endif

        if (pListEntry == pListHeader)
        {
            break;
        }

        ulCount++;
        if (ulCount >= ulMaxCount)
        {
            break;
        }

        pListEntry = pListEntry->Flink;
    }

    return Status;
}