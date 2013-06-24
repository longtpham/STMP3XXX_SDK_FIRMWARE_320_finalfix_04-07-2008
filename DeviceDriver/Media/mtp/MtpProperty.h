/****************************************************************************
 *
 * @module      MtpProperty.h
 *
 * @created     18-Feburary-2004
 *
 * @abstract    Contains the declaration of the SMtpDeviceProperty object
 *              and SMtpObjectPropDesc object.
 *
 * @copyright   (C) COPYRIGHT POWER Q INC., 2004
 *
 ****************************************************************************/
#ifndef __MTPPROPERTY_H_INCLUDED
#define __MTPPROPERTY_H_INCLUDED

#include "PtpString.h"

#define MAX_SIZE_IN_BYTES_OF_PROP_VALUE   256

#define     MAX_DEFAULT_PROP_VALUES     5
#define     MAX_CURRENT_PROP_VALUES     5
#define     MAX_DEVICE_PROP_VALUES      5
#define     MAX_ENUM_VALUES				10

typedef enum PropFormFlag
{
  None              = 0x00, 
  RangeForm             = 0x01,
  EnumForm              = 0x02,
  DateTimeForm          = 0x03,
  FixedLengthArrayForm  = 0x04,
  RegularExpressionForm = 0x05,
  ByteArray             = 0x06,
  LongStringForm        = 0xFF
} ePropFormFlag;

typedef enum _GroupCodeFlag
{
  GroupCodeNone        = 0x0000,
  GroupCodeImages      = 0x0001,
  GroupCodeAvi         = 0x0002,
  GroupCodeAll         = 0xFFFF
}GroupCodeFlag;

typedef struct MtpDevicePropDesc
{
    UINT16  PropCode;       /* Property code for this property */
    UINT16  DataType;       /* Contains the type of the data (2=BYTE, 4=WORD, 6=DWORD, 0xFFFF=String) */
                            /* For array data type, assume no form for now (no such property defined) */
    BYTE    GetSet;         /* Indicates whether the property can be set or not (0=get-only, 1=get-set) */
    /* Default value */
    union
    {
        UINT32       Integer;    /* Default value for any integer value type (UINT8, UINT16, UINT32) */
        SPtpString  Str;     /* Default value for a String value type */
        SPtpArray Array;    /* Default array */
        UINT32    DataArray[MAX_DEFAULT_PROP_VALUES];
    } Default;

    /* Current Value */
    union
    {
        UINT32       Integer;    /* Current value for any integer value type (UINT8, UINT16, UINT32) */
        SPtpString  Str;     /* Current value for a String value type */
        SPtpArray  Array;    /* Current value for array data type (AINT8, AUINT8) */
        UINT32      DataArray[MAX_CURRENT_PROP_VALUES];
    } Current;

    BYTE    FormFlag;       /* Indicates the form of the valid values (0=none, 1=range, 2=enum) */
    /*
     * Range values
     */
    union 
    {
		union {
			struct
			{
				UINT32    dwRangeMin;            /* Minimum value */
				UINT32    dwRangeMax;            /* Maximum value */
				UINT32    dwRangeStep;           /* Step value    */
			} Integer;
			struct
			{
	//fam:            SPtpString StrRangeMin;
	//fam:            SPtpString StrRangeMax;
	//fam:            SPtpString StrRangeStep;
				SPtpShortString StrRangeMin;
				SPtpShortString StrRangeMax;
				SPtpShortString StrRangeStep;
			} PtpString;
		} Range;

		/*
		* Enumeration values
		*/
//		SLinkList SupportedValueList;
		struct {
			// sigmatel device shall not support enums of type strings
			UINT16 NumEnumValues;
			UINT32 SupportedValueList[MAX_ENUM_VALUES];             
		} EnumList;
    } Form;

    UINT32   dwSizeDTS;      /* Count of Byte Size of the Data-Type-Specific value (DTS) */
	GroupCodeFlag	GroupCode;
} SMtpDevicePropDesc;

/**
 * Allocates the memory for a MTP device property structure.
 */
//SMtpDeviceProperty *allocMtpDeviceProperty();
/**
 * Frees up the memory used by the Mtp Device Property structure.  It also
 * deallocates all the memory used by the different elements of the structure.
 */
//void destroyMtpDeviceProperty(SMtpDeviceProperty *pProp);

/**
 * Checks to see if the value is a valid property
 */
BOOL isValidInteger(SMtpDevicePropDesc *pProp, UINT32 dwValue);

/**
 * Checks to see if the string is a valid property
 */
BOOL isValidString(SMtpDevicePropDesc *pProp, SPtpString* pStrValue);

/**
 * Sets the value of the current integer.
 */
BOOL setCurrentInteger(SMtpDevicePropDesc *pProp, UINT32 dwValue);

/**
 * Sets the value of the current string with a unicode character
 * array.
 */
BOOL setCurrentString(SMtpDevicePropDesc *pProp, SPtpString* pStrValue);

/**
 * Sets the value of the current array.
 */
BOOL  setCurrentArray(SMtpDevicePropDesc *pProp, WORD _X *pRawArray);

/**
 * Sets the value of the current union from a raw array of data.
 */
BOOL setCurrentRaw(SMtpDevicePropDesc *pProp, WORD _X *pRawValue, UINT32 cbSize);


/**
 * Sets the range for the integer.
 */
BOOL  setRangeInteger(SMtpDevicePropDesc *pProp, UINT32 dwRangeMin, UINT32 dwRangeMax, UINT32 dwRangeStep);

/**
 * Sets the range for the string.
 */
BOOL  setRangeString(SMtpDevicePropDesc *pProp, WCHAR* pszRangeMin, WCHAR* pszRangeMax, WCHAR* pszRangeStep);

/**
 * Adds a supported value for the property.
 */
BOOL  addSupportedValueInteger(SMtpDevicePropDesc *pProp, UINT32 dwValue);


/**
 * Returns the sizeof the value in bytes.
 */
UINT32 valueSizeInBytesMtpDevicePropDesc(SMtpDevicePropDesc *pProp);


/**
 * Initializes the MTP device property description structure.
 */
void initMtpDevicePropDesc(SMtpDevicePropDesc *pDesc, UINT16 PropCode,
    UINT16 DataType, BYTE GetSet, BYTE FormFlag, UINT32 Groupcode);

/**
 * Allocates memory and initalizes the property description structure.
 */
SMtpDevicePropDesc *allocMtpDevicePropDesc(void);

/**
 * Frees all the memory allocated by the property description structure.
 */
void destroyMtpDevicePropDesc(SMtpDevicePropDesc *pDesc);

/**
 * Sets the default integer value.
 */
BOOL setDefaultIntegerDesc(SMtpDevicePropDesc *pDesc, UINT32 dwValue);

/**
 * Sets the default string value.
 */
BOOL setDefaultStringDesc(SMtpDevicePropDesc *pDesc, char * pValue);

/**
 * Sets the default array value.
 */
BOOL setDefaultArrayDesc(SMtpDevicePropDesc *pDesc, BYTE* pArray, UINT32 dwNumOfElements);

/**
 * Sets the integer range in the device property description.
 */
BOOL  setRangeIntegerDesc(SMtpDevicePropDesc *pDesc, UINT32 dwRangeMin, UINT32 dwRangeMax, UINT32 dwRangeStep);

/**
 * Sets the string rage in the device property description.
 */
BOOL  setRangeStringDesc(SMtpDevicePropDesc *pDesc, WCHAR* pszRangeMin, WCHAR* pszRangeMax, WCHAR* pszRangeStep);

/**
 * Adds a supported integer value to the description.
 */
BOOL  addSupportedValueIntegerDesc(SMtpDevicePropDesc *pDesc, UINT32 dwValue);

/**
 * Checks to see if the value is a valid integer.
 */
BOOL  isValidIntegerDesc(SMtpDevicePropDesc *pDesc, UINT32 dwValue);

/**
 * Checks to see if the string is a valid string.
 */
BOOL  isValidStringDesc(SMtpDevicePropDesc *pDesc, SPtpString* pStrValue);

/**
 * Finds the size of the Mtp device property description.
 */
UINT32  sizeInBytesMtpDevicePropDesc(SMtpDevicePropDesc *pDesc);

/**
 * Packs the device property description into a raw byte array.
 */
//UINT32 packMtpDevicePropDesc(SMtpDevicePropDesc *pDesc, BYTE* pBuf, UINT32 cbBufSize);




typedef struct MtpDevicePropValue
{
    SMtpDevicePropDesc * pDesc;
/*    union
    {
      UINT32       Integer;      // Current value for any integer value type (UINT8, UINT16, UINT32)
      SPtpString  Str;    // Current value for a String value type
      SPtpArray Array;
      UINT32    DataArray[MAX_DEVICE_PROP_VALUES];
    } Current;
*/
} SMtpDevicePropValue;

UINT32 GetPropCode(SMtpDevicePropValue *pVal);
UINT32 GetDataType(SMtpDevicePropValue *pVal);
UINT32 GetGroupCode(SMtpDevicePropValue *pVal);
BYTE* Get(SMtpDevicePropValue *pVal); 
void  initMtpDevicePropValue(SMtpDevicePropValue *pVal, SMtpDevicePropDesc* pDesc);
SMtpDevicePropValue * allocMtpDevicePropValue(SMtpDevicePropDesc *pDesc);
UINT32  sizeInBytesMtpDevicePropValue(SMtpDevicePropValue *pVal);
void  destroyMtpDevicePropValue(SMtpDevicePropValue *pVal);
UINT32  packMtpDevicePropValue(SMtpDevicePropValue *pVal, BYTE* pBuf, UINT32 cbBufSize);
BOOL  setCurrentIntegerVal(SMtpDevicePropValue *pVal, UINT32 dwValue);
BOOL  setCurrentStringVal(SMtpDevicePropValue *pVal, SPtpString* pStrValue);
BOOL  setCurrentArrayVal(SMtpDevicePropValue *pVal, WORD _X *pRawArray, UINT32 *pActualArray);
BOOL  setCurrentRawVal(SMtpDevicePropValue *pVal, WORD _X *pRawValue, UINT32 cbSize);
void  resetMtpDevicePropVal(SMtpDevicePropValue *pVal);


/*
 * Define ObjectPropQuad and CMtpObjectPropList for GetObjectPropList
 */
typedef struct tagObjectPropQuad
{
    UINT32 ObjectHandle;
    UINT16 PropCode;
    UINT16 DataType;
    BYTE* pValue;
    UINT32 dwValueSize;    // a useful helper, not part of the Object Property Quadruple
} SObjectPropQuad;
/*
typedef struct MtpDevicePropList
{
  SLinkList PropQuadList;
} SMtpDevicePropList;
*/
//UINT32  sizeInBytesMtpDevicePropList(SMtpDevicePropList *pList);
//UINT32  packMtpDevicePropList(SMtpDevicePropList *pList, BYTE* pBuf, UINT32 cbBufSize);
//BOOL   appendMtpDevicePropList(SMtpDevicePropList *pList, UINT32 ObjectHandle, 
//                      UINT16 PropCode, UINT16 DataType, BYTE* pValue);
//SMtpDevicePropList *allocMtpDevicePropList();
//void destroyMtpDevicePropList(SMtpDevicePropList *pList);
//UINT32 countMtpDevicePropList(SMtpDevicePropList *pList);


#endif
