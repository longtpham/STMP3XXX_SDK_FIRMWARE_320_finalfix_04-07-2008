/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000
// STMP SmartMedia        
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(smequ))
#define smequ 1

// SmartMedia SmReadParams and SmWriteParams
#define SM_RWP_ADDRLOW 0
#define SM_RWP_ADDRHIGH 1
#define SM_RWP_PAGEBUFADDR 2
#define SM_RWP_NUMBYTES 3
#define SM_RWP_CARDINFO 4
#define SM_RWP_FLAGS 5
#define SM_RWP_STATUS 6

#define SM_ICMD_MASK 0x000100
#define SM_ADDRLOWBYTE_MASK 0xFFFF00
#define SM_ADDRLOWBYTEUPPER_MASK 0x0000FF

#define SM_BUFSHIFT 0x000080
#define SM_NUMBYTESSHIFT 0x000020

#define SM_RWP_FLAGS_ECCEN_MASK 0x000003
#define SM_RWP_FLAGS_ECCEN00 0x000000
#define SM_RWP_FLAGS_ECCEN01 0x000001
#define SM_RWP_FLAGS_ECCENLOW 0
#define SM_RWP_FLAGS_ECCENHIGH 1
#define SM_RWP_FLAGS_IDFEN 2
#define SM_RWP_FLAGS_BSBEN 3
#define SM_RWP_FLAGS_RTNERROR 4

// SmartMedia SmCardnInfo
#define SM_CI_CARDNUM 0
#define SM_CI_ID 1
#define SM_CI_PAGESIZE 2
#define SM_CI_BLOCKSIZE 3
#define SM_CI_BLOCKBYTESIZE 4
#define SM_CI_CARDSIZE 5
#define SM_CI_CARDMB 6
#define SM_CI_STATUS 7

// SmartMedia Device Codes for SM_CI_ID
//mask off the manufacturer id
#define SM_ID_MASK 0x00FF00    
#define SM_ID_1a 0x006E00
#define SM_ID_1b 0x00E800
#define SM_ID_1c 0x00EC00
#define SM_ID_2 0x00EA00
#define SM_ID_4a 0x00E300
#define SM_ID_4b 0x00E500
#define SM_ID_8 0x00E600
#define SM_ID_16 0x007300
#define SM_ID_32 0x007500
#define SM_ID_64 0x007600
#define SM_ID_128 0x007900

// SmReadIdParams
#define SM_RID_PAGEBUFADDR 0
#define SM_RID_CARDINFO 1
#define SM_RID_STATUS 2

// SmStatusReadParams
#define SM_SR_PAGEBUFADDR 0
#define SM_SR_CARDINFO 1
#define SM_SR_STATUS 2

// SmBlockEraseParams
#define SM_BE_ADDRLOW 0
#define SM_BE_ADDRHIGH 1
#define SM_BE_PAGEBUFADDR 2
#define SM_BE_CARDINFO 3
#define SM_BE_STATUS 4

#define SM_IDFZEROERRORCOUNT 4
#define SM_BSBZEROERRORCOUNT 2

// Card Status
#define SM_CS_PASSFAIL_BIT 0
#define SM_CS_READYBUSY_BIT 6
#define SM_CS_WRITEPROTECT_BIT 7

#define FLH_PAGEBUFSIZE 0x00B0
#define SM_PAGEBUFSIZE 0x00B0
// !!!!!!!!!!!!! We're assuming SmartMedia >= 4MB !!!!!!!!!!!!!!!!!!!!!!!!
//4 MBytes and up
#define SM_PAGESIZE 0x0200  
//2 MBytes and under
#define SM_PAGESIZE2 0x0100  
#define SM_PAGESPERBLOCK16 32
#define SM_PAGESPERBLOCK8 16
#define SM_PHYSBLOCKSIZE16 SM_PAGESIZE*SM_PAGESPERBLOCK16
#define SM_PHYSBLOCKSIZE8 SM_PAGESIZE*SM_PAGESPERBLOCK8
#define SM_PHYSBLOCKSIZE2 SM_PAGESIZE2*SM_PAGESPERBLOCK8
#define SM_PAGESTATE_A 1
#define SM_PAGESTATE_B 2
#define SM_PAGESTATE_C 3




// PageBuf offset $AC (172).
#define SM_STATUS_BYTES 0xAC     
//   CIS/IDI Field:
//      byte 0 = Invalid Data Flag
//      byte 1 = $ff
//   All other pages:
//      byte 0 = Data Status Byte
//      byte 1 = Block Status Byte
// For byte 0, if 4 or more bits are zero
//   then data is invalid
// For byte 1, if 2 or more bits are zero
//   the block is invalid

#define SM_INFO_CARD_DETECTED 0
#define SM_INFO_READ_ID_TIMEOUT 1
#define SM_INFO_READ_TIMEOUT 2
#define SM_INFO_END_OF_BLOCKS 3
#define SM_INFO_READ_ID_BAD_PROGRAMMING 4
#define SM_INFO_READ_BAD_PROGRAMMING 5

#endif


