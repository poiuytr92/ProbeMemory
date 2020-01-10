// UserCtrl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;


    hDevice = CreateFileW(WIN32_LINK_NAME,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE == hDevice)
    {
        printf("CreateFile failed\r\n");
        return 0;
    }

    if (DeviceIoControl(hDevice, IOCTL_ENUM_DRIVER, NULL, 0, NULL, 0, NULL, NULL))
    {
        printf("DeviceIoControl success\r\n");
    }

    CloseHandle(hDevice);

    return 0;
}

