@echo off
rem check for minimum of one arguments
goto :CheckArgs
:EchoUsage
@echo off
rem  This batch file is written to work with Win2k/Me/98, so
rem  the logic is simplified due to limitations in batch
rem  processing in Me/98
echo ***************************************************************************
echo ; Usage : NANDInfo [ALL] [NANDx] [DEVICE] [BUILDTYPE] [LIBS]
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                       This param must be first if it exists.
echo ;      NANDx        = Defaults to NAND1. Can be NAND2..NAND4
echo ;      DEVICE       = D3410, D3500 (defaults to D3500)
echo ;      BUILDTYPE    = DEBUG or RETAIL (defaults to DEBUG)
echo ;      LIBS         = *** SIGMATEL USE ONLY *** Forces libraries to build
echo ;      PLIBS        = Build the private libraries.
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

call ..\..\..\dspenv.bat

:BUILD
if "%9"=="" goto :ARG8
set BLD=-D%1 -D%2 -D%3 -D%4 -D%5 -D%6 -D%7 -D%8 -D%9
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4 -Wa-D%5 -Wa-D%6 -Wa-D%7 -Wa-D%8 -Wa-D%9
goto :READY
:ARG8
 if "%8"=="" goto :ARG7
set BLD=-D%1 -D%2 -D%3 -D%4 -D%5 -D%6 -D%7 -D%8
set C_ASM_OPTS=-Wa-D%1 -Wa-D%2 -Wa-D%3 -Wa-D%4 -Wa-D%5 -Wa-D%6 -Wa-D%7 -Wa-D%8
goto :READY
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
set BLD=-D%1 -D%2 -D%3 -D%4 -D%5
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
if "%1" == "ALL" echo ******************************************* 
if "%1" == "ALL" echo **********   Rebuilding ALL     ***********   
if "%1" == "ALL" echo ******************************************* 
if "%1" == "ALL" mk2410 -a -f NANDInfo.mk clean D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE D_%8=TRUE D_%9=TRUE
if "%1" == "ALL" echo mk2410 -a -f NANDInfo.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE D_%8=TRUE D_%9=TRUE
if "%1" == "ALL" mk2410 -a -f NANDInfo.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE D_%8=TRUE D_%9=TRUE
if not "%1" == "ALL" echo ******************************************* 
if not "%1" == "ALL" echo **********  Partial rebuild     ***********   
if not "%1" == "ALL" echo *******************************************
if not "%1" == "ALL" echo mk2410 -a -f NANDInfo.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE D_%8=TRUE D_%9=TRUE
if not "%1" == "ALL" mk2410 -f NANDInfo.mk D_%1=TRUE D_%2=TRUE D_%3=TRUE D_%4=TRUE D_%5=TRUE D_%6=TRUE D_%7=TRUE D_%8=TRUE D_%9=TRUE

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ******* NandInfo BUILD SUCCESSFUL  ********
echo ******************************************* 
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!!!!!! BUILD  eERRORS !!!!!!!!!!!!!!
echo *******************************************
:EXIT
