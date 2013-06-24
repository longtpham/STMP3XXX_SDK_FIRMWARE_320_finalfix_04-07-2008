@echo off
rem check for minimum of one arguments
goto :CheckArgs
:EchoUsage
rem  This batch file is written to work with Win2k/Me/98, so
rem  the logic is simplified due to limitations in batch
rem  processing in Me/98
echo *******************************************
echo ; Usage : fattest [ALL] DEVICE [INTMEDIA] [EXTMEDIA] [EXT] [LIBS]
echo ;
echo ;      ALL     = rebuilds all files (must be first param if present)
echo ;
echo ;      DEVICE   = D3500 (required parameter)
echo ;
echo ;      INTMEDIA     = NAND1, NAND2, NAND3, NAND4 (defaults to NAND1)
echo ;
echo ;      EXTMEDIA = MMC (optional)
echo ;
echo ;      EXT      = builds extended tests (unverified lately)
echo ;
echo ;      LIBS    = Rebuilds libs: fat12, io, stringlib
echo ;                Only required if change source related to these libs
echo ;
echo ;      PLIBS   = *** SIGMATEL USE ONLY *** Forces libraries to build
echo ;                       (defaults to pre-built libs)
echo ;      Notes:  Use emulator to see any failure details. DEBUG build.
echo ;              SDK2400 defines present by default.
echo *******************************************
@echo off
goto :ERROR

:CheckArgs
if not "%1"=="" goto :EnoughArgs
echo *******************************************
echo ; Usage error : Not enough arguments
goto :EchoUsage
:EnoughArgs
if "%1"=="D3500" goto :DeviceTypeOk             
if "%2"=="D3500" goto :DeviceTypeOk             
if "%3"=="D3500" goto :DeviceTypeOk             
if "%4"=="D3500" goto :DeviceTypeOk  
if "%5"=="D3500" goto :DeviceTypeOk
if "%6"=="D3500" goto :DeviceTypeOk
  
echo *******************************************
echo ; Usage error : Supported DEVICE type not entered
goto :EchoUsage
:DeviceTypeOk

call ..\..\..\dspenv.bat

set BLD=
set NO_LIB_DEPENDANCIES=
set fat32test_EXTENDED=

if "%1"=="EXT" goto :ExtendedTests
if "%2"=="EXT" goto :ExtendedTests
if "%3"=="EXT" goto :ExtendedTests
if "%4"=="EXT" goto :ExtendedTests
if "%5"=="EXT" goto :ExtendedTests
if "%6"=="EXT" goto :ExtendedTests
if not "%7"=="EXT" goto :NoExtendedTests
:ExtendedTests
echo *******************************************
echo **********   Extended Tests     ***********   
echo *******************************************
set fat32test_EXTENDED=fat32test_EXTENDED
set BLD=%BLD% -Dfat32test_EXTENDED
:NoExtendedTests

:BUILD


:ARG7
if "%7"=="" goto :ARG6
set BLD=-D%1 -D%2 -D%3 -D%4 -D%5 -D%6 -D%7
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4 -Wa-D%5 -Wa-D%6 -Wa-D%7
goto :READY

:ARG6
if "%6"=="" goto :ARG5
set BLD=-D%1 -D%2 -D%3 -D%4 -D%5 -D%6
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4 -Wa-D%5 -Wa-D%6
goto :READY

:ARG5
if "%5"=="" goto :ARG4
set BLD=-D%1 -D%2 -D%3 -D%4	-D%5
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4 -Wa-D%5
goto :READY

:ARG4
if "%4"=="" goto :ARG3
set BLD=-D%1 -D%2 -D%3 -D%4
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4
goto :READY
:ARG3
if "%3"=="" goto :ARG2
set BLD=-D%1 -D%2 -D%3
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3
goto :READY
:ARG2
if "%2"=="" goto :ARG1
set BLD=-D%1 -D%2
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2
goto :READY
:ARG1
set BLD=-D%1
set C_ASM_OPTS=-Wa-D%1

:READY
rem WinMe doesn't support "(" in an if statement, this is ugly below
if not "%1" == "ALL" goto :PartialBuild
echo *******************************************
echo **********   Rebuilding ALL     ***********
echo *******************************************
mk2410 -a -f fat32test.mk clean D_%2=TRUE
goto :Build

:PartialBuild
echo *******************************************
echo **********  Partial rebuild     ***********
echo *******************************************

:Build
echo on
mk2410 -f fat32test.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE
@echo off

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ************ BUILD SUCCESSFULL ************
echo ******************************************* 
goto :EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!!!!!! BUILD ERRORS !!!!!!!!!!!!!!
echo *******************************************
:EXIT

set BLD=
set C_ASM_OPTS=
