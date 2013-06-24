#ifndef __PHILIPS5767_H
#define __PHILIPS5767_H

#define PHILIPS_5767_I2C_ADDRESS 0xc0

#define PHILIPS_3WIRE_DELAY 25    // Thi & Tlow = 1usec.

// Commented code was used for older FM tuner cards
/*
#ifndef HW_3WIRE_CLOCK_ENR
#define HW_3WIRE_CLOCK_ENR  HW_GP0ENR.B.B18
#define HW_3WIRE_CLOCK_DOER HW_GP0DOER.B.B18
#define HW_3WIRE_CLOCK_PWR  HW_GP0PWR.B.B18
#define HW_3WIRE_CLOCK_DOR  HW_GP0DOR.B.B18
#endif
*/


// Changed HW_3WIRE_CLOCK_xxx from GP0/B18 to GP1/B9.  This change 
// supports the pin connections found in the STMP 3410/35xx TEA5767
// FM Tuner daughtercard Rev A and the STMP35xx EVK.
#ifndef HW_3WIRE_CLOCK_ENR
#define HW_3WIRE_CLOCK_ENR  HW_GP1ENR.B.B9
#define HW_3WIRE_CLOCK_DOER HW_GP1DOER.B.B9
#define HW_3WIRE_CLOCK_PWR  HW_GP1PWR.B.B9
#define HW_3WIRE_CLOCK_DOR  HW_GP1DOR.B.B9
#endif

#ifndef HW_3WIRE_DATA_ENR
#define HW_3WIRE_DATA_ENR  HW_GP0ENR.B.B19
#define HW_3WIRE_DATA_DOER HW_GP0DOER.B.B19
#define HW_3WIRE_DATA_DIR  HW_GP0DIR.B.B19
#define HW_3WIRE_DATA_PWR  HW_GP0PWR.B.B19
#define HW_3WIRE_DATA_DOR  HW_GP0DOR.B.B19
#endif


#ifndef HW_3WIRE_BUS_ENABLE_ENR
#define HW_3WIRE_BUS_ENABLE_ENR  HW_GP1ENR.B.B15
#define HW_3WIRE_BUS_ENABLE_DOER HW_GP1DOER.B.B15
#define HW_3WIRE_BUS_ENABLE_PWR  HW_GP1PWR.B.B15
#define HW_3WIRE_BUS_ENABLE_DOR  HW_GP1DOR.B.B15
#endif

#ifndef HW_3WIRE_WRITE_ENABLE_ENR
#define HW_3WIRE_WRITE_ENABLE_ENR  HW_GP2ENR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOER HW_GP2DOER.B.B3
//3200Merge: comment this #if defined(DEVICE_3410)||defined(DEVICE_3310)
#define HW_3WIRE_WRITE_ENABLE_PWR  HW_GP2PWR.B.B3
//3200Merge; comment this #endif
#define HW_3WIRE_WRITE_ENABLE_DOR  HW_GP2DOR.B.B3
#endif

#define HW_FMTUNER_READY_ENR  HW_GP0ENR.B.B16
#define HW_FMTUNER_READY_DOER HW_GP0DOER.B.B16
#define HW_FMTUNER_READY_PWR  HW_GP0PWR.B.B16
#define HW_FMTUNER_READY_DIR  HW_GP0DIR.B.B16


typedef union 
{
    struct {
        unsigned int m_uPLL8_13:     6;
        unsigned int m_bSearchMode:  1;
        unsigned int m_bMute:        1;
    } B;
    unsigned char I;
} Philips5767WriteRegister1Type;

typedef union 
{
    struct {
        unsigned int m_uPLL0_7:      8;
    } B;
    unsigned char I;
} Philips5767WriteRegister2Type;

typedef union 
{
    struct {
        unsigned int m_bSWPort1:        1;
        unsigned int m_bMuteRight:      1;
        unsigned int m_bMuteLeft:       1;
        unsigned int m_bMonoStereo:     1;
        unsigned int m_bHiLoInjection:  1;
        unsigned int m_uSearchStopLevel:2;
        unsigned int m_bSearchDirection:1;
    } B;
    unsigned char I;
} Philips5767WriteRegister3Type;

typedef union 
{
    struct {
        unsigned int m_bSearchIndicator:     1;
        unsigned int m_bSNC:                 1;
        unsigned int m_bHCC:                 1;
        unsigned int m_bSoftMute:            1;
        unsigned int m_bXTAL:                1;
        unsigned int m_bBandLimits:          1;
        unsigned int m_bStandby:             1;
        unsigned int m_bSWPort2:             1;
    } B;
    unsigned char I;
} Philips5767WriteRegister4Type;

typedef union 
{
    struct {
        unsigned int m_Reserved:         6;
        unsigned int m_bDeemphasisTime:  1;
        unsigned int m_bPLLReference:    1;
    } B;
    unsigned char I;
} Philips5767WriteRegister5Type;

typedef union 
{
    struct {
        unsigned int m_uPLL8_13:         6;
        unsigned int m_bBandLimit:       1;
        unsigned int m_bReadyFlag:       1;
    } B;
    unsigned char I;
} Philips5767ReadRegister1Type;

typedef union 
{
    struct {
        unsigned int m_uPLL0_7:          8;
    } B;
    unsigned char I;
} Philips5767ReadRegister2Type;

typedef union 
{
    struct {
        unsigned int m_uIFCounter:       7;
        unsigned int m_bStereo:          1;
    } B;
    unsigned char I;
} Philips5767ReadRegister3Type;

typedef union 
{
    struct {
        unsigned int m_bUnused:          1;
        unsigned int m_uChipIdent:       3;
        unsigned int m_uADCLevel:        4;        
    } B;
    unsigned char I;
} Philips5767ReadRegister4Type;

typedef union 
{
    struct {
        unsigned int m_Reserved:         8;
    } B;
    unsigned char I;
} Philips5767ReadRegister5Type;

union WriteRegisterBundle{
    struct {
        Philips5767WriteRegister1Type B1;
        Philips5767WriteRegister2Type B2;
        Philips5767WriteRegister3Type B3;
        Philips5767WriteRegister4Type B4;
        Philips5767WriteRegister5Type B5;
        } Bytes;
    WORD Array[5];
} ;

union ReadRegisterBundle
{
    struct {
        Philips5767ReadRegister1Type B1;
        Philips5767ReadRegister2Type B2;
        Philips5767ReadRegister3Type B3;
        Philips5767ReadRegister4Type B4;
        Philips5767ReadRegister5Type B5;
    } Bytes;
    WORD Array[5];
};


#endif