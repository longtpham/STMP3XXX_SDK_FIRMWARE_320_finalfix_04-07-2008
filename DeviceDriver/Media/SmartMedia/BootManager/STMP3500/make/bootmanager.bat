@echo off
rem  Filename: bootmanager.bat 
rem  Check for minimum number of arguments

:EchoUsage
@echo off
rem  This batch file is written to work with WinXP/2k/Me/98, so
rem  the logic is simplified due to limitations in batch
rem  processing in Me/98
echo *******************************************
echo ; Note: Usage print does not indicate error. 
echo ;
echo ; Usage : bootmanager ALL [EEPROM]
echo ;
echo ;  Runs after STMP35xx ROM and loads either the USBMSC or player firmware
echo ;  from flash usually. In boot EEPROM case, ROM loads EEPROM image of this 
echo ;  program via I2C and then typically loads player or USBMSC from flash or
echo ;  performs USB boot which is known as firmware recovery.
echo ; 
echo ;  ALL       = Rebuilds all files (must be first parameter)
echo ; [EEPROM]   = Extern EEPROM build. Use with ALL. 
echo ;  
echo ;  Notes:EEPROM build (I2C boot) is superset of bootmanager functionality
echo ;        EEPROM must be second parameter when used.
echo ;        Flash type parameter is only used with EEPROM builds. 
echo ;        Type 1 is small block single level cell flash. 2 is large block. 
echo ;        Type 4 flash has parallel multi-write potential (Renesas).
echo ;        Type 5 is 2kB page size MLC (eg Toshiba)
echo ;        Type 6 is 2kB page size MLC (samsung, hynix)
echo ;        Type 7 could do dual simultaneous page writes (will replace samsung type 2)
echo *******************************************
@echo off

call ..\..\..\..\..\..\dspenv.bat

rem undefine BLD. make file has BLD by default
set BLD=

rem For internal projects, we always want to test the librarie
rem dependancies
set NO_LIB_DEPENDANCIES=

if "%1"=="" echo No parameters present
if "%1"=="" goto :Exit

REM Note: this eeprom block must match the default in bootmanager.mk
if not "%2"=="EEPROM" echo Building non-EEPROM version of bootmanager
if not "%2"=="EEPROM" goto :NOT_EEPROM

REM Note that BLD & CC_OPTS for third and fourth parameters get set in bootmanager.mk
set BLD=-D%1 -D%2 
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 
if not "%3"=="" set C_ASM_OPTS=-Wa-D%3
if "%2" == "EEPROM" echo *********** Building EEPROM version *******
goto :READY

:NOT_EEPROM
set BLD=-DD3500
set C_ASM_OPTS=-Wa-DD3500

:READY
rem WinMe doesn't support "(" in an if statement, this is ugly below
if "%1" == "ALL" echo ******************************************* 
if "%1" == "ALL" echo **********   Rebuilding ALL     ***********   
if "%1" == "ALL" echo ******************************************* 
REM if "%1" == "ALL" mk2410 -a -f bootmanager.mk D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if "%1" == "ALL" mk2410 -a -f bootmanager.mk clean D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if "%1" == "ALL" echo mk2410 -a -f bootmanager.mk D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if "%1" == "ALL" mk2410 -a -f bootmanager.mk D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
goto ENDBUILD

REM Note: As in the previous version of this block, 
REM       these commands are unreachable due to the goto statement above. ALL param is required. 
if not "%1" == "ALL" echo ******************************************* 
if not "%1" == "ALL" echo **********  Partial rebuild     ***********   
if not "%1" == "ALL" echo *******************************************
REM if not "%1" == "ALL" mk2410 -f bootmanager.mk D_D3500=TRUE 
if not "%1" == "ALL" mk2410 -a -f bootmanager.mk clean D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if not "%1" == "ALL" echo mk2410 -a -f bootmanager.mk D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if not "%1" == "ALL" mk2410 -a -f bootmanager.mk D_D3500=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE

:ENDBUILD
if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ************ BUILD SUCCESSFUL *************
echo ******************************************* 
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!!!!!! BUILD ERRORS !!!!!!!!!!!!!!
echo *******************************************
:EXIT

set BLD=
set C_ASM_OPTS=

