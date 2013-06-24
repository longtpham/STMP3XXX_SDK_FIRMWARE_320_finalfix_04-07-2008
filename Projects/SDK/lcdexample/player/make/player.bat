@echo off
rem Filename: player.bat    SigmaTel LCD based audio player build
rem 
rem check for minimum of one argument
goto :CheckArgs
:EchoUsage
@echo off
echo ***************************************************************************
echo ; Usage : player [ALL] [BUILDTYPE] [INTMEDIA] [EXTMEDIA] [LIION] [DRM] [JANUS] [CAPLESSPHONES] 
echo ;                [PLAYLIST] [FMTUNER] [FMGRID] [C_LCD_16BIT]
echo ;                [C_LCD] [LYRIC_TOOL] [JPG_TOOL] [SMV_TOOL] [ALBUMART_TOOL]
echo ;                [AUDIBLEDEC] [SPECTRUM] [WOW_ENABLE] [DIS_FASTBOOT] [LIBS] [PLIBS] [NO_SYMBOLS]
echo ;
echo ;      ALL           = Rebuilds all files (defaults to partial build).
echo ;                      This param must be first if it exists.
echo ;      BUILDTYPE     = DEBUG or RETAIL (default to DEBUG)
echo ;      INTMEDIA      = NAND1, NAND2, NAND3, NAND4 (defaults to NAND1)
echo ;                      NAND3 is the max option supported with MMC due to 4 chip select pin limit
echo ;      EXTMEDIA      = MMC (SMEDIA not supported currently)
echo ;      LIION         = Lithium Ion battery V ranging in player (default alkaline/nimhy)
echo ;      DRM           = DRM enables Digital Rights Management 
echo ;                      (default: disabled) DRM without JANUS gives DRM-PD=v9 (as previously)
echo ;      JANUS         = JANUS enables MS DRM10. Must also specify DRM param. Supports both DRM 
echo ;                      (default: disabled). So DRM builds are advised to use DRM and JANUS param
echo ;      PLAYLIST      = PL2     (playlist2),
echo ;                      PL3     (playlist3),
echo ;                      PL3_FB  (playlist3 with folder browsing feature),
echo ;                      PL5     (playlist5)
echo ;                      (default to PL3)
echo ;      CAPLESSPHONES = option for STMP35xx headphone amp without external capacitors
echo ;      FMTUNER       = STFM1000, REMOVE_FM. (default: STFM1000) 
echo ;                      STFM1000  : System will compile STFM1000 code. 
echo ;                                  Version A2 and B2 will be autodetected.
echo ;                      REMOVE_FM : If there is not FM IC in your system, you could use this option. 
echo ;                                  By this option, the FM code will not be compiled. It can save P/X/Y RAM.
echo ;      FM_COUNTRY    = FM_EU, FM_USA, FM_JAPAN. (default: FM_EU). 
echo ;                      FM_EU   : Europe(China),87.5 ~ 108MHz,  Grid = 100KHz, de-emphasis = 50uS;
echo ;                      FM_USA  : The USA,      88.1 ~ 108MHz,  Grid = 200KHz, de-emphasis = 75uS ; 
echo ;                      FM_JAPAN: Japan,        76.1 ~ 89.8MHz, Grid = 100HKz, de-emphasis = 50uS;                      
echo ;      C_LCD_16BIT   = Enable LCD 16 bit interface (default: 8 bit interface) (Also needs C_LCD)
echo ;      C_LCD         = Color LCD driver and pin mapping (monochrome is default)
echo ;      LYRIC_TOOL    = Support lyrics display for songs that have embedded lyric text
echo ;      JPG_TOOL      = Include JPEG decoder and related GUI including slideshow feature
echo ;      SMV_TOOL      = Include SigmaTel Motion Video (SMV) decoder (PC app transcodes to SMV)
echo ;      ALBUMART_TOOL = Display album art when available. 
echo ;      AUDIBLEDEC    = Add support for Audible ebook audio format (untested in SDK 3.110)
echo ;      WOW_ENABLE    = Add support for SRS WOW (Must use SMALL_PLAYLIST param if playlist2.)  
echo ;                      untested in SDK 3.120
rem echo ;      SPECTRUM      = Spectrogram LCD display animation feature during audio decode. 
echo ;      LIBS          = LIBS Rebuilds distributed source libs: fat12, io, stringlib
echo ;                      only required if change source related to these libs
echo ;      PLIBS         = ** SIGMATEL INTERNAL USAGE ONLY ** 
echo ;                        forces rebuild of all libs
echo ;      DIS_FASTBOOT  = Disable fast boot option by enabling secondary FAT operation in 
echo ;                        checkdisk. (Default is fast boot enable)
echo ;      NO_SYMBOLS    = faster build that does not include debug symbols
echo ;      PLAYER_STRESS = Auto plays all songs, restarts at 6 hrs, repeats. (alpha test app)
echo ;            
echo ;      Notes:        SDK 3.120 playlist is the Media Based Content Management Architecture
echo ;                    (MB-CMA) and is file-based instead of PLAYLIST2 which is RAM based. 
echo ;                    Player.bat supports unlimited number of parameters.
echo ;                    Select contrast range for your LCD in lcdexample\project.inc
ECHO ;                    SDK2400 defines are automatic in this build.
echo ***************************************************************************
@echo off
goto :SILENT_EXIT

:CheckArgs
if not "%1"=="" goto :EnoughArgs
echo *******************************************
echo ; Usage error : Not enough arguments
goto :EchoUsage
:EnoughArgs

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
mk2410 -f player.mk clean

:Loop
if "%1"=="" goto Continue
REM if "%1"=="PLIBS" goto EchoUsage
set BLD=%BLD% -D%1
set CMD_LINE=%CMD_LINE% D_%1=TRUE
shift
goto Loop
:Continue

echo mk2410 -f player.mk %CMD_LINE%
mk2410 -f player.mk %CMD_LINE%

if ERRORLEVEL 1 goto :ERROR
echo *******************************************
echo ******* PLAYER BUILD SUCCESSFUL  **********
echo ******************************************* 
rem echo Creating ..\output_3500\FindFreeResultPlayer.txt     
..\..\..\..\..\bin\findfree.exe ..\output_3500\player.map>..\output_3500\FindFreeResultPlayer.txt
echo                                               P,X,Y free:
rem  Next line displays P X Y free numbers.
find "s **" ..\output_3500\FindFreeResultPlayer.txt
goto EXIT

:ERROR
echo *******************************************
echo !!!!!!!!!!! PLAYER BUILD ERRORS !!!!!!!!!!
echo *******************************************
:EXIT
rem Being here avoids BUILD ERRORS print from even possibly being caused by analyzemem. Util helps even if locator error.
echo Creating ..\output_3500\AnalyzeMemPlayer.html
rem              echo analyzemem.exe -s -d stmp3500.dsc -m ..\output_3500\player.map -o ..\output_3500\AnalyzeMemPlayer.html
..\..\..\..\..\bin\analyzemem.exe -s -d stmp3500.dsc -m ..\output_3500\player.map -o ..\output_3500\AnalyzeMemPlayer.html

:SILENT_EXIT

set BLD=
set CMD_LINE=
set ALL=
set C_ASM_OPTS=

