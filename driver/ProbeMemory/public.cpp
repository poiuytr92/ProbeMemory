#include "public.h"

//--------------------------------------------------------------------------------------------------------------
BOOLEAN IsAddressPagesValid(PVOID lpAddress, ULONG ulSize)
{
    BOOLEAN   bResult     = TRUE;
    ULONG_PTR ulStartAddr = 0;
    ULONG_PTR ulEndAddr   = 0;

    if (!lpAddress || !MmIsAddressValid(lpAddress))
    {
        return FALSE;
    }

    ulStartAddr = (ULONG_PTR)ALIGN_UP_POINTER_BY(lpAddress, PAGE_SIZE);
    ulEndAddr   = (ULONG_PTR)lpAddress + ulSize;

    while (TRUE)
    {
        if (ulStartAddr >= ulEndAddr)
        {
            break;
        }

        if (!MmIsAddressValid((PVOID)ulStartAddr))
        {
            bResult = FALSE;
            break;
        }

        ulStartAddr += PAGE_SIZE;
    }

    return bResult;
}
//--------------------------------------------------------------------------------------------------------------
BOOLEAN TryToLockMemory(PVOID lpAddr, ULONG ulSize, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    BOOLEAN bRet      = FALSE;
    PMDL    pAllocMDL = NULL;

    if (!lpAddr || !ulSize || !lpMDL)
    {
        return FALSE;
    }

    pAllocMDL = IoAllocateMdl(lpAddr, ulSize, FALSE, FALSE, NULL);
    if (!pAllocMDL)
    {
        return FALSE;
    }

    __try
    {
        MmProbeAndLockPages(pAllocMDL, Mode, IoReadAccess);
        *lpMDL = pAllocMDL;
        bRet   = TRUE;
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        IoFreeMdl(pAllocMDL);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------------------------------
VOID UnLockMemory(PMDL lpMDL)
{
    if (lpMDL)
    {
        MmUnlockPages(lpMDL);
        IoFreeMdl(lpMDL);
    }
}
//--------------------------------------------------------------------------------------------------------------
BOOLEAN IsMemoryValid(PVOID lpAddress, ULONG ulSize, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    if (!IsAddressPagesValid(lpAddress, ulSize))
    {
        return TryToLockMemory(lpAddress, ulSize, Mode, lpMDL);
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------------------------------
BOOLEAN IsUnicodeStringValid(PUNICODE_STRING uniString, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    if (!uniString->Length || !uniString->Buffer)
    {
        return FALSE;
    }

    return IsMemoryValid(uniString->Buffer, uniString->Length, Mode, lpMDL);
}
//--------------------------------------------------------------------------------------------------------------
BOOLEAN IsUnicodeString32Valid(PUNICODE_STRING32 uniString, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    BOOLEAN bRet   = FALSE;
    PVOID   lpAddr = NULL;

    if (!uniString->Length || !uniString->Buffer)
    {
        return FALSE;
    }

    lpAddr = (PVOID)uniString->Buffer;
    return IsMemoryValid(lpAddr, uniString->Length, Mode, lpMDL);
}
//--------------------------------------------------------------------------------------------------------------
