# ProbeMemory

This project shows how to determine that a paged-out memory is valid, sometimes MmIsAddressValid is not reliable, 
and we can try to lock which seems to be invalid.

- - -
  
## Using Driver Verifier

Let all memory in paged pool page out，just using Microsoft Kernel Tools named Driver Verifier, which locates in %windir%\System32\verifier.exe. then tick the option of [Force IRQL Checking](https://docs.microsoft.com/en-us/windows-hardware/drivers/devtest/force-irql-checking), finally we need to raise Irql level before accessing paged-out memory.

![](https://github.com/FaEryICE/ProbeMemory/blob/master/pngs/forceirqlchecking_verifier.png)

- - -

## Project 

driver is compiled by **WDK 7600 + VS2010**<br/>
user application is compiled by **VS2010**


- - -
## Directory Tree
```
|   README.md
|
+---driver
|   \---ProbeMemory
|           BuildDrv.bat
|           clean.bat
|           common.h
|           makefile
|           ProbeMemory.cpp
|           ProbeMemory.h
|           ProbeMemory.sln
|           ProbeMemory.vcxproj
|           ProbeMemory.vcxproj.filters
|           public.cpp
|           public.h
|           sources
|           struct.h
|           SysModule.cpp
|           SysModule.h
|
\---user
    \---UserCtrl
        |   UserCtrl.sln
        |
        \---UserCtrl
                common.h
                ReadMe.txt
                stdafx.cpp
                stdafx.h
                targetver.h
                UserCtrl.cpp
                UserCtrl.vcxproj
                UserCtrl.vcxproj.filters
                UserCtrl.vcxproj.user
```
