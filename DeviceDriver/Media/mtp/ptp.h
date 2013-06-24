/****************************************************************************
 *
 * @module      ptp.h
 *
 * @created     17-Februrary-2004
 *
 * @abstract    PTP functions and structures
 *
 * @copyright   (C) COPYRIGHT POWER Q INC., 2004
 *
 ****************************************************************************/

#ifndef __PTP_H__
#define __PTP_H__

/*
 *  Define standard min and max macros
 */

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define MAX(a,b) (((a) > (b)) ? (a) : (b))


/*
 * PTP Data type codes.
 */
#define PTP_DATATYPE_UNDEFINED   0x0000
#define PTP_DATATYPE_INT8        0x0001
#define PTP_DATATYPE_UINT8       0x0002
#define PTP_DATATYPE_INT16       0x0003
#define PTP_DATATYPE_UINT16      0x0004
#define PTP_DATATYPE_INT32       0x0005
#define PTP_DATATYPE_UINT32      0x0006
#define PTP_DATATYPE_INT64       0x0007
#define PTP_DATATYPE_UINT64      0x0008
#define PTP_DATATYPE_INT128      0x0009
#define PTP_DATATYPE_UINT128     0x000A
#define PTP_DATATYPE_AINT8       0x4001
#define PTP_DATATYPE_AUINT8      0x4002
#define PTP_DATATYPE_AINT16      0x4003
#define PTP_DATATYPE_AUINT16     0x4004
#define PTP_DATATYPE_AINT32      0x4005
#define PTP_DATATYPE_AUINT32     0x4006
#define PTP_DATATYPE_AINT64      0x4007
#define PTP_DATATYPE_AUINT64     0x4008
#define PTP_DATATYPE_AINT128     0x4009
#define PTP_DATATYPE_AUINT128    0x400A
#define PTP_DATATYPE_STRING      0xFFFF
#define PTP_DATATYPE_ARRAYMASK   0x4FF0
#define PTP_DATATYPE_ARRAY       0x4000
#define PTP_DATATYPE_VALUEMASK   0xFFF0
#define PTP_DATATYPE_VALUE       0x0000



/*
 * Data code ranges and masks. Each data code has 16 bits:
 *
 * Bit 15(std/vendor)
 *    0 -- the code is defined by PTP standard
 *    1 -- the code is vendor specific
 *
 * Bit 14 - 12(data type)
 *   14 13 12
 *   0  0  0    -- undefined data type
 *   0  0  1    -- op code
 *   0  1  0    -- response code
 *   0  1  1    -- format code
 *   1  0  0    -- event code
 *   1  0  1    -- property code
 *   1  1  0    -- reserved
 *   1  1  1    -- reserved
 *
 * Bit 11 - bit 0 (data value)
 */
#define  PTP_DATACODE_VENDORMASK           0x8000
#define  PTP_DATACODE_TYPEMASK             0x7000
#define  PTP_DATACODE_VALUEMASK            0x0FFF
#define  PTP_DATACODE_TYPE_UNKNOWN         0x0000
#define  PTP_DATACODE_TYPE_OPERATION       0x1000
#define  PTP_DATACODE_TYPE_RESPONSE        0x2000
#define  PTP_DATACODE_TYPE_FORMAT          0x3000
#define  PTP_DATACODE_TYPE_EVENT           0x4000
#define  PTP_DATACODE_TYPE_PROPERTY        0x5000
#define  PTP_DATACODE_TYPE_RESERVED_1      0x6000
#define  PTP_DATACODE_TYPE_RESERVED_2      0x7000

/*
 * To verify an op code
 *  (Code & PTP_DATACODE_TYPEMASK) == PTP_DATACODE_TYPE_OPERATION
 * To verify a response code
 *  (Code & PTP_DATACODE_TYPEMASK) == PTP_DATACODE_TYPE_RESPONSE)
 */

/*
 * Image format codes receive special treatment.
 */
#define  PTP_DATACODE_TYPEIMAGEMASK        0x7800;
#define  PTP_DATACODE_TYPE_IMAGEFORMAT     0x3800;
#define  PTP_DATACODE_VALUE_IMAGEVMASK     0x07FF;
/*
 * To verify an image code
 * (Code & PTP_DATACODE_TYPEIMAGEMASK) == PTP_DATACODE_TYPE_IMAGEFORMAT
 */

/*
 * PTP specially defined constants
 */
#define PTP_OBJECTHANDLE_ALL          0xFFFFFFFF
#define PTP_OBJECTHANDLE_UNDEFINED    0x0
#define PTP_OBJECTHANDLE_ROOT         0x0
#define PTP_PROPERTY_ALL              0xFFFFFFFF
#define PTP_PROPERTY_UNDEFINED        0x0
#define PTP_STORAGEID_ALL             0xFFFFFFFF
#define PTP_STORAGEID_DEFAULT         0
#define PTP_STORAGEID_UNDEFINED       0
#define PTP_STORAGEID_PHYSICAL        0x0000FFFF
#define PTP_STORAGEID_LOGICAL         0xFFFF0000
#define PTP_SESSIONID_ALL             0
#define PTP_SESSIONID_NOSESSION       0
#define PTP_FORMATCODE_NOTUSED        0x0
#define PTP_FORMATCODE_ALL            0xFFFFFFFF
#define PTP_FORMATCODE_DEFAULT        0x0000
#define PTP_TRANSACTIONID_ALL         0xFFFFFFFF
#define PTP_TRANSACTIONID_NOSESSION   0

/*
 * standard operation codes:
 */
#define PTP_OPCODE_UNDEFINED               0x1000
#define PTP_OPCODE_GETDEVICEINFO           0x1001
#define PTP_OPCODE_OPENSESSION             0x1002
#define PTP_OPCODE_CLOSESESSION            0x1003
#define PTP_OPCODE_GETSTORAGEIDS           0x1004
#define PTP_OPCODE_GETSTORAGEINFO          0x1005
#define PTP_OPCODE_GETNUMOBJECTS           0x1006
#define PTP_OPCODE_GETOBJECTHANDLES        0x1007
#define PTP_OPCODE_GETOBJECTINFO           0x1008
#define PTP_OPCODE_GETOBJECT               0x1009
#define PTP_OPCODE_GETTHUMB                0x100A
#define PTP_OPCODE_DELETEOBJECT            0x100B
#define PTP_OPCODE_SENDOBJECTINFO          0x100C
#define PTP_OPCODE_SENDOBJECT              0x100D
#define PTP_OPCODE_INITIATECAPTURE         0x100E
#define PTP_OPCODE_FORMATSTORE             0x100F
#define PTP_OPCODE_RESETDEVICE             0x1010
#define PTP_OPCODE_SELFTEST                0x1011
#define PTP_OPCODE_SETOBJECTPROTECTION     0x1012
#define PTP_OPCODE_POWERDOWN               0x1013
#define PTP_OPCODE_GETDEVICEPROPDESC       0x1014
#define PTP_OPCODE_GETDEVICEPROPVALUE      0x1015
#define PTP_OPCODE_SETDEVICEPROPVALUE      0x1016
#define PTP_OPCODE_RESETDEVICEPROPVALUE    0x1017
#define PTP_OPCODE_TERMINATECAPTURE        0x1018
#define PTP_OPCODE_MOVEOBJECT              0x1019
#define PTP_OPCODE_COPYOBJECT              0x101A
#define PTP_OPCODE_GETPARTIALOBJECT        0x101B
#define PTP_OPCODE_INITIATEOPENCAPTURE     0x101C
#define PTP_OPCODE_VENDOREXTENDEDBASE      0x9000

/*
 * MTP extended operations
 */
#define	MTP_OPCODE_UNDEFINED               0x9800
#define	MTP_OPCODE_GETOBJECTPROPSUPPORTED  0x9801
#define	MTP_OPCODE_GETOBJECTPROPDESC       0x9802
#define	MTP_OPCODE_GETOBJECTPROPVALUE      0x9803
#define	MTP_OPCODE_SETOBJECTPROPVALUE      0x9804
#define	MTP_OPCODE_GETOBJECTPROPLIST       0x9805
#define MTP_OPCODE_SETOBJECTPROPLIST       0x9806
#define	MTP_OPCODE_RESETOBJECTPROPVALUE    0x9807
#define MTP_OPCODE_SENDOBJECTPROPLIST      0x9808
#define	MTP_OPCODE_GETOBJECTREFERENCES     0x9810
#define	MTP_OPCODE_SETOBJECTREFERENCES     0x9811

/*
 * Operations for Janus MTP extension
 */
#define MTP_OPCODE_JANUS_GETSECURETIMECHALLENGE     0x9101
#define MTP_OPCODE_JANUS_SETSECURETIMERESPONSE      0x9102
#define MTP_OPCODE_JANUS_SETLICENSERESPONSE         0x9103
#define MTP_OPCODE_JANUS_GETSYNCLIST                0x9104
#define MTP_OPCODE_JANUS_SEND_METER_CHALLENGE_QUERY 0x9105
#define MTP_OPCODE_JANUS_GET_METER_CHALLENGE        0x9106
#define MTP_OPCODE_JANUS_SET_METER_RESPONSE         0x9107
#define MTP_OPCODE_JANUS_CLEANDATASTORE             0x9108
#define MTP_OPCODE_JANUS_GET_LICENSE_STATE          0x9109
#define MTP_OPCODE_JANUS_SEND_WMDRMPD_COMMAND       0x910A
#define MTP_OPCODE_JANUS_SEND_WMDRMPD_REQUEST       0x910B


/*
 * SigmaTel vendor-specific operations.
 * Note 9/12/05: sgtl changed middle 2 nibbles for undefined, reset and eraseboot to be more unique.
 *                                 eg: x9200 is now 0x97F0.
 *     Sdk3.1 stupdater required (has these values also). So do not use stupdater from sdk3.0x.
 */
#define MTP_OPCODE_SIGMATEL_UNDEFINED               0x97F0
#define MTP_OPCODE_SIGMATEL_RESET                   0x97F1
#define MTP_OPCODE_SIGMATEL_ERASEBOOT               0x97F2

/*
 * standard event codes:
 */
#define PTP_EVENTCODE_UNDEFINED                0x4000
#define PTP_EVENTCODE_CANCELTRANSACTION        0x4001
#define PTP_EVENTCODE_OBJECTADDED              0x4002
#define PTP_EVENTCODE_OBJECTREMOVED            0x4003
#define PTP_EVENTCODE_STOREADDED               0x4004
#define PTP_EVENTCODE_STOREREMOVED             0x4005
#define PTP_EVENTCODE_DEVICEPROPCHANGED        0x4006
#define PTP_EVENTCODE_OBJECTINFOCHANGED        0x4007
#define PTP_EVENTCODE_DEVICEINFOCHANGED        0x4008
#define PTP_EVENTCODE_REQUESTOBJECTTRANSFER    0x4009
#define PTP_EVENTCODE_STOREFULL                0x400A
#define PTP_EVENTCODE_DEVICERESET              0x400B
#define PTP_EVENTCODE_STORAGEINFOCHANGED       0x400C
#define PTP_EVENTCODE_CAPTURECOMPLETE          0x400D
#define PTP_EVENTCODE_UNREPORTEDSTATUS         0x400E
#define PTP_EVENTCODE_VENDOREXTENTION1         0xC001
#define PTP_EVENTCODE_VENDOREXTENTION2         0xC002

/*
 * MTP-extended Events
 */
#define MTP_EVENTCODE_UNDEFINED                0xB800
#define MTP_EVENTCODE_OBJECTPROPCHANGED        0xB801
#define MTP_EVENTCODE_OBJECTPROPDESCCHANGED    0xB802
#define MTP_EVENTCODE_OBJECTREFERENCESCHANGED  0xB803
#define MTP_EVENTCODE_DEVICEPROPDESCCHANGED    0xB804

/* Events for Janus MTP extension */
#define MTP_EVENTCODE_JANUS_UNDEFINED          0xC100
#define MTP_EVENTCODE_JANUS_EVENT1             0xC101


/*
 * standard response codes:
 */
#define PTP_RESPONSECODE_UNDEFINED                     0x2000
#define PTP_RESPONSECODE_OK                            0x2001
#define PTP_RESPONSECODE_GENERALERROR                  0x2002
#define PTP_RESPONSECODE_SESSIONNOTOPEN                0x2003
#define PTP_RESPONSECODE_INVALIDTRANSACTIONID          0x2004
#define PTP_RESPONSECODE_OPERATIONNOTSUPPORTED         0x2005
#define PTP_RESPONSECODE_PARAMETERNOTSUPPORTED         0x2006
#define PTP_RESPONSECODE_INCOMPLETETRANSFER            0x2007
#define PTP_RESPONSECODE_INVALIDSTORAGEID              0x2008
#define PTP_RESPONSECODE_INVALIDOBJECTHANDLE           0x2009
#define PTP_RESPONSECODE_PROPERTYNOTSUPPORTED          0x200A
#define PTP_RESPONSECODE_INVALIDOBJECTFORMATCODE       0x200B
#define PTP_RESPONSECODE_STOREFULL                     0x200C
#define PTP_RESPONSECODE_OBJECTWRITEPROTECTED          0x200D
#define PTP_RESPONSECODE_STOREWRITEPROTECTED           0x200E
#define PTP_RESPONSECODE_ACCESSDENIED                  0x200F
#define PTP_RESPONSECODE_NOTHUMBNAILPRESENT            0x2010
#define PTP_RESPONSECODE_SELFTESTFAILED                0x2011
#define PTP_RESPONSECODE_PARTIALDELETION               0x2012
#define PTP_RESPONSECODE_STORENOTAVAILABLE             0x2013
#define PTP_RESPONSECODE_NOSPECIFICATIONBYFORMAT       0x2014
#define PTP_RESPONSECODE_NOVALIDOBJECTINFO             0x2015
#define PTP_RESPONSECODE_INVALIDCODEFORMAT             0x2016
#define PTP_RESPONSECODE_UNKNOWNVENDORCODE             0x2017
#define PTP_RESPONSECODE_CAPTUREALREADYTERMINATED      0x2018
#define PTP_RESPONSECODE_DEVICEBUSY                    0x2019
#define PTP_RESPONSECODE_INVALIDPARENT                 0x201A
#define PTP_RESPONSECODE_INVALIDPROPFORMAT             0x201B
#define PTP_RESPONSECODE_INVALIDPROPVALUE              0x201C
#define PTP_RESPONSECODE_INVALIDPARAMETER              0x201D
#define PTP_RESPONSECODE_SESSIONALREADYOPENED          0x201E
#define PTP_RESPONSECODE_TRANSACTIONCANCELLED          0x201F
/*
 * MTP responses
 */
#define MTP_RESPONSECODE_UNDEFINED                     0xA800
#define MTP_RESPONSECODE_INVALIDOBJECTPROPCODE         0xA801
#define MTP_RESPONSECODE_INVALIDOBJECTPROPFORMAT       0xA802
#define MTP_RESPONSECODE_INVALIDOBJECTPROPVALUE        0xA803
#define MTP_RESPONSECODE_INVALIDOBJECTREFERENCE        0xA804

#define MTP_RESPONSECODE_GROUPUNSUPPORTED              0xA807
#define MTP_RESPONSECODE_DEPTHUNSUPPORTED              0xA808
#define MTP_RESPONSECODE_OBJECTTOOLARGE                0xA809

/* Responses for Janus MTP extension  */
#define MTP_RESPONSECODE_JANUS_UNDEFINED               0xA100
#define MTP_RESPONSECODE_JANUS_WMDRMPD_FAIL            0xA101


/*
 * standard property codes:
 */
#define PTP_PROPERTYCODE_UNDEFINED                 0x5000
#define PTP_PROPERTYCODE_BATTERYLEVEL              0x5001
#define PTP_PROPERTYCODE_FUNCTIONMODE              0x5002
#define PTP_PROPERTYCODE_IMAGESIZE                 0x5003
#define PTP_PROPERTYCODE_COMPRESSIONSETTING        0x5004
#define PTP_PROPERTYCODE_WHITEBALANCE              0x5005
#define PTP_PROPERTYCODE_RGBGAIN                   0x5006
#define PTP_PROPERTYCODE_FNUMBER                   0x5007
#define PTP_PROPERTYCODE_FOCALLENGTH               0x5008
#define PTP_PROPERTYCODE_FOCUSDISTANCE             0x5009
#define PTP_PROPERTYCODE_FOCUSMODE                 0x500A
#define PTP_PROPERTYCODE_EXPOSUREMETERINGMODE      0x500B
#define PTP_PROPERTYCODE_FLASHMODE                 0x500C
#define PTP_PROPERTYCODE_EXPOSURETIME              0x500D
#define PTP_PROPERTYCODE_EXPOSUREPROGRAMMODE       0x500E
#define PTP_PROPERTYCODE_EXPOSUREINDEX             0x500F
#define PTP_PROPERTYCODE_EXPOSURECOMPENSATION      0x5010
#define PTP_PROPERTYCODE_DATETIME                  0x5011
#define PTP_PROPERTYCODE_CAPTUREDELAY              0x5012
#define PTP_PROPERTYCODE_STILLCAPTUREMODE          0x5013
#define PTP_PROPERTYCODE_CONTRAST                  0x5014
#define PTP_PROPERTYCODE_SHARPNESS                 0x5015
#define PTP_PROPERTYCODE_DIGITALZOOM               0x5016
#define PTP_PROPERTYCODE_EFFECTMODE                0x5017
#define PTP_PROPERTYCODE_BURSTNUMBER               0x5018
#define PTP_PROPERTYCODE_BURSTINTERVAL             0x5019
#define PTP_PROPERTYCODE_TIMELAPSENUMBER           0x501A
#define PTP_PROPERTYCODE_TIMELAPSEINTERVAL         0x501B
#define PTP_PROPERTYCODE_FOCUSMETERINGMODE         0x501C
#define PTP_PROPERTYCODE_UPLOADURL          	   0x501D
#define PTP_PROPERTYCODE_ARTIST		          	   0x501E
#define PTP_PROPERTYCODE_COPYRIGHT	          	   0x501F
#define PTP_PROPERTYCODE_VENDOREXTENTION1          0xD001
#define PTP_PROPERTYCODE_VENDOREXTENTION2          0xD002

//
// MTP defined Device properties
//
#define MTP_PROPERTYCODE_UNDEFINED                 0xD400
#define MTP_PROPERTYCODE_SYNCHRONIZATIONPARTNER    0xD401
#define MTP_PROPERTYCODE_DEVICEFRIENDLYNAME		   0xD402

// Audible Device property extension
#define MTP_PROPERTYCODE_AUDIBLE_ACTIVATION_DATA   0xD100

// Device properties for Janus MTP extension
#define MTP_PROPERTYCODE_JANUS_SECURETIME          0xD101
#define MTP_PROPERTYCODE_JANUS_DEVICECERTIFICATE   0xD102


// Audible Object Properties
#define MTP_OBJ_PROPERTYCODE_AUDIBLECODEC		   0xDA00
#define MTP_OBJ_PROPERTYCODE_AUDIBLEPRODUCTID	   0xDA01
#define MTP_OBJ_PROPERTYCODE_AUDIBLEPLAYBACKPOS	   0xDA02
#define MTP_OBJ_PROPERTYCODE_AUDIBLEPLAYEDTHROUGH  0xDA03
#define MTP_OBJ_PROPERTYCODE_AUDIBLETITLESEGMENT   0xDA04
//
// MTP extended object property codes:
//

#define MTP_OBJ_PROPERTYCODE_STORAGEID             0xDC01
#define MTP_OBJ_PROPERTYCODE_OBJECTFORMAT          0xDC02
#define MTP_OBJ_PROPERTYCODE_PROTECTIONSTATUS      0xDC03
#define MTP_OBJ_PROPERTYCODE_OBJECTSIZE            0xDC04
#define MTP_OBJ_PROPERTYCODE_ASSOCIATIONTYPE       0xDC05
#define MTP_OBJ_PROPERTYCODE_ASSOCIATIONDESC       0xDC06
#define MTP_OBJ_PROPERTYCODE_OBJECTFILENAME        0xDC07
#define MTP_OBJ_PROPERTYCODE_DATECREATED           0xDC08
#define MTP_OBJ_PROPERTYCODE_DATEMODIFIED          0xDC09
#define MTP_OBJ_PROPERTYCODE_KEYWORDS              0xDC0A
#define MTP_OBJ_PROPERTYCODE_PARENT                0xDC0B

#define MTP_OBJ_PROPERTYCODE_PERSISTENTGUID        0xDC41
#define MTP_OBJ_PROPERTYCODE_SYNCID                0xDC42
#define MTP_OBJ_PROPERTYCODE_PROPERTYBAG           0xDC43
#define MTP_OBJ_PROPERTYCODE_NAME                  0xDC44
#define MTP_OBJ_PROPERTYCODE_CREATEDBY             0xDC45
#define MTP_OBJ_PROPERTYCODE_ARTIST                0xDC46
#define MTP_OBJ_PROPERTYCODE_DATEAUTHORED          0xDC47
#define MTP_OBJ_PROPERTYCODE_DESCRIPTION           0xDC48
#define MTP_OBJ_PROPERTYCODE_URLREFERENCE          0xDC49
#define MTP_OBJ_PROPERTYCODE_LANGUAGELOCALE        0xDC4A
#define MTP_OBJ_PROPERTYCODE_COPYRIGHTINFO         0xDC4B
#define MTP_OBJ_PROPERTYCODE_SOURCE                0xDC4C
#define MTP_OBJ_PROPERTYCODE_ORIGINLOCATION        0xDC4D
#define MTP_OBJ_PROPERTYCODE_DATEADDED             0xDC4E
#define MTP_OBJ_PROPERTYCODE_NONCONSUMABLE         0xDC4F
#define MTP_OBJ_PROPERTYCODE_CORRUPTUNPLAYABLE     0xDC50

#define MTP_OBJ_PROPERTYCODE_SAMPLEFORMAT          0xDC81
#define MTP_OBJ_PROPERTYCODE_SAMPLESIZE            0xDC82
#define MTP_OBJ_PROPERTYCODE_SAMPLEHEIGHT          0xDC83
#define MTP_OBJ_PROPERTYCODE_SAMPLEWIDTH           0xDC84
#define MTP_OBJ_PROPERTYCODE_SAMPLEDURATION        0xDC85
#define MTP_OBJ_PROPERTYCODE_SAMPLEDATA            0xDC86
#define MTP_OBJ_PROPERTYCODE_WIDTH                 0xDC87
#define MTP_OBJ_PROPERTYCODE_HEIGHT                0xDC88
#define MTP_OBJ_PROPERTYCODE_DURATION              0xDC89
#define MTP_OBJ_PROPERTYCODE_USERRATING            0xDC8A
#define MTP_OBJ_PROPERTYCODE_TRACK                 0xDC8B
#define MTP_OBJ_PROPERTYCODE_GENRE                 0xDC8C
#define MTP_OBJ_PROPERTYCODE_CREDITS               0xDC8D
#define MTP_OBJ_PROPERTYCODE_LYRICS                0xDC8E
#define MTP_OBJ_PROPERTYCODE_SUBSCRIPCONTENTID     0xDC8F
#define MTP_OBJ_PROPERTYCODE_PRODUCEDBY            0xDC90
#define MTP_OBJ_PROPERTYCODE_USECOUNT              0xDC91
#define MTP_OBJ_PROPERTYCODE_SKIPCOUNT             0xDC92
#define MTP_OBJ_PROPERTYCODE_LASTACCESSED          0xDC93
#define MTP_OBJ_PROPERTYCODE_PARENTALRATING        0xDC94
#define MTP_OBJ_PROPERTYCODE_METAGENRE             0xDC95
#define MTP_OBJ_PROPERTYCODE_COMPOSER              0xDC96
#define MTP_OBJ_PROPERTYCODE_EFFECTIVERATING       0xDC97
#define MTP_OBJ_PROPERTYCODE_SUBTITLE              0xDC98
#define MTP_OBJ_PROPERTYCODE_ORIGINALRELEASEDATE   0xDC99
#define MTP_OBJ_PROPERTYCODE_ALBUMNAME             0xDC9A
#define MTP_OBJ_PROPERTYCODE_ALBUMARTIST           0xDC9B
#define MTP_OBJ_PROPERTYCODE_MOOD                  0xDC9C
#define MTP_OBJ_PROPERTYCODE_DRMSTATUS             0xDC9D
#define MTP_OBJ_PROPERTYCODE_SUBDESCRIPTION        0xDC9E

#define MTP_OBJ_PROPERTYCODE_ISCROPPED             0xDCD1
#define MTP_OBJ_PROPERTYCODE_ISCOLORCORRECTED      0xDCD2

#define MTP_OBJ_PROPERTYCODE_TOTALBITRATE          0xDE91
#define MTP_OBJ_PROPERTYCODE_BITRATETYPE           0xDE92
#define MTP_OBJ_PROPERTYCODE_SAMPLERATE            0xDE93
#define MTP_OBJ_PROPERTYCODE_NUMBEROFCHANNELS      0xDE94
#define MTP_OBJ_PROPERTYCODE_AUDIOBITDEPTH         0xDE95
#define MTP_OBJ_PROPERTYCODE_SCANTYPE              0xDE97
#define MTP_OBJ_PROPERTYCODE_AUDIOWAVECODEC        0xDE99
#define MTP_OBJ_PROPERTYCODE_AUDIOBITRATE          0xDE9A
#define MTP_OBJ_PROPERTYCODE_VIDEOFOURCCCODEC      0xDE9B
#define MTP_OBJ_PROPERTYCODE_VIDEOBITRATE          0xDE9C
#define MTP_OBJ_PROPERTYCODE_FRAMESPER1KSECONDS    0xDE9D
#define MTP_OBJ_PROPERTYCODE_KEYFRAMEDISTANCE      0xDE9E
#define MTP_OBJ_PROPERTYCODE_BUFFERSIZE            0xDE9F
#define MTP_OBJ_PROPERTYCODE_ENCODINGQUALITY       0xDEA0


/*
 * standard format codes:
 */
#define  PTP_FORMATMASK_IMAGE          		0x0800

#define  PTP_FORMATCODE_UNDEFINED      		0x3000//
#define  PTP_FORMATCODE_ASSOCIATION    		0x3001
#define  PTP_FORMATCODE_SCRIPT         		0x3002
#define  PTP_FORMATCODE_EXECUTABLE     		0x3003
#define  PTP_FORMATCODE_TEXT           		0x3004//
#define  PTP_FORMATCODE_HTML           		0x3005
#define  PTP_FORMATCODE_DPOF           		0x3006
#define  PTP_FORMATCODE_AIFF           		0x3007
#define  PTP_FORMATCODE_WAVE           		0x3008//
#define  PTP_FORMATCODE_MP3            		0x3009//
#define  PTP_FORMATCODE_AVI            		0x300A
#define  PTP_FORMATCODE_MPEG           		0x300B
#define  PTP_FORMATCODE_ASF            		0x300C
#define  MTP_UNDEFINED_FIRMWARE				0xB802
#define  MTP_FORMATCODE_WMA					0xB901//
#define  MTP_FORMATCODE_AUDIBLE             0xB904
#define  MTP_FORMAT_CODE_ABSTRACT_PLAYLIST	0xBA05

#define  PTP_FORMATCODE_IMAGE_UNDEFINED    0x3800
#define  PTP_FORMATCODE_IMAGE_EXIF         0x3801
#define  PTP_FORMATCODE_IMAGE_TIFFEP       0x3802
#define  PTP_FORMATCODE_IMAGE_FLASHPIX     0x3803
#define  PTP_FORMATCODE_IMAGE_BMP          0x3804
#define  PTP_FORMATCODE_IMAGE_CIFF         0x3805
#define  PTP_FORMATCODE_IMAGE_GIF          0x3807
#define  PTP_FORMATCODE_IMAGE_JFIF         0x3808
#define  PTP_FORMATCODE_IMAGE_PCD          0x3809
#define  PTP_FORMATCODE_IMAGE_PICT         0x380A
#define  PTP_FORMATCODE_IMAGE_PNG          0x380B
#define  PTP_FORMATCODE_IMAGE_TIFF         0x380D
#define  PTP_FORMATCODE_IMAGE_TIFFIT       0x380E
#define  PTP_FORMATCODE_IMAGE_JP2          0x380F
#define  PTP_FORMATCODE_IMAGE_JPX          0x3810

/*
 * Property values definitions
 */

/*
 * Property description data set form flags definitions
 */
#define PTP_FORMFLAGS_NONE          0x00
#define PTP_FORMFLAGS_RANGE         0x01
#define PTP_FORMFLAGS_ENUM          0x02
#define PTP_FORMFLAGS_DATETIME      0x03
#define PTP_FORMFLAGS_FIXED         0x04
#define PTP_FORMFLAGS_REGULAR_EXP   0x05
#define PTP_FORMFLAGS_BYTE_ARRAY    0x06
#define PTP_FORMFLAGS_LONG_STRING   0xff

/*
 * power states:
 */
#define PTP_POWERSTATE_DEVICEOFF     0x0000
#define PTP_POWERSTATE_SLEEP         0x0001
#define PTP_POWERSTATE_FULL          0x0002


/*
 * white balances:
 */
#define PTP_WHITEBALANCE_UNDEFINED     0x0000
#define PTP_WHILEBALANCE_MANUAL        0x0001
#define PTP_WHITEBALANCE_AUTOMATIC     0x0002
#define PTP_WHITEBALANCE_ONEPUSHAUTO   0x0003
#define PTP_WHITEBALANCE_DAYLIGHT      0x0004
#define PTP_WHITEBALANCE_FLORESCENT    0x0005
#define PTP_WHITEBALANCE_TUNGSTEN      0x0006
#define PTP_WHITEBALANCE_FLASH         0x0007


/*
 * focus modes:
 */
#define PTP_FOCUSMODE_UNDEFINED   0x0000
#define PTP_FOCUSMODE_MANUAL      0x0001
#define PTP_FOCUSMODE_AUTO        0x0002
#define PTP_FOCUSMODE_MACRO       0x0003

/*
 * focus metering:
 */
#define  PTP_FOCUSMETERING_UNDEFINED      0x0000
#define  PTP_FOCUSMETERING_CENTERSPOT     0x0001
#define  PTP_FOCUSMETERING_MULTISPOT      0x0002

/*
 * flash modes:
 */
#define PTP_FLASHMODE_UNDEFINED       0x0000
#define PTP_FLASHMODE_AUTO            0x0001
#define PTP_FLASHMODE_OFF             0x0002
#define PTP_FLASHMODE_FILL            0x0003
#define PTP_FLASHMODE_REDEYEAUTO      0x0004
#define PTP_FLASHMODE_REDEYEFILL      0x0005
#define PTP_FLASHMODE_EXTERNALSYNC    0x0006
#define PTP_FLASHMODE_MASK            0xFFF0

/*
 * exposure modes:
 */
#define PTP_EXPOSUREMODE_UNDEFINED             0x0000
#define PTP_EXPOSUREMODE_MANUALSETTING         0x0001
#define PTP_EXPOSUREMODE_AUTOPROGRAM           0x0002
#define PTP_EXPOSUREMODE_APERTUREPRIORITY      0x0003
#define PTP_EXPOSUREMODE_SHUTTERPRIORITY       0x0004
#define PTP_EXPOSUREMODE_PROGRAMCREATIVE       0x0005
#define PTP_EXPOSUREMODE_PROGRAMACTION         0x0006
#define PTP_EXPOSUREMODE_PORTRAIT              0x0007

/*
 * capturing modes:
 */
#define  PTP_CAPTUREMODE_UNDEFINED      0x0000
#define  PTP_CAPTUREMODE_NORMAL         0x0001
#define  PTP_CAPTUREMODE_BURST          0x0002
#define  PTP_CAPTUREMODE_TIMELAPSE      0x0003

/*
 * focus metering modes:
 */
#define  PTP_FOCUSMETERMODE_UNDEFINED   0x0000
#define  PTP_FOCUSMETERMODE_CENTERSPOT  0x0001
#define  PTP_FOCUSMETERMODE_MULTISPOT   0x0002


/*
 * effect modes:
 */
#define PTP_EFFECTMODE_UNDEFINED   0x0000
#define PTP_EFFECTMODE_COLOR       0x0001
#define PTP_EFFECTMODE_BW          0x0002
#define PTP_EFFECTMODE_SEPIA       0x0003


/*
 * storage types:
 */
#define PTP_STORAGETYPE_UNDEFINED       0x0000
#define PTP_STORAGETYPE_FIXEDROM        0x0001
#define PTP_STORAGETYPE_REMOVABLEROM    0x0002
#define PTP_STORAGETYPE_FIXEDRAM        0x0003
#define PTP_STORAGETYPE_REMOVABLERAM    0x0004

/*
 * storage access capabilities:
 */
#define PTP_STORAGEACCESS_RWD   0x0000
#define PTP_STORAGEACCESS_R     0x0001
#define PTP_STORAGEACCESS_RD    0x0002

/*
 * association types:
 */
#define PTP_ASSOCIATIONTYPE_UNDEFINED          0x0000
#define PTP_ASSOCIATIONTYPE_FOLDER             0x0001
#define PTP_ASSOCIATIONTYPE_ALBUM              0x0002
#define PTP_ASSOCIATIONTYPE_BURST              0x0003
#define PTP_ASSOCIATIONTYPE_HPANORAMA          0x0004
#define PTP_ASSOCIATIONTYPE_VPANORAMA          0x0005
#define PTP_ASSOCIATIONTYPE_2DPANORAMA         0x0006
#define PTP_ASSOCIATIONTYPE_ANCILLARYDATA      0x0007
#define PTP_ASSOCIATIONTYPE_MASK               0xFFF0

/*
 * protection status:
 */
#define PTP_PROTECTIONSTATUS_NONE          0x0000
#define PTP_PROTECTIONSTATUS_READONLY      0x0001
#define PTP_PROTECTIONSTATUS_READONLY_DATA 0x8002
#define PTP_PROTECTIONSTATUS_NON_XFER      0x8003

#define MTP_PROTECTIONSTATUS_NOPROTECTION       0x0000
#define MTP_PROTECTIONSTATUS_READONLY           0x0001
#define MTP_PROTECTIONSTATUS_READONLY_PERMANENT 0x0002
#define MTP_PROTECTIONSTATUS_ABSTRACT_OBJECT    0x0003

//
// file system types:
//
#define PTP_FILESYSTEMTYPE_UNDEFINED       0x0000
#define PTP_FILESYSTEMTYPE_FLAT            0x0001
#define PTP_FILESYSTEMTYPE_HIERARCHICAL    0x0002
#define PTP_FILESYSTEMTYPE_DCF             0x0003

/*
 * functional modes:
 */
#define  PTP_FUNCTIONMODE_STDANDARD    0x0000
#define  PTP_FUNCTIONMODE_SLEEP        0x0001

/*
 * Get/Set
 */
#define  PTP_PROPGETSET_GETONLY    0x00
#define  PTP_PROPGETSET_GETSET     0x01

//
// Common Audio Sample Rate
//
#define MTP_AUDIO_SAMPLERATE_32K        32000
#define MTP_AUDIO_SAMPLERATE_CD         44100
#define MTP_AUDIO_SAMPLERATE_48K        48000
#define MTP_AUDIO_SAMPLERATE_DVD        96000

//
// Common Audio Bit Rate
//
#define MTP_AUDIO_BITRATE_GSM           13000
#define MTP_AUDIO_BITRATE_G721          32000
#define MTP_AUDIO_BITRATE_G711          64000
#define MTP_AUDIO_BITRATE_128K          128000
#define MTP_AUDIO_BITRATE_CDDA          144100
#define MTP_AUDIO_BITRATE_160K          160000
#define MTP_AUDIO_BITRATE_192K          192000
#define MTP_AUDIO_BITRATE_256K          256000
#define MTP_AUDIO_BITRATE_STUDIO        384000

//
// Common Metagenre defined
//
#define MTP_METAGENRE_NOT_USED                          0x0000
#define MTP_METAGENRE_GENERIC_MUSIC_AUDIO_FILE          0x0001
#define MTP_METAGENRE_GENERIC_NONMUSIC_AUDIO_FILE       0x0011
#define MTP_METAGENRE_SPOKEN_WORD_AUDIO_BOOK_FILES      0x0012
#define MTP_METAGENRE_SPOKEN_WORD_NONAUDIO_BOOK_FILES   0x0013
#define MTP_METAGENRE_SPOKEN_WORD_NEWS                  0x0014
#define MTP_METAGENRE_SPOKEN_WORD_TALK_SHOWS            0x0015
#define MTP_METAGENRE_GENERIC_VIDEO_FILE                0x0021
#define MTP_METAGENRE_NEWS_VIDEO_FILE                   0x0022
#define MTP_METAGENRE_MUSIC_VIDEO_FILE                  0x0023
#define MTP_METAGENRE_HOME_VIDEO_FILE                   0x0024
#define MTP_METAGENRE_FEATURE_FILM_VIDEO_FILE           0x0025
#define MTP_METAGENRE_TV_SHOW_VIDEO_FILE                0x0026
#define MTP_METAGENRE_TRAINING_VIDEO_FILE               0x0027
#define MTP_METAGENRE_PHOTO_MONTAGE_VIDEO_FILE          0x0028
#define MTP_METAGENRE_GENERIC_NONAUDIO_NONVIDEO_FILE    0x0030
#define MTP_METAGENRE_VENDOR_DEFINED_MASK               0x8000

//
// Common NumberOfChannels defined
//
#define MTP_CHANNELS_NOT_USED                           0x0000
#define MTP_CHANNELS_MONO                               0x0001
#define MTP_CHANNELS_STEREO                             0x0002
#define MTP_CHANNELS_2DOT1                              0x0003
#define MTP_CHANNELS_3                                  0x0004
#define MTP_CHANNELS_3DOT1                              0x0005
#define MTP_CHANNELS_4                                  0x0006
#define MTP_CHANNELS_4DOT1                              0x0007
#define MTP_CHANNELS_5                                  0x0008
#define MTP_CHANNELS_5DOT1                              0x0009
#define MTP_CHANNELS_6                                  0x000A
#define MTP_CHANNELS_6DOT1                              0x000B
#define MTP_CHANNELS_7                                  0x000C
#define MTP_CHANNELS_7DOT1                              0x000D
#define MTP_CHANNELS_8                                  0x000E
#define MTP_CHANNELS_8DOT1                              0x000F
#define MTP_CHANNELS_9                                  0x0010
#define MTP_CHANNELS_9DOT1                              0x0011

/*
 * Every structure must be packed on byte boundary
 */

/**
 * USB Setup Data Packet
 *
 * BYTE bmRequestType:
 *  ---------------------------------
 *  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *  ---------------------------------
 *  0-4 - Recipient Value.
 *          0 = Device
 *          1 = Interface
 *          2 = Endpoint
 *          3 = Other
 *          4-31 = Reserved
 *  5-6 - Type
 *          0 = Standard
 *          1 = Class
 *          2 = Vendor
 *          3 = Reserved
 *  7 - Data transfer direction
 *          0 = Host-to-device
 *          1 = Device-to-host
 *
 * BYTE bRequest:
 *  Specific request (refer to Table 9-3).
 *
 * WORD wValue:
 *  Varies according to request type.
 *
 * WORD wIndex:
 *  Varies according to request, but is typically used as a index or offset.
 *
 * WORD wLength:
 *  Number of bytes to transfer if there is a data stage.
 */
typedef struct tagUsbSetupData
{
    BYTE    bmRequestType;
    BYTE    bRequest;
    UINT16    wValue;
    UINT16    wIndex;
    UINT16    wLength;
} USB_SETUP_DATA, *PUSB_SETUP_DATA;

/*
 * USB class-specific requests
 */
#define USB_PTPREQUEST_TYPE_OUT   0x21    /* Host to Device */
#define USB_PTPREQUEST_TYPE_IN    0xA1    /* Device to Host */
#define USB_PTPREQUEST_CANCELIO   0x64    /* Cancel request */
#define USB_PTPREQUEST_GETEVENT   0x65    /* Get extened event data */
#define USB_PTPREQUEST_RESET      0x66    /* Reset Device */
#define USB_PTPREQUEST_GETSTATUS  0x67    /* Get Device Status */

#define USB_PTPCANCELIO_ID   0x4001

/**
 * PTP Cancellation Request
 *
 * WORD wCancelIOCode:
 *  Identifier for cancellation.  This must equal USB_PTPCANCELIO_ID.
 *
 * UINT32 TransactionId:
 *  Transaction to cancel.
 */
typedef struct tagCancelReqData
{
    UINT16 wCancelIOCode;  	//Identifier for cancellation.  This must equal USB_PTPCANCELIO_ID
    UINT32 TransactionId;  	//Transaction to cancel.
} USB_PTP_CANCELREQ_DATA, *PUSB_PTP_CANCELREQ_DATA;

/*
 * PTP Cancellation Request
 *
 * WORD wLength:
 *  Total length of the status data.
 *
 * WORD Code:
 *  Response code.
 *
 * UINT32 Params.
 *  Params depends on the status code.
 */

typedef struct tagStatusReqData
{
    UINT16    wLength;        	//Total length of the status data
    UINT16    Code;          		//Response code
//    UINT16   Params[1];    	//Params depends on the status code
    UINT16   Params[2];    	//Params depends on the status code
} USB_PTP_STATUSREQ_DATA, *PUSB_PTP_STATUSREQ_DATA;



/*
 * Every structure must be packed on byte boundary
 */

typedef enum eContainerType
{
  Undefined = 0x00,
  CommandBlock = 0x01,
  DataBlock = 0x02,
  ResponseBlock = 0x03,
  EventBlock = 0x04
} ContainerType;

#define MAX_MTP_PARAMS 5
#define MAX_MTP_EVENT_PARAMS 1
#define SIZE_IN_BYTES_Usb_Container_Header 12
#define SIZE_IN_WORDS_Usb_Container_Header 4
#define MAX_SIZE_IN_BYTES_Usb_Container SIZE_IN_BYTES_Usb_Container_Header + (4*MAX_MTP_PARAMS)
#define SIZE_IN_BYTES_Usb_Event_Container SIZE_IN_BYTES_Usb_Container_Header + (4*MAX_MTP_EVENT_PARAMS)

typedef struct sUsbGenericContainerHeader
{
  UINT32 Length;         /* the valid size, in BYTES, of the container  */
  UINT16 Type;           /* Container type */
  UINT16 Code;           /* Operation code, response code, or Event code */
  UINT32 TransactionID;  /* host generated number */
} UsbGenericContainerHeader;

typedef struct sUsbGenericContainer
{
  UINT32 Length;         /* the valid size, in BYTES, of the container */
  UINT16 Type;           /* Container type */
  UINT16 Code;           /* Operation code, response code, or Event code */
  UINT32 TransactionID;  /* host generated number */
  UINT32 Params[MAX_MTP_PARAMS];
  UINT32 NumParams;
} UsbGenericContainer;


typedef struct sUsbDataContainer
{
  UINT32 Length;         /* the valid size, in BYTES, of the container */
  UINT16 Type;           /* Container type */
  UINT16 Code;           /* Operation code, response code, or Event code */
  UINT32 TransactionID;  /* host generated number */
  UINT32 Params[MAX_MTP_PARAMS];
  UINT32 NumParams;
  BYTE *pData;
} UsbDataContainer;

typedef UsbGenericContainer SUsbCmdBlock;
typedef UsbGenericContainer SUsbResponseBlock;
typedef UsbDataContainer    SUsbDataBlock;

#endif

