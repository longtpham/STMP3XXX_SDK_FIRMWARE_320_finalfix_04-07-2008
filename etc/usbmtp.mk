#///////////////////////////////////////////////////////////////////////////////
#//
#// Filename: usbmtp.mk
#// Description: The usb mtp make file.
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
#///////////////////////////////////////////////////////////////////////////////
ifndef USBMTP_MK
USBMTP_MK = TRUE

INC_ALL = $(INC_ALL);$(USB_PATH)\usb20
INC_ALL_C = $(INC_ALL_C) -Wc-I$(USB_PATH)\usb20


include $(ETC_PATH)\usb.mk
include $(ETC_PATH)\usbmsc.mk


ifdef D_FUNCLETS
MTP_LIBNAME = mtp_funclets.a
else
MTP_LIBNAME = mtp.a
endif # ifdef D_FUNCLETS

ifdef D_PLIBS
MTP_LIB = $(OBJ_PATH)\$(MTP_LIBNAME)
else
MTP_LIB = $(CHIP_LIB_PATH)\$(MTP_LIBNAME)
endif # ifdef D_PLIBS

#MTP_PATH = $(ROOT)\DeviceDriver\Media\mtp
INC_ALL = $(INC_ALL);$(INC_STOR);$(AUDIBLE_MTP_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(INC_STOR) -Wc-I$(AUDIBLE_MTP_PATH)

#
# Indicate the location of the Janus MTP library.
#
MTP_JANUS_LIB =  $(ROOT)\Algorithms\DRM\Janus\lib\mtplib.a

#
# These symbols must be "undefined" to prevent them from being undefined in the
# library file.  This is not intuitive, but is required.
#
LK_OPTS = $(LK_OPTS) -uunpack_string -upack_string -uFGET_CHAR -uFStorReadObjectData
LK_OPTS = $(LK_OPTS) -uFStorWriteObjectData -uFStorIsObjectValid -uFStorGetNumObjects
LK_OPTS = $(LK_OPTS) -uFStorGetObjectList -uFStorDeleteObject -uFStorAddNewObject
LK_OPTS = $(LK_OPTS) -uFGenDeviceCertificate -uFRTCGetTime -uFDecrypt_data -uFOverlayBuffer
LK_OPTS = $(LK_OPTS) -uFResponseBuffer -uFDRM_MGR_Initialize -uFDRM_MGR_GetSecureClock
LK_OPTS = $(LK_OPTS) -uFStorGetStorageInfo -uFStorGetIDs -uFStorClose
#
# Include the resources used by this library
#
#TOVERIFY: remove line below after verif that we don't need capability object. Use ccr.a instead.
CAPABILITYREG_PATH  =$(ROOT)\libsource\capabilityregisters

RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
    $(SYMBOLS_PATH)\mtp_resource.inc

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
MTP_DEPENDENCIES= \
    "$(INC_PATH)\mtp.h" \
    "$(INC_PATH)\database.h" \
    "$(MTP_PATH)\mtprequests.h" \
    "$(MTP_PATH)\ptp.h" \
    "$(MTP_PATH)\ptpstring.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsdcdc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsrtc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regscodec.h" \
    "$(OUTPUT_INCLUDE_PATH)\regslradc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsicoll.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsemc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsgpflash.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsusb20.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsusb20phy.h"\
    $(USB_DEPENDENCIES)

ifdef D_PLIBS
# removed $(OBJ_PATH)\chipcapability.obj\ from mtplist since we use ccr.a instead.  Delete this line after ccr.a  & mtp build verif.

MTPLIST  =   \
    $(OBJ_PATH)\MtpJanusGetLicenseState.obj\
    $(OBJ_PATH)\MtpJanusSendWMDRMPDCommand.obj\
    $(OBJ_PATH)\MtpJanusSendWMDRMPDRequest.obj\
    $(OBJ_PATH)\MtpJanusSetLicenseResponse.obj\
    $(OBJ_PATH)\MtpJanusSetMeterResponse.obj\
    $(OBJ_PATH)\MtpJanusSetSecureTimeResponse.obj\
    $(OBJ_PATH)\MtpJanusGetSyncList.obj\
    $(OBJ_PATH)\MtpJanusSendMeterChallengeQuery.obj\
    $(OBJ_PATH)\MtpJanusGetSecureTimeChallenge.obj\
    $(OBJ_PATH)\MtpJanusGetMeterChallenge.obj\
    $(OBJ_PATH)\MtpJanusCleanDataStore.obj\
    $(OBJ_PATH)\mtphandler.obj\
    $(OBJ_PATH)\mtpresponse.obj\
    $(OBJ_PATH)\mtpinitdevice.obj\
    $(OBJ_PATH)\mtp.obj\
    $(OBJ_PATH)\ptpstring.obj\
    $(OBJ_PATH)\mtpgetnumobjects.obj\
    $(OBJ_PATH)\mtpgetobjecthandles.obj\
    $(OBJ_PATH)\mtpgetstorageids.obj\
    $(OBJ_PATH)\mtpgetstorageinfo.obj\
    $(OBJ_PATH)\mtpgetobjectinfo.obj\
    $(OBJ_PATH)\mtpgetobject.obj\
    $(OBJ_PATH)\mtpsendobjectinfo.obj\
    $(OBJ_PATH)\mtpsendobject.obj\
    $(OBJ_PATH)\mtpdeleteobject.obj\
    $(OBJ_PATH)\mtpformatstore.obj\
    $(OBJ_PATH)\mtpstore.obj\
    $(OBJ_PATH)\mtpgetdevicepropdesc.obj\
    $(OBJ_PATH)\mtpgetdevicepropvalue.obj\
    $(OBJ_PATH)\mtpsetdevicepropvalue.obj\
    $(OBJ_PATH)\mtpresetdevicepropvalue.obj\
    $(OBJ_PATH)\transport.obj\
    $(OBJ_PATH)\support.obj\
    $(OBJ_PATH)\SysFuncs.obj\
    $(OBJ_PATH)\mtprequests.obj\
    $(OBJ_PATH)\mtpopensession.obj\
    $(OBJ_PATH)\mtpclosesession.obj\
    $(OBJ_PATH)\mtpsendobjectproplist.obj\
    $(OBJ_PATH)\mtpgetobjectproplist.obj\
    $(OBJ_PATH)\mtpgetobjectpropsupported.obj\
    $(OBJ_PATH)\mtpgetobjectpropdesc.obj\
    $(OBJ_PATH)\mtpsetobjectpropvalue.obj\
    $(OBJ_PATH)\mtpgetobjectpropvalue.obj\
    $(OBJ_PATH)\mtppowerdown.obj\
    $(OBJ_PATH)\mtpresetdevice.obj\
    $(OBJ_PATH)\mtpobjectproperty.obj\
    $(OBJ_PATH)\MtpObjProp_SetPropVal.obj\
    $(OBJ_PATH)\MtpObjProp_GetPropVal.obj\
    $(OBJ_PATH)\mtpsetobjectproplist.obj\
    $(OBJ_PATH)\mtpselftest.obj\
    $(OBJ_PATH)\mtpsetobjectprotection.obj\
    $(OBJ_PATH)\mtpgetdeviceinfo.obj\
    $(OBJ_PATH)\MtpJanusPropertyHandler.obj\
    $(OBJ_PATH)\mtpsendresponse.obj\
    $(OBJ_PATH)\mtp_updater_scsi_commands.obj\
    $(OBJ_PATH)\mtp_getcustomerscsihandler.obj\
    $(OBJ_PATH)\convstmp2hoststr.obj\
    $(OBJ_PATH)\MtpLastPacketHandler.obj

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
"$(MTP_LIB)" : $(MTPLIST)
    @echo Creating IEEE archive library file for MTP Driver$@
    @$(AR_EXE) -r $(MTP_LIB) $(MTPLIST)


#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\mtp.obj" : "$(MTP_PATH)\mtp.c" $(MTP_DEPENDENCIES) $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h $(OUTPUT_INCLUDE_PATH)\regsusb20.h
"$(OBJ_PATH)\mtprequests.obj" : "$(MTP_PATH)\mtprequests.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\SysFuncs.obj" : "$(MTP_PATH)\SysFuncs.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\transport.obj" : "$(MTP_PATH)\transport.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\ptpstring.obj" : "$(MTP_PATH)\ptpstring.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\support.obj" : "$(MTP_PATH)\support.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpdevice.obj" : "$(MTP_PATH)\mtpdevice.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpinitdevice.obj" : "$(MTP_PATH)\mtpinitdevice.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtphandler.obj" : "$(MTP_PATH)\mtphandler.c" $(MTP_DEPENDENCIES) $(OUTPUT_INCLUDE_PATH)\regsrtc.h $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OBJ_PATH)\mtpsendresponse.obj" : "$(MTP_PATH)\mtpsendresponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpobject.obj" : "$(MTP_PATH)\mtpobject.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetstorageids.obj" : "$(MTP_PATH)\mtpgetstorageids.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetnumobjects.obj" : "$(MTP_PATH)\mtpgetnumobjects.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetstorageinfo.obj" : "$(MTP_PATH)\mtpgetstorageinfo.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetobjecthandles.obj" : "$(MTP_PATH)\mtpgetobjecthandles.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetobjectinfo.obj" : "$(MTP_PATH)\mtpgetobjectinfo.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetobject.obj" : "$(MTP_PATH)\mtpgetobject.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpsendobjectinfo.obj" : "$(MTP_PATH)\mtpsendobjectinfo.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpsendobject.obj" : "$(MTP_PATH)\mtpsendobject.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpdeleteobject.obj" : "$(MTP_PATH)\mtpdeleteobject.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetdevicepropdesc.obj" : "$(MTP_PATH)\mtpgetdevicepropdesc.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetdevicepropvalue.obj" : "$(MTP_PATH)\mtpgetdevicepropvalue.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpsetdevicepropvalue.obj" : "$(MTP_PATH)\mtpsetdevicepropvalue.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpresetdevicepropvalue.obj" : "$(MTP_PATH)\mtpresetdevicepropvalue.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpproperty.obj" : "$(MTP_PATH)\mtpproperty.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpformatstore.obj" : "$(MTP_PATH)\mtpformatstore.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpstore.obj" : "$(MTP_PATH)\mtpstore.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpopensession.obj" : "$(MTP_PATH)\mtpopensession.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpclosesession.obj" : "$(MTP_PATH)\mtpclosesession.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtppowerdown.obj" : "$(MTP_PATH)\mtppowerdown.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpresetdevice.obj" : "$(MTP_PATH)\mtpresetdevice.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpselftest.obj" : "$(MTP_PATH)\mtpselftest.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpresponse.obj" : "$(MTP_PATH)\mtpresponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpsetobjectprotection.obj" : "$(MTP_PATH)\mtpsetobjectprotection.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetdeviceinfo.obj" : "$(MTP_PATH)\mtpgetdeviceinfo.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtpgetobjectpropsupported.obj" : "$(MTP_PATH)\mtpgetobjectpropsupported.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpgetobjectpropdesc.obj" : "$(MTP_PATH)\mtpgetobjectpropdesc.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpsetobjectpropvalue.obj" : "$(MTP_PATH)\mtpsetobjectpropvalue.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpgetobjectpropvalue.obj" : "$(MTP_PATH)\mtpgetobjectpropvalue.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpobjectproperty.obj" : "$(MTP_PATH)\mtpobjectproperty.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\MtpObjProp_SetPropVal.obj" : "$(MTP_PATH)\MtpObjProp_SetPropVal.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\MtpObjProp_GetPropVal.obj" : "$(MTP_PATH)\MtpObjProp_GetPropVal.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpsetobjectproplist.obj" : "$(MTP_PATH)\mtpsetobjectproplist.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpsendobjectproplist.obj" : "$(MTP_PATH)\mtpsendobjectproplist.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtpgetobjectproplist.obj" : "$(MTP_PATH)\mtpgetobjectproplist.c" $(MTP_DEPENDENCIES) $(MTP_PATH)\mtpobjectproperty.h
"$(OBJ_PATH)\mtp_updater_scsi_commands.obj" : "$(MTP_PATH)\mtp_updater_scsi_commands.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtp_getcustomerscsihandler.obj" : "$(MTP_PATH)\mtp_getcustomerscsihandler.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\convstmp2hoststr.obj" : "$(MTP_PATH)\convstmp2hoststr.c" $(MTP_DEPENDENCIES) 
"$(OBJ_PATH)\MtpLastPacketHandler.obj" : "$(MTP_PATH)\MtpLastPacketHandler.c" $(MTP_DEPENDENCIES) 
"$(OBJ_PATH)\CalcPGUID.obj" : "$(MTP_PATH)\CalcPGUID.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSetSecureTimeResponse.obj" : "$(MTP_PATH)\Janus\MtpJanusSetSecureTimeResponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSetMeterResponse.obj" : "$(MTP_PATH)\Janus\MtpJanusSetMeterResponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSetLicenseResponse.obj" : "$(MTP_PATH)\Janus\MtpJanusSetLicenseResponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\JanusStubs.obj" : "$(MTP_PATH)\Janus\JanusStubs.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSetDevCertResponse.obj" : "$(MTP_PATH)\Janus\MtpJanusSetDevCertResponse.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSendMeterChallengeQuery.obj" : "$(MTP_PATH)\Janus\MtpJanusSendMeterChallengeQuery.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusGetSyncList.obj" : "$(MTP_PATH)\Janus\MtpJanusGetSyncList.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusGetSecureTimeChallenge.obj" : "$(MTP_PATH)\Janus\MtpJanusGetSecureTimeChallenge.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusGetMeterChallenge.obj" : "$(MTP_PATH)\Janus\MtpJanusGetMeterChallenge.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusGetDevCertChallenge.obj" : "$(MTP_PATH)\Janus\MtpJanusGetDevCertChallenge.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusCleanDataStore.obj" : "$(MTP_PATH)\Janus\MtpJanusCleanDataStore.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusGetLicenseState.obj" : "$(MTP_PATH)\Janus\MtpJanusGetLicenseState.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSendWMDRMPDCommand.obj" : "$(MTP_PATH)\Janus\MtpJanusSendWMDRMPDCommand.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusSendWMDRMPDRequest.obj" : "$(MTP_PATH)\Janus\MtpJanusSendWMDRMPDRequest.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\MtpJanusPropertyHandler.obj" : "$(MTP_PATH)\Janus\MtpJanusPropertyHandler.c" $(MTP_DEPENDENCIES)

"$(OUTPUT_INCLUDE_PATH)\errors.h" :"$(INC_PATH)\errors.inc"
    @echo Converting Header File "$(INC_PATH)\errors.inc"
    $(INCTOC_EXE) "$(INC_PATH)\errors.inc" "$(OUTPUT_INCLUDE_PATH)\errors.h"


endif # ifdef D_PLIBS

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\descriptors_3500.obj\
    $(OBJ_PATH)\mtp_services.obj\
    $(OBJ_PATH)\mtp_resource.obj

"$(OBJ_PATH)\descriptors_3500.obj" : "$(MTP_PATH)\descriptors_3500.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtp_services.obj" : "$(MTP_PATH)\mtp_services.c" $(MTP_DEPENDENCIES)
"$(OBJ_PATH)\mtp_resource.obj" : "$(SYMBOLS_PATH)\mtp_resource.asm"
# TOVERIFY  Deleted ccr.a's source obj from build here vs lbmlc. Del this ln after verif.

endif # ifndef USBMTP_MK
