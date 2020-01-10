#include "public.h"

//--------------------------------------------------------------------------------------------------------------
BOOLEAN MmIsAddressPagesValid(PVOID lpAddress, ULONG ulSize)
{
    ULONG_PTR ulStartAddr = 0;
    ULONG_PTR ulEndAddr   = 0;

    if (!lpAddress || !MmIsAddressValid(lpAddress))
    {
        return FALSE;
    }

    ulStartAddr = ALIGN_UP(lpAddress, PAGE_SIZE);
    ulEndAddr   = (ULONG_PTR)lpAddress + ulSize;

    for ( ; ulStartAddr < ulEndAddr ; ulStartAddr += PAGE_SIZE )
    {
        if (!MmIsAddressValid((PVOID)ulStartAddr))
        {
            return FALSE;
        }
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------------------------------
BOOLEAN TryToLockMemory(PVOID lpAddr, ULONG ulSize, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    BOOLEAN bRet      = FALSE;
    PMDL    pAllocMDL = NULL;

    KdPrint(("[%s] Get Called\r\n", __FUNCTION__));

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
BOOLEAN IsUnicodeStringValid(PUNICODE_STRING uniString, KPROCESSOR_MODE Mode, PMDL* lpMDL)
{
    BOOLEAN bRet = FALSE;

    if (!uniString->Length || !uniString->Buffer)
    {
        return FALSE;
    }

    if (!MmIsAddressPagesValid(uniString->Buffer, uniString->Length))
    {
        return TryToLockMemory(uniString->Buffer, uniString->Length, Mode, lpMDL);
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
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
    if (!MmIsAddressPagesValid(lpAddr, uniString->Length))
    {
        return TryToLockMemory(lpAddr, uniString->Length, Mode, lpMDL);
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}
//--------------------------------------------------------------------------------------------------------------
