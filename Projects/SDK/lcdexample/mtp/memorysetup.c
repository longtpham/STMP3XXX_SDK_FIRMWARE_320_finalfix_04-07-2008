#include "types.h"
#include "resource.h"

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
} MemoryRegion;

//define each memory region
extern _P INT _lc_u_bP_space_funclet;
extern _P INT _lc_u_eP_space_funclet;
MemoryRegion g_FuncletRegion = {0,&_lc_u_bP_space_funclet,&_lc_u_eP_space_funclet};

extern _P INT _lc_u_bP_FileSystem_cluster;
extern _P INT _lc_u_eP_FileSystem_cluster;
MemoryRegion g_P_FileSystem_cluster = {0,&_lc_u_bP_FileSystem_cluster,&_lc_u_eP_FileSystem_cluster};

extern _P INT _lc_u_bP_sys_cluster;
extern _P INT _lc_u_eP_sys_cluster;
MemoryRegion g_SysRegion = {0xFFFFFF,&_lc_u_bP_sys_cluster,&_lc_u_eP_sys_cluster};

//
// This region is used for the usb mass storage only code that gets swapped
// out when MTP is detected.
//
// Mar 1 2007: g_USBMSCRegion is not referred anymore and it was removed
//extern _P INT _lc_u_bP_usbmsc_cluster;
//extern _P INT _lc_u_eP_usbmsc_cluster;
//MemoryRegion g_USBMSCRegion = {0,&_lc_u_bP_usbmsc_cluster,&_lc_u_eP_usbmsc_cluster};

extern _P INT _lc_u_MediaRegion_b;
extern _P INT _lc_u_MediaRegion_e;
MemoryRegion g_MediaRegion = {RSRC_NANDMEDIAINIT,&_lc_u_MediaRegion_b,&_lc_u_MediaRegion_e};

extern _P INT _lc_u_MTPRegion_b;
extern _P INT _lc_u_MTPRegion_e;
MemoryRegion g_MTPRegion = {RSRC_MTP_INITDEVICE,&_lc_u_MTPRegion_b,&_lc_u_MTPRegion_e};

MemoryRegion *g_RegionTable[] = 
{
  &g_MTPRegion,
  &g_SysRegion,
  &g_MediaRegion,
  &g_FuncletRegion,
  &g_P_FileSystem_cluster,
  NULL
};

#pragma asm
    org p,".filler":2
    ds $7E
#pragma endasm


