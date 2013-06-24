#//
#//
#// Filename: mtp.mk
#// Description: The MTP makefile.
#//
#//
#// Copyright (c) SigmaTel, Inc. Unpublished
#//
#// SigmaTel, Inc.
#// Proprietary  Confidential
#//
#// This source code and the algorithms implemented therein constitute
#// confidential information and may comprise trade secrets of SigmaTel, Inc.
#// or its associates, and any use thereof is subject to the terms and
#// conditions of the Confidential Disclosure Agreement pursuant to which this
#// source code was originally received.
#//
USBMTP=TRUE
##### FUNCLETS #############
##Uncomment next 4 lines if using funclet
BLD=$(BLD) -DFUNCLET
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFUNCLET
D_FUNCLETS=TRUE
MAKE_FUNCLET_RSRC=YES
############################

##### MTP BUILD INDICATOR #############
MTP_BUILD=TRUE
MTP_ONLY=TRUE
BLD=$(BLD) -DMTP_BUILD -DD_LIBS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMTP_BUILD
############################

##### MTP FUNCLET INDICATOR #############
MTP_FUNCLET=TRUE
BLD=$(BLD) -DMTP_FUNCLET
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMTP_FUNCLET
############################

##### JANUS INDICATOR #############
ifdef D_JANUS
BLD=$(BLD) -DJANUS_BUILD
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DJANUS_BUILD
endif
############################

####################################
# march1'05 IN SDK3.0 USB MODE THESE TUNER MACROS ARE UNTESTED. Purpose was to reduce current by powering off tuner IC.
# Please uncomment the following two lines if using Philips 5757 FM Tuner
#AS_OPTS=$(AS_OPTS) -DTUNER_5757
#CC_OPTS=$(CC_OPTS) -DTUNER_5757
####################################
# Please uncomment the following two lines if using Philips 5767 FM Tuner
#AS_OPTS=$(AS_OPTS) -DTUNER_5767
#CC_OPTS=$(CC_OPTS) -DTUNER_5767
####################################
#### STFM1000 TUNER #####
# Uncomment following line if using STFM1000 Tuner
STFM_BUILD=TRUE
ifdef STFM_BUILD
STFM1000_LCD=TRUE
BLD=$(BLD) -DTUNER_STFM1000
BLD=$(BLD) -DSTFM1000_LCD=TRUE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DTUNER_STFM1000
endif

######################################
#Define USE_PLAYLIST3_MTP for music library stick bit
USE_PLAYLIST3_MTP = TRUE
ifdef USE_PLAYLIST3_MTP
BLD=$(BLD) -DUSE_PLAYLIST3_MTP
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST3_MTP
endif
######################################

####################################
include ../../config.mk
####################################

# Put here to force a default 3500 build.
D_D3500=TRUE

#Uncomment the following line to enable the USB throughput to be shown while connected to the PC.
#BLD=$(BLD) -DUSB_THROUGHPUT

#Uncomment the following line to Customize the USB Manufacturer and Product strings.
# Using Unicode characters.
#BLD=$(BLD) -DCUSTOMIZE_UNICODE

#Uncomment the following line to enable Audible Acelp decoder
#D_AUDIBLEDEC=TRUE
ifdef D_AUDIBLEDEC
BLD=$(BLD) -DAUDIBLE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DAUDIBLE
endif

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = hostlink
# points to \Mp3Firmware
RELBASE_PATH=..\..\..\..\..
ROOT    = $(RELBASE_PATH)

# these macros are project dependent
MAIN_PATH           = $(ROOT)\Projects\sdk\lcdexample
PROJ_PATH           = $(ROOT)\Projects\sdk\lcdexample\mtp
USBMSCPROJ_PATH         = $(ROOT)\Projects\sdk\lcdexample\usbmsc

PROJSYS_PATH        = $(PROJ_PATH)\System

USBMSCSYS_PATH      = $(ROOT)\Projects\sdk\lcdexample\usbmsc\System

PROJRSRC_PATH       = $(PROJ_PATH)\rsrc
PROJMAKE_PATH       = $(PROJ_PATH)\make

USB_PATH            = $(ROOT)\DeviceDriver\Media\usb

USBMSC_PATH         = $(ROOT)\DeviceDriver\Media\usbmsc
SCSI_PATH           = $(ROOT)\DeviceDriver\Media\scsi

MTP_PATH            = $(ROOT)\DeviceDriver\Media\mtp

BITMAP_PATH         = $(ROOT)\Bitmaps

MTP_INC_PATH        = $(ROOT)\Algorithms\DRM\Janus\src\h

OUTPUT_RESOURCE_BIN=$(OUTPUT_PATH)\hostrsc.bin

INC_ALL     =   $(INC_ALL);$(USBMSC_PATH);$(MTP_INC_PATH)
INC_ALL_C   =   $(INC_ALL_C) -Wc-I$(USBMSC_PATH) -Wc-I$(MTP_INC_PATH)

PROFILE_PATH      = $(ROOT)\Projects\SDK\lcdexample\mtp

INC_ALL_C =\
  $(INC_ALL_C)\
  -Wc-I$(PROFILE_PATH)


#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
include $(ROOT)\etc\registers.mk
include $(ROOT)\etc\overlay.mk
#include $(ROOT)\etc\kernel.mk
include $(ETC_PATH)\includes.mk

$(OUTPUT_INCLUDE_PATH)\sysspeed.h : $(HW_REGS_INC_PATH)\sysspeed.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////

PROJECT_H = $(OUTPUT_INCLUDE_PATH)\project.h
HWEQU_H   = $(OUTPUT_INCLUDE_PATH)\hwequ.h

MAIN_DEPENDENCIES= \
    $(USB_DEPENDENCIES) \
    $(USBMSC_DEPENDENCIES) \
    $(MTP_DEPENDENCIES) \
    $(PROJECT_H) \
    $(OUTPUT_INCLUDE_PATH)\resource.h \
    $(OUTPUT_INCLUDE_PATH)\errors.h \

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

# FAT12_PATH= $(ROOT)\FileSystem\Fat12

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL= -I$(MAIN_PATH);$(PROJRSRC_PATH);$(PROJSYS_PATH);$(INC_ALL);$(OUTPUT_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(INC_FS)


### $(BLD) is environment variable; set in batch file
#AS_OPTS=$(INC_ALL) -OJMNPRS -RDRS -gs -l -LCeIn -DTASKING $(BLD)
AS_OPTS=$(INC_ALL) -OJMPS -l -LCeIn -DBRD=REVB -DDPLUS=TRUE -DTASKING $(BLD)
LK_OPTS=-L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8 $(LK_OPTS)


CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) -DASCII_ONLY $(CC_OPTS)
#        -Wc-Dk_opt_single_fat   Moved to fat32.mk. SDK3.05 development assumed 2 fat tables and rework was done for sdk3.1.
# Line removed since no MMC/SD   -Wc-Dk_opt_dynamic_cache


### Avoids building abs files with symbols (for build speed)
ifdef D_NO_SYMBOLS
LC_OPTS = -s $(LC_OPTS)
endif


# Changing the order of library linking to avoid duplicate symbol error for FGetVendorSpecificSCSIHandler
ALLLIBLIST =    $(MTP_LIB)\
                $(ALLLIBLIST)\
                $(STANDARD_LIB_PATH)\ccr.a\
                $(MTP_JANUS_LIB)\
                $(DATABASE_LIB)\
                $(STSTORE_LIB)

#ALLLIBLIST = $(ALLLIBLIST)\
#             $(STANDARD_LIB_PATH)\ccr.a\
#             $(MTP_JANUS_LIB)\
#             $(DATABASE_LIB)\
#             $(STSTORE_LIB)\
#             $(MTP_LIB)



HWEQUCOMMON = $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\hwequ.h\
            $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h\
            $(OUTPUT_INCLUDE_PATH)\regscore.h\
            $(OUTPUT_INCLUDE_PATH)\regscodec.h\
            $(OUTPUT_INCLUDE_PATH)\regsdcdc.h\
            $(OUTPUT_INCLUDE_PATH)\regsemc.h\
            $(OUTPUT_INCLUDE_PATH)\regsgpio.h\
            $(OUTPUT_INCLUDE_PATH)\regsi2c.h\
            $(OUTPUT_INCLUDE_PATH)\regsi2s.h\
            $(OUTPUT_INCLUDE_PATH)\regsicoll.h\
            $(OUTPUT_INCLUDE_PATH)\regslradc.h\
            $(OUTPUT_INCLUDE_PATH)\regsrevision.h\
            $(OUTPUT_INCLUDE_PATH)\regsrtc.h\
            $(OUTPUT_INCLUDE_PATH)\regssdram.h\
            $(OUTPUT_INCLUDE_PATH)\regsspare.h\
            $(OUTPUT_INCLUDE_PATH)\regsspi.h\
            $(OUTPUT_INCLUDE_PATH)\regsswizzle.h\
            $(OUTPUT_INCLUDE_PATH)\regstb.h\
            $(OUTPUT_INCLUDE_PATH)\regstimer.h\

#Create hwequ list based on device
HWEQULIST = $(HWEQULIST)\
            $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\regsecc.h\
            $(OUTPUT_INCLUDE_PATH)\regsgpflash.h\
            $(OUTPUT_INCLUDE_PATH)\regsfilco.h\
            $(OUTPUT_INCLUDE_PATH)\regspwm.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h

OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\main.obj\
            $(OBJ_PATH)\usbmscproject.obj\
        $(OBJ_PATH)\usbmscproject_ovly_mem.obj\
        $(OBJ_PATH)\usbmtp_cmdoutbuf_ovly_mem.obj\
            $(OBJ_PATH)\mtpmisc.obj\
            $(OBJ_PATH)\powerinit.obj\
            $(OBJ_PATH)\flashtiming.obj\
            $(OBJ_PATH)\componentversion.obj\
            $(OBJ_PATH)\miscproject.obj\
            $(OBJ_PATH)\mtpsysirq.obj\
            $(OBJ_PATH)\mtpproject.obj\
            $(OBJ_PATH)\system.obj\
            $(OBJ_PATH)\mtp_lcd.obj\
            $(OBJ_PATH)\mtp_ids.obj\
            $(OBJ_PATH)\lowresolutionadc.obj\
            $(OBJ_PATH)\lowresolutionadcinit.obj\
            $(OBJ_PATH)\usbbattery.obj\
            $(OBJ_PATH)\usbbrownout.obj\
            $(OBJ_PATH)\fsproj.obj\
            $(OBJ_PATH)\fsutils.obj\
            $(OBJ_PATH)\fsutilsc.obj\
            $(OBJ_PATH)\fsunicode.obj\
            $(OBJ_PATH)\support_sysirq.obj\
            $(OBJ_PATH)\reentrant.obj\
            $(OBJ_PATH)\persistbits.obj\
            $(OBJ_PATH)\syserror.obj\
            $(OBJ_PATH)\mtpdeviceinfodata.obj\
            $(OBJ_PATH)\STFM1000TestPresence.obj



RSRCLIST = $(RSRCLIST)\
        $(RESOURCELIST)\
        $(PROJRSRC_PATH)\connectleft.src\
        $(PROJRSRC_PATH)\connectright.src\
        $(PROJRSRC_PATH)\font_table.src\
        $(PROJRSRC_PATH)\notes.src\
        $(PROJRSRC_PATH)\reading.src\
        $(PROJRSRC_PATH)\ready.src\
        $(PROJRSRC_PATH)\donotdisconnect.src\
        $(PROJRSRC_PATH)\writing.src\
        $(PROJRSRC_PATH)\period.src\
        $(PROJRSRC_PATH)\version.src\
        $(PROJRSRC_PATH)\lowbat.src\
        $(PROJRSRC_PATH)\fullspeed.src\
        $(PROJRSRC_PATH)\highspeed.src\
        $(OUTPUT_RSRC_PATH)\battery_00.src\
        $(OUTPUT_RSRC_PATH)\battery_01.src\
        $(OUTPUT_RSRC_PATH)\battery_02.src\
        $(OUTPUT_RSRC_PATH)\battery_03.src\
        $(OUTPUT_RSRC_PATH)\battery_04.src\
        $(OUTPUT_RSRC_PATH)\battery_05.src\
        $(OUTPUT_RSRC_PATH)\battery_06.src\
        $(OUTPUT_RSRC_PATH)\battery_07.src\
        $(OUTPUT_RSRC_PATH)\battery_08.src\
        $(OUTPUT_RSRC_PATH)\battery_09.src\
        $(OUTPUT_RSRC_PATH)\battery_10.src\
	    $(OUTPUT_RSRC_PATH)\lcd_controller_init.src\
		$(OUTPUT_RSRC_PATH)\font_PGM.src\
		$(OUTPUT_RSRC_PATH)\font_SGMs.src\
		$(OUTPUT_RSRC_PATH)\font_script_00.src\
		$(OUTPUT_RSRC_PATH)\font_scripts.src\
		$(OUTPUT_RSRC_PATH)\font_PDM.src\
		$(OUTPUT_RSRC_PATH)\font_SDMs.src\
        $(OUTPUT_RSRC_PATH)\nanddatadriveinit.src\
        $(OUTPUT_RSRC_PATH)\nanddatadrivereadsector.src\
        $(OUTPUT_RSRC_PATH)\mediastartup.src\
        $(OUTPUT_RSRC_PATH)\nandsystemdrivesetinfo.src\
        $(OUTPUT_RSRC_PATH)\mtpinitdevice.src\
        $(OUTPUT_RSRC_PATH)\mtpdeviceinfo.src\
        $(OUTPUT_RSRC_PATH)\mtpgetnumobjects.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjecthandles.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobject.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjectinfo.src\
        $(OUTPUT_RSRC_PATH)\mtpsendobject.src\
        $(OUTPUT_RSRC_PATH)\mtpsendobjectinfo.src\
        $(OUTPUT_RSRC_PATH)\mtpopensession.src\
        $(OUTPUT_RSRC_PATH)\mtpclosesession.src\
        $(OUTPUT_RSRC_PATH)\mtppowerdown.src\
        $(OUTPUT_RSRC_PATH)\mtpdeletemovecopy.src\
        $(OUTPUT_RSRC_PATH)\mtpstorage.src\
        $(OUTPUT_RSRC_PATH)\strebuild.src\
        $(OUTPUT_RSRC_PATH)\staddmetadata.src\
        $(OUTPUT_RSRC_PATH)\mtpdevprops.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjectpropdesc.src\
        $(OUTPUT_RSRC_PATH)\mtpsetobjectpropvalue.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjectpropvalue.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjectproplist.src\
        $(OUTPUT_RSRC_PATH)\mtpsetobjectproplist.src\
        $(OUTPUT_RSRC_PATH)\mtpsendobjectproplist.src\
        $(OUTPUT_RSRC_PATH)\mtpgetobjectpropsupported.src\
        $(OUTPUT_RSRC_PATH)\janussupport.src\
        $(OUTPUT_RSRC_PATH)\aesdec_p.src\
        $(OUTPUT_RSRC_PATH)\mtpdrmhds_impl.src\
        $(OUTPUT_RSRC_PATH)\mtp_database.src\
        $(OUTPUT_RSRC_PATH)\mtp_drmmd5.src\
        $(OUTPUT_RSRC_PATH)\mtpdeviceinfodata.src\
        $(OUTPUT_RSRC_PATH)\mtphandler.src\
        $(OUTPUT_RSRC_PATH)\filesystem.src\
        $(OUTPUT_RSRC_PATH)\restoresysdriveMTP.src


RSRCLIST =  $(RSRCLIST)\
            $(OUTPUT_RSRC_PATH)\rsrc_fatfilesystem_mtp.src


RSRCLIST = $(RSRCLIST)\
    $(OUTPUT_RSRC_PATH)\rsrc_display_codebank.src \
    $(OUTPUT_RSRC_PATH)\RSRC_TASK_UI_CODEBANK.src


#   $(OUTPUT_RSRC_PATH)\RSRC_TASK_USBINSERTIONMONITOR_CODEBANK.src
#   $(OUTPUT_RSRC_PATH)\RSRC_TASK_MTP_CODEBANK.src\

##### BEGIN FUNCLETS #############
ifdef MAKE_FUNCLET_RSRC


FUNCLETSRCLIST = $(FUNCLETSRCLIST)\
        $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressUnicode.src\
        $(OUTPUT_RSRC_PATH)\Funclet_LCDSetFont.src\
        $(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicode.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceUnicode.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressDBCS.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceDBCS.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetUnicodeCharacterBitmap.src\
        $(OUTPUT_RSRC_PATH)\Funclet_SysLowResourceAdcInit.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ChipGetSerialNumber.src\
        $(OUTPUT_RSRC_PATH)\Funclet_CreateNullSerialNumber.src\
        $(OUTPUT_RSRC_PATH)\Funclet_IntMediaSerialNumInit.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ConvertHexToAscii.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ChipSerialNumberInit.src\
        $(OUTPUT_RSRC_PATH)\arrangefilename.src\
        $(OUTPUT_RSRC_PATH)\changetorootdirectory.src\
        $(OUTPUT_RSRC_PATH)\clearcluster.src\
        $(OUTPUT_RSRC_PATH)\createdirectory.src\
        $(OUTPUT_RSRC_PATH)\deletecontent.src\
        $(OUTPUT_RSRC_PATH)\deleterecord.src\
        $(OUTPUT_RSRC_PATH)\fastopen.src\
        $(OUTPUT_RSRC_PATH)\fcreate.src\
        $(OUTPUT_RSRC_PATH)\filegetattrib.src\
        $(OUTPUT_RSRC_PATH)\filegetdate.src\
        $(OUTPUT_RSRC_PATH)\filesetattrib.src\
        $(OUTPUT_RSRC_PATH)\filesetdate.src\
        $(OUTPUT_RSRC_PATH)\filesystempresent.src\
        $(OUTPUT_RSRC_PATH)\readrootdirrecord.src\
        $(OUTPUT_RSRC_PATH)\setcurrentpos.src\
        $(OUTPUT_RSRC_PATH)\fsinit.src\
        $(OUTPUT_RSRC_PATH)\fsdriveinit.src\
        $(OUTPUT_RSRC_PATH)\setcwdhandle.src\
        $(OUTPUT_RSRC_PATH)\fsshutdown.src\
        $(OUTPUT_RSRC_PATH)\readdevicerecord.src\
        $(OUTPUT_RSRC_PATH)\checkspaceinrootdir.src\
        $(OUTPUT_RSRC_PATH)\fgetfasthandle.src\
        $(OUTPUT_RSRC_PATH)\freehandle.src\
        $(OUTPUT_RSRC_PATH)\updatehandlemode.src\
        $(OUTPUT_RSRC_PATH)\ishandlewriteallocated.src\
        $(OUTPUT_RSRC_PATH)\searchfreehandleallocate.src\
        $(OUTPUT_RSRC_PATH)\isfileopen.src\
        $(OUTPUT_RSRC_PATH)\iscurrworkdir.src\
        $(OUTPUT_RSRC_PATH)\changepath.src\
        $(OUTPUT_RSRC_PATH)\chdir.src\
        $(OUTPUT_RSRC_PATH)\deletetree.src\
        $(OUTPUT_RSRC_PATH)\deleteallrecords.src\
        $(OUTPUT_RSRC_PATH)\cleardata.src\
        $(OUTPUT_RSRC_PATH)\changetolowleveldir.src\
        $(OUTPUT_RSRC_PATH)\getrecordnumber.src\
        $(OUTPUT_RSRC_PATH)\fileremove.src\
        $(OUTPUT_RSRC_PATH)\charactersearch.src\
        $(OUTPUT_RSRC_PATH)\stringcompare.src\
        $(OUTPUT_RSRC_PATH)\fopen.src\
        $(OUTPUT_RSRC_PATH)\fopenw.src\
        $(OUTPUT_RSRC_PATH)\fremove.src\
        $(OUTPUT_RSRC_PATH)\fremovew.src\
        $(OUTPUT_RSRC_PATH)\mkdir.src\
        $(OUTPUT_RSRC_PATH)\rmdir.src\
        $(OUTPUT_RSRC_PATH)\rmdirw.src\
        $(OUTPUT_RSRC_PATH)\searchdirectory.src\
        $(OUTPUT_RSRC_PATH)\sethandleforsearch.src\
        $(OUTPUT_RSRC_PATH)\fclose.src\
        $(OUTPUT_RSRC_PATH)\fgetc.src\
        $(OUTPUT_RSRC_PATH)\fgets.src\
        $(OUTPUT_RSRC_PATH)\fputc.src\
        $(OUTPUT_RSRC_PATH)\fputs.src\
        $(OUTPUT_RSRC_PATH)\arrangelongfilename.src\
        $(OUTPUT_RSRC_PATH)\convert_itoa.src\
        $(OUTPUT_RSRC_PATH)\createdirrecord.src\
        $(OUTPUT_RSRC_PATH)\chksum.src\
        $(OUTPUT_RSRC_PATH)\createshortdirrecord.src\
        $(OUTPUT_RSRC_PATH)\discardtrailigperiods.src\
        $(OUTPUT_RSRC_PATH)\extractfilename.src\
        $(OUTPUT_RSRC_PATH)\extractfilenamew.src\
        $(OUTPUT_RSRC_PATH)\extractpathw.src\
        $(OUTPUT_RSRC_PATH)\extractpath.src\
        $(OUTPUT_RSRC_PATH)\findfreerecord.src\
        $(OUTPUT_RSRC_PATH)\getnamew.src\
        $(OUTPUT_RSRC_PATH)\getname.src\
        $(OUTPUT_RSRC_PATH)\isdirectoryempty.src\
        $(OUTPUT_RSRC_PATH)\isshortnamevalid.src\
        $(OUTPUT_RSRC_PATH)\longdirmatch.src\
        $(OUTPUT_RSRC_PATH)\changecase.src\
        $(OUTPUT_RSRC_PATH)\unicodetooem.src\
        $(OUTPUT_RSRC_PATH)\matchdirrecord.src\
        $(OUTPUT_RSRC_PATH)\matchdirrecordw.src\
        $(OUTPUT_RSRC_PATH)\setcwd.src\
        $(OUTPUT_RSRC_PATH)\setshortfilename.src\
        $(OUTPUT_RSRC_PATH)\generatefilenametail.src\
        $(OUTPUT_RSRC_PATH)\dbcstounicode.src\
        $(OUTPUT_RSRC_PATH)\shortdirmatch.src\
        $(OUTPUT_RSRC_PATH)\uppercase.src\
        $(OUTPUT_RSRC_PATH)\setfilename.src\
        $(OUTPUT_RSRC_PATH)\convertoshortname.src\
        $(OUTPUT_RSRC_PATH)\strcpy.src\
        $(OUTPUT_RSRC_PATH)\strcpyw.src\
        $(OUTPUT_RSRC_PATH)\strlengthw.src\
        $(OUTPUT_RSRC_PATH)\strlength.src\
        $(OUTPUT_RSRC_PATH)\discardtrailigperiodsucs3.src\
        $(OUTPUT_RSRC_PATH)\discardtrailigperiodsw.src\
        $(OUTPUT_RSRC_PATH)\strcpyucs3_2.src\
        $(OUTPUT_RSRC_PATH)\getvolumelabel.src\
        $(OUTPUT_RSRC_PATH)\setvolumelabel.src\
        $(OUTPUT_RSRC_PATH)\initchar2ptpstring.src\
        $(OUTPUT_RSRC_PATH)\catchar2ptpstring.src\
        $(OUTPUT_RSRC_PATH)\catptpstring.src\
        $(OUTPUT_RSRC_PATH)\inittime2ptpstring.src\
        $(OUTPUT_RSRC_PATH)\initptpstring2time.src\
        $(OUTPUT_RSRC_PATH)\mtpextractnumberfromptpstring.src\
        $(OUTPUT_RSRC_PATH)\mtpstringprintnumber.src\
        $(OUTPUT_RSRC_PATH)\copyptpstring.src\
        $(OUTPUT_RSRC_PATH)\copy_xptpstring.src\
        $(OUTPUT_RSRC_PATH)\isequalptpstring.src\
        $(OUTPUT_RSRC_PATH)\copyptptopackedstring.src\
        $(OUTPUT_RSRC_PATH)\copypackedtoptpstring.src\
        $(OUTPUT_RSRC_PATH)\initptparray.src\
        $(OUTPUT_RSRC_PATH)\sizeinbytesptparray.src\
        $(OUTPUT_RSRC_PATH)\findelementptparray.src\
        $(OUTPUT_RSRC_PATH)\appendelementptparray.src\
        $(OUTPUT_RSRC_PATH)\removeelementptparray.src\
        $(OUTPUT_RSRC_PATH)\copyptparray.src\
        $(OUTPUT_RSRC_PATH)\packptparray.src\
        $(OUTPUT_RSRC_PATH)\stor_isobjectvalid.src\
        $(OUTPUT_RSRC_PATH)\stor_openfrit.src\
        $(OUTPUT_RSRC_PATH)\stor_closefrit.src\
        $(OUTPUT_RSRC_PATH)\stor_openohit.src\
        $(OUTPUT_RSRC_PATH)\stor_closeohit.src\
        $(OUTPUT_RSRC_PATH)\stor_addobjecttoindextable.src\
        $(OUTPUT_RSRC_PATH)\stor_deleteobjectfromindextable.src\
        $(OUTPUT_RSRC_PATH)\stor_getnumobjects.src\
        $(OUTPUT_RSRC_PATH)\stor_flush.src\
        $(OUTPUT_RSRC_PATH)\fatwritep.src\
        $(OUTPUT_RSRC_PATH)\mtpgetlittleendianint32inx.src\
        $(OUTPUT_RSRC_PATH)\mtpgetlittleendianint16inx.src\
        $(OUTPUT_RSRC_PATH)\mtpputvalueinx.src\
        $(OUTPUT_RSRC_PATH)\memcpyfromx.src\
        $(OUTPUT_RSRC_PATH)\memcpytox.src\
        $(OUTPUT_RSRC_PATH)\Funclet_mmctesterase.src\
        $(OUTPUT_RSRC_PATH)\Funclet_mmchalinit.src\
        $(OUTPUT_RSRC_PATH)\mmcenumerate.src\
        $(OUTPUT_RSRC_PATH)\mmcresetdevice.src\
        $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src\
        $(OUTPUT_RSRC_PATH)\mmcprocesscid.src\
        $(OUTPUT_RSRC_PATH)\mmcprocesscid2.src\
        $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src\
        $(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src\
        $(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src\
        $(OUTPUT_RSRC_PATH)\two_adic_inverse.src\
        $(OUTPUT_RSRC_PATH)\mp_shift.src\
        $(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src\
        $(OUTPUT_RSRC_PATH)\set_immediate.src\
        $(OUTPUT_RSRC_PATH)\multiply_immediate.src\
        $(OUTPUT_RSRC_PATH)\multiply.src\
        $(OUTPUT_RSRC_PATH)\divide_precondition_1.src\
        $(OUTPUT_RSRC_PATH)\divide_immediate.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src\
        $(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src\
        $(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_addition.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src\
        $(OUTPUT_RSRC_PATH)\kfdesc_initialize.src\
        $(OUTPUT_RSRC_PATH)\kimmediate.src\
        $(OUTPUT_RSRC_PATH)\kprime_immediater.src\
        $(OUTPUT_RSRC_PATH)\kprime_sqrter.src\
        $(OUTPUT_RSRC_PATH)\kinitialize_prime.src\
        $(OUTPUT_RSRC_PATH)\mod_lucasuv.src\
        $(OUTPUT_RSRC_PATH)\mod_lucas.src\
        $(OUTPUT_RSRC_PATH)\bucket_multiply.src\
        $(OUTPUT_RSRC_PATH)\mod_exp2000.src\
        $(OUTPUT_RSRC_PATH)\mod_exp.src\
        $(OUTPUT_RSRC_PATH)\modmul_choices1.src\
        $(OUTPUT_RSRC_PATH)\mod_sqrt.src\
        $(OUTPUT_RSRC_PATH)\create_modulus.src\
        $(OUTPUT_RSRC_PATH)\from_modular.src\
        $(OUTPUT_RSRC_PATH)\add_immediate.src\
        $(OUTPUT_RSRC_PATH)\add_diff.src\
        $(OUTPUT_RSRC_PATH)\add_full.src\
        $(OUTPUT_RSRC_PATH)\compare_sum_same.src\
        $(OUTPUT_RSRC_PATH)\sub_immediate.src\
        $(OUTPUT_RSRC_PATH)\mp_initialization.src\
        $(OUTPUT_RSRC_PATH)\new_random_bytes.src\
        $(OUTPUT_RSRC_PATH)\new_random_dword_interval.src\
        $(OUTPUT_RSRC_PATH)\new_random_digit_interval.src\
        $(OUTPUT_RSRC_PATH)\new_random_mod.src\
        $(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src\
        $(OUTPUT_RSRC_PATH)\new_random_digits.src\
        $(OUTPUT_RSRC_PATH)\words_to_ecaffine.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src\
        $(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src\
        $(OUTPUT_RSRC_PATH)\pkinit.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src\
        $(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src\
        $(OUTPUT_RSRC_PATH)\fe2ipmod.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_sign.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_verify.src\
        $(OUTPUT_RSRC_PATH)\random_bytes.src\
        $(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src\
        $(OUTPUT_RSRC_PATH)\_testdevicekeypair.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src\
        $(OUTPUT_RSRC_PATH)\_decryptcontentkey.src\
        $(OUTPUT_RSRC_PATH)\verifychecksum.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src\
        $(OUTPUT_RSRC_PATH)\_findkeypair.src\
        $(OUTPUT_RSRC_PATH)\drm_b64_decodew.src\
        $(OUTPUT_RSRC_PATH)\drm_b64_encodew.src\
        $(OUTPUT_RSRC_PATH)\drm_mac_inv32.src\
        $(OUTPUT_RSRC_PATH)\_parselicenseattributes.src\
        $(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src\
        $(OUTPUT_RSRC_PATH)\_performactions.src\
        $(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src\
        $(OUTPUT_RSRC_PATH)\_processendofchain.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src\
        $(OUTPUT_RSRC_PATH)\drm_cphr_init.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_towlower.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_memmove.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part1.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part2.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part3.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part4.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part5.src\
        $(OUTPUT_RSRC_PATH)\performoperation_part6.src\
        $(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src\
        $(OUTPUT_RSRC_PATH)\isvalidfunction.src\
        $(OUTPUT_RSRC_PATH)\functiongetvalue.src\
        $(OUTPUT_RSRC_PATH)\globalsetvariable.src\
        $(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src\
        $(OUTPUT_RSRC_PATH)\variabledrmgetorset.src\
        $(OUTPUT_RSRC_PATH)\variablemachinegetorset.src\
        $(OUTPUT_RSRC_PATH)\variableappgetorset.src\
        $(OUTPUT_RSRC_PATH)\variablesecstategetorset.src\
        $(OUTPUT_RSRC_PATH)\variablelicensegetorset.src\
        $(OUTPUT_RSRC_PATH)\variablecontentgetorset.src\
        $(OUTPUT_RSRC_PATH)\variabledevicegetorset.src\
        $(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src\
        $(OUTPUT_RSRC_PATH)\_basicheaderchecks.src\
        $(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src\
        $(OUTPUT_RSRC_PATH)\drm_hdr_verify.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_createstore.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_init.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_uninit.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_openstore.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_closestore.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src\
        $(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src\
        $(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src\
        $(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src\
        $(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src\
        $(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src\
        $(OUTPUT_RSRC_PATH)\_writesrn.src\
        $(OUTPUT_RSRC_PATH)\_writecommonblockheader.src\
        $(OUTPUT_RSRC_PATH)\_writechildblockheader.src\
        $(OUTPUT_RSRC_PATH)\_readdatablockheader.src\
        $(OUTPUT_RSRC_PATH)\_writedatablockheader.src\
        $(OUTPUT_RSRC_PATH)\_hdsexpandstore.src\
        $(OUTPUT_RSRC_PATH)\_hdsallocblock.src\
        $(OUTPUT_RSRC_PATH)\_hdsfreeblock.src\
        $(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src\
        $(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src\
        $(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src\
        $(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src\
        $(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src\
        $(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src\
        $(OUTPUT_RSRC_PATH)\_hdsremoveslot.src\
        $(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src\
        $(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src\
        $(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src\
        $(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src\
        $(OUTPUT_RSRC_PATH)\_hdsslotresize.src\
        $(OUTPUT_RSRC_PATH)\_isnull.src\
        $(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src\
        $(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src\
        $(OUTPUT_RSRC_PATH)\_readsrn.src\
        $(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src\
        $(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src\
        $(OUTPUT_RSRC_PATH)\_hdsloadsrn.src\
        $(OUTPUT_RSRC_PATH)\_hdsslotwrite.src\
        $(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src\
        $(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src\
        $(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src\
        $(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src\
        $(OUTPUT_RSRC_PATH)\getlspubkey.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src\
        $(OUTPUT_RSRC_PATH)\_loadproritizedlist.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_open.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_close.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_initenum.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src\
        $(OUTPUT_RSRC_PATH)\_processextensions.src\
        $(OUTPUT_RSRC_PATH)\_processidlist.src\
        $(OUTPUT_RSRC_PATH)\_processexclusions.src\
        $(OUTPUT_RSRC_PATH)\_processinclusions.src\
        $(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src\
        $(OUTPUT_RSRC_PATH)\_getopllevel.src\
        $(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src\
        $(OUTPUT_RSRC_PATH)\_loadattributesintocache.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src\
        $(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_closekey.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src\
        $(OUTPUT_RSRC_PATH)\overlappingdates.src\
        $(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src\
        $(OUTPUT_RSRC_PATH)\neginfdate.src\
        $(OUTPUT_RSRC_PATH)\infdate.src\
        $(OUTPUT_RSRC_PATH)\isexpired.src\
        $(OUTPUT_RSRC_PATH)\getsecstateattr.src\
        $(OUTPUT_RSRC_PATH)\setexpirycategory.src\
        $(OUTPUT_RSRC_PATH)\getv2licenseinfo.src\
        $(OUTPUT_RSRC_PATH)\getnextlicense.src\
        $(OUTPUT_RSRC_PATH)\aggregate.src\
        $(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src\
        $(OUTPUT_RSRC_PATH)\_scannodeforattributew.src\
        $(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src\
        $(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src\
        $(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src\
        $(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src\
        $(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src\
        $(OUTPUT_RSRC_PATH)\_createdevicestore.src\
        $(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src\
        $(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src\
        $(OUTPUT_RSRC_PATH)\_mapdrmerror.src\
        $(OUTPUT_RSRC_PATH)\_comparemachineid.src\
        $(OUTPUT_RSRC_PATH)\initmgrcontext.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_bind.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_commit.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_openid.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src\
        $(OUTPUT_RSRC_PATH)\oem_openfile.src\
        $(OUTPUT_RSRC_PATH)\oem_setendoffile.src\
        $(OUTPUT_RSRC_PATH)\oem_writefile.src\
        $(OUTPUT_RSRC_PATH)\oem_genrandombytes.src\
        $(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src\
        $(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src\
        $(OUTPUT_RSRC_PATH)\oem_setdevicecert.src\
        $(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src\
        $(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src\
        $(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src\
        $(OUTPUT_RSRC_PATH)\oem_getuniqueid.src\
        $(OUTPUT_RSRC_PATH)\_getprivatekey.src\
        $(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src\
        $(OUTPUT_RSRC_PATH)\_getdevicecert.src\
        $(OUTPUT_RSRC_PATH)\oem_getdevicecert.src\
        $(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src\
        $(OUTPUT_RSRC_PATH)\printlicensestatedata.src\
        $(OUTPUT_RSRC_PATH)\doplay.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_openstore.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_closestore.src\
        $(OUTPUT_RSRC_PATH)\_setkidstoredata.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src\
        $(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src\
        $(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src\
        $(OUTPUT_RSRC_PATH)\eval.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_openslot.src\
        $(OUTPUT_RSRC_PATH)\_applydiffstostore.src\
        $(OUTPUT_RSRC_PATH)\_verifyslothash.src\
        $(OUTPUT_RSRC_PATH)\_hdssearchslotinblock.src\
        $(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src\
        $(OUTPUT_RSRC_PATH)\_hdsprealloc.src\
        $(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src\
        $(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src\
        $(OUTPUT_RSRC_PATH)\gendevicecertificate.src\
        $(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src\
        $(OUTPUT_RSRC_PATH)\copyhdsdtore.src\
        $(OUTPUT_RSRC_PATH)\generatedevicecert.src\
        $(OUTPUT_RSRC_PATH)\janus_wrapper.src\
        $(OUTPUT_RSRC_PATH)\fs_swizzletext.src\
        $(OUTPUT_RSRC_PATH)\_scantaga_split.src\
        $(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src\
        $(OUTPUT_RSRC_PATH)\getclockstate.src\
        $(OUTPUT_RSRC_PATH)\strtol.src\
        $(OUTPUT_RSRC_PATH)\mktime.src\
        $(OUTPUT_RSRC_PATH)\gmtime.src\
        $(OUTPUT_RSRC_PATH)\localtime.src\
        $(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src\
        $(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src\
        $(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src\
        $(OUTPUT_RSRC_PATH)\oem_setsystemtime.src\
        $(OUTPUT_RSRC_PATH)\mp_extend.src\
        $(OUTPUT_RSRC_PATH)\digits_to_dwords.src\
        $(OUTPUT_RSRC_PATH)\dwords_to_digits.src\
        $(OUTPUT_RSRC_PATH)\decumulate.src\
        $(OUTPUT_RSRC_PATH)\ec_concurrent_operations.src\
        $(OUTPUT_RSRC_PATH)\get_ec_concurrent_slot.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_to_ecproj5.src\
        $(OUTPUT_RSRC_PATH)\schedule_affine_add_or_sub.src\
        $(OUTPUT_RSRC_PATH)\schedule_affine_add_and_sub.src\
        $(OUTPUT_RSRC_PATH)\ecaffine_multiply.src\
        $(OUTPUT_RSRC_PATH)\kprime_exponentiator1.src\
        $(OUTPUT_RSRC_PATH)\kprime_freer.src\
        $(OUTPUT_RSRC_PATH)\kprime_inverter1.src\
        $(OUTPUT_RSRC_PATH)\kprime_quad1_solver.src\
        $(OUTPUT_RSRC_PATH)\kprime_randomizer.src\
        $(OUTPUT_RSRC_PATH)\kprime_sizer.src\
        $(OUTPUT_RSRC_PATH)\kinvert_many.src\
        $(OUTPUT_RSRC_PATH)\basepower_squaring.src\
        $(OUTPUT_RSRC_PATH)\mod_shift.src\
        $(OUTPUT_RSRC_PATH)\to_modular.src\
        $(OUTPUT_RSRC_PATH)\uncreate_modulus.src\
        $(OUTPUT_RSRC_PATH)\compare_diff.src\
        $(OUTPUT_RSRC_PATH)\neg_mod.src\
        $(OUTPUT_RSRC_PATH)\dw_to_modular.src\
        $(OUTPUT_RSRC_PATH)\lehmer_mat22.src\
        $(OUTPUT_RSRC_PATH)\div21_fast.src\
        $(OUTPUT_RSRC_PATH)\div21.src\
        $(OUTPUT_RSRC_PATH)\kfree.src\
        $(OUTPUT_RSRC_PATH)\kadder_2.src\
        $(OUTPUT_RSRC_PATH)\kimmediater_2.src\
        $(OUTPUT_RSRC_PATH)\ec_concurrent_operations_split.src\
        $(OUTPUT_RSRC_PATH)\ec_concurrent_operations_split1.src\
        $(OUTPUT_RSRC_PATH)\mp_gcdex.src\
        $(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src\
        $(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_datelessthan.src\
        $(OUTPUT_RSRC_PATH)\hexstringtodword.src\
        $(OUTPUT_RSRC_PATH)\drm_sha_init.src\
        $(OUTPUT_RSRC_PATH)\drm_sha_update.src\
        $(OUTPUT_RSRC_PATH)\drm_sha_finalize.src\
        $(OUTPUT_RSRC_PATH)\drm_md5_init.src\
        $(OUTPUT_RSRC_PATH)\drm_md5_final.src\
        $(OUTPUT_RSRC_PATH)\_iscachedevent.src\
        $(OUTPUT_RSRC_PATH)\extractstringtoken.src\
        $(OUTPUT_RSRC_PATH)\extractdatetoken.src\
        $(OUTPUT_RSRC_PATH)\drm_hmac_init.src\
        $(OUTPUT_RSRC_PATH)\drm_hmac_update.src\
        $(OUTPUT_RSRC_PATH)\drm_hmac_finalize.src\
        $(OUTPUT_RSRC_PATH)\_hdsverifyslotcontext.src\
        $(OUTPUT_RSRC_PATH)\_hdsgetputblockhdr.src\
        $(OUTPUT_RSRC_PATH)\_hdscopyblockbuffer.src\
        $(OUTPUT_RSRC_PATH)\_hdsreadwriteslot.src\
        $(OUTPUT_RSRC_PATH)\_hdsinitnscontext.src\
        $(OUTPUT_RSRC_PATH)\_md5_block.src\
        $(OUTPUT_RSRC_PATH)\oem_closefile.src\
        $(OUTPUT_RSRC_PATH)\extendhdsfile.src\
        $(OUTPUT_RSRC_PATH)\_getdeviceprivkey.src\
        $(OUTPUT_RSRC_PATH)\globalgetvariable.src\
        $(OUTPUT_RSRC_PATH)\_zerohdscontextignoringcritsec.src\
        $(OUTPUT_RSRC_PATH)\_hdsvalidatensentryfromdisk.src\
        $(OUTPUT_RSRC_PATH)\_gennamespacekeys.src\
        $(OUTPUT_RSRC_PATH)\_hdsinitslotcontext.src\
        $(OUTPUT_RSRC_PATH)\fillslotheadertobuffer.src\
        $(OUTPUT_RSRC_PATH)\_checksecureclock.src\
        $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src\
        $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src\
        $(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src\
        $(OUTPUT_RSRC_PATH)\drm_cphr_decrypt.src\
        $(OUTPUT_RSRC_PATH)\drm_cphr_initdecrypt.src\
        $(OUTPUT_RSRC_PATH)\drm_cbc64update.src\
        $(OUTPUT_RSRC_PATH)\drm_cbc64finalize.src\
        $(OUTPUT_RSRC_PATH)\drm_cbc64invert.src\
        $(OUTPUT_RSRC_PATH)\drm_cbc64initstate.src\
        $(OUTPUT_RSRC_PATH)\extractlongtoken.src\
        $(OUTPUT_RSRC_PATH)\extractsymboltoken.src\
        $(OUTPUT_RSRC_PATH)\_hdsopenexistingnamespace.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src\
        $(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src\
        $(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src\
        $(OUTPUT_RSRC_PATH)\getprivkey.src\
        $(OUTPUT_RSRC_PATH)\drm_b64_decodea.src\
        $(OUTPUT_RSRC_PATH)\drm_b64_encodea.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_decryptlicense.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_signdata.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_symmetricsign.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_asymmetrictosymmetrickey.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_rebindsymmetrickey.src\
        $(OUTPUT_RSRC_PATH)\drm_bbx_rebindslk.src\
        $(OUTPUT_RSRC_PATH)\_requiredcharsforchallenge.src\
        $(OUTPUT_RSRC_PATH)\synccounttest.src\
        $(OUTPUT_RSRC_PATH)\syncexpirationtest.src\
        $(OUTPUT_RSRC_PATH)\_syncrequired.src\
        $(OUTPUT_RSRC_PATH)\_getkidlicensedata.src\
        $(OUTPUT_RSRC_PATH)\_storerefreshrequired.src\
        $(OUTPUT_RSRC_PATH)\_writekidtolist.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src\
        $(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src\
        $(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src\
        $(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src\
        $(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src\
        $(OUTPUT_RSRC_PATH)\_hdscleanupstore.src\
        $(OUTPUT_RSRC_PATH)\_evaluatelicense.src\
        $(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetricsignature.src\
        $(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetrickey.src\
        $(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetricdata.src\
        $(OUTPUT_RSRC_PATH)\comparelicense.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_addlicense.src\
        $(OUTPUT_RSRC_PATH)\drm_lst_liccount.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_processlicenseresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_processmeterresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_generatesyncchallenge.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src\
        $(OUTPUT_RSRC_PATH)\_deletelicenses.src\
        $(OUTPUT_RSRC_PATH)\_processlicenserevocationlist.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_getalldata.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_initenum.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_enumnext.src\
        $(OUTPUT_RSRC_PATH)\_requiredcharsforbinary.src\
        $(OUTPUT_RSRC_PATH)\_requiredcharsforkid.src\
        $(OUTPUT_RSRC_PATH)\_getkiddata.src\
        $(OUTPUT_RSRC_PATH)\_createnewtid.src\
        $(OUTPUT_RSRC_PATH)\_verifysignature.src\
        $(OUTPUT_RSRC_PATH)\_createmeterstorepassword.src\
        $(OUTPUT_RSRC_PATH)\_processonekid.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_processmeterresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_getalldata.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_openenumerator.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_enumnext.src\
        $(OUTPUT_RSRC_PATH)\_getkidstoredata.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_initenum.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_enumnext.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_iscertdateok.src\
        $(OUTPUT_RSRC_PATH)\_utl_verifysignature.src\
        $(OUTPUT_RSRC_PATH)\_checkcertificate.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_checkcertificate.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_checkcert.src\
        $(OUTPUT_RSRC_PATH)\drm_pk_encryptlarge.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_verifyxmlsignature.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_verifyxmlsignatureex.src\
        $(OUTPUT_RSRC_PATH)\_pushdword.src\
        $(OUTPUT_RSRC_PATH)\_getpusheddword.src\
        $(OUTPUT_RSRC_PATH)\_alltrim2.src\
        $(OUTPUT_RSRC_PATH)\_createcontext.src\
        $(OUTPUT_RSRC_PATH)\_opennode.src\
        $(OUTPUT_RSRC_PATH)\_closecurrnode.src\
        $(OUTPUT_RSRC_PATH)\_calcnodesize.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_simulatecreatedocument.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_createdocument.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_closedocument.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_reallocdocument.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_opennode.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_closecurrnode.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_savestate.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_restorestate.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_encryptandclosecurrnode.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_getcurrnodename.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_getcontextsize.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_addattribute.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_adddata.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_requiredcharsfortag.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_writetag.src\
        $(OUTPUT_RSRC_PATH)\drm_xmb_remainingbuffer.src\
        $(OUTPUT_RSRC_PATH)\_alltrima.src\
        $(OUTPUT_RSRC_PATH)\_scantaga.src\
        $(OUTPUT_RSRC_PATH)\_scannodeforattributea.src\
        $(OUTPUT_RSRC_PATH)\_checktaga.src\
        $(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src\
        $(OUTPUT_RSRC_PATH)\drm_lic_verifycertchain.src\
        $(OUTPUT_RSRC_PATH)\_verifycachedcertificate.src\
        $(OUTPUT_RSRC_PATH)\_addcachedcertificate.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_getdata.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_setdata.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_getnodea.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_getsubnodea.src\
        $(OUTPUT_RSRC_PATH)\_readtidfrommeteringstore.src\
        $(OUTPUT_RSRC_PATH)\_writetidtometeringstore.src\
        $(OUTPUT_RSRC_PATH)\_parsecontroldata.src\
        $(OUTPUT_RSRC_PATH)\drm_la_processresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_processcommand.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_processrequest.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_getlicensestate.src\
        $(OUTPUT_RSRC_PATH)\drm_snc_generatesyncchallenge.src\
        $(OUTPUT_RSRC_PATH)\drm_utl_dasstrstringsequal.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_generatemeterchallenge.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_generatemeterchallenge.src\
        $(OUTPUT_RSRC_PATH)\_attemptgeneratemeterchallenge.src\
        $(OUTPUT_RSRC_PATH)\_buildchallenge.src\
        $(OUTPUT_RSRC_PATH)\_processallkids.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_clkprocessresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_clkgeneratechallenge.src\
        $(OUTPUT_RSRC_PATH)\_numbertoformatstring.src\
        $(OUTPUT_RSRC_PATH)\drm_clk_processresponse.src\
        $(OUTPUT_RSRC_PATH)\drm_clk_getsecureclock.src\
        $(OUTPUT_RSRC_PATH)\drm_mgr_getsecureclock.src\
        $(OUTPUT_RSRC_PATH)\drm_clk_createchallenge.src\
        $(OUTPUT_RSRC_PATH)\_hdsblockscaninit.src\
        $(OUTPUT_RSRC_PATH)\_ismatchingkey.src\
        $(OUTPUT_RSRC_PATH)\_coalesceblock.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_slotenumreloadcurrent.src\
        $(OUTPUT_RSRC_PATH)\drm_hds_blockscandelete.src\
        $(OUTPUT_RSRC_PATH)\_enumxmlsubnodew.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_enumnextnode.src\
        $(OUTPUT_RSRC_PATH)\drm_xml_enumnextsubnode.src\
        $(OUTPUT_RSRC_PATH)\_initslot.src\
        $(OUTPUT_RSRC_PATH)\_loadslot.src\
        $(OUTPUT_RSRC_PATH)\drm_sst_enumloadcurrent.src\
        $(OUTPUT_RSRC_PATH)\drm_mtr_enumloadcurrent.src\
        $(OUTPUT_RSRC_PATH)\retrievekeys.src\
        $(OUTPUT_RSRC_PATH)\retrieverandomdata.src\
        $(OUTPUT_RSRC_PATH)\aes_dec_key.src\
        $(OUTPUT_RSRC_PATH)\aes_enc_key.src\
        $(OUTPUT_RSRC_PATH)\janus_mtp_init.src\
        $(OUTPUT_RSRC_PATH)\init_key_schedule.src\
        $(OUTPUT_RSRC_PATH)\januscleandatastoremtphandle.src\
        $(OUTPUT_RSRC_PATH)\janusgetlicensestate.src\
        $(OUTPUT_RSRC_PATH)\janusgetmeterchallenge.src\
        $(OUTPUT_RSRC_PATH)\janusgetsecuretimechallenge.src\
        $(OUTPUT_RSRC_PATH)\janusgetsynclist.src\
        $(OUTPUT_RSRC_PATH)\mtpjanusgetpropertyvalue.src\
        $(OUTPUT_RSRC_PATH)\janussetlicenseresponse.src\
        $(OUTPUT_RSRC_PATH)\janussetmeterresponse.src\
        $(OUTPUT_RSRC_PATH)\janussetsecuretimeresponse.src\
        $(OUTPUT_RSRC_PATH)\janussendwmdrmpdcommand.src\
        $(OUTPUT_RSRC_PATH)\janusprocesswmdrmpdrequest.src\
        $(OUTPUT_RSRC_PATH)\aes_dec_table.src\
        $(OUTPUT_RSRC_PATH)\aes_key_table.src\
        $(OUTPUT_RSRC_PATH)\aes_enc_table.src\
        $(OUTPUT_RSRC_PATH)\janusy_const.src\
        $(OUTPUT_RSRC_PATH)\janusy_data.src\
        $(OUTPUT_RSRC_PATH)\janus_tables.src\
        $(OUTPUT_RSRC_PATH)\janusx_const.src\
        $(OUTPUT_RSRC_PATH)\const_pkcrypto.src\
        $(OUTPUT_RSRC_PATH)\janusx.src\
        $(OUTPUT_RSRC_PATH)\doplay_p.src\
        $(OUTPUT_RSRC_PATH)\pkcrypto_p.src\
        $(OUTPUT_RSRC_PATH)\janus_common.src\
        $(OUTPUT_RSRC_PATH)\janus_mtp.src\
        $(OUTPUT_RSRC_PATH)\aes_enc_p.src\
        $(OUTPUT_RSRC_PATH)\mtp_hds_nonovl.src\
        $(OUTPUT_RSRC_PATH)\januscommon_const.src\
        $(OUTPUT_RSRC_PATH)\usb_api.src\
        $(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src\
        $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TestPresence.src\
        $(OUTPUT_RSRC_PATH)\Funclet_I2CWriteTunerRegister.src\
        $(OUTPUT_RSRC_PATH)\Funclet_I2CReset.src\
        $(OUTPUT_RSRC_PATH)\SysLowResolutionAdcReadVolt.src \
		$(OUTPUT_RSRC_PATH)\Funclet_MiscProjectInit.src \
		$(OUTPUT_RSRC_PATH)\Funclet_MarkSystemDriveClean.src \
		$(OUTPUT_RSRC_PATH)\Funclet_GetDriveNumberFromTag.src \
        $(OUTPUT_RSRC_PATH)\BatteryChargeImplementatonGetCurrentLimit.src \
        $(OUTPUT_RSRC_PATH)\BatteryChargeImplementationInitialize.src \
        $(OUTPUT_RSRC_PATH)\Funclet_parseBasicObjectInfo.src \
        $(OUTPUT_RSRC_PATH)\BatteryChargeImplementationSample.src


endif



##### END FUNCLETS #############

MAKELIST =  $(PSUILIST) $(OBJLIST) $(ALLLIBLIST) $(SYMBOLLIST)

ALL : $(OUTPUT_RESOURCE_BIN) $(OUTPUT_PATH)\$(PROJ).s
#ALL : $(OUTPUT_PATH)\$(PROJ).abs


#########################################
#
include $(ROOT)\etc\ddildl.mk
#########################################
#

######################################
#include the filesystem makefile
include $(ROOT)\etc\fat32.mk
######################################

#
# Include the build for the store.
#
include $(ETC_PATH)\store.mk

#
include $(ROOT)\etc\NANDDataDrive.mk
#########################################
#
include $(ROOT)\etc\NANDCommonDdi.mk
#########################################
#
include $(ROOT)\etc\NANDSystemDrive.mk
#########################################
#
include $(ROOT)\etc\NANDMedia.mk
#########################################
#
include $(ROOT)\etc\NANDHal.mk
#########################################

include $(ROOT)\etc\PortHil.mk


#########################################
#
include $(ROOT)\etc\ResourceManager.mk
#########################################

######################################
# Add janus Build Files
#include the Janus decoder makefile
include $(RELBASE_PATH)\etc\janus_mtp.mk
######################################

######################################
#include the USB MTP makefile
include $(RELBASE_PATH)\etc\usbmtp.mk
######################################


######################################
#include the USB MSC makefile
include $(RELBASE_PATH)\etc\usbmsc.mk
######################################


######################################
#include the System Serial Number makefile
include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################
#Display module
ifdef D_CLCD
include $(RELBASE_PATH)\etc\display_task_color.mk
include $(RELBASE_PATH)\etc\samsung_s6b33b0a.mk
include $(RELBASE_PATH)\etc\colorlcdsal.mk
else
include $(RELBASE_PATH)\etc\display_task.mk
include $(RELBASE_PATH)\etc\epson_sed15xx.mk

# needed for directtolcd
$(OUTPUT_INCLUDE_PATH)\sysresources.h : $(INC_PATH)\sysresources.inc
    @echo Converting inc files to h file for $!
    $(INCTOC_EXE) $! $@

# To enable writing directly to the LCD (saves 299 words of Y), comment out the include of framebuffer.mk
# and uncomment the include "directtolcd.mk" line. For better LCD perf, use framebuffer.mk and comment out directtolcd.mk
# History: apps engr did not like the lcd perf that resulted from simply using directtolcd.mk so mtp sdk3.05 uses framebuffer (evaluate it for mtp).
include $(RELBASE_PATH)\etc\framebuffer.mk
#include $(RELBASE_PATH)\etc\directtolcd.mk
endif
######################################
include $(ROOT)\etc\stringlib.mk
######################################

######################################
include $(ETC_PATH)\metadata_mtp.mk
######################################

######################################
include $(ROOT)\etc\instrument.mk
######################################

######################################
include $(ROOT)\etc\battery.mk
######################################

######################################
#Define USE_PLAYLIST3_HOST for music library stick bit
USE_PLAYLIST3_HOST = TRUE
ifdef USE_PLAYLIST3_HOST
BLD=$(BLD) -DUSE_PLAYLIST3_HOST
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST3_HOST
endif
######################################

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////


"$(OUTPUT_PATH)\$(PROJ).out" :  mtp.mk $(HWEQULIST) $(PROJMAKE_PATH)\stmp3500.dsc $(MAKELIST) $(STANDARDLIBLIST)
    @echo Linking $(PROJ).out
    $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) $(match .sym $!)


#///////////////////////////////////////////////////////////////////////////////
# Locate sections from .out file to an .abs file per .dsc spec
# Only primary abs file's clusters contribute to final .s/.sb so make sure your static section is in the primary abs
STMP3500_DSC_LIST = $(CPU_PATH)\P_FuncletCluster_MTP.dsc    \
                    $(CPU_PATH)\P_MediaDDCluster_MSC.dsc    \
                    $(CPU_PATH)\P_MTPCluster.dsc            \
                    $(CPU_PATH)\mtp_tables.dsc              \
                    $(CPU_PATH)\mtp.cpu                     \
                    $(CPU_PATH)\mtp.mem                     \
                    $(PROJMAKE_PATH)\stmp3500.dsc           \
                    $(CPU_PATH)\ycopy.dsc                   \
                    $(CPU_PATH)\copy1.dsc                   \
                    $(CPU_PATH)\copy2.dsc                   \
                    $(CPU_PATH)\copy3.dsc                   \
                    $(CPU_PATH)\copy4.dsc                   \
                    $(CPU_PATH)\copy5.dsc                   \
                    $(CPU_PATH)\copy6.dsc                   \
                    $(CPU_PATH)\copy7.dsc                   \
                    $(CPU_PATH)\copy8.dsc                   \
                    $(CPU_PATH)\copy9.dsc                   \
                    $(CPU_PATH)\copy10.dsc                  \
                    $(CPU_PATH)\copy11.dsc                  \
                    $(CPU_PATH)\copy12.dsc                  \
                    $(CPU_PATH)\copy13.dsc                  \
                    $(CPU_PATH)\copy14.dsc                  \
                    $(CPU_PATH)\copylast.dsc

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" $(STMP3500_DSC_LIST)
"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" master.dsc
    @echo Creating IEEE-695 absolute file $@
    $(LC_EXE) -o $@ $(LC_OPTS) -dmaster.dsc $(OUTPUT_PATH)\$(PROJ).out



#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
$(OUTPUT_RESOURCE_BIN) : $(RSRCLIST) $(FUNCLETSRCLIST)
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(OUTPUT_INCLUDE_PATH)\resource.inc \
    -i$(ROOT)\fonts\iso8859\ \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(BITMAP_PATH)\icons\status\16 \
    -i$(BITMAP_PATH)\icons\battery\8x8 \
    -i$(BITMAP_PATH)\strings\arial_8 \
    -i$(RESOURCE_PATH)\shingyih \
    -i$(OUTPUT_RSRC_PATH) -i$(PROJRSRC_PATH) \
    -o$@ -a \
    -v$(VERSION)
    @$(BIN_PATH)\resparse $@ $(OUTPUT_PATH)\resource.txt


$(OUTPUT_RSRC_PATH)\version.src     :$(PROJRSRC_PATH)\version.bmp
$(OUTPUT_RSRC_PATH)\connectleft.src :$(PROJRSRC_PATH)\connectleft.bmp
$(OUTPUT_RSRC_PATH)\connectright.src:$(PROJRSRC_PATH)\connectright.bmp
$(OUTPUT_RSRC_PATH)\notes.src       :$(PROJRSRC_PATH)\notes.bmp
$(OUTPUT_RSRC_PATH)\reading.src     :$(PROJRSRC_PATH)\reading.bmp
$(OUTPUT_RSRC_PATH)\ready.src       :$(PROJRSRC_PATH)\ready.bmp
$(OUTPUT_RSRC_PATH)\donotdisconnect.src :$(PROJRSRC_PATH)\donotdisconnect.bmp
$(OUTPUT_RSRC_PATH)\writing.src     :$(PROJRSRC_PATH)\writing.bmp
$(OUTPUT_RSRC_PATH)\period.src     :$(PROJRSRC_PATH)\period.bmp
$(OUTPUT_RSRC_PATH)\lowbat.src     :$(PROJRSRC_PATH)\lowbat.bmp
ifdef D_CLCD
$(OUTPUT_RSRC_PATH)\fullspeed.src      :$(PROJRSRC_PATH)\color_rsrc\fullspeed.bmp
$(OUTPUT_RSRC_PATH)\highspeed.src      :$(PROJRSRC_PATH)\color_rsrc\highspeed.bmp
else
$(OUTPUT_RSRC_PATH)\fullspeed.src      :$(PROJRSRC_PATH)\fullspeed.bmp
$(OUTPUT_RSRC_PATH)\highspeed.src      :$(PROJRSRC_PATH)\highspeed.bmp
endif
$(OUTPUT_RSRC_PATH)\battery_00.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_00.bmp
$(OUTPUT_RSRC_PATH)\battery_01.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_01.bmp
$(OUTPUT_RSRC_PATH)\battery_02.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_02.bmp
$(OUTPUT_RSRC_PATH)\battery_03.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_03.bmp
$(OUTPUT_RSRC_PATH)\battery_04.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_04.bmp
$(OUTPUT_RSRC_PATH)\battery_05.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_05.bmp
$(OUTPUT_RSRC_PATH)\battery_06.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_06.bmp
$(OUTPUT_RSRC_PATH)\battery_07.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_07.bmp
$(OUTPUT_RSRC_PATH)\battery_08.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_08.bmp
$(OUTPUT_RSRC_PATH)\battery_09.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_09.bmp
$(OUTPUT_RSRC_PATH)\battery_10.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_10.bmp

ifdef D_CLCD
"$(OUTPUT_RSRC_PATH)\font_PGM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_PGM.src $(OUTPUT_RSRC_PATH)\font_PGM.src

"$(OUTPUT_RSRC_PATH)\font_SGMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_SGMs.src $(OUTPUT_RSRC_PATH)\font_SGMs.src

"$(OUTPUT_RSRC_PATH)\font_script_00.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_script_00.src $(OUTPUT_RSRC_PATH)\font_script_00.src

"$(OUTPUT_RSRC_PATH)\font_scripts.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_scripts.src $(OUTPUT_RSRC_PATH)\font_scripts.src

"$(OUTPUT_RSRC_PATH)\font_PDM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\color_PDM.src $(OUTPUT_RSRC_PATH)\font_PDM.src

"$(OUTPUT_RSRC_PATH)\font_SDMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\color_SDMs.src $(OUTPUT_RSRC_PATH)\font_SDMs.src

else #D_CLCD
"$(OUTPUT_RSRC_PATH)\font_PGM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_PGM.src $(OUTPUT_RSRC_PATH)\font_PGM.src

"$(OUTPUT_RSRC_PATH)\font_SGMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_SGMs.src $(OUTPUT_RSRC_PATH)\font_SGMs.src

"$(OUTPUT_RSRC_PATH)\font_script_00.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_script_00.src $(OUTPUT_RSRC_PATH)\font_script_00.src

"$(OUTPUT_RSRC_PATH)\font_scripts.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_scripts.src $(OUTPUT_RSRC_PATH)\font_scripts.src

"$(OUTPUT_RSRC_PATH)\font_PDM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\iso8859_PDM.src $(OUTPUT_RSRC_PATH)\font_PDM.src

"$(OUTPUT_RSRC_PATH)\font_SDMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\iso8859_SDMs.src $(OUTPUT_RSRC_PATH)\font_SDMs.src
endif #D_CLCD

ifdef S6B33B0A_LCD
$(OUTPUT_RSRC_PATH)\lcd_controller_init.src : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\truly_lcd_init_seq_65k.src $(OUTPUT_RSRC_PATH)\lcd_controller_init.src
endif

ifdef SED15XX_LCD
$(OUTPUT_RSRC_PATH)\lcd_controller_init.src : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(RESOURCE_PATH)\shingyih\system_lcd_init_seq.src $(OUTPUT_RSRC_PATH)\lcd_controller_init.src
endif

#////////////////////////////////////////////////////////////////////////////////
# MTP Overlays
#////////////////////////////////////////////////////////////////////////////////


#////////////////////////////////////////////////////////////////////////////////
# NAND overlays
#////////////////////////////////////////////////////////////////////////////////

$(OUTPUT_RSRC_PATH)\nanddatadriveinit.src       : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup_MTP.loc
$(OUTPUT_RSRC_PATH)\nanddatadrivereadsector.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup_MTP.loc
$(OUTPUT_RSRC_PATH)\mediastartup.src            : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup_MTP.loc
$(OUTPUT_RSRC_PATH)\nandsystemdrivesetinfo.src  : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup_MTP.loc
    @echo Extract $@
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

#
# Create the tasks src files
#
#   @echo Extract $@
#   $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -Ltasks.loc -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

$(OUTPUT_RSRC_PATH)\rsrc_task_ui_codebank.src 	: $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(PROJMAKE_PATH)\tasks.loc
    @echo Extract $@
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(PROJMAKE_PATH)\tasks.loc -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

$(OUTPUT_RSRC_PATH)\mtpinitdevice.src           : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetnumobjects.src        : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjecthandles.src             : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobject.src            : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjectinfo.src        : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpsendobject.src           : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpsendobjectinfo.src       : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpdeviceinfo.src           : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpstorage.src              : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpopensession.src          : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpclosesession.src         : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtppowerdown.src            : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpdeletemovecopy.src       : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpdevprops.src             : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjectpropdesc.src    : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpsetobjectpropvalue.src   : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjectpropvalue.src   : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpsendobjectproplist.src   : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpsetobjectproplist.src    : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjectproplist.src    : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpgetobjectpropsupported.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\janussupport.src            : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\aesdec_p.src 			: $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtpdrmhds_impl.src 			: $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtp_database.src 			: $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
$(OUTPUT_RSRC_PATH)\mtp_drmmd5.src 			: $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtphandlers.loc
    @echo Extract $@
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)
$(OUTPUT_RSRC_PATH)\filesystem.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\filesystem.loc
        @echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

# TOVERIFY2: Should delete above lines since RSC_FILESYSTEM is not even in resource.inc and never loaded.

$(OUTPUT_RSRC_PATH)\rsrc_fatfilesystem_mtp.src  : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map filesystem_mtp.loc
    @echo Extract $@
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)
$(OUTPUT_RSRC_PATH)\mtpdeviceinfodata.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mtpdeviceinfodata.loc
    @echo Extract $@
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

$(OUTPUT_RSRC_PATH)\strebuild.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\strebuild.loc
    @echo Extract Database Files
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(LOC_PATH)\strebuild.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\staddmetadata.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\staddmetadata.loc
    @echo Extract Database Files
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(LOC_PATH)\staddmetadata.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

#### create dummy resources so there are no changes to resource.inc
#
#     @echo Extract $@
#  $(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\funclet_mmchalinit.src      : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\funclet_mmchalinit.src

$(OUTPUT_RSRC_PATH)\funclet_mmctesterase.src    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\funclet_mmctesterase.src

$(OUTPUT_RSRC_PATH)\mmcmediastartup.src         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcmediastartup.src

$(OUTPUT_RSRC_PATH)\mmcerase.src                : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcerase.src

$(OUTPUT_RSRC_PATH)\mmcinfo.src                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcinfo.src

$(OUTPUT_RSRC_PATH)\mmcreadwrite.src            : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcreadwrite.src

$(OUTPUT_RSRC_PATH)\mmcenumerate.src            : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcenumerate.src

$(OUTPUT_RSRC_PATH)\mmcresetdevice.src          : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcresetdevice.src

$(OUTPUT_RSRC_PATH)\mmcprocesscsd.src           : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src

$(OUTPUT_RSRC_PATH)\mmcprocesscid.src           : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscid.src

$(OUTPUT_RSRC_PATH)\mmcprocesscid2.src          : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscid2.src

$(OUTPUT_RSRC_PATH)\mmcdetectpresence.src       : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src

$(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src     : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src

$(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src




#///////////////////////////////////////////////////////////////////////////////
# Project-specific project files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\Profile_tools.obj" : "$(PROFILE_PATH)\Profile_tools.c" "$(PROFILE_PATH)\Profile_Tools.h"


$(OBJ_PATH)\system.obj      :   $(PROJ_PATH)\system\system.c
$(OBJ_PATH)\system.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h

"$(OBJ_PATH)\timestamp.obj" : $(SYS_PATH)\debug\timestamp.asm

"$(OBJ_PATH)\startup.obj" : $(SYS_COMMON_PATH)\startup.asm

"$(OBJ_PATH)\main.obj" : "$(PROJ_PATH)\main.c" $(OUTPUT_INCLUDE_PATH)\regsgpio.h $(MAIN_DEPENDENCIES)
"$(OBJ_PATH)\main.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h
"$(OBJ_PATH)\main.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h

"$(OBJ_PATH)\fsproj.obj" : $(PROJ_PATH)\fsproj.c
"$(OBJ_PATH)\mtpdeviceinfodata.obj" : $(PROJ_PATH)\mtpdeviceinfodata.c
"$(OBJ_PATH)\fsutils.obj" : $(ROOT)\FileSystem\fsutils.asm
"$(OBJ_PATH)\fsutilsc.obj" : $(ROOT)\FileSystem\fsutilsc.c
$(OBJ_PATH)\fsunicode.obj : $(ROOT)\FileSystem\fsunicode.c
$(OBJ_PATH)\reentrant.obj : $(ROOT)\FileSystem\reentrant.asm
$(OBJ_PATH)\support_sysirq.obj: $(PROJ_PATH)\support_sysirq.asm

$(OBJ_PATH)\usbmsc_ids.obj : $(USBMSCPROJ_PATH)\usbmsc_ids.c

$(OBJ_PATH)\mtp_ids.obj : $(PROJ_PATH)\mtp_ids.c

"$(OBJ_PATH)\flashtiming.obj" : "$(SYS_COMMON_PATH)\flashtiming.c"
"$(OBJ_PATH)\flashtiming.obj" : $(INC_PATH)\types.h
"$(OBJ_PATH)\flashtiming.obj" : $(OUTPUT_INCLUDE_PATH)\regsemc.h
"$(OBJ_PATH)\flashtiming.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpflash.h

"$(OBJ_PATH)\powerinit.obj" : "$(SYS_COMMON_PATH)\powerinit.c"
"$(OBJ_PATH)\powerinit.obj" : $(INC_PATH)\types.h
"$(OBJ_PATH)\powerinit.obj" : $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
"$(OBJ_PATH)\powerinit.obj" : $(OUTPUT_INCLUDE_PATH)\regsdcdc.h


"$(OBJ_PATH)\mtpmisc.obj" : "$(SYS_COMMON_PATH)\mtpmisc.c"
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscore.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsemc.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscodec.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regslradc.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsdcdc.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsswizzle.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstb.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regssdram.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstimer.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regspwm.h

"$(OBJ_PATH)\usbmscmisc.obj" : "$(SYS_COMMON_PATH)\usbmscmisc.c"
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscore.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsemc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscodec.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regslradc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsdcdc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsswizzle.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstb.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regssdram.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstimer.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regspwm.h

$(OBJ_PATH)\usbmsc_lcd.obj      :   $(USBMSCPROJ_PATH)\usbmsc_lcd.c
$(OBJ_PATH)\usbmsc_lcd.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\usbmsc_lcd.obj      :   $(MAIN_PATH)\battery_config.h

"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpflash.h
"$(OBJ_PATH)\mtpmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsfilco.h

"$(OBJ_PATH)\mediadetection.obj" : $(SYS_COMMON_PATH)\mediadetection.c

"$(OBJ_PATH)\MediaMap.obj": $(PROJ_PATH)\system\MediaMap.asm

"$(OBJ_PATH)\mediaroutines.obj" : $(SYS_COMMON_PATH)\mediaroutines.c

"$(OBJ_PATH)\copy.obj" : $(PROJ_PATH)\copy.c

"$(OBJ_PATH)\componentversion.obj" : $(SYS_COMMON_PATH)\componentversion.asm

$(OBJ_PATH)\miscproject.obj : $(PROJSYS_PATH)\miscproject.c
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\types.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\project.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\sysserialnumber.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsicoll.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regscodec.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsdcdc.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsusb20.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsrevision.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regslradc.h
$(OBJ_PATH)\miscproject.obj : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\icoll.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\ipr.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\misc.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\kernel.h
$(OBJ_PATH)\miscproject.obj : $(INC_PATH)\batterycharge.h
$(OBJ_PATH)\miscproject.obj : $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\mtpsysirq.obj : $(PROJSYS_PATH)\mtpsysirq.asm
$(OBJ_PATH)\mtpsysirq.obj : $(HW_REGS_INC_PATH)\regsdcdc.inc
$(OBJ_PATH)\mtpsysirq.obj : $(SYS_PATH)\common\sysmacro.asm

"$(OBJ_PATH)\usbproject.obj" : "$(PROJSYS_PATH)\usbproject.c" $(USB_DEPENDENCIES)

"$(OBJ_PATH)\mtpproject.obj" : "$(PROJSYS_PATH)\mtpproject.c" $(MTP_DEPENDENCIES)

"$(OBJ_PATH)\usbmscproject.obj" : "$(USBMSCSYS_PATH)\usbmscproject.c" $(USBMSC_DEPENDENCIES)

"$(OBJ_PATH)\usbmscproject_ovly_mem.obj" : "$(USBMSCSYS_PATH)\usbmscproject_ovly_mem.c" $(USBMSC_DEPENDENCIES)

"$(OBJ_PATH)\usbmtp_cmdoutbuf_ovly_mem.obj" : "$(PROJSYS_PATH)\usbmtp_cmdoutbuf_ovly_mem.c" $(MTP_DEPENDENCIES)

$(OBJ_PATH)\usbmsc_resource.obj : $(USBMSCPROJ_PATH)\usbmsc_resource.asm $(OUTPUT_PATH)\resource.asm

"$(OBJ_PATH)\modulemsg.obj" : $(MAIN_PATH)\dcc\system\modulemsg.asm

"$(OBJ_PATH)\lcd.obj" : $(HW_MSGMODULES_PATH)\graphicallcd\lcd.asm

# NOT USED IN MTP SO COMMENTED OUT AT SDK3.1
#"$(OBJ_PATH)\systime.obj" : $(SYS_COMMON_PATH)\systime.asm

$(OBJ_PATH)\bufdesc.obj : $(SYS_COMMON_PATH)\bufdesc.asm

$(OBJ_PATH)\syspostmsg.obj : $(SYS_COMMON_PATH)\syspostmsg.asm

$(OBJ_PATH)\sysmodule.obj : $(SYS_EXEC_PATH)\sysmodule.asm

$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c

$(OBJ_PATH)\mtp_lcd.obj      :   $(PROJ_PATH)\mtp_lcd.c
$(OBJ_PATH)\mtp_lcd.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\mtp_lcd.obj      :   $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\usbbattery.obj      :   $(SYS_DEVICE_PATH)\usbbattery.c
$(OBJ_PATH)\usbbattery.obj      :   $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\lowresolutionadc.obj :  $(SYS_DEVICE_PATH)\lowresolutionadc.c $(MAIN_DEPENDENCIES)

$(OBJ_PATH)\lowresolutionadcinit.obj :  $(SYS_DEVICE_PATH)\lowresolutionadcinit.c $(MAIN_DEPENDENCIES)

$(OBJ_PATH)\persistbits.obj :  $(SYS_DEVICE_PATH)\persistbits.c
$(OBJ_PATH)\persistbits.obj :  $(INC_PATH)\persist_bits.h

$(OBJ_PATH)\usbbrownout.obj : $(SYS_COMMON_PATH)\usbbrownout.asm

# Remap LCD pins if STFM1000 chip present or not
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000TestPresence.c
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsi2c.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsi2s.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsspare.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsspi.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h

# SysDefaultAllVectors to a debug instruction in DEBUG builds or default vectors to 'jmp FatalError' for RETAIL build.
# SystemHalt aka "error" macro in sysmacro.asm in a RETAIL build will jmp to vector P:00000C which
# has "jmp FatalError" installed (FatalError routine in this module resets DSP)
$(OBJ_PATH)\syserror.obj : $(SYS_COMMON_PATH)\syserror.asm





#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////

ifdef D_C_LCD
FUNCLET_MTP=funclet_color_MTP
else
FUNCLET_MTP=funclet_MTP
endif
#///////////////////////////////////////////////////////////////////////////////
# Module code extraction & encryption from s to sb file format
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=P_vector,P_MTP_cluster,P_Funclet_cluster,P_sys_cluster,P_USBMSC_cluster,P_Media_DD_Read_cluster,X_clstr,X_MTP_Cluster,Y_clstr -V$(VERSION) -OFFE8=1818 -OFFE9=1020 -OFFED=380000
        @echo Encrypt $(PROJ).s to $(PROJ).sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\$(PROJ).sb

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressUnicode.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_LCDSetFont.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicode.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceUnicode.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressDBCS.src"   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceDBCS.src"  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_GetUnicodeCharacterBitmap.src" : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_SysLowResourceAdcInit.src"     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_ChipGetSerialNumber.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_CreateNullSerialNumber.src"    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_IntMediaSerialNumInit.src"     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_ConvertHexToAscii.src"         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_ChipSerialNumberInit.src"      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\arrangefilename.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\changetorootdirectory.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\clearcluster.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\createdirectory.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\deletecontent.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\deleterecord.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fastopen.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fcreate.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\filegetattrib.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\filegetdate.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\filesetattrib.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\filesetdate.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\filesystempresent.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\readrootdirrecord.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setcurrentpos.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fsinit.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fsdriveinit.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setcwdhandle.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fsshutdown.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\readdevicerecord.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\checkspaceinrootdir.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fgetfasthandle.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\freehandle.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\updatehandlemode.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\ishandlewriteallocated.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\searchfreehandleallocate.src"          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\isfileopen.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\iscurrworkdir.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\changepath.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\chdir.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\deletetree.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\deleteallrecords.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\cleardata.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\changetolowleveldir.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\getrecordnumber.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fileremove.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\charactersearch.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stringcompare.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fopen.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fopenw.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fremove.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fremovew.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mkdir.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\rmdir.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\rmdirw.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\searchdirectory.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\sethandleforsearch.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fclose.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fgetc.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fgets.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fputc.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\fputs.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\arrangelongfilename.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\convert_itoa.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\createdirrecord.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\chksum.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\createshortdirrecord.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\discardtrailigperiods.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\extractfilename.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\extractfilenamew.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\extractpathw.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\extractpath.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\findfreerecord.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\getnamew.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\getname.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\isdirectoryempty.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\isshortnamevalid.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\longdirmatch.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\changecase.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\unicodetooem.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\matchdirrecord.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\matchdirrecordw.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setcwd.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setshortfilename.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\generatefilenametail.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\dbcstounicode.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\shortdirmatch.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\uppercase.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setfilename.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\convertoshortname.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\strcpy.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\strcpyw.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\strlengthw.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\strlength.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\discardtrailigperiodsucs3.src"         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\discardtrailigperiodsw.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\strcpyucs3_2.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\getvolumelabel.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\setvolumelabel.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\initchar2ptpstring.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\catchar2ptpstring.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\catptpstring.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\inittime2ptpstring.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\initptpstring2time.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mtpextractnumberfromptpstring.src"     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mtpstringprintnumber.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\copyptpstring.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\copy_xptpstring.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\isequalptpstring.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\packptpstring.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\pack_Xptpstring.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\copyptptopackedstring.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\copypackedtoptpstring.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\initptparray.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\sizeinbytesptparray.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\findelementptparray.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\appendelementptparray.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\removeelementptparray.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\copyptparray.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\packptparray.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_isobjectvalid.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_openfrit.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_closefrit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_openohit.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_closeohit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_addobjecttoindextable.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_deleteobjectfromindextable.src"   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_getnumobjects.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\stor_flush.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mtpgetlittleendianint32inx.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mtpgetlittleendianint16inx.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\mtpputvalueinx.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\memcpyfromx.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\memcpytox.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\usb_api.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\SysLowResolutionAdcReadVolt.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_MiscProjectInit.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_MarkSystemDriveClean.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_GetDriveNumberFromTag.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
"$(OUTPUT_RSRC_PATH)\Funclet_parseBasicObjectInfo.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\$(FUNCLET_MTP).loc
    @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\$(FUNCLET_MTP).loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\$(FUNCLET_MTP).loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\two_adic_inverse.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mp_shift.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\set_immediate.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\multiply_immediate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\multiply.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\divide_precondition_1.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\divide_immediate.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_addition.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src"         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\kfdesc_initialize.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\kimmediate.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_immediater.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_sqrter.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\kinitialize_prime.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_lucasuv.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_lucas.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\bucket_multiply.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_exp2000.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_exp.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\modmul_choices1.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_sqrt.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\create_modulus.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\from_modular.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\add_immediate.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\add_diff.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\add_full.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\compare_sum_same.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\sub_immediate.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\mp_initialization.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_bytes.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_dword_interval.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_digit_interval.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_mod.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\new_random_digits.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\words_to_ecaffine.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\pkinit.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\fe2ipmod.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_sign.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_verify.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
"$(OUTPUT_RSRC_PATH)\random_bytes.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet1_MTP.loc
    @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet1_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet1_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
ifdef D_LIION
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementationInitialize.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\batterycharge.loc
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementationSample.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\batterycharge.loc
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementatonGetCurrentLimit.src"   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\batterycharge.loc
    @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\batterycharge.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\batterycharge.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
else
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementationInitialize.src"  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\BatteryChargeImplementationInitialize.src
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementationSample.src"  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\BatteryChargeImplementationSample.src
"$(OUTPUT_RSRC_PATH)\BatteryChargeImplementatonGetCurrentLimit.src"  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\BatteryChargeImplementatonGetCurrentLimit.src
endif


"$(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src"          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_testdevicekeypair.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_decryptcontentkey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\verifychecksum.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_findkeypair.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_b64_encodew.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mac_inv32.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_parselicenseattributes.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_performactions.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processendofchain.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src"          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cphr_init.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_towlower.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part1.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part2.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part3.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part4.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part5.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\performoperation_part6.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\isvalidfunction.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\functiongetvalue.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\globalsetvariable.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variabledrmgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablemachinegetorset.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variableappgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablesecstategetorset.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablelicensegetorset.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablecontentgetorset.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variabledevicegetorset.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_basicheaderchecks.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hdr_verify.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_init.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_uninit.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_closestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writesrn.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writecommonblockheader.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writechildblockheader.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_readdatablockheader.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writedatablockheader.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsexpandstore.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsallocblock.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsfreeblock.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsremoveslot.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src"     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src"         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsslotresize.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_isnull.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_readsrn.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsloadsrn.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsslotwrite.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet2_MTP.loc
		@echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet2_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet2_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\getlspubkey.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_loadproritizedlist.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_open.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_close.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_initenum.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processextensions.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processidlist.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processexclusions.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processinclusions.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getopllevel.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_loadattributesintocache.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_closekey.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src"         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src"        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\overlappingdates.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\neginfdate.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\infdate.src"                                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\isexpired.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\getsecstateattr.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\setexpirycategory.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\getv2licenseinfo.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\getnextlicense.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\aggregate.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_scannodeforattributew.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_createdevicestore.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_mapdrmerror.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_comparemachineid.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\initmgrcontext.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_bind.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_commit.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_openid.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_openfile.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_setendoffile.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_writefile.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_genrandombytes.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_setdevicecert.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getuniqueid.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getprivatekey.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getdevicecert.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getdevicecert.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src"      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\printlicensestatedata.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\doplay.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_openstore.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_closestore.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_setkidstoredata.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\eval.src"                                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_openslot.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_applydiffstostore.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_verifyslothash.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdssearchslotinblock.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsprealloc.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\gendevicecertificate.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\copyhdsdtore.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\generatedevicecert.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\janus_wrapper.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\fs_swizzletext.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_scantaga_split.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\getclockstate.src"                				: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\strtol.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\mktime.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\gmtime.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\localtime.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_setsystemtime.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet3_MTP.loc
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet3_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet3_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\mp_extend.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\digits_to_dwords.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\dwords_to_digits.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\decumulate.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\ec_concurrent_operations.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\get_ec_concurrent_slot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_to_ecproj5.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\schedule_affine_add_or_sub.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\schedule_affine_add_and_sub.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\ecaffine_multiply.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_exponentiator1.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_freer.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_inverter1.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_quad1_solver.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_randomizer.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kprime_sizer.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kinvert_many.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\basepower_squaring.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\mod_shift.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\to_modular.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\uncreate_modulus.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\compare_diff.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\neg_mod.src"                                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\dw_to_modular.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\lehmer_mat22.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\div21_fast.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\div21.src"                                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\ec_concurrent_operations_split.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\ec_concurrent_operations_split1.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\mp_gcdex.src"                                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kfree.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kadder_2.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
"$(OUTPUT_RSRC_PATH)\kimmediater_2.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet4_MTP.loc
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet4_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet4_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src"        			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_datelessthan.src"            			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\hexstringtodword.src"                			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sha_init.src"                    			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sha_update.src"                  			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sha_finalize.src"                			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_md5_init.src"                    			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_md5_final.src"                   			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_iscachedevent.src"                  			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\extractstringtoken.src"              			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\extractdatetoken.src"                			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hmac_init.src"                   			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hmac_update.src"                 			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hmac_finalize.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsverifyslotcontext.src"           			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsgetputblockhdr.src"              			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscopyblockbuffer.src"             			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsreadwriteslot.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsinitnscontext.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_md5_block.src"                      			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_closefile.src"                   			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\extendhdsfile.src"                   			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getdeviceprivkey.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\globalgetvariable.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_zerohdscontextignoringcritsec.src"  			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsvalidatensentryfromdisk.src"     			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_gennamespacekeys.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsinitslotcontext.src"             			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\fillslotheadertobuffer.src"          			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_checksecureclock.src"               			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src"  		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src"  		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src"      		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cphr_decrypt.src"                  		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cphr_initdecrypt.src"              		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cbc64update.src"                   		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cbc64finalize.src"                 		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cbc64invert.src"                   		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_cbc64initstate.src"                		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\extractlongtoken.src"                  		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\extractsymboltoken.src"                		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsopenexistingnamespace.src"         		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src"                		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src"           		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src"                     		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
"$(OUTPUT_RSRC_PATH)\getprivkey.src"           						: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet5_MTP.loc
		@echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet5_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet5_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\drm_b64_decodea.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_b64_encodea.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_decryptlicense.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_signdata.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_symmetricsign.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_asymmetrictosymmetrickey.src"          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_rebindsymmetrickey.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_bbx_rebindslk.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_requiredcharsforchallenge.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\synccounttest.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\syncexpirationtest.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_syncrequired.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getkidlicensedata.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_storerefreshrequired.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writekidtolist.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdscleanupstore.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_evaluatelicense.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetricsignature.src"      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetrickey.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_updatelicensewithsymmetricdata.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\comparelicense.src"           					: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_addlicense.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lst_liccount.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_processlicenseresponse.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_processmeterresponse.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_generatesyncchallenge.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_deletelicenses.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processlicenserevocationlist.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_getalldata.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_initenum.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_enumnext.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_requiredcharsforbinary.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_requiredcharsforkid.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getkiddata.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_createnewtid.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_verifysignature.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_createmeterstorepassword.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processonekid.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_processmeterresponse.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_getalldata.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_openenumerator.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_enumnext.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getkidstoredata.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_initenum.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_enumnext.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_iscertdateok.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_utl_verifysignature.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_checkcertificate.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_checkcertificate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_checkcert.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_pk_encryptlarge.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_verifyxmlsignature.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_verifyxmlsignatureex.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_pushdword.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_getpusheddword.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_alltrim2.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_createcontext.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_opennode.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_closecurrnode.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_calcnodesize.src"                             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_simulatecreatedocument.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_createdocument.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_closedocument.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_reallocdocument.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_opennode.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_closecurrnode.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_savestate.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_restorestate.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_encryptandclosecurrnode.src"           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_getcurrnodename.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_getcontextsize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_addattribute.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_adddata.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_requiredcharsfortag.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_writetag.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xmb_remainingbuffer.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_alltrima.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_scantaga.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_scannodeforattributea.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_checktaga.src"                                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_lic_verifycertchain.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_verifycachedcertificate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_addcachedcertificate.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_getdata.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_setdata.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodea.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_getsubnodea.src"                       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_readtidfrommeteringstore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_writetidtometeringstore.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_parsecontroldata.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_la_processresponse.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_processcommand.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_processrequest.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_getlicensestate.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_snc_generatesyncchallenge.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_utl_dasstrstringsequal.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_generatemeterchallenge.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_generatemeterchallenge.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_attemptgeneratemeterchallenge.src"            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_buildchallenge.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_processallkids.src"                           : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_clkprocessresponse.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_clkgeneratechallenge.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_numbertoformatstring.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_clk_processresponse.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_clk_getsecureclock.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mgr_getsecureclock.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_clk_createchallenge.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_hdsblockscaninit.src"                 		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_ismatchingkey.src"                 			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_coalesceblock.src"                 			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_slotenumreloadcurrent.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_hds_blockscandelete.src"                 	: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_enumxmlsubnodew.src"                 			: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_enumnextnode.src"                 		: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_xml_enumnextsubnode.src"                 	: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_initslot.src"                 				: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\_loadslot.src"                 				: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_sst_enumloadcurrent.src"                 	: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\drm_mtr_enumloadcurrent.src"                 	: $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\retrievekeys.src"                              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\retrieverandomdata.src"                        : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\aes_dec_key.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\aes_enc_key.src"                               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janus_mtp_init.src"                            : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\init_key_schedule.src"                         : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\januscleandatastoremtphandle.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janusgetlicensestate.src"                      : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janusgetmeterchallenge.src"                    : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janusgetsecuretimechallenge.src"               : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janusgetsynclist.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\mtpjanusgetpropertyvalue.src"                  : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janussetlicenseresponse.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janussetmeterresponse.src"                     : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janussetsecuretimeresponse.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janussendwmdrmpdcommand.src"                   : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\janusprocesswmdrmpdrequest.src"                : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000TestPresence.src"              : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\Funclet_I2CWriteTunerRegister.src"             : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
"$(OUTPUT_RSRC_PATH)\Funclet_I2CReset.src"                          : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\funclet6_MTP.loc
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet6_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet6_MTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)


"$(OUTPUT_RSRC_PATH)\aes_dec_table.src"                             : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_key_table.src"                             : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_enc_table.src"                             : $(OUTPUT_PATH)\$(PROJ).abs
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\aes_enc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\aes_enc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\janusy_const.src"                              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janusy_data.src"                               : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janus_tables.src"                              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janusx_const.src"                              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janusx.src"                                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\const_pkcrypto.src"                            : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\januscommon_const.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\janus_MTP_mem.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\janus_MTP_mem.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\pkcrypto_p.src"                                : $(OUTPUT_PATH)\$(PROJ).abs
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\pkcrypto_mtp_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\pkcrypto_mtp_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\janus_common.src"                              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\doplay_p.src"                                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janus_mtp.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_enc_p.src"                                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mtp_hds_nonovl.src"       					    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\janus_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
              @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\janus_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)


ifdef D_CLCD
"$(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Display Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\display_funclet_mtp.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
else
"$(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@
endif

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressUnicode.src"	: $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceUnicode.src"	: $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressDBCS.src"	: $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceDBCS.src"	: $(OUTPUT_PATH)\$(PROJ).abs
ifdef D_CLCD
    @echo Extract Funclets $(OUTPUT_PATH)\$(PROJ).abs
    @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet_unicode_color_mtp.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
else
    @echo Extract Funclets $(OUTPUT_PATH)\$(PROJ).abs
    @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\funclet_unicode_mtp.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
endif
"$(OUTPUT_RSRC_PATH)\restoresysdriveMTP.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\RestoreSysDriveMTP.loc
        @echo Extract RestoreSysDriveMTP.src from $(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\RestoreSysDriveMTP.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)