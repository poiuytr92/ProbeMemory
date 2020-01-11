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
        if (!IsMemoryValid(pLdrEntry, sizeof(LDR_DATA_TABLE_ENTRY), KernelMode, &lpMDL))
        {
            break;
        }

        if (lpMDL)
        {
            UnLockMemory(lpMDL);
            lpMDL = NULL;
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
        else
        {
#ifdef _WIN64
            KdPrint(("[%s] BaseDllName Length:%x MaxLength:%x Buffer:%I64x\r\n", __FUNCTION__, pLdrEntry->BaseDllName.Length, pLdrEntry->BaseDllName.MaximumLength, pLdrEntry->BaseDllName.Buffer));
#else
            KdPrint(("[%s] BaseDllName Length:%x MaxLength:%x Buffer:%x\r\n", __FUNCTION__, pLdrEntry->BaseDllName.Length, pLdrEntry->BaseDllName.MaximumLength, pLdrEntry->BaseDllName.Buffer));
#endif
        }

#ifdef _WIN64
        if (IsUnicodeStringValid(&pLdrEntry->FullDllName, KernelMode, &lpMDL))
#else
        if (IsUnicodeString32Valid(&pLdrEntry->FullDllName, KernelMode, &lpMDL))
#endif
        {
            if (lpMDL)
            {
                KdPrint(("[%s] LockMemory FullDllName:%wZ\r\n", __FUNCTION__, &pLdrEntry->FullDllName));
                UnLockMemory(lpMDL);
                lpMDL = NULL;
            }
        }
        else
        {
#ifdef _WIN64
            KdPrint(("[%s] FullDllName Length:%x MaxLength:%x Buffer:%I64x\r\n", __FUNCTION__, pLdrEntry->FullDllName.Length, pLdrEntry->FullDllName.MaximumLength, pLdrEntry->FullDllName.Buffer));
#else
            KdPrint(("[%s] FullDllName Length:%x MaxLength:%x Buffer:%x\r\n", __FUNCTION__, pLdrEntry->FullDllName.Length, pLdrEntry->FullDllName.MaximumLength, pLdrEntry->FullDllName.Buffer));
#endif
        }

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