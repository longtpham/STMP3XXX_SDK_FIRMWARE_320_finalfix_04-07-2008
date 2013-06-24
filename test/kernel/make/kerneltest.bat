@echo off
rem  Filename: kerneltest.bat
rem 
rem  Check for minimum num of arguments
goto :CheckArgs
:EchoUsage
@echo off
rem  This batch file is written to work with Win2k/Me/98, so
rem  the logic is simplified due to limitations in batch
rem  processing in Me/98
echo ***************************************************************************
echo ; Usage : kerneltest [ALL] DEVICE [SDK2400] [LIBS]
echo ;
echo ;      Tests the SigmaTel STMP3xxx kernel. 
echo ;      This kernel version is not used in SDK 2.400 or earlier.
echo ;     
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                       This param must be first if it exists.
echo ;      DEVICE       = D3410 or D3500 (required)
echo ;      SDK2400      = for SDK 2.4xx based projects (NAND HAL lib)
echo ;      LIBS         = Forces libraries to build
echo ;                       (defaults to pre-built libs)
echo ***************************************************************************
@echo off
goto :ERROR

:CheckArgs
if not "%1"=="" goto :EnoughArgs
echo *******************************************
echo ; Usage error : Not enough arguments
goto :EchoUsage
:EnoughArgs

if "%1"=="D3410" goto :DeviceTypeOk
if "%2"=="D3410" goto :DeviceTypeOk
if "%3"=="D3410" goto :DeviceTypeOk             
if "%4"=="D3410" goto :DeviceTypeOk             

if "%1"=="D3500" goto :DeviceTypeOk             
if "%2"=="D3500" goto :DeviceTypeOk             
if "%3"=="D3500" goto :DeviceTypeOk             
if "%4"=="D3500" goto :DeviceTypeOk             
echo *******************************************
echo ; Usage error : Supported DEVICE type not entered 
goto :EchoUsage
:DeviceTypeOk

call ..\..\..\dspenv.bat

:BUILD
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
if "%1" == "ALL" echo ******************************************* 
if "%1" == "ALL" echo **********   Rebuilding ALL     ***********   
if "%1" == "ALL" echo ******************************************* 
if "%1" == "ALL" mk2410 -a -f kerneltest3.mk clean
if "%1" == "ALL" echo mk2410 -a -f kerneltest3.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if "%1" == "ALL" mk2410 -a -f kerneltest3.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE 
if not "%1" == "ALL" echo ******************************************* 
if not "%1" == "ALL" echo **********  Partial rebuild     ***********   
if not "%1" == "ALL" echo *******************************************
if not "%1" == "ALL" echo mk2410 -f kerneltest3.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE 
if not "%1" == "ALL" mk2410 -f kerneltest3.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE 

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ************ BUILD SUCCESSFUL  ************
echo ******************************************* 
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!!!!!! BUILD ERRORS !!!!!!!!!!!!!!
echo *******************************************
:EXIT

set BLD=
set C_ASM_OPTS=
