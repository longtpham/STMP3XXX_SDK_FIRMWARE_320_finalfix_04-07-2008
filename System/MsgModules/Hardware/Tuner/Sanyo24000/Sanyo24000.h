/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
#ifndef __SANYO24000_H
#define __SANYO24000_H

//-----------------------------------------------------------------------------
// Some type def for Sanyo-LV24000
//-----------------------------------------------------------------------------
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U24;
//WORD is int ;	// 24 bits signed
typedef unsigned long  U48;	// 48 bits!!
typedef signed long    S48;	// 48 bits!!

typedef U8*  PU8;
typedef U16* PU16;
typedef U24* PU24;
typedef U48* PU48;
typedef S48* PS48;

// Function pointer to be used by LinTuneDac
typedef U8 (*FPTR_WRITE_OSC)(U16 wOscValue);	

//-----------------------------------------------------------------------------
// Macro's
//-----------------------------------------------------------------------------
#define LSB(x)				((U8)(x & 0xFF))
#define MSB(x)				((U8)(x >> 8))
#define MAKE_U16(byLo, byHi)( ((U16)byHi<<8) | (byLo) )

#define Hz		1
#define KHz 	1000
#define MHz 	1000000
#define GHz 	1000000000

//-----------------------------------------------------------------------------
// 3-wire GPIO (FM Tuner daughtercard Rev A and the STMP35xx EVK.)
//-----------------------------------------------------------------------------
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
#if defined(DEVICE_3410)||defined(DEVICE_3310)
#define HW_3WIRE_WRITE_ENABLE_PWR  HW_GP2PWR.B.B3
#endif
#define HW_3WIRE_WRITE_ENABLE_DOR  HW_GP2DOR.B.B3
#endif

#define HW_FMTUNER_READY_ENR  HW_GP0ENR.B.B16
#define HW_FMTUNER_READY_DOER HW_GP0DOER.B.B16
#define HW_FMTUNER_READY_PWR  HW_GP0PWR.B.B16
#define HW_FMTUNER_READY_DIR  HW_GP0DIR.B.B16

//-----------------------------------------------------------------------------
// Sanyo LV24-family device ID
//-----------------------------------------------------------------------------
#define LV24000_ID		0x05	// LV24000: FM
#define LV24002_ID		0x04	// LV24002: FM + headphone
#define LV24100_ID		0x07	// LV24100: FM + AM
#define LV24102_ID		0x06	// LV24102: FM + AM + headphone
#define LV24010_ID		0x0B	// LV24010: FM + RDS
#define LV24012_ID		0x0A	// LV24012: FM + RDS + headphone

//-----------------------------------------------------------------------------
// Register LV24000 (V6)
//-----------------------------------------------------------------------------
// Common registers (block address is don't care)
#define BLK_SEL_REG 		0x01			// Address of Block Select register
#define LV24000_DATA_MASK	0xFF			// Mask for 8 bit register data

// IMR01 registers
#define IR01_CHIP_ID_REG		0x0100	// Block 1- Reg00 (R): chip identify
#define IR01_MSRC_SEL_REG		0x0102	// Block 1- Reg02 (W): Measure source select
#define IR01_FM_OSC_REG			0x0103	// Block 1- Reg03 (W): DAC control for FM-RF oscillator
#define IR01_SD_OSC_REG			0x0104	// Block 1- Reg04 (W): DAC control for stereo decoder oscillator
#define IR01_IF_OSC_REG			0x0105	// Block 1- Reg05 (W): DAC control for IF oscillator
#define IR01_CNT_CTRL_REG		0x0106	// Block 1- Reg06 (W): Counter control
#define IR01_NA1_REG			0x0107	// Block 1- Reg07 (-): Reserved for RDS Oscillator
#define IR01_IRQ_MSK_REG		0x0108	// Block 1- Reg08 (W): Interrupt mask
#define IR01_FM_CAP_REG			0x0109	// Block 1- Reg09 (W): CAP bank control for RF-frequency
#define IR01_CNT_L_REG			0x010A	// Block 1- Reg0A (R): Counter value low byte
#define IR01_CNT_H_REG			0x010B	// Block 1- Reg0B (R): Counter value high byte
#define IR01_CTRL_STAT_REG		0x010C	// Block 1- Reg0C (R): Control status
#define IR01_RADIO_STAT_REG		0x010D	// Block 1- Reg0D (R): Radio station status
#define IR01_IRQ_ID_REG			0x010E	// Block 1- Reg0E (R): Interrupt identify
#define IR01_IRQ_OUT_REG		0x010F	// Block 1- Reg0F (W): Set interrupt on the DATA line

#define IR01_RADIO_CTRL1_REG	0x0202	// Block 2- Reg02 (W): Radio control 1
#define IR01_IFCEN_OSC_REG		0x0203	// Block 2- Reg03 (W): IF Center Frequency Oscillator
#define IR01_NA2_REG			0x0204	// Block 2- Reg04 (W): Reserved fro AM Antenna Capacitor
#define IR01_IF_BW_REG			0x0205	// Block 1- Reg05 (W): IF Bandwidth
#define IR01_RADIO_CTRL2_REG	0x0206	// Block 2- Reg06 (W): Radio Control 2
#define IR01_RADIO_CTRL3_REG	0x0207	// Block 2- Reg07 (W): Radio Control 3
#define IR01_STEREO_CTRL_REG	0x0208	// Block 2- Reg08 (W): Stereo Control
#define IR01_AUDIO_CTRL1_REG	0x0209	// Block 2- Reg09 (W): Audio Control 1
#define IR01_AUDIO_CTRL2_REG	0x020A	// Block 2- Reg0A (W): Audio Control 2
#define IR01_PW_SCTRL_REG		0x020B	// Block 2- Reg0B (W): Power and soft control

//////////////////////////////////////////////////////////////////////
// Register layout - Block 1
//////////////////////////////////////////////////////////////////////
// ----- IR01_MSRC_SEL_REG layout
#define	IR1_MSRCS_MSR_OUT	((U8)0x1<<7)	// Bit 7: Output measure source to DATA-pin when 1 - should be 0
#define	IR1_MSRCS_AFC_LVL	((U8)0x1<<6)	// Bit 6: AFC trigger level: 1(20dBuV) 0(0dBuV)
#define IR1_AFCLVL_LO	    ((U8)0<<6)		// Bit 6=0: AFC low trigger level
#define IR1_AFCLVL_HI	    ((U8)1<<6)		// Bit 6=1: AFC high trigger level
#define	IR1_MSRCS_AFC_SPD	((U8)0x1<<5)	// Bit 5: AFC speed: 1(8kHz) 0(3Hz)
#define IR1_AFCSPD_LO	    ((U8)0<<5)		// Bit 5=0: AFC low speed
#define IR1_AFCSPD_HI	    ((U8)1<<5)		// Bit 5=1: AFC high speed
											// Bit 3-4: Reserved
#define	IR1_MSRCS_MSS_SD	((U8)0x1<<2)	// Bit 2: Enable stereo decoder oscillator measurement when 1
#define	IR1_MSRCS_MSS_FM	((U8)0x1<<1)	// Bit 1: Enable FM RF oscillator measurement when 1
#define	IR1_MSRCS_MSS_IF	((U8)0x1<<0)	// Bit 0: Enable IF oscillator measurement when 1

// Measured source mask
#define IR1_MSRCS_MSS_MASK	(IR1_MSRCS_MSS_FM|IR1_MSRCS_MSS_SD|IR1_MSRCS_MSS_IF)

// ----- IR01_FM_OSC_REG layout
#define FMOSC_VALUE_MASK	LV24000_DATA_MASK
#define IR01_MAX_HW_RFOSC	LV24000_DATA_MASK	// Maximal value for RF osc. register
#define IR01_MIN_HW_RFOSC	0					// Minimal value for RF osc. register

// ----- IR01_SD_OSC_REG layout
#define SDOSC_VALUE_MASK	LV24000_DATA_MASK

// ----- IR01_IF_OSC_REG layout
#define IFOSC_VALUE_MASK	LV24000_DATA_MASK

// ----- IR01_CNT_CTRL_REG layout
#define	IR1_CCTL_CNT1_CLR	((U8)0x1<<7)	// Bit 7: Clear and keep counter 1 in reset mode when 1
#define	IR1_CCTL_CNT_TAB	((U8)7<<4)	    // Bit [6:4]: tab select for counter 2 measuring interval
#define IR1_CTAB_2          ((U8)0<<4)		// Stop after 2 counts
#define IR1_CTAB_8          ((U8)1<<4)		// Stop after 8 counts 
#define IR1_CTAB_32         ((U8)2<<4)		// Stop after 32 counts 
#define IR1_CTAB_128        ((U8)3<<4)		// Stop after 128 counts
#define IR1_CTAB_512        ((U8)4<<4)		// Stop after 512 counts
#define IR1_CTAB_2048       ((U8)5<<4)		// Stop after 2048 counts
#define IR1_CTAB_8192       ((U8)6<<4)		// Stop after 8192 counts
#define IR1_CTAB_32768      ((U8)7<<4)		// Stop after 327682 counts
#define	IR1_CCTL_SWP_CNT_L	((U8)0x1<<3)	// Bit 3: Swap clock source 1 to counter 2, clock source 2 to counter 1 when 1
#define	IR1_CCTL_CNT_EN		((U8)0x1<<2)	// Bit 2: Enable counter (counting mode) when 1
#define	IR1_CCTL_CNT_SEL	((U8)0x1<<1)	// Bit 1: Select counter 2 when 1 (counter 1 when 0)
#define	IR1_CCTL_CNT_SET	((U8)0x1<<0)	// Bit 0: Set both counter 1 and counter 2 to FFFFh and keep them set when 1

// ----- IR01_IRQ_MSK_REG layout
												// Bit 7: Reserved
#define	IR1_IRQM_EN_MS			((U8)0x1<<6)	// Bit 6: Enable mono/stereo change interrupt when 1
												// Bit 5-4: Reserved
#define	IR1_IRQM_ACT_LOW		((U8)0x1<<3)	// Bit 3 is 1: IRQ active low
#define	IR1_IRQM_ACT_HIGH	    ((U8)0x0<<3)	// Bit 3 is 0: IRQ active high
#define	IR1_IRQM_EN_AFC			((U8)0x1<<2)	// Bit 2: Enable AFC out of range interrupt whne 1
#define	IR1_IRQM_EN_FS			((U8)0x1<<1)	// Bit 1: Enable field strength change interrupt when 1
#define	IR1_IRQM_EN_CNT2		((U8)0x1<<0)	// Bit 0: Enable counter 2 counting done interrupt when 1

// ----- IR01_FM_CAP_REG layout
#define FMCAP_VALUE_MASK		LV24000_DATA_MASK

// ----- IR01_CTRL_STAT_REG layout
												// Bit [7:1]: Reserved
#define	IR1_CSTAT_AFC_FLAG		((U8)0x1<<0)	// Bit 0: AFC is out of control range when 1

// ----- IR01_RADIO_STAT_REG layout
#define	IR1_RSTAT_STEREO		((U8)0x1<<7)	// Bit 7: Radio station is stereo when 1 (0=mono)
#define	IR1_RSTAT_FS			((U8)0x7F<<0)	// Bit [6:0]: Radio station field strength bits

// ----- IR01_IRQ_ID_REG layout
												// Bit [7:6]: Reserved (read as 1)
#define	IR1_IRQID_CNT2			((U8)0x1<<5)	// Bit 5: counter 2 counting done interrupt when 1
												// Bit 4: Reserved (read as 0)
#define	IR1_IRQID_AFC			((U8)0x1<<3)	// Bit 3: AFC interrupt when 1
												// Bit 2: Reserved (read as 0)
												// Bit 1: Reserved (read as 0)
#define	IR1_IRQID_FS_MS			((U8)0x1<<0)	// Bit 0: Field strength/Mono-Stereo changed interrupt when 1

//////////////////////////////////////////////////////////////////////
// Register layout - Block 2
//////////////////////////////////////////////////////////////////////
// ----- IR01_RADIO_CTRL1_REG layout
#define	IR1_RCTL1_EN_MEAS		((U8)0x1<<7)	// Bit 7: Measurement mode if 1
#define	IR1_RCTL1_EN_AFC		((U8)0x1<<6)	// Bit 6: Enable AFC if 1
												// Bit 5: Reserved (write with 0)
#define	IR1_RCTL1_NA_H_4		((U8)0x1<<4)	// Bit 4: Reserved - must be written with 1
#define	IR1_RCTL1_DIR_AFC		((U8)0x1<<3)	// Bit 3: reverse AFC direction if 1
#define	IR1_RCTL1_RST_AFC		((U8)0x1<<2)	// Bit 2: Reset AFC to the middle of the control range if 1
#define	IR1_RCTL1_NA_H_1		((U8)0x1<<1)	// Bit 1: Reserved - must be written with 1
#define	IR1_RCTL1_NA_H_0		((U8)0x1<<0)	// Bit 0: Reserved - must be written with 1

// ----- IR01_RADIO_CTRL2_REG layout
#define	IR1_RCTL2_VREF2_L		((U8)0x1<<7)	// Bit 7: VREF2 control (0=ON)
#define	IR1_RCTL2_VREF_L		((U8)0x1<<6)	// Bit 6: VREF control (0=ON)
#define	IR1_RCTL2_BYPASS_STABI	((U8)0x1<<5)	// Bit 5: Bypass the stabi circuit (should be 0: not bypassed)
#define	IR1_RCTL2_IF_PM_L		((U8)0x1<<4)	// Bit 4: IF PLL mute - active low
#define	IR1_RCTL2_IF_PM_ON	    ((U8)0x0<<4)	// bit 4=0: IF PLL mute on (presetting IF mode)
#define	IR1_RCTL2_IF_PM_OFF	    ((U8)0x1<<4)	// bit 4=1: IF PLL mute off (normal operating mode)
#define IR1_RCTL2_DCFB_SPEED	((U8)0x1<<3)	// Bit 3: DC feedback speed
#define IR1_RCTL2_DCFB_OFF		((U8)0x1<<2)	// Bit 2: Turn off DC feedback if 1
#define	IR1_RCTL2_AGC_SPD		((U8)0x1<<1)	// Bit 1: AGC high speed if 1
												// Bit 0: Reserved

// ----- IR01_RADIO_CTRL3_REG layout
#define	IR1_RCTL3_AGC_SETLVL	((U8)0x1<<7)	// Bit 7: AGC set level (=1 for FM)
#define	IR1_RCTL3_VOL_SHIFT		((U8)0x1<<6)	// Bit 6: Shift volume extra 12dB if 1
#define	IR1_RCTL3_TONE_MUTE_L	((U8)0x1<<5)	// Bit 5: Tone mute control (0=mute)
#define	IR1_RCTL3_AUDIO_MUTE_L	((U8)0x1<<4)	// Bit 4: Audio mute if 0
#define	IR1_RCTL3_SE_FM			((U8)0x1<<3)	// Bit 3: Source enable: FM
												// Bit 2: Reserved
#define	IR1_RCTL3_SE_BEEP		((U8)0x1<<1)	// Bit 1: Source enable: Beep
#define	IR1_RCTL3_SE_EXT		((U8)0x1<<0)	// Bit 0: Source enable: external

// ----- IR01_STEREO_CTRL_REG layout
#define	IR1_STCTL_FRC_STEREO	((U8)0x1<<7)	// Bit 7: Force stereo if 1
#define	IR1_STCTL_FMCS			((U8)7<<4)	    // Bit 6-4: FM channel separation level
#define	IR1_STCTL_AUTO_SLEWRATE	((U8)0x1<<3)	// Bit 3: Stereo auto slew rate
#define	IR1V6_CANCEL_PILOT		((U8)0x1<<2)	// Bit 2: Pilot cancellation
#define	IR1_STCTL_SD_PM			((U8)0x1<<1)	// Bit 1: Mute stereo decoder PLL if 1 (presetting mode)
#define	IR1_STCTL_STEREO_L		((U8)0x1<<0)	// Bit 0: Enable stereo if 0

// ----- IR01_AUDIO_CTRL1_REG layout
#define	IR1_ACTL1_TONE_LVL		((U8)0xF<<4)	// Bit [7:4]: Tone level
#define	IR1_ACTL1_VOL_LVL		((U8)0xF<<0)	// Bit [3:0]: Volume level

// ----- IR01_AUDIO_CTRL2_REG layout
#define	IR1_ACTL2_BPFREQ		((U8)0x3<<6)	// Bit [7:6]: Beep frequency bits
#define	IR1_BEEP_2KHz	        ((U8)0<<6)		// 2 kHz beep tone
#define	IR1_BEEP_1KHz	        ((U8)1<<6)		// 1 kHz beep tone
#define	IR1_BEEP_500Hz          ((U8)2<<6)		// 0.5 kHz beep tone
#define	IR1_BEEP_HIGH	        ((U8)3<<6)		// Beep output high
#define	IR1_BEEP_OFF	        IR1_BEEP_HIGH	// Beep off (drive output high)
#define	IR1_ACTL2_DEEMP75		((U8)0x1<<5)	// Bit 5=1: De-emphasis 75
#define	IR1_ACTL2_DEEMP50	    ((U8)0<<5)		// Bit 5=0: De-emphasis 50
#define	IR1_ACTL2_TREB_N		((U8)0x1<<4)	// Bit 4: Negative treble if 1
#define	IR1_ACTL2_TREB_P		((U8)0x1<<3)	// Bit 3: Positive treble if 1
#define	IR1_ACTL2_BASS_N		((U8)0x1<<2)	// Bit 2: Negative bass if 1
#define	IR1_ACTL2_BASS_P		((U8)0x1<<1)	// Bit 1: Positive bass if 1
#define	IR1_ACTL2_BASS_LVL		((U8)0x1<<0)	// Bit 0: Bass level

#define	IR1_ATREBLE_MASK	(IR1_ACTL2_TREB_N | IR1_ACTL2_TREB_P)
#define	IR1_ABASS_MASK		(IR1_ACTL2_BASS_N | IR1_ACTL2_BASS_P | IR1_ACTL2_BASS_LVL)
/*
	Treble levels
	-------------
	Treble_N (Bit4) - Treble_P (Bit 3) - Level - Value
	1					0					-1		2
	0					0					0		0
	0					1					+1		1

	Bass levels
	-------------
	Bass_N (Bit2) - Bass_P (Bit 1) - Bass_Lvl (bit 0) - Level - Value
	1					0				1				-2		5
	1					0				0				-1		4
	0					0				0				0		0
	0					1				0				+1		2
	0					1				1				+2		3

  When treble level != 0 OR bass level != 0
	IR1_RCTL3_TONE_MUTE_L bit must be high (unmute tone control)
*/

// ----- IR01_PW_SCTRL_REG layout
#define	IR1_PSCTL_SOFT_ST	((U8)0x7<<5)	// Bit [7:5]: Soft stereo control
#define	IR1_PSCTL_SOFT_MUTE	((U8)0x7<<2)	// Bit [4:2]: Soft mute control
#define	IR1_PSCTL_NA1		((U8)0x1<<1)	// Bit 1: NA for IR1
#define	IR2_PSCTL_HPA		((U8)0x1<<1)	// Bit 1: Headphone power on (LV24xx2 only)
#define	IR1_PSCTL_PW_RAD	((U8)0x1<<0)	// Bit 0: Radio circuitry power on if 1

//////////////////////////////////////////////////////////////////////
// Frequency divider factor
#define IMR01_FM_DIVIDER		256			// FM frequency is divided before measuring
#define IMR01_CNT2_PRESCALER	2			// Counter 2 prescaler

//////////////////////////////////////////////////////////////////////
// Radio default frequencies
#define DEFAULT_IF_FREQ			((U24)110000)	// Default IF value (in Hz)
#define DEFAULT_SD_FREQ			((U24)38000)	// Default SD value (in Hz)

//////////////////////////////////////////////////////////////////////
// Equation for quick scan data
#define QSS_MIN_RFCAP	0
#define QSS_MAX_RFCAP	191

#define QSS_MIN_RFOSC	5
#define QSS_MAX_RFOSC	240

//-----------------------------------------------------------------------------
// Radio definitions
//-----------------------------------------------------------------------------
// De-emphasis value
//#define DEEMP_75 	1
//#define DEEMP_50 	0

// FM Regions
//#define REGION_NONE			0	// None: use hardware limit
//#define REGION_EUROPE			1	// FM Europe (87.5MHz - 108 MHz, de-emp=50)
//#define REGION_JAPAN			2	// FM Japan  (76MHz - 90MHz, de-emp=50)
//#define REGION_US				3	// FM US (same as Europe, De-emp=75)
//#define REGION_JAPAN_WIDE		4	// FM Japan wide: Same as Japan, with extra CH1(95.75) CH2(101.75) CH3(107.75)

//-----------------------------------------------------------------------------
// Time constances used (value in us for Pulse3wNRW)
//-----------------------------------------------------------------------------
#define LV_MSR_TIME_4ms			4000	// us
#define LV_MSR_TIME_6ms			6000	// us
#define LV_MSR_TIME_8ms			8000	// us
#define LV_MSR_TIME_16ms		16000	// us
#define LV_MSR_TIME_20ms		20000	// us
#define LV_MSR_TIME_24ms		24000	// us
#define LV_MSR_TIME_32ms		32000	// us
#define LV_MSR_TIME_64ms		64000	// us
#define LV_MSR_TIME_100ms		100000	// us

//-----------------------------------------------------------------------------
// Time constances for LvDelay 
//-----------------------------------------------------------------------------
#define DLT_1ms	1	//ms
#define DLT_5ms	5	//ms
#define DLT_8ms	8	//ms

//-----------------------------------------------------------------------------
// Constances used by SetChipOutput
//-----------------------------------------------------------------------------
#define CO_NONE			0					// 0: nothing at output - idle state
#define	CO_RF_OSC		IR1_MSRCS_MSS_FM	// FREQ1: RF frequency
#define	CO_IF_OSC		IR1_MSRCS_MSS_IF	// FREQ2: IF Osccilator output
#define	CO_SD_OSC		IR1_MSRCS_MSS_SD	// FREQ3: Stereo decoder clock output 
#define CO_IF_FREE		0xCC				// IF in free mode - software defined - Any value except above values

//-----------------------------------------------------------------------------
// Definitions for function's parameters
//-----------------------------------------------------------------------------
// Scan station parameters
#define SCANSTN_NEXT	((U8)1<<0)	// Bit 0: Up scan if 1
#define SCANSTN_PREV	((U8)1<<1)	// Bit 1: Down scan if 1
#define SCANSTN_AUTO 	((U8)1<<2)	// Bit 2: Auto scan if 1, scan next/previous if 0

//-----------------------------------------------------------------------------
// Bit definitions for global variables
//-----------------------------------------------------------------------------
// Bit definitions of g_byStnFlag
#define STN_VALID				((BYTE)1<<7)	// Bit 7: Valid station mark
#define STN_FREQ_CHG			((BYTE)1<<6)	// Bit 6: Station frequency changed

// Following bits are only needed when USE_SCANNING_DISPLAY is defined (keep the same as ScanStation parameter)
#define STN_USCAN				SCANSTN_NEXT	// Bit 0: Up scanning in progress flag
#define STN_DSCAN				SCANSTN_PREV	// Bit 1: Down scanning in progress flag
#define STN_ASCAN				SCANSTN_AUTO	// Bit 2: Auto scanning in progress flag
#define STN_SCAN_IN_PROGRESS	(STN_USCAN|STN_DSCAN|STN_ASCAN)	// Scanning in progress mark

//-----------------------------------------------------------------------------
// LV24xxx internal error codes
//-----------------------------------------------------------------------------
#define LVLS_NO_ERROR					0		// no error
#define LVLS_CHIP_BUSY_ERR			    0x07	// Chip busy
#define LVLS_TUNE_OSC_ERR				0x20	// Tuning Osc-register failed
#define LVLS_TUNEOSC_LOWFREQ_ERR		0x21	// Tuning Osc-register failed (HW Frequency too low)
#define LVLS_TUNEOSC_HIGHFREQ_ERR		0x22	// Tuning Osc-register failed (HW Frequency too high)
#define LVLS_RFOSC_OUTOFLIMIT_ERR		0x23	// An invalid value is sent to RF Osc register
#define LVLS_RFCAP_OUTOFLIMIT_ERR		0x24	// An invalid value is sent to RF CAP register
#define LVLS_CAPOSC_CALC_ERR			0x28	// Unable to calculate CAP/OSC value
#define LVLS_POLLING_CNT_TIMEOUT		0x29	// Polling counting done timeout
#define LVLS_UNREACHABLE_FREQ_ERR		0x33	// Requested frequency exceeds hardware limit
#define LVLS_NOT_STATION_ERR			0x34	// Current frequency is not a valid station
#define LVLS_NO_STATION_FOUND_ERR		0x35	// No station found after searching the whole band
#define LVLS_SCAN_UPDATE_ERR			0x36	// Pause scanning for update

#endif //#ifndef __SANYO24000_H 
