@echo off
REM  FileName:    FMTunerTest.bat
REM  Description: Verify SigmaTel SOC's FM Tuner capability.

rem check for minimum of one argument
goto :CheckArgs
:EchoUsage
@echo off
rem  This batch file is written to work with Win2k/Me/98, so
rem  the logic is simplified due to limitations in batch
rem  processing in Me/98
echo ***************************************************************************
echo ; Usage : fmtunertest [ALL] DEVICE TUNERMODEL [LIBS]
echo ;
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                     This parameter must be first if it exists.
echo ;
echo ;      DEVICE       = D3410 or D3500 (required parameter)
echo ;
echo ;      TUNERMODEL   = FM5757, FM5767, or STFM1000 supported (required param)
echo ;                     FM24000(Sanyo FM tuner) not supported anymore
echo ;
echo ;      LIBS         = *** SIGMATEL USE ONLY *** Forces libraries to build
echo ;                       (defaults to pre-built libs)
echo ;
echo ;      Notes:        SDK2400 defines are automatic in this build. 
echo ;                    Supports and defaults to STMP3410 device. DEBUG build.
echo ***************************************************************************
@echo off
goto :ERROR

:CheckArgs
REM  Verify valid device arg is present
if "%1"=="D3410" goto :DeviceTypeOk
if "%2"=="D3410" goto :DeviceTypeOk
if "%3"=="D3410" goto :DeviceTypeOk
if "%4"=="D3410" goto :DeviceTypeOk
if "%1"=="D3500" goto :DeviceTypeOk             
if "%2"=="D3500" goto :DeviceTypeOk             
if "%3"=="D3500" goto :DeviceTypeOk             
if "%4"=="D3500" goto :DeviceTypeOk             

echo *******************************************
echo ; Usage error : A valid DEVICE param was not entered  
goto :EchoUsage
:DeviceTypeOk

REM  Require one of the arguments to be FM5767, FM5757, or STFM1000
if "%1"=="FM5757" goto :EnoughArgs 
if "%2"=="FM5757" goto :EnoughArgs 
if "%3"=="FM5757" goto :EnoughArgs 
if "%4"=="FM5757" goto :EnoughArgs 

if "%1"=="FM5767" goto :EnoughArgs 
if "%2"=="FM5767" goto :EnoughArgs 
if "%3"=="FM5767" goto :EnoughArgs 
if "%4"=="FM5767" goto :EnoughArgs

if "%1"=="STFM1000" goto :EnoughArgs 
if "%2"=="STFM1000" goto :EnoughArgs 
if "%3"=="STFM1000" goto :EnoughArgs 
if "%4"=="STFM1000" goto :EnoughArgs 
 
rem if "%1"=="FM24000" goto :EnoughArgs 
rem if "%2"=="FM24000" goto :EnoughArgs 
rem if "%3"=="FM24000" goto :EnoughArgs 
rem if "%4"=="FM24000" goto :EnoughArgs 

echo *******************************************
echo ; Usage error : A valid TUNERMODEL parameter was not entered 
goto :EchoUsage
:EnoughArgs

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
if "%1" == "ALL" mk2410 -a -f fmtunertest.mk clean D_%2=TRUE
if "%1" == "ALL" echo mk2410 -a -f fmtunertest.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if "%1" == "ALL" mk2410 -a -f fmtunertest.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE
if not "%1" == "ALL" echo ******************************************* 
if not "%1" == "ALL" echo **********  Partial rebuild     ***********   
if not "%1" == "ALL" echo *******************************************
if not "%1" == "ALL" echo mk2410 -f fmtunertest.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE 
if not "%1" == "ALL" mk2410 -f fmtunertest.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE 

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

