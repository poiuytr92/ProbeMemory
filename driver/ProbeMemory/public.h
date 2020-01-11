#ifndef _PUBLIC_H
#define _PUBLIC_H

#include <ntddk.h>

#define MAX_PATH                  260
#define PAGE_SIZE                 0x1000
//#define ALIGN_UP(x, y)            ((ULONG_PTR)x + ((ULONG_PTR)y - (ULONG_PTR)x%(ULONG_PTR)y))
//#define ALIGN_DOWN(x, y)          ((ULONG_PTR)x - ((ULONG_PTR)x%(ULONG_PTR)y))

typedef struct _LDR_DATA_TABLE_ENTRY32
{
    LIST_ENTRY32 InLoadOrderLinks;
    LIST_ENTRY32 InMemoryOrderLinks;
    LIST_ENTRY32 InInitializationOrderLinks;
    ULONG DllBase;
    ULONG EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING32 FullDllName;
    UNICODE_STRING32 BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    union {
        LIST_ENTRY32 HashLinks;
        struct {
            ULONG SectionPointer;
            ULONG CheckSum;
        };
    };
    union {
        struct {
            ULONG TimeDateStamp;
        };
        struct {
            ULONG LoadedImports;
        };
    };
} LDR_DATA_TABLE_ENTRY32, *PLDR_DATA_TABLE_ENTRY32;

typedef struct _LDR_DATA_TABLE_ENTRY64
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    union {
        LIST_ENTRY HashLinks;
        struct {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    union {
        struct {
            ULONG TimeDateStamp;
        };
        struct {
            PVOID LoadedImports;
        };
    };
} LDR_DATA_TABLE_ENTRY64, *PLDR_DATA_TABLE_ENTRY64;


#ifndef _WIN64
typedef LDR_DATA_TABLE_ENTRY32  LDR_DATA_TABLE_ENTRY;
typedef PLDR_DATA_TABLE_ENTRY32 PLDR_DATA_TABLE_ENTRY;
#else
typedef LDR_DATA_TABLE_ENTRY64  LDR_DATA_TABLE_ENTRY;
typedef PLDR_DATA_TABLE_ENTRY64 PLDR_DATA_TABLE_ENTRY;
#endif


BOOLEAN IsAddressPagesValid(PVOID lpAddress, ULONG ulSize);
BOOLEAN TryToLockMemory(PVOID lpAddr, ULONG ulSize, KPROCESSOR_MODE Mode, PMDL* lpMDL);
VOID    UnLockMemory(PMDL lpMDL);
BOOLEAN IsMemoryValid(PVOID lpAddress, ULONG ulSize, KPROCESSOR_MODE Mode, PMDL* lpMDL);
BOOLEAN IsUnicodeStringValid(PUNICODE_STRING uniString, KPROCESSOR_MODE Mode, PMDL* lpMDL);
BOOLEAN IsUnicodeString32Valid(PUNICODE_STRING32 uniString, KPROCESSOR_MODE Mode, PMDL* lpMDL);

#endif