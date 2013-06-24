#ifndef __PHILIPS5757_H
#define __PHILIPS5757_H


#define PHILIPS_3WIRE_DELAY 41  // Thi & Tlow = 1.67usec.

#ifndef HW_3WIRE_CLOCK_ENR
#define HW_3WIRE_CLOCK_ENR  HW_GP0ENR.B.B18
#define HW_3WIRE_CLOCK_DOER HW_GP0DOER.B.B18
#define HW_3WIRE_CLOCK_PWR  HW_GP0PWR.B.B18
#define HW_3WIRE_CLOCK_DOR  HW_GP0DOR.B.B18
#endif

#ifndef HW_3WIRE_DATA_ENR
#define HW_3WIRE_DATA_ENR  HW_GP0ENR.B.B19
#define HW_3WIRE_DATA_DOER HW_GP0DOER.B.B19
#define HW_3WIRE_DATA_DIR  HW_GP0DIR.B.B19
#define HW_3WIRE_DATA_PWR  HW_GP0PWR.B.B19
#define HW_3WIRE_DATA_DOR  HW_GP0DOR.B.B19
#endif
/* Note: FM_WE is on GP51 for SDK */
#ifndef HW_3WIRE_WRITE_ENABLE_ENR
#define HW_3WIRE_WRITE_ENABLE_ENR  HW_GP2ENR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOER HW_GP2DOER.B.B3
#define HW_3WIRE_WRITE_ENABLE_PWR  HW_GP2PWR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOR  HW_GP2DOR.B.B3
#endif

/* FM Tuner mono/stereo when BUS-CLOCK is low and NOT Tuned/Tuned when BUS-CLOCK is high */
#ifndef HW_3WIRE_MONO_UNTUNED_ENR
#define HW_3WIRE_MONO_UNTUNED_ENR  HW_GP1ENR.B.B15
#define HW_3WIRE_MONO_UNTUNED_DOER HW_GP1DOER.B.B15
#define HW_3WIRE_MONO_UNTUNED_PWR  HW_GP1PWR.B.B15
#define HW_3WIRE_MONO_UNTUNED_DIR  HW_GP1DIR.B.B15
#endif

/*  FM Tuner's power switch control bit */	 
#ifndef HW_3WIRE_FM_POWER_DOWN_ENR
#define HW_3WIRE_FM_POWER_DOWN_ENR  HW_GP0ENR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DOER HW_GP0DOER.B.B16
#define HW_3WIRE_FM_POWER_DOWN_PWR  HW_GP0PWR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DOR  HW_GP0DOR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DIR  HW_GP0DIR.B.B16
#endif


#define FMIF_FREQUENCY 10700


typedef union
{
    struct {
        unsigned int m_uPLL0_14:        15;
        unsigned int m_Dummy:           1;
        unsigned int m_uSearchLevel:    2;
        unsigned int m_uPort:           2;
        unsigned int m_uBand:           2;
        unsigned int m_bMonoStereo:     1;
        unsigned int m_bSearchUp:       1;
    } B;
    WORD I;
} Philips5757ShiftRegisterType;



#endif