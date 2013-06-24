@echo off
rem  Filename:   mtp.bat (LCD based example)
rem  
rem  Check for minimum num of arguments
goto :CheckArgs
:EchoUsage
@echo off
echo ***************************************************************************
echo ; Usage : mtp [ALL] [BUILDTYPE] [INTMEDIA] [BATTERY] [C_LCD] [C_LCD_16BIT]
echo ;                                           [DRM] [JANUS] [AUDIBLEDEC] [LIBS]
echo ; 
echo ; Media Transport Protocol for portable file storage digital audio products w/ LCD
echo ;
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                       This param must be first if it exists.
echo ;      BUILDTYPE    = DEBUG or RETAIL (defaults to DEBUG)
echo ;      INTMEDIA     = NAND1, NAND2, NAND3, NAND4 (physical num. Defaults to NAND1)
echo ;      BATTERY      = LIION, NIMHY, or defaults to non-rechargeable.
echo ;      C_LCD        = Enable general color LCD driver. (8 bit color; 8 bit monochrome default)
echo ;      C_LCD_16BIT  = Enable LCD 16 bit interface (must also use C_LCD param)
echo ;                     Requires C_LCD option also which gives general color support. 
echo ;      DRM          = DRM enables Digital Rights Management (Use Media Serial Num)
echo ;                       (defaults to DRM disabled)
echo ;      JANUS        = JANUS enables. Must also specify DRM. Supports both DRM types 
echo ;                           (default: disabled)
echo ;      AUDIBLEDEC   = Add Audible firmware (Not Tested in SDK 3.110)
echo ;      LIBS         = Rebuild libs (like io, stringlib) distributed with source.
echo ;                        only required if change source related to these libs
echo ;       
echo ;      PLIBS        = ** SIGMATEL INTERNAL USAGE ONLY ** 
echo ;                        forces rebuild of all private libs
echo ;      NOTE:        Unlimited number of parameters due to internal shift command. 
echo ;                   Select contrast range for your LCD in lcdexample\project.inc
echo ***************************************************************************
@echo off
goto :SILENT_EXIT

:CheckArgs
if not "%1"=="" goto :EnoughArgs
echo *******************************************
echo ; Usage error : Not enough arguments
goto :EchoUsage
:EnoughArgs


if "%0"=="JANUS" goto OK_JANUS
if "%1"=="JANUS" goto OK_JANUS
if "%2"=="JANUS" goto OK_JANUS
if "%3"=="JANUS" goto OK_JANUS
if "%4"=="JANUS" goto OK_JANUS
if "%5"=="JANUS" goto OK_JANUS
if "%6"=="JANUS" goto OK_JANUS
if "%7"=="JANUS" goto OK_JANUS
if "%8"=="JANUS" goto OK_JANUS
if "%9"=="JANUS" goto OK_JANUS
echo *******************************************
echo ; Usage error : If there is not "JANUS", please use "USBMSC" to build "Hostlink"
goto :EchoUsage

:OK_JANUS

set BLD=
set CMD_LINE=
set ALL=TRUE

call ..\..\..\..\..\dspenv.bat

rem Add "del mtpdeviceinfodata.c" in order to resolve Stmp00013802
del /F ..\mtpdeviceinfodata.c
start /w ..\..\..\..\..\bin\StMtpDIGen /i ..\mtpdeviceinfo.txt  /o ..\mtpdeviceinfodata.c

if "%1" == "ALL" goto All
echo *******************************************
echo **********  Partial rebuild     ***********
echo *******************************************
goto Loop
:All
echo ******************************************* 
echo **********   Rebuilding ALL     ***********   
echo ******************************************* 

rem 
rem Add the -a to the command line to build everything regardless of timestamp.
rem
set CMD_LINE=-a %CMD_LINE%

rem 
rem Do a clean build first.
rem 
mk2410 -f mtp.mk clean

:Loop
if "%1"=="" goto Continue
REM if "%1"=="PLIBS" goto EchoUsage
set BLD=%BLD% -D%1
set CMD_LINE=%CMD_LINE% D_%1=TRUE

if not "%1"=="AUDIBLEDEC" goto NextParam
start /w ..\..\..\..\..\bin\StMtpDIGen /a /i ..\mtpdeviceinfo.txt  /o ..\mtpdeviceinfodata.c

:NextParam
shift
goto Loop
:Continue

echo mk2410 -f mtp.mk %CMD_LINE%
mk2410 -f mtp.mk %CMD_LINE%

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ********** MTP BUILD SUCCESSFUL ***********
echo ******************************************* 
rem echo Creating ..\output_3500\FindFreeResultMtp.txt
..\..\..\..\..\bin\findfree.exe ..\output_3500\hostlink.map C000 6000 6000>..\output_3500\FindFreeResultMtp.txt
echo                                               P,X,Y free:
rem  Next line displays P X Y free numbers. 
find "s **" ..\output_3500\FindFreeResultMtp.txt
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!! MTP BUILD ERRORS !!!!!!!!!!!!!!
echo *******************************************
:EXIT
rem Being here avoids BUILD ERRORS print from possibly being caused by analyzemem. 
echo Creating ..\output_3500\AnalyzeMemMtp.html
..\..\..\..\..\bin\analyzemem.exe -s -d stmp3500.dsc -m ..\output_3500\hostlink.map -o ..\output_3500\AnalyzeMemMtp.html
echo Creating ..\output_3500\CheckMemResultMtp.txt
..\..\..\..\..\bin\checkmem.exe ..\output_3500\hostlink.map >..\output_3500\CheckMemResultMtp.txt
type  ..\output_3500\CheckMemResultMtp.txt
:SILENT_EXIT

set BLD=
set CMD_LINE=
set ALL=
set C_ASM_OPTS=
