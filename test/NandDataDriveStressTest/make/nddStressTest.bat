@echo off
rem  Filename: nddStressTest.bat
rem 
rem check for minimum of one arguments
goto :CheckArgs
:EchoUsage
@echo off
echo ***************************************************************************
echo ; Usage : nddStressTest [ALL] NAND[1-4] [LIBS] [PLIBS]
echo ;
echo ;      Stress tests the NAND flash device.
echo ;
echo ;      ALL          = Rebuilds all files (defaults to partial build).
echo ;                       This param must be first if it exists.
echo ;      NANDx        = Number of NANDs (default is NAND1)
echo ;      LIBS         = *** SIGMATEL USE ONLY *** Forces libraries to build
echo ;      PLIBS        = Build the Private Libraries
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

set BLD=
set CMD_LINE=

call ..\..\..\dspenv.bat

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
set CMD_LINE=-a%CMD_LINE%

rem 
rem Do a clean build first.
rem 
mk2410 -f nddStressTest.mk clean

:Loop
if "%1"=="" goto Continue
set BLD=%BLD% -D%1
set CMD_LINE=%CMD_LINE% D_%1=TRUE
shift
goto Loop
:Continue

rem
rem Now do the actual build
rem 
mk2410 -f nddStressTest.mk %CMD_LINE%

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
