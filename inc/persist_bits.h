//;///////////////////////////////////////////////////////////////////////////////
//; Copyright(C) SigmaTel, Inc. 2000-2005
//;
//; Filename:    persist_bits.h
//; Description: General purpose interface to Persistent Bits
//;              Persistent bits are mapped to more generic bits in regsrtc.inc.
//;///////////////////////////////////////////////////////////////////////////////

#ifndef _PERSIST_BITS_H
#define _PERSIST_BITS_H

#include "regsrtc.h"

// Reserve bit 0 for Low Power USB, if hub can't supply 500mA, need to reboot with 100mA
#define HW_RTC_LOW_POWER_USB_REG                     (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTANT1_LOW_POWER_USB_BITPOS      (0)
#define HW_RTC_PERSISTANT1_LOW_POWER_USB_WIDTH       (1)
#define HW_RTC_PERSISTANT1_LOW_POWER_USB_SETMASK     (((1<<HW_RTC_PERSISTANT1_LOW_POWER_USB_WIDTH)-1)<<HW_RTC_PERSISTANT1_LOW_POWER_USB_BITPOS) 
#define HW_RTC_PERSISTANT1_LOW_POWER_USB_CLRMASK     (~(WORD)HW_RTC_PERSISTANT1_LOW_POWER_USB_SETMASK)

// Reserve bit 1 for Skip Checkdisk.  If disk hasn't changed, don't run checkdisk.
#define HW_RTC_SKIP_CHECKDISK_REG                    (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTENT1_SKIP_CHECKDISK_BITPOS     (1)
#define HW_RTC_PERSISTENT1_SKIP_CHECKDISK_WIDTH      (1)
#define HW_RTC_PERSISTENT1_SKIP_CHECKDISK_SETMASK    (((1<<HW_RTC_PERSISTANT1_SKIP_CHECKDISK_WIDTH)-1)<<HW_RTC_PERSISTANT1_SKIP_CHECKDISK_BITPOS) 
#define HW_RTC_PERSISTANT1_SKIP_CHECKDISK_CLRMASK    (~(WORD)HW_RTC_PERSISTANT1_SKIP_CHECKDISK_SETMASK)

// Reserve bit 2 for Skip building music library internal flash.  If disk hasn't changed, don't run building.
#define HW_RTC_SKIP_REBUILD_REG                  (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS   (2)
#define HW_RTC_PERSISTENT1_SKIP_REBUILD_WIDTH    (1)
#define HW_RTC_PERSISTENT1_SKIP_REBUILD_SETMASK  (((1<<HW_RTC_PERSISTENT1_SKIP_REBUILD_WIDTH)-1)<<HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS) 
#define HW_RTC_PERSISTENT1_SKIP_REBUILD_CLRMASK  (~(WORD)HW_RTC_PERSISTENT1_SKIP_REBUILD_SETMASK)

// Reserve bit 3 for 2nd try Low Power USB, if hub can't supply 500mA (try 2 times)
// need to reboot with 100mA
#define HW_RTC_PERSISTANT1_LOW_POWER2_USB_BITPOS     (3)
#define HW_RTC_PERSISTANT1_LOW_POWER2_USB_WIDTH      (1)
#define HW_RTC_PERSISTANT1_LOW_POWER2_USB_SETMASK    (((1<<HW_RTC_PERSISTANT1_LOW_POWER2_USB_WIDTH)-1)<<HW_RTC_PERSISTANT1_LOW_POWER2_USB_BITPOS) 
#define HW_RTC_PERSISTANT1_LOW_POWER2_USB_CLRMASK    (~(WORD)HW_RTC_PERSISTANT1_LOW_POWER2_USB_SETMASK)

// Reserve bit 5 for MTP database clean flag. 1 => database "clean", 0 => database "dirty"
#define HW_RTC_MTP_DB_CLEAN_FLAG_REG                 (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS  (5)
#define HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_WIDTH   (1)
#define HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_SETMASK (((1<<HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_WIDTH)-1)<<HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS) 
#define HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_CLRMASK (~(WORD)HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_SETMASK)

// Reserve bit 6 for when to use backup resource.bin.  1 => require resource.bat refresh, 0 => no refresh reqd.
#define HW_RTC_RESOURCE_BIN_REFRESH_FLAG_REG              (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_BITPOS  (6)
#define HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_WIDTH   (1)
#define HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_SETMASK (((1<<HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_WIDTH)-1)<<HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_BITPOS) 
#define HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_CLRMASK (~(WORD)HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_SETMASK)

// Reserve bit 7 for when to use backup resource.bin.  1 => require resource.bat refresh, 0 => no refresh reqd.
#define HW_RTC_HOST_RESOURCE_BIN_REFRESH_FLAG_REG              (HW_RTC_PERSISTENT1)
#define HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_BITPOS  (7)
#define HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_WIDTH   (1)
#define HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_SETMASK (((1<<HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_WIDTH)-1)<<HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_BITPOS) 
#define HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_CLRMASK (~(WORD)HW_RTC_PERSISTANT1_HOST_RESOURCE_BIN_REFRESH_FLAG_SETMASK)

// Want to wait 2000 clocks before failing.
#define PERSIST_WAIT_TIME                            2000

void _reentrant StickyBitInit(void);
RETCODE _reentrant ReadStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition, BOOL * bResult);
RETCODE _reentrant SetStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition);
RETCODE _reentrant ClearStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition);
void _reentrant SetBitResourceRefresh(INT bBitValue); // should this line be commented out? I think the func def was commented out since unused.
INT _reentrant GetBitResourceRefresh(void);           // "
void _reentrant StickyBitForceUpdate(void);

#endif // _PERSIST_BITS_H
