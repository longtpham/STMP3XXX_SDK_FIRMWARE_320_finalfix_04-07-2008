    IF (!@def(resources_mtp))
resources_mtp equ 1

RSRC_MTP_INITDEVICE                     equ     0 ;$FILENAME mtpinitdevice.src

RSRC_MTP_GETDEVICEINFO                  equ     0 ;$FILENAME mtpdeviceinfo.src
RSRC_MTP_GETSTORAGEIDSMTPHANDLE         equ     RSRC_MTP_GETDEVICEINFO


RSRC_MTP_GETNUMOBJECTS                  equ     0 ;$FILENAME mtpgetnumobjects.src
RSRC_MTP_STSTORE                        equ     RSRC_MTP_GETNUMOBJECTS
RSRC_MTP_GETOBJECTHANDLES               equ     0 ;$FILENAME mtpgetobjecthandles.src


RSRC_MTP_GETOBJECT                      equ     0 ;$FILENAME mtpgetobject.src

RSRC_MTP_GETOBJECTINFO                  equ     0 ;$FILENAME mtpgetobjectinfo.src

RSRC_MTP_SENDOBJECT                     equ     0 ;$FILENAME mtpsendobject.src

RSRC_MTP_SENDOBJECTINFO                 equ     0 ;$FILENAME mtpsendobjectinfo.src

RSRC_MTP_GETSTORAGEINFOMTPHANDLE        equ     0 ;$FILENAME mtpstorage.src
RSRC_MTP_FORMATSTOREMTPHANDLE           equ     0 ;$FILENAME mtpformatstore.src

RSRC_MTP_OPENSESSION                    equ     0 ;$FILENAME mtpopensession.src
RSRC_MTP_CLOSESESSION                   equ     0 ;$FILENAME mtpclosesession.src

RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE     equ     0 ;$FILENAME mtpdevprops.src
RSRC_MTP_GETDEVICEPROPVALUEMTPHANDLE    equ     RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE
RSRC_MTP_SETDEVICEPROPVALUEMTPHANDLE    equ     RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE
RSRC_MTP_RESETDEVICEPROPVALUEMTPHANDLE  equ     RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE

RSRC_MTP_DELETEOBJECTMTPHANDLE          equ     0 ;$FILENAME mtpdeletemovecopy.src
RSRC_MTP_REMOVEOBJECT                   equ     RSRC_MTP_DELETEOBJECTMTPHANDLE

RSRC_MTP_POWERDOWN                      equ     0 ;$FILENAME mtppowerdown.src
RSRC_MTP_RESETDEVICE                    equ     RSRC_MTP_POWERDOWN
RSRC_MTP_SELFTEST                       equ     RSRC_MTP_POWERDOWN

RSRC_MTP_SETOBJECTPROTECTION            equ     0 ;$FILENAME mtpsetobjectprotection.src

RSRC_MTP_GETOBJECTPROPDESCMTPHANDLE     equ     0 ;$FILENAME mtpgetobjectpropdesc.src

RSRC_MTP_SETOBJECTPROPVALUEMTPHANDLE    equ     0 ;$FILENAME mtpsetobjectpropvalue.src
RSRC_MTP_GETOBJECTPROPVALUEMTPHANDLE    equ     0 ;$FILENAME mtpgetobjectpropvalue.src
RSRC_MTP_GETOBJECTPROPSUPPORTEDMTPHANDLE equ    0 ;$FILENAME mtpgetobjectpropsupported.src

RSRC_MTP_SENDOBJECTPROPLISTMTPHANDLE      equ   0 ;$FILENAME mtpsendobjectproplist.src
RSRC_MTP_SETOBJECTPROPLISTMTPHANDLE       equ   0 ;$FILENAME mtpsetobjectproplist.src
RSRC_MTP_GETOBJECTPROPLISTMTPHANDLE       equ   0 ;$FILENAME mtpgetobjectproplist.src

RSRC_MTP_JANUS_GETSECURETIMECHALLENGE     equ   0 ;$FILENAME mtpjanusgetsecuretimechallenge.src
RSRC_MTP_JANUS_SETSECURETIMERESPONSE      equ   0 ;$FILENAME mtpjanussetsecuretimeresponse.src
RSRC_MTP_JANUS_SETLICENSERESPONSE         equ   0 ;$FILENAME mtpjanussetlicenseresponse.src
RSRC_MTP_JANUS_GETSYNCLIST                equ   0 ;$FILENAME mtpjanusgetsynclist.src
RSRC_MTP_JANUS_SEND_METER_CHALLENGE_QUERY equ   0 ;$FILENAME mtpjanussendmeterchallengequery.src
RSRC_MTP_JANUS_GETMETERCHALLENGE          equ   0 ;$FILENAME mtpjanusgetmeterchallenge.src
RSRC_MTP_JANUS_SETMETERRESPONSE           equ   0 ;$FILENAME mtpjanussetmeterresponse.src
RSRC_MTP_JANUS_CLEANDATASTORE             equ   0 ;$FILENAME mtpjanuscleandatastore.src
RSRC_MTP_JANUS_GET_LICENSE_STATE          equ   0 ;$FILENAME mtpjanusgetlicensestate.src
RSRC_MTP_JANUS_SEND_WMDRMPD_COMMAND       equ   0 ;$FILENAME mtpjanussendwmdrmpdcommand.src
RSRC_MTP_JANUS_SEND_WMDRMPD_REQUEST       equ   0 ;$FILENAME mtpjanussendwmdrmpdrequest.src

RSRC_MTP_HANDLER                          equ   0 ;$FILENAME mtphandler.src
RSRC_MTP_RESPONSE_HANDLER                 equ   RSRC_MTP_HANDLER
;RSRC_MTP_PROPERTY                         equ   0 ;$FILENAME mtpproperty.src
RSRC_MTP_PROJECT                          equ   RSRC_MTP_HANDLER
;RSRC_MTP_SERVICES                         equ   0 ;$FILENAME mtpservices.src
;RSRC_MTP_DEVICE                           equ   0 ;$FILENAME mtpdevice.src
;
;RSRC_MTP_LCD                              equ   0 ;$FILENAME mtplcd.src
;RSRC_MTP_STORE                            equ   0 ;$FILENAME mtpstore.src
;RSRC_MTP_REQUESTS                         equ   0 ;$FILENAME mtprequests.src
;RSRC_MTP_OBJECT                           equ   0 ;$FILENAME mtpobject.src
;
;RSRC_MTP_SUPPORT                          equ   0 ;$FILENAME mtpsupport.src
;RSRC_MTP_TRANSPORT                        equ   0 ;$FILENAME mtptransport.src
;RSRC_MTP_LIST                             equ   0 ;$FILENAME mtplist.src

RSRC_FILE_SYSTEM                          equ    0 ;$FILENAME filesystem.src

RSRC_VBR_CODEBANK                                   equ     0 ;$FILENAME vbr_codebank.src
RSRC_METADATA_CODEBANK                              equ     0 ;$FILENAME metadata_codebank.src
RSRC_MP3_METADATA_CODEBANK                          equ     0 ;$FILENAME mp3metadata_codebank.src
RSRC_WMA_METADATA_CODEBANK                          equ     0 ;$FILENAME wmametadata_codebank.src
RSRC_WAV_METADATA_CODEBANK                          equ     0 ;$FILENAME wavmetadata_codebank.src
RSRC_ID3V2_CODEBANK                                 equ     0 ;$FILENAME id3v2_codebank.src
;RSRC_MTP_DEV_INFO_TABLE                             equ     0 ;$FILENAME mtpdevinfotable.src

RSRC_AUDIBLE_METADATA_CODEBANK              equ     0 ;$FILENAME audiblemtpmetadata.src
RSRC_AUDIBLE_OBJECT_PROP_HANDLER            equ     0 ;$FILENAME audibleobjecthandler.src
RSRC_AUDIBLE_DEVICE_PROP_HANDLER            equ     0 ;$FILENAME audibledevicehandler.src

  ENDIF ;IF (!@def(resources_mtp))