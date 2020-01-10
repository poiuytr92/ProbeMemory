/***************************************************************************************
* AUTHOR : FaEry
* DATE   : 2019-12-30
* MODULE : ProbeMemory.cpp
*
****************************************************************************************
* Copyright (C) 2019 FaEry.
****************************************************************************************/

#include "ProbeMemory.h"
#include "SysModule.h"

PDRIVER_OBJECT  g_MyDriverObject = NULL;

//-----------------------------------------------------------------------------------------------------------------------
VOID NTAPI EnumDriverCallBack(PDRIVER_INFO lpDriverInfo, PVOID lpContext)
{

}
//-----------------------------------------------------------------------------------------------------------------------
NTSTATUS
    DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
    NTSTATUS        status       = STATUS_SUCCESS;
    UNICODE_STRING  ustrLinkName = {0};
    UNICODE_STRING  ustrDevName  = {0};
    PDEVICE_OBJECT  pDevObj;
    int             i            = 0;

    dprintf("[%s] Compiled %s %s\r\nIn DriverEntry : %wZ\r\n",
        __FUNCTION__, __DATE__, __TIME__, pRegistryString);

    // Register dispatch routines
    for(i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
        pDriverObj->MajorFunction[i] = DispatchCommon;
    }

    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;

    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

    // Unload routine
    pDriverObj->DriverUnload = DriverUnload;

    // Initialize the device name.
    RtlInitUnicodeString(&ustrDevName, NT_DEVICE_NAME);

    // Create the device object and device extension
    status = IoCreateDevice(pDriverObj, 
        0,
        &ustrDevName, 
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &pDevObj);

    if(!NT_SUCCESS(status))
    {
        dprintf("Error, IoCreateDevice = 0x%x\r\n", status);
        return status;
    }

    RtlInitUnicodeString(&ustrLinkName, SYMBOLIC_LINK_NAME);

    // Create a symbolic link to allow USER applications to access it. 
    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);  
    if(!NT_SUCCESS(status))
    {
        dprintf("Error, IoCreateSymbolicLink = 0x%x\r\n", status);

        IoDeleteDevice(pDevObj); 
        return status;
    }

    //
    //  TODO: Add initialization code here.
    //
    g_MyDriverObject = pDriverObj;

    dprintf("DriverEntry Success\r\n");

    return STATUS_SUCCESS;
}

VOID
    DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;

    // Unloading - no resources to free so just return.
    dprintf("Unloading...\r\n");

    //
    // TODO: Add uninstall code here.
    //

    // Delete the symbolic link
    RtlInitUnicodeString(&strLink, SYMBOLIC_LINK_NAME);
    IoDeleteSymbolicLink(&strLink);

    // Delete the DeviceObject
    IoDeleteDevice(pDriverObj->DeviceObject);

    dprintf("Unloaded Success\r\n");

    return;
}

NTSTATUS
    DispatchCreate(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;

    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}


NTSTATUS
    DispatchClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;

    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    // Return success
    return STATUS_SUCCESS;
}

NTSTATUS
    DispatchCommon(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0L;

    IoCompleteRequest( pIrp, 0 );

    // Return success
    return STATUS_SUCCESS;
}

NTSTATUS 
    DispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
    NTSTATUS status               = STATUS_INVALID_DEVICE_REQUEST;	 // STATUS_UNSUCCESSFUL
    PIO_STACK_LOCATION pIrpStack  = IoGetCurrentIrpStackLocation(pIrp);
    ULONG uIoControlCode          = 0;
    PVOID pIoBuffer               = NULL;
    ULONG uInSize                 = 0;
    ULONG uOutSize                = 0;

    // Get the IoCtrl Code
    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;

    pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    switch(uIoControlCode)
    {
    case IOCTL_ENUM_DRIVER:
        {
            EnumDriverByWalkDriverObjectLdr(g_MyDriverObject, EnumDriverCallBack, NULL);
        }
        break;
        //
        // TODO: Add execute code here.
        //

    default:
        {
            // Invalid code sent
            dprintf("Unknown IOCTL: 0x%X (%04X,%04X)\r\n", 
                uIoControlCode,
                DEVICE_TYPE_FROM_CTL_CODE(uIoControlCode),
                IoGetFunctionCodeFromCtlCode(uIoControlCode));
            status = STATUS_INVALID_PARAMETER;	
        }
        break;
    }

    if(status == STATUS_SUCCESS)
    {
        pIrp->IoStatus.Information = uOutSize;
    }
    else
    {
        pIrp->IoStatus.Information = 0;
    }

    // Complete the I/O Request
    pIrp->IoStatus.Status = status;

    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}


//
// TODO: Add your module definitions here.
//



/* EOF */
