//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sat Jun 28 16:37:44 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sat Jun 28 16:13:49 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sat Jun 28 15:55:53 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Thu Jun 26 08:58:05 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 25 17:00:35 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 25 11:42:38 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 25 11:17:50 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 25 11:17:05 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Tue Jun 24 15:21:11 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sun Jun 22 02:54:01 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sun Jun 22 00:47:39 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Fri Jun 20 16:15:57 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Thu Jun 19 09:47:30 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 23:11:16 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 22:57:19 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 22:49:16 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 11:53:38 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 10:45:13 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Wed Jun 18 09:54:56 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Tue Jun 17 16:47:21 2008 using resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Tue Jun 17 11:30:49 2008 using resource.inc as input.



/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2001-2007
// Reviews: DanhNguyen (06-2008) for X8iTF/STFM1000
// LCD example resource listing
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(resources))
#define resources 1

/////////////////////////////////////////////////////////////////////////////////
//  Player version number  no leading zeros in version number!!!!!
/////////////////////////////////////////////////////////////////////////////////

#define VERSION_MAJOR 3
#define VERSION_MIDDLE 200
#define VERSION_MINOR 910

#define LCD_SEG_OFFSET 0x000000
#define NUMBER_OF_PRESETS 10


/////////////////////////////////////////////////////////////////////////////////
//  High usage resource. These have been moved here to take maximum advantage of
//  the resource index cache if it was added.
//  This block is sorted by frequency of use while loading a new song.
/////////////////////////////////////////////////////////////////////////////////

//$FILENAME searchdirectory.src
#define RSRC_FUNCLET_SEARCHDIRECTORY 1    
//$FILENAME shortdirmatch.src
#define RSRC_FUNCLET_SHORTDIRMATCH 2    
//$FILENAME fopen.src
#define RSRC_FUNCLET_FOPEN 3    
//$FILENAME musicmenu.src
#define RSRC_MUSIC_MENU_CODE_BANK 4    
//$FILENAME changepath.src
#define RSRC_FUNCLET_CHANGEPATH 5    
//$FILENAME _openandverifyslot.src
#define RSRC_FUNCLET__OPENANDVERIFYSLOT 6    
//$FILENAME _loadslot.src
#define RSRC_FUNCLET__LOADSLOT 7    
//$FILENAME getname.src
#define RSRC_FUNCLET_GETNAME 8    
//$FILENAME Funclet_SteppingVoltageSet.src
#define RSRC_FUNCLET_STEPPINGVOLTAGESET 9    
//$FILENAME sethandleforsearch.src
#define RSRC_FUNCLET_SETHANDLEFORSEARCH 10    
//$FILENAME wmaWrap.src
#define RSRC_WMADEC_CODE 11    
//$FILENAME extractfilename.src
#define RSRC_FUNCLET_EXTRACTFILENAME 12    
//$FILENAME oem_getgroupcertprivatekey.src
#define RSRC_FUNCLET_OEM_GETGROUPCERTPRIVATEKEY 13    
//$FILENAME SoftTimerMod.src
#define RSRC_SOFT_TIMER_MODULE_CODE 14    
//$FILENAME GetShortfilename.src
#define RSRC_FUNCLET_GETSHORTFILENAME 15    
//$FILENAME drm_dcp_loadpropertiescache.src
#define RSRC_FUNCLET_DRM_DCP_LOADPROPERTIESCACHE 16    
//$FILENAME playerstatemachine.src
#define RSRC_PLAY_STATE_MACHINE_CODE_BANK 17    
//$FILENAME SysMod.src
#define RSRC_SYSMOD_CODE 18    
//$FILENAME drm_b64_decodew.src
#define RSRC_FUNCLET_DRM_B64_DECODEW 19    
//$FILENAME discardtrailigperiods.src
#define RSRC_FUNCLET_DISCARDTRAILIGPERIODS 20    
//$FILENAME uppercase.src
#define RSRC_FUNCLET_UPPERCASE 21    
//$FILENAME strlength.src
#define RSRC_FUNCLET_STRLENGTH 22    
//$FILENAME ConverToShortname.src
#define RSRC_FUNCLET_CONVERTOSHORTNAME 23    
//$FILENAME drm_bbx_hashvalue.src
#define RSRC_FUNCLET_DRM_BBX_HASHVALUE 24    
//$FILENAME drm_expr_evaluateexpression.src
#define RSRC_FUNCLET_DRM_EXPR_EVALUATEEXPRESSION 25    
//$FILENAME drm_sst_closekey.src
#define RSRC_FUNCLET_DRM_SST_CLOSEKEY 26    
//$FILENAME Funclet_SysSetSpeed.src
#define RSRC_FUNCLET_SYSSETSPEED 27    
//$FILENAME freehandle.src
#define RSRC_FUNCLET_FREEHANDLE 28    
//$FILENAME searchfreehandleallocate.src
#define RSRC_FUNCLET_SEARCHFREEHANDLEALLOCATE 29    
//$FILENAME _parselicenseattributes.src
#define RSRC_FUNCLET__PARSELICENSEATTRIBUTES 30    
//$FILENAME variablesecstategetorset.src
#define RSRC_FUNCLET_VARIABLESECSTATEGETORSET 31    
//$FILENAME _setuplicevalobjecttoshare.src
#define RSRC_FUNCLET__SETUPLICEVALOBJECTTOSHARE 32    
//$FILENAME drm_mgr_initialize.src
#define RSRC_FUNCLET_DRM_MGR_INITIALIZE 33    
//$FILENAME display.src
#define RSRC_DISPLAY_CODE_BANK 34    
//$FILENAME DisplayModule.src
#define RSRC_DISPLAY_MODULE 35    
//$FILENAME extractpath.src
#define RSRC_FUNCLET_EXTRACTPATH 36    
//$FILENAME drm_sst_createglobalstorepassword.src
#define RSRC_FUNCLET_DRM_SST_CREATEGLOBALSTOREPASSWORD 37    
//$FILENAME _getprivatekey.src
#define RSRC_FUNCLET__GETPRIVATEKEY 38    
//$FILENAME drm_hds_opennamespace.src
#define RSRC_FUNCLET_DRM_HDS_OPENNAMESPACE 39    
//$FILENAME drm_hds_openslot.src
#define RSRC_FUNCLET_DRM_HDS_OPENSLOT 40    
//$FILENAME fclose.src
#define RSRC_FUNCLET_FCLOSE 41    
//$FILENAME drm_cphr_init.src
#define RSRC_FUNCLET_DRM_CPHR_INIT 42    
//$FILENAME drm_pk_symmetriccrypt.src
#define RSRC_FUNCLET_DRM_PK_SYMMETRICCRYPT 43    
//$FILENAME drm_mgr_bind.src
#define RSRC_FUNCLET_DRM_MGR_BIND 44    
//$FILENAME _decryptcontentkey.src
#define RSRC_FUNCLET__DECRYPTCONTENTKEY 45    
//$FILENAME drm_mac_inv32.src
#define RSRC_FUNCLET_DRM_MAC_INV32 46    
//$FILENAME drm_lic_getattribute.src
#define RSRC_FUNCLET_DRM_LIC_GETATTRIBUTE 47    
//$FILENAME drm_utl_getversionasstring.src
#define RSRC_FUNCLET_DRM_UTL_GETVERSIONASSTRING 48    
//$FILENAME drm_dcp_getattribute.src
#define RSRC_FUNCLET_DRM_DCP_GETATTRIBUTE 49    
//$FILENAME effectsmodules.src
#define RSRC_EFFECTS_MODULES_P 50    
//$FILENAME janusx.src
#define RSRC_JANUSX_CODE 51    
//$FILENAME drm_sst_openkeytokens.src
#define RSRC_FUNCLET_DRM_SST_OPENKEYTOKENS 52    
//$FILENAME eval.src
#define RSRC_FUNCLET_EVAL 53    
//$FILENAME _verifyslothash.src
#define RSRC_FUNCLET__VERIFYSLOTHASH 54    
//$FILENAME januscommon.src
#define RSRC_JANUS_COMMON 55    
//$FILENAME changecase.src
#define RSRC_FUNCLET_CHANGECASE 56    
//$FILENAME drm_bbx_symmetricverify.src
#define RSRC_FUNCLET_DRM_BBX_SYMMETRICVERIFY 57    
//$FILENAME _loadlicenseattributes.src
#define RSRC_FUNCLET__LOADLICENSEATTRIBUTES 58    
//$FILENAME drm_hds_slotseek.src
#define RSRC_FUNCLET_DRM_HDS_SLOTSEEK 59    
//$FILENAME drm_hds_slotwrite.src
#define RSRC_FUNCLET_DRM_HDS_SLOTWRITE 60    
//$FILENAME drm_levl_performoperations.src
#define RSRC_FUNCLET_DRM_LEVL_PERFORMOPERATIONS 61    
//$FILENAME drm_lic_verifysignature.src
#define RSRC_FUNCLET_DRM_LIC_VERIFYSIGNATURE 62    
//$FILENAME drm_lst_getlicense.src
#define RSRC_FUNCLET_DRM_LST_GETLICENSE 63    
//$FILENAME drm_utl_numbertostring.src
#define RSRC_FUNCLET_DRM_UTL_NUMBERTOSTRING 64    
//$FILENAME oem_writefile.src
#define RSRC_FUNCLET_OEM_WRITEFILE 65    
//$FILENAME drm_sst_getdata.src
#define RSRC_FUNCLET_DRM_SST_GETDATA 66    
//$FILENAME updatehandlemode.src
#define RSRC_FUNCLET_UPDATEHANDLEMODE 67    
//$FILENAME _getlicenseinfoandsetup.src
#define RSRC_FUNCLET__GETLICENSEINFOANDSETUP 68    
//$FILENAME drm_lic_completelicensechain.src
#define RSRC_FUNCLET_DRM_LIC_COMPLETELICENSECHAIN 69    
//$FILENAME doplay_p.src
#define RSRC_DOPLAY_P 70    
//$FILENAME fatwritep.src
#define RSRC_FATWRITE_P_CODE 71    
//$FILENAME findfirst.src
#define RSRC_FUNCLET_FINDFIRST 72    
//$FILENAME Funclet_SysLoadFatWrite.src
#define RSRC_FUNCLET_SYSLOADFATWRITE 73    
//$FILENAME changetorootdirectory.src
#define RSRC_FUNCLET_CHANGETOROOTDIRECTORY 74    
//$FILENAME _findkeypair.src
#define RSRC_FUNCLET__FINDKEYPAIR 75    
//$FILENAME variablemachinegetorset.src
#define RSRC_FUNCLET_VARIABLEMACHINEGETORSET 76    
//$FILENAME _hdsslotenumnext.src
#define RSRC_FUNCLET__HDSSLOTENUMNEXT 77    
//$FILENAME getlspubkey.src
#define RSRC_FUNCLET_GETLSPUBKEY 78    
//$FILENAME drm_sst_gettokenvalue.src
#define RSRC_FUNCLET_DRM_SST_GETTOKENVALUE 79    
//$FILENAME drm_utl_decodekid.src
#define RSRC_FUNCLET_DRM_UTL_DECODEKID 80    
//$FILENAME drm_xml_getnodecdata.src
#define RSRC_FUNCLET_DRM_XML_GETNODECDATA 81    
//$FILENAME _freedrmmanagerinternalcontexts.src
#define RSRC_FUNCLET__FREEDRMMANAGERINTERNALCONTEXTS 82    
//$FILENAME drm_clk_initchecksecureclock.src
#define RSRC_FUNCLET_DRM_CLK_INITCHECKSECURECLOCK 83    
//$FILENAME drm_sst_openandlockslot.src
#define RSRC_FUNCLET_DRM_SST_OPENANDLOCKSLOT 84    
//$FILENAME aes_enc.src
#define RSRC_AES_ENC 85    
//$FILENAME getprivkey.src
#define RSRC_FUNCLET_GETPRIVKEY 86    
//$FILENAME Funclet_GetUnicodeCharacterBitmap.src
#define RSRC_FUNCLET_GET_UNICODE_CHARACTER_BITMAP 87    
//$FILENAME playlist_codebank.src
#define RSRC_PLAYLIST_CODEBANK 88    
//$FILENAME drm_lic_getenablingbits.src
#define RSRC_FUNCLET_DRM_LIC_GETENABLINGBITS 89    
//$FILENAME drm_mgr_uninitialize.src
#define RSRC_FUNCLET_DRM_MGR_UNINITIALIZE 90    
//$FILENAME _getdevicecert.src
#define RSRC_FUNCLET__GETDEVICECERT 91    
//$FILENAME drm_lic_reportactions.src
#define RSRC_FUNCLET_DRM_LIC_REPORTACTIONS 92    
//$FILENAME drmcrt_wcsntol.src
#define RSRC_FUNCLET_DRMCRT_WCSNTOL 93    
//$FILENAME _basicheaderchecks.src
#define RSRC_FUNCLET__BASICHEADERCHECKS 94    
//$FILENAME drm_hdr_getattribute.src
#define RSRC_FUNCLET_DRM_HDR_GETATTRIBUTE 95    
//$FILENAME drm_hds_slotenumnext.src
#define RSRC_FUNCLET_DRM_HDS_SLOTENUMNEXT 96    
//$FILENAME drm_levl_evaluateexpression.src
#define RSRC_FUNCLET_DRM_LEVL_EVALUATEEXPRESSION 97    
//$FILENAME drm_lst_open.src
#define RSRC_FUNCLET_DRM_LST_OPEN 98    
//$FILENAME drm_xml_getnodeattribute.src
#define RSRC_FUNCLET_DRM_XML_GETNODEATTRIBUTE 99    
//$FILENAME _verifysymmerticsignature.src
#define RSRC_FUNCLET__VERIFYSYMMERTICSIGNATURE 100    
//$FILENAME oem_openfile.src
#define RSRC_FUNCLET_OEM_OPENFILE 101    
//$FILENAME _getdrmfullpathname.src
#define RSRC_FUNCLET__GETDRMFULLPATHNAME 102    
//$FILENAME oem_getsecurestoreglobalpasswordseed.src
#define RSRC_FUNCLET_OEM_GETSECURESTOREGLOBALPASSWORDSEED 103    
//$FILENAME _applydiffstostore.src
#define RSRC_FUNCLET__APPLYDIFFSTOSTORE 104    
//$FILENAME drm_sst_setdata.src
#define RSRC_FUNCLET_DRM_SST_SETDATA 105    
//$FILENAME drm_sst_getlockeddata.src
#define RSRC_FUNCLET_DRM_SST_GETLOCKEDDATA 106    
//$FILENAME drm_sst_setlockeddata.src
#define RSRC_FUNCLET_DRM_SST_SETLOCKEDDATA 107    
//$FILENAME playerlib_extra.src
#define RSRC_PLAYERLIB_EXTRA_CODE_BANK 108    
//$FILENAME wmaCommon.src
#define RSRC_WMA_COMMON 109    
//$FILENAME wmainit.src
#define RSRC_WMA_INIT 110    
//$FILENAME playlist2traverse_codebank.src
#define RSRC_PLAYLIST2TRAVERSE_CODEBANK 111    
//$FILENAME drm_bbx_cipherkeysetup.src
#define RSRC_FUNCLET_DRM_BBX_CIPHERKEYSETUP 112    
//$FILENAME drm_lic_checkclockrollback.src
#define RSRC_FUNCLET_DRM_LIC_CHECKCLOCKROLLBACK 113    
//$FILENAME drm_hds_closestore.src
#define RSRC_FUNCLET_DRM_HDS_CLOSESTORE 114    
//$FILENAME _hdsloadsrn.src
#define RSRC_FUNCLET__HDSLOADSRN 115    
//$FILENAME _loadproritizedlist.src
#define RSRC_FUNCLET__LOADPRORITIZEDLIST 116    
//$FILENAME drm_lst_initenum.src
#define RSRC_FUNCLET_DRM_LST_INITENUM 117    
//$FILENAME _loadattributesintocache.src
#define RSRC_FUNCLET__LOADATTRIBUTESINTOCACHE 118    
//$FILENAME drm_pk_symmetricverify.src
#define RSRC_FUNCLET_DRM_PK_SYMMETRICVERIFY 119    

/////////////////////////////////////////////////////////////////////////////////
//  Menu Modules (codebanks)
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME mainmenu.src
#define RSRC_MAIN_MENU_CODE_BANK 120    
//$FILENAME displaylists.src
#define RSRC_DISPLAY_LISTS_CODE_BANK 121    

//$FILENAME voicemenu.src
#define RSRC_VOICE_MENU_CODE_BANK 122    
//$FILENAME fmtunermenu.src
#define RSRC_FMTUNER_MENU_CODE_BANK 123    
//$FILENAME recorderstatemachine.src
#define RSRC_RECORDER_STATE_MACHINE_CODE_BANK 124    

//$FILENAME eqmenu.src
#define RSRC_EQ_MENU_CODE_BANK 125    
//$FILENAME playmodemenu.src
#define RSRC_PLAYMODE_MENU_CODE_BANK 126    
//$FILENAME contrastmenu.src
#define RSRC_CONTRAST_MENU_CODE_BANK 127    
//$FILENAME pwrsettingsmenu.src
#define RSRC_PWRSETTINGS_MENU_CODE_BANK 128    
//$FILENAME timedatemenu.src
#define RSRC_TIMEDATE_MENU_CODE_BANK 129    
//$FILENAME settimemenu.src
#define RSRC_SETTIME_MENU_CODE_BANK 130    
//$FILENAME setdatemenu.src
#define RSRC_SETDATE_MENU_CODE_BANK 131    
//$FILENAME settingsmenu.src
#define RSRC_SETTINGS_MENU_CODE_BANK 132    
//$FILENAME string_system_menu.src
#define RSRC_SYSTEM_MENU_CODE_BANK 133    
//$FILENAME deletemenu.src
#define RSRC_DELETE_MENU_CODE_BANK 134    
//$FILENAME aboutmenu.src
#define RSRC_ABOUT_MENU_CODE_BANK 135    

//$FILENAME spectrogram.src
#define RSRC_SPECTROGRAM_MENU_CODE_BANK 136    

//$FILENAME motionvideomenu.src
#define RSRC_MOTION_VIDEO_MENU_CODE_BANK 137    
//$FILENAME motionvideomenuinitstate.src
#define RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK 138    
//$FILENAME jpegdisplaymenu.src
#define RSRC_JPEG_DISPLAY_MENU_CODE_BANK 139    
//$FILENAME jpegmanualmenu.src
#define RSRC_JPEG_MANUAL_MENU_CODE_BANK 140    
//$FILENAME jpegthumbnailmenu.src
#define RSRC_JPEG_THUMBNAIL_MENU_CODE_BANK 141    
//$FILENAME jpegslideshowmenu.src
#define RSRC_JPEG_SLIDESHOW_MENU_CODE_BANK 142    
//$FILENAME albumartmenu.src
#define RSRC_ALBUM_ART_MENU_CODE_BANK 143    
//$FILENAME jpegfileutilextra.src
#define RSRC_JPEG_FILEUTIL_EXTRA_MENU_CODE_BANK 144    

////////////////////////////////////////////////////////////////////////////////////////////////////
// General Modules
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME MixMod.src
#define RSRC_MIXMOD_CODE 145    
//$FILENAME TunerModule.src
#define RSRC_TUNER_MODULE 146    
//$FILENAME geqoverlay.src
#define RSRC_GEQOVERLAY_CODE 147    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Decoders/Encoders
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME DecMod.src
#define RSRC_DECMOD_CODE 148    
//$FILENAME mp3p.src
#define RSRC_MP3P_CODE 149    
//$FILENAME mp3x.src
#define RSRC_MP3X_CODE 150    
//$FILENAME mp3y.src
#define RSRC_MP3Y_CODE 151    
//$FILENAME janusp.src
#define RSRC_JANUSP_CODE 152    
//RSRC_JANUSY_CODE        equ       29    ;$FILENAME janusy.src

//$FILENAME decadpcmimamod.src
#define RSRC_DEC_ADPCM_MOD_CODE 153    
//$FILENAME dec_adpcmp.src
#define RSRC_DEC_ADPCMP_CODE 154    
//$FILENAME dec_adpcmx.src
#define RSRC_DEC_ADPCMX_CODE 155    
//$FILENAME dec_adpcmy.src
#define RSRC_DEC_ADPCMY_CODE 156    

//$FILENAME decadpcmsmvmod.src
#define RSRC_DEC_SMVADPCM_MOD_CODE 157    
//$FILENAME dec_smvadpcmp.src
#define RSRC_DEC_SMVADPCMP_CODE 158    
//$FILENAME dec_smvadpcmx.src
#define RSRC_DEC_SMVADPCMX_CODE 159    
//$FILENAME dec_smvadpcmy.src
#define RSRC_DEC_SMVADPCMY_CODE 160    

//$FILENAME encadpcmimamod.src
#define RSRC_ENC_ADPCM_MOD_CODE 161    
//$FILENAME enc_adpcmp.src
#define RSRC_ENC_ADPCMP_CODE 162    
//$FILENAME enc_adpcmx.src
#define RSRC_ENC_ADPCMX_CODE 163    
//$FILENAME enc_adpcmy.src
#define RSRC_ENC_ADPCMY_CODE 164    

//$FILENAME jpeg_p.src
#define RSRC_JPEG_DECODER_P 165    
//$FILENAME jpeg_x.src
#define RSRC_JPEG_DECODER_X 166    
//$FILENAME jpeg_y.src
#define RSRC_JPEG_DECODER_Y 167    
//$FILENAME jpeg2_y.src
#define RSRC_JPEG_DECODER_Y_2 168    
//$FILENAME bmp2_y.src
#define RSRC_BMP_DECODER_Y_2 169    
//$FILENAME bmp_p.src
#define RSRC_BMP_DECODER_P 170    

//RSRC_SMVJPEG_DECODER_P    equ       57    ;$FILENAME smvjpeg_p.src
//$FILENAME smvjpeg_x.src
#define RSRC_SMVJPEG_DECODER_X 171    
//$FILENAME smvjpeg_y.src
#define RSRC_SMVJPEG_DECODER_Y 172    


////////////////////////////////////////////////////////////////////////////////////////////////////
// System Settings
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME settings.src
#define RSRC_SETTINGS_COMMANDS 173    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Media Device Drivers
////////////////////////////////////////////////////////////////////////////////////////////////////
//This resource is filled with garbage unless it is the MMC build
//$FILENAME null.src
#define RSRC_MMCDD_CODE 174    
//$FILENAME null.src
#define RSRC_SMDD_HIGH_CODE 175    

/////////////////////////////////////////////////////////////////////////////////
//  PlayState resources
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME play_icon_with_border.src
#define RSRC_PLAY_ICON_WITH_BORDER 176    
//$FILENAME pause_icon_with_border.src
#define RSRC_PAUSE_ICON_WITH_BORDER 177    
//$FILENAME stop_icon_with_border.src
#define RSRC_STOP_ICON_WITH_BORDER 178    
//$FILENAME record_icon_with_border.src
#define RSRC_RECORD_ICON_WITH_BORDER 179    
//$FILENAME paused_record_icon_with_border.src
#define RSRC_PAUSED_RECORD_ICON_BORDER 180    
//$FILENAME ffwd_icon_with_border.src
#define RSRC_FFWD_ICON_WITH_BORDER 181    
//$FILENAME rwnd_icon_with_border.src
#define RSRC_RWND_ICON_WITH_BORDER 182    

/////////////////////////////////////////////////////////////////////////////////
//  PlayMode resources
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME repeatall_icon.src
#define RSRC_REPEAT_ALL_ICON 183    
//$FILENAME repeatsong_icon.src
#define RSRC_REPEAT_SONG_ICON 184    
//$FILENAME shuffle_icon.src
#define RSRC_SHUFFLE_ICON 185    
//$FILENAME random_icon.src
#define RSRC_RANDOM_ICON 186    
//$FILENAME repeatallclear_icon.src
#define RSRC_REPEAT_ALL_CLEAR_ICON 187    
//$FILENAME repeatsongclear_icon.src
#define RSRC_REPEAT_SONG_CLEAR_ICON 188    
//$FILENAME shuffleclear_icon.src
#define RSRC_SHUFFLE_CLEAR_ICON 189    

/////////////////////////////////////////////////////////////////////////////////
//  Battery Status
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME battery_00.src
#define RSRC_BATTERY_00 190    
//$FILENAME battery_01.src
#define RSRC_BATTERY_01 191    
//$FILENAME battery_02.src
#define RSRC_BATTERY_02 192    
//$FILENAME battery_03.src
#define RSRC_BATTERY_03 193    
//$FILENAME battery_04.src
#define RSRC_BATTERY_04 194    
//$FILENAME battery_05.src
#define RSRC_BATTERY_05 195    
//$FILENAME battery_06.src
#define RSRC_BATTERY_06 196    
//$FILENAME battery_07.src
#define RSRC_BATTERY_07 197    
//$FILENAME battery_08.src
#define RSRC_BATTERY_08 198    
//$FILENAME battery_09.src
#define RSRC_BATTERY_09 199    
//$FILENAME battery_10.src
#define RSRC_BATTERY_10 200    

/////////////////////////////////////////////////////////////////////////////////
//  System Icons
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME disk_small.src
#define RSRC_DISK_ICON 201    
//$FILENAME lock_small.src
#define RSRC_LOCK_ICON 202    
//$FILENAME icon_music_mode.src
#define RSRC_MUSIC_MODE_ICON 203    
//$FILENAME icon_voice_mode.src
#define RSRC_VOICE_MODE_ICON 204    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Volume Bitmaps
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME icon_vol_00.src
#define RSRC_ICON_VOL_00 205    
//$FILENAME icon_vol_01.src
#define RSRC_ICON_VOL_01 206    
//$FILENAME icon_vol_02.src
#define RSRC_ICON_VOL_02 207    
//$FILENAME icon_vol_03.src
#define RSRC_ICON_VOL_03 208    
//$FILENAME icon_vol_04.src
#define RSRC_ICON_VOL_04 209    
//$FILENAME icon_vol_05.src
#define RSRC_ICON_VOL_05 210    
//$FILENAME icon_vol_06.src
#define RSRC_ICON_VOL_06 211    
//$FILENAME icon_vol_07.src
#define RSRC_ICON_VOL_07 212    
//$FILENAME icon_vol_08.src
#define RSRC_ICON_VOL_08 213    
//$FILENAME icon_vol_09.src
#define RSRC_ICON_VOL_09 214    
//$FILENAME icon_vol_10.src
#define RSRC_ICON_VOL_10 215    
//$FILENAME icon_vol_11.src
#define RSRC_ICON_VOL_11 216    
//$FILENAME icon_vol_12.src
#define RSRC_ICON_VOL_12 217    
//$FILENAME icon_vol_13.src
#define RSRC_ICON_VOL_13 218    
//$FILENAME icon_vol_14.src
#define RSRC_ICON_VOL_14 219    
//$FILENAME icon_vol_15.src
#define RSRC_ICON_VOL_15 220    
//$FILENAME icon_vol_16.src
#define RSRC_ICON_VOL_16 221    
//$FILENAME icon_vol_17.src
#define RSRC_ICON_VOL_17 222    
//$FILENAME icon_vol_18.src
#define RSRC_ICON_VOL_18 223    
//$FILENAME icon_vol_19.src
#define RSRC_ICON_VOL_19 224    
//$FILENAME icon_vol_20.src
#define RSRC_ICON_VOL_20 225    
//$FILENAME icon_vol_21.src
#define RSRC_ICON_VOL_21 226    
//$FILENAME icon_vol_22.src
#define RSRC_ICON_VOL_22 227    
//$FILENAME icon_vol_23.src
#define RSRC_ICON_VOL_23 228    
//$FILENAME icon_vol_24.src
#define RSRC_ICON_VOL_24 229    
//$FILENAME icon_vol_25.src
#define RSRC_ICON_VOL_25 230    
//$FILENAME icon_vol_26.src
#define RSRC_ICON_VOL_26 231    
//$FILENAME icon_vol_27.src
#define RSRC_ICON_VOL_27 232    
//$FILENAME icon_vol_28.src
#define RSRC_ICON_VOL_28 233    
//$FILENAME icon_vol_29.src
#define RSRC_ICON_VOL_29 234    
//$FILENAME icon_vol_30.src
#define RSRC_ICON_VOL_30 235    
//$FILENAME icon_vol_31.src
#define RSRC_ICON_VOL_31 236    

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Splash Screen Stuff
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME st_bw1.src
#define RSRC_SPLASH_SCREEN 237    
//$FILENAME siglogo1.src
#define RSRC_SPLASH_SCREEN_1 238    
//$FILENAME siglogo2.src
#define RSRC_SPLASH_SCREEN_2 239    
//$FILENAME siglogo3.src
#define RSRC_SPLASH_SCREEN_3 240    
//$FILENAME siglogo4.src
#define RSRC_SPLASH_SCREEN_4 241    
//$FILENAME siglogo5.src
#define RSRC_SPLASH_SCREEN_5 242    
//$FILENAME siglogo6.src
#define RSRC_SPLASH_SCREEN_6 243    
//$FILENAME siglogo7.src
#define RSRC_SPLASH_SCREEN_7 244    
//$FILENAME siglogo8.src
#define RSRC_SPLASH_SCREEN_8 245    
//$FILENAME siglogo9.src
#define RSRC_SPLASH_SCREEN_9 246    
//$FILENAME siglogo10.src
#define RSRC_SPLASH_SCREEN_10 247    
//$FILENAME siglogo11.src
#define RSRC_SPLASH_SCREEN_11 248    
//$FILENAME siglogo12.src
#define RSRC_SPLASH_SCREEN_12 249    
//$FILENAME siglogo13.src
#define RSRC_SPLASH_SCREEN_13 250    
//$FILENAME siglogo.src
#define RSRC_SPLASH_SCREEN_ALL 251    

//$FILENAME locked.src
#define RSRC_LOCKED_SCREEN 252    

/////////////////////////////////////////////////////////////////////////////////
//  Shutdown
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME string_arial_8_continue_holding_to_power_off.src
#define RSRC_PDOWN_HOLD_STRING 253    
//$FILENAME status_16_6_steps_0.src
#define RSRC_PDOWN_STATUS_0 254    
//$FILENAME status_16_6_steps_1.src
#define RSRC_PDOWN_STATUS_1 255    
//$FILENAME status_16_6_steps_2.src
#define RSRC_PDOWN_STATUS_2 256    
//$FILENAME status_16_6_steps_3.src
#define RSRC_PDOWN_STATUS_3 257    
//$FILENAME status_16_6_steps_4.src
#define RSRC_PDOWN_STATUS_4 258    
//$FILENAME status_16_6_steps_5.src
#define RSRC_PDOWN_STATUS_5 259    
//$FILENAME status_16_6_steps_6.src
#define RSRC_PDOWN_STATUS_6 260    

/////////////////////////////////////////////////////////////////////////////////////////////////////
// EQ
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME eq_clear_icon.src
#define RSRC_EQ_CLEAR_ICON 261    
//$FILENAME rock_icon.src
#define RSRC_ROCK_ICON 262    
//$FILENAME jazz_icon.src
#define RSRC_JAZZ_ICON 263    
//$FILENAME classic_icon.src
#define RSRC_CLASSIC_ICON 264    
//$FILENAME pop_icon.src
#define RSRC_POP_ICON 265    
//$FILENAME custom_icon.src
#define RSRC_CUSTOM_ICON 266    

/////////////////////////////////////////////////////////////////////////////////////////////////////
// AB
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME ab_mark_a.src
#define RSRC_AB_MARK_A_ICON 267    
//$FILENAME ab_mark_b.src
#define RSRC_AB_MARK_B_ICON 268    

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu Display Resources
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME string_music_menu.src
#define RSRC_STRING_MUSIC_MENU 269    
//$FILENAME string_mvideo_menu.src
#define RSRC_STRING_MVIDEO_MENU 270    
//$FILENAME string_jpeg_display_menu.src
#define RSRC_STRING_JPEG_DISPLAY_MENU 271    
//$FILENAME string_jpeg_manual_menu.src
#define RSRC_STRING_JPEG_MANUAL_MENU 272    
//$FILENAME string_jpeg_slideshow_menu.src
#define RSRC_STRING_JPEG_SLIDESHOW_MENU 273    
//$FILENAME string_jpeg_thumbnail_menu.src
#define RSRC_STRING_JPEG_THUMBNAIL_MENU 274    
//$FILENAME string_voice_menu.src
#define RSRC_STRING_VOICE_MENU 275    
//$FILENAME string_audible_menu.src
#define RSRC_STRING_AUDIBLE_MENU 276    
//$FILENAME string_fmtuner_menu.src
#define RSRC_STRING_FMTUNER_MENU 277    
//$FILENAME string_settings_menu.src
#define RSRC_STRING_SETTINGS_MENU 278    
//$FILENAME string_eq_menu.src
#define RSRC_STRING_EQ_MENU 279    
//$FILENAME string_playmode_menu.src
#define RSRC_STRING_PLAYMODE_MENU 280    
//$FILENAME string_contrast_menu.src
#define RSRC_STRING_CONTRAST_MENU 281    
//$FILENAME string_pwrsavings_menu.src
#define RSRC_STRING_PWRSAVINGS_MENU 282    
//$FILENAME string_time_date_menu.src
#define RSRC_STRING_TIME_DATE_MENU 283    
//$FILENAME string_set_time_menu.src
#define RSRC_STRING_SET_TIME_MENU 284    
//$FILENAME string_set_date_menu.src
#define RSRC_STRING_SET_DATE_MENU 285    
//$FILENAME string_exit_menu.src
#define RSRC_STRING_EXIT_MENU 286    
//$FILENAME string_rock_menu.src
#define RSRC_STRING_ROCK_MENU 287    
//$FILENAME string_pop_menu.src
#define RSRC_STRING_POP_MENU 288    
//$FILENAME string_classic_menu.src
#define RSRC_STRING_CLASSIC_MENU 289    
//$FILENAME string_normal_menu.src
#define RSRC_STRING_NORMAL_MENU 290    
//$FILENAME string_jazz_menu.src
#define RSRC_STRING_JAZZ_MENU 291    
//$FILENAME string_repeat1_menu.src
#define RSRC_STRING_REPEAT1_MENU 292    
//$FILENAME string_repeatall_menu.src
#define RSRC_STRING_REPEATALL_MENU 293    
//$FILENAME string_shuffle_menu.src
#define RSRC_STRING_SHUFFLE_MENU 294    
//$FILENAME string_repeatshuffle_menu.src
#define RSRC_STRING_SHUFFLEREPEAT_MENU 295    
//$FILENAME string_disable_menu.src
#define RSRC_STRING_DISABLE_MENU 296    
//$FILENAME string_1min_menu.src
#define RSRC_STRING_1MIN_MENU 297    
//$FILENAME string_2min_menu.src
#define RSRC_STRING_2MIN_MENU 298    
//$FILENAME string_5min_menu.src
#define RSRC_STRING_5MIN_MENU 299    
//$FILENAME string_10min_menu.src
#define RSRC_STRING_10MIN_MENU 300    
//$FILENAME string_system_menu.src
#define RSRC_STRING_SYSTEM_MENU 301    
//$FILENAME string_about_menu.src
#define RSRC_STRING_ABOUT_MENU 302    
//$FILENAME string_delete_menu.src
#define RSRC_STRING_DELETE_MENU 303    
//$FILENAME string_record_menu.src
#define RSRC_STRING_RECORD_MENU 304    
//$FILENAME string_spectrogram_menu.src
#define RSRC_STRING_SPECTROGRAM_MENU 305    

//$FILENAME string_end_of_slide_show.src
#define RSRC_STRING_END_OF_SLIDE_SHOW 306    

//$FILENAME string_mb.src
#define RSRC_STRING_MB 307    

//$FILENAME internal_media.src
#define RSRC_INT_MEDIA 308    
//$FILENAME external_media.src
#define RSRC_EXT_MEDIA 309    

//$FILENAME about_title.src
#define RSRC_ABOUT_TITLE 310    
//$FILENAME player_name.src
#define RSRC_PLAYER_NAME_VER 311    

//$FILENAME settings_title.src
#define RSRC_SETTINGS_TITLE 312    
//$FILENAME jpeg_display_title.src
#define RSRC_JPEG_DISPLAY_TITLE 313    
//$FILENAME erase_title.src
#define RSRC_ERASE_TITLE 314    

//$FILENAME del_warning_no.src
#define RSRC_DELETE_NO 315    
//$FILENAME del_warning_yes.src
#define RSRC_DELETE_YES 316    
//$FILENAME del_warning_line1.src
#define RSRC_WARNING_MSG_LINE1 317    
//$FILENAME del_warning_line2.src
#define RSRC_WARNING_MSG_LINE2 318    
//$FILENAME lowbattery.src
#define RSRC_BROWNOUT_BITMAP 319    
//$FILENAME vbr.src
#define RSRC_VBR_BITMAP 320    

//$FILENAME string_song.src
#define RSRC_STRING_SONG_COLON 321    
//$FILENAME string_voice.src
#define RSRC_STRING_VOICE_COLON 322    

//$FILENAME time_date_title.src
#define RSRC_TIME_DATE_TITLE 323    
//$FILENAME set_time_title.src
#define RSRC_SET_TIME_TITLE 324    
//$FILENAME set_date_title.src
#define RSRC_SET_DATE_TITLE 325    
//$FILENAME string_searching.src
#define RSRC_STRING_SEARCHING 326    
/////////////////////////////////////////////////////////////////////////////////
//  Save Changes
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME save_changes_yes.src
#define RSRC_SAVE_CHANGES_YES 327    
//$FILENAME save_changes_no.src
#define RSRC_SAVE_CHANGES_NO 328    
//$FILENAME save_changes_cancel.src
#define RSRC_SAVE_CHANGES_CANCEL 329    
//$FILENAME save_changes_clear.src
#define RSRC_SAVE_CHANGES_CLEAR 330    
/////////////////////////////////////////////////////////////////////////////////
//  Contrast
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME contrast_title.src
#define RSRC_CONTRAST_TITLE 331    
//$FILENAME contrast_frame.src
#define RSRC_CONTRAST_FRAME 332    
//$FILENAME contrast_level0.src
#define RSRC_CONTRAST_LEVEL_0 333    
//$FILENAME contrast_level1.src
#define RSRC_CONTRAST_LEVEL_1 334    
//$FILENAME contrast_level2.src
#define RSRC_CONTRAST_LEVEL_2 335    
//$FILENAME contrast_level3.src
#define RSRC_CONTRAST_LEVEL_3 336    
//$FILENAME contrast_level4.src
#define RSRC_CONTRAST_LEVEL_4 337    
//$FILENAME contrast_level5.src
#define RSRC_CONTRAST_LEVEL_5 338    
//$FILENAME contrast_level6.src
#define RSRC_CONTRAST_LEVEL_6 339    
//$FILENAME contrast_level7.src
#define RSRC_CONTRAST_LEVEL_7 340    
//$FILENAME contrast_level8.src
#define RSRC_CONTRAST_LEVEL_8 341    
//$FILENAME contrast_level9.src
#define RSRC_CONTRAST_LEVEL_9 342    
//$FILENAME contrast_level10.src
#define RSRC_CONTRAST_LEVEL_10 343    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Funclets
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME Funclet_SetRTC.src
#define RSRC_FUNCLET_SET_RTC 344    
//$FILENAME Funclet_InitRTC.src
#define RSRC_FUNCLET_INIT_RTC 345    
//$FILENAME Funclet_ReadRTC.src
#define RSRC_FUNCLET_READ_RTC 346    
//$FILENAME Funclet_SysResetAllInt.src
#define RSRC_FUNCLET_SYSRESETALLINT 347    
//$FILENAME Funclet_SysSetupDecVec.src
#define RSRC_FUNCLET_SYSSETUPDECVEC 348    
//$FILENAME Funclet_SysSetupEncVec.src
#define RSRC_FUNCLET_SYSSETUPENCVEC 349    
//$FILENAME Funclet_SysTimeInit.src
#define RSRC_FUNCLET_SYSTIMEINIT 350    
//$FILENAME Funclet_AnalogInit.src
#define RSRC_FUNCLET_ANALOGINIT 351    
//$FILENAME Funclet_UsbConnected.src
#define RSRC_FUNCLET_USBCONNECTED 352    
//$FILENAME Funclet_ButtonInit.src
#define RSRC_FUNCLET_BUTTONINIT 353    
//$FILENAME Funclet_PowerUpHeadPhones.src
#define RSRC_FUNCLET_POWERUPHEADPHONES 354    
//$FILENAME Funclet_PowerDownHeadPhones.src
#define RSRC_FUNCLET_POWERDOWNHEADPHONES 355    
//$FILENAME Funclet_StartProject.src
#define RSRC_FUNCLET_STARTPROJECT 356    
//$FILENAME Funclet_ModuleSetTimer.src
#define RSRC_FUNCLET_MODULESETTIMER 357    
//$FILENAME null.src
#define RSRC_FUNCLET_MEDIADETECTION 358    
//$FILENAME Funclet_SysStopDecoder.src
#define RSRC_FUNCLET_SYSSTOPDECODER 359    
//$FILENAME Funclet_SysStopEncoder.src
#define RSRC_FUNCLET_SYSSTOPENCODER 360    
//$FILENAME Funclet_SysSetDecoder.src
#define RSRC_FUNCLET_SYSSETDECODER 361    
//$FILENAME Funclet_SysSetEncoder.src
#define RSRC_FUNCLET_SYSSETENCODER 362    
//$FILENAME null.src
#define RSRC_FUNCLET_PROCESSPT 363    
//$FILENAME Funclet_DisplaySplashScreen.src
#define RSRC_FUNCLET_DISPLAYSPLASHSCREEN 364    
//$FILENAME Funclet_SysGetVolume.src
#define RSRC_FUNCLET_SYSGETVOLUME 365    
//$FILENAME null.src
#define RSRC_FUNCLET_MMCENUMERATEPHYSICALDEVICE 366    
//$FILENAME null.src
#define RSRC_FUNCLET_RESETDEVICE 367    
//$FILENAME null.src
#define RSRC_FUNCLET_PROCESSCSD 368    
//$FILENAME null.src
#define RSRC_FUNCLET_PROCESSPARTITION 369    
//$FILENAME null.src
#define RSRC_FUNCLET_PARTITIONENTRYCOPY 370    
//$FILENAME Funclet_MixerMasterFadeOut.src
#define RSRC_FUNCLET_MIXER_MASTER_FADE_OUT 371    
//$FILENAME Funclet_MixerMasterFadeIn.src
#define RSRC_FUNCLET_MIXER_MASTER_FADE_IN 372    
//$FILENAME Funclet_EncSetProperties.src
#define RSRC_FUNCLET_ENCSETPROPERTIES 373    
//$FILENAME null.src
#define RSRC_FUNCLET_DOESMBREXIST 374    
//$FILENAME null.src
#define RSRC_FUNCLET_EXTRACTPARTITIONPARAMETERS 375    
//$FILENAME Funclet_SaveSettings.src
#define RSRC_FUNCLET_SYSSAVESETTINGS 376    
//$FILENAME Funclet_LoadSettings.src
#define RSRC_FUNCLET_SYSLOADSETTINGS 377    
///////////////////////////////////////////////////////////////
// Sanyo FM Tuner Fuclet
///////////////////////////////////////////////////////////////
//RSRC_FUNCLET_TUNER_DRIVER_INIT    equ      334    ;$FILENAME Funclet_TunerDriverInit.src
//RSRC_FUNCLET_TUNER_SCAN_STATIONS    equ      335    ;$FILENAME Funclet_TunerScanStations.src
//RSRC_FUNCLET_TUNER_SET_TO_PRESET    equ      336    ;$FILENAME Funclet_TunerSetToPreset.src
//RSRC_FUNCLET_TUNER_SET_MONO_STEREO    equ      337    ;$FILENAME Funclet_TunerSetMonoStereo.src
//RSRC_FUNCLET_TUNER_SET_STANDBY    equ      338    ;$FILENAME Funclet_TunerSetStandby.src
//RSRC_FUNCLET_TUNER_SET_PRESET    equ      339    ;$FILENAME Funclet_TunerSetPreset.src
//RSRC_FUNCLET_TUNER_ERASE_PRESET    equ      340    ;$FILENAME Funclet_TunerErasePreset.src
//RSRC_FUNCLET_TUNER_SET_IF    equ      341    ;$FILENAME Funclet_TunerSetIf.src
//RSRC_FUNCLET_TUNER_WRITE_IF_OSC    equ      342    ;$FILENAME Funclet_TunerWriteIfOSc.src
//RSRC_FUNCLET_TUNER_GET_FIELD_STRENGTH    equ      343    ;$FILENAME Funclet_TunerGetFieldStrength.src
//RSRC_FUNCLET_TUNER_INIT_RF_CAP_OSC    equ      344    ;$FILENAME Funclet_TunerInitRfCapOsc.src
//RSRC_FUNCLET_TUNER_STORE_FM_STATION    equ      345    ;$FILENAME Funclet_TunerStoreFmStation.src
//RSRC_FUNCLET_TUNER_STORE_FM_PRESET    equ      346    ;$FILENAME Funclet_TunerStoreFmPreset.src
//RSRC_FUNCLET_TUNER_SET_REGION    equ      347    ;$FILENAME Funclet_TunerSetRegion.src

////////////////////////////////////////////////////////////////////////////////////////////////////
// WMA Resources
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME wmaCore.src
#define RSRC_WMA_CORE 378    
//$FILENAME wmaMidLow.src
#define RSRC_WMA_MIDLOW 379    
//$FILENAME wmaHigh.src
#define RSRC_WMA_HIGH 380    
//$FILENAME wmaHighMid.src
#define RSRC_WMA_HIGHMID 381    
//$FILENAME wmaMid.src
#define RSRC_WMA_MID 382    
//$FILENAME wmaLow.src
#define RSRC_WMA_LOW 383    
//$FILENAME wmaX1mem.src
#define RSRC_WMA_DATA_X1 384    
//$FILENAME wmaYmem.src
#define RSRC_WMA_DATA_Y 385    
//$FILENAME wmaLXmem.src
#define RSRC_WMA_DATA_L_X 386    
//$FILENAME wmaLYmem.src
#define RSRC_WMA_DATA_L_Y 387    
//$FILENAME wmaHuff44Qb.src
#define RSRC_WMA_HUFF44QB 388    
//$FILENAME wmaHuff44Ob.src
#define RSRC_WMA_HUFF44OB 389    
//$FILENAME wmaHuff16Ob.src
#define RSRC_WMA_HUFF16OB 390    
//$FILENAME drmpdcommon.src
#define RSRC_DRMPD_COMMON 391    
//$FILENAME januswmasupport.src
#define RSRC_JANUS_WMASUPPORT 392    
//$FILENAME wmalicenseinit.src
#define RSRC_WMA_LICENSEINIT 393    
//$FILENAME wma_tables.src
#define RSRC_WMA_TABLES 394    
//$FILENAME janus_tables.src
#define RSRC_JANUS_TABLES 395    
//$FILENAME wma_constants.src
#define RSRC_WMA_CONSTANTS 396    
//$FILENAME janus_constants.src
#define RSRC_JANUS_CONSTANTS 397    
//$FILENAME janus_xmem.src
#define RSRC_JANUS_X 398    
//$FILENAME janusy_data.src
#define RSRC_JANUSY_DATA 399    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonts -- these are last because they are very large
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME font_table.src
#define RSRC_FONT_TABLE 400    
//$FILENAME font_PGM.src
#define RSRC_PGM_8 401    
//$FILENAME font_SGMs.src
#define RSRC_SGMS_8 402    
//$FILENAME font_script_00.src
#define RSRC_SCRIPT_00_8 403    
//$FILENAME font_scripts.src
#define RSRC_SCRIPTS_8 404    
//$FILENAME font_PDM.src
#define RSRC_PDM 405    
//$FILENAME font_SDMs.src
#define RSRC_SDMS 406    
//$FILENAME bitmap_warning.src
#define RSRC_WARNING 407    
//$FILENAME bitmap_device_full.src
#define RSRC_DEVICE_FULL 408    


//$FILENAME lcd_controller_init.src
#define RSRC_LCD_INIT_SEQ 409    

//$FILENAME Funclet_DBCSToUnicode.src
#define RSRC_FUNCLET_DBCS_TO_UNICODE 410    
//$FILENAME Funclet_DBCSToUnicodeDstXSrcY.src
#define RSRC_FUNCLET_DBCS_TO_UNICODE_DSTX_SRCY 411    
//$FILENAME Funclet_LCDSetFont.src
#define RSRC_FUNCLET_SET_FONT 412    
//$FILENAME Funclet_GetTextWidthAddressUnicode.src
#define RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE 413    
//$FILENAME Funclet_GetTextWidthResourceUnicode.src
#define RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE 414    
//$FILENAME Funclet_GetTextWidthAddressDBCS.src
#define RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS 415    
//$FILENAME Funclet_GetTextWidthResourceDBCS.src
#define RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS 416    
//$FILENAME Funclet_ChipSerialNumberInit.src
#define RSRC_FUNCLET_CHIPSERIALNUMBERINIT 417    
//$FILENAME Funclet_InternalMediaSerialNumberInit.src
#define RSRC_FUNCLET_INTERNALMEDIASERIALNUMBERINIT 418    
//$FILENAME Funclet_ChipGetSerialNumber.src
#define RSRC_FUNCLET_CHIPGETSERIALNUMBER 419    
//$FILENAME Funclet_CreateNullSerialNumber.src
#define RSRC_FUNCLET_CREATENULLSERIALNUMBER 420    
//$FILENAME Funclet_ConvertHexToASCII.src
#define RSRC_FUNCLET_CONVERTHEXTOASCII 421    
//$FILENAME Funclet_LowResolutionAdcInit.src
#define RSRC_FUNCLET_LOWRESOLUTIONADCINIT 422    


//$FILENAME sysrecord.src
#define RSRC_SYSRECORD_CODE 423    

//$FILENAME string_record_settings.src
#define RSRC_STRING_RECORD_SETTINGS 424    
//$FILENAME string_sample_rate.src
#define RSRC_STRING_SAMPLE_RATE 425    
//$FILENAME string_encoder.src
#define RSRC_STRING_ENCODER 426    
//$FILENAME string_adpcm.src
#define RSRC_STRING_ADPCM 427    
//$FILENAME string_msadpcm.src
#define RSRC_STRING_MSADPCM 428    
//$FILENAME string_imadpcm.src
#define RSRC_STRING_IMADPCM 429    
//$FILENAME string_pcm.src
#define RSRC_STRING_PCM 430    
//$FILENAME string_internal.src
#define RSRC_STRING_INTERNAL 431    
//$FILENAME string_external.src
#define RSRC_STRING_EXTERNAL 432    
//$FILENAME string_device.src
#define RSRC_STRING_DEVICE 433    
//$FILENAME string_source.src
#define RSRC_STRING_SOURCE 434    
//$FILENAME string_microphone.src
#define RSRC_STRING_MICROPHONE 435    
//$FILENAME string_linein.src
#define RSRC_STRING_LINEIN 436    
//$FILENAME string_bits.src
#define RSRC_STRING_BITS 437    
//$FILENAME string_4.src
#define RSRC_STRING_4 438    
//$FILENAME string_8.src
#define RSRC_STRING_8 439    
//$FILENAME string_16.src
#define RSRC_STRING_16 440    
//$FILENAME string_24.src
#define RSRC_STRING_24 441    
//$FILENAME string_fm.src
#define RSRC_STRING_FM 442    
//$FILENAME string_mono.src
#define RSRC_STRING_MONO 443    
//$FILENAME string_stereo.src
#define RSRC_STRING_STEREO 444    
//$FILENAME string_8000hz.src
#define RSRC_STRING_8000HZ 445    
//$FILENAME string_11025hz.src
#define RSRC_STRING_11025HZ 446    
//$FILENAME string_16000hz.src
#define RSRC_STRING_16000HZ 447    
//$FILENAME string_22050hz.src
#define RSRC_STRING_22050HZ 448    
//$FILENAME string_32000hz.src
#define RSRC_STRING_32000HZ 449    
//$FILENAME string_44100hz.src
#define RSRC_STRING_44100HZ 450    
//$FILENAME string_48000hz.src
#define RSRC_STRING_48000HZ 451    
//$FILENAME string_channels.src
#define RSRC_STRING_CHANNELS 452    
//$FILENAME string_spaces.src
#define RSRC_STRING_SPACES 453    
//$FILENAME slider_bar.src
#define RSRC_SLIDER_BAR 454    
//$FILENAME slider_bar_inv.src
#define RSRC_SLIDER_BAR_INV 455    
//$FILENAME slider_track.src
#define RSRC_SLIDER_TRACK 456    
//$FILENAME string_no_files.src
#define RSRC_STRING_NO_FILES 457    

/////////////////////////////////////////////////////////////////////
//  Time and Date Resource Strings
/////////////////////////////////////////////////////////////////////
//$FILENAME string_sunday.src
#define RSRC_STRING_SUNDAY 458    
//$FILENAME string_monday.src
#define RSRC_STRING_MONDAY 459    
//$FILENAME string_tuesday.src
#define RSRC_STRING_TUESDAY 460    
//$FILENAME string_wednesday.src
#define RSRC_STRING_WEDNESDAY 461    
//$FILENAME string_thursday.src
#define RSRC_STRING_THURSDAY 462    
//$FILENAME string_friday.src
#define RSRC_STRING_FRIDAY 463    
//$FILENAME string_saturday.src
#define RSRC_STRING_SATURDAY 464    
//$FILENAME string_am.src
#define RSRC_STRING_AM 465    
//$FILENAME string_pm.src
#define RSRC_STRING_PM 466    
//$FILENAME string_amclear.src
#define RSRC_STRING_AMCLEAR 467    
//$FILENAME string_slash.src
#define RSRC_STRING_SLASH 468    
//$FILENAME string_colon.src
#define RSRC_STRING_COLON 469    
//$FILENAME string_12hour.src
#define RSRC_STRING_12HOUR 470    
//$FILENAME string_24hour.src
#define RSRC_STRING_24HOUR 471    
//$FILENAME string_format.src
#define RSRC_STRING_FORMAT 472    
//$FILENAME string_mmddyyyy.src
#define RSRC_STRING_MMDDYYYY 473    
//$FILENAME string_ddmmyyyy.src
#define RSRC_STRING_DDMMYYYY 474    
//$FILENAME string_yyyymmdd.src
#define RSRC_STRING_YYYYMMDD 475    
//$FILENAME string_ok.src
#define RSRC_STRING_OK 476    
//$FILENAME string_cancel.src
#define RSRC_STRING_CANCEL 477    
//$FILENAME negative_sign.src
#define RSRC_NEGATIVE_SIGN 478    
//$FILENAME string_dec_pt5.src
#define RSRC_DEC_PT5_STRING 479    
//$FILENAME string_dec_pt0.src
#define RSRC_DEC_PT0_STRING 480    
//$FILENAME string_db.src
#define RSRC_DB_STRING 481    
//$FILENAME string_hz2.src
#define RSRC_HZ2_STRING 482    


//$FILENAME record_settings_menu_code_bank.src
#define RSRC_RECORD_SETTINGS_MENU_CODE_BANK 483    
//$FILENAME metadata_codebank.src
#define RSRC_METADATA_CODEBANK 484    
//$FILENAME mp3metadata_codebank.src
#define RSRC_MP3_METADATA_CODEBANK 485    
//$FILENAME wmametadata_codebank.src
#define RSRC_WMA_METADATA_CODEBANK 486    
//$FILENAME wavmetadata_codebank.src
#define RSRC_WAV_METADATA_CODEBANK 487    
//$FILENAME smvmetadata_codebank.src
#define RSRC_SMV_METADATA_CODEBANK 488    
//$FILENAME playlist2init_codebank.src
#define RSRC_PLAYLIST2INIT_CODEBANK 489    

//$FILENAME delete_successful.src
#define RSRC_DELETE_SUCCESSFUL 490    
//$FILENAME delete_error.src
#define RSRC_DELETE_ERROR 491    
//$FILENAME lic_expired.src
#define RSRC_LIC_EXPIRED 492    
//$FILENAME id3v2_codebank.src
#define RSRC_ID3V2_CODEBANK 493    
//$FILENAME id3v2lyrics_codebank.src
#define RSRC_ID3V2_LYRICS_CODEBANK 494    
//$FILENAME lyrics3_codebank.src
#define RSRC_LYRICS3_CODEBANK 495    
//$FILENAME lrc_codebank.src
#define RSRC_LRC_CODEBANK 496    
//$FILENAME lyrics_api_codebank.src
#define RSRC_LYRICS_API_CODEBANK 497    
//$FILENAME wmalyrics_codebank.src
#define RSRC_WMA_LYRICS_CODEBANK 498    
//$FILENAME apicframe_codebank.src
#define RSRC_APIC_FRAME_CODEBANK 499    

//$FILENAME exmediaerror1.src
#define RSRC_EXTERNAL_MEDIA_ERROR1 500    
//$FILENAME exmediaerror2.src
#define RSRC_EXTERNAL_MEDIA_ERROR2 501    
//$FILENAME inmediaerror1.src
#define RSRC_INTERNAL_MEDIA_ERROR1 502    

//$FILENAME backlight_title.src
#define RSRC_BACKLIGHT_TITLE 503    
//$FILENAME backlight_state_on.src
#define RSRC_BACKLIGHT_STATE_ON 504    
//$FILENAME backlight_state_off.src
#define RSRC_BACKLIGHT_STATE_OFF 505    
//$FILENAME backlightmenu.src
#define RSRC_BACKLIGHT_MENU_CODE_BANK 506    
//$FILENAME string_backlight_menu.src
#define RSRC_STRING_BACKLIGHT_MENU 507    

//$FILENAME enc_mp3mod.src
#define RSRC_ENC_MP3_MOD_CODE 508    
//$FILENAME enc_mp3p.src
#define RSRC_ENC_MP3P_CODE 509    
//$FILENAME enc_mp3x.src
#define RSRC_ENC_MP3X_CODE 510    
//$FILENAME enc_mp3y.src
#define RSRC_ENC_MP3Y_CODE 511    
//$FILENAME mp3_implementation.src
#define RSRC_MP3_IMPLEMENTATION 512    
//$FILENAME string_mp3.src
#define RSRC_STRING_MP3 513    
//$FILENAME string_all.src
#define RSRC_STRING_ALL 514    

//$FILENAME mediastartup.src
#define RSRC_NANDMEDIAINIT 515    
#define RSRC_NANDMEDIAALLOCATE 0
#define RSRC_NANDMEDIADISCOVERALLOCATION 0
#define RSRC_NANDMEDIAGETMEDIATABLE RSRC_NANDMEDIAINIT
#define RSRC_NANDMEDIAGETINFO RSRC_NANDMEDIAINIT
#define RSRC_NANDMEDIAERASE 0

//$FILENAME nanddatadriveinit.src
#define RSRC_NANDDATADRIVEINIT 516    
#define RSRC_NANDDATADRIVEGETINFO RSRC_NANDDATADRIVEREADSECTOR
#define RSRC_NANDDATADRIVESETINFO 0
//$FILENAME nanddatadrivereadsector.src
#define RSRC_NANDDATADRIVEREADSECTOR 517    
#define RSRC_NANDDATADRIVEWRITESECTOR RSRC_NANDDATADRIVEREADSECTOR
#define RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP 0
#define RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR 0
#define RSRC_NANDDATADRIVECACHEMULTIWRITESETUP 0
#define RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR 0
#define RSRC_NANDDATADRIVEERASE 0
#define RSRC_NANDDATADRIVEFLUSH RSRC_NANDDATADRIVEREADSECTOR

#define RSRC_NANDSYSTEMDRIVEINIT RSRC_NANDMEDIAINIT
#define RSRC_NANDSYSTEMDRIVEGETINFO RSRC_NANDMEDIAINIT
#define RSRC_NANDSYSTEMDRIVESETINFO 0
#define RSRC_NANDSYSTEMDRIVEREADSECTOR 0
#define RSRC_NANDSYSTEMDRIVEWRITESECTOR 0
#define RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP 0
#define RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR 0
#define RSRC_NANDSYSTEMDRIVEERASE 0
#define RSRC_NANDSYSTEMDRIVEFLUSH 0

//$FILENAME vbr_codebank.src
#define RSRC_VBR_CODEBANK 518    

//$FILENAME string_recordtest_menu.src
#define RSRC_STRING_RECORDTEST_MENU 519    
//$FILENAME string_recordtest_duration.src
#define RSRC_STRING_DURATION 520    
//$FILENAME string_recordtest_time5.src
#define RSRC_STRING_TIME5 521    
//$FILENAME string_recordtest_time10.src
#define RSRC_STRING_TIME10 522    
//$FILENAME string_recordtest_time30.src
#define RSRC_STRING_TIME30 523    
//$FILENAME string_recordtest_time60.src
#define RSRC_STRING_TIME60 524    
//$FILENAME string_recordtest_time300.src
#define RSRC_STRING_TIME300 525    
//$FILENAME string_recordtest_time600.src
#define RSRC_STRING_TIME600 526    

//$FILENAME test_title.src
#define RSRC_TEST_TITLE 527    
//$FILENAME testmenu.src
#define RSRC_TEST_MENU_CODE_BANK 528    


//$FILENAME mmcmediastartup.src
#define RSRC_MMCDATADRIVEINIT 529    
#define RSRC_MMCMEDIAALLOCATE RSRC_MMCDATADRIVEINIT
#define RSRC_MMCMEDIADISCOVERALLOCATION RSRC_MMCDATADRIVEINIT
//$FILENAME mmcinfo.src
#define RSRC_MMCMEDIAGETINFO 530    
#define RSRC_MMCMEDIAGETMEDIATABLE RSRC_MMCMEDIAGETINFO
//$FILENAME mmcerase.src
#define RSRC_MMCMEDIAERASE 531    


#define RSRC_MMCDATADRIVEFLUSH RSRC_MMCDATADRIVEINIT

//$FILENAME mmcenumerate.src
#define RSRC_MMCENUMERATE 532    
//$FILENAME mmcresetdevice.src
#define RSRC_MMC_RESETDEVICE 533    
//$FILENAME mmcprocesscsd.src
#define RSRC_MMC_PROCESSCSD 534    
//$FILENAME mmcprocesscid.src
#define RSRC_MMC_PROCESSCID 535    
//$FILENAME mmcprocesscid2.src
#define RSRC_FUNCLET_MMCPROCESSCID2 536    
//$FILENAME mmcdetectpresence.src
#define RSRC_MMC_DETECTPRESENCE 537    
//$FILENAME mmcserialnumberinit.src
#define RSRC_MMCSERIALNUMBERINIT 538    
//$FILENAME mmccheckwriteprotect.src
#define RSRC_MMC_CHECKWRITEPROTECT 539    

//$FILENAME mmcread.src
#define RSRC_MMCDATADRIVEREADSECTOR 540    
//$FILENAME mmcmediainit.src
#define RSRC_MMCMEDIAINIT 541    
//$FILENAME mmcdatadriveinfo.src
#define RSRC_MMCDATADRIVESETINFO 542    
#define RSRC_MMCDATADRIVEGETINFO RSRC_MMCDATADRIVESETINFO
//$FILENAME mmcdatadriveerase.src
#define RSRC_MMCDATADRIVEERASE 543    

#define RSRC_MMCDATADRIVEWRITESECTOR RSRC_MMCDATADRIVEREADSECTOR
#define RSRC_MMCDATADRIVEMULTIWRITESETUP RSRC_MMCDATADRIVEWRITESECTOR
#define RSRC_MMCDATADRIVEMULTIWRITESECTOR RSRC_MMCDATADRIVEWRITESECTOR


/////////////////////////////////////////////////////////////////////////////////
//  File system
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME arrangefilename.src
#define RSRC_FUNCLET_ARRANGEFILENAME 544    
//$FILENAME clearcluster.src
#define RSRC_FUNCLET_CLEARCLUSTER 545    
//$FILENAME createdirectory.src
#define RSRC_FUNCLET_CREATEDIRECTORY 546    
//$FILENAME deletecontent.src
#define RSRC_FUNCLET_DELETECONTENT 547    
//$FILENAME deleterecord.src
#define RSRC_FUNCLET_DELETERECORD 548    
//$FILENAME fastopen.src
#define RSRC_FUNCLET_FASTOPEN 549    
//$FILENAME fcreate.src
#define RSRC_FUNCLET_FCREATE 550    
//$FILENAME filegetattrib.src
#define RSRC_FUNCLET_FILEGETATTRIB 551    
//$FILENAME filegetdate.src
#define RSRC_FUNCLET_FILEGETDATE 552    
//$FILENAME filesetattrib.src
#define RSRC_FUNCLET_FILESETATTRIB 553    
//$FILENAME filesetdate.src
#define RSRC_FUNCLET_FILESETDATE 554    
//$FILENAME fsinit.src
#define RSRC_FUNCLET_FSINIT 555    
//$FILENAME fsshutdown.src
#define RSRC_FUNCLET_FSSHUTDOWN 556    
//$FILENAME readdevicerecord.src
#define RSRC_FUNCLET_READDEVICERECORD 557    
//$FILENAME checkspaceinrootdir.src
#define RSRC_FUNCLET_CHECKSPACEINROOTDIR 558    
//$FILENAME setcwdhandle.src
#define RSRC_FUNCLET_SETCWDHANDLE 559    
//$FILENAME fsdriveinit.src
#define RSRC_FUNCLET_FSDRIVEINIT 560    
//$FILENAME fsclearBuf.src
#define RSRC_FUNCLET_FSCLEARDRIVEBUF 561    
//RSRC_FUNCLET_TOTALFREECLUSTER    equ      328    ;$FILENAME totalfreecluster.src
//RSRC_FUNCLET_TOTALFREECLUSTERFAT16    equ      328    ;$FILENAME totalfreeclusterfat16.src
//$FILENAME fgetfasthandle.src
#define RSRC_FUNCLET_FGETFASTHANDLE 562    
//$FILENAME ishandlewriteallocated.src
#define RSRC_FUNCLET_ISHANDLEWRITEALLOCATED 563    
//$FILENAME isfileopen.src
#define RSRC_FUNCLET_ISFILEOPEN 564    
//$FILENAME iscurrworkdir.src
#define RSRC_FUNCLET_ISCURRWORKDIR 565    
//$FILENAME chdir.src
#define RSRC_FUNCLET_CHDIR 566    
//$FILENAME chdirFromOffset.src
#define RSRC_FUNCLET_CHDIR_FROM_OFFSET 567    
//$FILENAME deletetree.src
#define RSRC_FUNCLET_DELETETREE 568    
//$FILENAME deleteallrecords.src
#define RSRC_FUNCLET_DELETEALLRECORDS 569    
//$FILENAME cleardata.src
#define RSRC_FUNCLET_CLEARDATA 570    
//$FILENAME changetolowleveldir.src
#define RSRC_FUNCLET_CHANGETOLOWLEVELDIR 571    
//$FILENAME getrecordnumber.src
#define RSRC_FUNCLET_GETRECORDNUMBER 572    
//$FILENAME fileremove.src
#define RSRC_FUNCLET_FILEREMOVE 573    
//$FILENAME charactersearch.src
#define RSRC_FUNCLET_CHARACTERSEARCH 574    
//$FILENAME stringcompare.src
#define RSRC_FUNCLET_STRINGCOMPARE 575    
//$FILENAME fopenw.src
#define RSRC_FUNCLET_FOPENW 576    
//$FILENAME fremove.src
#define RSRC_FUNCLET_FREMOVE 577    
//$FILENAME fremovew.src
#define RSRC_FUNCLET_FREMOVEW 578    
//$FILENAME mkdir.src
#define RSRC_FUNCLET_MKDIR 579    
//$FILENAME mkdirw.src
#define RSRC_FUNCLET_MKDIRW 580    
//$FILENAME rmdir.src
#define RSRC_FUNCLET_RMDIR 581    
//$FILENAME rmdirw.src
#define RSRC_FUNCLET_RMDIRW 582    
//$FILENAME fgetc.src
#define RSRC_FUNCLET_FGETC 583    
//$FILENAME fgets.src
#define RSRC_FUNCLET_FGETS 584    
//$FILENAME fputc.src
#define RSRC_FUNCLET_FPUTC 585    
//$FILENAME fputs.src
#define RSRC_FUNCLET_FPUTS 586    
//$FILENAME arrangelongfilename.src
#define RSRC_FUNCLET_ARRANGELONGFILENAME 587    
//$FILENAME convert_itoa.src
#define RSRC_FUNCLET_CONVERT_ITOA 588    
//$FILENAME createdirrecord.src
#define RSRC_FUNCLET_CREATEDIRRECORD 589    
//$FILENAME chksum.src
#define RSRC_FUNCLET_CHKSUM 590    
//$FILENAME createshortdirrecord.src
#define RSRC_FUNCLET_CREATESHORTDIRRECORD 591    
//$FILENAME discardtrailigperiodsucs3.src
#define RSRC_FUNCLET_DISCARDTRAILIGPERIODSUCS3 592    
//$FILENAME discardtrailigperiodsw.src
#define RSRC_FUNCLET_DISCARDTRAILIGPERIODSW 593    
//$FILENAME extractfilenamew.src
#define RSRC_FUNCLET_EXTRACTFILENAMEW 594    
//$FILENAME extractpathw.src
#define RSRC_FUNCLET_EXTRACTPATHW 595    
//$FILENAME findfreerecord.src
#define RSRC_FUNCLET_FINDFREERECORD 596    
//$FILENAME getnamew.src
#define RSRC_FUNCLET_GETNAMEW 597    
//$FILENAME isdirectoryempty.src
#define RSRC_FUNCLET_ISDIRECTORYEMPTY 598    
//$FILENAME isshortnamevalid.src
#define RSRC_FUNCLET_ISSHORTNAMEVALID 599    
//$FILENAME longdirmatch.src
#define RSRC_FUNCLET_LONGDIRMATCH 600    
//$FILENAME unicodetooem.src
#define RSRC_FUNCLET_UNICODETOOEM 601    
//$FILENAME matchdirrecordw.src
#define RSRC_FUNCLET_MATCHDIRRECORDW 602    
//$FILENAME setcwd.src
#define RSRC_FUNCLET_SETCWD 603    
//$FILENAME setshortfilename.src
#define RSRC_FUNCLET_SETSHORTFILENAME 604    
//$FILENAME generatefilenametail.src
#define RSRC_FUNCLET_GENERATEFILENAMETAIL 605    
//$FILENAME dbcstounicode.src
#define RSRC_FUNCLET_DBCSTOUNICODE 606    
//$FILENAME strcpy.src
#define RSRC_FUNCLET_STRCPY 607    
//$FILENAME strcpyw.src
#define RSRC_FUNCLET_STRCPYW 608    
//$FILENAME strlengthw.src
#define RSRC_FUNCLET_STRLENGTHW 609    
//$FILENAME filesystempresent.src
#define RSRC_FUNCLET_FILESYSTEMPRESENT 610    
//$FILENAME DataDriveInit.src
#define RSRC_FUNCLET_DATADRIVEINIT 611    
//$FILENAME FSGetDataDrivePbsLocation.src
#define RSRC_FUNCLET_FSGETDATADRIVEPBSLOCATION 612    
//$FILENAME FSPartitionEntryCopy.src
#define RSRC_FUNCLET_FSPARTITIONENTRYCOPYFSDD 613    
//$FILENAME DataDriveGetSize.src
#define RSRC_FUNCLET_DATADRIVEGETSIZE 614    
//$FILENAME ConstructLongFileName.src
#define RSRC_FUNCLET_CONSTRUCTLONGFILENAME 615    
//$FILENAME strcpyucs3_2.src
#define RSRC_FUNCLET_STRCPYUCS3_2 616    
//$FILENAME getvolumelabel.src
#define RSRC_FUNCLET_GETVOLUMELABEL 617    
//$FILENAME setvolumelabel.src
#define RSRC_FUNCLET_SETVOLUMELABEL 618    
//$FILENAME disk_full.src
#define RSRC_DISK_FULL 619    
//$FILENAME chkdskstartup.src
#define RSRC_CHECKDISK 620    
//$FILENAME chkdskstartupy.src
#define RSRC_CHKDSK_YMEM 621    
//$FILENAME low_level_pwr_line1.src
#define RSRC_LOW_PWR_EXT_DEV_LINE1 622    
//$FILENAME low_level_pwr_line2.src
#define RSRC_LOW_PWR_EXT_DEV_LINE2 623    
//$FILENAME string_bit_rate.src
#define RSRC_STRING_BIT_RATE 624    
//$FILENAME string_96000hz.src
#define RSRC_STRING_96KBPS 625    
//$FILENAME string_112000hz.src
#define RSRC_STRING_112KBPS 626    
//$FILENAME string_128000hz.src
#define RSRC_STRING_128KBPS 627    
//$FILENAME string_160000hz.src
#define RSRC_STRING_160KBPS 628    
//$FILENAME string_192000hz.src
#define RSRC_STRING_192KBPS 629    
//$FILENAME string_224000hz.src
#define RSRC_STRING_224KBPS 630    
//$FILENAME string_256000hz.src
#define RSRC_STRING_256KBPS 631    
//$FILENAME string_320000hz.src
#define RSRC_STRING_320KBPS 632    
//$FILENAME string_hz.src
#define RSRC_STRING_HZ 633    
//$FILENAME EncCommonp.src
#define RSRC_ENCODERCOMMON_P_CODE 634    
//$FILENAME adc_adcx.src
#define RSRC_ADC_ADCX_CODE 635    
//$FILENAME adc_adcy.src
#define RSRC_ADC_ADCY_CODE 636    
//$FILENAME Funclet_encodercommon.src
#define RSRC_FUNCLET_LOADENCODERCOMMON 637    
//$FILENAME string_album.src
#define RSRC_STRING_ALBUM 638    
//$FILENAME string_encoder_song.src
#define RSRC_STRING_SONG 639    
//$FILENAME string_mode.src
#define RSRC_STRING_MODE 640    

//////////////////////////////////////////////////////////////////////////////////
// display related
//////////////////////////////////////////////////////////////////////////////////
//$FILENAME Funclet_HalDisplayInit.src
#define RSRC_FUNCLET_HAL_DISPLAY_INIT 641    
//$FILENAME Funclet_SalDisplayInit.src
#define RSRC_FUNCLET_SAL_DISPLAY_INIT 642    
//$FILENAME Funclet_SalHandleMessage.src
#define RSRC_FUNCLET_SAL_HANDLE_MSG 643    
//$FILENAME Funclet_FixBitmapHeader.src
#define RSRC_FUNCLET_FIXBITMAPHEADER 644    
//$FILENAME Funclet_BitmapReadHeader.src
#define RSRC_FUNCLET_BITMAPREADHEADER 645    
//$FILENAME Funclet_BitmapReadPalette.src
#define RSRC_FUNCLET_BITMAPREADPALETTE 646    
//$FILENAME Funclet_BitmapReadBitMask.src
#define RSRC_FUNCLET_BITMAPREADBITMASK 647    
//$FILENAME Funclet_BitmapPrintNonPalette.src
#define RSRC_FUNCLET_BITMAPPRINTNONPALETTE 648    
//$FILENAME Funclet_BitmapPrintPalette.src
#define RSRC_FUNCLET_BITMAPPRINTPALETTE 649    
//$FILENAME Funclet_DisplayPrintBitmapFile.src
#define RSRC_FUNCLET_DISPLAYPRINTBITMAPFILE 650    
//$FILENAME Funclet_DisplayPrintBitmapResource.src
#define RSRC_FUNCLET_DISPLAYPRINTBITMAPRESOURCE 651    

//////////////////////////////////////////////////////////////////////////////////
//WMDRM Related
//////////////////////////////////////////////////////////////////////////////////
//$FILENAME legacy_decryptcontentkey.src
#define RSRC_FUNCLET_LEGACY_DECRYPTCONTENTKEY 652    
//$FILENAME drm_bbx_legacycipherkeysetup.src
#define RSRC_FUNCLET_DRM_BBX_LEGACYCIPHERKEYSETUP 653    
//$FILENAME drm_bbx_initialize.src
#define RSRC_FUNCLET_DRM_BBX_INITIALIZE 654    
//$FILENAME drm_bbx_canbind.src
#define RSRC_FUNCLET_DRM_BBX_CANBIND 655    
//$FILENAME verifychecksum.src
#define RSRC_FUNCLET_VERIFYCHECKSUM 656    
//$FILENAME drm_b64_encodew.src
#define RSRC_FUNCLET_DRM_B64_ENCODEW 657    
//$FILENAME _performactions.src
#define RSRC_FUNCLET__PERFORMACTIONS 658    
//$FILENAME _processendofchain.src
#define RSRC_FUNCLET__PROCESSENDOFCHAIN 659    
//$FILENAME drmcrt_iswxdigit.src
#define RSRC_FUNCLET_DRMCRT_ISWXDIGIT 660    
//$FILENAME drmcrt_towlower.src
#define RSRC_FUNCLET_DRMCRT_TOWLOWER 661    
//$FILENAME drmcrt_wcslen.src
#define RSRC_FUNCLET_DRMCRT_WCSLEN 662    
//$FILENAME drmcrt_wcsncpy.src
#define RSRC_FUNCLET_DRMCRT_WCSNCPY 663    
//$FILENAME drmcrt_memmove.src
#define RSRC_FUNCLET_DRMCRT_MEMMOVE 664    
//$FILENAME performoperation_part1.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART1 665    
//$FILENAME performoperation_part2.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART2 666    
//$FILENAME performoperation_part3.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART3 667    
//$FILENAME performoperation_part4.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART4 668    
//$FILENAME performoperation_part5.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART5 669    
//$FILENAME performoperation_part6.src
#define RSRC_FUNCLET_PERFORMOPERATION_PART6 670    
//$FILENAME isvalidfunction.src
#define RSRC_FUNCLET_ISVALIDFUNCTION 671    
//$FILENAME functiongetvalue.src
#define RSRC_FUNCLET_FUNCTIONGETVALUE 672    
//$FILENAME globalsetvariable.src
#define RSRC_FUNCLET_GLOBALSETVARIABLE 673    
//$FILENAME variabledrmkgetorset.src
#define RSRC_FUNCLET_VARIABLEDRMKGETORSET 674    
//$FILENAME variabledrmgetorset.src
#define RSRC_FUNCLET_VARIABLEDRMGETORSET 675    
//$FILENAME variableappgetorset.src
#define RSRC_FUNCLET_VARIABLEAPPGETORSET 676    
//$FILENAME variablelicensegetorset.src
#define RSRC_FUNCLET_VARIABLELICENSEGETORSET 677    
//$FILENAME variablecontentgetorset.src
#define RSRC_FUNCLET_VARIABLECONTENTGETORSET 678    
//$FILENAME variabledevicegetorset.src
#define RSRC_FUNCLET_VARIABLEDEVICEGETORSET 679    
//$FILENAME variablepmlicensegetorset.src
#define RSRC_FUNCLET_VARIABLEPMLICENSEGETORSET 680    
//$FILENAME drm_hds_createstore.src
#define RSRC_FUNCLET_DRM_HDS_CREATESTORE 681    
//$FILENAME drm_hds_init.src
#define RSRC_FUNCLET_DRM_HDS_INIT 682    
//$FILENAME drm_hds_uninit.src
#define RSRC_FUNCLET_DRM_HDS_UNINIT 683    
//$FILENAME drm_hds_openstore.src
#define RSRC_FUNCLET_DRM_HDS_OPENSTORE 684    
//$FILENAME drm_hds_deleteslot.src
#define RSRC_FUNCLET_DRM_HDS_DELETESLOT 685    
//$FILENAME drm_hds_slotresize.src
#define RSRC_FUNCLET_DRM_HDS_SLOTRESIZE 686    
//$FILENAME drm_hds_initslotenum.src
#define RSRC_FUNCLET_DRM_HDS_INITSLOTENUM 687    
//$FILENAME drm_hds_cleanupstore.src
#define RSRC_FUNCLET_DRM_HDS_CLEANUPSTORE 688    
//$FILENAME _hdscopyslot_child2child.src
#define RSRC_FUNCLET__HDSCOPYSLOT_CHILD2CHILD 689    
//$FILENAME _hdscopychildpayload.src
#define RSRC_FUNCLET__HDSCOPYCHILDPAYLOAD 690    
//$FILENAME _hdsdefragmentfile.src
#define RSRC_FUNCLET__HDSDEFRAGMENTFILE 691    
//$FILENAME _hdscleanupnamespace.src
#define RSRC_FUNCLET__HDSCLEANUPNAMESPACE 692    
//$FILENAME _hdscleanupstore.src
#define RSRC_FUNCLET__HDSCLEANUPSTORE 693    
//$FILENAME drm_lst_clean.src
#define RSRC_FUNCLET_DRM_LST_CLEAN 694    
//$FILENAME _hdslocatefreeblockforslot.src
#define RSRC_FUNCLET__HDSLOCATEFREEBLOCKFORSLOT 695    
//$FILENAME _hdslockblock2deleteslot.src
#define RSRC_FUNCLET__HDSLOCKBLOCK2DELETESLOT 696    
//$FILENAME _hdsunlockblock2deleteslot.src
#define RSRC_FUNCLET__HDSUNLOCKBLOCK2DELETESLOT 697    
//$FILENAME _hdscreatenamespace.src
#define RSRC_FUNCLET__HDSCREATENAMESPACE 698    
//$FILENAME _hdsfilepos2blocknum.src
#define RSRC_FUNCLET__HDSFILEPOS2BLOCKNUM 699    
//$FILENAME _writesrn.src
#define RSRC_FUNCLET__WRITESRN 700    
//$FILENAME _writecommonblockheader.src
#define RSRC_FUNCLET__WRITECOMMONBLOCKHEADER 701    
//$FILENAME _writechildblockheader.src
#define RSRC_FUNCLET__WRITECHILDBLOCKHEADER 702    
//$FILENAME _readdatablockheader.src
#define RSRC_FUNCLET__READDATABLOCKHEADER 703    
//$FILENAME _writedatablockheader.src
#define RSRC_FUNCLET__WRITEDATABLOCKHEADER 704    
//$FILENAME _hdsexpandstore.src
#define RSRC_FUNCLET__HDSEXPANDSTORE 705    
//$FILENAME _hdsallocblock.src
#define RSRC_FUNCLET__HDSALLOCBLOCK 706    
//$FILENAME _hdsfreeblock.src
#define RSRC_FUNCLET__HDSFREEBLOCK 707    
//$FILENAME _hdscreateoversizedslot.src
#define RSRC_FUNCLET__HDSCREATEOVERSIZEDSLOT 708    
//$FILENAME _hdsallocslotinfile.src
#define RSRC_FUNCLET__HDSALLOCSLOTINFILE 709    
//$FILENAME _hdswriteblockhdr.src
#define RSRC_FUNCLET__HDSWRITEBLOCKHDR 710    
//$FILENAME _hdsadjustchildpayload.src
#define RSRC_FUNCLET__HDSADJUSTCHILDPAYLOAD 711    
//$FILENAME _hdsfindleftmostleafblock.src
#define RSRC_FUNCLET__HDSFINDLEFTMOSTLEAFBLOCK 712    
//$FILENAME _hdscreateandopenslot.src
#define RSRC_FUNCLET__HDSCREATEANDOPENSLOT 713    
//$FILENAME _hdsremoveslot.src
#define RSRC_FUNCLET__HDSREMOVESLOT 714    
//$FILENAME _hdscopyandlocknewslot.src
#define RSRC_FUNCLET__HDSCOPYANDLOCKNEWSLOT 715    
//$FILENAME _hdsrelocateslotandkeepcurrlock.src
#define RSRC_FUNCLET__HDSRELOCATESLOTANDKEEPCURRLOCK 716    
//$FILENAME _hdstraversenextrightsiblingblock.src
#define RSRC_FUNCLET__HDSTRAVERSENEXTRIGHTSIBLINGBLOCK 717    
//$FILENAME _hdstraverseblocksinpostorder.src
#define RSRC_FUNCLET__HDSTRAVERSEBLOCKSINPOSTORDER 718    
//$FILENAME _hdsslotresize.src
#define RSRC_FUNCLET__HDSSLOTRESIZE 719    
//$FILENAME _isnull.src
#define RSRC_FUNCLET__ISNULL 720    
//$FILENAME _hdsgensrnhash.src
#define RSRC_FUNCLET__HDSGENSRNHASH 721    
//$FILENAME _hdsallocblockbuffer.src
#define RSRC_FUNCLET__HDSALLOCBLOCKBUFFER 722    
//$FILENAME _readsrn.src
#define RSRC_FUNCLET__READSRN 723    
//$FILENAME _hdsgetputchildblocknum.src
#define RSRC_FUNCLET__HDSGETPUTCHILDBLOCKNUM 724    
//$FILENAME _hdshashkeytoindex.src
#define RSRC_FUNCLET__HDSHASHKEYTOINDEX 725    
//$FILENAME _hdsslotwrite.src
#define RSRC_FUNCLET__HDSSLOTWRITE 726    
//$FILENAME _hdsinitslotenum.src
#define RSRC_FUNCLET__HDSINITSLOTENUM 727    
//$FILENAME drm_lst_close.src
#define RSRC_FUNCLET_DRM_LST_CLOSE 728    
//$FILENAME drm_lst_enumnext.src
#define RSRC_FUNCLET_DRM_LST_ENUMNEXT 729    
//$FILENAME drm_lst_enumdelete.src
#define RSRC_FUNCLET_DRM_LST_ENUMDELETE 730    
//$FILENAME _processextensions.src
#define RSRC_FUNCLET__PROCESSEXTENSIONS 731    
//$FILENAME _processidlist.src
#define RSRC_FUNCLET__PROCESSIDLIST 732    
//$FILENAME _processexclusions.src
#define RSRC_FUNCLET__PROCESSEXCLUSIONS 733    
//$FILENAME _processinclusions.src
#define RSRC_FUNCLET__PROCESSINCLUSIONS 734    
//$FILENAME drm_opl_processcopyoutputleveldata.src
#define RSRC_FUNCLET_DRM_OPL_PROCESSCOPYOUTPUTLEVELDATA 735    
//$FILENAME _getopllevel.src
#define RSRC_FUNCLET__GETOPLLEVEL 736    
//$FILENAME drm_opl_processplayoutputleveldata.src
#define RSRC_FUNCLET_DRM_OPL_PROCESSPLAYOUTPUTLEVELDATA 737    
//$FILENAME _updateattributetokeninslot.src
#define RSRC_FUNCLET__UPDATEATTRIBUTETOKENINSLOT 738    
//$FILENAME drm_sst_settokenvalue.src
#define RSRC_FUNCLET_DRM_SST_SETTOKENVALUE 739    
//$FILENAME drm_sst_createlicensestatepassword.src
#define RSRC_FUNCLET_DRM_SST_CREATELICENSESTATEPASSWORD 740    
//$FILENAME drm_utl_promoteansitounicode.src
#define RSRC_FUNCLET_DRM_UTL_PROMOTEANSITOUNICODE 741    
//$FILENAME drm_pk_symmetricsign.src
#define RSRC_FUNCLET_DRM_PK_SYMMETRICSIGN 742    
//$FILENAME drm_utl_getversionfromstring.src
#define RSRC_FUNCLET_DRM_UTL_GETVERSIONFROMSTRING 743    
//$FILENAME drm_utl_stringinsertblanksubstring.src
#define RSRC_FUNCLET_DRM_UTL_STRINGINSERTBLANKSUBSTRING 744    
//$FILENAME drm_utl_stringremovesubstring.src
#define RSRC_FUNCLET_DRM_UTL_STRINGREMOVESUBSTRING 745    
//$FILENAME drm_utl_dstrsearch.src
#define RSRC_FUNCLET_DRM_UTL_DSTRSEARCH 746    
//$FILENAME drm_utl_dstrsearchreverse.src
#define RSRC_FUNCLET_DRM_UTL_DSTRSEARCHREVERSE 747    
//$FILENAME drm_utl_stringtoguid.src
#define RSRC_FUNCLET_DRM_UTL_STRINGTOGUID 748    
//$FILENAME drm_utl_ensuredataalignment.src
#define RSRC_FUNCLET_DRM_UTL_ENSUREDATAALIGNMENT 749    
//$FILENAME overlappingdates.src
#define RSRC_FUNCLET_OVERLAPPINGDATES 750    
//$FILENAME drm_asd_parsev2license.src
#define RSRC_FUNCLET_DRM_ASD_PARSEV2LICENSE 751    
//$FILENAME neginfdate.src
#define RSRC_FUNCLET_NEGINFDATE 752    
//$FILENAME infdate.src
#define RSRC_FUNCLET_INFDATE 753    
//$FILENAME isexpired.src
#define RSRC_FUNCLET_ISEXPIRED 754    
//$FILENAME getsecstateattr.src
#define RSRC_FUNCLET_GETSECSTATEATTR 755    
//$FILENAME setexpirycategory.src
#define RSRC_FUNCLET_SETEXPIRYCATEGORY 756    
//$FILENAME getv2licenseinfo.src
#define RSRC_FUNCLET_GETV2LICENSEINFO 757    
//$FILENAME getnextlicense.src
#define RSRC_FUNCLET_GETNEXTLICENSE 758    
//$FILENAME aggregate.src
#define RSRC_FUNCLET_AGGREGATE 759    
//$FILENAME drm_asd_getlicenseaggregatedata.src
#define RSRC_FUNCLET_DRM_ASD_GETLICENSEAGGREGATEDATA 760    
//$FILENAME _scannodeforattributew.src
#define RSRC_FUNCLET__SCANNODEFORATTRIBUTEW 761    
//$FILENAME _getxmlnodecdataw.src
#define RSRC_FUNCLET__GETXMLNODECDATAW 762    
//$FILENAME drm_ddc_getdevicecertificate.src
#define RSRC_FUNCLET_DRM_DDC_GETDEVICECERTIFICATE 763    
//$FILENAME _createdevicestore.src
#define RSRC_FUNCLET__CREATEDEVICESTORE 764    
//$FILENAME _mapdrmerror.src
#define RSRC_FUNCLET__MAPDRMERROR 765    
//$FILENAME _comparemachineid.src
#define RSRC_FUNCLET__COMPAREMACHINEID 766    
//$FILENAME initmgrcontext.src
#define RSRC_FUNCLET_INITMGRCONTEXT 767    
//$FILENAME drm_mgr_setv2header.src
#define RSRC_FUNCLET_DRM_MGR_SETV2HEADER 768    
//$FILENAME drm_mgr_commit.src
#define RSRC_FUNCLET_DRM_MGR_COMMIT 769    
//$FILENAME drm_mgr_cleanuplicensestore.src
#define RSRC_FUNCLET_DRM_MGR_CLEANUPLICENSESTORE 770    
//$FILENAME januscleandatastore.src
#define RSRC_FUNCLET_JANUSCLEANDATASTORE 771    
//$FILENAME drm_mtr_openid.src
#define RSRC_FUNCLET_DRM_MTR_OPENID 772    
//$FILENAME drm_mtr_incrementcount.src
#define RSRC_FUNCLET_DRM_MTR_INCREMENTCOUNT 773    
//$FILENAME drm_mtr_closecontext.src
#define RSRC_FUNCLET_DRM_MTR_CLOSECONTEXT 774    
//$FILENAME oem_setendoffile.src
#define RSRC_FUNCLET_OEM_SETENDOFFILE 775    
//$FILENAME oem_genrandombytes.src
#define RSRC_FUNCLET_OEM_GENRANDOMBYTES 776    
//$FILENAME oem_getfallbacksigningkey.src
#define RSRC_FUNCLET_OEM_GETFALLBACKSIGNINGKEY 777    
//$FILENAME oem_getdevicecerttemplate.src
#define RSRC_FUNCLET_OEM_GETDEVICECERTTEMPLATE 778    
//$FILENAME oem_setdevicecert.src
#define RSRC_FUNCLET_OEM_SETDEVICECERT 779    
//$FILENAME oem_getclockresetstate.src
#define RSRC_FUNCLET_OEM_GETCLOCKRESETSTATE 780    
//$FILENAME oem_setclockresetstate.src
#define RSRC_FUNCLET_OEM_SETCLOCKRESETSTATE 781    
//$FILENAME oem_getuniqueid.src
#define RSRC_FUNCLET_OEM_GETUNIQUEID 782    
//$FILENAME oem_getdevicecert.src
#define RSRC_FUNCLET_OEM_GETDEVICECERT 783    
//$FILENAME drm_snc_openstore.src
#define RSRC_FUNCLET_DRM_SNC_OPENSTORE 784    
//$FILENAME drm_snc_closestore.src
#define RSRC_FUNCLET_DRM_SNC_CLOSESTORE 785    
//$FILENAME _setkidstoredata.src
#define RSRC_FUNCLET__SETKIDSTOREDATA 786    
//$FILENAME drm_snc_deletekid.src
#define RSRC_FUNCLET_DRM_SNC_DELETEKID 787    
//$FILENAME drm_snc_updatekid.src
#define RSRC_FUNCLET_DRM_SNC_UPDATEKID 788    
//$FILENAME drm_mgr_setv1header.src
#define RSRC_FUNCLET_DRM_MGR_SETV1HEADER 789    
//$FILENAME functiongetvalue_part1.src
#define RSRC_FUNCLET_FUNCTIONGETVALUE_PART1 790    
//$FILENAME functiongetvalue_fn_datediff.src
#define RSRC_FUNCLET_FUNCTIONGETVALUE_FN_DATEDIFF 791    
//$FILENAME _hdscreatenamespacestore.src
#define RSRC_FUNCLET__HDSCREATENAMESPACESTORE 792    
//$FILENAME drm_hds_createstore2.src
#define RSRC_FUNCLET_DRM_HDS_CREATESTORE2 793    
//$FILENAME drm_hds_openstore2.src
#define RSRC_FUNCLET_DRM_HDS_OPENSTORE2 794    
//$FILENAME _hdsprealloc.src
#define RSRC_FUNCLET__HDSPREALLOC 795    
//$FILENAME oem_systemtimetofiletime.src
#define RSRC_FUNCLET_OEM_SYSTEMTIMETOFILETIME 796    
//$FILENAME oem_filetimetosystemtime.src
#define RSRC_FUNCLET_OEM_FILETIMETOSYSTEMTIME 797    
//$FILENAME gendevicecertificate.src
#define RSRC_FUNCLET_GENDEVICECERTIFICATE 798    
//$FILENAME drmcrt_memmove_mem.src
#define RSRC_FUNCLET_DRMCRT_MEMMOVE_MEM 799    
//$FILENAME copyhdsdtore.src
#define RSRC_FUNCLET_COPYHDSDTORE 800    
//$FILENAME generatedevicecert.src
#define RSRC_FUNCLET_GENERATEDEVICECERT 801    
//$FILENAME oem_getdevicedatetime.src
#define RSRC_FUNCLET_OEM_GETDEVICEDATETIME 802    
//$FILENAME drm_mtr_updatedata.src
#define RSRC_FUNCLET_DRM_MTR_UPDATEDATA 803    
//$FILENAME _hdsupdatesrn.src
#define RSRC_FUNCLET__HDSUPDATESRN 804    
//$FILENAME drm_b64_decodew_inplace.src
#define RSRC_FUNCLET_DRM_B64_DECODEW_INPLACE 805    
//$FILENAME _checksecureclock.src
#define RSRC_FUNCLET__CHECKSECURECLOCK 806    
//$FILENAME _preparesecureclockdataforwriting.src
#define RSRC_FUNCLET__PREPARESECURECLOCKDATAFORWRITING 807    
//$FILENAME _preparesecureclockdataforreading.src
#define RSRC_FUNCLET__PREPARESECURECLOCKDATAFORREADING 808    
//$FILENAME drm_sst_closelockedslot.src
#define RSRC_FUNCLET_DRM_SST_CLOSELOCKEDSLOT 809    
//$FILENAME variablesecuretimegetorset.src
#define RSRC_FUNCLET_VARIABLESECURETIMEGETORSET 810    
//$FILENAME strtol.src
#define RSRC_FUNCLET_STRTOL 811    
//$FILENAME mktime.src
#define RSRC_FUNCLET_MKTIME 812    
//$FILENAME gmtime.src
#define RSRC_FUNCLET_GMTIME 813    
//$FILENAME localtime.src
#define RSRC_FUNCLET_LOCALTIME 814    
//$FILENAME _struct_tm_to_systemtime.src
#define RSRC_FUNCLET__STRUCT_TM_TO_SYSTEMTIME 815    
//$FILENAME _systemtime_to_struct_tm.src
#define RSRC_FUNCLET__SYSTEMTIME_TO_STRUCT_TM 816    
//$FILENAME _systemtimetotime_t.src
#define RSRC_FUNCLET__SYSTEMTIMETOTIME_T 817    
//$FILENAME oem_setsystemtime.src
#define RSRC_FUNCLET_OEM_SETSYSTEMTIME 818    
//$FILENAME const_pkcrypto.src
#define RSRC_CONST_PKCRYPTO 819    
//$FILENAME const_y.src
#define RSRC_CONST_Y 820    
//$FILENAME aes_dec_table.src
#define RSRC_AES_DEC_TABLE 821    
//$FILENAME aes_key_table.src
#define RSRC_AES_KEY_TABLE 822    
//$FILENAME aes_enc_table.src
#define RSRC_AES_ENC_TABLE 823    
//$FILENAME device_cert.src
#define RSRC_DEVCERT 824    
//$FILENAME devcert_template.src
#define RSRC_DEVCERT_TEMPLATE 825    
//$FILENAME getbase64decodedkey.src
#define RSRC_FUNCLET_GETBASE64DECODEDKEY 826    
//$FILENAME _initslot.src
#define RSRC_FUNCLET__INITSLOT 827    
//$FILENAME hdsimplcommon.src
#define RSRC_HDSIMPLCOMMON_P 828    
//$FILENAME hdsimpl_p.src
#define RSRC_HDSIMPL_P 829    


//////////////////////////////////////////////////////////////////////////////////
//pkcrypto Related
//////////////////////////////////////////////////////////////////////////////////
//$FILENAME two_adic_inverse.src
#define RSRC_FUNCLET_TWO_ADIC_INVERSE 830    
//$FILENAME mp_shift.src
#define RSRC_FUNCLET_MP_SHIFT 831    
//$FILENAME mp_significant_bit_count.src
#define RSRC_FUNCLET_MP_SIGNIFICANT_BIT_COUNT 832    
//$FILENAME set_immediate.src
#define RSRC_FUNCLET_SET_IMMEDIATE 833    
//$FILENAME multiply_immediate.src
#define RSRC_FUNCLET_MULTIPLY_IMMEDIATE 834    
//$FILENAME multiply.src
#define RSRC_FUNCLET_MULTIPLY 835    
//$FILENAME divide_precondition_1.src
#define RSRC_FUNCLET_DIVIDE_PRECONDITION_1 836    
//$FILENAME divide_immediate.src
#define RSRC_FUNCLET_DIVIDE_IMMEDIATE 837    
//$FILENAME ecaffine_exponentiation_tabular.src
#define RSRC_FUNCLET_ECAFFINE_EXPONENTIATION_TABULAR 838    
//$FILENAME ecaffine_table_construction.src
#define RSRC_FUNCLET_ECAFFINE_TABLE_CONSTRUCTION 839    
//$FILENAME ecproj5_to_ecaffine.src
#define RSRC_FUNCLET_ECPROJ5_TO_ECAFFINE 840    
//$FILENAME ecproj5_set_infinite.src
#define RSRC_FUNCLET_ECPROJ5_SET_INFINITE 841    
//$FILENAME ecaffine_on_curve.src
#define RSRC_FUNCLET_ECAFFINE_ON_CURVE 842    
//$FILENAME ecaffine_addition.src
#define RSRC_FUNCLET_ECAFFINE_ADDITION 843    
//$FILENAME ecaffine_addition_subtraction.src
#define RSRC_FUNCLET_ECAFFINE_ADDITION_SUBTRACTION 844    
//$FILENAME ecaffine_attributes2.src
#define RSRC_FUNCLET_ECAFFINE_ATTRIBUTES2 845    
//$FILENAME kfdesc_initialize.src
#define RSRC_FUNCLET_KFDESC_INITIALIZE 846    
//$FILENAME kimmediate.src
#define RSRC_FUNCLET_KIMMEDIATE 847    
//$FILENAME kprime_immediater.src
#define RSRC_FUNCLET_KPRIME_IMMEDIATER 848    
//$FILENAME kprime_sqrter.src
#define RSRC_FUNCLET_KPRIME_SQRTER 849    
//$FILENAME kinitialize_prime.src
#define RSRC_FUNCLET_KINITIALIZE_PRIME 850    
//$FILENAME mod_lucasuv.src
#define RSRC_FUNCLET_MOD_LUCASUV 851    
//$FILENAME mod_lucas.src
#define RSRC_FUNCLET_MOD_LUCAS 852    
//$FILENAME bucket_multiply.src
#define RSRC_FUNCLET_BUCKET_MULTIPLY 853    
//$FILENAME mod_exp2000.src
#define RSRC_FUNCLET_MOD_EXP2000 854    
//$FILENAME mod_exp.src
#define RSRC_FUNCLET_MOD_EXP 855    
//$FILENAME modmul_choices1.src
#define RSRC_FUNCLET_MODMUL_CHOICES1 856    
//$FILENAME mod_sqrt.src
#define RSRC_FUNCLET_MOD_SQRT 857    
//$FILENAME create_modulus.src
#define RSRC_FUNCLET_CREATE_MODULUS 858    
//$FILENAME from_modular.src
#define RSRC_FUNCLET_FROM_MODULAR 859    
//$FILENAME add_immediate.src
#define RSRC_FUNCLET_ADD_IMMEDIATE 860    
//$FILENAME add_diff.src
#define RSRC_FUNCLET_ADD_DIFF 861    
//$FILENAME add_full.src
#define RSRC_FUNCLET_ADD_FULL 862    
//$FILENAME compare_sum_same.src
#define RSRC_FUNCLET_COMPARE_SUM_SAME 863    
//$FILENAME sub_immediate.src
#define RSRC_FUNCLET_SUB_IMMEDIATE 864    
//$FILENAME mp_initialization.src
#define RSRC_FUNCLET_MP_INITIALIZATION 865    
//$FILENAME new_random_bytes.src
#define RSRC_FUNCLET_NEW_RANDOM_BYTES 866    
//$FILENAME new_random_dword_interval.src
#define RSRC_FUNCLET_NEW_RANDOM_DWORD_INTERVAL 867    
//$FILENAME new_random_digit_interval.src
#define RSRC_FUNCLET_NEW_RANDOM_DIGIT_INTERVAL 868    
//$FILENAME new_random_mod.src
#define RSRC_FUNCLET_NEW_RANDOM_MOD 869    
//$FILENAME new_random_mod_nonzero.src
#define RSRC_FUNCLET_NEW_RANDOM_MOD_NONZERO 870    
//$FILENAME new_random_digits.src
#define RSRC_FUNCLET_NEW_RANDOM_DIGITS 871    
//$FILENAME words_to_ecaffine.src
#define RSRC_FUNCLET_WORDS_TO_ECAFFINE 872    
//$FILENAME ecaffine_to_dwords.src
#define RSRC_FUNCLET_ECAFFINE_TO_DWORDS 873    
//$FILENAME _threadunsafepkinit.src
#define RSRC_FUNCLET__THREADUNSAFEPKINIT 874    
//$FILENAME pkinit.src
#define RSRC_FUNCLET_PKINIT 875    
//$FILENAME drm_pk_genkeypair.src
#define RSRC_FUNCLET_DRM_PK_GENKEYPAIR 876    
//$FILENAME drm_pk_encrypt.src
#define RSRC_FUNCLET_DRM_PK_ENCRYPT 877    
//$FILENAME drm_pk_decrypt.src
#define RSRC_FUNCLET_DRM_PK_DECRYPT 878    
//$FILENAME byte_array_mod_bignum.src
#define RSRC_FUNCLET_BYTE_ARRAY_MOD_BIGNUM 879    
//$FILENAME fe2ipmod.src
#define RSRC_FUNCLET_FE2IPMOD 880    
//$FILENAME drm_pk_sign.src
#define RSRC_FUNCLET_DRM_PK_SIGN 881    
//$FILENAME drm_pk_verify.src
#define RSRC_FUNCLET_DRM_PK_VERIFY 882    
//$FILENAME random_bytes.src
#define RSRC_FUNCLET_RANDOM_BYTES 883    
//$FILENAME mp_gcdex.src
#define RSRC_FUNCLET_MP_GCDEX 884    
//$FILENAME mp_gcdex_split1.src
#define RSRC_FUNCLET_MP_GCDEX_SPLIT1 885    
//$FILENAME pkcrypto_p.src
#define RSRC_PKCRYPTO_P 886    
//$FILENAME pkcrypto_ovl_p.src
#define RSRC_PKCRYPTO_OVL_P 887    
//$FILENAME del_all_warning_line2.src
#define RSRC_WARNING_MSG_ALL_LINE2 888    
//$FILENAME del_all_file_star.src
#define RSRC_FILE_DELETE_STAR 889    
//$FILENAME string_voice_menu_delete_all.src
#define RSRC_STRING_VOICE_MENU_DELETE_ALL 890    
//$FILENAME Funclet_Init5VSense.src
#define RSRC_FUNCLET_INIT5VSENSE 891    
//$FILENAME Funclet_UpdateDCDCDutyCycle.src
#define RSRC_FUNCLET_UPDATEDCDCDUTYCYCLE 892    
//$FILENAME Funclet_changeplayset.src
#define RSRC_FUNCLET_CHANGE_PLAYSET 893    

//$FILENAME Funclet_mmctesterase.src
#define RSRC_FUNCLET_MMC_TEST_ERASE 894    
//$FILENAME Funclet_mmchalinit.src
#define RSRC_FUNCLET_MMC_HALINIT 895    


// Added to allow rechargeable battery configurations to build
//$FILENAME battery_charging.src
#define RSRC_BATTERY_CHARGING 896    
//$FILENAME batterychargecodebank.src
#define RSRC_BATTERY_CHARGE_CODEBANK 897    
//$FILENAME updatevolume.src
#define RSRC_FUNCLET_SYSUPDATEVOLUME 898    
//$FILENAME Funclet_TestSkipCheckDisk.src
#define RSRC_FUNCLET_TESTSKIPCHECKDISK 899    
//$FILENAME Funclet_DisableStmpGeneratedMicBias.src
#define RSRC_FUNCLET_DISABLE_STMP_GENERATED_MICBIAS 900    
//$FILENAME Funclet_EnableStmpGeneratedMicBias.src
#define RSRC_FUNCLET_ENABLE_STMP_GENERATED_MICBIAS 901    
//$FILENAME Funclet_SetSkipCheckDisk.src
#define RSRC_FUNCLET_SETSKIPCHECKDISK 902    
//$FILENAME drm_expr_evaluateexpression_no_more_tokens.src
#define RSRC_FUNCLET_DRM_EXPR_EVALUATEEXPRESSION_NO_MORE_TOKENS 903    
//$FILENAME _iscachedevent.src
#define RSRC_FUNCLET__ISCACHEDEVENT 904    
//$FILENAME setcountedexpirycategory.src
#define RSRC_FUNCLET_SETCOUNTEDEXPIRYCATEGORY 905    
//$FILENAME oem_data.src
#define RSRC_OEM_DATA 906    
//$FILENAME gpk_p.src
#define RSRC_GPK_P 907    
//$FILENAME key_data.src
#define RSRC_KEY_DATA 908    
//$FILENAME string_arial_8_defragmenting_store.src
#define RSRC_DEFRAGMENT_STORE_STRING 909    
//$FILENAME string_working.src
#define RSRC_STRING_WORKING 910    
//$FILENAME Funclet_loadusertime.src
#define RSRC_FUNCLET_LOADUSERTIME 911    
//$FILENAME Funclet_saveusertime.src
#define RSRC_FUNCLET_SAVEUSERTIME 912    

//$FILENAME Funclet_SysLRADCBrownoutInit.src
#define RSRC_FUNCLET_SYSLRADCBROWNOUTINIT 913    
//$FILENAME Funclet_SysBatteryGetLevel.src
#define RSRC_FUNCLET_SYSBATTERYGETLEVEL 914    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Audible ACELP Resources
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME AudibleAcelpDec.src
#define RSRC_AUDIBLE_ACELPDEC_CODE 915    
//$FILENAME AudibleAcelpP.src
#define RSRC_AUDIBLE_ACELP_CODE_P 916    
//$FILENAME AudibleAcelpX.src
#define RSRC_AUDIBLE_ACELP_DATA_X 917    
//$FILENAME AudibleAcelpY.src
#define RSRC_AUDIBLE_ACELP_DATA_Y 918    

//$FILENAME AudibleDecMod.src
#define RSRC_AUDIBLE_DECMOD_CODE 919    
//$FILENAME audiblemp3p.src
#define RSRC_AUDIBLE_MP3P_CODE 920    
//$FILENAME audiblemp3x.src
#define RSRC_AUDIBLE_MP3X_CODE 921    
//$FILENAME audiblemp3y.src
#define RSRC_AUDIBLE_MP3Y_CODE 922    

//$FILENAME audiblemetadata_p.src
#define RSRC_AUDIBLE_METADATA_P 923    
//$FILENAME audiblemetadata_y.src
#define RSRC_AUDIBLE_METADATA_Y 924    
//$FILENAME audiblesongposition_p.src
#define RSRC_AUDIBLE_SONG_POSITION_P 925    
//$FILENAME audibletargetcheck_p.src
#define RSRC_AUDIBLE_TARGET_CHECK_P 926    
//$FILENAME audiblefillfileinfo_p.src
#define RSRC_AUDIBLE_FILLFILEINFO_P 927    
//$FILENAME audibledsa_p.src
#define RSRC_AUDIBLE_DSA_P 928    
//$FILENAME audiblefillmetadata_p.src
#define RSRC_AUDIBLE_FILLMETADATA_P 929    
//$FILENAME audiblemetastrings_p.src
#define RSRC_AUDIBLE_METASTRINGS_P 930    
//$FILENAME aaactivationrecords_p.src
#define RSRC_AUDIBLE_ACTIVATION_P 931    

////////////////////////////////////////////////////////////////////////////////////////////////////
// Effects and SRS Resources
////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME srs_effectsmodules_p.src
#define RSRC_SRS_EFFECTS_MODULES_P 932    
//$FILENAME srs_effectsmodules_x.src
#define RSRC_SRS_EFFECTS_MODULES_X 933    
//$FILENAME wowctrl.src
#define RSRC_WOW_CTRL 934    

//$FILENAME wowmenu.src
#define RSRC_WOW_MENU_CODE_BANK 935    
//$FILENAME string_wowbrightness_menu.src
#define RSRC_STRING_WOW_BRIGHTNESS 936    
//$FILENAME string_wow_menu.src
#define RSRC_STRING_WOW_MENU 937    
//$FILENAME string_wowtrubass_menu.src
#define RSRC_STRING_WOW_TRUBASS 938    
//$FILENAME string_wowvolume_menu.src
#define RSRC_STRING_WOW_VOLUME 939    
//$FILENAME string_wowbrightness_low_menu.src
#define RSRC_STRING_WOW_BRIGHT_LOW 940    
//$FILENAME string_wowbrightness_mid_menu.src
#define RSRC_STRING_WOW_BRIGHT_MID 941    
//$FILENAME string_wowbrightness_high_menu.src
#define RSRC_STRING_WOW_BRIGHT_HIGH 942    
//$FILENAME wow_icon.src
#define RSRC_WOW_ICON 943    

//$FILENAME wow16k.src
#define RSRC_WOW16K 944    
//$FILENAME wow32k.src
#define RSRC_WOW32K 945    
//$FILENAME wow8k.src
#define RSRC_WOW8K 946    
//$FILENAME wow11k.src
#define RSRC_WOW11K 947    
//$FILENAME wow22k.src
#define RSRC_WOW22K 948    
//$FILENAME wow24k.src
#define RSRC_WOW24K 949    
//$FILENAME wow44k.src
#define RSRC_WOW44K 950    
//$FILENAME wow48k.src
#define RSRC_WOW48K 951    

//$FILENAME wow16k_Y.src
#define RSRC_WOW16K_Y 952    
//$FILENAME wow32k_Y.src
#define RSRC_WOW32K_Y 953    
//$FILENAME wow8k_Y.src
#define RSRC_WOW8K_Y 954    
//$FILENAME wow11k_Y.src
#define RSRC_WOW11K_Y 955    
//$FILENAME wow22k_Y.src
#define RSRC_WOW22K_Y 956    
//$FILENAME wow24k_Y.src
#define RSRC_WOW24K_Y 957    
//$FILENAME wow44k_Y.src
#define RSRC_WOW44K_Y 958    
//$FILENAME wow48k_Y.src
#define RSRC_WOW48K_Y 959    

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Audible Section Navigation
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$FILENAME audible_secnav.src
#define RSRC_AUDIBLE_SECNAV_ICON 960    

////////////////////////////////////////////////////////////////////////////////////////////////////
// PLAYLIST3 and Music Library
////////////////////////////////////////////////////////////////////////////////////////////////////

//$FILENAME build_ml.src
#define RSRC_BUILD_ML 961    
//$FILENAME build_ml_warning.src
#define RSRC_BUILD_ML_WARNING 962    
//$FILENAME build_ml_warning2.src
#define RSRC_BUILD_ML_WARNING2 963    
//$FILENAME build_flash1.src
#define RSRC_BUILD_FLASH1 964    
//$FILENAME build_flash2.src
#define RSRC_BUILD_FLASH2 965    
//$FILENAME build_flash3.src
#define RSRC_BUILD_FLASH3 966    
//$FILENAME build_sd1.src
#define RSRC_BUILD_SD1 967    
//$FILENAME build_sd2.src
#define RSRC_BUILD_SD2 968    
//$FILENAME build_sd3.src
#define RSRC_BUILD_SD3 969    
//$FILENAME build_newmusic.src
#define RSRC_BUILD_NEWMUSIC 970    
//$FILENAME sdmd.src
#define RSRC_SDMD_MENU_CODE_BANK 971    
//$FILENAME musiclib_build_function_sec.src
#define RSRC_MUSICLIB_BUILD_FUNCTION_SEC 972    
//$FILENAME musiclib_build_function_flush.src
#define RSRC_MUSICLIB_BUILD_FUNCTION_FLUSH 973    
//$FILENAME MusicLibBuildModule.src
#define RSRC_MUSICLIB_BUILD_MODULE 974    
//$FILENAME MusicLibBuildGenreTable.src
#define RSRC_MUSICLIB_BUILD_GENRE_TABLE 975    
//$FILENAME MusicLibBuildFlushMem.src
#define RSRC_MUSICLIB_BUILD_FLUSH_MEM 976    
//$FILENAME MusicLibBrowseModule.src
#define RSRC_MUSICLIB_BROWSE_MODULE 977    
//$FILENAME MusicLibPlayModule.src
#define RSRC_MUSICLIB_PLAY_MODULE 978    
//$FILENAME MusicLibPlayAllNextModule.src
#define RSRC_MUSICLIB_PLAYALLNEXT_MODULE 979    
//$FILENAME MusicLibPlayAllPrevModule.src
#define RSRC_MUSICLIB_PLAYALLPREV_MODULE 980    
//$FILENAME MusicLibShuffleModule.src
#define RSRC_MUSICLIB_SHUFFLE_MODULE 981    
//$FILENAME Funclet_write_detection_access.src
#define RSRC_FUNCLET_WRITE_DETECTION_ACCESS 982    
//$FILENAME Funclet_LOCAL_GetLvlOneItem_info.src
#define RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO 983    
//$FILENAME Funclet_LOCAL_GetFlashAssociatedList_info.src
#define RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO 984    
//$FILENAME Funclet_ML_ResumePlayState.src
#define RSRC_FUNCLET_ML_RESUMEPLAYSTATE 985    
//$FILENAME Funclet_ML_UpdateOnTheGo.src
#define RSRC_FUNCLET_ML_UPDATE_ON_THE_GO 986    
//$FILENAME Funclet_ML_ChOnTheGo.src
#define RSRC_FUNCLET_ML_CH_ON_THE_GO 987    
//$FILENAME Funclet_ML_LoadOnTheGo.src
#define RSRC_FUNCLET_ML_LOAD_ON_THE_GO 988    
//$FILENAME Funclet_Build_InitandUseCache.src
#define RSRC_FUNCLET_BUILD_INIT_AND_USE_CACHE 989    
//$FILENAME Funclet_Build_FlushAndCloseCache.src
#define RSRC_FUNCLET_BUILD_FLUSH_AND_CLOSE_CACHE 990    
//$FILENAME Funclet_ML_movePointerBackward.src
#define RSRC_FUNCLET_ML_MOVE_POINTER_BACKWARD 991    
//$FILENAME Funclet_ML_findLastItemPointer.src
#define RSRC_FUNCLET_ML_FIND_LAST_ITEM_POINTER 992    
//$FILENAME Funclet_ML_startShuffle.src
#define RSRC_FUNCLET_ML_START_SHUFFLE 993    
//$FILENAME Funclet_ML_GetFlashDirAssocListId.src
#define RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ID 994    
//$FILENAME Funclet_ML_GetFlashDirAssocListRootId.src
#define RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ROOT_ID 995    
//$FILENAME Funclet_ML_GetFlashDirAssocListSize.src
#define RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_SIZE 996    
//$FILENAME Funclet_ML_GetIdByStateAndPosition.src
#define RSRC_FUNCLET_ML_GET_ID_BY_STATE_AND_POSITION 997    
//$FILENAME Funclet_ML_StartPlayDirSongInit.src
#define RSRC_FUNCLET_ML_START_PLAY_DIR_SONG_INIT 998    
//$FILENAME Funclet_ML_ShuffleNextDirSong.src
#define RSRC_FUNCLET_ML_SHUFFLE_NEXT_DIR_SONG 999    
//$FILENAME Funclet_ML_BrowseMenu.src
#define RSRC_FUNCLET_ML_BROWSE_MENU 1000    
//$FILENAME Funclet_ML_StartPlayAllSong.src
#define RSRC_FUNCLET_ML_START_PLAY_ALL_SONG 1001    
//$FILENAME Funclet_ML_PlayAllNextSong.src
#define RSRC_FUNCLET_ML_PLAY_ALL_NEXT_SONG 1002    
//$FILENAME Funclet_ML_PlayAllPreviousSong.src
#define RSRC_FUNCLET_ML_PLAY_ALL_PREVIOUS_SONG 1003    
//$FILENAME Funclet_ML_PlayPreviousDirSong.src
#define RSRC_FUNCLET_ML_PLAY_PREVIOUS_DIR_SONG 1004    
//$FILENAME Funclet_ML_ShuffleNextSong.src
#define RSRC_FUNCLET_ML_SHUFFLE_NEXT_SONG 1005    
//$FILENAME Funclet_ML_PlayNextSong.src
#define RSRC_FUNCLET_ML_PLAY_NEXT_SONG 1006    
//$FILENAME Funclet_ML_PlayPreviousSong.src
#define RSRC_FUNCLET_ML_PLAY_PREVIOUS_SONG 1007    
//$FILENAME Funclet_BuildVoiceFilePath.src
#define RSRC_FUNCLET_BUILDVOICEFILEPATH 1008    
//$FILENAME Funclet_BuildFMFilePath.src
#define RSRC_FUNCLET_BUILDFMFILEPATH 1009    
//$FILENAME Funclet_BuildLINEINFilePath.src
#define RSRC_FUNCLET_BUILDLINEINFILEPATH 1010    
//$FILENAME Funclet_GetVoiceFastKey.src
#define RSRC_FUNCLET_GETVOICEFASTKEY 1011    
//$FILENAME MusicLibVoiceApp.src
#define RSRC_MUSICLIB_VOICE_APP 1012    
//$FILENAME MusicLibVoiceModule.src
#define RSRC_MUSICLIB_VOICE_MODULE 1013    
//$FILENAME MusicLibVoicePlayModule.src
#define RSRC_MUSICLIB_VOICEPLAY_MODULE 1014    
//$FILENAME MusicLibMergeFunction.src
#define RSRC_MUSICLIB_MERGE_FUNCTION 1015    
//$FILENAME MusicLibMergeModule.src
#define RSRC_MUSICLIB_MERGE_MODULE 1016    
//$FILENAME MusicLibBrowseApp.src
#define RSRC_MUSICLIB_BROWSE_APP 1017    
//$FILENAME playmusicmenu.src
#define RSRC_PLAYMUSIC_MENU_CODE_BANK 1018    
//$FILENAME browsemenu.src
#define RSRC_BROWSE_MENU_CODE_BANK 1019    
//$FILENAME browsemenu_extra.src
#define RSRC_BROWSE_MENU_EXTRA_CODE_BANK 1020    
//$FILENAME string_play_all.src
#define RSRC_STRING_PLAY_ALL 1021    
//$FILENAME string_play.src
#define RSRC_STRING_PLAY 1022    
//$FILENAME string_unknown_year.src
#define RSRC_STRING_UNKNOWN_YEAR 1023    
//$FILENAME string_year_width.src
#define RSRC_STRING_YEAR_WIDTH 1024    
//$FILENAME string_artist.src
#define RSRC_STRING_ARTIST 1025    
//$FILENAME string_songs.src
#define RSRC_STRING_SONGS 1026    
//$FILENAME string_on_the_fly.src
#define RSRC_STRING_ON_THE_FLY 1027    
//$FILENAME string_new_music.src
#define RSRC_STRING_NEW_MUSIC_MENU 1028    
//$FILENAME string_genre.src
#define RSRC_STRING_GENRE 1029    
//$FILENAME string_year.src
#define RSRC_STRING_YEAR 1030    
//$FILENAME string_playlist.src
#define RSRC_STRING_PLAYLIST 1031    
//$FILENAME string_fm_rec.src
#define RSRC_STRING_FM_REC 1032    
//$FILENAME string_linein_rec.src
#define RSRC_STRING_LINEIN_REC 1033    
//$FILENAME string_play_music.src
#define RSRC_STRING_PLAY_MUSIC 1034    
//$FILENAME highlight_back.src
#define RSRC_HIGHLIGHT_BACK 1035    
//$FILENAME newmusicmenu.src
#define RSRC_NEW_MUSIC_MENU_CODE_BANK 1036    
//$FILENAME string_1_day.src
#define RSRC_STRING_1_DAY 1037    
//$FILENAME string_1_week.src
#define RSRC_STRING_1_WEEK 1038    
//$FILENAME string_1_month.src
#define RSRC_STRING_1_MONTH 1039    
//$FILENAME on_the_fly_full.src
#define RSRC_LIST_FULL_MESSAGE 1040    
//$FILENAME on_the_fly_free1.src
#define RSRC_LIST_FREE_MESSAGE 1041    
//$FILENAME on_the_fly_free2.src
#define RSRC_LIST_FREE_MESSAGE2 1042    
//$FILENAME on_the_fly_delete1.src
#define RSRC_LIST_DELETE_MESSAGE 1043    
//$FILENAME on_the_fly_delete2.src
#define RSRC_LIST_DELETE_MESSAGE2 1044    
//$FILENAME empty_favourite.src
#define RSRC_EMPTY_FAVOURITE 1045    
//$FILENAME sd_remove.src
#define RSRC_SD_REMOVE 1046    
//$FILENAME sd_insert.src
#define RSRC_SD_INSERT 1047    
//$FILENAME check_disk_1.src
#define RSRC_CHECK_DISK_1 1048    
//$FILENAME check_disk_2.src
#define RSRC_CHECK_DISK_2 1049    
//$FILENAME check_disk_3.src
#define RSRC_CHECK_DISK_3 1050    
//$FILENAME flash_error.src
#define RSRC_FLASH_ERROR 1051    

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// STFM1000 Tuner funclet
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//$FILENAME Funclet_TunerDriverInit.src
#define RSRC_FUNCLET_TUNER_DRIVER_INIT 1052    
//$FILENAME Funclet_TunerDriverIncUp.src
#define RSRC_FUNCLET_TUNER_DRIVER_INC_UP 1053    
//$FILENAME Funclet_TunerDriverIncDown.src
#define RSRC_FUNCLET_TUNER_DRIVER_INC_DOWN 1054    
//$FILENAME Funclet_TunerDriverStandyby.src
#define RSRC_FUNCLET_TUNER_DRIVER_SET_STANDBY 1055    
//$FILENAME Funclet_TunerDriverReplacePreset.src
#define RSRC_FUNCLET_TUNER_DRIVER_REPLACE_PRESET 1056    
//$FILENAME Funclet_TunerDriverErasePreset.src
#define RSRC_FUNCLET_TUNER_DRIVER_ERASE_PRESET 1057    
//$FILENAME Funclet_TunerDriverTuneToPreset.src
#define RSRC_FUNCLET_TUNER_DRIVER_TUNE_TO_PRESET 1058    
//$FILENAME Funclet_TunerDriverSearchDown.src
#define RSRC_FUNCLET_TUNER_DRIVER_SEARCH_DOWN 1059    
//$FILENAME Funclet_TunerSTFM1000SafePowerDown.src
#define RSRC_FUNCLET_TUNER_SAFE_POWER_DOWN 1060    
//$FILENAME Funclet_TunerSTFM1000OptimizeChannel.src
#define RSRC_FUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL 1061    
//$FILENAME decstfmmod.src
#define RSRC_DEC_STFM_MOD_CODE 1062    
//$FILENAME dec_stfmp.src
#define RSRC_DEC_STFMP_CODE 1063    
//$FILENAME dec_stfmx.src
#define RSRC_DEC_STFMX_CODE 1064    
//$FILENAME dec_stfmy.src
#define RSRC_DEC_STFMY_CODE 1065    
//$FILENAME Funclet_TunerSTFM1000I2CInit.src
#define RSRC_FUNCLET_STFM1000_I2C_INIT 1066    
//$FILENAME Funclet_TunerDriverScanStations.src
#define RSRC_FUNCLET_TUNER_DRIVER_SCAN_STATION 1067    
//$FILENAME Funclet_TunerDriverSetMonoStereoMode.src
#define RSRC_FUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE 1068    
//$FILENAME Funclet_STFM1000TestPresence.src
#define RSRC_FUNCLET_STFM1000_TEST_PRESENCE 1069    
//$FILENAME Funclet_I2CWriteTunerRegister.src
#define RSRC_FUNCLET_I2C_WRITE_TUNER_REGISTER 1070    
//$FILENAME Funclet_I2CReset.src
#define RSRC_FUNCLET_I2C_RESET 1071    
//$FILENAME fmtunersecondrsrc.src
#define RSRC_FM_TUNER_SECOND_RSRC 1072    
//$FILENAME Funclet_TunerSTFM1000RegPowerUp1.src
#define RSRC_FUNCLET_STFM1000_REG_POWER_UP1 1073    
//$FILENAME Funclet_TunerSTFM1000RegPowerUp2.src
#define RSRC_FUNCLET_STFM1000_REG_POWER_UP2 1074    
//$FILENAME Funclet_TunerSTFM1000FindRssiThreshold.src
#define RSRC_FUNCLET_STFM1000_FIND_RSSI_THRESHOLD 1075    
//$FILENAME Funclet_TunerSTFM1000DataPathOff.src
#define RSRC_FUNCLET_STFM1000_DATA_PATH_OFF 1076    
//$FILENAME Funclet_TunerSTFM1000ContinueSearching.src
#define RSRC_FUNCLET_STFM1000_CONTINUE_SEARCHING 1077    
//RSRC_FUNCLET_STFM1000_SET_LNA_BIAS    equ      975    ;$FILENAME Funclet_STFM1000SetLnaBias.src
//RSRC_FUNCLET_STFM1000_LOOK_FOR_PILOT    equ      976    ;$FILENAME Funclet_STFM1000LookForPilot.src
//$FILENAME Funclet_STFM1000TunerSearchEnded.src
#define RSRC_FUNCLET_STFM1000_TUNER_SEARCH_ENDED 1078    
// for RestoreDriveFromBackup
//$FILENAME restoresysdrive.src
#define RSRC_RESTORESYSDRIVE_P_CODE 1079    

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// Playlist5 sources
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//$FILENAME playlist5_browsemenu.src
#define RSRC_PL5BROWSE_MENU_CODEBANK 1080    
//$FILENAME playlist5_browsemenu_extra.src
#define RSRC_PL5_BROWSEMENU_EXTRA 1081    
//$FILENAME playlist5_browse_submenu.src
#define RSRC_PL5BROWSE_SUBMENU_CODEBANK 1082    
//$FILENAME playlist5_playback_module.src
#define RSRC_PLAYLIST5_PLAYBACK_MODULE 1083    
//$FILENAME playlist5_browse_module.src
#define RSRC_PLAYLIST5_BROWSE_MODULE 1084    

//$FILENAME Funclet_Get_LRCDirName_PL5.src
#define RSRC_FUNCLET_GET_LRC_DIR_NAME_PL5 1085    
//$FILENAME Funclet_PL5_PL_QueuePushBack.src   
#define RSRC_FUNCLET_PL5_PL_QUEUEPUSHBACK 1086    
//$FILENAME Funclet_PL5_PL_QueuePushFront.src 
#define RSRC_FUNCLET_PL5_PL_QUEUEPUSHFRONT 1087    

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// DanhNguyen added bitmaps
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//$FILENAME icon_folder.src
#define RSRC_ICON_FOLDER 1088    
//$FILENAME icon_song.src
#define RSRC_ICON_SONG 1089    

//$FILENAME menu_music.src
#define RSRC_MENU_MUSIC 1090    
//$FILENAME vie_menu_music.src
#define RSRC_VIE_MENU_MUSIC 1091    

//$FILENAME menu_voice.src
#define RSRC_MENU_VOICE 1092    
//$FILENAME vie_menu_voice.src
#define RSRC_VIE_MENU_VOICE 1093    

//$FILENAME menu_fmtuner.src
#define RSRC_MENU_FMTUNER 1094    
//$FILENAME vie_menu_fmtuner.src
#define RSRC_VIE_MENU_FMTUNER 1095    

//$FILENAME menu_record.src
#define RSRC_MENU_RECORD 1096    
//$FILENAME vie_menu_record.src
#define RSRC_VIE_MENU_RECORD 1097    

//$FILENAME menu_settings.src
#define RSRC_MENU_SETTINGS 1098    
//$FILENAME vie_menu_settings.src
#define RSRC_VIE_MENU_SETTINGS 1099    

//$FILENAME menu_shutdown.src
#define RSRC_MENU_SHUTDOWN 1100    
//$FILENAME vie_menu_shutdown.src
#define RSRC_VIE_MENU_SHUTDOWN 1101    

//$FILENAME menu_clock.src
#define RSRC_MENU_CLOCK 1102    
//$FILENAME vie_menu_clock.src
#define RSRC_VIE_MENU_CLOCK 1103    

//$FILENAME menu_ab.src
#define RSRC_MENU_AB 1104    
//$FILENAME vie_menu_ab.src
#define RSRC_VIE_MENU_AB 1105    

//$FILENAME menu_delete.src
#define RSRC_MENU_DELETE 1106    
//$FILENAME vie_menu_delete.src
#define RSRC_VIE_MENU_DELETE 1107    

//$FILENAME menu_about.src
#define RSRC_MENU_ABOUT 1108    
//$FILENAME vie_menu_about.src
#define RSRC_VIE_MENU_ABOUT 1109    

//$FILENAME menu_exit.src
#define RSRC_MENU_EXIT 1110    
//$FILENAME vie_menu_exit.src
#define RSRC_VIE_MENU_EXIT 1111    

//$FILENAME music_play_all.src
#define RSRC_MUSIC_PLAY_ALL 1112    
//$FILENAME vie_music_play_all.src
#define RSRC_VIE_MUSIC_PLAY_ALL 1113    

//$FILENAME music_folder_internal.src
#define RSRC_MUSIC_FOLDER_INTERNAL 1114    
//$FILENAME vie_music_folder_internal.src
#define RSRC_VIE_MUSIC_FOLDER_INTERNAL 1115    

//$FILENAME music_folder_external.src
#define RSRC_MUSIC_FOLDER_EXTERNAL 1116    
//$FILENAME vie_music_folder_external.src
#define RSRC_VIE_MUSIC_FOLDER_EXTERNAL 1117    

//$FILENAME music_songs.src
#define RSRC_MUSIC_SONGS 1118    
//$FILENAME vie_music_songs.src
#define RSRC_VIE_MUSIC_SONGS 1119    

//$FILENAME music_favorites.src
#define RSRC_MUSIC_FAVORITES 1120    
//$FILENAME vie_music_favorites.src
#define RSRC_VIE_MUSIC_FAVORITES 1121    

//$FILENAME music_fm_record.src
#define RSRC_MUSIC_FM_RECORD 1122    
//$FILENAME vie_music_fm_record.src
#define RSRC_VIE_MUSIC_FM_RECORD 1123    

//$FILENAME music_exit.src
#define RSRC_MUSIC_EXIT 1124    
//$FILENAME vie_music_exit.src
#define RSRC_VIE_MUSIC_EXIT 1125    

//$FILENAME browse_music_folder_internal.src
#define RSRC_BROWSE_MUSIC_FOLDER_INTERNAL 1126    
//$FILENAME vie_browse_music_folder_internal.src
#define RSRC_VIE_BROWSE_MUSIC_FOLDER_INTERNAL 1127    

//$FILENAME browse_music_folder_external.src
#define RSRC_BROWSE_MUSIC_FOLDER_EXTERNAL 1128    
//$FILENAME vie_browse_music_folder_external.src
#define RSRC_VIE_BROWSE_MUSIC_FOLDER_EXTERNAL 1129    

//$FILENAME browse_music_list_songs.src
#define RSRC_BROWSE_MUSIC_LIST_SONGS 1130    
//$FILENAME vie_browse_music_list_songs.src
#define RSRC_VIE_BROWSE_MUSIC_LIST_SONGS 1131    

//$FILENAME browse_music_favourites.src
#define RSRC_BROWSE_MUSIC_FAVOURITES 1132    
//$FILENAME vie_browse_music_favourites.src
#define RSRC_VIE_BROWSE_MUSIC_FAVOURITES 1133    

//$FILENAME browse_music_fm_files.src
#define RSRC_BROWSE_MUSIC_FM_FILES 1134    
//$FILENAME vie_browse_music_fm_files.src
#define RSRC_VIE_BROWSE_MUSIC_FM_FILES 1135    

//$FILENAME browse_voice.src
#define RSRC_BROWSE_VOICE 1136    
//$FILENAME vie_browse_voice.src
#define RSRC_VIE_BROWSE_VOICE 1137    

//$FILENAME favourites_list_add.src
#define RSRC_FAVOURITES_LIST_ADD 1138    
//$FILENAME vie_favourites_list_add.src
#define RSRC_VIE_FAVOURITES_LIST_ADD 1139    

//$FILENAME favourites_list_remove.src
#define RSRC_FAVOURITES_LIST_REMOVE 1140    
//$FILENAME vie_favourites_list_remove.src
#define RSRC_VIE_FAVOURITES_LIST_REMOVE 1141    

//$FILENAME favourites_list_is_full.src
#define RSRC_FAVOURITES_LIST_IS_FULL 1142    
//$FILENAME vie_favourites_list_is_full.src
#define RSRC_VIE_FAVOURITES_LIST_IS_FULL 1143    

//$FILENAME about_screen_1.src
#define RSRC_ABOUT_SCREEN_1 1144    
//$FILENAME vie_about_screen_1.src
#define RSRC_VIE_ABOUT_SCREEN_1 1145    

//$FILENAME about_screen_2.src
#define RSRC_ABOUT_SCREEN_2 1146    
//$FILENAME vie_about_screen_2.src
#define RSRC_VIE_ABOUT_SCREEN_2 1147    

//$FILENAME about_screen_3.src
#define RSRC_ABOUT_SCREEN_3 1148    
//$FILENAME vie_about_screen_3.src
#define RSRC_VIE_ABOUT_SCREEN_3 1149    

//$FILENAME about_screen_4.src
#define RSRC_ABOUT_SCREEN_4 1150    
//$FILENAME vie_about_screen_4.src
#define RSRC_VIE_ABOUT_SCREEN_4 1151    

//$FILENAME time_date_exit_title.src
#define RSRC_TIME_DATE_EXIT_TITLE 1152    
//$FILENAME vie_time_date_exit_title.src
#define RSRC_VIE_TIME_DATE_EXIT_TITLE 1153    

//$FILENAME time_clean_desktop.src
#define RSRC_TIME_CLEAN_DESKTOP 1154    
//$FILENAME time_dash.src
#define RSRC_TIME_DASH 1155    

//$FILENAME time_day_7.src
#define RSRC_TIME_DAY_SAT 1156    
//$FILENAME vie_time_day_7.src
#define RSRC_VIE_TIME_DAY_SAT 1157    
//$FILENAME time_day_cn.src
#define RSRC_TIME_DAY_SUN 1158    
//$FILENAME vie_time_day_cn.src
#define RSRC_VIE_TIME_DAY_SUN 1159    
//$FILENAME time_day_2.src
#define RSRC_TIME_DAY_MON 1160    
//$FILENAME vie_time_day_2.src
#define RSRC_VIE_TIME_DAY_MON 1161    
//$FILENAME time_day_3.src
#define RSRC_TIME_DAY_TUE 1162    
//$FILENAME vie_time_day_3.src
#define RSRC_VIE_TIME_DAY_TUE 1163    
//$FILENAME time_day_4.src
#define RSRC_TIME_DAY_WED 1164    
//$FILENAME vie_time_day_4.src
#define RSRC_VIE_TIME_DAY_WED 1165    
//$FILENAME time_day_5.src
#define RSRC_TIME_DAY_THU 1166    
//$FILENAME vie_time_day_5.src
#define RSRC_VIE_TIME_DAY_THU 1167    
//$FILENAME time_day_6.src
#define RSRC_TIME_DAY_FRI 1168    
//$FILENAME vie_time_day_6.src
#define RSRC_VIE_TIME_DAY_FRI 1169    

//$FILENAME time_month_1.src
#define RSRC_TIME_MONTH_1 1170    
//$FILENAME vie_time_month_1.src
#define RSRC_VIE_TIME_MONTH_1 1171    
//$FILENAME time_month_2.src
#define RSRC_TIME_MONTH_2 1172    
//$FILENAME vie_time_month_2.src
#define RSRC_VIE_TIME_MONTH_2 1173    
//$FILENAME time_month_3.src
#define RSRC_TIME_MONTH_3 1174    
//$FILENAME vie_time_month_3.src
#define RSRC_VIE_TIME_MONTH_3 1175    
//$FILENAME time_month_4.src
#define RSRC_TIME_MONTH_4 1176    
//$FILENAME vie_time_month_4.src
#define RSRC_VIE_TIME_MONTH_4 1177    
//$FILENAME time_month_5.src
#define RSRC_TIME_MONTH_5 1178    
//$FILENAME vie_time_month_5.src
#define RSRC_VIE_TIME_MONTH_5 1179    
//$FILENAME time_month_6.src
#define RSRC_TIME_MONTH_6 1180    
//$FILENAME vie_time_month_6.src
#define RSRC_VIE_TIME_MONTH_6 1181    
//$FILENAME time_month_7.src
#define RSRC_TIME_MONTH_7 1182    
//$FILENAME vie_time_month_7.src
#define RSRC_VIE_TIME_MONTH_7 1183    
//$FILENAME time_month_8.src
#define RSRC_TIME_MONTH_8 1184    
//$FILENAME vie_time_month_8.src
#define RSRC_VIE_TIME_MONTH_8 1185    
//$FILENAME time_month_9.src
#define RSRC_TIME_MONTH_9 1186    
//$FILENAME vie_time_month_9.src
#define RSRC_VIE_TIME_MONTH_9 1187    
//$FILENAME time_month_10.src
#define RSRC_TIME_MONTH_10 1188    
//$FILENAME vie_time_month_10.src
#define RSRC_VIE_TIME_MONTH_10 1189    
//$FILENAME time_month_11.src
#define RSRC_TIME_MONTH_11 1190    
//$FILENAME vie_time_month_11.src
#define RSRC_VIE_TIME_MONTH_11 1191    
//$FILENAME time_month_12.src
#define RSRC_TIME_MONTH_12 1192    
//$FILENAME vie_time_month_12.src
#define RSRC_VIE_TIME_MONTH_12 1193    

//$FILENAME time_num_am.src
#define RSRC_TIME_NUM_AM 1194    
//$FILENAME time_num_am.src
#define RSRC_TIME_NUM_PM 1195    
//$FILENAME settime_format_12h.src
#define RSRC_SETTIME_FORMAT_12H 1196    
//$FILENAME settime_format_24h.src
#define RSRC_SETTIME_FORMAT_24H 1197    
//$FILENAME setdate_format_dmy.src
#define RSRC_SETDATE_FORMAT_DMY 1198    
//$FILENAME setdate_format_mdy.src
#define RSRC_SETDATE_FORMAT_MDY 1199    
//$FILENAME setdate_format_ymd.src
#define RSRC_SETDATE_FORMAT_YMD 1200    

//$FILENAME time_num_large_0.src
#define RSRC_TIME_NUM_LARGE_0 1201    
//$FILENAME time_num_large_1.src
#define RSRC_TIME_NUM_LARGE_1 1202    
//$FILENAME time_num_large_2.src
#define RSRC_TIME_NUM_LARGE_2 1203    
//$FILENAME time_num_large_3.src
#define RSRC_TIME_NUM_LARGE_3 1204    
//$FILENAME time_num_large_4.src
#define RSRC_TIME_NUM_LARGE_4 1205    
//$FILENAME time_num_large_5.src
#define RSRC_TIME_NUM_LARGE_5 1206    
//$FILENAME time_num_large_6.src
#define RSRC_TIME_NUM_LARGE_6 1207    
//$FILENAME time_num_large_7.src
#define RSRC_TIME_NUM_LARGE_7 1208    
//$FILENAME time_num_large_8.src
#define RSRC_TIME_NUM_LARGE_8 1209    
//$FILENAME time_num_large_9.src
#define RSRC_TIME_NUM_LARGE_9 1210    

//$FILENAME time_num_medium_0.src
#define RSRC_TIME_NUM_MEDIUM_0 1211    
//$FILENAME time_num_medium_1.src
#define RSRC_TIME_NUM_MEDIUM_1 1212    
//$FILENAME time_num_medium_2.src
#define RSRC_TIME_NUM_MEDIUM_2 1213    
//$FILENAME time_num_medium_3.src
#define RSRC_TIME_NUM_MEDIUM_3 1214    
//$FILENAME time_num_medium_4.src
#define RSRC_TIME_NUM_MEDIUM_4 1215    
//$FILENAME time_num_medium_5.src
#define RSRC_TIME_NUM_MEDIUM_5 1216    
//$FILENAME time_num_medium_6.src
#define RSRC_TIME_NUM_MEDIUM_6 1217    
//$FILENAME time_num_medium_7.src
#define RSRC_TIME_NUM_MEDIUM_7 1218    
//$FILENAME time_num_medium_8.src
#define RSRC_TIME_NUM_MEDIUM_8 1219    
//$FILENAME time_num_medium_9.src
#define RSRC_TIME_NUM_MEDIUM_9 1220    

//$FILENAME time_colon.src
#define RSRC_TIME_COLON 1221    

//$FILENAME settings_backlight_title.src
#define RSRC_SETTINGS_BACKLIGHT_TITLE 1222    
//$FILENAME settings_auto_shutdown_title.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_TITLE 1223    
//$FILENAME settings_playmode_title.src
#define RSRC_SETTINGS_PLAYMODE_TITLE 1224    

//$FILENAME settings_contrast_title.src
#define RSRC_SETTINGS_CONTRAST_TITLE 1225    
//$FILENAME vie_settings_contrast_title.src
#define RSRC_VIE_SETTINGS_CONTRAST_TITLE 1226    

//$FILENAME settings_eq_title.src
#define RSRC_SETTINGS_EQ_TITLE 1227    
//$FILENAME vie_settings_eq_title.src
#define RSRC_VIE_SETTINGS_EQ_TITLE 1228    

//$FILENAME settings_exit_title.src
#define RSRC_SETTINGS_EXIT_TITLE 1229    
//$FILENAME vie_settings_exit_title.src
#define RSRC_VIE_SETTINGS_EXIT_TITLE 1230    

//$FILENAME settings_set_date_title.src
#define RSRC_SETTINGS_SET_DATE_TITLE 1231    
//$FILENAME vie_settings_set_date_title.src
#define RSRC_VIE_SETTINGS_SET_DATE_TITLE 1232    

//$FILENAME settings_set_time_title.src
#define RSRC_SETTINGS_SET_TIME_TITLE 1233    
//$FILENAME vie_settings_set_time_title.src
#define RSRC_VIE_SETTINGS_SET_TIME_TITLE 1234    

//$FILENAME settings_playmode_normal.src
#define RSRC_SETTINGS_PLAYMODE_NORMAL 1235    
//$FILENAME vie_settings_playmode_normal.src
#define RSRC_VIE_SETTINGS_PLAYMODE_NORMAL 1236    

//$FILENAME settings_playmode_repeatone.src
#define RSRC_SETTINGS_PLAYMODE_REPEATONE 1237    
//$FILENAME vie_settings_playmode_repeatone.src
#define RSRC_VIE_SETTINGS_PLAYMODE_REPEATONE 1238    

//$FILENAME settings_playmode_repeatall.src
#define RSRC_SETTINGS_PLAYMODE_REPEATALL 1239    
//$FILENAME vie_settings_playmode_repeatall.src
#define RSRC_VIE_SETTINGS_PLAYMODE_REPEATALL 1240    

//$FILENAME settings_playmode_shuffle.src
#define RSRC_SETTINGS_PLAYMODE_SHUFFLE 1241    
//$FILENAME vie_settings_playmode_shuffle.src
#define RSRC_VIE_SETTINGS_PLAYMODE_SHUFFLE 1242    

//$FILENAME settings_playmode_shuffle_repeat.src
#define RSRC_SETTINGS_PLAYMODE_SHUFFLE_REPEAT 1243    
//$FILENAME vie_settings_playmode_shuffle_repeat.src
#define RSRC_VIE_SETTINGS_PLAYMODE_SHUFFLE_REPEAT 1244    

//$FILENAME settings_backlight_on.src
#define RSRC_SETTINGS_BACKLIGHT_ON 1245    
//$FILENAME vie_settings_backlight_on.src
#define RSRC_VIE_SETTINGS_BACKLIGHT_ON 1246    

//$FILENAME settings_backlight_10s.src
#define RSRC_SETTINGS_BACKLIGHT_10S 1247    
//$FILENAME vie_settings_backlight_10s.src
#define RSRC_VIE_SETTINGS_BACKLIGHT_10S 1248    

//$FILENAME settings_backlight_20s.src
#define RSRC_SETTINGS_BACKLIGHT_20S 1249    
//$FILENAME vie_settings_backlight_20s.src
#define RSRC_VIE_SETTINGS_BACKLIGHT_20s 1250    

//$FILENAME settings_backlight_30s.src
#define RSRC_SETTINGS_BACKLIGHT_30S 1251    
//$FILENAME vie_settings_backlight_30s.src
#define RSRC_VIE_SETTINGS_BACKLIGHT_30S 1252    

//$FILENAME settings_auto_shutdown_disable.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_DISABLE 1253    
//$FILENAME vie_settings_auto_shutdown_disable.src
#define RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_DISABLE 1254    

//$FILENAME settings_auto_shutdown_1minute.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_1MINUTE 1255    
//$FILENAME vie_settings_auto_shutdown_1min.src
#define RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_1MINUTE 1256    

//$FILENAME settings_auto_shutdown_2minutes.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_2MINUTES 1257    
//$FILENAME vie_settings_auto_shutdown_2min.src
#define RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_2MINUTES 1258    

//$FILENAME settings_auto_shutdown_5minutes.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_5MINUTES 1259    
//$FILENAME vie_settings_auto_shutdown_5min.src
#define RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_5MINUTES 1260    

//$FILENAME settings_auto_shutdown_10minutes.src
#define RSRC_SETTINGS_AUTO_SHUTDOWN_10MINUTES 1261    
//$FILENAME vie_settings_auto_shutdown_10min.src
#define RSRC_VIE_SETTINGS_AUTO_SHUTDOWN_10MINUTES 1262    

//$FILENAME settings_languages_eng.src
#define RSRC_SETTINGS_LANGUAGES_ENG 1263    
//$FILENAME settings_languages_vie.src
#define RSRC_SETTINGS_LANGUAGES_VIE 1264    

//$FILENAME fraction_dot.src
#define RSRC_FRACTION_DOT 1265    

//$FILENAME fm_background.src
#define RSRC_FM_BACKGROUND 1266    
//$FILENAME vie_fm_background.src
#define RSRC_VIE_FM_BACKGROUND 1267    

//$FILENAME searching_please_wait.src
#define RSRC_SEARCHING_PLEASE_WAIT 1268    
//$FILENAME vie_searching_please_wait.src
#define RSRC_VIE_SEARCHING_PLEASE_WAIT 1269    

//$FILENAME fm_auto_search.src
#define RSRC_FM_AUTO_SEARCH 1270    
//$FILENAME vie_fm_auto_search.src
#define RSRC_VIE_FM_AUTO_SEARCH 1271    

//$FILENAME jvj_shutdown_player.src
#define RSRC_JVJ_SHUTDOWN_PLAYER 1272    
//$FILENAME vie_jvj_shutdown_player.src
#define RSRC_VIE_JVJ_SHUTDOWN_PLAYER 1273    

#endif //IF (!@def(resources))

