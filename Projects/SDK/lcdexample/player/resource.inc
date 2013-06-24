;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sat Jun 28 16:37:44 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sat Jun 28 16:13:49 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sat Jun 28 15:55:53 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Thu Jun 26 08:58:05 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 25 17:00:35 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 25 11:42:38 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 25 11:17:50 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 25 11:17:05 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Tue Jun 24 15:21:11 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sun Jun 22 02:54:01 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sun Jun 22 00:47:39 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Fri Jun 20 16:15:57 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Thu Jun 19 09:47:30 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 23:11:16 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 22:57:19 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 22:49:16 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 11:53:38 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 10:45:13 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Wed Jun 18 09:54:56 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Tue Jun 17 16:47:21 2008 using resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Tue Jun 17 11:30:49 2008 using resource.inc as input.



;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2001-2007
; Reviews: DanhNguyen (06-2008) for X8iTF/STFM1000
; LCD example resource listing
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(resources))
resources equ 1

;///////////////////////////////////////////////////////////////////////////////
;  Player version number  no leading zeros in version number!!!!!
;///////////////////////////////////////////////////////////////////////////////

VERSION_MAJOR               equ     3
VERSION_MIDDLE              equ     200
VERSION_MINOR               equ     910

LCD_SEG_OFFSET              equ     $000000
NUMBER_OF_PRESETS           equ     10


;///////////////////////////////////////////////////////////////////////////////
;  High usage resource. These have been moved here to take maximum advantage of
;  the resource index cache if it was added.
;  This block is sorted by frequency of use while loading a new song.
;///////////////////////////////////////////////////////////////////////////////

RSRC_FUNCLET_SEARCHDIRECTORY    equ        1    ;$FILENAME searchdirectory.src
RSRC_FUNCLET_SHORTDIRMATCH    equ        2    ;$FILENAME shortdirmatch.src
RSRC_FUNCLET_FOPEN      equ        3    ;$FILENAME fopen.src
RSRC_MUSIC_MENU_CODE_BANK    equ        4    ;$FILENAME musicmenu.src
RSRC_FUNCLET_CHANGEPATH    equ        5    ;$FILENAME changepath.src
RSRC_FUNCLET__OPENANDVERIFYSLOT    equ        6    ;$FILENAME _openandverifyslot.src
RSRC_FUNCLET__LOADSLOT    equ        7    ;$FILENAME _loadslot.src
RSRC_FUNCLET_GETNAME    equ        8    ;$FILENAME getname.src
RSRC_FUNCLET_STEPPINGVOLTAGESET    equ        9    ;$FILENAME Funclet_SteppingVoltageSet.src
RSRC_FUNCLET_SETHANDLEFORSEARCH    equ       10    ;$FILENAME sethandleforsearch.src
RSRC_WMADEC_CODE        equ       11    ;$FILENAME wmaWrap.src
RSRC_FUNCLET_EXTRACTFILENAME    equ       12    ;$FILENAME extractfilename.src
RSRC_FUNCLET_OEM_GETGROUPCERTPRIVATEKEY    equ       13    ;$FILENAME oem_getgroupcertprivatekey.src
RSRC_SOFT_TIMER_MODULE_CODE    equ       14    ;$FILENAME SoftTimerMod.src
RSRC_FUNCLET_GETSHORTFILENAME    equ       15    ;$FILENAME GetShortfilename.src
RSRC_FUNCLET_DRM_DCP_LOADPROPERTIESCACHE    equ       16    ;$FILENAME drm_dcp_loadpropertiescache.src
RSRC_PLAY_STATE_MACHINE_CODE_BANK    equ       17    ;$FILENAME playerstatemachine.src
RSRC_SYSMOD_CODE        equ       18    ;$FILENAME SysMod.src
RSRC_FUNCLET_DRM_B64_DECODEW    equ       19    ;$FILENAME drm_b64_decodew.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODS    equ       20    ;$FILENAME discardtrailigperiods.src
RSRC_FUNCLET_UPPERCASE    equ       21    ;$FILENAME uppercase.src
RSRC_FUNCLET_STRLENGTH    equ       22    ;$FILENAME strlength.src
RSRC_FUNCLET_CONVERTOSHORTNAME    equ       23    ;$FILENAME ConverToShortname.src
RSRC_FUNCLET_DRM_BBX_HASHVALUE    equ       24    ;$FILENAME drm_bbx_hashvalue.src
RSRC_FUNCLET_DRM_EXPR_EVALUATEEXPRESSION    equ       25    ;$FILENAME drm_expr_evaluateexpression.src
RSRC_FUNCLET_DRM_SST_CLOSEKEY    equ       26    ;$FILENAME drm_sst_closekey.src
RSRC_FUNCLET_SYSSETSPEED    equ       27    ;$FILENAME Funclet_SysSetSpeed.src
RSRC_FUNCLET_FREEHANDLE    equ       28    ;$FILENAME freehandle.src
RSRC_FUNCLET_SEARCHFREEHANDLEALLOCATE    equ       29    ;$FILENAME searchfreehandleallocate.src
RSRC_FUNCLET__PARSELICENSEATTRIBUTES    equ       30    ;$FILENAME _parselicenseattributes.src
RSRC_FUNCLET_VARIABLESECSTATEGETORSET    equ       31    ;$FILENAME variablesecstategetorset.src
RSRC_FUNCLET__SETUPLICEVALOBJECTTOSHARE    equ       32    ;$FILENAME _setuplicevalobjecttoshare.src
RSRC_FUNCLET_DRM_MGR_INITIALIZE    equ       33    ;$FILENAME drm_mgr_initialize.src
RSRC_DISPLAY_CODE_BANK    equ       34    ;$FILENAME display.src
RSRC_DISPLAY_MODULE     equ       35    ;$FILENAME DisplayModule.src
RSRC_FUNCLET_EXTRACTPATH    equ       36    ;$FILENAME extractpath.src
RSRC_FUNCLET_DRM_SST_CREATEGLOBALSTOREPASSWORD    equ       37    ;$FILENAME drm_sst_createglobalstorepassword.src
RSRC_FUNCLET__GETPRIVATEKEY    equ       38    ;$FILENAME _getprivatekey.src
RSRC_FUNCLET_DRM_HDS_OPENNAMESPACE    equ       39    ;$FILENAME drm_hds_opennamespace.src
RSRC_FUNCLET_DRM_HDS_OPENSLOT    equ       40    ;$FILENAME drm_hds_openslot.src
RSRC_FUNCLET_FCLOSE     equ       41    ;$FILENAME fclose.src
RSRC_FUNCLET_DRM_CPHR_INIT    equ       42    ;$FILENAME drm_cphr_init.src
RSRC_FUNCLET_DRM_PK_SYMMETRICCRYPT    equ       43    ;$FILENAME drm_pk_symmetriccrypt.src
RSRC_FUNCLET_DRM_MGR_BIND    equ       44    ;$FILENAME drm_mgr_bind.src
RSRC_FUNCLET__DECRYPTCONTENTKEY    equ       45    ;$FILENAME _decryptcontentkey.src
RSRC_FUNCLET_DRM_MAC_INV32    equ       46    ;$FILENAME drm_mac_inv32.src
RSRC_FUNCLET_DRM_LIC_GETATTRIBUTE    equ       47    ;$FILENAME drm_lic_getattribute.src
RSRC_FUNCLET_DRM_UTL_GETVERSIONASSTRING    equ       48    ;$FILENAME drm_utl_getversionasstring.src
RSRC_FUNCLET_DRM_DCP_GETATTRIBUTE    equ       49    ;$FILENAME drm_dcp_getattribute.src
RSRC_EFFECTS_MODULES_P    equ       50    ;$FILENAME effectsmodules.src
RSRC_JANUSX_CODE        equ       51    ;$FILENAME janusx.src
RSRC_FUNCLET_DRM_SST_OPENKEYTOKENS    equ       52    ;$FILENAME drm_sst_openkeytokens.src
RSRC_FUNCLET_EVAL       equ       53    ;$FILENAME eval.src
RSRC_FUNCLET__VERIFYSLOTHASH    equ       54    ;$FILENAME _verifyslothash.src
RSRC_JANUS_COMMON       equ       55    ;$FILENAME januscommon.src
RSRC_FUNCLET_CHANGECASE    equ       56    ;$FILENAME changecase.src
RSRC_FUNCLET_DRM_BBX_SYMMETRICVERIFY    equ       57    ;$FILENAME drm_bbx_symmetricverify.src
RSRC_FUNCLET__LOADLICENSEATTRIBUTES    equ       58    ;$FILENAME _loadlicenseattributes.src
RSRC_FUNCLET_DRM_HDS_SLOTSEEK    equ       59    ;$FILENAME drm_hds_slotseek.src
RSRC_FUNCLET_DRM_HDS_SLOTWRITE    equ       60    ;$FILENAME drm_hds_slotwrite.src
RSRC_FUNCLET_DRM_LEVL_PERFORMOPERATIONS    equ       61    ;$FILENAME drm_levl_performoperations.src
RSRC_FUNCLET_DRM_LIC_VERIFYSIGNATURE    equ       62    ;$FILENAME drm_lic_verifysignature.src
RSRC_FUNCLET_DRM_LST_GETLICENSE    equ       63    ;$FILENAME drm_lst_getlicense.src
RSRC_FUNCLET_DRM_UTL_NUMBERTOSTRING    equ       64    ;$FILENAME drm_utl_numbertostring.src
RSRC_FUNCLET_OEM_WRITEFILE    equ       65    ;$FILENAME oem_writefile.src
RSRC_FUNCLET_DRM_SST_GETDATA    equ       66    ;$FILENAME drm_sst_getdata.src
RSRC_FUNCLET_UPDATEHANDLEMODE    equ       67    ;$FILENAME updatehandlemode.src
RSRC_FUNCLET__GETLICENSEINFOANDSETUP    equ       68    ;$FILENAME _getlicenseinfoandsetup.src
RSRC_FUNCLET_DRM_LIC_COMPLETELICENSECHAIN    equ       69    ;$FILENAME drm_lic_completelicensechain.src
RSRC_DOPLAY_P           equ       70    ;$FILENAME doplay_p.src
RSRC_FATWRITE_P_CODE    equ       71    ;$FILENAME fatwritep.src
RSRC_FUNCLET_FINDFIRST    equ       72    ;$FILENAME findfirst.src
RSRC_FUNCLET_SYSLOADFATWRITE    equ       73    ;$FILENAME Funclet_SysLoadFatWrite.src
RSRC_FUNCLET_CHANGETOROOTDIRECTORY    equ       74    ;$FILENAME changetorootdirectory.src
RSRC_FUNCLET__FINDKEYPAIR    equ       75    ;$FILENAME _findkeypair.src
RSRC_FUNCLET_VARIABLEMACHINEGETORSET    equ       76    ;$FILENAME variablemachinegetorset.src
RSRC_FUNCLET__HDSSLOTENUMNEXT    equ       77    ;$FILENAME _hdsslotenumnext.src
RSRC_FUNCLET_GETLSPUBKEY    equ       78    ;$FILENAME getlspubkey.src
RSRC_FUNCLET_DRM_SST_GETTOKENVALUE    equ       79    ;$FILENAME drm_sst_gettokenvalue.src
RSRC_FUNCLET_DRM_UTL_DECODEKID    equ       80    ;$FILENAME drm_utl_decodekid.src
RSRC_FUNCLET_DRM_XML_GETNODECDATA    equ       81    ;$FILENAME drm_xml_getnodecdata.src
RSRC_FUNCLET__FREEDRMMANAGERINTERNALCONTEXTS    equ       82    ;$FILENAME _freedrmmanagerinternalcontexts.src
RSRC_FUNCLET_DRM_CLK_INITCHECKSECURECLOCK    equ       83    ;$FILENAME drm_clk_initchecksecureclock.src
RSRC_FUNCLET_DRM_SST_OPENANDLOCKSLOT    equ       84    ;$FILENAME drm_sst_openandlockslot.src
RSRC_AES_ENC            equ       85    ;$FILENAME aes_enc.src
RSRC_FUNCLET_GETPRIVKEY    equ       86    ;$FILENAME getprivkey.src
RSRC_FUNCLET_GET_UNICODE_CHARACTER_BITMAP    equ       87    ;$FILENAME Funclet_GetUnicodeCharacterBitmap.src
RSRC_PLAYLIST_CODEBANK    equ       88    ;$FILENAME playlist_codebank.src
RSRC_FUNCLET_DRM_LIC_GETENABLINGBITS    equ       89    ;$FILENAME drm_lic_getenablingbits.src
RSRC_FUNCLET_DRM_MGR_UNINITIALIZE    equ       90    ;$FILENAME drm_mgr_uninitialize.src
RSRC_FUNCLET__GETDEVICECERT    equ       91    ;$FILENAME _getdevicecert.src
RSRC_FUNCLET_DRM_LIC_REPORTACTIONS    equ       92    ;$FILENAME drm_lic_reportactions.src
RSRC_FUNCLET_DRMCRT_WCSNTOL    equ       93    ;$FILENAME drmcrt_wcsntol.src
RSRC_FUNCLET__BASICHEADERCHECKS    equ       94    ;$FILENAME _basicheaderchecks.src
RSRC_FUNCLET_DRM_HDR_GETATTRIBUTE    equ       95    ;$FILENAME drm_hdr_getattribute.src
RSRC_FUNCLET_DRM_HDS_SLOTENUMNEXT    equ       96    ;$FILENAME drm_hds_slotenumnext.src
RSRC_FUNCLET_DRM_LEVL_EVALUATEEXPRESSION    equ       97    ;$FILENAME drm_levl_evaluateexpression.src
RSRC_FUNCLET_DRM_LST_OPEN    equ       98    ;$FILENAME drm_lst_open.src
RSRC_FUNCLET_DRM_XML_GETNODEATTRIBUTE    equ       99    ;$FILENAME drm_xml_getnodeattribute.src
RSRC_FUNCLET__VERIFYSYMMERTICSIGNATURE    equ      100    ;$FILENAME _verifysymmerticsignature.src
RSRC_FUNCLET_OEM_OPENFILE    equ      101    ;$FILENAME oem_openfile.src
RSRC_FUNCLET__GETDRMFULLPATHNAME    equ      102    ;$FILENAME _getdrmfullpathname.src
RSRC_FUNCLET_OEM_GETSECURESTOREGLOBALPASSWORDSEED    equ      103    ;$FILENAME oem_getsecurestoreglobalpasswordseed.src
RSRC_FUNCLET__APPLYDIFFSTOSTORE    equ      104    ;$FILENAME _applydiffstostore.src
RSRC_FUNCLET_DRM_SST_SETDATA    equ      105    ;$FILENAME drm_sst_setdata.src
RSRC_FUNCLET_DRM_SST_GETLOCKEDDATA    equ      106    ;$FILENAME drm_sst_getlockeddata.src
RSRC_FUNCLET_DRM_SST_SETLOCKEDDATA    equ      107    ;$FILENAME drm_sst_setlockeddata.src
RSRC_PLAYERLIB_EXTRA_CODE_BANK    equ      108    ;$FILENAME playerlib_extra.src
RSRC_WMA_COMMON         equ      109    ;$FILENAME wmaCommon.src
RSRC_WMA_INIT           equ      110    ;$FILENAME wmainit.src
RSRC_PLAYLIST2TRAVERSE_CODEBANK    equ      111    ;$FILENAME playlist2traverse_codebank.src
RSRC_FUNCLET_DRM_BBX_CIPHERKEYSETUP    equ      112    ;$FILENAME drm_bbx_cipherkeysetup.src
RSRC_FUNCLET_DRM_LIC_CHECKCLOCKROLLBACK    equ      113    ;$FILENAME drm_lic_checkclockrollback.src
RSRC_FUNCLET_DRM_HDS_CLOSESTORE    equ      114    ;$FILENAME drm_hds_closestore.src
RSRC_FUNCLET__HDSLOADSRN    equ      115    ;$FILENAME _hdsloadsrn.src
RSRC_FUNCLET__LOADPRORITIZEDLIST    equ      116    ;$FILENAME _loadproritizedlist.src
RSRC_FUNCLET_DRM_LST_INITENUM    equ      117    ;$FILENAME drm_lst_initenum.src
RSRC_FUNCLET__LOADATTRIBUTESINTOCACHE    equ      118    ;$FILENAME _loadattributesintocache.src
RSRC_FUNCLET_DRM_PK_SYMMETRICVERIFY    equ      119    ;$FILENAME drm_pk_symmetricverify.src

;///////////////////////////////////////////////////////////////////////////////
;  Menu Modules (codebanks)
;///////////////////////////////////////////////////////////////////////////////
RSRC_MAIN_MENU_CODE_BANK    equ      120    ;$FILENAME mainmenu.src
RSRC_DISPLAY_LISTS_CODE_BANK    equ      121    ;$FILENAME displaylists.src

RSRC_VOICE_MENU_CODE_BANK    equ      122    ;$FILENAME voicemenu.src
RSRC_FMTUNER_MENU_CODE_BANK    equ      123    ;$FILENAME fmtunermenu.src
RSRC_RECORDER_STATE_MACHINE_CODE_BANK    equ      124    ;$FILENAME recorderstatemachine.src

RSRC_EQ_MENU_CODE_BANK    equ      125    ;$FILENAME eqmenu.src
RSRC_PLAYMODE_MENU_CODE_BANK    equ      126    ;$FILENAME playmodemenu.src
RSRC_CONTRAST_MENU_CODE_BANK    equ      127    ;$FILENAME contrastmenu.src
RSRC_PWRSETTINGS_MENU_CODE_BANK    equ      128    ;$FILENAME pwrsettingsmenu.src
RSRC_TIMEDATE_MENU_CODE_BANK    equ      129    ;$FILENAME timedatemenu.src
RSRC_SETTIME_MENU_CODE_BANK    equ      130    ;$FILENAME settimemenu.src
RSRC_SETDATE_MENU_CODE_BANK    equ      131    ;$FILENAME setdatemenu.src
RSRC_SETTINGS_MENU_CODE_BANK    equ      132    ;$FILENAME settingsmenu.src
RSRC_SYSTEM_MENU_CODE_BANK    equ      133    ;$FILENAME string_system_menu.src
RSRC_DELETE_MENU_CODE_BANK    equ      134    ;$FILENAME deletemenu.src
RSRC_ABOUT_MENU_CODE_BANK    equ      135    ;$FILENAME aboutmenu.src

RSRC_SPECTROGRAM_MENU_CODE_BANK    equ      136    ;$FILENAME spectrogram.src

RSRC_MOTION_VIDEO_MENU_CODE_BANK    equ      137    ;$FILENAME motionvideomenu.src
RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK    equ      138    ;$FILENAME motionvideomenuinitstate.src
RSRC_JPEG_DISPLAY_MENU_CODE_BANK    equ      139    ;$FILENAME jpegdisplaymenu.src
RSRC_JPEG_MANUAL_MENU_CODE_BANK    equ      140    ;$FILENAME jpegmanualmenu.src
RSRC_JPEG_THUMBNAIL_MENU_CODE_BANK    equ      141    ;$FILENAME jpegthumbnailmenu.src
RSRC_JPEG_SLIDESHOW_MENU_CODE_BANK    equ      142    ;$FILENAME jpegslideshowmenu.src
RSRC_ALBUM_ART_MENU_CODE_BANK    equ      143    ;$FILENAME albumartmenu.src
RSRC_JPEG_FILEUTIL_EXTRA_MENU_CODE_BANK    equ      144    ;$FILENAME jpegfileutilextra.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; General Modules
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_MIXMOD_CODE        equ      145    ;$FILENAME MixMod.src
RSRC_TUNER_MODULE       equ      146    ;$FILENAME TunerModule.src
RSRC_GEQOVERLAY_CODE    equ      147    ;$FILENAME geqoverlay.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Decoders/Encoders
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_DECMOD_CODE        equ      148    ;$FILENAME DecMod.src
RSRC_MP3P_CODE          equ      149    ;$FILENAME mp3p.src
RSRC_MP3X_CODE          equ      150    ;$FILENAME mp3x.src
RSRC_MP3Y_CODE          equ      151    ;$FILENAME mp3y.src
RSRC_JANUSP_CODE        equ      152    ;$FILENAME janusp.src
;RSRC_JANUSY_CODE        equ       29    ;$FILENAME janusy.src

RSRC_DEC_ADPCM_MOD_CODE    equ      153    ;$FILENAME decadpcmimamod.src
RSRC_DEC_ADPCMP_CODE    equ      154    ;$FILENAME dec_adpcmp.src
RSRC_DEC_ADPCMX_CODE    equ      155    ;$FILENAME dec_adpcmx.src
RSRC_DEC_ADPCMY_CODE    equ      156    ;$FILENAME dec_adpcmy.src

RSRC_DEC_SMVADPCM_MOD_CODE    equ      157    ;$FILENAME decadpcmsmvmod.src
RSRC_DEC_SMVADPCMP_CODE    equ      158    ;$FILENAME dec_smvadpcmp.src
RSRC_DEC_SMVADPCMX_CODE    equ      159    ;$FILENAME dec_smvadpcmx.src
RSRC_DEC_SMVADPCMY_CODE    equ      160    ;$FILENAME dec_smvadpcmy.src

RSRC_ENC_ADPCM_MOD_CODE    equ      161    ;$FILENAME encadpcmimamod.src
RSRC_ENC_ADPCMP_CODE    equ      162    ;$FILENAME enc_adpcmp.src
RSRC_ENC_ADPCMX_CODE    equ      163    ;$FILENAME enc_adpcmx.src
RSRC_ENC_ADPCMY_CODE    equ      164    ;$FILENAME enc_adpcmy.src

RSRC_JPEG_DECODER_P     equ      165    ;$FILENAME jpeg_p.src
RSRC_JPEG_DECODER_X     equ      166    ;$FILENAME jpeg_x.src
RSRC_JPEG_DECODER_Y     equ      167    ;$FILENAME jpeg_y.src
RSRC_JPEG_DECODER_Y_2    equ      168    ;$FILENAME jpeg2_y.src
RSRC_BMP_DECODER_Y_2    equ      169    ;$FILENAME bmp2_y.src
RSRC_BMP_DECODER_P      equ      170    ;$FILENAME bmp_p.src

;RSRC_SMVJPEG_DECODER_P    equ       57    ;$FILENAME smvjpeg_p.src
RSRC_SMVJPEG_DECODER_X    equ      171    ;$FILENAME smvjpeg_x.src
RSRC_SMVJPEG_DECODER_Y    equ      172    ;$FILENAME smvjpeg_y.src


;//////////////////////////////////////////////////////////////////////////////////////////////////
; System Settings
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_SETTINGS_COMMANDS    equ      173    ;$FILENAME settings.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Media Device Drivers
;//////////////////////////////////////////////////////////////////////////////////////////////////
;This resource is filled with garbage unless it is the MMC build
RSRC_MMCDD_CODE         equ      174    ;$FILENAME null.src
RSRC_SMDD_HIGH_CODE     equ      175    ;$FILENAME null.src

;///////////////////////////////////////////////////////////////////////////////
;  PlayState resources
;///////////////////////////////////////////////////////////////////////////////
RSRC_PLAY_ICON_WITH_BORDER    equ      176    ;$FILENAME play_icon_with_border.src
RSRC_PAUSE_ICON_WITH_BORDER    equ      177    ;$FILENAME pause_icon_with_border.src
RSRC_STOP_ICON_WITH_BORDER    equ      178    ;$FILENAME stop_icon_with_border.src
RSRC_RECORD_ICON_WITH_BORDER    equ      179    ;$FILENAME record_icon_with_border.src
RSRC_PAUSED_RECORD_ICON_BORDER    equ      180    ;$FILENAME paused_record_icon_with_border.src
RSRC_FFWD_ICON_WITH_BORDER    equ      181    ;$FILENAME ffwd_icon_with_border.src
RSRC_RWND_ICON_WITH_BORDER    equ      182    ;$FILENAME rwnd_icon_with_border.src

;///////////////////////////////////////////////////////////////////////////////
;  PlayMode resources
;///////////////////////////////////////////////////////////////////////////////
RSRC_REPEAT_ALL_ICON    equ      183    ;$FILENAME repeatall_icon.src
RSRC_REPEAT_SONG_ICON    equ      184    ;$FILENAME repeatsong_icon.src
RSRC_SHUFFLE_ICON       equ      185    ;$FILENAME shuffle_icon.src
RSRC_RANDOM_ICON        equ      186    ;$FILENAME random_icon.src
RSRC_REPEAT_ALL_CLEAR_ICON    equ      187    ;$FILENAME repeatallclear_icon.src
RSRC_REPEAT_SONG_CLEAR_ICON    equ      188    ;$FILENAME repeatsongclear_icon.src
RSRC_SHUFFLE_CLEAR_ICON    equ      189    ;$FILENAME shuffleclear_icon.src

;///////////////////////////////////////////////////////////////////////////////
;  Battery Status
;///////////////////////////////////////////////////////////////////////////////
RSRC_BATTERY_00         equ      190    ;$FILENAME battery_00.src
RSRC_BATTERY_01         equ      191    ;$FILENAME battery_01.src
RSRC_BATTERY_02         equ      192    ;$FILENAME battery_02.src
RSRC_BATTERY_03         equ      193    ;$FILENAME battery_03.src
RSRC_BATTERY_04         equ      194    ;$FILENAME battery_04.src
RSRC_BATTERY_05         equ      195    ;$FILENAME battery_05.src
RSRC_BATTERY_06         equ      196    ;$FILENAME battery_06.src
RSRC_BATTERY_07         equ      197    ;$FILENAME battery_07.src
RSRC_BATTERY_08         equ      198    ;$FILENAME battery_08.src
RSRC_BATTERY_09         equ      199    ;$FILENAME battery_09.src
RSRC_BATTERY_10         equ      200    ;$FILENAME battery_10.src

;///////////////////////////////////////////////////////////////////////////////
;  System Icons
;///////////////////////////////////////////////////////////////////////////////
RSRC_DISK_ICON          equ      201    ;$FILENAME disk_small.src
RSRC_LOCK_ICON          equ      202    ;$FILENAME lock_small.src
RSRC_MUSIC_MODE_ICON    equ      203    ;$FILENAME icon_music_mode.src
RSRC_VOICE_MODE_ICON    equ      204    ;$FILENAME icon_voice_mode.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Volume Bitmaps
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_ICON_VOL_00        equ      205    ;$FILENAME icon_vol_00.src
RSRC_ICON_VOL_01        equ      206    ;$FILENAME icon_vol_01.src
RSRC_ICON_VOL_02        equ      207    ;$FILENAME icon_vol_02.src
RSRC_ICON_VOL_03        equ      208    ;$FILENAME icon_vol_03.src
RSRC_ICON_VOL_04        equ      209    ;$FILENAME icon_vol_04.src
RSRC_ICON_VOL_05        equ      210    ;$FILENAME icon_vol_05.src
RSRC_ICON_VOL_06        equ      211    ;$FILENAME icon_vol_06.src
RSRC_ICON_VOL_07        equ      212    ;$FILENAME icon_vol_07.src
RSRC_ICON_VOL_08        equ      213    ;$FILENAME icon_vol_08.src
RSRC_ICON_VOL_09        equ      214    ;$FILENAME icon_vol_09.src
RSRC_ICON_VOL_10        equ      215    ;$FILENAME icon_vol_10.src
RSRC_ICON_VOL_11        equ      216    ;$FILENAME icon_vol_11.src
RSRC_ICON_VOL_12        equ      217    ;$FILENAME icon_vol_12.src
RSRC_ICON_VOL_13        equ      218    ;$FILENAME icon_vol_13.src
RSRC_ICON_VOL_14        equ      219    ;$FILENAME icon_vol_14.src
RSRC_ICON_VOL_15        equ      220    ;$FILENAME icon_vol_15.src
RSRC_ICON_VOL_16        equ      221    ;$FILENAME icon_vol_16.src
RSRC_ICON_VOL_17        equ      222    ;$FILENAME icon_vol_17.src
RSRC_ICON_VOL_18        equ      223    ;$FILENAME icon_vol_18.src
RSRC_ICON_VOL_19        equ      224    ;$FILENAME icon_vol_19.src
RSRC_ICON_VOL_20        equ      225    ;$FILENAME icon_vol_20.src
RSRC_ICON_VOL_21        equ      226    ;$FILENAME icon_vol_21.src
RSRC_ICON_VOL_22        equ      227    ;$FILENAME icon_vol_22.src
RSRC_ICON_VOL_23        equ      228    ;$FILENAME icon_vol_23.src
RSRC_ICON_VOL_24        equ      229    ;$FILENAME icon_vol_24.src
RSRC_ICON_VOL_25        equ      230    ;$FILENAME icon_vol_25.src
RSRC_ICON_VOL_26        equ      231    ;$FILENAME icon_vol_26.src
RSRC_ICON_VOL_27        equ      232    ;$FILENAME icon_vol_27.src
RSRC_ICON_VOL_28        equ      233    ;$FILENAME icon_vol_28.src
RSRC_ICON_VOL_29        equ      234    ;$FILENAME icon_vol_29.src
RSRC_ICON_VOL_30        equ      235    ;$FILENAME icon_vol_30.src
RSRC_ICON_VOL_31        equ      236    ;$FILENAME icon_vol_31.src

;///////////////////////////////////////////////////////////////////////////////////////////////////
; Splash Screen Stuff
;///////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_SPLASH_SCREEN      equ      237    ;$FILENAME st_bw1.src
RSRC_SPLASH_SCREEN_1    equ      238    ;$FILENAME siglogo1.src
RSRC_SPLASH_SCREEN_2    equ      239    ;$FILENAME siglogo2.src
RSRC_SPLASH_SCREEN_3    equ      240    ;$FILENAME siglogo3.src
RSRC_SPLASH_SCREEN_4    equ      241    ;$FILENAME siglogo4.src
RSRC_SPLASH_SCREEN_5    equ      242    ;$FILENAME siglogo5.src
RSRC_SPLASH_SCREEN_6    equ      243    ;$FILENAME siglogo6.src
RSRC_SPLASH_SCREEN_7    equ      244    ;$FILENAME siglogo7.src
RSRC_SPLASH_SCREEN_8    equ      245    ;$FILENAME siglogo8.src
RSRC_SPLASH_SCREEN_9    equ      246    ;$FILENAME siglogo9.src
RSRC_SPLASH_SCREEN_10    equ      247    ;$FILENAME siglogo10.src
RSRC_SPLASH_SCREEN_11    equ      248    ;$FILENAME siglogo11.src
RSRC_SPLASH_SCREEN_12    equ      249    ;$FILENAME siglogo12.src
RSRC_SPLASH_SCREEN_13    equ      250    ;$FILENAME siglogo13.src
RSRC_SPLASH_SCREEN_ALL    equ      251    ;$FILENAME siglogo.src

RSRC_LOCKED_SCREEN      equ      252    ;$FILENAME locked.src

;///////////////////////////////////////////////////////////////////////////////
;  Shutdown
;///////////////////////////////////////////////////////////////////////////////
RSRC_PDOWN_HOLD_STRING    equ      253    ;$FILENAME string_arial_8_continue_holding_to_power_off.src
RSRC_PDOWN_STATUS_0     equ      254    ;$FILENAME status_16_6_steps_0.src
RSRC_PDOWN_STATUS_1     equ      255    ;$FILENAME status_16_6_steps_1.src
RSRC_PDOWN_STATUS_2     equ      256    ;$FILENAME status_16_6_steps_2.src
RSRC_PDOWN_STATUS_3     equ      257    ;$FILENAME status_16_6_steps_3.src
RSRC_PDOWN_STATUS_4     equ      258    ;$FILENAME status_16_6_steps_4.src
RSRC_PDOWN_STATUS_5     equ      259    ;$FILENAME status_16_6_steps_5.src
RSRC_PDOWN_STATUS_6     equ      260    ;$FILENAME status_16_6_steps_6.src

;///////////////////////////////////////////////////////////////////////////////////////////////////
; EQ
;///////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_EQ_CLEAR_ICON      equ      261    ;$FILENAME eq_clear_icon.src
RSRC_ROCK_ICON          equ      262    ;$FILENAME rock_icon.src
RSRC_JAZZ_ICON          equ      263    ;$FILENAME jazz_icon.src
RSRC_CLASSIC_ICON       equ      264    ;$FILENAME classic_icon.src
RSRC_POP_ICON           equ      265    ;$FILENAME pop_icon.src
RSRC_CUSTOM_ICON        equ      266    ;$FILENAME custom_icon.src

;///////////////////////////////////////////////////////////////////////////////////////////////////
; AB
;///////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_AB_MARK_A_ICON     equ      267    ;$FILENAME ab_mark_a.src
RSRC_AB_MARK_B_ICON     equ      268    ;$FILENAME ab_mark_b.src

;///////////////////////////////////////////////////////////////////////////////////////////////////
; Menu Display Resources
;///////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_STRING_MUSIC_MENU    equ      269    ;$FILENAME string_music_menu.src
RSRC_STRING_MVIDEO_MENU    equ      270    ;$FILENAME string_mvideo_menu.src
RSRC_STRING_JPEG_DISPLAY_MENU    equ      271    ;$FILENAME string_jpeg_display_menu.src
RSRC_STRING_JPEG_MANUAL_MENU    equ      272    ;$FILENAME string_jpeg_manual_menu.src
RSRC_STRING_JPEG_SLIDESHOW_MENU    equ      273    ;$FILENAME string_jpeg_slideshow_menu.src
RSRC_STRING_JPEG_THUMBNAIL_MENU    equ      274    ;$FILENAME string_jpeg_thumbnail_menu.src
RSRC_STRING_VOICE_MENU    equ      275    ;$FILENAME string_voice_menu.src
RSRC_STRING_AUDIBLE_MENU    equ      276    ;$FILENAME string_audible_menu.src
RSRC_STRING_FMTUNER_MENU    equ      277    ;$FILENAME string_fmtuner_menu.src
RSRC_STRING_SETTINGS_MENU    equ      278    ;$FILENAME string_settings_menu.src
RSRC_STRING_EQ_MENU     equ      279    ;$FILENAME string_eq_menu.src
RSRC_STRING_PLAYMODE_MENU    equ      280    ;$FILENAME string_playmode_menu.src
RSRC_STRING_CONTRAST_MENU    equ      281    ;$FILENAME string_contrast_menu.src
RSRC_STRING_PWRSAVINGS_MENU    equ      282    ;$FILENAME string_pwrsavings_menu.src
RSRC_STRING_TIME_DATE_MENU    equ      283    ;$FILENAME string_time_date_menu.src
RSRC_STRING_SET_TIME_MENU    equ      284    ;$FILENAME string_set_time_menu.src
RSRC_STRING_SET_DATE_MENU    equ      285    ;$FILENAME string_set_date_menu.src
RSRC_STRING_EXIT_MENU    equ      286    ;$FILENAME string_exit_menu.src
RSRC_STRING_ROCK_MENU    equ      287    ;$FILENAME string_rock_menu.src
RSRC_STRING_POP_MENU    equ      288    ;$FILENAME string_pop_menu.src
RSRC_STRING_CLASSIC_MENU    equ      289    ;$FILENAME string_classic_menu.src
RSRC_STRING_NORMAL_MENU    equ      290    ;$FILENAME string_normal_menu.src
RSRC_STRING_JAZZ_MENU    equ      291    ;$FILENAME string_jazz_menu.src
RSRC_STRING_REPEAT1_MENU    equ      292    ;$FILENAME string_repeat1_menu.src
RSRC_STRING_REPEATALL_MENU    equ      293    ;$FILENAME string_repeatall_menu.src
RSRC_STRING_SHUFFLE_MENU    equ      294    ;$FILENAME string_shuffle_menu.src
RSRC_STRING_SHUFFLEREPEAT_MENU    equ      295    ;$FILENAME string_repeatshuffle_menu.src
RSRC_STRING_DISABLE_MENU    equ      296    ;$FILENAME string_disable_menu.src
RSRC_STRING_1MIN_MENU    equ      297    ;$FILENAME string_1min_menu.src
RSRC_STRING_2MIN_MENU    equ      298    ;$FILENAME string_2min_menu.src
RSRC_STRING_5MIN_MENU    equ      299    ;$FILENAME string_5min_menu.src
RSRC_STRING_10MIN_MENU    equ      300    ;$FILENAME string_10min_menu.src
RSRC_STRING_SYSTEM_MENU    equ      301    ;$FILENAME string_system_menu.src
RSRC_STRING_ABOUT_MENU    equ      302    ;$FILENAME string_about_menu.src
RSRC_STRING_DELETE_MENU    equ      303    ;$FILENAME string_delete_menu.src
RSRC_STRING_RECORD_MENU    equ      304    ;$FILENAME string_record_menu.src
RSRC_STRING_SPECTROGRAM_MENU    equ      305    ;$FILENAME string_spectrogram_menu.src

RSRC_STRING_END_OF_SLIDE_SHOW    equ      306    ;$FILENAME string_end_of_slide_show.src

RSRC_STRING_MB          equ      307    ;$FILENAME string_mb.src

RSRC_INT_MEDIA          equ      308    ;$FILENAME internal_media.src
RSRC_EXT_MEDIA          equ      309    ;$FILENAME external_media.src

RSRC_ABOUT_TITLE        equ      310    ;$FILENAME about_title.src
RSRC_PLAYER_NAME_VER    equ      311    ;$FILENAME player_name.src

RSRC_SETTINGS_TITLE     equ      312    ;$FILENAME settings_title.src
RSRC_JPEG_DISPLAY_TITLE    equ      313    ;$FILENAME jpeg_display_title.src
RSRC_ERASE_TITLE        equ      314    ;$FILENAME erase_title.src

RSRC_DELETE_NO          equ      315    ;$FILENAME del_warning_no.src
RSRC_DELETE_YES         equ      316    ;$FILENAME del_warning_yes.src
RSRC_WARNING_MSG_LINE1    equ      317    ;$FILENAME del_warning_line1.src
RSRC_WARNING_MSG_LINE2    equ      318    ;$FILENAME del_warning_line2.src
RSRC_BROWNOUT_BITMAP    equ      319    ;$FILENAME lowbattery.src
RSRC_VBR_BITMAP         equ      320    ;$FILENAME vbr.src

RSRC_STRING_SONG_COLON    equ      321    ;$FILENAME string_song.src
RSRC_STRING_VOICE_COLON    equ      322    ;$FILENAME string_voice.src

RSRC_TIME_DATE_TITLE    equ      323    ;$FILENAME time_date_title.src
RSRC_SET_TIME_TITLE     equ      324    ;$FILENAME set_time_title.src
RSRC_SET_DATE_TITLE     equ      325    ;$FILENAME set_date_title.src
RSRC_STRING_SEARCHING    equ      326    ;$FILENAME string_searching.src
;///////////////////////////////////////////////////////////////////////////////
;  Save Changes
;///////////////////////////////////////////////////////////////////////////////
RSRC_SAVE_CHANGES_YES    equ      327    ;$FILENAME save_changes_yes.src
RSRC_SAVE_CHANGES_NO    equ      328    ;$FILENAME save_changes_no.src
RSRC_SAVE_CHANGES_CANCEL    equ      329    ;$FILENAME save_changes_cancel.src
RSRC_SAVE_CHANGES_CLEAR    equ      330    ;$FILENAME save_changes_clear.src
;///////////////////////////////////////////////////////////////////////////////
;  Contrast
;///////////////////////////////////////////////////////////////////////////////
RSRC_CONTRAST_TITLE     equ      331    ;$FILENAME contrast_title.src
RSRC_CONTRAST_FRAME     equ      332    ;$FILENAME contrast_frame.src
RSRC_CONTRAST_LEVEL_0    equ      333    ;$FILENAME contrast_level0.src
RSRC_CONTRAST_LEVEL_1    equ      334    ;$FILENAME contrast_level1.src
RSRC_CONTRAST_LEVEL_2    equ      335    ;$FILENAME contrast_level2.src
RSRC_CONTRAST_LEVEL_3    equ      336    ;$FILENAME contrast_level3.src
RSRC_CONTRAST_LEVEL_4    equ      337    ;$FILENAME contrast_level4.src
RSRC_CONTRAST_LEVEL_5    equ      338    ;$FILENAME contrast_level5.src
RSRC_CONTRAST_LEVEL_6    equ      339    ;$FILENAME contrast_level6.src
RSRC_CONTRAST_LEVEL_7    equ      340    ;$FILENAME contrast_level7.src
RSRC_CONTRAST_LEVEL_8    equ      341    ;$FILENAME contrast_level8.src
RSRC_CONTRAST_LEVEL_9    equ      342    ;$FILENAME contrast_level9.src
RSRC_CONTRAST_LEVEL_10    equ      343    ;$FILENAME contrast_level10.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Funclets
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_SET_RTC    equ      344    ;$FILENAME Funclet_SetRTC.src
RSRC_FUNCLET_INIT_RTC    equ      345    ;$FILENAME Funclet_InitRTC.src
RSRC_FUNCLET_READ_RTC    equ      346    ;$FILENAME Funclet_ReadRTC.src
RSRC_FUNCLET_SYSRESETALLINT    equ      347    ;$FILENAME Funclet_SysResetAllInt.src
RSRC_FUNCLET_SYSSETUPDECVEC    equ      348    ;$FILENAME Funclet_SysSetupDecVec.src
RSRC_FUNCLET_SYSSETUPENCVEC    equ      349    ;$FILENAME Funclet_SysSetupEncVec.src
RSRC_FUNCLET_SYSTIMEINIT    equ      350    ;$FILENAME Funclet_SysTimeInit.src
RSRC_FUNCLET_ANALOGINIT    equ      351    ;$FILENAME Funclet_AnalogInit.src
RSRC_FUNCLET_USBCONNECTED    equ      352    ;$FILENAME Funclet_UsbConnected.src
RSRC_FUNCLET_BUTTONINIT    equ      353    ;$FILENAME Funclet_ButtonInit.src
RSRC_FUNCLET_POWERUPHEADPHONES    equ      354    ;$FILENAME Funclet_PowerUpHeadPhones.src
RSRC_FUNCLET_POWERDOWNHEADPHONES    equ      355    ;$FILENAME Funclet_PowerDownHeadPhones.src
RSRC_FUNCLET_STARTPROJECT    equ      356    ;$FILENAME Funclet_StartProject.src
RSRC_FUNCLET_MODULESETTIMER    equ      357    ;$FILENAME Funclet_ModuleSetTimer.src
RSRC_FUNCLET_MEDIADETECTION    equ      358    ;$FILENAME null.src
RSRC_FUNCLET_SYSSTOPDECODER    equ      359    ;$FILENAME Funclet_SysStopDecoder.src
RSRC_FUNCLET_SYSSTOPENCODER    equ      360    ;$FILENAME Funclet_SysStopEncoder.src
RSRC_FUNCLET_SYSSETDECODER    equ      361    ;$FILENAME Funclet_SysSetDecoder.src
RSRC_FUNCLET_SYSSETENCODER    equ      362    ;$FILENAME Funclet_SysSetEncoder.src
RSRC_FUNCLET_PROCESSPT    equ      363    ;$FILENAME null.src
RSRC_FUNCLET_DISPLAYSPLASHSCREEN    equ      364    ;$FILENAME Funclet_DisplaySplashScreen.src
RSRC_FUNCLET_SYSGETVOLUME    equ      365    ;$FILENAME Funclet_SysGetVolume.src
RSRC_FUNCLET_MMCENUMERATEPHYSICALDEVICE    equ      366    ;$FILENAME null.src
RSRC_FUNCLET_RESETDEVICE    equ      367    ;$FILENAME null.src
RSRC_FUNCLET_PROCESSCSD    equ      368    ;$FILENAME null.src
RSRC_FUNCLET_PROCESSPARTITION    equ      369    ;$FILENAME null.src
RSRC_FUNCLET_PARTITIONENTRYCOPY    equ      370    ;$FILENAME null.src
RSRC_FUNCLET_MIXER_MASTER_FADE_OUT    equ      371    ;$FILENAME Funclet_MixerMasterFadeOut.src
RSRC_FUNCLET_MIXER_MASTER_FADE_IN    equ      372    ;$FILENAME Funclet_MixerMasterFadeIn.src
RSRC_FUNCLET_ENCSETPROPERTIES    equ      373    ;$FILENAME Funclet_EncSetProperties.src
RSRC_FUNCLET_DOESMBREXIST    equ      374    ;$FILENAME null.src
RSRC_FUNCLET_EXTRACTPARTITIONPARAMETERS    equ      375    ;$FILENAME null.src
RSRC_FUNCLET_SYSSAVESETTINGS    equ      376    ;$FILENAME Funclet_SaveSettings.src
RSRC_FUNCLET_SYSLOADSETTINGS    equ      377    ;$FILENAME Funclet_LoadSettings.src
;/////////////////////////////////////////////////////////////
; Sanyo FM Tuner Fuclet
;/////////////////////////////////////////////////////////////
;RSRC_FUNCLET_TUNER_DRIVER_INIT    equ      334    ;$FILENAME Funclet_TunerDriverInit.src
;RSRC_FUNCLET_TUNER_SCAN_STATIONS    equ      335    ;$FILENAME Funclet_TunerScanStations.src
;RSRC_FUNCLET_TUNER_SET_TO_PRESET    equ      336    ;$FILENAME Funclet_TunerSetToPreset.src
;RSRC_FUNCLET_TUNER_SET_MONO_STEREO    equ      337    ;$FILENAME Funclet_TunerSetMonoStereo.src
;RSRC_FUNCLET_TUNER_SET_STANDBY    equ      338    ;$FILENAME Funclet_TunerSetStandby.src
;RSRC_FUNCLET_TUNER_SET_PRESET    equ      339    ;$FILENAME Funclet_TunerSetPreset.src
;RSRC_FUNCLET_TUNER_ERASE_PRESET    equ      340    ;$FILENAME Funclet_TunerErasePreset.src
;RSRC_FUNCLET_TUNER_SET_IF    equ      341    ;$FILENAME Funclet_TunerSetIf.src
;RSRC_FUNCLET_TUNER_WRITE_IF_OSC    equ      342    ;$FILENAME Funclet_TunerWriteIfOSc.src
;RSRC_FUNCLET_TUNER_GET_FIELD_STRENGTH    equ      343    ;$FILENAME Funclet_TunerGetFieldStrength.src
;RSRC_FUNCLET_TUNER_INIT_RF_CAP_OSC    equ      344    ;$FILENAME Funclet_TunerInitRfCapOsc.src
;RSRC_FUNCLET_TUNER_STORE_FM_STATION    equ      345    ;$FILENAME Funclet_TunerStoreFmStation.src
;RSRC_FUNCLET_TUNER_STORE_FM_PRESET    equ      346    ;$FILENAME Funclet_TunerStoreFmPreset.src
;RSRC_FUNCLET_TUNER_SET_REGION    equ      347    ;$FILENAME Funclet_TunerSetRegion.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; WMA Resources
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_WMA_CORE           equ      378    ;$FILENAME wmaCore.src
RSRC_WMA_MIDLOW         equ      379    ;$FILENAME wmaMidLow.src
RSRC_WMA_HIGH           equ      380    ;$FILENAME wmaHigh.src
RSRC_WMA_HIGHMID        equ      381    ;$FILENAME wmaHighMid.src
RSRC_WMA_MID            equ      382    ;$FILENAME wmaMid.src
RSRC_WMA_LOW            equ      383    ;$FILENAME wmaLow.src
RSRC_WMA_DATA_X1        equ      384    ;$FILENAME wmaX1mem.src
RSRC_WMA_DATA_Y         equ      385    ;$FILENAME wmaYmem.src
RSRC_WMA_DATA_L_X       equ      386    ;$FILENAME wmaLXmem.src
RSRC_WMA_DATA_L_Y       equ      387    ;$FILENAME wmaLYmem.src
RSRC_WMA_HUFF44QB       equ      388    ;$FILENAME wmaHuff44Qb.src
RSRC_WMA_HUFF44OB       equ      389    ;$FILENAME wmaHuff44Ob.src
RSRC_WMA_HUFF16OB       equ      390    ;$FILENAME wmaHuff16Ob.src
RSRC_DRMPD_COMMON       equ      391    ;$FILENAME drmpdcommon.src
RSRC_JANUS_WMASUPPORT    equ      392    ;$FILENAME januswmasupport.src
RSRC_WMA_LICENSEINIT    equ      393    ;$FILENAME wmalicenseinit.src
RSRC_WMA_TABLES         equ      394    ;$FILENAME wma_tables.src
RSRC_JANUS_TABLES       equ      395    ;$FILENAME janus_tables.src
RSRC_WMA_CONSTANTS      equ      396    ;$FILENAME wma_constants.src
RSRC_JANUS_CONSTANTS    equ      397    ;$FILENAME janus_constants.src
RSRC_JANUS_X            equ      398    ;$FILENAME janus_xmem.src
RSRC_JANUSY_DATA        equ      399    ;$FILENAME janusy_data.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Fonts -- these are last because they are very large
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_FONT_TABLE         equ      400    ;$FILENAME font_table.src
RSRC_PGM_8              equ      401    ;$FILENAME font_PGM.src
RSRC_SGMS_8             equ      402    ;$FILENAME font_SGMs.src
RSRC_SCRIPT_00_8        equ      403    ;$FILENAME font_script_00.src
RSRC_SCRIPTS_8          equ      404    ;$FILENAME font_scripts.src
RSRC_PDM                equ      405    ;$FILENAME font_PDM.src
RSRC_SDMS               equ      406    ;$FILENAME font_SDMs.src
RSRC_WARNING            equ      407    ;$FILENAME bitmap_warning.src
RSRC_DEVICE_FULL        equ      408    ;$FILENAME bitmap_device_full.src


RSRC_LCD_INIT_SEQ       equ      409    ;$FILENAME lcd_controller_init.src

RSRC_FUNCLET_DBCS_TO_UNICODE    equ      410    ;$FILENAME Funclet_DBCSToUnicode.src
RSRC_FUNCLET_DBCS_TO_UNICODE_DSTX_SRCY    equ      411    ;$FILENAME Funclet_DBCSToUnicodeDstXSrcY.src
RSRC_FUNCLET_SET_FONT    equ      412    ;$FILENAME Funclet_LCDSetFont.src
RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE    equ      413    ;$FILENAME Funclet_GetTextWidthAddressUnicode.src
RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE    equ      414    ;$FILENAME Funclet_GetTextWidthResourceUnicode.src
RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS    equ      415    ;$FILENAME Funclet_GetTextWidthAddressDBCS.src
RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS    equ      416    ;$FILENAME Funclet_GetTextWidthResourceDBCS.src
RSRC_FUNCLET_CHIPSERIALNUMBERINIT    equ      417    ;$FILENAME Funclet_ChipSerialNumberInit.src
RSRC_FUNCLET_INTERNALMEDIASERIALNUMBERINIT    equ      418    ;$FILENAME Funclet_InternalMediaSerialNumberInit.src
RSRC_FUNCLET_CHIPGETSERIALNUMBER    equ      419    ;$FILENAME Funclet_ChipGetSerialNumber.src
RSRC_FUNCLET_CREATENULLSERIALNUMBER    equ      420    ;$FILENAME Funclet_CreateNullSerialNumber.src
RSRC_FUNCLET_CONVERTHEXTOASCII    equ      421    ;$FILENAME Funclet_ConvertHexToASCII.src
RSRC_FUNCLET_LOWRESOLUTIONADCINIT    equ      422    ;$FILENAME Funclet_LowResolutionAdcInit.src


RSRC_SYSRECORD_CODE     equ      423    ;$FILENAME sysrecord.src

RSRC_STRING_RECORD_SETTINGS    equ      424    ;$FILENAME string_record_settings.src
RSRC_STRING_SAMPLE_RATE    equ      425    ;$FILENAME string_sample_rate.src
RSRC_STRING_ENCODER     equ      426    ;$FILENAME string_encoder.src
RSRC_STRING_ADPCM       equ      427    ;$FILENAME string_adpcm.src
RSRC_STRING_MSADPCM     equ      428    ;$FILENAME string_msadpcm.src
RSRC_STRING_IMADPCM     equ      429    ;$FILENAME string_imadpcm.src
RSRC_STRING_PCM         equ      430    ;$FILENAME string_pcm.src
RSRC_STRING_INTERNAL    equ      431    ;$FILENAME string_internal.src
RSRC_STRING_EXTERNAL    equ      432    ;$FILENAME string_external.src
RSRC_STRING_DEVICE      equ      433    ;$FILENAME string_device.src
RSRC_STRING_SOURCE      equ      434    ;$FILENAME string_source.src
RSRC_STRING_MICROPHONE    equ      435    ;$FILENAME string_microphone.src
RSRC_STRING_LINEIN      equ      436    ;$FILENAME string_linein.src
RSRC_STRING_BITS        equ      437    ;$FILENAME string_bits.src
RSRC_STRING_4           equ      438    ;$FILENAME string_4.src
RSRC_STRING_8           equ      439    ;$FILENAME string_8.src
RSRC_STRING_16          equ      440    ;$FILENAME string_16.src
RSRC_STRING_24          equ      441    ;$FILENAME string_24.src
RSRC_STRING_FM          equ      442    ;$FILENAME string_fm.src
RSRC_STRING_MONO        equ      443    ;$FILENAME string_mono.src
RSRC_STRING_STEREO      equ      444    ;$FILENAME string_stereo.src
RSRC_STRING_8000HZ      equ      445    ;$FILENAME string_8000hz.src
RSRC_STRING_11025HZ     equ      446    ;$FILENAME string_11025hz.src
RSRC_STRING_16000HZ     equ      447    ;$FILENAME string_16000hz.src
RSRC_STRING_22050HZ     equ      448    ;$FILENAME string_22050hz.src
RSRC_STRING_32000HZ     equ      449    ;$FILENAME string_32000hz.src
RSRC_STRING_44100HZ     equ      450    ;$FILENAME string_44100hz.src
RSRC_STRING_48000HZ     equ      451    ;$FILENAME string_48000hz.src
RSRC_STRING_CHANNELS    equ      452    ;$FILENAME string_channels.src
RSRC_STRING_SPACES      equ      453    ;$FILENAME string_spaces.src
RSRC_SLIDER_BAR         equ      454    ;$FILENAME slider_bar.src
RSRC_SLIDER_BAR_INV     equ      455    ;$FILENAME slider_bar_inv.src
RSRC_SLIDER_TRACK       equ      456    ;$FILENAME slider_track.src
RSRC_STRING_NO_FILES    equ      457    ;$FILENAME string_no_files.src

;///////////////////////////////////////////////////////////////////
;  Time and Date Resource Strings
;///////////////////////////////////////////////////////////////////
RSRC_STRING_SUNDAY      equ      458    ;$FILENAME string_sunday.src
RSRC_STRING_MONDAY      equ      459    ;$FILENAME string_monday.src
RSRC_STRING_TUESDAY     equ      460    ;$FILENAME string_tuesday.src
RSRC_STRING_WEDNESDAY    equ      461    ;$FILENAME string_wednesday.src
RSRC_STRING_THURSDAY    equ      462    ;$FILENAME string_thursday.src
RSRC_STRING_FRIDAY      equ      463    ;$FILENAME string_friday.src
RSRC_STRING_SATURDAY    equ      464    ;$FILENAME string_saturday.src
RSRC_STRING_AM          equ      465    ;$FILENAME string_am.src
RSRC_STRING_PM          equ      466    ;$FILENAME string_pm.src
RSRC_STRING_AMCLEAR     equ      467    ;$FILENAME string_amclear.src
RSRC_STRING_SLASH       equ      468    ;$FILENAME string_slash.src
RSRC_STRING_COLON       equ      469    ;$FILENAME string_colon.src
RSRC_STRING_12HOUR      equ      470    ;$FILENAME string_12hour.src
RSRC_STRING_24HOUR      equ      471    ;$FILENAME string_24hour.src
RSRC_STRING_FORMAT      equ      472    ;$FILENAME string_format.src
RSRC_STRING_MMDDYYYY    equ      473    ;$FILENAME string_mmddyyyy.src
RSRC_STRING_DDMMYYYY    equ      474    ;$FILENAME string_ddmmyyyy.src
RSRC_STRING_YYYYMMDD    equ      475    ;$FILENAME string_yyyymmdd.src
RSRC_STRING_OK          equ      476    ;$FILENAME string_ok.src
RSRC_STRING_CANCEL      equ      477    ;$FILENAME string_cancel.src
RSRC_NEGATIVE_SIGN      equ      478    ;$FILENAME negative_sign.src
RSRC_DEC_PT5_STRING     equ      479    ;$FILENAME string_dec_pt5.src
RSRC_DEC_PT0_STRING     equ      480    ;$FILENAME string_dec_pt0.src
RSRC_DB_STRING          equ      481    ;$FILENAME string_db.src
RSRC_HZ2_STRING         equ      482    ;$FILENAME string_hz2.src


RSRC_RECORD_SETTINGS_MENU_CODE_BANK    equ      483    ;$FILENAME record_settings_menu_code_bank.src
RSRC_METADATA_CODEBANK    equ      484    ;$FILENAME metadata_codebank.src
RSRC_MP3_METADATA_CODEBANK    equ      485    ;$FILENAME mp3metadata_codebank.src
RSRC_WMA_METADATA_CODEBANK    equ      486    ;$FILENAME wmametadata_codebank.src
RSRC_WAV_METADATA_CODEBANK    equ      487    ;$FILENAME wavmetadata_codebank.src
RSRC_SMV_METADATA_CODEBANK    equ      488    ;$FILENAME smvmetadata_codebank.src
RSRC_PLAYLIST2INIT_CODEBANK    equ      489    ;$FILENAME playlist2init_codebank.src

RSRC_DELETE_SUCCESSFUL    equ      490    ;$FILENAME delete_successful.src
RSRC_DELETE_ERROR       equ      491    ;$FILENAME delete_error.src
RSRC_LIC_EXPIRED        equ      492    ;$FILENAME lic_expired.src
RSRC_ID3V2_CODEBANK     equ      493    ;$FILENAME id3v2_codebank.src
RSRC_ID3V2_LYRICS_CODEBANK    equ      494    ;$FILENAME id3v2lyrics_codebank.src
RSRC_LYRICS3_CODEBANK    equ      495    ;$FILENAME lyrics3_codebank.src
RSRC_LRC_CODEBANK       equ      496    ;$FILENAME lrc_codebank.src
RSRC_LYRICS_API_CODEBANK    equ      497    ;$FILENAME lyrics_api_codebank.src
RSRC_WMA_LYRICS_CODEBANK    equ      498    ;$FILENAME wmalyrics_codebank.src
RSRC_APIC_FRAME_CODEBANK    equ      499    ;$FILENAME apicframe_codebank.src

RSRC_EXTERNAL_MEDIA_ERROR1    equ      500    ;$FILENAME exmediaerror1.src
RSRC_EXTERNAL_MEDIA_ERROR2    equ      501    ;$FILENAME exmediaerror2.src
RSRC_INTERNAL_MEDIA_ERROR1    equ      502    ;$FILENAME inmediaerror1.src

RSRC_BACKLIGHT_TITLE    equ      503    ;$FILENAME backlight_title.src
RSRC_BACKLIGHT_STATE_ON    equ      504    ;$FILENAME backlight_state_on.src
RSRC_BACKLIGHT_STATE_OFF    equ      505    ;$FILENAME backlight_state_off.src
RSRC_BACKLIGHT_MENU_CODE_BANK    equ      506    ;$FILENAME backlightmenu.src
RSRC_STRING_BACKLIGHT_MENU    equ      507    ;$FILENAME string_backlight_menu.src

RSRC_ENC_MP3_MOD_CODE    equ      508    ;$FILENAME enc_mp3mod.src
RSRC_ENC_MP3P_CODE      equ      509    ;$FILENAME enc_mp3p.src
RSRC_ENC_MP3X_CODE      equ      510    ;$FILENAME enc_mp3x.src
RSRC_ENC_MP3Y_CODE      equ      511    ;$FILENAME enc_mp3y.src
RSRC_MP3_IMPLEMENTATION    equ      512    ;$FILENAME mp3_implementation.src
RSRC_STRING_MP3         equ      513    ;$FILENAME string_mp3.src
RSRC_STRING_ALL         equ      514    ;$FILENAME string_all.src

RSRC_NANDMEDIAINIT      equ      515    ;$FILENAME mediastartup.src
RSRC_NANDMEDIAALLOCATE                  equ     0
RSRC_NANDMEDIADISCOVERALLOCATION        equ     0
RSRC_NANDMEDIAGETMEDIATABLE             equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAGETINFO                   equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAERASE                     equ     0

RSRC_NANDDATADRIVEINIT    equ      516    ;$FILENAME nanddatadriveinit.src
RSRC_NANDDATADRIVEGETINFO               equ     RSRC_NANDDATADRIVEREADSECTOR
RSRC_NANDDATADRIVESETINFO               equ     0
RSRC_NANDDATADRIVEREADSECTOR    equ      517    ;$FILENAME nanddatadrivereadsector.src
RSRC_NANDDATADRIVEWRITESECTOR           equ     RSRC_NANDDATADRIVEREADSECTOR
RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP   equ     0
RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR  equ     0
RSRC_NANDDATADRIVECACHEMULTIWRITESETUP  equ     0
RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR equ     0
RSRC_NANDDATADRIVEERASE                 equ     0
RSRC_NANDDATADRIVEFLUSH                 equ     RSRC_NANDDATADRIVEREADSECTOR

RSRC_NANDSYSTEMDRIVEINIT                equ     RSRC_NANDMEDIAINIT
RSRC_NANDSYSTEMDRIVEGETINFO             equ     RSRC_NANDMEDIAINIT
RSRC_NANDSYSTEMDRIVESETINFO             equ     0
RSRC_NANDSYSTEMDRIVEREADSECTOR          equ     0
RSRC_NANDSYSTEMDRIVEWRITESECTOR         equ     0
RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP     equ     0
RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR    equ     0
RSRC_NANDSYSTEMDRIVEERASE               equ     0
RSRC_NANDSYSTEMDRIVEFLUSH               equ     0

RSRC_VBR_CODEBANK       equ      518    ;$FILENAME vbr_codebank.src

RSRC_STRING_RECORDTEST_MENU    equ      519    ;$FILENAME string_recordtest_menu.src
RSRC_STRING_DURATION    equ      520    ;$FILENAME string_recordtest_duration.src
RSRC_STRING_TIME5       equ      521    ;$FILENAME string_recordtest_time5.src
RSRC_STRING_TIME10      equ      522    ;$FILENAME string_recordtest_time10.src
RSRC_STRING_TIME30      equ      523    ;$FILENAME string_recordtest_time30.src
RSRC_STRING_TIME60      equ      524    ;$FILENAME string_recordtest_time60.src
RSRC_STRING_TIME300     equ      525    ;$FILENAME string_recordtest_time300.src
RSRC_STRING_TIME600     equ      526    ;$FILENAME string_recordtest_time600.src

RSRC_TEST_TITLE         equ      527    ;$FILENAME test_title.src
RSRC_TEST_MENU_CODE_BANK    equ      528    ;$FILENAME testmenu.src


RSRC_MMCDATADRIVEINIT    equ      529    ;$FILENAME mmcmediastartup.src
RSRC_MMCMEDIAALLOCATE                  equ     RSRC_MMCDATADRIVEINIT
RSRC_MMCMEDIADISCOVERALLOCATION        equ     RSRC_MMCDATADRIVEINIT
RSRC_MMCMEDIAGETINFO    equ      530    ;$FILENAME mmcinfo.src
RSRC_MMCMEDIAGETMEDIATABLE             equ     RSRC_MMCMEDIAGETINFO
RSRC_MMCMEDIAERASE      equ      531    ;$FILENAME mmcerase.src


RSRC_MMCDATADRIVEFLUSH                 equ     RSRC_MMCDATADRIVEINIT

RSRC_MMCENUMERATE       equ      532    ;$FILENAME mmcenumerate.src
RSRC_MMC_RESETDEVICE    equ      533    ;$FILENAME mmcresetdevice.src
RSRC_MMC_PROCESSCSD     equ      534    ;$FILENAME mmcprocesscsd.src
RSRC_MMC_PROCESSCID     equ      535    ;$FILENAME mmcprocesscid.src
RSRC_FUNCLET_MMCPROCESSCID2    equ      536    ;$FILENAME mmcprocesscid2.src
RSRC_MMC_DETECTPRESENCE    equ      537    ;$FILENAME mmcdetectpresence.src
RSRC_MMCSERIALNUMBERINIT    equ      538    ;$FILENAME mmcserialnumberinit.src
RSRC_MMC_CHECKWRITEPROTECT    equ      539    ;$FILENAME mmccheckwriteprotect.src

RSRC_MMCDATADRIVEREADSECTOR    equ      540    ;$FILENAME mmcread.src
RSRC_MMCMEDIAINIT       equ      541    ;$FILENAME mmcmediainit.src
RSRC_MMCDATADRIVESETINFO    equ      542    ;$FILENAME mmcdatadriveinfo.src
RSRC_MMCDATADRIVEGETINFO    equ      RSRC_MMCDATADRIVESETINFO
RSRC_MMCDATADRIVEERASE    equ      543    ;$FILENAME mmcdatadriveerase.src

RSRC_MMCDATADRIVEWRITESECTOR    equ      RSRC_MMCDATADRIVEREADSECTOR
RSRC_MMCDATADRIVEMULTIWRITESETUP       equ     RSRC_MMCDATADRIVEWRITESECTOR
RSRC_MMCDATADRIVEMULTIWRITESECTOR      equ     RSRC_MMCDATADRIVEWRITESECTOR


;///////////////////////////////////////////////////////////////////////////////
;  File system
;///////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_ARRANGEFILENAME    equ      544    ;$FILENAME arrangefilename.src
RSRC_FUNCLET_CLEARCLUSTER    equ      545    ;$FILENAME clearcluster.src
RSRC_FUNCLET_CREATEDIRECTORY    equ      546    ;$FILENAME createdirectory.src
RSRC_FUNCLET_DELETECONTENT    equ      547    ;$FILENAME deletecontent.src
RSRC_FUNCLET_DELETERECORD    equ      548    ;$FILENAME deleterecord.src
RSRC_FUNCLET_FASTOPEN    equ      549    ;$FILENAME fastopen.src
RSRC_FUNCLET_FCREATE    equ      550    ;$FILENAME fcreate.src
RSRC_FUNCLET_FILEGETATTRIB    equ      551    ;$FILENAME filegetattrib.src
RSRC_FUNCLET_FILEGETDATE    equ      552    ;$FILENAME filegetdate.src
RSRC_FUNCLET_FILESETATTRIB    equ      553    ;$FILENAME filesetattrib.src
RSRC_FUNCLET_FILESETDATE    equ      554    ;$FILENAME filesetdate.src
RSRC_FUNCLET_FSINIT     equ      555    ;$FILENAME fsinit.src
RSRC_FUNCLET_FSSHUTDOWN    equ      556    ;$FILENAME fsshutdown.src
RSRC_FUNCLET_READDEVICERECORD    equ      557    ;$FILENAME readdevicerecord.src
RSRC_FUNCLET_CHECKSPACEINROOTDIR    equ      558    ;$FILENAME checkspaceinrootdir.src
RSRC_FUNCLET_SETCWDHANDLE    equ      559    ;$FILENAME setcwdhandle.src
RSRC_FUNCLET_FSDRIVEINIT    equ      560    ;$FILENAME fsdriveinit.src
RSRC_FUNCLET_FSCLEARDRIVEBUF    equ      561    ;$FILENAME fsclearBuf.src
;RSRC_FUNCLET_TOTALFREECLUSTER    equ      328    ;$FILENAME totalfreecluster.src
;RSRC_FUNCLET_TOTALFREECLUSTERFAT16    equ      328    ;$FILENAME totalfreeclusterfat16.src
RSRC_FUNCLET_FGETFASTHANDLE    equ      562    ;$FILENAME fgetfasthandle.src
RSRC_FUNCLET_ISHANDLEWRITEALLOCATED    equ      563    ;$FILENAME ishandlewriteallocated.src
RSRC_FUNCLET_ISFILEOPEN    equ      564    ;$FILENAME isfileopen.src
RSRC_FUNCLET_ISCURRWORKDIR    equ      565    ;$FILENAME iscurrworkdir.src
RSRC_FUNCLET_CHDIR      equ      566    ;$FILENAME chdir.src
RSRC_FUNCLET_CHDIR_FROM_OFFSET    equ      567    ;$FILENAME chdirFromOffset.src
RSRC_FUNCLET_DELETETREE    equ      568    ;$FILENAME deletetree.src
RSRC_FUNCLET_DELETEALLRECORDS    equ      569    ;$FILENAME deleteallrecords.src
RSRC_FUNCLET_CLEARDATA    equ      570    ;$FILENAME cleardata.src
RSRC_FUNCLET_CHANGETOLOWLEVELDIR    equ      571    ;$FILENAME changetolowleveldir.src
RSRC_FUNCLET_GETRECORDNUMBER    equ      572    ;$FILENAME getrecordnumber.src
RSRC_FUNCLET_FILEREMOVE    equ      573    ;$FILENAME fileremove.src
RSRC_FUNCLET_CHARACTERSEARCH    equ      574    ;$FILENAME charactersearch.src
RSRC_FUNCLET_STRINGCOMPARE    equ      575    ;$FILENAME stringcompare.src
RSRC_FUNCLET_FOPENW     equ      576    ;$FILENAME fopenw.src
RSRC_FUNCLET_FREMOVE    equ      577    ;$FILENAME fremove.src
RSRC_FUNCLET_FREMOVEW    equ      578    ;$FILENAME fremovew.src
RSRC_FUNCLET_MKDIR      equ      579    ;$FILENAME mkdir.src
RSRC_FUNCLET_MKDIRW     equ      580    ;$FILENAME mkdirw.src
RSRC_FUNCLET_RMDIR      equ      581    ;$FILENAME rmdir.src
RSRC_FUNCLET_RMDIRW     equ      582    ;$FILENAME rmdirw.src
RSRC_FUNCLET_FGETC      equ      583    ;$FILENAME fgetc.src
RSRC_FUNCLET_FGETS      equ      584    ;$FILENAME fgets.src
RSRC_FUNCLET_FPUTC      equ      585    ;$FILENAME fputc.src
RSRC_FUNCLET_FPUTS      equ      586    ;$FILENAME fputs.src
RSRC_FUNCLET_ARRANGELONGFILENAME    equ      587    ;$FILENAME arrangelongfilename.src
RSRC_FUNCLET_CONVERT_ITOA    equ      588    ;$FILENAME convert_itoa.src
RSRC_FUNCLET_CREATEDIRRECORD    equ      589    ;$FILENAME createdirrecord.src
RSRC_FUNCLET_CHKSUM     equ      590    ;$FILENAME chksum.src
RSRC_FUNCLET_CREATESHORTDIRRECORD    equ      591    ;$FILENAME createshortdirrecord.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODSUCS3    equ      592    ;$FILENAME discardtrailigperiodsucs3.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODSW    equ      593    ;$FILENAME discardtrailigperiodsw.src
RSRC_FUNCLET_EXTRACTFILENAMEW    equ      594    ;$FILENAME extractfilenamew.src
RSRC_FUNCLET_EXTRACTPATHW    equ      595    ;$FILENAME extractpathw.src
RSRC_FUNCLET_FINDFREERECORD    equ      596    ;$FILENAME findfreerecord.src
RSRC_FUNCLET_GETNAMEW    equ      597    ;$FILENAME getnamew.src
RSRC_FUNCLET_ISDIRECTORYEMPTY    equ      598    ;$FILENAME isdirectoryempty.src
RSRC_FUNCLET_ISSHORTNAMEVALID    equ      599    ;$FILENAME isshortnamevalid.src
RSRC_FUNCLET_LONGDIRMATCH    equ      600    ;$FILENAME longdirmatch.src
RSRC_FUNCLET_UNICODETOOEM    equ      601    ;$FILENAME unicodetooem.src
RSRC_FUNCLET_MATCHDIRRECORDW    equ      602    ;$FILENAME matchdirrecordw.src
RSRC_FUNCLET_SETCWD     equ      603    ;$FILENAME setcwd.src
RSRC_FUNCLET_SETSHORTFILENAME    equ      604    ;$FILENAME setshortfilename.src
RSRC_FUNCLET_GENERATEFILENAMETAIL    equ      605    ;$FILENAME generatefilenametail.src
RSRC_FUNCLET_DBCSTOUNICODE    equ      606    ;$FILENAME dbcstounicode.src
RSRC_FUNCLET_STRCPY     equ      607    ;$FILENAME strcpy.src
RSRC_FUNCLET_STRCPYW    equ      608    ;$FILENAME strcpyw.src
RSRC_FUNCLET_STRLENGTHW    equ      609    ;$FILENAME strlengthw.src
RSRC_FUNCLET_FILESYSTEMPRESENT    equ      610    ;$FILENAME filesystempresent.src
RSRC_FUNCLET_DATADRIVEINIT    equ      611    ;$FILENAME DataDriveInit.src
RSRC_FUNCLET_FSGETDATADRIVEPBSLOCATION    equ      612    ;$FILENAME FSGetDataDrivePbsLocation.src
RSRC_FUNCLET_FSPARTITIONENTRYCOPYFSDD    equ      613    ;$FILENAME FSPartitionEntryCopy.src
RSRC_FUNCLET_DATADRIVEGETSIZE    equ      614    ;$FILENAME DataDriveGetSize.src
RSRC_FUNCLET_CONSTRUCTLONGFILENAME    equ      615    ;$FILENAME ConstructLongFileName.src
RSRC_FUNCLET_STRCPYUCS3_2    equ      616    ;$FILENAME strcpyucs3_2.src
RSRC_FUNCLET_GETVOLUMELABEL    equ      617    ;$FILENAME getvolumelabel.src
RSRC_FUNCLET_SETVOLUMELABEL    equ      618    ;$FILENAME setvolumelabel.src
RSRC_DISK_FULL          equ      619    ;$FILENAME disk_full.src
RSRC_CHECKDISK          equ      620    ;$FILENAME chkdskstartup.src
RSRC_CHKDSK_YMEM        equ      621    ;$FILENAME chkdskstartupy.src
RSRC_LOW_PWR_EXT_DEV_LINE1    equ      622    ;$FILENAME low_level_pwr_line1.src
RSRC_LOW_PWR_EXT_DEV_LINE2    equ      623    ;$FILENAME low_level_pwr_line2.src
RSRC_STRING_BIT_RATE    equ      624    ;$FILENAME string_bit_rate.src
RSRC_STRING_96KBPS      equ      625    ;$FILENAME string_96000hz.src
RSRC_STRING_112KBPS     equ      626    ;$FILENAME string_112000hz.src
RSRC_STRING_128KBPS     equ      627    ;$FILENAME string_128000hz.src
RSRC_STRING_160KBPS     equ      628    ;$FILENAME string_160000hz.src
RSRC_STRING_192KBPS     equ      629    ;$FILENAME string_192000hz.src
RSRC_STRING_224KBPS     equ      630    ;$FILENAME string_224000hz.src
RSRC_STRING_256KBPS     equ      631    ;$FILENAME string_256000hz.src
RSRC_STRING_320KBPS     equ      632    ;$FILENAME string_320000hz.src
RSRC_STRING_HZ          equ      633    ;$FILENAME string_hz.src
RSRC_ENCODERCOMMON_P_CODE    equ      634    ;$FILENAME EncCommonp.src
RSRC_ADC_ADCX_CODE      equ      635    ;$FILENAME adc_adcx.src
RSRC_ADC_ADCY_CODE      equ      636    ;$FILENAME adc_adcy.src
RSRC_FUNCLET_LOADENCODERCOMMON    equ      637    ;$FILENAME Funclet_encodercommon.src
RSRC_STRING_ALBUM       equ      638    ;$FILENAME string_album.src
RSRC_STRING_SONG        equ      639    ;$FILENAME string_encoder_song.src
RSRC_STRING_MODE        equ      640    ;$FILENAME string_mode.src

;////////////////////////////////////////////////////////////////////////////////
; display related
;////////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_HAL_DISPLAY_INIT    equ      641    ;$FILENAME Funclet_HalDisplayInit.src
RSRC_FUNCLET_SAL_DISPLAY_INIT    equ      642    ;$FILENAME Funclet_SalDisplayInit.src
RSRC_FUNCLET_SAL_HANDLE_MSG    equ      643    ;$FILENAME Funclet_SalHandleMessage.src
RSRC_FUNCLET_FIXBITMAPHEADER    equ      644    ;$FILENAME Funclet_FixBitmapHeader.src
RSRC_FUNCLET_BITMAPREADHEADER    equ      645    ;$FILENAME Funclet_BitmapReadHeader.src
RSRC_FUNCLET_BITMAPREADPALETTE    equ      646    ;$FILENAME Funclet_BitmapReadPalette.src
RSRC_FUNCLET_BITMAPREADBITMASK    equ      647    ;$FILENAME Funclet_BitmapReadBitMask.src
RSRC_FUNCLET_BITMAPPRINTNONPALETTE    equ      648    ;$FILENAME Funclet_BitmapPrintNonPalette.src
RSRC_FUNCLET_BITMAPPRINTPALETTE    equ      649    ;$FILENAME Funclet_BitmapPrintPalette.src
RSRC_FUNCLET_DISPLAYPRINTBITMAPFILE    equ      650    ;$FILENAME Funclet_DisplayPrintBitmapFile.src
RSRC_FUNCLET_DISPLAYPRINTBITMAPRESOURCE    equ      651    ;$FILENAME Funclet_DisplayPrintBitmapResource.src

;////////////////////////////////////////////////////////////////////////////////
;WMDRM Related
;////////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_LEGACY_DECRYPTCONTENTKEY    equ      652    ;$FILENAME legacy_decryptcontentkey.src
RSRC_FUNCLET_DRM_BBX_LEGACYCIPHERKEYSETUP    equ      653    ;$FILENAME drm_bbx_legacycipherkeysetup.src
RSRC_FUNCLET_DRM_BBX_INITIALIZE    equ      654    ;$FILENAME drm_bbx_initialize.src
RSRC_FUNCLET_DRM_BBX_CANBIND    equ      655    ;$FILENAME drm_bbx_canbind.src
RSRC_FUNCLET_VERIFYCHECKSUM    equ      656    ;$FILENAME verifychecksum.src
RSRC_FUNCLET_DRM_B64_ENCODEW    equ      657    ;$FILENAME drm_b64_encodew.src
RSRC_FUNCLET__PERFORMACTIONS    equ      658    ;$FILENAME _performactions.src
RSRC_FUNCLET__PROCESSENDOFCHAIN    equ      659    ;$FILENAME _processendofchain.src
RSRC_FUNCLET_DRMCRT_ISWXDIGIT    equ      660    ;$FILENAME drmcrt_iswxdigit.src
RSRC_FUNCLET_DRMCRT_TOWLOWER    equ      661    ;$FILENAME drmcrt_towlower.src
RSRC_FUNCLET_DRMCRT_WCSLEN    equ      662    ;$FILENAME drmcrt_wcslen.src
RSRC_FUNCLET_DRMCRT_WCSNCPY    equ      663    ;$FILENAME drmcrt_wcsncpy.src
RSRC_FUNCLET_DRMCRT_MEMMOVE    equ      664    ;$FILENAME drmcrt_memmove.src
RSRC_FUNCLET_PERFORMOPERATION_PART1    equ      665    ;$FILENAME performoperation_part1.src
RSRC_FUNCLET_PERFORMOPERATION_PART2    equ      666    ;$FILENAME performoperation_part2.src
RSRC_FUNCLET_PERFORMOPERATION_PART3    equ      667    ;$FILENAME performoperation_part3.src
RSRC_FUNCLET_PERFORMOPERATION_PART4    equ      668    ;$FILENAME performoperation_part4.src
RSRC_FUNCLET_PERFORMOPERATION_PART5    equ      669    ;$FILENAME performoperation_part5.src
RSRC_FUNCLET_PERFORMOPERATION_PART6    equ      670    ;$FILENAME performoperation_part6.src
RSRC_FUNCLET_ISVALIDFUNCTION    equ      671    ;$FILENAME isvalidfunction.src
RSRC_FUNCLET_FUNCTIONGETVALUE    equ      672    ;$FILENAME functiongetvalue.src
RSRC_FUNCLET_GLOBALSETVARIABLE    equ      673    ;$FILENAME globalsetvariable.src
RSRC_FUNCLET_VARIABLEDRMKGETORSET    equ      674    ;$FILENAME variabledrmkgetorset.src
RSRC_FUNCLET_VARIABLEDRMGETORSET    equ      675    ;$FILENAME variabledrmgetorset.src
RSRC_FUNCLET_VARIABLEAPPGETORSET    equ      676    ;$FILENAME variableappgetorset.src
RSRC_FUNCLET_VARIABLELICENSEGETORSET    equ      677    ;$FILENAME variablelicensegetorset.src
RSRC_FUNCLET_VARIABLECONTENTGETORSET    equ      678    ;$FILENAME variablecontentgetorset.src
RSRC_FUNCLET_VARIABLEDEVICEGETORSET    equ      679    ;$FILENAME variabledevicegetorset.src
RSRC_FUNCLET_VARIABLEPMLICENSEGETORSET    equ      680    ;$FILENAME variablepmlicensegetorset.src
RSRC_FUNCLET_DRM_HDS_CREATESTORE    equ      681    ;$FILENAME drm_hds_createstore.src
RSRC_FUNCLET_DRM_HDS_INIT    equ      682    ;$FILENAME drm_hds_init.src
RSRC_FUNCLET_DRM_HDS_UNINIT    equ      683    ;$FILENAME drm_hds_uninit.src
RSRC_FUNCLET_DRM_HDS_OPENSTORE    equ      684    ;$FILENAME drm_hds_openstore.src
RSRC_FUNCLET_DRM_HDS_DELETESLOT    equ      685    ;$FILENAME drm_hds_deleteslot.src
RSRC_FUNCLET_DRM_HDS_SLOTRESIZE    equ      686    ;$FILENAME drm_hds_slotresize.src
RSRC_FUNCLET_DRM_HDS_INITSLOTENUM    equ      687    ;$FILENAME drm_hds_initslotenum.src
RSRC_FUNCLET_DRM_HDS_CLEANUPSTORE    equ      688    ;$FILENAME drm_hds_cleanupstore.src
RSRC_FUNCLET__HDSCOPYSLOT_CHILD2CHILD    equ      689    ;$FILENAME _hdscopyslot_child2child.src
RSRC_FUNCLET__HDSCOPYCHILDPAYLOAD    equ      690    ;$FILENAME _hdscopychildpayload.src
RSRC_FUNCLET__HDSDEFRAGMENTFILE    equ      691    ;$FILENAME _hdsdefragmentfile.src
RSRC_FUNCLET__HDSCLEANUPNAMESPACE    equ      692    ;$FILENAME _hdscleanupnamespace.src
RSRC_FUNCLET__HDSCLEANUPSTORE    equ      693    ;$FILENAME _hdscleanupstore.src
RSRC_FUNCLET_DRM_LST_CLEAN    equ      694    ;$FILENAME drm_lst_clean.src
RSRC_FUNCLET__HDSLOCATEFREEBLOCKFORSLOT    equ      695    ;$FILENAME _hdslocatefreeblockforslot.src
RSRC_FUNCLET__HDSLOCKBLOCK2DELETESLOT    equ      696    ;$FILENAME _hdslockblock2deleteslot.src
RSRC_FUNCLET__HDSUNLOCKBLOCK2DELETESLOT    equ      697    ;$FILENAME _hdsunlockblock2deleteslot.src
RSRC_FUNCLET__HDSCREATENAMESPACE    equ      698    ;$FILENAME _hdscreatenamespace.src
RSRC_FUNCLET__HDSFILEPOS2BLOCKNUM    equ      699    ;$FILENAME _hdsfilepos2blocknum.src
RSRC_FUNCLET__WRITESRN    equ      700    ;$FILENAME _writesrn.src
RSRC_FUNCLET__WRITECOMMONBLOCKHEADER    equ      701    ;$FILENAME _writecommonblockheader.src
RSRC_FUNCLET__WRITECHILDBLOCKHEADER    equ      702    ;$FILENAME _writechildblockheader.src
RSRC_FUNCLET__READDATABLOCKHEADER    equ      703    ;$FILENAME _readdatablockheader.src
RSRC_FUNCLET__WRITEDATABLOCKHEADER    equ      704    ;$FILENAME _writedatablockheader.src
RSRC_FUNCLET__HDSEXPANDSTORE    equ      705    ;$FILENAME _hdsexpandstore.src
RSRC_FUNCLET__HDSALLOCBLOCK    equ      706    ;$FILENAME _hdsallocblock.src
RSRC_FUNCLET__HDSFREEBLOCK    equ      707    ;$FILENAME _hdsfreeblock.src
RSRC_FUNCLET__HDSCREATEOVERSIZEDSLOT    equ      708    ;$FILENAME _hdscreateoversizedslot.src
RSRC_FUNCLET__HDSALLOCSLOTINFILE    equ      709    ;$FILENAME _hdsallocslotinfile.src
RSRC_FUNCLET__HDSWRITEBLOCKHDR    equ      710    ;$FILENAME _hdswriteblockhdr.src
RSRC_FUNCLET__HDSADJUSTCHILDPAYLOAD    equ      711    ;$FILENAME _hdsadjustchildpayload.src
RSRC_FUNCLET__HDSFINDLEFTMOSTLEAFBLOCK    equ      712    ;$FILENAME _hdsfindleftmostleafblock.src
RSRC_FUNCLET__HDSCREATEANDOPENSLOT    equ      713    ;$FILENAME _hdscreateandopenslot.src
RSRC_FUNCLET__HDSREMOVESLOT    equ      714    ;$FILENAME _hdsremoveslot.src
RSRC_FUNCLET__HDSCOPYANDLOCKNEWSLOT    equ      715    ;$FILENAME _hdscopyandlocknewslot.src
RSRC_FUNCLET__HDSRELOCATESLOTANDKEEPCURRLOCK    equ      716    ;$FILENAME _hdsrelocateslotandkeepcurrlock.src
RSRC_FUNCLET__HDSTRAVERSENEXTRIGHTSIBLINGBLOCK    equ      717    ;$FILENAME _hdstraversenextrightsiblingblock.src
RSRC_FUNCLET__HDSTRAVERSEBLOCKSINPOSTORDER    equ      718    ;$FILENAME _hdstraverseblocksinpostorder.src
RSRC_FUNCLET__HDSSLOTRESIZE    equ      719    ;$FILENAME _hdsslotresize.src
RSRC_FUNCLET__ISNULL    equ      720    ;$FILENAME _isnull.src
RSRC_FUNCLET__HDSGENSRNHASH    equ      721    ;$FILENAME _hdsgensrnhash.src
RSRC_FUNCLET__HDSALLOCBLOCKBUFFER    equ      722    ;$FILENAME _hdsallocblockbuffer.src
RSRC_FUNCLET__READSRN    equ      723    ;$FILENAME _readsrn.src
RSRC_FUNCLET__HDSGETPUTCHILDBLOCKNUM    equ      724    ;$FILENAME _hdsgetputchildblocknum.src
RSRC_FUNCLET__HDSHASHKEYTOINDEX    equ      725    ;$FILENAME _hdshashkeytoindex.src
RSRC_FUNCLET__HDSSLOTWRITE    equ      726    ;$FILENAME _hdsslotwrite.src
RSRC_FUNCLET__HDSINITSLOTENUM    equ      727    ;$FILENAME _hdsinitslotenum.src
RSRC_FUNCLET_DRM_LST_CLOSE    equ      728    ;$FILENAME drm_lst_close.src
RSRC_FUNCLET_DRM_LST_ENUMNEXT    equ      729    ;$FILENAME drm_lst_enumnext.src
RSRC_FUNCLET_DRM_LST_ENUMDELETE    equ      730    ;$FILENAME drm_lst_enumdelete.src
RSRC_FUNCLET__PROCESSEXTENSIONS    equ      731    ;$FILENAME _processextensions.src
RSRC_FUNCLET__PROCESSIDLIST    equ      732    ;$FILENAME _processidlist.src
RSRC_FUNCLET__PROCESSEXCLUSIONS    equ      733    ;$FILENAME _processexclusions.src
RSRC_FUNCLET__PROCESSINCLUSIONS    equ      734    ;$FILENAME _processinclusions.src
RSRC_FUNCLET_DRM_OPL_PROCESSCOPYOUTPUTLEVELDATA    equ      735    ;$FILENAME drm_opl_processcopyoutputleveldata.src
RSRC_FUNCLET__GETOPLLEVEL    equ      736    ;$FILENAME _getopllevel.src
RSRC_FUNCLET_DRM_OPL_PROCESSPLAYOUTPUTLEVELDATA    equ      737    ;$FILENAME drm_opl_processplayoutputleveldata.src
RSRC_FUNCLET__UPDATEATTRIBUTETOKENINSLOT    equ      738    ;$FILENAME _updateattributetokeninslot.src
RSRC_FUNCLET_DRM_SST_SETTOKENVALUE    equ      739    ;$FILENAME drm_sst_settokenvalue.src
RSRC_FUNCLET_DRM_SST_CREATELICENSESTATEPASSWORD    equ      740    ;$FILENAME drm_sst_createlicensestatepassword.src
RSRC_FUNCLET_DRM_UTL_PROMOTEANSITOUNICODE    equ      741    ;$FILENAME drm_utl_promoteansitounicode.src
RSRC_FUNCLET_DRM_PK_SYMMETRICSIGN    equ      742    ;$FILENAME drm_pk_symmetricsign.src
RSRC_FUNCLET_DRM_UTL_GETVERSIONFROMSTRING    equ      743    ;$FILENAME drm_utl_getversionfromstring.src
RSRC_FUNCLET_DRM_UTL_STRINGINSERTBLANKSUBSTRING    equ      744    ;$FILENAME drm_utl_stringinsertblanksubstring.src
RSRC_FUNCLET_DRM_UTL_STRINGREMOVESUBSTRING    equ      745    ;$FILENAME drm_utl_stringremovesubstring.src
RSRC_FUNCLET_DRM_UTL_DSTRSEARCH    equ      746    ;$FILENAME drm_utl_dstrsearch.src
RSRC_FUNCLET_DRM_UTL_DSTRSEARCHREVERSE    equ      747    ;$FILENAME drm_utl_dstrsearchreverse.src
RSRC_FUNCLET_DRM_UTL_STRINGTOGUID    equ      748    ;$FILENAME drm_utl_stringtoguid.src
RSRC_FUNCLET_DRM_UTL_ENSUREDATAALIGNMENT    equ      749    ;$FILENAME drm_utl_ensuredataalignment.src
RSRC_FUNCLET_OVERLAPPINGDATES    equ      750    ;$FILENAME overlappingdates.src
RSRC_FUNCLET_DRM_ASD_PARSEV2LICENSE    equ      751    ;$FILENAME drm_asd_parsev2license.src
RSRC_FUNCLET_NEGINFDATE    equ      752    ;$FILENAME neginfdate.src
RSRC_FUNCLET_INFDATE    equ      753    ;$FILENAME infdate.src
RSRC_FUNCLET_ISEXPIRED    equ      754    ;$FILENAME isexpired.src
RSRC_FUNCLET_GETSECSTATEATTR    equ      755    ;$FILENAME getsecstateattr.src
RSRC_FUNCLET_SETEXPIRYCATEGORY    equ      756    ;$FILENAME setexpirycategory.src
RSRC_FUNCLET_GETV2LICENSEINFO    equ      757    ;$FILENAME getv2licenseinfo.src
RSRC_FUNCLET_GETNEXTLICENSE    equ      758    ;$FILENAME getnextlicense.src
RSRC_FUNCLET_AGGREGATE    equ      759    ;$FILENAME aggregate.src
RSRC_FUNCLET_DRM_ASD_GETLICENSEAGGREGATEDATA    equ      760    ;$FILENAME drm_asd_getlicenseaggregatedata.src
RSRC_FUNCLET__SCANNODEFORATTRIBUTEW    equ      761    ;$FILENAME _scannodeforattributew.src
RSRC_FUNCLET__GETXMLNODECDATAW    equ      762    ;$FILENAME _getxmlnodecdataw.src
RSRC_FUNCLET_DRM_DDC_GETDEVICECERTIFICATE    equ      763    ;$FILENAME drm_ddc_getdevicecertificate.src
RSRC_FUNCLET__CREATEDEVICESTORE    equ      764    ;$FILENAME _createdevicestore.src
RSRC_FUNCLET__MAPDRMERROR    equ      765    ;$FILENAME _mapdrmerror.src
RSRC_FUNCLET__COMPAREMACHINEID    equ      766    ;$FILENAME _comparemachineid.src
RSRC_FUNCLET_INITMGRCONTEXT    equ      767    ;$FILENAME initmgrcontext.src
RSRC_FUNCLET_DRM_MGR_SETV2HEADER    equ      768    ;$FILENAME drm_mgr_setv2header.src
RSRC_FUNCLET_DRM_MGR_COMMIT    equ      769    ;$FILENAME drm_mgr_commit.src
RSRC_FUNCLET_DRM_MGR_CLEANUPLICENSESTORE    equ      770    ;$FILENAME drm_mgr_cleanuplicensestore.src
RSRC_FUNCLET_JANUSCLEANDATASTORE    equ      771    ;$FILENAME januscleandatastore.src
RSRC_FUNCLET_DRM_MTR_OPENID    equ      772    ;$FILENAME drm_mtr_openid.src
RSRC_FUNCLET_DRM_MTR_INCREMENTCOUNT    equ      773    ;$FILENAME drm_mtr_incrementcount.src
RSRC_FUNCLET_DRM_MTR_CLOSECONTEXT    equ      774    ;$FILENAME drm_mtr_closecontext.src
RSRC_FUNCLET_OEM_SETENDOFFILE    equ      775    ;$FILENAME oem_setendoffile.src
RSRC_FUNCLET_OEM_GENRANDOMBYTES    equ      776    ;$FILENAME oem_genrandombytes.src
RSRC_FUNCLET_OEM_GETFALLBACKSIGNINGKEY    equ      777    ;$FILENAME oem_getfallbacksigningkey.src
RSRC_FUNCLET_OEM_GETDEVICECERTTEMPLATE    equ      778    ;$FILENAME oem_getdevicecerttemplate.src
RSRC_FUNCLET_OEM_SETDEVICECERT    equ      779    ;$FILENAME oem_setdevicecert.src
RSRC_FUNCLET_OEM_GETCLOCKRESETSTATE    equ      780    ;$FILENAME oem_getclockresetstate.src
RSRC_FUNCLET_OEM_SETCLOCKRESETSTATE    equ      781    ;$FILENAME oem_setclockresetstate.src
RSRC_FUNCLET_OEM_GETUNIQUEID    equ      782    ;$FILENAME oem_getuniqueid.src
RSRC_FUNCLET_OEM_GETDEVICECERT    equ      783    ;$FILENAME oem_getdevicecert.src
RSRC_FUNCLET_DRM_SNC_OPENSTORE    equ      784    ;$FILENAME drm_snc_openstore.src
RSRC_FUNCLET_DRM_SNC_CLOSESTORE    equ      785    ;$FILENAME drm_snc_closestore.src
RSRC_FUNCLET__SETKIDSTOREDATA    equ      786    ;$FILENAME _setkidstoredata.src
RSRC_FUNCLET_DRM_SNC_DELETEKID    equ      787    ;$FILENAME drm_snc_deletekid.src
RSRC_FUNCLET_DRM_SNC_UPDATEKID    equ      788    ;$FILENAME drm_snc_updatekid.src
RSRC_FUNCLET_DRM_MGR_SETV1HEADER    equ      789    ;$FILENAME drm_mgr_setv1header.src
RSRC_FUNCLET_FUNCTIONGETVALUE_PART1    equ      790    ;$FILENAME functiongetvalue_part1.src
RSRC_FUNCLET_FUNCTIONGETVALUE_FN_DATEDIFF    equ      791    ;$FILENAME functiongetvalue_fn_datediff.src
RSRC_FUNCLET__HDSCREATENAMESPACESTORE    equ      792    ;$FILENAME _hdscreatenamespacestore.src
RSRC_FUNCLET_DRM_HDS_CREATESTORE2    equ      793    ;$FILENAME drm_hds_createstore2.src
RSRC_FUNCLET_DRM_HDS_OPENSTORE2    equ      794    ;$FILENAME drm_hds_openstore2.src
RSRC_FUNCLET__HDSPREALLOC    equ      795    ;$FILENAME _hdsprealloc.src
RSRC_FUNCLET_OEM_SYSTEMTIMETOFILETIME    equ      796    ;$FILENAME oem_systemtimetofiletime.src
RSRC_FUNCLET_OEM_FILETIMETOSYSTEMTIME    equ      797    ;$FILENAME oem_filetimetosystemtime.src
RSRC_FUNCLET_GENDEVICECERTIFICATE    equ      798    ;$FILENAME gendevicecertificate.src
RSRC_FUNCLET_DRMCRT_MEMMOVE_MEM    equ      799    ;$FILENAME drmcrt_memmove_mem.src
RSRC_FUNCLET_COPYHDSDTORE    equ      800    ;$FILENAME copyhdsdtore.src
RSRC_FUNCLET_GENERATEDEVICECERT    equ      801    ;$FILENAME generatedevicecert.src
RSRC_FUNCLET_OEM_GETDEVICEDATETIME    equ      802    ;$FILENAME oem_getdevicedatetime.src
RSRC_FUNCLET_DRM_MTR_UPDATEDATA    equ      803    ;$FILENAME drm_mtr_updatedata.src
RSRC_FUNCLET__HDSUPDATESRN    equ      804    ;$FILENAME _hdsupdatesrn.src
RSRC_FUNCLET_DRM_B64_DECODEW_INPLACE    equ      805    ;$FILENAME drm_b64_decodew_inplace.src
RSRC_FUNCLET__CHECKSECURECLOCK    equ      806    ;$FILENAME _checksecureclock.src
RSRC_FUNCLET__PREPARESECURECLOCKDATAFORWRITING    equ      807    ;$FILENAME _preparesecureclockdataforwriting.src
RSRC_FUNCLET__PREPARESECURECLOCKDATAFORREADING    equ      808    ;$FILENAME _preparesecureclockdataforreading.src
RSRC_FUNCLET_DRM_SST_CLOSELOCKEDSLOT    equ      809    ;$FILENAME drm_sst_closelockedslot.src
RSRC_FUNCLET_VARIABLESECURETIMEGETORSET    equ      810    ;$FILENAME variablesecuretimegetorset.src
RSRC_FUNCLET_STRTOL     equ      811    ;$FILENAME strtol.src
RSRC_FUNCLET_MKTIME     equ      812    ;$FILENAME mktime.src
RSRC_FUNCLET_GMTIME     equ      813    ;$FILENAME gmtime.src
RSRC_FUNCLET_LOCALTIME    equ      814    ;$FILENAME localtime.src
RSRC_FUNCLET__STRUCT_TM_TO_SYSTEMTIME    equ      815    ;$FILENAME _struct_tm_to_systemtime.src
RSRC_FUNCLET__SYSTEMTIME_TO_STRUCT_TM    equ      816    ;$FILENAME _systemtime_to_struct_tm.src
RSRC_FUNCLET__SYSTEMTIMETOTIME_T    equ      817    ;$FILENAME _systemtimetotime_t.src
RSRC_FUNCLET_OEM_SETSYSTEMTIME    equ      818    ;$FILENAME oem_setsystemtime.src
RSRC_CONST_PKCRYPTO     equ      819    ;$FILENAME const_pkcrypto.src
RSRC_CONST_Y            equ      820    ;$FILENAME const_y.src
RSRC_AES_DEC_TABLE      equ      821    ;$FILENAME aes_dec_table.src
RSRC_AES_KEY_TABLE      equ      822    ;$FILENAME aes_key_table.src
RSRC_AES_ENC_TABLE      equ      823    ;$FILENAME aes_enc_table.src
RSRC_DEVCERT            equ      824    ;$FILENAME device_cert.src
RSRC_DEVCERT_TEMPLATE    equ      825    ;$FILENAME devcert_template.src
RSRC_FUNCLET_GETBASE64DECODEDKEY    equ      826    ;$FILENAME getbase64decodedkey.src
RSRC_FUNCLET__INITSLOT    equ      827    ;$FILENAME _initslot.src
RSRC_HDSIMPLCOMMON_P    equ      828    ;$FILENAME hdsimplcommon.src
RSRC_HDSIMPL_P          equ      829    ;$FILENAME hdsimpl_p.src


;////////////////////////////////////////////////////////////////////////////////
;pkcrypto Related
;////////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_TWO_ADIC_INVERSE    equ      830    ;$FILENAME two_adic_inverse.src
RSRC_FUNCLET_MP_SHIFT    equ      831    ;$FILENAME mp_shift.src
RSRC_FUNCLET_MP_SIGNIFICANT_BIT_COUNT    equ      832    ;$FILENAME mp_significant_bit_count.src
RSRC_FUNCLET_SET_IMMEDIATE    equ      833    ;$FILENAME set_immediate.src
RSRC_FUNCLET_MULTIPLY_IMMEDIATE    equ      834    ;$FILENAME multiply_immediate.src
RSRC_FUNCLET_MULTIPLY    equ      835    ;$FILENAME multiply.src
RSRC_FUNCLET_DIVIDE_PRECONDITION_1    equ      836    ;$FILENAME divide_precondition_1.src
RSRC_FUNCLET_DIVIDE_IMMEDIATE    equ      837    ;$FILENAME divide_immediate.src
RSRC_FUNCLET_ECAFFINE_EXPONENTIATION_TABULAR    equ      838    ;$FILENAME ecaffine_exponentiation_tabular.src
RSRC_FUNCLET_ECAFFINE_TABLE_CONSTRUCTION    equ      839    ;$FILENAME ecaffine_table_construction.src
RSRC_FUNCLET_ECPROJ5_TO_ECAFFINE    equ      840    ;$FILENAME ecproj5_to_ecaffine.src
RSRC_FUNCLET_ECPROJ5_SET_INFINITE    equ      841    ;$FILENAME ecproj5_set_infinite.src
RSRC_FUNCLET_ECAFFINE_ON_CURVE    equ      842    ;$FILENAME ecaffine_on_curve.src
RSRC_FUNCLET_ECAFFINE_ADDITION    equ      843    ;$FILENAME ecaffine_addition.src
RSRC_FUNCLET_ECAFFINE_ADDITION_SUBTRACTION    equ      844    ;$FILENAME ecaffine_addition_subtraction.src
RSRC_FUNCLET_ECAFFINE_ATTRIBUTES2    equ      845    ;$FILENAME ecaffine_attributes2.src
RSRC_FUNCLET_KFDESC_INITIALIZE    equ      846    ;$FILENAME kfdesc_initialize.src
RSRC_FUNCLET_KIMMEDIATE    equ      847    ;$FILENAME kimmediate.src
RSRC_FUNCLET_KPRIME_IMMEDIATER    equ      848    ;$FILENAME kprime_immediater.src
RSRC_FUNCLET_KPRIME_SQRTER    equ      849    ;$FILENAME kprime_sqrter.src
RSRC_FUNCLET_KINITIALIZE_PRIME    equ      850    ;$FILENAME kinitialize_prime.src
RSRC_FUNCLET_MOD_LUCASUV    equ      851    ;$FILENAME mod_lucasuv.src
RSRC_FUNCLET_MOD_LUCAS    equ      852    ;$FILENAME mod_lucas.src
RSRC_FUNCLET_BUCKET_MULTIPLY    equ      853    ;$FILENAME bucket_multiply.src
RSRC_FUNCLET_MOD_EXP2000    equ      854    ;$FILENAME mod_exp2000.src
RSRC_FUNCLET_MOD_EXP    equ      855    ;$FILENAME mod_exp.src
RSRC_FUNCLET_MODMUL_CHOICES1    equ      856    ;$FILENAME modmul_choices1.src
RSRC_FUNCLET_MOD_SQRT    equ      857    ;$FILENAME mod_sqrt.src
RSRC_FUNCLET_CREATE_MODULUS    equ      858    ;$FILENAME create_modulus.src
RSRC_FUNCLET_FROM_MODULAR    equ      859    ;$FILENAME from_modular.src
RSRC_FUNCLET_ADD_IMMEDIATE    equ      860    ;$FILENAME add_immediate.src
RSRC_FUNCLET_ADD_DIFF    equ      861    ;$FILENAME add_diff.src
RSRC_FUNCLET_ADD_FULL    equ      862    ;$FILENAME add_full.src
RSRC_FUNCLET_COMPARE_SUM_SAME    equ      863    ;$FILENAME compare_sum_same.src
RSRC_FUNCLET_SUB_IMMEDIATE    equ      864    ;$FILENAME sub_immediate.src
RSRC_FUNCLET_MP_INITIALIZATION    equ      865    ;$FILENAME mp_initialization.src
RSRC_FUNCLET_NEW_RANDOM_BYTES    equ      866    ;$FILENAME new_random_bytes.src
RSRC_FUNCLET_NEW_RANDOM_DWORD_INTERVAL    equ      867    ;$FILENAME new_random_dword_interval.src
RSRC_FUNCLET_NEW_RANDOM_DIGIT_INTERVAL    equ      868    ;$FILENAME new_random_digit_interval.src
RSRC_FUNCLET_NEW_RANDOM_MOD    equ      869    ;$FILENAME new_random_mod.src
RSRC_FUNCLET_NEW_RANDOM_MOD_NONZERO    equ      870    ;$FILENAME new_random_mod_nonzero.src
RSRC_FUNCLET_NEW_RANDOM_DIGITS    equ      871    ;$FILENAME new_random_digits.src
RSRC_FUNCLET_WORDS_TO_ECAFFINE    equ      872    ;$FILENAME words_to_ecaffine.src
RSRC_FUNCLET_ECAFFINE_TO_DWORDS    equ      873    ;$FILENAME ecaffine_to_dwords.src
RSRC_FUNCLET__THREADUNSAFEPKINIT    equ      874    ;$FILENAME _threadunsafepkinit.src
RSRC_FUNCLET_PKINIT     equ      875    ;$FILENAME pkinit.src
RSRC_FUNCLET_DRM_PK_GENKEYPAIR    equ      876    ;$FILENAME drm_pk_genkeypair.src
RSRC_FUNCLET_DRM_PK_ENCRYPT    equ      877    ;$FILENAME drm_pk_encrypt.src
RSRC_FUNCLET_DRM_PK_DECRYPT    equ      878    ;$FILENAME drm_pk_decrypt.src
RSRC_FUNCLET_BYTE_ARRAY_MOD_BIGNUM    equ      879    ;$FILENAME byte_array_mod_bignum.src
RSRC_FUNCLET_FE2IPMOD    equ      880    ;$FILENAME fe2ipmod.src
RSRC_FUNCLET_DRM_PK_SIGN    equ      881    ;$FILENAME drm_pk_sign.src
RSRC_FUNCLET_DRM_PK_VERIFY    equ      882    ;$FILENAME drm_pk_verify.src
RSRC_FUNCLET_RANDOM_BYTES    equ      883    ;$FILENAME random_bytes.src
RSRC_FUNCLET_MP_GCDEX    equ      884    ;$FILENAME mp_gcdex.src
RSRC_FUNCLET_MP_GCDEX_SPLIT1    equ      885    ;$FILENAME mp_gcdex_split1.src
RSRC_PKCRYPTO_P         equ      886    ;$FILENAME pkcrypto_p.src
RSRC_PKCRYPTO_OVL_P     equ      887    ;$FILENAME pkcrypto_ovl_p.src
RSRC_WARNING_MSG_ALL_LINE2    equ      888    ;$FILENAME del_all_warning_line2.src
RSRC_FILE_DELETE_STAR    equ      889    ;$FILENAME del_all_file_star.src
RSRC_STRING_VOICE_MENU_DELETE_ALL    equ      890    ;$FILENAME string_voice_menu_delete_all.src
RSRC_FUNCLET_INIT5VSENSE    equ      891    ;$FILENAME Funclet_Init5VSense.src
RSRC_FUNCLET_UPDATEDCDCDUTYCYCLE    equ      892    ;$FILENAME Funclet_UpdateDCDCDutyCycle.src
RSRC_FUNCLET_CHANGE_PLAYSET    equ      893    ;$FILENAME Funclet_changeplayset.src

RSRC_FUNCLET_MMC_TEST_ERASE    equ      894    ;$FILENAME Funclet_mmctesterase.src
RSRC_FUNCLET_MMC_HALINIT    equ      895    ;$FILENAME Funclet_mmchalinit.src


; Added to allow rechargeable battery configurations to build
RSRC_BATTERY_CHARGING    equ      896    ;$FILENAME battery_charging.src
RSRC_BATTERY_CHARGE_CODEBANK    equ      897    ;$FILENAME batterychargecodebank.src
RSRC_FUNCLET_SYSUPDATEVOLUME    equ      898    ;$FILENAME updatevolume.src
RSRC_FUNCLET_TESTSKIPCHECKDISK    equ      899    ;$FILENAME Funclet_TestSkipCheckDisk.src
RSRC_FUNCLET_DISABLE_STMP_GENERATED_MICBIAS    equ      900    ;$FILENAME Funclet_DisableStmpGeneratedMicBias.src
RSRC_FUNCLET_ENABLE_STMP_GENERATED_MICBIAS    equ      901    ;$FILENAME Funclet_EnableStmpGeneratedMicBias.src
RSRC_FUNCLET_SETSKIPCHECKDISK    equ      902    ;$FILENAME Funclet_SetSkipCheckDisk.src
RSRC_FUNCLET_DRM_EXPR_EVALUATEEXPRESSION_NO_MORE_TOKENS    equ      903    ;$FILENAME drm_expr_evaluateexpression_no_more_tokens.src
RSRC_FUNCLET__ISCACHEDEVENT    equ      904    ;$FILENAME _iscachedevent.src
RSRC_FUNCLET_SETCOUNTEDEXPIRYCATEGORY    equ      905    ;$FILENAME setcountedexpirycategory.src
RSRC_OEM_DATA           equ      906    ;$FILENAME oem_data.src
RSRC_GPK_P              equ      907    ;$FILENAME gpk_p.src
RSRC_KEY_DATA           equ      908    ;$FILENAME key_data.src
RSRC_DEFRAGMENT_STORE_STRING    equ      909    ;$FILENAME string_arial_8_defragmenting_store.src
RSRC_STRING_WORKING     equ      910    ;$FILENAME string_working.src
RSRC_FUNCLET_LOADUSERTIME    equ      911    ;$FILENAME Funclet_loadusertime.src
RSRC_FUNCLET_SAVEUSERTIME    equ      912    ;$FILENAME Funclet_saveusertime.src

RSRC_FUNCLET_SYSLRADCBROWNOUTINIT    equ      913    ;$FILENAME Funclet_SysLRADCBrownoutInit.src
RSRC_FUNCLET_SYSBATTERYGETLEVEL    equ      914    ;$FILENAME Funclet_SysBatteryGetLevel.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Audible ACELP Resources
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_AUDIBLE_ACELPDEC_CODE    equ      915    ;$FILENAME AudibleAcelpDec.src
RSRC_AUDIBLE_ACELP_CODE_P    equ      916    ;$FILENAME AudibleAcelpP.src
RSRC_AUDIBLE_ACELP_DATA_X    equ      917    ;$FILENAME AudibleAcelpX.src
RSRC_AUDIBLE_ACELP_DATA_Y    equ      918    ;$FILENAME AudibleAcelpY.src

RSRC_AUDIBLE_DECMOD_CODE    equ      919    ;$FILENAME AudibleDecMod.src
RSRC_AUDIBLE_MP3P_CODE    equ      920    ;$FILENAME audiblemp3p.src
RSRC_AUDIBLE_MP3X_CODE    equ      921    ;$FILENAME audiblemp3x.src
RSRC_AUDIBLE_MP3Y_CODE    equ      922    ;$FILENAME audiblemp3y.src

RSRC_AUDIBLE_METADATA_P    equ      923    ;$FILENAME audiblemetadata_p.src
RSRC_AUDIBLE_METADATA_Y    equ      924    ;$FILENAME audiblemetadata_y.src
RSRC_AUDIBLE_SONG_POSITION_P    equ      925    ;$FILENAME audiblesongposition_p.src
RSRC_AUDIBLE_TARGET_CHECK_P    equ      926    ;$FILENAME audibletargetcheck_p.src
RSRC_AUDIBLE_FILLFILEINFO_P    equ      927    ;$FILENAME audiblefillfileinfo_p.src
RSRC_AUDIBLE_DSA_P      equ      928    ;$FILENAME audibledsa_p.src
RSRC_AUDIBLE_FILLMETADATA_P    equ      929    ;$FILENAME audiblefillmetadata_p.src
RSRC_AUDIBLE_METASTRINGS_P    equ      930    ;$FILENAME audiblemetastrings_p.src
RSRC_AUDIBLE_ACTIVATION_P    equ      931    ;$FILENAME aaactivationrecords_p.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; Effects and SRS Resources
;//////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_SRS_EFFECTS_MODULES_P    equ      932    ;$FILENAME srs_effectsmodules_p.src
RSRC_SRS_EFFECTS_MODULES_X    equ      933    ;$FILENAME srs_effectsmodules_x.src
RSRC_WOW_CTRL           equ      934    ;$FILENAME wowctrl.src

RSRC_WOW_MENU_CODE_BANK    equ      935    ;$FILENAME wowmenu.src
RSRC_STRING_WOW_BRIGHTNESS    equ      936    ;$FILENAME string_wowbrightness_menu.src
RSRC_STRING_WOW_MENU    equ      937    ;$FILENAME string_wow_menu.src
RSRC_STRING_WOW_TRUBASS    equ      938    ;$FILENAME string_wowtrubass_menu.src
RSRC_STRING_WOW_VOLUME    equ      939    ;$FILENAME string_wowvolume_menu.src
RSRC_STRING_WOW_BRIGHT_LOW    equ      940    ;$FILENAME string_wowbrightness_low_menu.src
RSRC_STRING_WOW_BRIGHT_MID    equ      941    ;$FILENAME string_wowbrightness_mid_menu.src
RSRC_STRING_WOW_BRIGHT_HIGH    equ      942    ;$FILENAME string_wowbrightness_high_menu.src
RSRC_WOW_ICON           equ      943    ;$FILENAME wow_icon.src

RSRC_WOW16K             equ      944    ;$FILENAME wow16k.src
RSRC_WOW32K             equ      945    ;$FILENAME wow32k.src
RSRC_WOW8K              equ      946    ;$FILENAME wow8k.src
RSRC_WOW11K             equ      947    ;$FILENAME wow11k.src
RSRC_WOW22K             equ      948    ;$FILENAME wow22k.src
RSRC_WOW24K             equ      949    ;$FILENAME wow24k.src
RSRC_WOW44K             equ      950    ;$FILENAME wow44k.src
RSRC_WOW48K             equ      951    ;$FILENAME wow48k.src

RSRC_WOW16K_Y           equ      952    ;$FILENAME wow16k_Y.src
RSRC_WOW32K_Y           equ      953    ;$FILENAME wow32k_Y.src
RSRC_WOW8K_Y            equ      954    ;$FILENAME wow8k_Y.src
RSRC_WOW11K_Y           equ      955    ;$FILENAME wow11k_Y.src
RSRC_WOW22K_Y           equ      956    ;$FILENAME wow22k_Y.src
RSRC_WOW24K_Y           equ      957    ;$FILENAME wow24k_Y.src
RSRC_WOW44K_Y           equ      958    ;$FILENAME wow44k_Y.src
RSRC_WOW48K_Y           equ      959    ;$FILENAME wow48k_Y.src

;///////////////////////////////////////////////////////////////////////////////////////////////////
; Audible Section Navigation
;///////////////////////////////////////////////////////////////////////////////////////////////////
RSRC_AUDIBLE_SECNAV_ICON    equ      960    ;$FILENAME audible_secnav.src

;//////////////////////////////////////////////////////////////////////////////////////////////////
; PLAYLIST3 and Music Library
;//////////////////////////////////////////////////////////////////////////////////////////////////

RSRC_BUILD_ML           equ      961    ;$FILENAME build_ml.src
RSRC_BUILD_ML_WARNING    equ      962    ;$FILENAME build_ml_warning.src
RSRC_BUILD_ML_WARNING2    equ      963    ;$FILENAME build_ml_warning2.src
RSRC_BUILD_FLASH1       equ      964    ;$FILENAME build_flash1.src
RSRC_BUILD_FLASH2       equ      965    ;$FILENAME build_flash2.src
RSRC_BUILD_FLASH3       equ      966    ;$FILENAME build_flash3.src
RSRC_BUILD_SD1          equ      967    ;$FILENAME build_sd1.src
RSRC_BUILD_SD2          equ      968    ;$FILENAME build_sd2.src
RSRC_BUILD_SD3          equ      969    ;$FILENAME build_sd3.src
RSRC_BUILD_NEWMUSIC     equ      970    ;$FILENAME build_newmusic.src
RSRC_SDMD_MENU_CODE_BANK    equ      971    ;$FILENAME sdmd.src
RSRC_MUSICLIB_BUILD_FUNCTION_SEC    equ      972    ;$FILENAME musiclib_build_function_sec.src
RSRC_MUSICLIB_BUILD_FUNCTION_FLUSH    equ      973    ;$FILENAME musiclib_build_function_flush.src
RSRC_MUSICLIB_BUILD_MODULE    equ      974    ;$FILENAME MusicLibBuildModule.src
RSRC_MUSICLIB_BUILD_GENRE_TABLE    equ      975    ;$FILENAME MusicLibBuildGenreTable.src
RSRC_MUSICLIB_BUILD_FLUSH_MEM    equ      976    ;$FILENAME MusicLibBuildFlushMem.src
RSRC_MUSICLIB_BROWSE_MODULE    equ      977    ;$FILENAME MusicLibBrowseModule.src
RSRC_MUSICLIB_PLAY_MODULE    equ      978    ;$FILENAME MusicLibPlayModule.src
RSRC_MUSICLIB_PLAYALLNEXT_MODULE    equ      979    ;$FILENAME MusicLibPlayAllNextModule.src
RSRC_MUSICLIB_PLAYALLPREV_MODULE    equ      980    ;$FILENAME MusicLibPlayAllPrevModule.src
RSRC_MUSICLIB_SHUFFLE_MODULE    equ      981    ;$FILENAME MusicLibShuffleModule.src
RSRC_FUNCLET_WRITE_DETECTION_ACCESS    equ      982    ;$FILENAME Funclet_write_detection_access.src
RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO    equ      983    ;$FILENAME Funclet_LOCAL_GetLvlOneItem_info.src
RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO    equ      984    ;$FILENAME Funclet_LOCAL_GetFlashAssociatedList_info.src
RSRC_FUNCLET_ML_RESUMEPLAYSTATE    equ      985    ;$FILENAME Funclet_ML_ResumePlayState.src
RSRC_FUNCLET_ML_UPDATE_ON_THE_GO    equ      986    ;$FILENAME Funclet_ML_UpdateOnTheGo.src
RSRC_FUNCLET_ML_CH_ON_THE_GO    equ      987    ;$FILENAME Funclet_ML_ChOnTheGo.src
RSRC_FUNCLET_ML_LOAD_ON_THE_GO    equ      988    ;$FILENAME Funclet_ML_LoadOnTheGo.src
RSRC_FUNCLET_BUILD_INIT_AND_USE_CACHE    equ      989    ;$FILENAME Funclet_Build_InitandUseCache.src
RSRC_FUNCLET_BUILD_FLUSH_AND_CLOSE_CACHE    equ      990    ;$FILENAME Funclet_Build_FlushAndCloseCache.src
RSRC_FUNCLET_ML_MOVE_POINTER_BACKWARD    equ      991    ;$FILENAME Funclet_ML_movePointerBackward.src
RSRC_FUNCLET_ML_FIND_LAST_ITEM_POINTER    equ      992    ;$FILENAME Funclet_ML_findLastItemPointer.src
RSRC_FUNCLET_ML_START_SHUFFLE    equ      993    ;$FILENAME Funclet_ML_startShuffle.src
RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ID    equ      994    ;$FILENAME Funclet_ML_GetFlashDirAssocListId.src
RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ROOT_ID    equ      995    ;$FILENAME Funclet_ML_GetFlashDirAssocListRootId.src
RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_SIZE    equ      996    ;$FILENAME Funclet_ML_GetFlashDirAssocListSize.src
RSRC_FUNCLET_ML_GET_ID_BY_STATE_AND_POSITION    equ      997    ;$FILENAME Funclet_ML_GetIdByStateAndPosition.src
RSRC_FUNCLET_ML_START_PLAY_DIR_SONG_INIT    equ      998    ;$FILENAME Funclet_ML_StartPlayDirSongInit.src
RSRC_FUNCLET_ML_SHUFFLE_NEXT_DIR_SONG    equ      999    ;$FILENAME Funclet_ML_ShuffleNextDirSong.src
RSRC_FUNCLET_ML_BROWSE_MENU    equ     1000    ;$FILENAME Funclet_ML_BrowseMenu.src
RSRC_FUNCLET_ML_START_PLAY_ALL_SONG    equ     1001    ;$FILENAME Funclet_ML_StartPlayAllSong.src
RSRC_FUNCLET_ML_PLAY_ALL_NEXT_SONG    equ     1002    ;$FILENAME Funclet_ML_PlayAllNextSong.src
RSRC_FUNCLET_ML_PLAY_ALL_PREVIOUS_SONG    equ     1003    ;$FILENAME Funclet_ML_PlayAllPreviousSong.src
RSRC_FUNCLET_ML_PLAY_PREVIOUS_DIR_SONG    equ     1004    ;$FILENAME Funclet_ML_PlayPreviousDirSong.src
RSRC_FUNCLET_ML_SHUFFLE_NEXT_SONG    equ     1005    ;$FILENAME Funclet_ML_ShuffleNextSong.src
RSRC_FUNCLET_ML_PLAY_NEXT_SONG    equ     1006    ;$FILENAME Funclet_ML_PlayNextSong.src
RSRC_FUNCLET_ML_PLAY_PREVIOUS_SONG    equ     1007    ;$FILENAME Funclet_ML_PlayPreviousSong.src
RSRC_FUNCLET_BUILDVOICEFILEPATH    equ     1008    ;$FILENAME Funclet_BuildVoiceFilePath.src
RSRC_FUNCLET_BUILDFMFILEPATH    equ     1009    ;$FILENAME Funclet_BuildFMFilePath.src
RSRC_FUNCLET_BUILDLINEINFILEPATH    equ     1010    ;$FILENAME Funclet_BuildLINEINFilePath.src
RSRC_FUNCLET_GETVOICEFASTKEY    equ     1011    ;$FILENAME Funclet_GetVoiceFastKey.src
RSRC_MUSICLIB_VOICE_APP    equ     1012    ;$FILENAME MusicLibVoiceApp.src
RSRC_MUSICLIB_VOICE_MODULE    equ     1013    ;$FILENAME MusicLibVoiceModule.src
RSRC_MUSICLIB_VOICEPLAY_MODULE    equ     1014    ;$FILENAME MusicLibVoicePlayModule.src
RSRC_MUSICLIB_MERGE_FUNCTION    equ     1015    ;$FILENAME MusicLibMergeFunction.src
RSRC_MUSICLIB_MERGE_MODULE    equ     1016    ;$FILENAME MusicLibMergeModule.src
RSRC_MUSICLIB_BROWSE_APP    equ     1017    ;$FILENAME MusicLibBrowseApp.src
RSRC_PLAYMUSIC_MENU_CODE_BANK    equ     1018    ;$FILENAME playmusicmenu.src
RSRC_BROWSE_MENU_CODE_BANK    equ     1019    ;$FILENAME browsemenu.src
RSRC_BROWSE_MENU_EXTRA_CODE_BANK    equ     1020    ;$FILENAME browsemenu_extra.src
RSRC_STRING_PLAY_ALL    equ     1021    ;$FILENAME string_play_all.src
RSRC_STRING_PLAY        equ     1022    ;$FILENAME string_play.src
RSRC_STRING_UNKNOWN_YEAR    equ     1023    ;$FILENAME string_unknown_year.src
RSRC_STRING_YEAR_WIDTH    equ     1024    ;$FILENAME string_year_width.src
RSRC_STRING_ARTIST      equ     1025    ;$FILENAME string_artist.src
RSRC_STRING_SONGS       equ     1026    ;$FILENAME string_songs.src
RSRC_STRING_ON_THE_FLY    equ     1027    ;$FILENAME string_on_the_fly.src
RSRC_STRING_NEW_MUSIC_MENU    equ     1028    ;$FILENAME string_new_music.src
RSRC_STRING_GENRE       equ     1029    ;$FILENAME string_genre.src
RSRC_STRING_YEAR        equ     1030    ;$FILENAME string_year.src
RSRC_STRING_PLAYLIST    equ     1031    ;$FILENAME string_playlist.src
RSRC_STRING_FM_REC      equ     1032    ;$FILENAME string_fm_rec.src
RSRC_STRING_LINEIN_REC    equ     1033    ;$FILENAME string_linein_rec.src
RSRC_STRING_PLAY_MUSIC    equ     1034    ;$FILENAME string_play_music.src
RSRC_HIGHLIGHT_BACK     equ     1035    ;$FILENAME highlight_back.src
RSRC_NEW_MUSIC_MENU_CODE_BANK    equ     1036    ;$FILENAME newmusicmenu.src
RSRC_STRING_1_DAY       equ     1037    ;$FILENAME string_1_day.src
RSRC_STRING_1_WEEK      equ     1038    ;$FILENAME string_1_week.src
RSRC_STRING_1_MONTH     equ     1039    ;$FILENAME string_1_month.src
RSRC_LIST_FULL_MESSAGE    equ     1040    ;$FILENAME on_the_fly_full.src
RSRC_LIST_FREE_MESSAGE    equ     1041    ;$FILENAME on_the_fly_free1.src
RSRC_LIST_FREE_MESSAGE2    equ     1042    ;$FILENAME on_the_fly_free2.src
RSRC_LIST_DELETE_MESSAGE    equ     1043    ;$FILENAME on_the_fly_delete1.src
RSRC_LIST_DELETE_MESSAGE2    equ     1044    ;$FILENAME on_the_fly_delete2.src
RSRC_EMPTY_FAVOURITE    equ     1045    ;$FILENAME empty_favourite.src
RSRC_SD_REMOVE          equ     1046    ;$FILENAME sd_remove.src
RSRC_SD_INSERT          equ     1047    ;$FILENAME sd_insert.src
RSRC_CHECK_DISK_1       equ     1048    ;$FILENAME check_disk_1.src
RSRC_CHECK_DISK_2       equ     1049    ;$FILENAME check_disk_2.src
RSRC_CHECK_DISK_3       equ     1050    ;$FILENAME check_disk_3.src
RSRC_FLASH_ERROR        equ     1051    ;$FILENAME flash_error.src

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; STFM1000 Tuner funclet
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RSRC_FUNCLET_TUNER_DRIVER_INIT    equ     1052    ;$FILENAME Funclet_TunerDriverInit.src
RSRC_FUNCLET_TUNER_DRIVER_INC_UP    equ     1053    ;$FILENAME Funclet_TunerDriverIncUp.src
RSRC_FUNCLET_TUNER_DRIVER_INC_DOWN    equ     1054    ;$FILENAME Funclet_TunerDriverIncDown.src
RSRC_FUNCLET_TUNER_DRIVER_SET_STANDBY    equ     1055    ;$FILENAME Funclet_TunerDriverStandyby.src
RSRC_FUNCLET_TUNER_DRIVER_REPLACE_PRESET    equ     1056    ;$FILENAME Funclet_TunerDriverReplacePreset.src
RSRC_FUNCLET_TUNER_DRIVER_ERASE_PRESET    equ     1057    ;$FILENAME Funclet_TunerDriverErasePreset.src
RSRC_FUNCLET_TUNER_DRIVER_TUNE_TO_PRESET    equ     1058    ;$FILENAME Funclet_TunerDriverTuneToPreset.src
RSRC_FUNCLET_TUNER_DRIVER_SEARCH_DOWN    equ     1059    ;$FILENAME Funclet_TunerDriverSearchDown.src
RSRC_FUNCLET_TUNER_SAFE_POWER_DOWN    equ     1060    ;$FILENAME Funclet_TunerSTFM1000SafePowerDown.src
RSRC_FUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL    equ     1061    ;$FILENAME Funclet_TunerSTFM1000OptimizeChannel.src
RSRC_DEC_STFM_MOD_CODE    equ     1062    ;$FILENAME decstfmmod.src
RSRC_DEC_STFMP_CODE     equ     1063    ;$FILENAME dec_stfmp.src
RSRC_DEC_STFMX_CODE     equ     1064    ;$FILENAME dec_stfmx.src
RSRC_DEC_STFMY_CODE     equ     1065    ;$FILENAME dec_stfmy.src
RSRC_FUNCLET_STFM1000_I2C_INIT    equ     1066    ;$FILENAME Funclet_TunerSTFM1000I2CInit.src
RSRC_FUNCLET_TUNER_DRIVER_SCAN_STATION    equ     1067    ;$FILENAME Funclet_TunerDriverScanStations.src
RSRC_FUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE    equ     1068    ;$FILENAME Funclet_TunerDriverSetMonoStereoMode.src
RSRC_FUNCLET_STFM1000_TEST_PRESENCE    equ     1069    ;$FILENAME Funclet_STFM1000TestPresence.src
RSRC_FUNCLET_I2C_WRITE_TUNER_REGISTER    equ     1070    ;$FILENAME Funclet_I2CWriteTunerRegister.src
RSRC_FUNCLET_I2C_RESET    equ     1071    ;$FILENAME Funclet_I2CReset.src
RSRC_FM_TUNER_SECOND_RSRC    equ     1072    ;$FILENAME fmtunersecondrsrc.src
RSRC_FUNCLET_STFM1000_REG_POWER_UP1    equ     1073    ;$FILENAME Funclet_TunerSTFM1000RegPowerUp1.src
RSRC_FUNCLET_STFM1000_REG_POWER_UP2    equ     1074    ;$FILENAME Funclet_TunerSTFM1000RegPowerUp2.src
RSRC_FUNCLET_STFM1000_FIND_RSSI_THRESHOLD    equ     1075    ;$FILENAME Funclet_TunerSTFM1000FindRssiThreshold.src
RSRC_FUNCLET_STFM1000_DATA_PATH_OFF    equ     1076    ;$FILENAME Funclet_TunerSTFM1000DataPathOff.src
RSRC_FUNCLET_STFM1000_CONTINUE_SEARCHING    equ     1077    ;$FILENAME Funclet_TunerSTFM1000ContinueSearching.src
;RSRC_FUNCLET_STFM1000_SET_LNA_BIAS    equ      975    ;$FILENAME Funclet_STFM1000SetLnaBias.src
;RSRC_FUNCLET_STFM1000_LOOK_FOR_PILOT    equ      976    ;$FILENAME Funclet_STFM1000LookForPilot.src
RSRC_FUNCLET_STFM1000_TUNER_SEARCH_ENDED    equ     1078    ;$FILENAME Funclet_STFM1000TunerSearchEnded.src
; for RestoreDriveFromBackup
RSRC_RESTORESYSDRIVE_P_CODE    equ     1079    ;$FILENAME restoresysdrive.src

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Playlist5 sources
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RSRC_PL5BROWSE_MENU_CODEBANK    equ     1080    ;$FILENAME playlist5_browsemenu.src
RSRC_PL5_BROWSEMENU_EXTRA    equ     1081    ;$FILENAME playlist5_browsemenu_extra.src
RSRC_PL5BROWSE_SUBMENU_CODEBANK    equ     1082    ;$FILENAME playlist5_browse_submenu.src
RSRC_PLAYLIST5_PLAYBACK_MODULE    equ     1083    ;$FILENAME playlist5_playback_module.src
RSRC_PLAYLIST5_BROWSE_MODULE    equ     1084    ;$FILENAME playlist5_browse_module.src

RSRC_FUNCLET_GET_LRC_DIR_NAME_PL5    equ     1085    ;$FILENAME Funclet_Get_LRCDirName_PL5.src
RSRC_FUNCLET_PL5_PL_QUEUEPUSHBACK    equ     1086    ;$FILENAME Funclet_PL5_PL_QueuePushBack.src   
RSRC_FUNCLET_PL5_PL_QUEUEPUSHFRONT    equ     1087    ;$FILENAME Funclet_PL5_PL_QueuePushFront.src 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DanhNguyen added bitmaps
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RSRC_ICON_FOLDER        equ     1088    ;$FILENAME icon_folder.src
RSRC_ICON_SONG          equ     1089    ;$FILENAME icon_song.src

RSRC_MENU_MUSIC         equ     1090    ;$FILENAME menu_music.src
RSRC_VIE_MENU_MUSIC     equ     1091    ;$FILENAME vie_menu_music.src

RSRC_MENU_VOICE         equ     1092    ;$FILENAME menu_voice.src
RSRC_VIE_MENU_VOICE     equ     1093    ;$FILENAME vie_menu_voice.src

RSRC_MENU_FMTUNER       equ     1094    ;$FILENAME menu_fmtuner.src
RSRC_VIE_MENU_FMTUNER    equ     1095    ;$FILENAME vie_menu_fmtuner.src

RSRC_MENU_RECORD        equ     1096    ;$FILENAME menu_record.src
RSRC_VIE_MENU_RECORD    equ     1097    ;$FILENAME vie_menu_record.src

RSRC_MENU_SETTINGS      equ     1098    ;$FILENAME menu_settings.src
RSRC_VIE_MENU_SETTINGS    equ     1099    ;$FILENAME vie_menu_settings.src

RSRC_MENU_SHUTDOWN      equ     1100    ;$FILENAME menu_shutdown.src
RSRC_VIE_MENU_SHUTDOWN    equ     1101    ;$FILENAME vie_menu_shutdown.src

RSRC_MENU_CLOCK         equ     1102    ;$FILENAME menu_clock.src
RSRC_VIE_MENU_CLOCK     equ     1103    ;$FILENAME vie_menu_clock.src

RSRC_MENU_AB            equ     1104    ;$FILENAME menu_ab.src
RSRC_VIE_MENU_AB        equ     1105    ;$FILENAME vie_menu_ab.src

RSRC_MENU_DELETE        equ     1106    ;$FILENAME menu_delete.src
RSRC_VIE_MENU_DELETE    equ     1107    ;$FILENAME vie_menu_delete.src

RSRC_MENU_ABOUT         equ     1108    ;$FILENAME menu_about.src
RSRC_VIE_MENU_ABOUT     equ     1109    ;$FILENAME vie_menu_about.src

RSRC_MENU_EXIT          equ     1110    ;$FILENAME menu_exit.src
RSRC_VIE_MENU_EXIT      equ     1111    ;$FILENAME vie_menu_exit.src

RSRC_MUSIC_PLAY_ALL     equ     1112    ;$FILENAME music_play_all.src
RSRC_VIE_MUSIC_PLAY_ALL    equ     1113    ;$FILENAME vie_music_play_all.src

RSRC_MUSIC_FOLDER_INTERNAL    equ     1114    ;$FILENAME music_folder_internal.src
RSRC_VIE_MUSIC_FOLDER_INTERNAL    equ     1115    ;$FILENAME vie_music_folder_internal.src

RSRC_MUSIC_FOLDER_EXTERNAL    equ     1116    ;$FILENAME music_folder_external.src
RSRC_VIE_MUSIC_FOLDER_EXTERNAL    equ     1117    ;$FILENAME vie_music_folder_external.src

RSRC_MUSIC_SONGS        equ     1118    ;$FILENAME music_songs.src
RSRC_VIE_MUSIC_SONGS    equ     1119    ;$FILENAME vie_music_songs.src

RSRC_MUSIC_FAVORITES    equ     1120    ;$FILENAME music_favorites.src
RSRC_VIE_MUSIC_FAVORITES    equ     1121    ;$FILENAME vie_music_favorites.src

RSRC_MUSIC_FM_RECORD    equ     1122    ;$FILENAME music_fm_record.src
RSRC_VIE_MUSIC_FM_RECORD    equ     1123    ;$FILENAME vie_music_fm_record.src

RSRC_MUSIC_EXIT         equ     1124    ;$FILENAME music_exit.src
RSRC_VIE_MUSIC_EXIT     equ     1125    ;$FILENAME vie_music_exit.src

RSRC_BROWSE_MUSIC_FOLDER_INTERNAL    equ     1126    ;$FILENAME browse_music_folder_internal.src
RSRC_VIE_BROWSE_MUSIC_FOLDER_INTERNAL    equ     1127    ;$FILENAME vie_browse_music_folder_internal.src

RSRC_BROWSE_MUSIC_FOLDER_EXTERNAL    equ     1128    ;$FILENAME browse_music_folder_external.src
RSRC_VIE_BROWSE_MUSIC_FOLDER_EXTERNAL    equ     1129    ;$FILENAME vie_browse_music_folder_external.src

RSRC_BROWSE_MUSIC_LIST_SONGS    equ     1130    ;$FILENAME browse_music_list_songs.src
RSRC_VIE_BROWSE_MUSIC_LIST_SONGS    equ     1131    ;$FILENAME vie_browse_music_list_songs.src

RSRC_BROWSE_MUSIC_FAVOURITES    equ     1132    ;$FILENAME browse_music_favourites.src
RSRC_VIE_BROWSE_MUSIC_FAVOURITES    equ     1133    ;$FILENAME vie_browse_music_favourites.src

RSRC_BROWSE_MUSIC_FM_FILES    equ     1134    ;$FILENAME browse_music_fm_files.src
RSRC_VIE_BROWSE_MUSIC_FM_FILES    equ     1135    ;$FILENAME vie_browse_music_fm_files.src

RSRC_BROWSE_VOICE       equ     1136    ;$FILENAME browse_voice.src
RSRC_VIE_BROWSE_VOICE    equ     1137    ;$FILENAME vie_browse_voice.src

RSRC_FAVOURITES_LIST_ADD    equ     1138    ;$FILENAME favourites_list_add.src
RSRC_VIE_FAVOURITES_LIST_ADD    equ     1139    ;$FILENAME vie_favourites_list_add.src

RSRC_FAVOURITES_LIST_REMOVE    equ     1140    ;$FILENAME favourites_list_remove.src
RSRC_VIE_FAVOURITES_LIST_REMOVE    equ     1141    ;$FILENAME vie_favourites_list_remove.src

RSRC_FAVOURITES_LIST_IS_FULL    equ     1142    ;$FILENAME favourites_list_is_full.src
RSRC_VIE_FAVOURITES_LIST_IS_FULL    equ     1143    ;$FILENAME vie_favourites_list_is_full.src

RSRC_ABOUT_SCREEN_1     equ     1144    ;$FILENAME about_screen_1.src
RSRC_VIE_ABOUT_SCREEN_1    equ     1145    ;$FILENAME vie_about_screen_1.src

RSRC_ABOUT_SCREEN_2     equ     1146    ;$FILENAME about_screen_2.src
RSRC_VIE_ABOUT_SCREEN_2    equ     1147    ;$FILENAME vie_about_screen_2.src

RSRC_ABOUT_SCREEN_3     equ     1148    ;$FILENAME about_screen_3.src
RSRC_VIE_ABOUT_SCREEN_3    equ     1149    ;$FILENAME vie_about_screen_3.src

RSRC_ABOUT_SCREEN_4     equ     1150    ;$FILENAME about_screen_4.src
RSRC_VIE_ABOUT_SCREEN_4    equ     1151    ;$FILENAME vie_about_screen_4.src

RSRC_TIME_DATE_EXIT_TITLE    equ     1152    ;$FILENAME time_date_exit_title.src
RSRC_VIE_TIME_DATE_EXIT_TITLE    equ     1153    ;$FILENAME vie_time_date_exit_title.src

RSRC_TIME_CLEAN_DESKTOP    equ     1154    ;$FILENAME time_clean_desktop.src
RSRC_TIME_DASH          equ     1155    ;$FILENAME time_dash.src

RSRC_TIME_DAY_SAT       equ     1156    ;$FILENAME time_day_7.src
RSRC_VIE_TIME_DAY_SAT    equ     1157    ;$FILENAME vie_time_day_7.src
RSRC_TIME_DAY_SUN       equ     1158    ;$FILENAME time_day_cn.src
RSRC_VIE_TIME_DAY_SUN    equ     1159    ;$FILENAME vie_time_day_cn.src
RSRC_TIME_DAY_MON       equ     1160    ;$FILENAME time_day_2.src
RSRC_VIE_TIME_DAY_MON    equ     1161    ;$FILENAME vie_time_day_2.src
RSRC_TIME_DAY_TUE       equ     1162    ;$FILENAME time_day_3.src
RSRC_VIE_TIME_DAY_TUE    equ     1163    ;$FILENAME vie_time_day_3.src
RSRC_TIME_DAY_WED       equ     1164    ;$FILENAME time_day_4.src
RSRC_VIE_TIME_DAY_WED    equ     1165    ;$FILENAME vie_time_day_4.src
RSRC_TIME_DAY_THU       equ     1166    ;$FILENAME time_day_5.src
RSRC_VIE_TIME_DAY_THU    equ     1167    ;$FILENAME vie_time_day_5.src
RSRC_TIME_DAY_FRI       equ     1168    ;$FILENAME time_day_6.src
RSRC_VIE_TIME_DAY_FRI    equ     1169    ;$FILENAME vie_time_day_6.src

RSRC_TIME_MONTH_1       equ     1170    ;$FILENAME time_month_1.src
RSRC_VIE_TIME_MONTH_1    equ     1171    ;$FILENAME vie_time_month_1.src
RSRC_TIME_MONTH_2       equ     1172    ;$FILENAME time_month_2.src
RSRC_VIE_TIME_MONTH_2    equ     1173    ;$FILENAME vie_time_month_2.src
RSRC_TIME_MONTH_3       equ     1174    ;$FILENAME time_month_3.src
RSRC_VIE_TIME_MONTH_3    equ     1175    ;$FILENAME vie_time_month_3.src
RSRC_TIME_MONTH_4       equ     1176    ;$FILENAME time_month_4.src
RSRC_VIE_TIME_MONTH_4    equ     1177    ;$FILENAME vie_time_month_4.src
RSRC_TIME_MONTH_5       equ     1178    ;$FILENAME time_month_5.src
RSRC_VIE_TIME_MONTH_5    equ     1179    ;$FILENAME vie_time_month_5.src
RSRC_TIME_MONTH_6       equ     1180    ;$FILENAME time_month_6.src
RSRC_VIE_TIME_MONTH_6    equ     1181    ;$FILENAME vie_time_month_6.src
RSRC_TIME_MONTH_7       equ     1182    ;$FILENAME time_month_7.src
RSRC_VIE_TIME_MONTH_7    equ     1183    ;$FILENAME vie_time_month_7.src
RSRC_TIME_MONTH_8       equ     1184    ;$FILENAME time_month_8.src
RSRC_VIE_TIME_MONTH_8    equ     1185    ;$FILENAME vie_time_month_8.src
RSRC_TIME_MONTH_9       equ     1186    ;$FILENAME time_month_9.src
RSRC_VIE_TIME_MONTH_9    equ     1187    ;$FILENAME vie_time_month_9.src
RSRC_TIME_MONTH_10      equ     1188    ;$FILENAME time_month_10.src
RSRC_VIE_TIME_MONTH_10    equ     1189    ;$FILENAME vie_time_month_10.src
RSRC_TIME_MONTH_11      equ     1190    ;$FILENAME time_month_11.src
RSRC_VIE_TIME_MONTH_11    equ     1191    ;$FILENAME vie_time_month_11.src
RSRC_TIME_MONTH_12      equ     1192    ;$FILENAME time_month_12.src
RSRC_VIE_TIME_MONTH_12    equ     1193    ;$FILENAME vie_time_month_12.src

RSRC_TIME_NUM_AM        equ     1194    ;$FILENAME time_num_am.src
RSRC_TIME_NUM_PM        equ     1195    ;$FILENAME time_num_am.src
RSRC_SETTIME_FORMAT_12H    equ     1196    ;$FILENAME settime_format_12h.src
RSRC_SETTIME_FORMAT_24H    equ     1197    ;$FILENAME settime_format_24h.src
RSRC_SETDATE_FORMAT_DMY    equ     1198    ;$FILENAME setdate_format_dmy.src
RSRC_SETDATE_FORMAT_MDY    equ     1199    ;$FILENAME setdate_format_mdy.src
RSRC_SETDATE_FORMAT_YMD    equ     1200    ;$FILENAME setdate_format_ymd.src

RSRC_TIME_NUM_LARGE_0    equ     1201    ;$FILENAME time_num_large_0.src
RSRC_TIME_NUM_LARGE_1    equ     1202    ;$FILENAME time_num_large_1.src
RSRC_TIME_NUM_LARGE_2    equ     1203    ;$FILENAME time_num_large_2.src
RSRC_TIME_NUM_LARGE_3    equ     1204    ;$FILENAME time_num_large_3.src
RSRC_TIME_NUM_LARGE_4    equ     1205    ;$FILENAME time_num_large_4.src
RSRC_TIME_NUM_LARGE_5    equ     1206    ;$FILENAME time_num_large_5.src
RSRC_TIME_NUM_LARGE_6    equ     1207    ;$FILENAME time_num_large_6.src
RSRC_TIME_NUM_LARGE_7    equ     1208    ;$FILENAME time_num_large_7.src
RSRC_TIME_NUM_LARGE_8    equ     1209    ;$FILENAME time_num_large_8.src
RSRC_TIME_NUM_LARGE_9    equ     1210    ;$FILENAME time_num_large_9.src

RSRC_TIME_NUM_MEDIUM_0    equ     1211    ;$FILENAME time_num_medium_0.src
RSRC_TIME_NUM_MEDIUM_1    equ     1212    ;$FILENAME time_num_medium_1.src
RSRC_TIME_NUM_MEDIUM_2    equ     1213    ;$FILENAME time_num_medium_2.src
RSRC_TIME_NUM_MEDIUM_3    equ     1214    ;$FILENAME time_num_medium_3.src
RSRC_TIME_NUM_MEDIUM_4    equ     1215    ;$FILENAME time_num_medium_4.src
RSRC_TIME_NUM_MEDIUM_5    equ     1216    ;$FILENAME time_num_medium_5.src
RSRC_TIME_NUM_MEDIUM_6    equ     1217    ;$FILENAME time_num_medium_6.src
RSRC_TIME_NUM_MEDIUM_7    equ     1218    ;$FILENAME time_num_medium_7.src
RSRC_TIME_NUM_MEDIUM_8    equ     1219    ;$FILENAME time_num_medium_8.src
RSRC_TIME_NUM_MEDIUM_9    equ     1220    ;$FILENAME time_num_medium_9.src

RSRC_TIME_COLON         equ     1221    ;$FILENAME time_colon.src

RSRC_SETTINGS_BACKLIGHT_TITLE    equ     1222    ;$FILENAME settings_backlight_title.src
RSRC_SETTINGS_AUTO_SHUTDOWN_TITLE    equ     1223    ;$FILENAME settings_auto_shutdown_title.src
RSRC_SETTINGS_PLAYMODE_TITLE    equ     1224    ;$FILENAME settings_playmode_title.src

RSRC_SETTINGS_CONTRAST_TITLE    equ     1225    ;$FILENAME settings_contrast_title.src
RSRC_VIE_SETTINGS_CONTRAST_TITLE    equ     1226    ;$FILENAME vie_settings_contrast_title.src

RSRC_SETTINGS_EQ_TITLE    equ     1227    ;$FILENAME settings_eq_title.src
RSRC_VIE_SETTINGS_EQ_TITLE    equ     1228    ;$FILENAME vie_settings_eq_title.src

RSRC_SETTINGS_EXIT_TITLE    equ     1229    ;$FILENAME settings_exit_title.src
RSRC_VIE_SETTINGS_EXIT_TITLE    equ     1230    ;$FILENAME vie_settings_exit_title.src

RSRC_SETTINGS_SET_DATE_TITLE    equ     1231    ;$FILENAME settings_set_date_title.src
RSRC_VIE_SETTINGS_SET_DATE_TITLE    equ     1232    ;$FILENAME vie_settings_set_date_title.src

RSRC_SETTINGS_SET_TIME_TITLE    equ     1233    ;$FILENAME settings_set_time_title.src
RSRC_VIE_SETTINGS_SET_TIME_TITLE    equ     1234    ;$FILENAME vie_settings_set_time_title.src

RSRC_SETTINGS_PLAYMODE_NORMAL    equ     1235    ;$FILENAME settings_playmode_normal.src
RSRC_VIE_SETTINGS_PLAYMODE_NORMAL    equ     1236    ;$FILENAME vie_settings_playmode_normal.src

RSRC_SETTINGS_PLAYMODE_REPEATONE    equ     1237    ;$FILENAME settings_playmode_repeatone.src
RSRC_VIE_SETTINGS_PLAYMODE_REPEATONE    equ     1238    ;$FILENAME vie_settings_playmode_repeatone.src

RSRC_SETTINGS_PLAYMODE_REPEATALL    equ     1239    ;$FILENAME settings_playmode_repeatall.src
RSRC_VIE_SETTINGS_PLAYMODE_REPEATALL    equ     1240    ;$FILENAME vie_settings_playmode_repeatall.src

RSRC_SETTINGS_PLAYMODE_SHUFFLE    equ     1241    ;$FILENAME settings_playmode_shuffle.src
RSRC_VIE_SETTINGS_PLAYMODE_SHUFFLE    equ     1242    ;$FILENAME vie_settings_playmode_shuffle.src

RSRC_SETTINGS_PLAYMODE_SHUFFLE_REPEAT    equ     1243    ;$FILENAME settings_playmode_shuffle_repeat.src
RSRC_VIE_SETTINGS_PLAYMODE_SHUFFLE_REPEAT    equ     1244    ;$FILENAME vie_settings_playmode_shuffle_repeat.src

RSRC_SETTINGS_BACKLIGHT_ON    equ     1245    ;$FILENAME settings_backlight_on.src
RSRC_VIE_SETTINGS_BACKLIGHT_ON    equ     1246    ;$FILENAME vie_settings_backlight_on.src

RSRC_SETTINGS_BACKLIGHT_10S    equ     1247    ;$FILENAME settings_backlight_10s.src
RSRC_VIE_SETTINGS_BACKLIGHT_10S    equ     1248    ;$FILENAME vie_settings_backlight_10s.src

RSRC_SETTINGS_BACKLIGHT_20S    equ     1249    ;$FILENAME settings_backlight_20s.src
RSRC_VIE_SETTINGS_BACKLIGHT_20s    equ     1250    ;$FILENAME vie_settings_backlight_20s.src

RSRC_SETTINGS_BACKLIGHT_30S    equ     1251    ;$FILENAME settings_backlight_30s.src
RSRC_VIE_SETTINGS_BACKLIGHT_30S    equ     1252    ;$FILENAME vie_settings_backlight_30s.src

RSRC_SETTINGS_AUTO_SHUTDOWN_DISABLE    equ     1253    ;$FILENAME settings_auto_shutdown_disable.src
RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_DISABLE    equ     1254    ;$FILENAME vie_settings_auto_shutdown_disable.src

RSRC_SETTINGS_AUTO_SHUTDOWN_1MINUTE    equ     1255    ;$FILENAME settings_auto_shutdown_1minute.src
RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_1MINUTE    equ     1256    ;$FILENAME vie_settings_auto_shutdown_1min.src

RSRC_SETTINGS_AUTO_SHUTDOWN_2MINUTES    equ     1257    ;$FILENAME settings_auto_shutdown_2minutes.src
RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_2MINUTES    equ     1258    ;$FILENAME vie_settings_auto_shutdown_2min.src

RSRC_SETTINGS_AUTO_SHUTDOWN_5MINUTES    equ     1259    ;$FILENAME settings_auto_shutdown_5minutes.src
RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_5MINUTES    equ     1260    ;$FILENAME vie_settings_auto_shutdown_5min.src

RSRC_SETTINGS_AUTO_SHUTDOWN_10MINUTES    equ     1261    ;$FILENAME settings_auto_shutdown_10minutes.src
RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_10MINUTES    equ     1262    ;$FILENAME vie_settings_auto_shutdown_10min.src

RSRC_SETTINGS_LANGUAGES_ENG    equ     1263    ;$FILENAME settings_languages_eng.src
RSRC_SETTINGS_LANGUAGES_VIE    equ     1264    ;$FILENAME settings_languages_vie.src

RSRC_FRACTION_DOT       equ     1265    ;$FILENAME fraction_dot.src

RSRC_FM_BACKGROUND      equ     1266    ;$FILENAME fm_background.src
RSRC_VIE_FM_BACKGROUND    equ     1267    ;$FILENAME vie_fm_background.src

RSRC_SEARCHING_PLEASE_WAIT    equ     1268    ;$FILENAME searching_please_wait.src
RSRC_VIE_SEARCHING_PLEASE_WAIT    equ     1269    ;$FILENAME vie_searching_please_wait.src

RSRC_FM_AUTO_SEARCH     equ     1270    ;$FILENAME fm_auto_search.src
RSRC_VIE_FM_AUTO_SEARCH    equ     1271    ;$FILENAME vie_fm_auto_search.src

RSRC_JVJ_SHUTDOWN_PLAYER    equ     1272    ;$FILENAME jvj_shutdown_player.src
RSRC_VIE_JVJ_SHUTDOWN_PLAYER    equ     1273    ;$FILENAME vie_jvj_shutdown_player.src

  ENDIF ;IF (!@def(resources))
