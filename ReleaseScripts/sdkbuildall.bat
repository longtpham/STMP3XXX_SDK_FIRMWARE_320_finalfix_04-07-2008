@echo off
goto :ClearArguments
:UsageError

echo ;******************************************************************************
echo ;
echo ;   Name: SdkBuildAll.bat (player, MTP-USBMSC autoswitch, USBMSC named updater, and tests)
echo ;
echo ;   Type:        Batch function.  For SigmaTel STMP35xx SDK 3.1xx 2006
echo ;
echo ;   Description: This function is used to build all projects in the SDK code
echo ;                tree to simplify testing SDK releases.  All firmware
echo ;                builds are put in a directory tree called SdkBuildAll_3500
echo ;
echo ;******************************************************************************
echo ;
echo ;   Usage:   sdkbuildall.bat verMajor.Mid.Minor [NANDx] 
echo ;               [LIBS/NOLIBS] [DEBUG] [PB1/PB2/PB3] [LARGEST]
echo ;
echo ;            [NANDx] - OPTIONAL (defaults to NAND1)
echo ;                Options: NAND1, NAND2, NAND3, NAND4
echo ;
echo ;            [LIBS/NOLIBS] - OPTIONAL (defaults to LIBS)
echo ;                LIBS:   defaults to build libraries
echo ;                NOLIBS: do not build lib, libs must be present for build success
echo ;
echo ;            [DEBUG]  : Halts the DSP on SystemHalt macro execution. Default is RETAIL mode.
echo ;                       RETAIL mode restarts the SOC on error macro or SystemHalt macro.
echo ;
echo ;            [PB1/PB2/PB3] - OPTIONAL (Default all binares) It use more one
echo ;                computer to build the binaries and echo builds part of them.
echo ;                PB1:  Build binar 01 to 04
echo ;                PB2:  Build binar 05 to 08
echo ;                PB3:  Build binar 09 to 13
echo ;
echo ;            [LARGEST]: Run buildlibs and largest mtp and player only.
echo ;
echo ;            [EXTRANET]: Build binaries for extranet
echo ;
echo ;   Options: verMajor.Mid.Minor - REQUIRED
echo ;
echo ;******************************************************************************
goto :End

:ClearArguments
rem ================================================================================
rem Clear out the variables (compile options) used in this file.
rem ================================================================================
@set sba_Chip=3500
@set sba_DEBUGorRETAIL=
@set sba_NandX=
@set sba_LargestOnly=
@set sba_BuildLibs=
@set sba_Build_Partion=
@set sba_Extranet_Binaries=

@set sba_MMC=
@set sba_Playlist=
@set sba_Clcd=
@set sba_Lcd_16bit=
@set sba_Audible=
@set sba_Fm_Country=
@set sba_Lyric=
@set sba_Jpeg=
@set sba_SMV=
@set sba_AlbumArt=
@set sba_Drm=
@set sba_Janus=
@set sba_Spectrum=
@set sba_Capless=
@set sba_Battery=


:CheckArguments
rem ================================================================================
rem Check to see if there is at least 1 argument
rem ================================================================================
if "%1"=="" (
echo ;///////////////////////////////////////////////////////////////////////////////
echo ; Usage error!!!!! : Not enough arguments
echo ;///////////////////////////////////////////////////////////////////////////////
goto :UsageError
)

if not "%1"=="0.000" goto :ProjectVersionOk
echo ;///////////////////////////////////////////////////////////////////////////////
echo ; Usage error!!!!! : ProjectVersion = %1 is invalid, range is 0.001-999.999
echo ;///////////////////////////////////////////////////////////////////////////////
goto :UsageError

:ProjectVersionOk
set VERSION=%1

rem ================================================================================
rem  Display the time on screen now and at all build completion to monitor total build time.
rem ----------- Bat inline Function: PrintDateAndTime   Inputs: none.
rem                         Outputs: Prints date & time iff NT based OS else does nothing.
rem  Display current time. Win98 time command prompts user to enter new time. (avoids that)
rem  Note: winXP reports %OS%=="Windows_NT" also.
rem ================================================================================
if "%OS%"=="Windows_NT" goto :Ntbased
goto :NonNtBased

:NtBased
echo SDK Build All start time:
date /t
time /t

:NonNtBased
rem ----------- End PrintDateAndTime
rem Output the command line used.
@echo Command Line:
@echo %0 %*

rem ---- Set sba_LargestOnly flag if requested
set sba_LargestOnly=FALSE
if "%2"=="LARGEST" set sba_LargestOnly=TRUE
if "%3"=="LARGEST" set sba_LargestOnly=TRUE
if "%4"=="LARGEST" set sba_LargestOnly=TRUE
if "%5"=="LARGEST" set sba_LargestOnly=TRUE
if "%6"=="LARGEST" set sba_LargestOnly=TRUE
if "%7"=="LARGEST" set sba_LargestOnly=TRUE
if "%8"=="LARGEST" set sba_LargestOnly=TRUE
if "%9"=="LARGEST" set sba_LargestOnly=TRUE

rem ---- Setup sba_BuildLibs
set sba_BuildLibs=TRUE
if "%2"=="NOLIBS" set sba_BuildLibs=FALSE
if "%3"=="NOLIBS" set sba_BuildLibs=FALSE
if "%4"=="NOLIBS" set sba_BuildLibs=FALSE
if "%5"=="NOLIBS" set sba_BuildLibs=FALSE
if "%6"=="NOLIBS" set sba_BuildLibs=FALSE
if "%7"=="NOLIBS" set sba_BuildLibs=FALSE
if "%8"=="NOLIBS" set sba_BuildLibs=FALSE
if "%9"=="NOLIBS" set sba_BuildLibs=FALSE

rem ---- Setup NAND BUILD OPTION
if "%2"=="NAND2" set sba_NandX=NAND2
if "%3"=="NAND2" set sba_NandX=NAND2
if "%4"=="NAND2" set sba_NandX=NAND2
if "%5"=="NAND2" set sba_NandX=NAND2
if "%6"=="NAND2" set sba_NandX=NAND2
if "%7"=="NAND2" set sba_NandX=NAND2
if "%8"=="NAND2" set sba_NandX=NAND2
if "%9"=="NAND2" set sba_NandX=NAND2

if "%2"=="NAND3" set sba_NandX=NAND3
if "%3"=="NAND3" set sba_NandX=NAND3
if "%4"=="NAND3" set sba_NandX=NAND3
if "%5"=="NAND3" set sba_NandX=NAND3
if "%6"=="NAND3" set sba_NandX=NAND3
if "%7"=="NAND3" set sba_NandX=NAND3
if "%8"=="NAND3" set sba_NandX=NAND3
if "%9"=="NAND3" set sba_NandX=NAND3

if "%2"=="NAND4" set sba_NandX=NAND4
if "%3"=="NAND4" set sba_NandX=NAND4
if "%4"=="NAND4" set sba_NandX=NAND4
if "%5"=="NAND4" set sba_NandX=NAND4
if "%6"=="NAND4" set sba_NandX=NAND4
if "%7"=="NAND4" set sba_NandX=NAND4
if "%8"=="NAND4" set sba_NandX=NAND4
if "%9"=="NAND4" set sba_NandX=NAND4

if "%sba_NandX%"=="" set sba_NandX=NAND1

rem ---- Setup Retail or Debug BUILD OPTION
set sba_DEBUGorRETAIL=RETAIL
if "%2"=="DEBUG" goto :AllAsDebugRequested
if "%3"=="DEBUG" goto :AllAsDebugRequested
if "%4"=="DEBUG" goto :AllAsDebugRequested
if "%5"=="DEBUG" goto :AllAsDebugRequested
if "%6"=="DEBUG" goto :AllAsDebugRequested
if "%7"=="DEBUG" goto :AllAsDebugRequested
if "%8"=="DEBUG" goto :AllAsDebugRequested
if "%9"=="DEBUG" goto :AllAsDebugRequested
goto :AllAsDebugNotRequested
:AllAsDebugRequested
rem RETAIL uses around 26 more P words than DEBUG.
set sba_DEBUGorRETAIL=DEBUG

:AllAsDebugNotRequested
rem ---- Builds extranet binaries. They are different from the internal testing ones
if "%2"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%3"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%4"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%5"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%6"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%7"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%8"=="EXTRANET" set sba_Extranet_Binaries=TRUE
if "%9"=="EXTRANET" set sba_Extranet_Binaries=TRUE


rem ---- Use 3 computers and each builds some binaries
if "%2"=="PB1" set sba_Build_Partion=PB1
if "%3"=="PB1" set sba_Build_Partion=PB1
if "%4"=="PB1" set sba_Build_Partion=PB1
if "%5"=="PB1" set sba_Build_Partion=PB1
if "%6"=="PB1" set sba_Build_Partion=PB1
if "%7"=="PB1" set sba_Build_Partion=PB1
if "%8"=="PB1" set sba_Build_Partion=PB1
if "%9"=="PB1" set sba_Build_Partion=PB1

if "%2"=="PB2" set sba_Build_Partion=PB2
if "%3"=="PB2" set sba_Build_Partion=PB2
if "%4"=="PB2" set sba_Build_Partion=PB2
if "%5"=="PB2" set sba_Build_Partion=PB2
if "%6"=="PB2" set sba_Build_Partion=PB2
if "%7"=="PB2" set sba_Build_Partion=PB2
if "%8"=="PB2" set sba_Build_Partion=PB2
if "%9"=="PB2" set sba_Build_Partion=PB2

if "%2"=="PB3" set sba_Build_Partion=PB3
if "%3"=="PB3" set sba_Build_Partion=PB3
if "%4"=="PB3" set sba_Build_Partion=PB3
if "%5"=="PB3" set sba_Build_Partion=PB3
if "%6"=="PB3" set sba_Build_Partion=PB3
if "%7"=="PB3" set sba_Build_Partion=PB3
if "%8"=="PB3" set sba_Build_Partion=PB3
if "%9"=="PB3" set sba_Build_Partion=PB3

rem ---- only support this chip for SDK3.x
set sba_ReleaseRootReference=..
set sba_BootManagerRootReference=..\..\..\..\..\..
set sba_PlayerRootReference=..\..\..\..\..
set sba_MscRootReference=..\..\..\..\..
set sba_MTPRootReference=..\..\..\..\..
set sba_LibRootReference=..\..\..
set sba_TestRootReference=..\..\..
set sba_FatTestRootReference=..\..\..
set sba_OutputDir=SdkBuildAll

rem ================================================================================
rem Delete and recreate the destination directory for this automated build
rem ================================================================================
if not exist %sba_OutputDir%_3500 goto :NoPreviousTree3500
echo "Previous output tree exists, removing previous output tree"
rmdir /s /q %sba_OutputDir%_3500

:NoPreviousTree3500
cd %sba_ReleaseRootReference%
call .\dspenv.bat

if "%sba_BuildLibs%"=="TRUE" (
echo *******************************************************************************
echo ****************** ERASING AND REBUILDING LIBRARIES ***************************
echo *******************************************************************************
pushd libsource

rem Builds both debug and retail libs by default
echo call buildlibs.bat
call buildlibs.bat

echo call buildlibs.bat PLAYER
call buildlibs.bat PLAYER

popd
) else (
echo *******************************************************************************
echo ************************ NOT REBUILDING LIBRARIES *****************************
echo *******************************************************************************
)

rem Check if want to build Largest only for memory space verification
if "%sba_LargestOnly%"=="TRUE" (
goto :DoLargestBuilds
)


rem ================================================================================
rem ========================= BUILDING Boot Manager ================================
rem ================================================================================
:BootManager
set sba_Customer=BootManager
set sba_Product=BootManager
set sba_NextBuild=Updater_Options
goto :BMBuild


rem ================================================================================
rem ========================= BUILDING Updaters ====================================
rem ================================================================================
:Updater_Options
set sba_Customer=updater
set sba_Product=updater

:Updater
set sba_updater_dir=updater
set sba_Clcd=
set sba_Lcd_16bit=
set sba_NextBuild=Updater_CLCD
goto :UpdaterBuild

:Updater_CLCD
set sba_updater_dir=updater_CLCD
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_NextBuild=Updater_CLCD_16BIT
goto :UpdaterBuild

:Updater_CLCD_16BIT
set sba_updater_dir=updater_16bit_CLCD
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_NextBuild=Player_Options
goto :UpdaterBuild


rem ================================================================================
rem ========================= BUILDING Players =====================================
rem ================================================================================
:Player_Options
set sba_Customer=Sdk
set sba_Product=LcdExample


rem ---- ****************** Extranet binaries for SDK3200 ********************
if "%sba_Extranet_Binaries%"=="TRUE" goto :ExtranetBinaries


rem ---- Template
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
rem :LcdExample_PL2_JANUS_LIION_CLCD_LYRICS_JPEG_SMV_ALBUMART_AUDIBLE_FMJAPAN
rem set sba_release_dir=LcdExample_PL2_JANUS_LIION_CLCD_LYRICS_JPEG_SMV_ALBUMART_AUDIBLE_FMJAPAN
rem set sba_updater_dir=updater_CLCD
rem set sba_MMC=MMC
rem set sba_Playlist=
rem set sba_Clcd=C_LCD
rem set sba_Lcd_16bit=
rem set sba_Audible=
rem set sba_Fm_Country=
rem set sba_Lyric=
rem set sba_Jpeg=
rem set sba_SMV=
rem set sba_AlbumArt=
rem set sba_Drm=
rem set sba_Janus=
rem set sba_Spectrum=
rem set sba_Capless=
rem set sba_Battery=
rem set sba_NextBuild=End
rem goto :sba_PlayerBuild
rem goto :End


rem ---- ****************** Test binaries for SDK3200 ********************

rem ---- **** Partial build ****
rem ---- PB1 Binary01 to Binary04
rem ---- PB2 Binary05 to Binary08
rem ---- PB3 Binary09 to Binary13
if "%sba_Build_Partion%"=="PB2" goto :Binary05
if "%sba_Build_Partion%"=="PB3" goto :Binary09

rem - Binary 1 -
:Binary01
set sba_release_dir=LcdExample_PL2_DRM_JANUS_%sba_NandX%_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_JAPAN
set sba_updater_dir=updater_CLCD
set sba_MMC=
set sba_Playlist=PL2
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_JAPAN
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary02
goto :sba_PlayerBuild
goto :End


rem - Binary 2 -
:Binary02
set sba_release_dir=LcdExample_PL3_DRM_JANUS_%sba_NandX%_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_EU
set sba_updater_dir=updater_CLCD
set sba_MMC=
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_EU
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary03
goto :sba_PlayerBuild
goto :End


rem - Binary 3 -
:Binary03
set sba_release_dir=LcdExample_PL3_FB_DRM_JANUS_%sba_NandX%_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_US
set sba_updater_dir=updater_CLCD
set sba_MMC=
set sba_Playlist=PL3_FB
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_US
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary04
goto :sba_PlayerBuild
goto :End


rem - Binary 4 -
:Binary04
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL2_DRM_NAND2_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_JAPAN
set sba_updater_dir=updater_CLCD
set sba_MMC=MMC
set sba_Playlist=PL2
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_JAPAN
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
rem ---- **** Goto End if this is apartial build else continue ****
if "%sba_Build_Partion%"=="" (
    set sba_NextBuild=Binary05
) else (
    set sba_NextBuild=End
)
goto :sba_PlayerBuild
goto :End


rem - Binary 5 -
:Binary05
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_DRM_NAND2_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_EU
set sba_updater_dir=updater_CLCD
set sba_MMC=MMC
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_EU
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary06
goto :sba_PlayerBuild
goto :End


rem - Binary 6 -
:Binary06
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_FB_DRM_NAND2_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_US
set sba_updater_dir=updater_CLCD
set sba_MMC=MMC
set sba_Playlist=PL3_FB
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_US
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary07
goto :sba_PlayerBuild
goto :End



rem - Binary 7 -
:Binary07
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_PL3_FB_DRM_NAND2_LIION_CLCD_16BIT_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_JAPAN
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=PL3_FB
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_JAPAN
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary08
goto :sba_PlayerBuild
goto :End


rem - Binary 8 -
:Binary08
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_PL3_FB_NAND2_CLCD_16BIT_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_EU
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=PL3_FB
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_EU
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
rem ---- **** Goto End if this is apartial build else continue ****
if "%sba_Build_Partion%"=="" (
    set sba_NextBuild=Binary09
) else (
    set sba_NextBuild=End
)
goto :sba_PlayerBuild
goto :End


rem - Binary 9 -
:Binary09
set sba_release_dir=LcdExample_PL3_FB_DRM_%sba_NandX%_LYRIC_AUDIBLE_FM_US
set sba_updater_dir=updater
set sba_MMC=
set sba_Playlist=PL3_FB
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_US
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=Binary10
goto :sba_PlayerBuild
goto :End


rem - Binary 10 -
:Binary10
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_FB_NAND2_LYRIC_AUDIBLE_REMOVEFM
set sba_updater_dir=updater
set sba_MMC=MMC
set sba_Playlist=PL3_FB
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=REMOVE_FM
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=Binary11
goto :sba_PlayerBuild
goto :End


rem - Binary 11 -
:Binary11
set sba_release_dir=LcdExample_PL2_%sba_NandX%_CLCD_ROMOVEFM_SPECTRUM_CAPLESS
set sba_updater_dir=updater_CLCD
set sba_MMC=
set sba_Playlist=PL2
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=REMOVE_FM
set sba_Lyric=
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=SPECTRUM
set sba_Capless=CAPLESSPHONES
set sba_Battery=
set sba_NextBuild=Binary12
goto :sba_PlayerBuild
goto :End


rem - Binary 12 -
:Binary12
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_PL3_NAND2_CLCD_16BIT_ROMOVEFM_SPECTRUM_CAPLESS
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=
set sba_Fm_Country=REMOVE_FM
set sba_Lyric=
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=SPECTRUM
set sba_Capless=CAPLESSPHONES
set sba_Battery=
set sba_NextBuild=Binary13
goto :sba_PlayerBuild
goto :End


rem - Binary 13 -
:Binary13
set sba_release_dir=LcdExample_PL3_FB_%sba_NandX%_ROMOVEFM_SPECTRUM_CAPLESS
set sba_updater_dir=updater
set sba_MMC=
set sba_Playlist=PL3_FB
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=REMOVE_FM
set sba_Lyric=
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=SPECTRUM
set sba_Capless=CAPLESSPHONES
set sba_Battery=
set sba_NextBuild=Binary14
goto :sba_PlayerBuild
goto :End


rem - Binary 14 -
:Binary14
set sba_release_dir=LcdExample_PL5_NAND2_LYRIC_AUDIBLE_REMOVEFM
set sba_updater_dir=updater
set sba_MMC=
set sba_Playlist=PL5
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=REMOVE_FM
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=Binary15
goto :sba_PlayerBuild
goto :End


rem - Binary 15 -
:Binary15
set sba_release_dir=LcdExample_PL5_DRM_JANUS_%sba_NandX%_LIION_CLCD_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_US
set sba_updater_dir=updater
set sba_MMC=
set sba_Playlist=PL5
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_US
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=LIION
set sba_NextBuild=Binary16
goto :sba_PlayerBuild
goto :End


rem - Binary 16 -
:Binary16
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_PL5_DRM_JANUS_NAND2_CLCD_16BIT_LYRIC_JPEG_SMV_ALBUM_AUDIBLE_FM_EU
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=PL5
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=AUDIBLEDEC
set sba_Fm_Country=FM_EU
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=End
goto :sba_PlayerBuild
goto :End


:Tests
set sba_NextBuild=End
set sba_Drm=DRM
goto :sba_TestBuilds
goto :End


:ExtranetBinaries
rem - Extranet Binary 01 -
:ExtranetBinary01
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_DRM_NAND2_CLCD_LYRIC_JPEG_SMV_ALBUM
set sba_updater_dir=updater_CLCD
set sba_MMC=MMC
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=ExtranetBinary02
goto :sba_PlayerBuild
goto :End


rem - Extranet Binary 02 -
:ExtranetBinary02
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_DRM_NAND2_CLCD_16BIT_LYRIC_JPEG_SMV_ALBUM
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=ExtranetBinary03
goto :sba_PlayerBuild
goto :End


rem - Extranet Binary 03 -
:ExtranetBinary03
rem ---- **** MMC can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_MMC_PL3_NAND2
set sba_updater_dir=updater
set sba_MMC=MMC
set sba_Playlist=
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=
set sba_Janus=
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=ExtranetBinary04
goto :sba_PlayerBuild
goto :End


rem - Extranet Binary 04 -
:ExtranetBinary04
set sba_release_dir=LcdExample_PL3_DRM_JANUS_%sba_NandX%_CLCD_LYRIC_JPEG_SMV_ALBUM
set sba_updater_dir=updater_CLCD
set sba_MMC=
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=ExtranetBinary05
goto :sba_PlayerBuild
goto :End


rem - Extranet Binary 05 -
:ExtranetBinary05
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
set sba_release_dir=LcdExample_PL3_DRM_JANUS_NAND2_CLCD_16BIT_LYRIC_JPEG_SMV_ALBUM
set sba_updater_dir=updater_16BIT_CLCD
set sba_MMC=
set sba_Playlist=
set sba_Clcd=C_LCD
set sba_Lcd_16bit=C_LCD_16BIT
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=LYRIC_TOOL
set sba_Jpeg=JPG_TOOL
set sba_SMV=SMV_TOOL
set sba_AlbumArt=ALBUMART_TOOL
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=ExtranetBinary06
goto :sba_PlayerBuild
goto :End


rem - Extranet Binary 06 -
:ExtranetBinary06
set sba_release_dir=LcdExample_PL3_DRM_JANUS_%sba_NandX%
set sba_updater_dir=updater
set sba_MMC=
set sba_Playlist=
set sba_Clcd=
set sba_Lcd_16bit=
set sba_Audible=
set sba_Fm_Country=
set sba_Lyric=
set sba_Jpeg=
set sba_SMV=
set sba_AlbumArt=
set sba_Drm=DRM
set sba_Janus=JANUS
set sba_Spectrum=
set sba_Capless=
set sba_Battery=
set sba_NextBuild=End
goto :sba_PlayerBuild
goto :End





:BMBuild
rem ---- ****************** Bootmanager build subroutine ********************
rem ---- *    Inputs : sba_Customer = sba_Customer directory name
rem ---- *             sba_Product = sba_Customer sba_Product directory name
rem ---- *             sba_NextBuild = continuation label in batch file
rem ---- *    Outputs : Builds the USB MSC firmware
rem ---- ********************************************************************

echo *******************************************************************************
echo ************************ BUILDING Boot Manager %sba_Chip% *********************
echo *******************************************************************************

if not exist ReleaseScripts\%sba_OutputDir%_%sba_Chip%\bootmanager mkdir "ReleaseScripts\%sba_OutputDir%_%sba_Chip%\bootmanager"
cd DeviceDriver\Media\SmartMedia\BootManager\STMP%sba_Chip%\make

echo call bootmanager.bat ALL D%sba_Chip%
call bootmanager.bat ALL D%sba_Chip%
copy ..\output_%sba_Chip%\bootmanager.sb %sba_BootManagerRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\bootmanager

echo call bootmanager.bat ALL D%sba_Chip% EEPROM
call bootmanager.bat ALL D%sba_Chip% EEPROM
copy ..\output_%sba_Chip%\bootmanager.se %sba_BootManagerRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\bootmanager

cd %sba_BootManagerRootReference%
goto :%sba_NextBuild%





:UpdaterBuild
rem ---- ****************** updater.sb build subroutine ********************
rem ---- *    Inputs : sba_Customer = sba_Customer directory name.            INPUT UNUSED.
rem ---- *             sba_Product = sba_Customer sba_Product directory name. INPUT UNUSED. 
rem ---- *             sba_NextBuild = continuation label in batch file.      REQUIRED.
rem ---- *    Outputs : Builds and saves the updater.sb firmware once for later copy to each output config
rem ---- *****************************************************************

echo ****************************************************************************************
echo ********** Building updater for %sba_Chip% %sba_colorlcd% %sba_lcd_16bit% **************
echo ****************************************************************************************

if not exist ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%  mkdir "ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%"
cd Projects\SDK\LCDEXAMPLE\USBMSC\MAKE

rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
if "%sba_Lcd_16bit%"=="C_LCD_16BIT" (
echo call usbmsc.bat ALL RETAIL NAND2 LIBS %sba_Battery% %sba_Clcd% %sba_Lcd_16bit% NO_SYMBOLS UPDATER
call usbmsc.bat ALL RETAIL NAND2 LIBS %sba_Battery% %sba_Clcd% %sba_Lcd_16bit% NO_SYMBOLS UPDATER
) else (
echo call usbmsc.bat ALL RETAIL NAND4 LIBS %sba_Battery% %sba_Clcd% %sba_Lcd_16bit% NO_SYMBOLS UPDATER
call usbmsc.bat ALL RETAIL NAND4 LIBS %sba_Battery% %sba_Clcd% %sba_Lcd_16bit% NO_SYMBOLS UPDATER
)

copy ..\output_%sba_Chip%\updater.sb %sba_MscRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%
copy ..\output_%sba_Chip%\usbmsc.map %sba_MscRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%

copy ..\output_%sba_Chip%\FindFreeResultUsbmsc.txt %sba_MscRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%
copy ..\output_%sba_Chip%\AnalyzeMemUsbmsc.html    %sba_MscRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%
copy ..\output_%sba_Chip%\CheckMemResultUsbmsc.txt %sba_MscRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%

cd %sba_MscRootReference%
goto :%sba_NextBuild%





:sba_PlayerBuild
rem ******************** Player build subroutine ********************
rem *    Inputs :  sba_Customer = Customer directory name
rem *              sba_Product = Customer Product directory name
rem *              sba_Drm     = DRM or blank
rem *              sba_Battery = LIION or blank for to alkaline/nimh
rem *              sba_NextBuild = continuation label in batch file
rem *
rem *    Outputs : Builds the player and copies it to the output
rem *              hierarchy
rem *****************************************************************


rem ================================================================================
rem Player - Boot manager & Updater
rem ================================================================================

echo **********************************************************************
echo ******* Building name: %sba_release_dir%
echo **********************************************************************

echo **********************************************************************
echo ******* Copy Boot Manager and Updater
echo **********************************************************************

cd Projects\%sba_Customer%\%sba_Product%\Player\make
set ReleaseDir=ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_release_dir%
if not exist %sba_PlayerRootReference%\%ReleaseDir% mkdir "%sba_PlayerRootReference%\%ReleaseDir%"

rem ---- Copy the boot manager to the same destination directory so the firmware set is available as an easy copy
copy %sba_PlayerRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\bootmanager\bootmanager.sb %sba_PlayerRootReference%\%ReleaseDir%
copy %sba_PlayerRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%\updater.sb %sba_PlayerRootReference%\%ReleaseDir%
copy %sba_PlayerRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\%sba_updater_dir%\usbmsc.map %sba_PlayerRootReference%\%ReleaseDir%

cd %sba_MscRootReference%


rem ================================================================================
rem Player - Build the player itself
rem ================================================================================

set sba_Local_NandX=%sba_NandX%
rem ---- **** 16Bit CLCD can't use NAND4 due to EVK design ****
if "%sba_Lcd_16bit%"=="C_LCD_16BIT" (
set sba_Local_NandX=NAND2
)

rem ---- **** MMC can't use NAND4 due to EVK design ****
if "%sba_MMC%"=="MMC" (
set sba_Local_NandX=NAND2
)

echo **********************************************************************
echo ******* Building the %sba_Customer% %sba_Product% %sba_ProductBuild% Player
echo ******* Building with %sba_Local_NandX% %sba_MMC% %sba_Playlist% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Lyric% %sba_Jpeg% %sba_SMV% %sba_AlbumArt% %sba_Drm% %sba_Janus% %sba_Battery%
echo **********************************************************************

cd Projects\%sba_Customer%\%sba_Product%\Player\make

echo call player.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Playlist% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Lyric% %sba_Jpeg% %sba_SMV% %sba_AlbumArt% %sba_Drm% %sba_Janus% %sba_Battery% %sba_Capless% LIBS NO_SYMBOLS
call player.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Playlist% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Lyric% %sba_Jpeg% %sba_SMV% %sba_AlbumArt% %sba_Drm% %sba_Janus% %sba_Battery% %sba_Capless% LIBS NO_SYMBOLS

rem ---- Copy player and resource files to the output directory
copy ..\output_%sba_Chip%\player.map %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\stmpsys.sb %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\resource.bin %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Copy Findfree and analyzemem utils called in player.bat
copy ..\output_%sba_Chip%\AnalyzeMemPlayer.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\FindFreeResultPlayer.txt %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Generate and copy check memory result
..\..\..\..\..\bin\checkmem.exe ..\output_%sba_Chip%\player.map>..\output_%sba_Chip%\CheckMemResultPlayer.txt
copy ..\output_%sba_Chip%\CheckMemResultPlayer.txt %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Copy player.abs for debug
if "%sba_DEBUGorRETAIL%"=="DEBUG" copy ..\output_%sba_Chip%\player.abs %sba_PlayerRootReference%\%ReleaseDir%

cd %sba_PlayerRootReference%


rem ================================================================================
rem Player - Hostlink
rem ================================================================================

echo **********************************************************************
echo ******* Building %sba_Customer% %sba_Product% %sba_ProductBuild% MTP
echo ******* Building with %sba_Local_NandX% %sba_MMC% %sba_Drm% %sba_Janus% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Battery%
echo **********************************************************************

if "%sba_Janus%"=="JANUS" goto :HostlinkForJanus

rem ================================================================================
rem Player - Hostlink for no Janus (No MTP)
rem ================================================================================

echo **********************************************************************
echo ******* Hostlink for no MTP
echo **********************************************************************

cd Projects\%sba_Customer%\%sba_Product%\usbmsc\make
echo call usbmsc.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Drm% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Battery% LIBS NO_SYMBOLS
call usbmsc.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Drm% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Battery% LIBS NO_SYMBOLS

rem ---- copy usbmsc output files to the output directory for this config
copy ..\output_%sba_Chip%\hostlink.sb %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\hostrsc.bin %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\usbmsc.map %sba_PlayerRootReference%\%ReleaseDir%\hostlink.map

rem ---- Findfree, checkmem, and analyzemem called in usbmsc.bat
copy ..\output_%sba_Chip%\AnalyzeMemUsbmsc.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\CheckMemResultUsbmsc.txt %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\FindFreeResultUsbmsc.txt %sba_PlayerRootReference%\%ReleaseDir%

rem Copy *.abs for debug
if "%sba_DEBUGorRETAIL%"=="DEBUG" copy ..\output_%sba_Chip%\*.abs %sba_PlayerRootReference%\%ReleaseDir%

cd   %sba_MTPRootReference%
goto :%sba_NextBuild%


:HostlinkForJanus
rem ================================================================================
rem Player - Hostlink for Janus (MTP)
rem ================================================================================

echo **********************************************************************
echo ******* Hostlink for MTP
echo **********************************************************************

cd Projects\%sba_Customer%\%sba_Product%\MTP\make
echo call mtp.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Drm% %sba_Janus% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Battery% LIBS NO_SYMBOLS
call mtp.bat ALL %sba_DEBUGorRETAIL% %sba_Local_NandX% %sba_MMC% %sba_Drm% %sba_Janus% %sba_Clcd% %sba_Lcd_16bit% %sba_Audible% %sba_Fm_Country% %sba_Battery% LIBS NO_SYMBOLS

rem ---- Copy player and resource files to the output directory
copy ..\output_%sba_Chip%\hostlink.map %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\hostlink.sb %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\hostrsc.bin %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Copy findfree, analyzemem, checkmem utils called in mtp.bat
copy ..\output_%sba_Chip%\AnalyzeMemMtp.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\CheckMemResultMtp.txt %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\FindFreeResultMtp.txt %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Copy player.abs for debug
if "%sba_DEBUGorRETAIL%"=="DEBUG" copy ..\output_%sba_Chip%\hostlink.abs %sba_PlayerRootReference%\%ReleaseDir%

cd   %sba_MTPRootReference%
goto :%sba_NextBuild%





:sba_TestBuilds
rem ================================================================================
rem TESTS
rem ================================================================================
echo ************************************************************
echo ********************** Building KernelTest *****************
echo ************************************************************

pushd test\kernel\make
call kerneltest.bat ALL D%sba_Chip%

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\KernelTest"
copy ..\output_%sba_Chip%\kerneltest_resource.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\kerneltest
popd


:MakeDdiTest
echo ************************************************************
echo ********************** Building DDI Test *******************
echo ************************************************************

pushd test\dditest\make
call dditest.bat ALL D%sba_Chip% %sba_SDK% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\dditest"
copy ..\output_%sba_Chip%\dditest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\dditest
popd


:MakeNandMediaTest
echo ************************************************************
echo *************** Building NAND Media Test *******************
echo ************************************************************

pushd test\Nandmediatest\make
call nandmediatest.bat ALL D%sba_Chip% %sba_SDK% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDMediaTest"
copy ..\output_%sba_Chip%\nandmediatest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\Nandmediatest
popd


:MakeRscmgrTest
echo ************************************************************
echo ************* Building Resource Manager Test ***************
echo ************************************************************

pushd test\rscmgrtest\make
call rscmgrtest.bat ALL D%sba_Chip% %sba_SDK% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\rscmgrtest"
copy ..\output_%sba_Chip%\rscmgrtest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\rscmgrtest
copy ..\output_%sba_Chip%\rscmgrtest.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\rscmgrtest\stmpsys.sb
popd

rem -------------------------------------------------------------
rem TODO: Skip nddstress for sdk extraction sdk3.11 (like 3.10) since it requires PLIBS currently. 
rem You can do this PLIBS test build iff you have full source else skip this test build.
rem            PLIBS required for this test build to run correctly. 
if "%sba_plibs_ok%"=="TRUE" (
goto :MakeNddStressTest
)
rem     skip nddstresstest since it needs PLIBS and this sdkbuildall cmd did not give param PLIBS_OK
goto :MakeNandInfo


:MakeNddStressTest
echo ************************************************************
echo *************** Building NAND Stress Test ******************
echo ************************************************************
rem 3.11 feb'06, and sdk3.1 august 20  sept 14 '05: nddstresstest MUST currently have PLIBS param else links wrong version of an object.
rem PN has more info on which obj is linked wrong in non-PLIBS case (mk file defect) so PLIBS is workaround to link correct ver of that object.
rem If QA wants to use it, build it manually with command below plus PLIBS

pushd test\NandDataDriveStressTest\make
call nddStressTest.bat ALL D%sba_Chip% %sba_SDK% LIBS PLIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
echo !!!! nddstresstest.bat build error PLIBS build failed- customer extraction does not have source- todo: remove PLIBS after defect fixed !!!!
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDStresstest"
copy ..\output_%sba_Chip%\nddStressTest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDStresstest
copy ..\output_%sba_Chip%\nddStressTest.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDStresstest\stmpsys.sb
copy ..\readme.txt %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDStresstest
popd


:MakeNandInfo
echo ************************************************************
echo *************** Building NAND Info *************************
echo ************************************************************

pushd utils\NandInfo\make
call nandinfo.bat ALL NAND4 D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDInfo"
copy ..\output_%sba_Chip%\nandinfo.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDInfo
copy ..\readme.txt %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDInfo
popd


:MakeNandBadBlockMarker
echo ************************************************************
echo **************** Building NANDBadBlockMarker ***************
echo ************************************************************

pushd utils\NANDBadBlockMarker\make
call NANDBadBlockMarker.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDBadBlockMarker"
copy ..\output_%sba_Chip%\NandBadBlockMarker.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDBadBlockMarker
copy ..\readme.txt %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\NANDBadBlockMarker
popd
rem skip buttontest since this linker err occurs:  unresolved external(s):     SysWaitOnEvent - (rsc_manager.a:..\output_3500\obj\sysresourceapi.obj)
rem skip QETest since this linker err occurs:  lk2410 E208 (0): Found unresolved external(s): SysWaitOnEvent - (rsc_manager.a:..\output_3500\obj\sysresourceapi.obj)
goto :MakeLedTest


:MakeButtonTest
echo ************************************************************
echo ******************* Building buttontest ********************
echo ************************************************************

pushd test\buttontest\make
call buttontest.bat ALL D%sba_Chip% LIBS FORCE_HOSTLINK_RSCMGR

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\buttontest"
copy ..\output_%sba_Chip%\buttontest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\buttontest
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\buttontest
popd


:MakeQeTest
echo ************************************************************
echo ******************* Building QEtest ************************
echo ************************************************************

pushd test\QEtest\make
call QEtest.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\QEtest"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\QEtest
copy ..\output_%sba_Chip%\resource.bin %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\QEtest
popd


:MakeLcdTest
echo ************************************************************
echo ******************* Building lcdtest************************
echo ************************************************************

pushd test\lcdtest\make
call lcdtest.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lcdtest"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lcdtest
copy ..\output_%sba_Chip%\resource.bin %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lcdtest
popd


:MakeFmTunerTest
echo ************************************************************
echo ****************** Building FMTunertest 5757 ***************
echo ************************************************************

pushd test\fmtunertest\make
call fmtunertest.bat ALL D%sba_Chip% FM5757

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fmtunertest5757"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fmtunertest5757
popd


echo ************************************************************
echo ***************** Building FMTunertest 5767 ****************
echo ************************************************************

pushd test\fmtunertest\make
call fmtunertest.bat ALL D%sba_Chip% FM5767

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fmtunertest5767"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fmtunertest5767
popd


:MakeI2cTest
echo ************************************************************
echo ****************** Building i2ctest ************************
echo ************************************************************

pushd test\i2ctest\make
call i2ctest.bat ALL D%sba_Chip%

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\i2ctest"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\i2ctest
copy ..\output_%sba_Chip%\resource.bin %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\i2ctest
popd


:MakeLradcTest
echo ************************************************************
echo ***************** Building lradctest ***********************
echo ************************************************************

pushd test\lradctest\make
call lradctest.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lradctest"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lradctest
copy ..\output_%sba_Chip%\resource.bin %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\lradctest
popd


:MakeSdramTest
echo ************************************************************
echo ******************* Building sdramtest *********************
echo ************************************************************

pushd   test\sdramtest\make
call sdramtest.bat ALL D%sba_Chip%

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\sdramtest"
copy ..\output_%sba_Chip%\sdramtest.abs %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\sdramtest
popd


echo ************************************************************
echo ****************** Building Seqledtest *********************
echo ************************************************************

pushd test\seqledtest\make
call seqledtest.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\seqledtest"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\seqledtest
copy ..\output_%sba_Chip%\resource.bin %sba_TestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\seqledtest
popd
goto :MakeFat32Test


:DoLargestBuilds
echo ************************************************************
echo ****************** Building largest build ******************
echo ************************************************************

rem ================================================================================
rem PLAYER
rem ================================================================================

cd Projects\%sba_Customer%\%sba_Product%\Player\make

rem ---- WOW_ENABLE SPECTRUM SMALL_PLAYLIST
call player.bat ALL RETAIL NAND4 PL3_FB LIION DRM JANUS NO_SYMBOLS C_LCD LYRIC_TOOL JPG_TOOL SMV_TOOL ALBUMART_TOOL AUDIBLEDEC

rem ---- Create the output directory
set ReleaseDir=ReleaseScripts\%sba_OutputDir%_%sba_Chip%\Largest_Build
if not exist %sba_PlayerRootReference%\%ReleaseDir% mkdir "%sba_PlayerRootReference%\%ReleaseDir%"

rem ---- copy player and resource files to the output directory
copy ..\output_%sba_Chip%\player.map %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\stmpsys.sb %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\resource.bin %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Findfree and analyzemem utils called in player.bat
copy ..\output_%sba_Chip%\AnalyzeMemPlayer.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\FindFreeResultPlayer.txt %sba_PlayerRootReference%\%ReleaseDir%

cd %sba_PlayerRootReference%


rem ================================================================================
rem MTP
rem ================================================================================

cd Projects\%sba_Customer%\%sba_Product%\MTP\make

rem ---- RETAIL param needs to be enabled below and change buildlibs call above to build retail and debug before the release!!!!!
call mtp.bat ALL NAND4 RETAIL LIION DRM JANUS LIBS NO_SYMBOLS C_LCD AUDIBLEDEC

set ReleaseDir=ReleaseScripts\%sba_OutputDir%_%sba_Chip%\Largest_Build

rem ---- copy player and resource files to the output directory
copy ..\output_%sba_Chip%\hostlink.map %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\hostlink.sb %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\hostrsc.bin %sba_PlayerRootReference%\%ReleaseDir%

rem findfree and analyzemem utils called in mtp.bat
copy ..\output_%sba_Chip%\AnalyzeMemMtp.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\FindFreeResultMtp.txt %sba_PlayerRootReference%\%ReleaseDir%

cd %sba_MTPRootReference%


rem ================================================================================
rem Mass Storage Class
rem ================================================================================

cd Projects\%sba_Customer%\%sba_Product%\usbmsc\make
call usbmsc.bat ALL DEBUG NAND4 LIION DRM LIBS NO_SYMBOLS C_LCD

set ReleaseDir=ReleaseScripts\%sba_OutputDir%_%sba_Chip%\Largest_Build

rem ---- copy usbmsc output files to the output directory for this config
copy ..\output_%sba_Chip%\usbmsc.map %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\updater.sb %sba_PlayerRootReference%\%ReleaseDir%

rem ---- Findfree, checkmem, and analyzemem called in usbmsc.bat TOVERIFY make it so.
copy ..\output_%sba_Chip%\FindFreeResultUsbmsc.txt %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\AnalyzeMemUsbmsc.html %sba_PlayerRootReference%\%ReleaseDir%
copy ..\output_%sba_Chip%\CheckMemResultUsbmsc.txt %sba_PlayerRootReference%\%ReleaseDir%

if "%sba_DEBUGorRETAIL%"=="DEBUG" copy ..\output_%sba_Chip%\*.abs %sba_PlayerRootReference%\%ReleaseDir%

cd %sba_MscRootReference%


:MakeFat32Test
echo ************************************************************
echo ******************** Building fat32test  *******************
echo ************************************************************

pushd   test\fat32test\make
call fattest.bat ALL D%sba_Chip% LIBS

@if ERRORLEVEL 1 (
popd
goto :ERROR
)

mkdir "%sba_FatTestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fat32test"
copy ..\output_%sba_Chip%\stmpsys.sb %sba_FatTestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fat32test
copy ..\output_%sba_Chip%\resource.bin %sba_FatTestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fat32test
copy ..\output_%sba_Chip%\fattest.abs %sba_FatTestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\fat32test
xcopy /e /i ..\fat32testdirstructure %sba_FatTestRootReference%\ReleaseScripts\%sba_OutputDir%_%sba_Chip%\Fat32Test\fat32testdirstructure
popd





goto :End
REM ---------------------- END SDKBUILDALL build attempts -----------
:ERROR
echo
echo *******************************************
echo ********** SDKBA Build Error **************
echo *******************************************




:End
rem ================================================================================
rem  ==================================== End ======================================
rem ================================================================================

rem ---- Display the completion time
echo Build completion time:
date /t
time /t

rem ---- Return to the ReleaseScripts directory
cd ReleaseScripts
