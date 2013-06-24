@echo off
rem  Filename:   usbmsc.bat (LCD based example)
rem
rem  Check for minimum num of arguments
goto :CheckArgs
:EchoUsage
@echo off
echo ***************************************************************************
echo ; Usage : usbmsc [ALL] [BUILDTYPE] [INTMEDIA] [EXTMEDIA] [BATTERY] [DRM] [C_LCD] [C_LCD_16BIT]
echo ;                                                [LIBS] [UPDATER] [MFGTOOL]
echo ;
echo ; USB Mass Storage Class for portable file storage digital audio products w/ LCD
echo ;     In sdk3, use UPDATER param which builds updater.sb for usb boot mode
echo ;     (firmware recovery) formerly known as usbmsc.sb. Updater.sb is not flashed
echo ;     and only gets loaded to 35xx RAM across the USB cable from the USB host peer.
echo ;     Without UPDATER param, builds USBMSC non-overlay app.
echo ;     Note that SDK2.620 and before use this app for usb rather than the mtp.bat
echo ;     version of hostlink.
echo ;
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                       This param must be first if it exists.
echo ;      BUILDTYPE    = DEBUG or RETAIL (defaults to DEBUG)
echo ;      C_LCD             = use Color LCD device driver (defaults to monochrome)
echo ;      C_LCD_16BIT  = Enable LCD 16 bit interface (default: 8 bit interface)
echo ;      INTMEDIA     = NAND1, NAND2, NAND3, NAND4 (physical num. Defaults to NAND1)
echo ;                     NAND3 is the max option supported with MMC due to 4 chip select pin limit
echo ;      EXTMEDIA     = MMC (defaults to no external media; SMEDIA unsupported)
echo ;      BATTERY      = LIION, NIMHY, or defaults to non-rechargeable.
echo ;      DRM          = DRM enables Digital Rights Management (Use Media Serial Num)
echo ;                       (defaults to DRM disabled. DRM not needed in this build for sdk3)
echo ;      LIBS         = Rebuilds libs: fat12, io, stringlib
echo ;                        only required if change source related to these libs
echo ;      PLIBS        = ** SIGMATEL INTERNAL USAGE ONLY **
echo ;                        forces rebuild of all libs
echo ;      UPDATER      = required when building updater.sb file to sett proper IDs
echo ;      MFGTOOL      = Builds USBMSC MFG Tool mode for fast parallel 35xx updates
echo ;
echo ;      NOTE:        Select contrast range for your LCD in lcdexample\project.inc
echo ***************************************************************************
@echo off
goto :SILENT_EXIT

:CheckArgs
if not "%1"=="" goto :EnoughArgs
echo *******************************************
echo ; Usage error : Not enough arguments
goto :EchoUsage
:EnoughArgs

if "%0"=="JANUS" goto Error_JANUS
if "%1"=="JANUS" goto Error_JANUS
if "%2"=="JANUS" goto Error_JANUS
if "%3"=="JANUS" goto Error_JANUS
if "%4"=="JANUS" goto Error_JANUS
if "%5"=="JANUS" goto Error_JANUS
if "%6"=="JANUS" goto Error_JANUS
if "%7"=="JANUS" goto Error_JANUS
if "%8"=="JANUS" goto Error_JANUS
if "%9"=="JANUS" goto Error_JANUS
goto OK_No_JANUS

:Error_JANUS
echo *******************************************
echo ; Usage error : If there is "JANUS", please use "MTP" to build
goto :EchoUsage

:OK_No_JANUS


set BLD=
set CMD_LINE=
set ALL=TRUE

call ..\..\..\..\..\dspenv.bat

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
mk2410 -f usbmsc.mk clean

rem Default build type is now retail
set BuildType=

:Loop
if "%1"=="" goto Continue
if "%1"=="PLIBS" goto EchoUsage
if "%1"=="DEBUG" set BuildType=DEBUG
if "%1"=="RETAIL" set BuildType=RETAIL
set BLD=%BLD% -D%1
set CMD_LINE=%CMD_LINE% D_%1=TRUE
shift
goto Loop
:Continue

if "%BuildType%"=="" (
set CMD_LINE=%CMD_LINE% D_RETAIL=TRUE
)

echo mk2410 -f usbmsc.mk %CMD_LINE%
mk2410 -f usbmsc.mk %CMD_LINE%

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ********* USBMSC BUILD SUCCESSFUL  ********
echo *******************************************
rem echo Creating ..\output_3500\FindFreeResultUsbmsc.txt
..\..\..\..\..\bin\findfree.exe ..\output_3500\usbmsc.map 10000 2000 6000>..\output_3500\FindFreeResultUsbmsc.txt
echo                                               P,X,Y free:
rem  Next line displays P X Y free numbers.
find "s **" ..\output_3500\FindFreeResultUsbmsc.txt
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!! USBMSC BUILD ERRORS !!!!!!!!!!!!!
echo *******************************************
:EXIT
rem Being here avoids BUILD ERRORS print from being caused by analyzemem.
echo Creating ..\output_3500\AnalyzeMemUsbmsc.html
..\..\..\..\..\bin\analyzemem.exe -s -d stmp3500.dsc -m ..\output_3500\usbmsc.map -o ..\output_3500\AnalyzeMemUsbmsc.html
echo Creating ..\output_3500\CheckMemResultUsbmsc.txt
..\..\..\..\..\bin\checkmem.exe ..\output_3500\usbmsc.map >..\output_3500\CheckMemResultUsbmsc.txt
:SILENT_EXIT

set BLD=
set CMD_LINE=
set ALL=
set C_ASM_OPTS=
