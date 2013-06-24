
#define DRM_DRMMANAGER_CONTEXT_BUFFER_SIZE 3379
#define MAX_LICRESPONSE_SIZE          4600
#define DRM_MAX_HEADERDATASIZE        500                         /* 5K of header data                                                      */
#define DRM_MAX_LICENSESIZE           3700
#define MAX_METER_CHALLENGE_SIZE      3000
#define MAX_DRM_STACKBUFFER_SIZE      1500

#if !DRM_LICENSE_STATE_TYPES_DEFINED
/* Enum and structure for license properties queries */
typedef enum DRM_LICENSE_STATE_CATEGORY
{
    WM_DRM_LICENSE_STATE_NORIGHT = 0,
    WM_DRM_LICENSE_STATE_UNLIM,
    WM_DRM_LICENSE_STATE_COUNT,
    WM_DRM_LICENSE_STATE_FROM,
    WM_DRM_LICENSE_STATE_UNTIL,
    WM_DRM_LICENSE_STATE_FROM_UNTIL,
    WM_DRM_LICENSE_STATE_COUNT_FROM,
    WM_DRM_LICENSE_STATE_COUNT_UNTIL,
    WM_DRM_LICENSE_STATE_COUNT_FROM_UNTIL,
    WM_DRM_LICENSE_STATE_EXPIRATION_AFTER_FIRSTUSE,
    WM_DRM_LICENSE_STATE_FORCE_SYNC
} DRM_LICENSE_STATE_CATEGORY;
#endif

typedef enum DRM_DEVICE_PROPERTY
{
    WM_DRM_CLIENTINFO = 0,
    WM_DRM_DRMVERSION,
    WM_DRM_SECURITYVERSION,
    WM_DRM_V2_HEADER_KID
} DRM_DEVICE_PROPERTY;

typedef struct __tagDRM_MANAGER_CONTEXT
{
    /* This data is Opaque.  Do not set any value in it. */
    DRM_BYTE rgbOpaqueBuffer[ __CB_DECL( DRM_DRMMANAGER_CONTEXT_BUFFER_SIZE ) ];
} DRM_MANAGER_CONTEXT;

#define DRM_METER_RESPONSE_PARTIAL 0x00000001

/**********************************************************************
**
** Function:    DRM_MGR_Initialize
**
** Synopsis:    Initializes DRM.  The context that is initialized after this call can be used in later calls
**              to most DRM functions to perform a varety of DRM related tasks.
**
** Arguments:   [pDrmContext]          -- Opaque DRM Manager context
**                [pdstrDeviceStoreName]    -- Path DRM should use as the current store.
**
** Returns:        DRM_SUCCESS on success or failed with,
**              DRM_E_NEEDDEVCERTINDIV if the device needs to be individualized
**              DRM_E_INVALIDARG if either parameter is incorrect or 
**                other errors found in drmresults.h
**
** Notes:       The pDrmContext should be uninitialized with a call to DRM_MGR_Uninitialize
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_Initialize( 
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN const DRM_STRING     *pdstrDeviceStoreName );

/**********************************************************************
**
** Function:    DRM_MGR_Uninitialize
**
** Synopsis:    Allows DRM to shutdown any allocated resources.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**
** Returns:        None.
**
**********************************************************************/
DRM_VOID DRM_API DRM_MGR_Uninitialize( 
    IN DRM_MANAGER_CONTEXT *pDrmContext );


#if DRM_SUPPORT_DLA
/**********************************************************************
**
** Function:    DRM_MGR_GenerateLicenseChallenge
**
** Synopsis:    Using the current DRM header set by SetV2Header DRM will generate a liecense challenge that can be sent
**              to a license server.  The license server generally issues a response that should be passed to 
**              DRM_MGR_ProcessLicenseResponse.
**
** Arguments:   [pDrmContext]   -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**                [ppdstrRights] -- An array of DSTR*s representing rights that the caller would like to access the content with.
**                [cRights]      -- Count of the pointers in the rgpdstrRights array.
**                [pwszUrl]      -- user allocated buffer to get URL in WCHAR. Null Terminated.
**                [pcchUrl]      --  character count of pwszUrl. Can be NULL if URL is not required.
**                [pszChallenge] -- The license challenge will be copied into this string buffer
**                [pcchChallenge]-- Length in characters of the pcchChallenge buffer.
**
** Returns:        DRM_SUCCESS on success or
**              DRM_E_V1_NOT_SUPPORTED if a V1 content header was only set,
**              DRM_E_HEADER_NOT_SET if no content header was set,
**              DRM_E_BUFFERTOOSMALL if the input pszURL or pszChallenge buffers were too small,
**              or other error codes as defined in drmresults.h
**
** Notes:       DRM_MGR_SetV2Header must be called prior to this function.  On return from this function the character
**              count parameters will be updated with the number of characters copied to the buffer if successful
**              or the required length of the buffer if the current buffer size is too small.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GenerateLicenseChallenge(
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN const DRM_CONST_STRING    **ppdstrRights, /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD             cRights,
       OUT   DRM_WCHAR            *pwszUrl,
//    IN OUT   DRM_DWORD            *pcchUrl,
       OUT   DRM_CHAR   TFR_MEMORY  *pszChallenge,
//    IN OUT   DRM_DWORD            *pcchChallenge );
    IN       DRM_DWORD            *Store[]); 


/**********************************************************************
**
** Function:    DRM_MGR_GetDeviceProperty
**
** Synopsis:    Retrieves clients properties.  Available properties are defined in the enum DRM_DEVICE_PROPERTY.
**
** Arguments:   [pDrmContext]   -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [eProperty]     -- The property to retrieve.  Must be a value from the DRM_DEVICE_PROPERTY enum
**              [pbProperty]    -- Buffer to receive the property value.  The format of the data depends upon the property.
**              [pcbProperty]   -- Count of bytes in pbProperty
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_HEADER_NOT_SET if no content header was set,
**              DRM_E_BUFFERTOOSMALL if the input pbProperty buffer is too small,
**              or other error codes as defined in drmresults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetDeviceProperty(
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN       DRM_DEVICE_PROPERTY    eProperty, 
       OUT   DRM_BYTE    TFR_MEMORY         *pbProperty,
    IN OUT   DRM_DWORD            *pcbProperty );
#endif

/**********************************************************************
**
** Function:    DRM_MGR_ProcessLicenseResponse
**
** Synopsis:    Process a response from a license server.  Usually this is a series of licenses that will ultimately be
**              stored in the device license store.
**
** Arguments:   [f_pDrmContext]       -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**                [f_pfnCallback]       -- Pointer to a callback function which is called for each license in response. Can be NULL.
**                [f_pvCallbackContext] -- User context returned in callback function. Can be NULL
**                [f_pbResponse]        -- Pointer to a response blob from a license server
**                [f_cbResponse]        -- Count of bytes in pbResponse
**
** Returns:        DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if the arguments are invalid
**              or any other value as defined in drmresults.h
**
** Notes:       pbResponse is not declared as const.  This buffer will be modified in place to conserve device memory.
**              If caller needs a copy unmodified they should copy it elsewhere first.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ProcessLicenseResponse(
    IN DRM_MANAGER_CONTEXT     *f_pDrmContext,
    IN DRM_BYTE TFR_MEMORY *f_pbResponse,
    IN DRM_DWORD                f_cbResponse );

#if DRM_SUPPORT_METERING
/**********************************************************************
**
** Function:    DRM_MGR_GenerateMeterChallenge
**
** Synopsis:    Generates a challenge that includes all the metering information for a specific MAS identified by the
**              metering certificate.
**
** Arguments:   [pDrmContext]    -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**                [pdstrMeterCert]    -- Metering certificate that identifies the metering ID that wants to collect data.  This is an XML string.
**              [pwszUrl] -- user allocated buffer to get URL in WCHAR.
**              [pcchUrl] --  character count of pwszUrl. Can be NULL if URL is not required.
**              [pbChallenge]    -- Buffer to hold the metering challenge
**                [pcbChallenge]-- Size in bytes of pbData on input.  Number of bytes copied to pbData on return or length required if the buffer was too small.
**
** Returns:        DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any of the parameters are invalid or
**              any of the return codes as defined in drmresults.h
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_GenerateMeterChallenge(
    IN        DRM_MANAGER_CONTEXT *pDrmContext,
    IN  const DRM_CONST_STRING_MEM    *pdstrMeterCert,
       OUT    DRM_WCHAR  TFR_MEMORY         *pwszUrl,
//    IN OUT    DRM_DWORD           *pcchUrl,
       OUT    DRM_BYTE   TFR_MEMORY         *pbChallenge,
    IN OUT    DRM_DWORD           *Store[]
//    IN OUT    DRM_DWORD           *pcbChallenge
);

/**********************************************************************
**
** Function:    DRM_MGR_ProcessMeterResponse
**
** Synopsis:    A meter repsonse is sent to the device after a sever process the challenge.  This response is handled
**              by DRM here to possible reset the metering data just reported.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**                [pbData]    -- Input buffer containing the response to process.
**                [cbData]    -- Count of bytes in pbData
**                [pfFlagsOut]    -- Status output flags.  Either 0 or DRM_METER_RESPONSE_PARTIAL
**
** Returns:        DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if the argument is invalid or
**              any return code as defined in drmresults.h
**
** Notes:       If DRM_METER_RESPONSE_PARTIAL is returned in pfFlagsOut it indicates that the challenge/response
**              round trip was successful but only a partial amout of the entire metering data was sent.  The application
**              should perform the entire DRM_MGR_GenerateMeterChallenge/DRM_MGR_ProcessMeterReponse cycle again.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ProcessMeterResponse(
    IN      DRM_MANAGER_CONTEXT  *pDrmContext,
    IN      DRM_BYTE    TFR_MEMORY         *pbData,
    IN      DRM_DWORD             cbData,
       OUT  DRM_DWORD            *pfFlagsOut);
#endif   /* DRM_SUPPORT_METERING  */


#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER    0xFFFFFFFF

#if DRM_SUPPORT_LICENSE_SYNC
/**********************************************************************
**
** Function:    DRM_MGR_GenerateSyncChallenge
**
** Synopsis:    Creates a license synchronization challenge in multiple session.
**
** Arguments:   [pDrmContext]  -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [cMaxCount]    -- maximum remaining playcount before license expiration to use evaluating licenses with
**                                count-based expirations.  Use DRM_SYNC_IGNORE_THRESHOLD_PARAMETER (0xFFFFFFFF) to
**                                ignore this parameter and always sync count-based licenses
**              [cMaxHours]    -- maximum number of hours before license expiration to use evaluating licenses with
**                                date-based expirations.  Use DRM_SYNC_IGNORE_THRESHOLD_PARAMETER (0xFFFFFFFF) to
**                                ignore this parameter and always sync date-based licenses
**              [iStart]       -- starting index for challenge
**              [cItemsToProcess]   -- total number items to be processed 
**              [piNextStart]       -- Starting index for next challenge 
**              [pcProcessed]       -- total number of item processed

**              [pbData]       -- user-allocated buffer to be used as output buffer
**              [pcbData]      -- size of pbChallenge
**
** Returns:     DRM_SUCCESS on success. On failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**
** Notes:       To get the minimum size required, first call with pbChallenge = NULL and
**              *pcbChallenge =0.  This will return DRM_E_BUFFERTOOSMALL and the pcbChallenge 
**              will point to the required buffer size.
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_GenerateSyncChallenge(
    IN     DRM_MANAGER_CONTEXT *pDrmContext,
    IN     DRM_DWORD            cMaxCount,
    IN     DRM_DWORD            cMaxHours,
    IN     DRM_DWORD            iStart,
    IN     DRM_DWORD            cItemsToProcess,
       OUT DRM_DWORD           *piNextStart,
       OUT DRM_DWORD           *pcProcessed,
       OUT DRM_BYTE   TFR_MEMORY         *pbData,
    IN OUT DRM_DWORD           *pcbData);


#endif /* DRM_SUPPORT_LICENSE_SYNC */

#if DRM_SUPPORT_SECURE_CLOCK
/**********************************************************************
**
** Function:    DRM_MGR_ClkProcessResponse
**
** Synopsis:    Process Secure clockrequest response received from server
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pbResponse] -- Response string received from server
**              [cbResponse] -- size of pbResponse in bytes.
**              [pResult] --pointer to DRM_RESULT to get error from server included in response
**
** Returns:     DRM_SUCCESS on success, or failed with 
**              DRM_E_INVALIDARG,
**              DRM_E_CLK_INVALID_RESPONSE
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ClkProcessResponse(
    IN     DRM_MANAGER_CONTEXT *pDrmContext,
    IN     DRM_BYTE  TFR_MEMORY *pbResponse,
    IN     DRM_DWORD            cbResponse,
       OUT DRM_RESULT          *pResult);

/**********************************************************************
**
** Function:    DRM_MGR_ClkGenerateChallenge
**
** Synopsis:    Creates a Secure Clock challenge.
**
** Arguments:   [pDrmContext]  -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pwszUrl] --  user allocated buffer to get URL in WCHAR. Null Terminated.
**              [pcchUrl] -- character count of pwszUrl. Can be NULL if URL is not required.
**              [pbChallenge] -- user allocated buffer to be used as output buffer
**              [pcbChallenge] -- size of pbChallenge
**
** Returns:     DRM_SUCCESS on success. On failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**
** Notes:       To get the minimum size required, first call with pbChallenge=NULL,
**              *pcbChallenge =0.  
**              This will return DRM_E_BUFFERTOOSMALL and pcchUrl and pcbChallenge 
**              will point to a value of minimum context size.
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_ClkGenerateChallenge(
    IN     DRM_MANAGER_CONTEXT  *pDrmContext,
       OUT DRM_WCHAR  TFR_MEMORY *pwszUrl,
    IN OUT DRM_DWORD            *pcchUrl,
       OUT DRM_BYTE   TFR_MEMORY  *pbChallenge,
    IN OUT DRM_DWORD            *pcbChallenge );

/**********************************************************************
**
** Function:    DRM_MGR_GetSecureClock
**
** Synopsis:    Get Secure clock state
**
** Arguments:   [pDrmContext]             -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pwszSecureTime]          -- pointer to WCHAR buffer to get Secure time. Optional 
**              [pcchSecureTime] -- size of buffer in WCHAR. Optioanl
**              [pdwFlags] -- Pointer to DWORD. Optional
**              [pbSecureTimeWithStatus] -- pointer to BYTE buffer to get Secure time and flag status in XML format. Optional 
**              [pcbSecureTimeWithStatus] -- size of buffer. Optioanl
**
** Returns:     DRM_SUCCESS on success. 
**
** Notes:       There are 2 forms in which the secure clock data is being returned. Either in
**              poSecureTime and pdwFlags or in pbSecureTimeWithStatus. The second one in in XML format
**              which contains both information as in poSecureTime and pdwFlags. You can choose either format.
**              To get minimum buffer size, this API should be called with *pcchSecureTime =0 (or *pcbSecureTimeWithStatus =0)
**              This will return DRM_E_BUFFERTOOSMALL and pcchSecureTime and pcbSecureTimeWithStatus 
**              will point to a value of minimum context size.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetSecureClock(
    IN     DRM_MANAGER_CONTEXT  *pDrmContext,
    IN     DRM_WCHAR            *pwszSecureTime,
//    IN OUT DRM_DWORD            *pcchSecureTime, 
    IN     DRM_DWORD            *pdwFlags,
    IN     DRM_BYTE  TFR_MEMORY           *pbSecureTimeWithStatus,
    IN     DRM_DWORD            *Ptr[]);
//    IN OUT  DRM_DWORD           *pcbSecureTimeWithStatus  )

#endif /* DRM_SUPPORT_SECURE_CLOCK */



/**********************************************************************
**
** Function:    DRM_MGR_CleanupLicenseStore
**
** Synopsis:    Instructs DRM to delete all unusable licenses from the license store and perform maintaince functions 
**              on the data store file.
**
** Arguments:   [pDrmContext]        -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**                [pvCallerData]    -- Opaque context data to be funnelled back to caller during callback. Could be NULL.
**              [dwCallbackInterval] -- % completion after which Callback (if any) should be invoked. 
**                                      e.g. if dwCallbackInterval = 5, callback will be invoked after 5%, 10%, 15%...completion
**                [pfnCallback]    -- optional caller supplied callback function - can be NULL.
**
** Returns:        DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              any return code as defined in drmrestults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_CleanupLicenseStore(
    IN       DRM_MANAGER_CONTEXT     *pDrmContext,
    IN DRM_VOID                *pvCallerData,
    IN       DRM_DWORD                dwCallbackInterval,
    IN       void  *pfnCallback );


/**********************************************************************
**
** Function:    DRM_MGR_GetLicenseState
**
** Synopsis:    Gets License state for the KID included in pdstrQuery. 
**              Currently it tells whether license is usable for Play action.
**
** Arguments:   [pDrmContext]        -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pdstrQuery]         -- Query containing KID, Action etc. 
**              [pCategory]          -- returns DRM_LICENSE_STATE_CATEGORY. 
**              [pdwReserved1]       -- Reserved for future use. Must be NULL. 
**              [pdwReserved2]       -- Reserved for future use. Must be NULL. 
**              [pdwReserved3]       -- Reserved for future use. Must be NULL. 
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              any return code as defined in drmrestults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetLicenseState(
    IN        DRM_MANAGER_CONTEXT        *pDrmContext,
    IN   DRM_CONST_STRING_MEM              *pdstrQuery,
        OUT   DRM_LICENSE_STATE_CATEGORY *pCategory,
    OUT      DRM_DWORD                      *pdwReserved[]);




/**********************************************************************
**
** Function:    DRM_MGR_ProcessCommand
**
**
**********************************************************************/


DRM_RESULT DRM_API DRM_MGR_ProcessCommand(
    IN       DRM_MANAGER_CONTEXT   *f_pcontextMGR,
    IN       DRM_DWORD              f_dwOperationCode,
    IN       DRM_DWORD              f_dwRequestArgument1,
    IN       DRM_DWORD              f_dwRequestArgument2,
    IN       DRM_BYTE  TFR_MEMORY   *f_pbRequestData,
    IN       DRM_DWORD              f_dwRequestDataLength,
        OUT  DRM_DWORD             *f_pdwResponseResult1,
        OUT  DRM_DWORD             *f_pdwResponseResult2);

/**********************************************************************
**
** Function:    DRM_MGR_ProcessRequest
**
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_ProcessRequest( 
    IN       DRM_MANAGER_CONTEXT   *f_pcontextMGR,
    IN       DRM_DWORD              f_dwOperationCode,
//    IN       DRM_DWORD              f_dwRequestArgument1,
//    IN       DRM_DWORD              f_dwRequestArgument2,
//    IN       DRM_DWORD              f_dwRequestArgument3,
//    IN       DRM_DWORD              f_dwRequestArgument4,
        OUT  DRM_DWORD             *f_pdwResponseResult1,
        OUT  DRM_DWORD             *f_pdwResponseResult2,
        OUT  DRM_DWORD             *f_pdwResponseResult3,
        OUT  DRM_DWORD             *f_pdwResponseResult4,
        OUT  DRM_BYTE              *f_pbResponseData,
        OUT  DRM_DWORD             *f_pdwResponseDataLength);


extern DRM_BYTE JanusContext[sizeof(DRM_MANAGER_CONTEXT)];
extern int Janus_InitializeFlag;
extern DRM_STRING s_strLicense;
extern DRM_RESULT CloseHDSStore(DRM_MANAGER_CONTEXT  *pDrmContext);
extern DRM_BYTE TFR_MEMORY ResponseBuffer[MAX_LICRESPONSE_SIZE];
extern DRM_BYTE TFR_MEMORY OverlayBuffer[DRM_MAX_LICENSESIZE+DRM_MAX_HEADERDATASIZE];
extern DRM_BYTE StackBuffer[MAX_DRM_STACKBUFFER_SIZE];
extern void PutByte(void *buffer, INT byte,INT Offset ,INT MemoryType);
extern int FSGetByte(void *buffer, int Offset,int MemoryType);
extern int FSGetWord(void *buffer, int Offset,int MemoryType);
extern void LoadAes_Enc_P(void);
extern _asmfunc void SysUnLoadFATWrite(void);
extern void SysLoadWrite(void);
extern void SysLoadDoplay(void);
extern void SysLoadJanusCommon(void);
extern _asmfunc void SysLoadFATWrite(void);
extern void LoadDRMMD5_P(void);
extern void LoadMTPDataBase_P(void);
extern void CopyMemToMem(void* dest,void *Source,int DestMem,int SourceMem,int Size);

