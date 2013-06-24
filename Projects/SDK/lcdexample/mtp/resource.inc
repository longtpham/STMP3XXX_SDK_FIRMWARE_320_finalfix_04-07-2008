;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Thu Aug 31 17:00:16 2006 using resource.inc as input.



;///////////////////////////////////////////////////////////////////////////////
;//
;// Filename: resource.inc
;// Description: The resources for the mtp project.
;//
;//
;// Copyright (c) SigmaTel, Inc. Unpublished
;//
;// SigmaTel, Inc.
;// Proprietary  Confidential
;//
;// This source code and the algorithms implemented therein constitute
;// confidential information and may comprise trade secrets of SigmaTel, Inc.
;// or its associates, and any use thereof is subject to the terms and
;// conditions of the Confidential Disclosure Agreement pursuant to which this
;// source code was originally received.
;//
;///////////////////////////////////////////////////////////////////////////////
  IF (!@def(resources))
resources equ 1
;///////////////////////////////////////////////////////////////////////////////
;  version number                   no leading zeros here!
;///////////////////////////////////////////////////////////////////////////////

VERSION_MAJOR           equ         3
VERSION_MIDDLE          equ         200
VERSION_MINOR           equ         910

LCD_SEG_OFFSET          equ         0       ;Set to 34 if right side up


;///////////////////////////////////////////////////////////////////////////////
; High usage resources (placed at start to make sure that they are cached.)
; Resources in this block account for about 94% of all resource loads during a typical
; mtp run in PlaysForSure. The other most of the other 6% come from files that
; appended to this one at build time.
;///////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_SEARCHDIRECTORY    equ        1    ;$FILENAME searchdirectory.src
RSRC_FUNCLET_MATCHDIRRECORDW    equ        2    ;$FILENAME matchdirrecordw.src
RSRC_FUNCLET_LONGDIRMATCH    equ        3    ;$FILENAME longdirmatch.src
RSRC_FUNCLET_SHORTDIRMATCH    equ        4    ;$FILENAME shortdirmatch.src
RSRC_FUNCLET_MTPGETLITTLEENDIANINT32INX    equ        5    ;$FILENAME mtpgetlittleendianint32inx.src
RSRC_FUNCLET_MTPGETLITTLEENDIANINT16INX    equ        6    ;$FILENAME mtpgetlittleendianint16inx.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODSUCS3    equ        7    ;$FILENAME discardtrailigperiodsucs3.src
RSRC_FUNCLET_CHANGECASE    equ        8    ;$FILENAME changecase.src
RSRC_FUNCLET_COPY_XPTPSTRING    equ        9    ;$FILENAME copy_xptpstring.src
RSRC_TASK_UI_CODEBANK    equ       10    ;$FILENAME rsrc_task_ui_codebank.src
RSRC_DATABASE_ACCESS    equ       11    ;$FILENAME database_access.src
RSRC_FUNCLET_CHDIR      equ       12    ;$FILENAME chdir.src
RSRC_FUNCLET_SETHANDLEFORSEARCH    equ       13    ;$FILENAME sethandleforsearch.src
RSRC_FUNCLET_STRLENGTH    equ       14    ;$FILENAME strlength.src
RSRC_FUNCLET_FOPENW     equ       15    ;$FILENAME fopenw.src
RSRC_FUNCLET_CHANGEPATH    equ       16    ;$FILENAME changepath.src
RSRC_FUNCLET_CREATEDIRRECORD    equ       17    ;$FILENAME createdirrecord.src
RSRC_FUNCLET_FOPEN      equ       18    ;$FILENAME fopen.src
RSRC_FUNCLET_FREMOVE    equ       19    ;$FILENAME fremove.src
RSRC_FUNCLET_UPPERCASE    equ       20    ;$FILENAME uppercase.src
RSRC_FUNCLET_FCLOSE     equ       21    ;$FILENAME fclose.src
RSRC_FUNCLET_SETSHORTFILENAME    equ       22    ;$FILENAME setshortfilename.src
RSRC_FUNCLET_CHANGETOROOTDIRECTORY    equ       23    ;$FILENAME changetorootdirectory.src
RSRC_FUNCLET_SEARCHFREEHANDLEALLOCATE    equ       24    ;$FILENAME searchfreehandleallocate.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODS    equ       25    ;$FILENAME discardtrailigperiods.src
RSRC_FUNCLET_CONVERTOSHORTNAME    equ       26    ;$FILENAME ConverToShortname.src
RSRC_FUNCLET_UPDATEHANDLEMODE    equ       27    ;$FILENAME updatehandlemode.src
RSRC_FUNCLET_GETNAME    equ       28    ;$FILENAME getname.src
RSRC_FUNCLET_STRCPY     equ       29    ;$FILENAME strcpy.src
RSRC_FUNCLET_ARRANGEFILENAME    equ       30    ;$FILENAME arrangefilename.src
RSRC_FUNCLET_FASTOPEN    equ       31    ;$FILENAME fastopen.src
RSRC_FUNCLET_GETSHORTFILENAME    equ       32    ;$FILENAME GetShortfilename.src
RSRC_FUNCLET_EXTRACTFILENAME    equ       33    ;$FILENAME extractfilename.src
RSRC_FUNCLET_STOR_OPENFRIT    equ       34    ;$FILENAME stor_openfrit.src
RSRC_FUNCLET_FCREATE    equ       35    ;$FILENAME fcreate.src
RSRC_FUNCLET_EXTRACTFILENAMEW    equ       36    ;$FILENAME extractfilenamew.src
RSRC_FUNCLET_GETNAMEW    equ       37    ;$FILENAME getnamew.src
RSRC_FUNCLET_STRLENGTHW    equ       38    ;$FILENAME strlengthw.src
RSRC_FUNCLET_STOR_ISOBJECTVALID    equ       39    ;$FILENAME stor_isobjectvalid.src
RSRC_FUNCLET_STOR_DELETEOBJECTFROMINDEXTABLE    equ       40    ;$FILENAME stor_deleteobjectfromindextable.src
RSRC_HIGH_SPEED         equ       41    ;$FILENAME highspeed.src
RSRC_FUNCLET_DELETECONTENT    equ       42    ;$FILENAME deletecontent.src
RSRC_FUNCLET_DELETERECORD    equ       43    ;$FILENAME deleterecord.src
RSRC_FUNCLET_CHECKSPACEINROOTDIR    equ       44    ;$FILENAME checkspaceinrootdir.src
RSRC_FUNCLET_FGETFASTHANDLE    equ       45    ;$FILENAME fgetfasthandle.src
RSRC_FUNCLET_ISFILEOPEN    equ       46    ;$FILENAME isfileopen.src
RSRC_FUNCLET_ARRANGELONGFILENAME    equ       47    ;$FILENAME arrangelongfilename.src
RSRC_FUNCLET_CHKSUM     equ       48    ;$FILENAME chksum.src
RSRC_FUNCLET_CREATESHORTDIRRECORD    equ       49    ;$FILENAME createshortdirrecord.src
RSRC_FUNCLET_DISCARDTRAILIGPERIODSW    equ       50    ;$FILENAME discardtrailigperiodsw.src
RSRC_FUNCLET_UNICODETOOEM    equ       51    ;$FILENAME unicodetooem.src
RSRC_FUNCLET_STRCPYUCS3_2    equ       52    ;$FILENAME strcpyucs3_2.src
RSRC_FUNCLET_STOR_ADDOBJECTTOINDEXTABLE    equ       53    ;$FILENAME stor_addobjecttoindextable.src
RSRC_DATABASE_DELETE    equ       54    ;$FILENAME database_delete.src

;///////////////////////////////////////////////////////////////////////////////
;  Common system resources
;///////////////////////////////////////////////////////////////////////////////

ConnectImageLeft        equ       55    ;$FILENAME connectleft.src
ConnectImageRight       equ       56    ;$FILENAME connectright.src
ReadyImage              equ       57    ;$FILENAME ready.src
ReadingImage            equ       58    ;$FILENAME reading.src
WritingImage            equ       59    ;$FILENAME writing.src
NotesImage              equ       60    ;$FILENAME notes.src
LowBatImage             equ       61    ;$FILENAME lowbat.src
DoNotDisconnectImage    equ       62    ;$FILENAME donotdisconnect.src
RSRC_FULL_SPEED         equ       63    ;$FILENAME fullspeed.src
RSRC_LCD_INIT_SEQ       equ       64    ;$FILENAME lcd_controller_init.src
RSRC_VERSION            equ       65    ;$FILENAME version.src
RSRC_PERIOD             equ       66    ;$FILENAME period.src
RSRC_FONT_TABLE         equ       67    ;$FILENAME font_table.src

;///////////////////////////////////////////////////////////////////////////////
;  Battery Status
;///////////////////////////////////////////////////////////////////////////////
RSRC_BATTERY_00         equ       68    ;$FILENAME battery_00.src
RSRC_BATTERY_01         equ       69    ;$FILENAME battery_01.src
RSRC_BATTERY_02         equ       70    ;$FILENAME battery_02.src
RSRC_BATTERY_03         equ       71    ;$FILENAME battery_03.src
RSRC_BATTERY_04         equ       72    ;$FILENAME battery_04.src
RSRC_BATTERY_05         equ       73    ;$FILENAME battery_05.src
RSRC_BATTERY_06         equ       74    ;$FILENAME battery_06.src
RSRC_BATTERY_07         equ       75    ;$FILENAME battery_07.src
RSRC_BATTERY_08         equ       76    ;$FILENAME battery_08.src
RSRC_BATTERY_09         equ       77    ;$FILENAME battery_09.src
RSRC_BATTERY_10         equ       78    ;$FILENAME battery_10.src

;///////////////////////////////////////////////////////////////////////////////
;  Font
;///////////////////////////////////////////////////////////////////////////////
RSRC_PGM_8              equ       79    ;$FILENAME font_PGM.src
RSRC_SGMS_8             equ       80    ;$FILENAME font_SGMs.src
RSRC_SCRIPT_00_8        equ       81    ;$FILENAME font_script_00.src
RSRC_SCRIPTS_8          equ       82    ;$FILENAME font_scripts.src
RSRC_PDM                equ       83    ;$FILENAME font_PDM.src
RSRC_SDMS               equ       84    ;$FILENAME font_SDMs.src

RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE    equ       85    ;$FILENAME Funclet_GetTextWidthAddressUnicode.src
RSRC_FUNCLET_SET_FONT    equ       86    ;$FILENAME Funclet_LCDSetFont.src
RSRC_FUNCLET_DBCS_TO_UNICODE    equ       87    ;$FILENAME Funclet_DBCSToUnicode.src
RSRC_FUNCLET_HAL_DISPLAY_INIT    equ       88    ;$FILENAME Funclet_HalDisplayInit.src
RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE    equ       89    ;$FILENAME Funclet_GetTextWidthResourceUnicode.src
RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS    equ       90    ;$FILENAME Funclet_GetTextWidthAddressDBCS.src
RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS    equ       91    ;$FILENAME Funclet_GetTextWidthResourceDBCS.src
RSRC_FUNCLET_GET_UNICODE_CHARACTER_BITMAP    equ       92    ;$FILENAME Funclet_GetUnicodeCharacterBitmap.src
RSRC_FUNCLET_CHIPGETSERIALNUMBER    equ       93    ;$FILENAME Funclet_ChipGetSerialNumber.src
RSRC_FUNCLET_LOWRESOLUTIONADCINIT    equ       94    ;$FILENAME Funclet_SysLowResourceAdcInit.src
RSRC_FUNCLET_CREATENULLSERIALNUMBER    equ       95    ;$FILENAME Funclet_CreateNullSerialNumber.src
RSRC_FUNCLET_INTERNALMEDIASERIALNUMBERINIT    equ       96    ;$FILENAME Funclet_IntMediaSerialNumInit.src
RSRC_FUNCLET_CONVERTHEXTOASCII    equ       97    ;$FILENAME Funclet_ConvertHexToAscii.src
RSRC_FUNCLET_CHIPSERIALNUMBERINIT    equ       98    ;$FILENAME Funclet_ChipSerialNumberInit.src

;///////////////////////////////////////////////////////////////////////////////
;  File system
;///////////////////////////////////////////////////////////////////////////////
RSRC_FUNCLET_CLEARCLUSTER    equ       99    ;$FILENAME clearcluster.src
RSRC_FUNCLET_CREATEDIRECTORY    equ      100    ;$FILENAME createdirectory.src
RSRC_FUNCLET_FILEGETATTRIB    equ      101    ;$FILENAME filegetattrib.src
RSRC_FUNCLET_FILEGETDATE    equ      102    ;$FILENAME filegetdate.src
RSRC_FUNCLET_FILESETATTRIB    equ      103    ;$FILENAME filesetattrib.src
RSRC_FUNCLET_FILESETDATE    equ      104    ;$FILENAME filesetdate.src
RSRC_FUNCLET_FSINIT     equ      105    ;$FILENAME fsinit.src
RSRC_FUNCLET_FSSHUTDOWN    equ      106    ;$FILENAME fsshutdown.src
RSRC_FUNCLET_READDEVICERECORD    equ      107    ;$FILENAME readdevicerecord.src
RSRC_FUNCLET_FSDRIVEINIT    equ      108    ;$FILENAME fsdriveinit.src
RSRC_FUNCLET_SETCWDHANDLE    equ      109    ;$FILENAME setcwdhandle.src
;RSRC_FUNCLET_TOTALFREECLUSTER                      equ 0 ;$ FILENAME totalfreecluster.src
;RSRC_FUNCLET_TOTALFREECLUSTERFAT16                 equ 0 ;$ FILENAME totalfreeclusterfat16.src
RSRC_FUNCLET_FREEHANDLE    equ      110    ;$FILENAME freehandle.src
RSRC_FUNCLET_ISHANDLEWRITEALLOCATED    equ      111    ;$FILENAME ishandlewriteallocated.src
RSRC_FUNCLET_ISCURRWORKDIR    equ      112    ;$FILENAME iscurrworkdir.src
RSRC_FUNCLET_DELETETREE    equ      113    ;$FILENAME deletetree.src
RSRC_FUNCLET_DELETEALLRECORDS    equ      114    ;$FILENAME deleteallrecords.src
RSRC_FUNCLET_CLEARDATA    equ      115    ;$FILENAME cleardata.src
RSRC_FUNCLET_CHANGETOLOWLEVELDIR    equ      116    ;$FILENAME changetolowleveldir.src
RSRC_FUNCLET_GETRECORDNUMBER    equ      117    ;$FILENAME getrecordnumber.src
RSRC_FUNCLET_FILEREMOVE    equ      118    ;$FILENAME fileremove.src
RSRC_FUNCLET_CHARACTERSEARCH    equ      119    ;$FILENAME charactersearch.src
RSRC_FUNCLET_STRINGCOMPARE    equ      120    ;$FILENAME stringcompare.src
RSRC_FUNCLET_FREMOVEW    equ      121    ;$FILENAME fremovew.src
RSRC_FUNCLET_MKDIR      equ      122    ;$FILENAME mkdir.src
RSRC_FUNCLET_MKDIRW     equ      123    ;$FILENAME mkdirw.src
RSRC_FUNCLET_RMDIR      equ      124    ;$FILENAME rmdir.src
RSRC_FUNCLET_RMDIRW     equ      125    ;$FILENAME rmdirw.src
RSRC_FUNCLET_FGETC      equ      126    ;$FILENAME fgetc.src
RSRC_FUNCLET_FGETS      equ      127    ;$FILENAME fgets.src
RSRC_FUNCLET_FPUTC      equ      128    ;$FILENAME fputc.src
RSRC_FUNCLET_FPUTS      equ      129    ;$FILENAME fputs.src
RSRC_FUNCLET_CONVERT_ITOA    equ      130    ;$FILENAME convert_itoa.src
RSRC_FUNCLET_EXTRACTPATHW    equ      131    ;$FILENAME extractpathw.src
RSRC_FUNCLET_EXTRACTPATH    equ      132    ;$FILENAME extractpath.src
RSRC_FUNCLET_FINDFREERECORD    equ      133    ;$FILENAME findfreerecord.src
RSRC_FUNCLET_ISDIRECTORYEMPTY    equ      134    ;$FILENAME isdirectoryempty.src
RSRC_FUNCLET_ISSHORTNAMEVALID    equ      135    ;$FILENAME isshortnamevalid.src
RSRC_FUNCLET_SETCWD     equ      136    ;$FILENAME setcwd.src
RSRC_FUNCLET_GENERATEFILENAMETAIL    equ      137    ;$FILENAME generatefilenametail.src
RSRC_FUNCLET_DBCSTOUNICODE    equ      138    ;$FILENAME dbcstounicode.src
RSRC_FUNCLET_STRCPYW    equ      139    ;$FILENAME strcpyw.src
RSRC_FUNCLET_FILESYSTEMPRESENT    equ      140    ;$FILENAME filesystempresent.src
RSRC_FUNCLET_DATADRIVEINIT    equ      141    ;$FILENAME DataDriveInit.src
RSRC_FUNCLET_FSGETDATADRIVEPBSLOCATION    equ      142    ;$FILENAME FSGetDataDrivePbsLocation.src
RSRC_FUNCLET_FSPARTITIONENTRYCOPYFSDD    equ      143    ;$FILENAME FSPartitionEntryCopy.src
RSRC_FUNCLET_DATADRIVEGETSIZE    equ      144    ;$FILENAME DataDriveGetSize.src
RSRC_FUNCLET_CONSTRUCTLONGFILENAME    equ      145    ;$FILENAME ConstructLongFileName.src
RSRC_FUNCLET_GETVOLUMELABEL    equ      146    ;$FILENAME getvolumelabel.src
RSRC_FUNCLET_SETVOLUMELABEL    equ      147    ;$FILENAME setvolumelabel.src
RSRC_FUNCLET_INITCHAR2PTPSTRING    equ      148    ;$FILENAME initchar2ptpstring.src
RSRC_FUNCLET_CATCHAR2PTPSTRING    equ      149    ;$FILENAME catchar2ptpstring.src
RSRC_FUNCLET_CATPTPSTRING    equ      150    ;$FILENAME catptpstring.src
RSRC_FUNCLET_INITTIME2PTPSTRING    equ      151    ;$FILENAME inittime2ptpstring.src
RSRC_FUNCLET_INITPTPSTRING2TIME    equ      152    ;$FILENAME initptpstring2time.src
RSRC_FUNCLET_MTPEXTRACTNUMBERFROMPTPSTRING    equ      153    ;$FILENAME mtpextractnumberfromptpstring.src
RSRC_FUNCLET_MTPSTRINGPRINTNUMBER    equ      154    ;$FILENAME mtpstringprintnumber.src
RSRC_FUNCLET_COPYPTPSTRING    equ      155    ;$FILENAME copyptpstring.src
RSRC_FUNCLET_ISEQUALPTPSTRING    equ      156    ;$FILENAME isequalptpstring.src
RSRC_FUNCLET_COPYPTPTOPACKEDSTRING    equ      157    ;$FILENAME copyptptopackedstring.src
RSRC_FUNCLET_COPYPACKEDTOPTPSTRING    equ      158    ;$FILENAME copypackedtoptpstring.src
RSRC_FUNCLET_INITPTPARRAY    equ      159    ;$FILENAME initptparray.src
RSRC_FUNCLET_SIZEINBYTESPTPARRAY    equ      160    ;$FILENAME sizeinbytesptparray.src
RSRC_FUNCLET_FINDELEMENTPTPARRAY    equ      161    ;$FILENAME findelementptparray.src
RSRC_FUNCLET_APPENDELEMENTPTPARRAY    equ      162    ;$FILENAME appendelementptparray.src
RSRC_FUNCLET_REMOVEELEMENTPTPARRAY    equ      163    ;$FILENAME removeelementptparray.src
RSRC_FUNCLET_COPYPTPARRAY    equ      164    ;$FILENAME copyptparray.src
RSRC_FUNCLET_PACKPTPARRAY    equ      165    ;$FILENAME packptparray.src
RSRC_FUNCLET_DRM_B64_DECODEA    equ      166    ;$FILENAME drm_b64_decodea.src
RSRC_FUNCLET_DRM_BBX_DECRYPTLICENSE    equ      167    ;$FILENAME drm_bbx_decryptlicense.src
RSRC_FUNCLET_DRM_BBX_SIGNDATA    equ      168    ;$FILENAME drm_bbx_signdata.src
RSRC_FUNCLET_DRM_BBX_SYMMETRICSIGN    equ      169    ;$FILENAME drm_bbx_symmetricsign.src
RSRC_FUNCLET_DRM_BBX_ASYMMETRICTOSYMMETRICKEY    equ      170    ;$FILENAME drm_bbx_asymmetrictosymmetrickey.src
RSRC_FUNCLET_DRM_BBX_REBINDSYMMETRICKEY    equ      171    ;$FILENAME drm_bbx_rebindsymmetrickey.src
RSRC_FUNCLET_DRM_BBX_REBINDSLK    equ      172    ;$FILENAME drm_bbx_rebindslk.src
RSRC_FUNCLET__REQUIREDCHARSFORCHALLENGE    equ      173    ;$FILENAME _requiredcharsforchallenge.src
RSRC_FUNCLET_SYNCCOUNTTEST    equ      174    ;$FILENAME synccounttest.src
RSRC_FUNCLET_SYNCEXPIRATIONTEST    equ      175    ;$FILENAME syncexpirationtest.src
RSRC_FUNCLET__SYNCREQUIRED    equ      176    ;$FILENAME _syncrequired.src
RSRC_FUNCLET__GETKIDLICENSEDATA    equ      177    ;$FILENAME _getkidlicensedata.src
RSRC_FUNCLET__STOREREFRESHREQUIRED    equ      178    ;$FILENAME _storerefreshrequired.src
RSRC_FUNCLET__WRITEKIDTOLIST    equ      179    ;$FILENAME _writekidtolist.src



RSRC_FUNCLET_DRM_HDS_CLEANUPSTORE    equ      180    ;$FILENAME drm_hds_cleanupstore.src
RSRC_FUNCLET__HDSCOPYSLOT_CHILD2CHILD    equ      181    ;$FILENAME _hdscopyslot_child2child.src
RSRC_FUNCLET__HDSCOPYCHILDPAYLOAD    equ      182    ;$FILENAME _hdscopychildpayload.src
RSRC_FUNCLET__HDSDEFRAGMENTFILE    equ      183    ;$FILENAME _hdsdefragmentfile.src
RSRC_FUNCLET__HDSCLEANUPNAMESPACE    equ      184    ;$FILENAME _hdscleanupnamespace.src
RSRC_FUNCLET__HDSCLEANUPSTORE    equ      185    ;$FILENAME _hdscleanupstore.src
RSRC_FUNCLET__EVALUATELICENSE    equ      186    ;$FILENAME _evaluatelicense.src
RSRC_FUNCLET__UPDATELICENSEWITHSYMMETRICSIGNATURE    equ      187    ;$FILENAME _updatelicensewithsymmetricsignature.src
RSRC_FUNCLET__UPDATELICENSEWITHSYMMETRICKEY    equ      188    ;$FILENAME _updatelicensewithsymmetrickey.src
RSRC_FUNCLET__UPDATELICENSEWITHSYMMETRICDATA    equ      189    ;$FILENAME _updatelicensewithsymmetricdata.src
RSRC_FUNCLET_COMPARELICENSE    equ      190    ;$FILENAME comparelicense.src
RSRC_FUNCLET_DRM_LST_ADDLICENSE    equ      191    ;$FILENAME drm_lst_addlicense.src
RSRC_FUNCLET_DRM_LST_LICCOUNT    equ      192    ;$FILENAME drm_lst_liccount.src
RSRC_FUNCLET_DRM_MGR_PROCESSLICENSERESPONSE    equ      193    ;$FILENAME drm_mgr_processlicenseresponse.src
RSRC_FUNCLET_DRM_MGR_PROCESSMETERRESPONSE    equ      194    ;$FILENAME drm_mgr_processmeterresponse.src
RSRC_FUNCLET_DRM_MGR_GENERATESYNCCHALLENGE    equ      195    ;$FILENAME drm_mgr_generatesyncchallenge.src
RSRC_FUNCLET_DRM_MGR_CLEANUPLICENSESTORE    equ      196    ;$FILENAME drm_mgr_cleanuplicensestore.src
RSRC_FUNCLET__DELETELICENSES    equ      197    ;$FILENAME _deletelicenses.src
RSRC_FUNCLET__PROCESSLICENSEREVOCATIONLIST    equ      198    ;$FILENAME _processlicenserevocationlist.src
RSRC_FUNCLET_DRM_MTR_GETALLDATA    equ      199    ;$FILENAME drm_mtr_getalldata.src
RSRC_FUNCLET_DRM_MTR_INITENUM    equ      200    ;$FILENAME drm_mtr_initenum.src
RSRC_FUNCLET_DRM_MTR_ENUMNEXT    equ      201    ;$FILENAME drm_mtr_enumnext.src
RSRC_FUNCLET__REQUIREDCHARSFORBINARY    equ      202    ;$FILENAME _requiredcharsforbinary.src
RSRC_FUNCLET__REQUIREDCHARSFORKID    equ      203    ;$FILENAME _requiredcharsforkid.src
RSRC_FUNCLET__GETKIDDATA    equ      204    ;$FILENAME _getkiddata.src
RSRC_FUNCLET__CREATENEWTID    equ      205    ;$FILENAME _createnewtid.src
RSRC_FUNCLET__VERIFYSIGNATURE    equ      206    ;$FILENAME _verifysignature.src
RSRC_FUNCLET__CREATEMETERSTOREPASSWORD    equ      207    ;$FILENAME _createmeterstorepassword.src
RSRC_FUNCLET__PROCESSONEKID    equ      208    ;$FILENAME _processonekid.src
RSRC_FUNCLET_DRM_MTR_PROCESSMETERRESPONSE    equ      209    ;$FILENAME drm_mtr_processmeterresponse.src
RSRC_FUNCLET_DRM_SST_GETLOCKEDDATA    equ      210    ;$FILENAME drm_sst_getlockeddata.src
RSRC_FUNCLET_DRM_SST_SETLOCKEDDATA    equ      211    ;$FILENAME drm_sst_setlockeddata.src
RSRC_FUNCLET_DRM_SST_CLOSELOCKEDSLOT    equ      212    ;$FILENAME drm_sst_closelockedslot.src
RSRC_FUNCLET_DRM_SST_GETALLDATA    equ      213    ;$FILENAME drm_sst_getalldata.src
RSRC_FUNCLET_DRM_SST_OPENENUMERATOR    equ      214    ;$FILENAME drm_sst_openenumerator.src
RSRC_FUNCLET_DRM_SST_ENUMNEXT    equ      215    ;$FILENAME drm_sst_enumnext.src
RSRC_FUNCLET__GETKIDSTOREDATA    equ      216    ;$FILENAME _getkidstoredata.src
RSRC_FUNCLET_DRM_SNC_INITENUM    equ      217    ;$FILENAME drm_snc_initenum.src
RSRC_FUNCLET_DRM_SNC_ENUMNEXT    equ      218    ;$FILENAME drm_snc_enumnext.src
RSRC_FUNCLET_DRM_UTL_ISCERTDATEOK    equ      219    ;$FILENAME drm_utl_iscertdateok.src
RSRC_FUNCLET__UTL_VERIFYSIGNATURE    equ      220    ;$FILENAME _utl_verifysignature.src
RSRC_FUNCLET__CHECKCERTIFICATE    equ      221    ;$FILENAME _checkcertificate.src
RSRC_FUNCLET_DRM_UTL_CHECKCERTIFICATE    equ      222    ;$FILENAME drm_utl_checkcertificate.src
RSRC_FUNCLET_DRM_UTL_CHECKCERT    equ      223    ;$FILENAME drm_utl_checkcert.src
RSRC_FUNCLET_DRM_PK_ENCRYPTLARGE    equ      224    ;$FILENAME drm_pk_encryptlarge.src
RSRC_FUNCLET_DRM_UTL_VERIFYXMLSIGNATURE    equ      225    ;$FILENAME drm_utl_verifyxmlsignature.src
RSRC_FUNCLET_DRM_UTL_VERIFYXMLSIGNATUREEX    equ      226    ;$FILENAME drm_utl_verifyxmlsignatureex.src
RSRC_FUNCLET__PUSHDWORD    equ      227    ;$FILENAME _pushdword.src
RSRC_FUNCLET__GETPUSHEDDWORD    equ      228    ;$FILENAME _getpusheddword.src
RSRC_FUNCLET__ALLTRIM2    equ      229    ;$FILENAME _alltrim2.src
RSRC_FUNCLET__CREATECONTEXT    equ      230    ;$FILENAME _createcontext.src
RSRC_FUNCLET__OPENNODE    equ      231    ;$FILENAME _opennode.src
RSRC_FUNCLET__CLOSECURRNODE    equ      232    ;$FILENAME _closecurrnode.src
RSRC_FUNCLET__CALCNODESIZE    equ      233    ;$FILENAME _calcnodesize.src
RSRC_FUNCLET_DRM_XMB_SIMULATECREATEDOCUMENT    equ      234    ;$FILENAME drm_xmb_simulatecreatedocument.src
RSRC_FUNCLET_DRM_XMB_CREATEDOCUMENT    equ      235    ;$FILENAME drm_xmb_createdocument.src
RSRC_FUNCLET_DRM_XMB_REALLOCDOCUMENT    equ      236    ;$FILENAME drm_xmb_reallocdocument.src
RSRC_FUNCLET_DRM_XMB_CLOSEDOCUMENT    equ      237    ;$FILENAME drm_xmb_closedocument.src
RSRC_FUNCLET_DRM_XMB_OPENNODE    equ      238    ;$FILENAME drm_xmb_opennode.src
RSRC_FUNCLET_DRM_XMB_CLOSECURRNODE    equ      239    ;$FILENAME drm_xmb_closecurrnode.src
RSRC_FUNCLET_DRM_XMB_SAVESTATE    equ      240    ;$FILENAME drm_xmb_savestate.src
RSRC_FUNCLET_DRM_XMB_RESTORESTATE    equ      241    ;$FILENAME drm_xmb_restorestate.src
RSRC_FUNCLET_DRM_XMB_ENCRYPTANDCLOSECURRNODE    equ      242    ;$FILENAME drm_xmb_encryptandclosecurrnode.src
RSRC_FUNCLET_DRM_XMB_GETCURRNODENAME    equ      243    ;$FILENAME drm_xmb_getcurrnodename.src
RSRC_FUNCLET_DRM_XMB_GETCONTEXTSIZE    equ      244    ;$FILENAME drm_xmb_getcontextsize.src
RSRC_FUNCLET_DRM_XMB_ADDATTRIBUTE    equ      245    ;$FILENAME drm_xmb_addattribute.src
RSRC_FUNCLET_DRM_XMB_ADDDATA    equ      246    ;$FILENAME drm_xmb_adddata.src
RSRC_FUNCLET_DRM_XMB_REQUIREDCHARSFORTAG    equ      247    ;$FILENAME drm_xmb_requiredcharsfortag.src
RSRC_FUNCLET_DRM_XMB_WRITETAG    equ      248    ;$FILENAME drm_xmb_writetag.src
RSRC_FUNCLET_DRM_XMB_REMAININGBUFFER    equ      249    ;$FILENAME drm_xmb_remainingbuffer.src
RSRC_FUNCLET__ALLTRIMA    equ      250    ;$FILENAME _alltrima.src
RSRC_FUNCLET__SCANTAGA    equ      251    ;$FILENAME _scantaga.src
RSRC_FUNCLET__SCANNODEFORATTRIBUTEA    equ      252    ;$FILENAME _scannodeforattributea.src
RSRC_FUNCLET__CHECKTAGA    equ      253    ;$FILENAME _checktaga.src
RSRC_FUNCLET_DRM_XML_GETNODEATTRIBUTEA    equ      254    ;$FILENAME drm_xml_getnodeattributea.src
RSRC_FUNCLET_DRM_B64_ENCODEA    equ      255    ;$FILENAME drm_b64_encodea.src
RSRC_FUNCLET__VERIFYCACHEDCERTIFICATE    equ      256    ;$FILENAME _verifycachedcertificate.src
RSRC_FUNCLET__ADDCACHEDCERTIFICATE    equ      257    ;$FILENAME _addcachedcertificate.src
;RSRC_MTP_P                                         equ 0 ;$FILENAME mtp_p.src
RSRC_FUNCLET_DRM_SST_GETDATA    equ      258    ;$FILENAME drm_sst_getdata.src
RSRC_FUNCLET_DRM_SST_SETDATA    equ      259    ;$FILENAME drm_sst_setdata.src
RSRC_FUNCLET_DRM_SST_OPENANDLOCKSLOT    equ      260    ;$FILENAME drm_sst_openandlockslot.src
RSRC_FUNCLET_DRM_XML_GETNODEA    equ      261    ;$FILENAME drm_xml_getnodea.src
RSRC_FUNCLET_DRM_XML_GETSUBNODEA    equ      262    ;$FILENAME drm_xml_getsubnodea.src
RSRC_FUNCLET__READTIDFROMMETERINGSTORE    equ      263    ;$FILENAME _readtidfrommeteringstore.src
RSRC_FUNCLET__WRITETIDTOMETERINGSTORE    equ      264    ;$FILENAME _writetidtometeringstore.src
RSRC_FUNCLET__PARSECONTROLDATA    equ      265    ;$FILENAME _parsecontroldata.src
RSRC_FUNCLET_DRM_LA_PROCESSRESPONSE    equ      266    ;$FILENAME drm_la_processresponse.src
RSRC_FUNCLET_DRM_MGR_PROCESSCOMMAND    equ      267    ;$FILENAME drm_mgr_processcommand.src
RSRC_FUNCLET_DRM_MGR_PROCESSREQUEST    equ      268    ;$FILENAME drm_mgr_processrequest.src
RSRC_FUNCLET_DRM_MGR_GETLICENSESTATE    equ      269    ;$FILENAME drm_mgr_getlicensestate.src
RSRC_FUNCLET_DRM_SNC_GENERATESYNCCHALLENGE    equ      270    ;$FILENAME drm_snc_generatesyncchallenge.src
RSRC_FUNCLET_DRM_UTL_DASSTRSTRINGSEQUAL    equ      271    ;$FILENAME drm_utl_dasstrstringsequal.src
RSRC_FUNCLET_DRM_MGR_GENERATEMETERCHALLENGE    equ      272    ;$FILENAME drm_mgr_generatemeterchallenge.src
RSRC_FUNCLET_DRM_MTR_GENERATEMETERCHALLENGE    equ      273    ;$FILENAME drm_mtr_generatemeterchallenge.src
RSRC_FUNCLET__ATTEMPTGENERATEMETERCHALLENGE    equ      274    ;$FILENAME _attemptgeneratemeterchallenge.src
RSRC_FUNCLET__BUILDCHALLENGE    equ      275    ;$FILENAME _buildchallenge.src
RSRC_FUNCLET__PROCESSALLKIDS    equ      276    ;$FILENAME _processallkids.src
RSRC_FUNCLET_DRM_MGR_CLKPROCESSRESPONSE    equ      277    ;$FILENAME drm_mgr_clkprocessresponse.src
RSRC_FUNCLET_DRM_MGR_CLKGENERATECHALLENGE    equ      278    ;$FILENAME drm_mgr_clkgeneratechallenge.src
RSRC_FUNCLET__NUMBERTOFORMATSTRING    equ      279    ;$FILENAME _numbertoformatstring.src
RSRC_FUNCLET_DRM_CLK_PROCESSRESPONSE    equ      280    ;$FILENAME drm_clk_processresponse.src
RSRC_FUNCLET_DRM_CLK_GETSECURECLOCK    equ      281    ;$FILENAME drm_clk_getsecureclock.src
RSRC_FUNCLET_DRM_MGR_GETSECURECLOCK    equ      282    ;$FILENAME drm_mgr_getsecureclock.src
RSRC_FUNCLET_DRM_CLK_CREATECHALLENGE    equ      283    ;$FILENAME drm_clk_createchallenge.src
RSRC_FUNCLET__HDSBLOCKSCANINIT    equ      284    ;$FILENAME _hdsblockscaninit.src
RSRC_FUNCLET__ISMATCHINGKEY    equ      285    ;$FILENAME _ismatchingkey.src
RSRC_FUNCLET__COALESCEBLOCK    equ      286    ;$FILENAME _coalesceblock.src
RSRC_FUNCLET_DRM_HDS_SLOTENUMRELOADCURRENT    equ      287    ;$FILENAME drm_hds_slotenumreloadcurrent.src
RSRC_FUNCLET_DRM_HDS_BLOCKSCANDELETE    equ      288    ;$FILENAME drm_hds_blockscandelete.src
RSRC_FUNCLET__ENUMXMLSUBNODEW    equ      289    ;$FILENAME _enumxmlsubnodew.src
RSRC_FUNCLET_DRM_XML_ENUMNEXTNODE    equ      290    ;$FILENAME drm_xml_enumnextnode.src
RSRC_FUNCLET_DRM_XML_ENUMNEXTSUBNODE    equ      291    ;$FILENAME drm_xml_enumnextsubnode.src
RSRC_FUNCLET__INITSLOT    equ      292    ;$FILENAME _initslot.src
RSRC_FUNCLET__LOADSLOT    equ      293    ;$FILENAME _loadslot.src
RSRC_FUNCLET_DRM_SST_ENUMLOADCURRENT    equ      294    ;$FILENAME drm_sst_enumloadcurrent.src
RSRC_FUNCLET_DRM_MTR_ENUMLOADCURRENT    equ      295    ;$FILENAME drm_mtr_enumloadcurrent.src
;;;;;;;;;;;;;;;;;;;;;;;;;;
;Janus Lib related resources
;;;;;;;;;;;;;;;;;;;;;;;;;
RSRC_FUNCLET_LEGACY_DECRYPTCONTENTKEY    equ      296    ;$FILENAME legacy_decryptcontentkey.src
RSRC_FUNCLET_DRM_BBX_SYMMETRICVERIFY    equ      297    ;$FILENAME drm_bbx_symmetricverify.src
RSRC_FUNCLET_DRM_BBX_LEGACYCIPHERKEYSETUP    equ      298    ;$FILENAME drm_bbx_legacycipherkeysetup.src
RSRC_FUNCLET_DRM_BBX_INITIALIZE    equ      299    ;$FILENAME drm_bbx_initialize.src
RSRC_FUNCLET_DRM_BBX_CANBIND    equ      300    ;$FILENAME drm_bbx_canbind.src
RSRC_FUNCLET__DECRYPTCONTENTKEY    equ      301    ;$FILENAME _decryptcontentkey.src
RSRC_FUNCLET_VERIFYCHECKSUM    equ      302    ;$FILENAME verifychecksum.src
RSRC_FUNCLET_DRM_BBX_HASHVALUE    equ      303    ;$FILENAME drm_bbx_hashvalue.src
RSRC_FUNCLET_DRM_BBX_CIPHERKEYSETUP    equ      304    ;$FILENAME drm_bbx_cipherkeysetup.src
RSRC_FUNCLET__FINDKEYPAIR    equ      305    ;$FILENAME _findkeypair.src
RSRC_FUNCLET_DRM_B64_DECODEW    equ      306    ;$FILENAME drm_b64_decodew.src
RSRC_FUNCLET_DRM_B64_ENCODEW    equ      307    ;$FILENAME drm_b64_encodew.src
RSRC_FUNCLET_DRM_MAC_INV32    equ      308    ;$FILENAME drm_mac_inv32.src
RSRC_FUNCLET__PARSELICENSEATTRIBUTES    equ      309    ;$FILENAME _parselicenseattributes.src
RSRC_FUNCLET__LOADLICENSEATTRIBUTES    equ      310    ;$FILENAME _loadlicenseattributes.src
RSRC_FUNCLET__PERFORMACTIONS    equ      311    ;$FILENAME _performactions.src
RSRC_FUNCLET__GETLICENSEINFOANDSETUP    equ      312    ;$FILENAME _getlicenseinfoandsetup.src
RSRC_FUNCLET__PROCESSENDOFCHAIN    equ      313    ;$FILENAME _processendofchain.src
RSRC_FUNCLET_DRM_LIC_COMPLETELICENSECHAIN    equ      314    ;$FILENAME drm_lic_completelicensechain.src
RSRC_FUNCLET_DRM_LIC_REPORTACTIONS    equ      315    ;$FILENAME drm_lic_reportactions.src
RSRC_FUNCLET_DRM_LIC_CHECKCLOCKROLLBACK    equ      316    ;$FILENAME drm_lic_checkclockrollback.src
RSRC_FUNCLET_DRM_CPHR_INIT    equ      317    ;$FILENAME drm_cphr_init.src
RSRC_FUNCLET_DRMCRT_ISWXDIGIT    equ      318    ;$FILENAME drmcrt_iswxdigit.src
RSRC_FUNCLET_DRMCRT_TOWLOWER    equ      319    ;$FILENAME drmcrt_towlower.src
RSRC_FUNCLET_DRMCRT_WCSNTOL    equ      320    ;$FILENAME drmcrt_wcsntol.src
RSRC_FUNCLET_DRMCRT_WCSLEN    equ      321    ;$FILENAME drmcrt_wcslen.src
RSRC_FUNCLET_DRMCRT_WCSNCPY    equ      322    ;$FILENAME drmcrt_wcsncpy.src
RSRC_FUNCLET_DRMCRT_MEMMOVE    equ      323    ;$FILENAME drmcrt_memmove.src
RSRC_FUNCLET_PERFORMOPERATION_PART1    equ      324    ;$FILENAME performoperation_part1.src
RSRC_FUNCLET_PERFORMOPERATION_PART2    equ      325    ;$FILENAME performoperation_part2.src
RSRC_FUNCLET_PERFORMOPERATION_PART3    equ      326    ;$FILENAME performoperation_part3.src
RSRC_FUNCLET_PERFORMOPERATION_PART4    equ      327    ;$FILENAME performoperation_part4.src
RSRC_FUNCLET_PERFORMOPERATION_PART5    equ      328    ;$FILENAME performoperation_part5.src
RSRC_FUNCLET_PERFORMOPERATION_PART6    equ      329    ;$FILENAME performoperation_part6.src
RSRC_FUNCLET_ISVALIDFUNCTION    equ      330    ;$FILENAME isvalidfunction.src
RSRC_FUNCLET_FUNCTIONGETVALUE    equ      331    ;$FILENAME functiongetvalue.src
RSRC_FUNCLET_GLOBALSETVARIABLE    equ      332    ;$FILENAME globalsetvariable.src
RSRC_FUNCLET_VARIABLEDRMKGETORSET    equ      333    ;$FILENAME variabledrmkgetorset.src
RSRC_FUNCLET_VARIABLEDRMGETORSET    equ      334    ;$FILENAME variabledrmgetorset.src
RSRC_FUNCLET_VARIABLEMACHINEGETORSET    equ      335    ;$FILENAME variablemachinegetorset.src
RSRC_FUNCLET_VARIABLEAPPGETORSET    equ      336    ;$FILENAME variableappgetorset.src
RSRC_FUNCLET_VARIABLESECSTATEGETORSET    equ      337    ;$FILENAME variablesecstategetorset.src
RSRC_FUNCLET_VARIABLELICENSEGETORSET    equ      338    ;$FILENAME variablelicensegetorset.src
RSRC_FUNCLET_VARIABLECONTENTGETORSET    equ      339    ;$FILENAME variablecontentgetorset.src
RSRC_FUNCLET_VARIABLEDEVICEGETORSET    equ      340    ;$FILENAME variabledevicegetorset.src
RSRC_FUNCLET_VARIABLEPMLICENSEGETORSET    equ      341    ;$FILENAME variablepmlicensegetorset.src
RSRC_FUNCLET__BASICHEADERCHECKS    equ      342    ;$FILENAME _basicheaderchecks.src
RSRC_FUNCLET_DRM_HDR_GETATTRIBUTE    equ      343    ;$FILENAME drm_hdr_getattribute.src
RSRC_FUNCLET_DRM_HDS_CREATESTORE    equ      344    ;$FILENAME drm_hds_createstore.src
RSRC_FUNCLET_DRM_HDS_INIT    equ      345    ;$FILENAME drm_hds_init.src
RSRC_FUNCLET_DRM_HDS_UNINIT    equ      346    ;$FILENAME drm_hds_uninit.src
RSRC_FUNCLET_DRM_HDS_OPENSTORE    equ      347    ;$FILENAME drm_hds_openstore.src
RSRC_FUNCLET_DRM_HDS_CLOSESTORE    equ      348    ;$FILENAME drm_hds_closestore.src
RSRC_FUNCLET_DRM_HDS_DELETESLOT    equ      349    ;$FILENAME drm_hds_deleteslot.src
RSRC_FUNCLET_DRM_HDS_SLOTSEEK    equ      350    ;$FILENAME drm_hds_slotseek.src
RSRC_FUNCLET_DRM_HDS_SLOTWRITE    equ      351    ;$FILENAME drm_hds_slotwrite.src
RSRC_FUNCLET_DRM_HDS_SLOTRESIZE    equ      352    ;$FILENAME drm_hds_slotresize.src
RSRC_FUNCLET_DRM_HDS_INITSLOTENUM    equ      353    ;$FILENAME drm_hds_initslotenum.src
RSRC_FUNCLET_DRM_HDS_SLOTENUMNEXT    equ      354    ;$FILENAME drm_hds_slotenumnext.src
RSRC_FUNCLET__HDSLOCATEFREEBLOCKFORSLOT    equ      355    ;$FILENAME _hdslocatefreeblockforslot.src
RSRC_FUNCLET__HDSLOCKBLOCK2DELETESLOT    equ      356    ;$FILENAME _hdslockblock2deleteslot.src
RSRC_FUNCLET__HDSUNLOCKBLOCK2DELETESLOT    equ      357    ;$FILENAME _hdsunlockblock2deleteslot.src
RSRC_FUNCLET__HDSCREATENAMESPACE    equ      358    ;$FILENAME _hdscreatenamespace.src
RSRC_FUNCLET__HDSFILEPOS2BLOCKNUM    equ      359    ;$FILENAME _hdsfilepos2blocknum.src
RSRC_FUNCLET__WRITESRN    equ      360    ;$FILENAME _writesrn.src
RSRC_FUNCLET__WRITECOMMONBLOCKHEADER    equ      361    ;$FILENAME _writecommonblockheader.src
RSRC_FUNCLET__WRITECHILDBLOCKHEADER    equ      362    ;$FILENAME _writechildblockheader.src
RSRC_FUNCLET__READDATABLOCKHEADER    equ      363    ;$FILENAME _readdatablockheader.src
RSRC_FUNCLET__WRITEDATABLOCKHEADER    equ      364    ;$FILENAME _writedatablockheader.src
RSRC_FUNCLET__HDSEXPANDSTORE    equ      365    ;$FILENAME _hdsexpandstore.src
RSRC_FUNCLET__HDSALLOCBLOCK    equ      366    ;$FILENAME _hdsallocblock.src
RSRC_FUNCLET__HDSFREEBLOCK    equ      367    ;$FILENAME _hdsfreeblock.src
RSRC_FUNCLET__HDSCREATEOVERSIZEDSLOT    equ      368    ;$FILENAME _hdscreateoversizedslot.src
RSRC_FUNCLET__HDSALLOCSLOTINFILE    equ      369    ;$FILENAME _hdsallocslotinfile.src
RSRC_FUNCLET__HDSWRITEBLOCKHDR    equ      370    ;$FILENAME _hdswriteblockhdr.src
RSRC_FUNCLET__HDSADJUSTCHILDPAYLOAD    equ      371    ;$FILENAME _hdsadjustchildpayload.src
RSRC_FUNCLET__HDSFINDLEFTMOSTLEAFBLOCK    equ      372    ;$FILENAME _hdsfindleftmostleafblock.src
RSRC_FUNCLET__HDSCREATEANDOPENSLOT    equ      373    ;$FILENAME _hdscreateandopenslot.src
RSRC_FUNCLET__HDSREMOVESLOT    equ      374    ;$FILENAME _hdsremoveslot.src
RSRC_FUNCLET__HDSCOPYANDLOCKNEWSLOT    equ      375    ;$FILENAME _hdscopyandlocknewslot.src
RSRC_FUNCLET__HDSRELOCATESLOTANDKEEPCURRLOCK    equ      376    ;$FILENAME _hdsrelocateslotandkeepcurrlock.src
RSRC_FUNCLET__HDSTRAVERSENEXTRIGHTSIBLINGBLOCK    equ      377    ;$FILENAME _hdstraversenextrightsiblingblock.src
RSRC_FUNCLET__HDSTRAVERSEBLOCKSINPOSTORDER    equ      378    ;$FILENAME _hdstraverseblocksinpostorder.src
RSRC_FUNCLET__HDSSLOTRESIZE    equ      379    ;$FILENAME _hdsslotresize.src
RSRC_FUNCLET__ISNULL    equ      380    ;$FILENAME _isnull.src
RSRC_FUNCLET__HDSGENSRNHASH    equ      381    ;$FILENAME _hdsgensrnhash.src
RSRC_FUNCLET__HDSALLOCBLOCKBUFFER    equ      382    ;$FILENAME _hdsallocblockbuffer.src
RSRC_FUNCLET__READSRN    equ      383    ;$FILENAME _readsrn.src
RSRC_FUNCLET__HDSGETPUTCHILDBLOCKNUM    equ      384    ;$FILENAME _hdsgetputchildblocknum.src
RSRC_FUNCLET__HDSHASHKEYTOINDEX    equ      385    ;$FILENAME _hdshashkeytoindex.src
RSRC_FUNCLET__HDSLOADSRN    equ      386    ;$FILENAME _hdsloadsrn.src
RSRC_FUNCLET__HDSSLOTWRITE    equ      387    ;$FILENAME _hdsslotwrite.src
RSRC_FUNCLET__HDSINITSLOTENUM    equ      388    ;$FILENAME _hdsinitslotenum.src
RSRC_FUNCLET__HDSSLOTENUMNEXT    equ      389    ;$FILENAME _hdsslotenumnext.src
RSRC_FUNCLET_DRM_LEVL_EVALUATEEXPRESSION    equ      390    ;$FILENAME drm_levl_evaluateexpression.src
RSRC_FUNCLET_DRM_LEVL_PERFORMOPERATIONS    equ      391    ;$FILENAME drm_levl_performoperations.src
RSRC_FUNCLET_DRM_LIC_GETATTRIBUTE    equ      392    ;$FILENAME drm_lic_getattribute.src
RSRC_FUNCLET_DRM_LIC_GETENABLINGBITS    equ      393    ;$FILENAME drm_lic_getenablingbits.src
RSRC_FUNCLET_GETLSPUBKEY    equ      394    ;$FILENAME getlspubkey.src
RSRC_FUNCLET_DRM_LIC_VERIFYSIGNATURE    equ      395    ;$FILENAME drm_lic_verifysignature.src
RSRC_FUNCLET__LOADPRORITIZEDLIST    equ      396    ;$FILENAME _loadproritizedlist.src
RSRC_FUNCLET_DRM_LST_OPEN    equ      397    ;$FILENAME drm_lst_open.src
RSRC_FUNCLET_DRM_LST_CLOSE    equ      398    ;$FILENAME drm_lst_close.src
RSRC_FUNCLET_DRM_LST_GETLICENSE    equ      399    ;$FILENAME drm_lst_getlicense.src
RSRC_FUNCLET_DRM_LST_INITENUM    equ      400    ;$FILENAME drm_lst_initenum.src
RSRC_FUNCLET_DRM_LST_ENUMNEXT    equ      401    ;$FILENAME drm_lst_enumnext.src
RSRC_FUNCLET_DRM_LST_ENUMDELETE    equ      402    ;$FILENAME drm_lst_enumdelete.src
RSRC_FUNCLET__PROCESSEXTENSIONS    equ      403    ;$FILENAME _processextensions.src
RSRC_FUNCLET__PROCESSIDLIST    equ      404    ;$FILENAME _processidlist.src
RSRC_FUNCLET__PROCESSEXCLUSIONS    equ      405    ;$FILENAME _processexclusions.src
RSRC_FUNCLET__PROCESSINCLUSIONS    equ      406    ;$FILENAME _processinclusions.src
RSRC_FUNCLET_DRM_OPL_PROCESSCOPYOUTPUTLEVELDATA    equ      407    ;$FILENAME drm_opl_processcopyoutputleveldata.src
RSRC_FUNCLET__GETOPLLEVEL    equ      408    ;$FILENAME _getopllevel.src
RSRC_FUNCLET_DRM_OPL_PROCESSPLAYOUTPUTLEVELDATA    equ      409    ;$FILENAME drm_opl_processplayoutputleveldata.src
RSRC_FUNCLET__LOADATTRIBUTESINTOCACHE    equ      410    ;$FILENAME _loadattributesintocache.src
RSRC_FUNCLET_DRM_SST_OPENKEYTOKENS    equ      411    ;$FILENAME drm_sst_openkeytokens.src
RSRC_FUNCLET__UPDATEATTRIBUTETOKENINSLOT    equ      412    ;$FILENAME _updateattributetokeninslot.src
RSRC_FUNCLET_DRM_SST_CLOSEKEY    equ      413    ;$FILENAME drm_sst_closekey.src
RSRC_FUNCLET_DRM_SST_GETTOKENVALUE    equ      414    ;$FILENAME drm_sst_gettokenvalue.src
RSRC_FUNCLET_DRM_SST_SETTOKENVALUE    equ      415    ;$FILENAME drm_sst_settokenvalue.src
RSRC_FUNCLET_DRM_SST_CREATELICENSESTATEPASSWORD    equ      416    ;$FILENAME drm_sst_createlicensestatepassword.src
RSRC_FUNCLET_DRM_SST_CREATEGLOBALSTOREPASSWORD    equ      417    ;$FILENAME drm_sst_createglobalstorepassword.src
RSRC_FUNCLET_DRM_UTL_PROMOTEANSITOUNICODE    equ      418    ;$FILENAME drm_utl_promoteansitounicode.src
RSRC_FUNCLET_DRM_PK_SYMMETRICVERIFY    equ      419    ;$FILENAME drm_pk_symmetricverify.src
RSRC_FUNCLET_DRM_PK_SYMMETRICCRYPT    equ      420    ;$FILENAME drm_pk_symmetriccrypt.src
RSRC_FUNCLET_DRM_PK_SYMMETRICSIGN    equ      421    ;$FILENAME drm_pk_symmetricsign.src
RSRC_FUNCLET_DRM_UTL_GETVERSIONFROMSTRING    equ      422    ;$FILENAME drm_utl_getversionfromstring.src
RSRC_FUNCLET_DRM_UTL_STRINGINSERTBLANKSUBSTRING    equ      423    ;$FILENAME drm_utl_stringinsertblanksubstring.src
RSRC_FUNCLET_DRM_UTL_STRINGREMOVESUBSTRING    equ      424    ;$FILENAME drm_utl_stringremovesubstring.src
RSRC_FUNCLET_DRM_UTL_DSTRSEARCH    equ      425    ;$FILENAME drm_utl_dstrsearch.src
RSRC_FUNCLET_DRM_UTL_DSTRSEARCHREVERSE    equ      426    ;$FILENAME drm_utl_dstrsearchreverse.src
RSRC_FUNCLET_DRM_UTL_NUMBERTOSTRING    equ      427    ;$FILENAME drm_utl_numbertostring.src
RSRC_FUNCLET_DRM_UTL_GETVERSIONASSTRING    equ      428    ;$FILENAME drm_utl_getversionasstring.src
RSRC_FUNCLET_DRM_UTL_STRINGTOGUID    equ      429    ;$FILENAME drm_utl_stringtoguid.src
RSRC_FUNCLET_DRM_UTL_DECODEKID    equ      430    ;$FILENAME drm_utl_decodekid.src
RSRC_FUNCLET_DRM_UTL_ENSUREDATAALIGNMENT    equ      431    ;$FILENAME drm_utl_ensuredataalignment.src
RSRC_FUNCLET_OVERLAPPINGDATES    equ      432    ;$FILENAME overlappingdates.src
RSRC_FUNCLET_DRM_ASD_PARSEV2LICENSE    equ      433    ;$FILENAME drm_asd_parsev2license.src
RSRC_FUNCLET_NEGINFDATE    equ      434    ;$FILENAME neginfdate.src
RSRC_FUNCLET_INFDATE    equ      435    ;$FILENAME infdate.src
RSRC_FUNCLET_ISEXPIRED    equ      436    ;$FILENAME isexpired.src
RSRC_FUNCLET_GETSECSTATEATTR    equ      437    ;$FILENAME getsecstateattr.src
RSRC_FUNCLET_SETEXPIRYCATEGORY    equ      438    ;$FILENAME setexpirycategory.src
RSRC_FUNCLET_GETV2LICENSEINFO    equ      439    ;$FILENAME getv2licenseinfo.src
RSRC_FUNCLET_GETNEXTLICENSE    equ      440    ;$FILENAME getnextlicense.src
RSRC_FUNCLET_AGGREGATE    equ      441    ;$FILENAME aggregate.src
RSRC_FUNCLET_DRM_ASD_GETLICENSEAGGREGATEDATA    equ      442    ;$FILENAME drm_asd_getlicenseaggregatedata.src
RSRC_FUNCLET__SCANNODEFORATTRIBUTEW    equ      443    ;$FILENAME _scannodeforattributew.src
RSRC_FUNCLET__GETXMLNODECDATAW    equ      444    ;$FILENAME _getxmlnodecdataw.src
RSRC_FUNCLET_DRM_XML_GETNODEATTRIBUTE    equ      445    ;$FILENAME drm_xml_getnodeattribute.src
RSRC_FUNCLET_DRM_XML_GETNODECDATA    equ      446    ;$FILENAME drm_xml_getnodecdata.src
RSRC_FUNCLET_DRM_DCP_GETATTRIBUTE    equ      447    ;$FILENAME drm_dcp_getattribute.src
RSRC_FUNCLET_DRM_DCP_LOADPROPERTIESCACHE    equ      448    ;$FILENAME drm_dcp_loadpropertiescache.src
RSRC_FUNCLET__VERIFYSYMMERTICSIGNATURE    equ      449    ;$FILENAME _verifysymmerticsignature.src
RSRC_FUNCLET_DRM_DDC_GETDEVICECERTIFICATE    equ      450    ;$FILENAME drm_ddc_getdevicecertificate.src
RSRC_FUNCLET__CREATEDEVICESTORE    equ      451    ;$FILENAME _createdevicestore.src
RSRC_FUNCLET__FREEDRMMANAGERINTERNALCONTEXTS    equ      452    ;$FILENAME _freedrmmanagerinternalcontexts.src
RSRC_FUNCLET__SETUPLICEVALOBJECTTOSHARE    equ      453    ;$FILENAME _setuplicevalobjecttoshare.src
RSRC_FUNCLET__MAPDRMERROR    equ      454    ;$FILENAME _mapdrmerror.src
RSRC_FUNCLET__COMPAREMACHINEID    equ      455    ;$FILENAME _comparemachineid.src
RSRC_FUNCLET_INITMGRCONTEXT    equ      456    ;$FILENAME initmgrcontext.src
RSRC_FUNCLET_DRM_MGR_INITIALIZE    equ      457    ;$FILENAME drm_mgr_initialize.src
RSRC_FUNCLET_DRM_MGR_UNINITIALIZE    equ      458    ;$FILENAME drm_mgr_uninitialize.src
RSRC_FUNCLET_DRM_MGR_SETV2HEADER    equ      459    ;$FILENAME drm_mgr_setv2header.src
RSRC_FUNCLET_DRM_MGR_BIND    equ      460    ;$FILENAME drm_mgr_bind.src
RSRC_FUNCLET_DRM_MGR_COMMIT    equ      461    ;$FILENAME drm_mgr_commit.src
RSRC_FUNCLET_DRM_MTR_OPENID    equ      462    ;$FILENAME drm_mtr_openid.src
RSRC_FUNCLET_DRM_MTR_INCREMENTCOUNT    equ      463    ;$FILENAME drm_mtr_incrementcount.src
RSRC_FUNCLET_DRM_MTR_CLOSECONTEXT    equ      464    ;$FILENAME drm_mtr_closecontext.src
RSRC_FUNCLET_OEM_OPENFILE    equ      465    ;$FILENAME oem_openfile.src
RSRC_FUNCLET_OEM_SETENDOFFILE    equ      466    ;$FILENAME oem_setendoffile.src
RSRC_FUNCLET_OEM_WRITEFILE    equ      467    ;$FILENAME oem_writefile.src
RSRC_FUNCLET_OEM_GENRANDOMBYTES    equ      468    ;$FILENAME oem_genrandombytes.src
RSRC_FUNCLET_OEM_GETFALLBACKSIGNINGKEY    equ      469    ;$FILENAME oem_getfallbacksigningkey.src
RSRC_FUNCLET_OEM_GETDEVICECERTTEMPLATE    equ      470    ;$FILENAME oem_getdevicecerttemplate.src
RSRC_FUNCLET_OEM_SETDEVICECERT    equ      471    ;$FILENAME oem_setdevicecert.src
RSRC_FUNCLET_OEM_GETCLOCKRESETSTATE    equ      472    ;$FILENAME oem_getclockresetstate.src
RSRC_FUNCLET_OEM_SETCLOCKRESETSTATE    equ      473    ;$FILENAME oem_setclockresetstate.src
RSRC_FUNCLET__GETDRMFULLPATHNAME    equ      474    ;$FILENAME _getdrmfullpathname.src
RSRC_FUNCLET_OEM_GETUNIQUEID    equ      475    ;$FILENAME oem_getuniqueid.src
RSRC_FUNCLET__GETPRIVATEKEY    equ      476    ;$FILENAME _getprivatekey.src
RSRC_FUNCLET_OEM_GETGROUPCERTPRIVATEKEY    equ      477    ;$FILENAME oem_getgroupcertprivatekey.src
RSRC_FUNCLET__GETDEVICECERT    equ      478    ;$FILENAME _getdevicecert.src
RSRC_FUNCLET_OEM_GETDEVICECERT    equ      479    ;$FILENAME oem_getdevicecert.src
RSRC_FUNCLET_OEM_GETSECURESTOREGLOBALPASSWORDSEED    equ      480    ;$FILENAME oem_getsecurestoreglobalpasswordseed.src
RSRC_FUNCLET_DRM_SNC_OPENSTORE    equ      481    ;$FILENAME drm_snc_openstore.src
RSRC_FUNCLET_DRM_SNC_CLOSESTORE    equ      482    ;$FILENAME drm_snc_closestore.src
RSRC_FUNCLET__SETKIDSTOREDATA    equ      483    ;$FILENAME _setkidstoredata.src
RSRC_FUNCLET_DRM_SNC_DELETEKID    equ      484    ;$FILENAME drm_snc_deletekid.src
RSRC_FUNCLET_DRM_SNC_UPDATEKID    equ      485    ;$FILENAME drm_snc_updatekid.src
RSRC_FUNCLET_DRM_MGR_SETV1HEADER    equ      486    ;$FILENAME drm_mgr_setv1header.src
RSRC_FUNCLET_FUNCTIONGETVALUE_PART1    equ      487    ;$FILENAME functiongetvalue_part1.src
RSRC_FUNCLET_FUNCTIONGETVALUE_FN_DATEDIFF    equ      488    ;$FILENAME functiongetvalue_fn_datediff.src
RSRC_FUNCLET_EVAL       equ      489    ;$FILENAME eval.src
RSRC_FUNCLET_DRM_HDS_OPENNAMESPACE    equ      490    ;$FILENAME drm_hds_opennamespace.src
RSRC_FUNCLET_DRM_HDS_OPENSLOT    equ      491    ;$FILENAME drm_hds_openslot.src
RSRC_FUNCLET__APPLYDIFFSTOSTORE    equ      492    ;$FILENAME _applydiffstostore.src
RSRC_FUNCLET__VERIFYSLOTHASH    equ      493    ;$FILENAME _verifyslothash.src
RSRC_FUNCLET__HDSSEARCHSLOTINBLOCK    equ      494    ;$FILENAME _hdssearchslotinblock.src
RSRC_FUNCLET__HDSCREATENAMESPACESTORE    equ      495    ;$FILENAME _hdscreatenamespacestore.src
RSRC_FUNCLET_DRM_HDS_CREATESTORE2    equ      496    ;$FILENAME drm_hds_createstore2.src
RSRC_FUNCLET_DRM_HDS_OPENSTORE2    equ      497    ;$FILENAME drm_hds_openstore2.src
RSRC_FUNCLET__HDSPREALLOC    equ      498    ;$FILENAME _hdsprealloc.src
RSRC_FUNCLET_OEM_SYSTEMTIMETOFILETIME    equ      499    ;$FILENAME oem_systemtimetofiletime.src
RSRC_FUNCLET_OEM_FILETIMETOSYSTEMTIME    equ      500    ;$FILENAME oem_filetimetosystemtime.src
RSRC_FUNCLET_DRMCRT_MEMMOVE_MEM    equ      501    ;$FILENAME drmcrt_memmove_mem.src
RSRC_FUNCLET_OEM_GETDEVICEDATETIME    equ      502    ;$FILENAME oem_getdevicedatetime.src
RSRC_FUNCLET_DRM_LIC_VERIFYCERTCHAIN    equ      503    ;$FILENAME drm_lic_verifycertchain.src
RSRC_FUNCLET__CHECKSECURECLOCK    equ      504    ;$FILENAME _checksecureclock.src
RSRC_FUNCLET__PREPARESECURECLOCKDATAFORWRITING    equ      505    ;$FILENAME _preparesecureclockdataforwriting.src
RSRC_FUNCLET__PREPARESECURECLOCKDATAFORREADING    equ      506    ;$FILENAME _preparesecureclockdataforreading.src
RSRC_FUNCLET_DRM_CLK_INITCHECKSECURECLOCK    equ      507    ;$FILENAME drm_clk_initchecksecureclock.src
RSRC_FUNCLET_SETCOUNTEDEXPIRYCATEGORY    equ      508    ;$FILENAME setcountedexpirycategory.src
RSRC_FUNCLET_DRM_UTL_DATELESSTHAN    equ      509    ;$FILENAME drm_utl_datelessthan.src
RSRC_FUNCLET_HEXSTRINGTODWORD    equ      510    ;$FILENAME hexstringtodword.src
RSRC_FUNCLET_DRM_SHA_INIT    equ      511    ;$FILENAME drm_sha_init.src
RSRC_FUNCLET_DRM_SHA_UPDATE    equ      512    ;$FILENAME drm_sha_update.src
RSRC_FUNCLET_DRM_SHA_FINALIZE    equ      513    ;$FILENAME drm_sha_finalize.src
RSRC_FUNCLET_DRM_MD5_INIT    equ      514    ;$FILENAME drm_md5_init.src
RSRC_FUNCLET_DRM_MD5_FINAL    equ      515    ;$FILENAME drm_md5_final.src
RSRC_FUNCLET__ISCACHEDEVENT    equ      516    ;$FILENAME _iscachedevent.src
RSRC_FUNCLET_EXTRACTSTRINGTOKEN    equ      517    ;$FILENAME extractstringtoken.src
RSRC_FUNCLET_EXTRACTDATETOKEN    equ      518    ;$FILENAME extractdatetoken.src
RSRC_FUNCLET_DRM_HMAC_INIT    equ      519    ;$FILENAME drm_hmac_init.src
RSRC_FUNCLET_DRM_HMAC_UPDATE    equ      520    ;$FILENAME drm_hmac_update.src
RSRC_FUNCLET_DRM_HMAC_FINALIZE    equ      521    ;$FILENAME drm_hmac_finalize.src
RSRC_FUNCLET__HDSVERIFYSLOTCONTEXT    equ      522    ;$FILENAME _hdsverifyslotcontext.src
RSRC_FUNCLET__HDSGETPUTBLOCKHDR    equ      523    ;$FILENAME _hdsgetputblockhdr.src
RSRC_FUNCLET__HDSCOPYBLOCKBUFFER    equ      524    ;$FILENAME _hdscopyblockbuffer.src
RSRC_FUNCLET__HDSREADWRITESLOT    equ      525    ;$FILENAME _hdsreadwriteslot.src
RSRC_FUNCLET__HDSINITNSCONTEXT    equ      526    ;$FILENAME _hdsinitnscontext.src
RSRC_FUNCLET__MD5_BLOCK    equ      527    ;$FILENAME _md5_block.src
RSRC_FUNCLET_OEM_CLOSEFILE    equ      528    ;$FILENAME oem_closefile.src
RSRC_FUNCLET_EXTENDHDSFILE    equ      529    ;$FILENAME extendhdsfile.src
RSRC_FUNCLET__GETDEVICEPRIVKEY    equ      530    ;$FILENAME _getdeviceprivkey.src
RSRC_FUNCLET_GLOBALGETVARIABLE    equ      531    ;$FILENAME globalgetvariable.src
RSRC_FUNCLET__ZEROHDSCONTEXTIGNORINGCRITSEC    equ      532    ;$FILENAME _zerohdscontextignoringcritsec.src
RSRC_FUNCLET__HDSVALIDATENSENTRYFROMDISK    equ      533    ;$FILENAME _hdsvalidatensentryfromdisk.src
RSRC_FUNCLET__GENNAMESPACEKEYS    equ      534    ;$FILENAME _gennamespacekeys.src
RSRC_FUNCLET__HDSINITSLOTCONTEXT    equ      535    ;$FILENAME _hdsinitslotcontext.src
RSRC_FUNCLET_FILLSLOTHEADERTOBUFFER    equ      536    ;$FILENAME fillslotheadertobuffer.src
RSRC_FUNCLET_COPYHDSDTORE    equ      537    ;$FILENAME copyhdsdtore.src
RSRC_FUNCLET_GENERATEDEVICECERT    equ      538    ;$FILENAME generatedevicecert.src
RSRC_FUNCLET_JANUS_WRAPPER    equ      539    ;$FILENAME janus_wrapper.src
RSRC_FUNCLET_FS_SWIZZLETEXT    equ      540    ;$FILENAME fs_swizzletext.src
RSRC_FUNCLET_GENDEVICECERTIFICATE    equ      541    ;$FILENAME gendevicecertificate.src
RSRC_FUNCLET__SCANTAGA_SPLIT    equ      542    ;$FILENAME _scantaga_split.src
RSRC_FUNCLET_VARIABLESECURETIMEGETORSET    equ      543    ;$FILENAME variablesecuretimegetorset.src
RSRC_FUNCLET_GETCLOCKSTATE    equ      544    ;$FILENAME getclockstate.src
RSRC_FUNCLET_STRTOL     equ      545    ;$FILENAME strtol.src
RSRC_FUNCLET_MKTIME     equ      546    ;$FILENAME mktime.src
RSRC_FUNCLET_GMTIME     equ      547    ;$FILENAME gmtime.src
RSRC_FUNCLET_LOCALTIME    equ      548    ;$FILENAME localtime.src
RSRC_FUNCLET__STRUCT_TM_TO_SYSTEMTIME    equ      549    ;$FILENAME _struct_tm_to_systemtime.src
RSRC_FUNCLET__SYSTEMTIME_TO_STRUCT_TM    equ      550    ;$FILENAME _systemtime_to_struct_tm.src
RSRC_FUNCLET__SYSTEMTIMETOTIME_T    equ      551    ;$FILENAME _systemtimetotime_t.src
RSRC_FUNCLET_OEM_SETSYSTEMTIME    equ      552    ;$FILENAME oem_setsystemtime.src
RSRC_FUNCLET_DRM_CPHR_DECRYPT    equ      553    ;$FILENAME drm_cphr_decrypt.src
RSRC_FUNCLET_DRM_CPHR_INITDECRYPT    equ      554    ;$FILENAME drm_cphr_initdecrypt.src
RSRC_FUNCLET_DRM_CBC64UPDATE    equ      555    ;$FILENAME drm_cbc64update.src
RSRC_FUNCLET_DRM_CBC64FINALIZE    equ      556    ;$FILENAME drm_cbc64finalize.src
RSRC_FUNCLET_DRM_CBC64INVERT    equ      557    ;$FILENAME drm_cbc64invert.src
RSRC_FUNCLET_DRM_CBC64INITSTATE    equ      558    ;$FILENAME drm_cbc64initstate.src
RSRC_FUNCLET_EXTRACTLONGTOKEN    equ      559    ;$FILENAME extractlongtoken.src
RSRC_FUNCLET_EXTRACTSYMBOLTOKEN    equ      560    ;$FILENAME extractsymboltoken.src
RSRC_FUNCLET__HDSOPENEXISTINGNAMESPACE    equ      561    ;$FILENAME _hdsopenexistingnamespace.src
RSRC_FUNCLET_DRM_MTR_UPDATEDATA    equ      562    ;$FILENAME drm_mtr_updatedata.src
RSRC_FUNCLET_DRM_B64_DECODEW_INPLACE    equ      563    ;$FILENAME drm_b64_decodew_inplace.src
RSRC_FUNCLET__HDSUPDATESRN    equ      564    ;$FILENAME _hdsupdatesrn.src
RSRC_FUNCLET_GETPRIVKEY    equ      565    ;$FILENAME getprivkey.src
RSRC_FUNCLET_RETRIEVEKEYS    equ      566    ;$FILENAME retrievekeys.src
RSRC_FUNCLET_RETRIEVERANDOMDATA    equ      567    ;$FILENAME retrieverandomdata.src


;;;;;;;;;;;;;;;;
;PK crypto
;;;;;;;;;;;;;;;
RSRC_FUNCLET_TWO_ADIC_INVERSE    equ      568    ;$FILENAME two_adic_inverse.src
RSRC_FUNCLET_MP_SHIFT    equ      569    ;$FILENAME mp_shift.src
RSRC_FUNCLET_MP_SIGNIFICANT_BIT_COUNT    equ      570    ;$FILENAME mp_significant_bit_count.src
RSRC_FUNCLET_SET_IMMEDIATE    equ      571    ;$FILENAME set_immediate.src
RSRC_FUNCLET_MULTIPLY_IMMEDIATE    equ      572    ;$FILENAME multiply_immediate.src
RSRC_FUNCLET_MULTIPLY    equ      573    ;$FILENAME multiply.src
RSRC_FUNCLET_DIVIDE_PRECONDITION_1    equ      574    ;$FILENAME divide_precondition_1.src
RSRC_FUNCLET_DIVIDE_IMMEDIATE    equ      575    ;$FILENAME divide_immediate.src
RSRC_FUNCLET_ECAFFINE_EXPONENTIATION_TABULAR    equ      576    ;$FILENAME ecaffine_exponentiation_tabular.src
RSRC_FUNCLET_ECAFFINE_TABLE_CONSTRUCTION    equ      577    ;$FILENAME ecaffine_table_construction.src
RSRC_FUNCLET_ECPROJ5_TO_ECAFFINE    equ      578    ;$FILENAME ecproj5_to_ecaffine.src
RSRC_FUNCLET_ECPROJ5_SET_INFINITE    equ      579    ;$FILENAME ecproj5_set_infinite.src
RSRC_FUNCLET_ECAFFINE_ON_CURVE    equ      580    ;$FILENAME ecaffine_on_curve.src
RSRC_FUNCLET_ECAFFINE_ADDITION    equ      581    ;$FILENAME ecaffine_addition.src
RSRC_FUNCLET_ECAFFINE_ADDITION_SUBTRACTION    equ      582    ;$FILENAME ecaffine_addition_subtraction.src
RSRC_FUNCLET_ECAFFINE_ATTRIBUTES2    equ      583    ;$FILENAME ecaffine_attributes2.src
RSRC_FUNCLET_KFDESC_INITIALIZE    equ      584    ;$FILENAME kfdesc_initialize.src
RSRC_FUNCLET_KIMMEDIATE    equ      585    ;$FILENAME kimmediate.src
RSRC_FUNCLET_KPRIME_IMMEDIATER    equ      586    ;$FILENAME kprime_immediater.src
RSRC_FUNCLET_KPRIME_SQRTER    equ      587    ;$FILENAME kprime_sqrter.src
RSRC_FUNCLET_KINITIALIZE_PRIME    equ      588    ;$FILENAME kinitialize_prime.src
RSRC_FUNCLET_MOD_LUCASUV    equ      589    ;$FILENAME mod_lucasuv.src
RSRC_FUNCLET_MOD_LUCAS    equ      590    ;$FILENAME mod_lucas.src
RSRC_FUNCLET_BUCKET_MULTIPLY    equ      591    ;$FILENAME bucket_multiply.src
RSRC_FUNCLET_MOD_EXP2000    equ      592    ;$FILENAME mod_exp2000.src
RSRC_FUNCLET_MOD_EXP    equ      593    ;$FILENAME mod_exp.src
RSRC_FUNCLET_MODMUL_CHOICES1    equ      594    ;$FILENAME modmul_choices1.src
RSRC_FUNCLET_MOD_SQRT    equ      595    ;$FILENAME mod_sqrt.src
RSRC_FUNCLET_CREATE_MODULUS    equ      596    ;$FILENAME create_modulus.src
RSRC_FUNCLET_FROM_MODULAR    equ      597    ;$FILENAME from_modular.src
RSRC_FUNCLET_ADD_IMMEDIATE    equ      598    ;$FILENAME add_immediate.src
RSRC_FUNCLET_ADD_DIFF    equ      599    ;$FILENAME add_diff.src
RSRC_FUNCLET_ADD_FULL    equ      600    ;$FILENAME add_full.src
RSRC_FUNCLET_COMPARE_SUM_SAME    equ      601    ;$FILENAME compare_sum_same.src
RSRC_FUNCLET_SUB_IMMEDIATE    equ      602    ;$FILENAME sub_immediate.src
RSRC_FUNCLET_MP_INITIALIZATION    equ      603    ;$FILENAME mp_initialization.src
RSRC_FUNCLET_NEW_RANDOM_BYTES    equ      604    ;$FILENAME new_random_bytes.src
RSRC_FUNCLET_NEW_RANDOM_DIGIT_INTERVAL    equ      605    ;$FILENAME new_random_digit_interval.src
RSRC_FUNCLET_NEW_RANDOM_MOD    equ      606    ;$FILENAME new_random_mod.src
RSRC_FUNCLET_NEW_RANDOM_MOD_NONZERO    equ      607    ;$FILENAME new_random_mod_nonzero.src
RSRC_FUNCLET_NEW_RANDOM_DIGITS    equ      608    ;$FILENAME new_random_digits.src
RSRC_FUNCLET_WORDS_TO_ECAFFINE    equ      609    ;$FILENAME words_to_ecaffine.src
RSRC_FUNCLET_ECAFFINE_TO_DWORDS    equ      610    ;$FILENAME ecaffine_to_dwords.src
RSRC_FUNCLET__THREADUNSAFEPKINIT    equ      611    ;$FILENAME _threadunsafepkinit.src
RSRC_FUNCLET_PKINIT     equ      612    ;$FILENAME pkinit.src
RSRC_FUNCLET_DRM_PK_GENKEYPAIR    equ      613    ;$FILENAME drm_pk_genkeypair.src
RSRC_FUNCLET_DRM_PK_ENCRYPT    equ      614    ;$FILENAME drm_pk_encrypt.src
RSRC_FUNCLET_DRM_PK_DECRYPT    equ      615    ;$FILENAME drm_pk_decrypt.src
RSRC_FUNCLET_BYTE_ARRAY_MOD_BIGNUM    equ      616    ;$FILENAME byte_array_mod_bignum.src
RSRC_FUNCLET_FE2IPMOD    equ      617    ;$FILENAME fe2ipmod.src
RSRC_FUNCLET_DRM_PK_SIGN    equ      618    ;$FILENAME drm_pk_sign.src
RSRC_FUNCLET_DRM_PK_VERIFY    equ      619    ;$FILENAME drm_pk_verify.src
RSRC_FUNCLET_RANDOM_BYTES    equ      620    ;$FILENAME random_bytes.src
RSRC_FUNCLET_NEW_RANDOM_DWORD_INTERVAL    equ      621    ;$FILENAME new_random_dword_interval.src
RSRC_FUNCLET_MP_EXTEND    equ      622    ;$FILENAME mp_extend.src
RSRC_FUNCLET_DIGITS_TO_DWORDS    equ      623    ;$FILENAME digits_to_dwords.src
RSRC_FUNCLET_DWORDS_TO_DIGITS    equ      624    ;$FILENAME dwords_to_digits.src
RSRC_FUNCLET_DECUMULATE    equ      625    ;$FILENAME decumulate.src
RSRC_FUNCLET_EC_CONCURRENT_OPERATIONS    equ      626    ;$FILENAME ec_concurrent_operations.src
RSRC_FUNCLET_GET_EC_CONCURRENT_SLOT    equ      627    ;$FILENAME get_ec_concurrent_slot.src
RSRC_FUNCLET_ECAFFINE_TO_ECPROJ5    equ      628    ;$FILENAME ecaffine_to_ecproj5.src
RSRC_FUNCLET_SCHEDULE_AFFINE_ADD_OR_SUB    equ      629    ;$FILENAME schedule_affine_add_or_sub.src
RSRC_FUNCLET_SCHEDULE_AFFINE_ADD_AND_SUB    equ      630    ;$FILENAME schedule_affine_add_and_sub.src
RSRC_FUNCLET_KPRIME_EXPONENTIATOR1    equ      631    ;$FILENAME kprime_exponentiator1.src
RSRC_FUNCLET_KPRIME_FREER    equ      632    ;$FILENAME kprime_freer.src
RSRC_FUNCLET_KPRIME_INVERTER1    equ      633    ;$FILENAME kprime_inverter1.src
RSRC_FUNCLET_KPRIME_QUAD1_SOLVER    equ      634    ;$FILENAME kprime_quad1_solver.src
RSRC_FUNCLET_KPRIME_RANDOMIZER    equ      635    ;$FILENAME kprime_randomizer.src
RSRC_FUNCLET_KPRIME_SIZER    equ      636    ;$FILENAME kprime_sizer.src
RSRC_FUNCLET_KINVERT_MANY    equ      637    ;$FILENAME kinvert_many.src
RSRC_FUNCLET_BASEPOWER_SQUARING    equ      638    ;$FILENAME basepower_squaring.src
RSRC_FUNCLET_MOD_SHIFT    equ      639    ;$FILENAME mod_shift.src
RSRC_FUNCLET_TO_MODULAR    equ      640    ;$FILENAME to_modular.src
RSRC_FUNCLET_UNCREATE_MODULUS    equ      641    ;$FILENAME uncreate_modulus.src
RSRC_FUNCLET_COMPARE_DIFF    equ      642    ;$FILENAME compare_diff.src
RSRC_FUNCLET_NEG_MOD    equ      643    ;$FILENAME neg_mod.src
RSRC_FUNCLET_DW_TO_MODULAR    equ      644    ;$FILENAME dw_to_modular.src
RSRC_FUNCLET_LEHMER_MAT22    equ      645    ;$FILENAME lehmer_mat22.src
RSRC_FUNCLET_ESTIMATED_QUOTIENT_1    equ      646    ;$FILENAME estimated_quotient_1.src
RSRC_FUNCLET_DIV21_FAST    equ      647    ;$FILENAME div21_fast.src
RSRC_FUNCLET_DIV21      equ      648    ;$FILENAME div21.src
RSRC_FUNCLET_KFREE      equ      649    ;$FILENAME kfree.src
RSRC_FUNCLET_KADDER_2    equ      650    ;$FILENAME kadder_2.src
RSRC_FUNCLET_KIMMEDIATER_2    equ      651    ;$FILENAME kimmediater_2.src
RSRC_FUNCLET_EC_CONCURRENT_OPERATIONS_SPLIT    equ      652    ;$FILENAME ec_concurrent_operations_split.src
RSRC_FUNCLET_EC_CONCURRENT_OPERATIONS_SPLIT1    equ      653    ;$FILENAME ec_concurrent_operations_split1.src
RSRC_FUNCLET_MP_GCDEX    equ      654    ;$FILENAME mp_gcdex.src
RSRC_FUNCLET_MP_GCDEX_SPLIT1    equ      655    ;$FILENAME mp_gcdex_split1.src
;RSRC_PKCRYPTO_Y                                    equ 0 ;$FILENAME pkcrypto_y.src

RSRC_FUNCLET_AES_DEC_KEY    equ      656    ;$FILENAME aes_dec_key.src
RSRC_FUNCLET_AES_ENC_KEY    equ      657    ;$FILENAME aes_enc_key.src
RSRC_FUNCLET_JANUS_MTP_INIT    equ      658    ;$FILENAME janus_mtp_init.src
RSRC_FUNCLET_INIT_KEY_SCHEDULE    equ      659    ;$FILENAME init_key_schedule.src


RSRC_AES_DEC_TABLE      equ      660    ;$FILENAME aes_dec_table.src
RSRC_AES_KEY_TABLE      equ      661    ;$FILENAME aes_key_table.src
RSRC_AES_ENC_TABLE      equ      662    ;$FILENAME aes_enc_table.src
RSRC_JANUSX_CONST       equ      663    ;$FILENAME janusx_const.src
RSRC_JANUS_TABLES       equ      664    ;$FILENAME janus_tables.src
RSRC_JANUSY_CONST       equ      665    ;$FILENAME janusy_const.src
RSRC_JANUSY_DATA        equ      666    ;$FILENAME janusy_data.src
RSRC_DOPLAY_P           equ      667    ;$FILENAME doplay_p.src
RSRC_PKCRYPTO_P         equ      668    ;$FILENAME pkcrypto_p.src
RSRC_JANUS_COMMON       equ      669    ;$FILENAME janus_common.src
RSRC_JANUS_MTP          equ      670    ;$FILENAME janus_mtp.src
RSRC_AES_ENC_P          equ      671    ;$FILENAME aes_enc_p.src
RSRC_JANUS_X            equ      672    ;$FILENAME janusx.src
RSRC_CONST_PKCRYPTO     equ      673    ;$FILENAME const_pkcrypto.src
RSRC_JANUS_COMMON_CONST    equ      674    ;$FILENAME januscommon_const.src
RSRC_MTP_HDS_NONOVL     equ      675    ;$FILENAME mtp_hds_nonovl.src

RSRC_WRITEP             equ      676    ;$FILENAME writep.src
RSRC_MTPBOOTINIT        equ      677    ;$FILENAME mtpbootinit.src

RSRC_JANUS_SUPPORT      equ      678    ;$FILENAME janussupport.src
RSRC_AES_DEC_P          equ      679    ;$FILENAME aesdec_p.src
RSRC_MTP_DRMHDS_IMPL_P    equ      680    ;$FILENAME mtpdrmhds_impl.src
RSRC_MTP_DATABASE_P     equ      681    ;$FILENAME mtp_database.src
RSRC_MTP_DRMMD5_P       equ      682    ;$FILENAME mtp_drmmd5.src

RSRC_FUNCLET_JANUSCLEANDATASTOREMTPHANDLE    equ      683    ;$FILENAME januscleandatastoremtphandle.src
RSRC_FUNCLET_JANUSGETLICENSESTATE    equ      684    ;$FILENAME janusgetlicensestate.src
RSRC_FUNCLET_JANUSGETMETERCHALLENGE    equ      685    ;$FILENAME janusgetmeterchallenge.src
RSRC_FUNCLET_JANUSGETSECURETIMECHALLENGE    equ      686    ;$FILENAME janusgetsecuretimechallenge.src
RSRC_FUNCLET_JANUSGETSYNCLIST    equ      687    ;$FILENAME janusgetsynclist.src
RSRC_FUNCLET_MTPJANUSGETPROPERTYVALUE    equ      688    ;$FILENAME mtpjanusgetpropertyvalue.src
RSRC_FUNCLET_JANUSSETLICENSERESPONSE    equ      689    ;$FILENAME janussetlicenseresponse.src
RSRC_FUNCLET_JANUSSETMETERRESPONSE    equ      690    ;$FILENAME janussetmeterresponse.src
RSRC_FUNCLET_JANUSSETSECURETIMERESPONSE    equ      691    ;$FILENAME janussetsecuretimeresponse.src
RSRC_FUNCLET_JANUSSENDWMDRMPDCOMMAND    equ      692    ;$FILENAME janussendwmdrmpdcommand.src
RSRC_FUNCLET_JANUSPROCESSWMDRMPDREQUEST    equ      693    ;$FILENAME janusprocesswmdrmpdrequest.src

;RSRC_DISPLAY_MODULE                                equ    $ffff    ;BOGUS number that isn't ever used for this application.

;
; Funclets defined in store module
;
RSRC_FUNCLET_STOR_CLOSEFRIT    equ      694    ;$FILENAME stor_closefrit.src
RSRC_FUNCLET_STOR_OPENOHIT    equ      695    ;$FILENAME stor_openohit.src
RSRC_FUNCLET_STOR_CLOSEOHIT    equ      696    ;$FILENAME stor_closeohit.src
RSRC_FUNCLET_STOR_GETNUMOBJECTS    equ      697    ;$FILENAME stor_getnumobjects.src

RSRC_FUNCLET_STOR_FLUSH    equ      698    ;$FILENAME stor_flush.src

;
; The resources for the database APIs.
;
RSRC_DATABASE_OPEN      equ      699    ;$FILENAME database_open.src
RSRC_DATABASE_CLOSE     equ      700    ;$FILENAME database_close.src

;RSRC_TASK_MTP_CODEBANK                             equ 0 ;$FILENAME rsrc_task_mtp_codebank.src
;RSRC_TASK_USBINSERTIONMONITOR_CODEBANK             equ 0 ;$FILENAME rsrc_task_usbinsertionmonitor_codebank.src


;
; The resources for the store APIs.
; moved to load with opensession
;RSRC_STOR_ENUM                                     equ 0 ;$FILENAME ststoreenum.src
;RSRC_STOR_STORE                                    equ     RSRC_STOR_ENUM

RSRC_MMCENUMERATE       equ      701    ;$FILENAME mmcenumerate.src
RSRC_MMC_RESETDEVICE    equ      702    ;$FILENAME mmcresetdevice.src
RSRC_MMC_PROCESSCSD     equ      703    ;$FILENAME mmcprocesscsd.src
RSRC_MMC_PROCESSCID     equ      704    ;$FILENAME mmcprocesscid.src
RSRC_FUNCLET_MMCPROCESSCID2    equ      705    ;$FILENAME mmcprocesscid2.src
RSRC_MMC_DETECTPRESENCE    equ      706    ;$FILENAME mmcdetectpresence.src
RSRC_MMCSERIALNUMBERINIT    equ      707    ;$FILENAME mmcserialnumberinit.src
RSRC_MMC_CHECKWRITEPROTECT    equ      708    ;$FILENAME mmccheckwriteprotect.src

RSRC_FUNCLET_MMC_TEST_ERASE    equ      709    ;$FILENAME Funclet_mmctesterase.src
RSRC_FUNCLET_MMC_HALINIT    equ      710    ;$FILENAME Funclet_mmchalinit.src

RSRC_FATWRITE_P_CODE    equ      711    ;$FILENAME fatwritep.src

RSRC_FUNCLET_FINDFIRST    equ      712    ;$FILENAME findfirst.src
RSRC_FUNCLET_FINDNEXT    equ      713    ;$FILENAME findnext.src

RSRC_FUNCLET_MTPPUTVALUEINX    equ      714    ;$FILENAME mtpputvalueinx.src
RSRC_FUNCLET_MEMCPYFROMX    equ      715    ;$FILENAME memcpyfromx.src
RSRC_FUNCLET_MEMCPYTOX    equ      716    ;$FILENAME memcpytox.src


RSRC_FATFILESYSTEM_MTP    equ      717    ;$FILENAME rsrc_fatfilesystem_mtp.src
RSRC_FUNCLET_INIT5VSENSE    equ      718    ;$FILENAME Funclet_Init5VSense.src
RSRC_FUNCLET_DRM_EXPR_EVALUATEEXPRESSION_NO_MORE_TOKENS    equ      719    ;$FILENAME drm_expr_evaluateexpression_no_more_tokens.src

RSRC_FUNCLET_USB_DEVICE_INIT    equ      720    ;$FILENAME usb_api.src
RSRC_FUNCLET_SYSTIMEINIT                            equ 0 ;FILENAME Funclet_SysTimeInit.src
RSRC_MTPDEVICEINFODATA    equ      721    ;$FILENAME mtpdeviceinfodata.src

RSRC_FUNCLET_STFM1000_TEST_PRESENCE    equ      722    ;$FILENAME Funclet_STFM1000TestPresence.src
RSRC_FUNCLET_I2C_WRITE_TUNER_REGISTER    equ      723    ;$FILENAME Funclet_I2CWriteTunerRegister.src
RSRC_FUNCLET_I2C_RESET    equ      724    ;$FILENAME Funclet_I2CReset.src

RSRC_STORE_REBUILD      equ      725    ;$FILENAME strebuild.src
RSRC_STORE_ADDMETADATA    equ      726    ;$FILENAME staddmetadata.src

; for RestoreDriveFromBackup
RSRC_RESTORESYSDRIVE_P_CODE    equ      727    ;$FILENAME restoresysdriveMTP.src

RSRC_FUNCLET_SYSLOWRESOLUTIONADCREADVOLT    equ      728    ;$FILENAME syslowresolutionadcreadvolt.src
RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATONGETCURRENTLIMIT    equ      729    ;$FILENAME batterychargeimplementatongetcurrentlimit.src
RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONINITIALIZE    equ      730    ;$FILENAME batterychargeimplementationinitialize.src
RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONSAMPLE    equ      731    ;$FILENAME batterychargeimplementationsample.src
RSRC_FUNCLET_MISC_PROJECT_INIT                    equ      732    ;$FILENAME Funclet_MiscProjectInit.src
RSRC_FUNCLET_MARK_SYSTEM_DRIVE_CLEAN                   equ      733    ;$FILENAME Funclet_MarkSystemDriveClean.src
RSRC_FUNCLET_GET_DRIVE_FROM_TAG                 equ      734    ;$FILENAME Funclet_GetDriveNumberFromTag.src

RSRC_FUNCLET_PARSEBASICOBJECTINFO    equ      735    ;$FILENAME Funclet_parseBasicObjectInfo.src
  ENDIF ;IF (!@def(resources))
