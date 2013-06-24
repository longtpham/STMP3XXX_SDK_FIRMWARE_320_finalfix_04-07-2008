////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: sysvolume.h
// Description: Prototypes for the Multi-Stage Volume control and variables
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSVOLUME_H
#define _SYSVOLUME_H


// Volume adjustment modes
#define VOLUME_MODE_DAC         0
#define VOLUME_MODE_LINE1       1
#define VOLUME_MODE_LINE2       2
#define VOLUME_MODE_DACLINE1    3


extern int g_iUserVolume;                                   // User-Controlled Volume
extern int g_iVolumeSteps;                                  // Number of available volume steps in current mode
extern int g_iVolumeMode;

void _reentrant SysUpdateVolume(void);

_inline void _reentrant SysIncrementVolume(void)
{
    g_iUserVolume ++;
    SysUpdateVolume();
}

_inline void _reentrant SysDecrementVolume(void)
{
    g_iUserVolume --;
    SysUpdateVolume();
}
#endif // #ifndef _SYSVOLUME_H
