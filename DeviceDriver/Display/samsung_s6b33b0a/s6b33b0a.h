#ifndef S6B33B0A_H
#define S6B33B0A_H

#include "regsgpio.h"
#include "regspwm.h"

#ifndef LCD_RESET_GPDO
#define LCD_RESET_GPDO      HW_GP1DOR.B.B20
#define LCD_RESET_GPDOE     HW_GP1DOER.B.B20
#define LCD_RESET_GPEN      HW_GP1ENR.B.B20
#define LCD_RESET_GPPWR     HW_GP1PWR.B.B20
#endif 

//for now WR will be in a Write Only state  no reading will be implemented intially.  This will require an additional GPIO.


#ifndef LCD_RD_GPD0
#define LCD_RD_GPDO      	HW_GP0DOR.B.B12	  
#define LCD_RD_GPDI      	HW_GP0DIR.B.B12	  
#define LCD_RD_GPDOE     	HW_GP0DOER.B.B12  
#define LCD_RD_GPEN      	HW_GP0ENR.B.B12	  
#define LCD_RD_GPPWR     	HW_GP0PWR.B.B12	  
#endif

#ifndef LCD_WR_GPD0
#ifdef CLCD_8BIT
#define LCD_WR_GPDO        	HW_GP0DOR.B.B10
#define LCD_WR_GPDI        	HW_GP0DIR.B.B10
#define LCD_WR_GPDOE       	HW_GP0DOER.B.B10
#define LCD_WR_GPEN        	HW_GP0ENR.B.B10
#define LCD_WR_GPPWR       	HW_GP0PWR.B.B10
#else //#ifdef CLCD_8BIT
#define LCD_WR_GPDO        	HW_GP1DOR.B.B12
#define LCD_WR_GPDI        	HW_GP1DIR.B.B12
#define LCD_WR_GPDOE       	HW_GP1DOER.B.B12
#define LCD_WR_GPEN        	HW_GP1ENR.B.B12
#define LCD_WR_GPPWR       	HW_GP1PWR.B.B12
#endif //#ifdef CLCD_8BIT
#endif

#ifndef LCD_CS_GPDO         
#ifdef CLCD_8BIT
#define LCD_CS_GPDO         HW_GP0DOR.B.B8
#define LCD_CS_GPDI         HW_GP0DIR.B.B8
#define LCD_CS_GPDOE        HW_GP0DOER.B.B8
#define LCD_CS_GPEN         HW_GP0ENR.B.B8
#define LCD_CS_GPPWR        HW_GP0PWR.B.B8
#else //#ifdef CLCD_8BIT
#define LCD_CS_GPDO         HW_GP1DOR.B.B8
#define LCD_CS_GPDI         HW_GP1DIR.B.B8
#define LCD_CS_GPDOE        HW_GP1DOER.B.B8
#define LCD_CS_GPEN         HW_GP1ENR.B.B8
#define LCD_CS_GPPWR        HW_GP1PWR.B.B8
#endif //#ifdef CLCD_8BIT
#endif

#ifndef LCD_DC_GPDO
#ifdef CLCD_8BIT
#define LCD_DC_GPDO         HW_GP0DOR.B.B11
#define LCD_DC_GPDI         HW_GP0DIR.B.B11
#define LCD_DC_GPDOE        HW_GP0DOER.B.B11
#define LCD_DC_GPPWR        HW_GP0PWR.B.B11
#define LCD_DC_GPEN         HW_GP0ENR.B.B11
#else //#ifdef CLCD_8BIT
#define LCD_DC_GPDO         HW_GP2DOR.B.B4
#define LCD_DC_GPDI         HW_GP2DIR.B.B4
#define LCD_DC_GPDOE        HW_GP2DOER.B.B4
#define LCD_DC_GPPWR        HW_GP2PWR.B.B4
#define LCD_DC_GPEN         HW_GP2ENR.B.B4
#endif //#ifdef CLCD_8BIT
#endif

// LCD_D0   GPIO_19
#ifndef LCD_DATA_D00 
#define LCD_DATA_D00           HW_GP0DOR.B.B19
#define LCD_DATA_D00_DI        HW_GP0DIR.B.B19
#define LCD_DATA_D00_GPDOE     HW_GP0DOER.B.B19
#define LCD_DATA_D00_GPEN      HW_GP0ENR.B.B19
#define LCD_DATA_D00_GPPWR     HW_GP0PWR.B.B19
#endif

// LCD_D3   GPIO_33
#ifndef LCD_DATA_D03 
#define LCD_DATA_D03           HW_GP1DOR.B.B9
#define LCD_DATA_D03_DI        HW_GP1DIR.B.B9
#define LCD_DATA_D03_GPDOE     HW_GP1DOER.B.B9
#define LCD_DATA_D03_GPEN      HW_GP1ENR.B.B9
#define LCD_DATA_D03_GPPWR     HW_GP1PWR.B.B9
#endif

// LCD_D4   GPIO_51
#ifndef LCD_DATA_D04 
#define LCD_DATA_D04           HW_GP2DOR.B.B3
#define LCD_DATA_D04_DI        HW_GP2DIR.B.B3
#define LCD_DATA_D04_GPDOE     HW_GP2DOER.B.B3
#define LCD_DATA_D04_GPEN      HW_GP2ENR.B.B3
#define LCD_DATA_D04_GPPWR     HW_GP2PWR.B.B3
#endif

//PWM0
#ifndef LCD_BLON_GPDO
#ifdef CLCD_8BIT
#define LCD_BLON_GPDO         HW_GP0DOR.B.B9
#define LCD_BLON_GPDI         HW_GP0DIR.B.B9
#define LCD_BLON_GPDOE        HW_GP0DOER.B.B9
#define LCD_BLON_GPPWR        HW_GP0PWR.B.B9
#define LCD_BLON_GPEN         HW_GP0ENR.B.B9
#else //#ifdef CLCD_8BIT
#define LCD_BLON_GPDO         HW_GP0DOR.B.B18
#define LCD_BLON_GPDI         HW_GP0DIR.B.B18
#define LCD_BLON_GPDOE        HW_GP0DOER.B.B18
#define LCD_BLON_GPPWR        HW_GP0PWR.B.B18
#define LCD_BLON_GPEN         HW_GP0ENR.B.B18
#endif //#ifdef CLCD_8BIT
#endif

#ifdef CLCD_16BIT
#ifndef CLCD_16BIT_SWITCH
#define CLCD_16BIT_SWITCH_16BIT_ON  FALSE               // depends on PNP or NPN
#define CLCD_16BIT_SWITCH_16BIT_OFF TRUE                // depends on PNP or NPN
#define CLCD_16BIT_SWITCH           HW_GP1DOR.B.B13
#define CLCD_16BIT_SWITCH_GPDI      HW_GP1DIR.B.B13
#define CLCD_16BIT_SWITCH_GPDOE     HW_GP1DOER.B.B13
#define CLCD_16BIT_SWITCH_GPEN      HW_GP1ENR.B.B13
#define CLCD_16BIT_SWITCH_GPPWR     HW_GP1PWR.B.B13
#endif
#endif

#define ACK_AND_XTAL_ENABLE_MASK 0x040009
#define PWM_ENABLE_MASK	0x800000

//#define PWM0_SETUPA 0x024000
//#define PWM0_SETUPAOFF  0x024000
#define PWM0_SETUPA 0x06C000
#define PWM0_SETUPAOFF  0x024000
#define PWM0_SETUPB	0x00E090
#define PWM0_SETUPB_SLOW_PWM	0x00E2D0
#define PWM0_ENABLE_MASK 0x800001
#define PWM0_DISABLE_MASK 0xFFFFFE

#define PWM2_SETUPA 0x006000
#define PWM2_SETUPAOFF  0x002000
#define PWM2_SETUPB	0x00E008    //divide by 10 XTALS to get ~150 kHz
#define PWM2_SETUPB_SLOW_PWM	0x00E028 //divide by 50 XTALS to get ~30 kHz
#define PWM2_ENABLE_MASK 0x800004   //divide XTAL by 16 for 1.5 MHz
#define PWM2_DISABLE_MASK 0xFFFFFB

//change the below #define statments to switch bewteen PWM0 and PWM2
//currently setup to use PWM2

#ifdef CLCD_8BIT
#define CLCD_PWM_ENABLE_MASK PWM2_ENABLE_MASK
#define CLCD_PWM_DISABLE_MASK PWM2_DISABLE_MASK
#define PWMA HW_PWM_CH2AR.I
#define PWMB HW_PWM_CH2BR.I
#define PWM_SETUPA PWM2_SETUPA
#define PWM_SETUPAOFF PWM2_SETUPAOFF
#define PWM_SETUPB PWM2_SETUPB
#define PWM_SETUPB_SLOW_PWM PWM2_SETUPB_SLOW_PWM
#define PWM_ENABLE_MASK PWM2_ENABLE_MASK
#else //#ifdef CLCD_8BIT
#define CLCD_PWM_ENABLE_MASK PWM0_ENABLE_MASK
#define CLCD_PWM_DISABLE_MASK PWM0_DISABLE_MASK
#define PWMA HW_PWM_CH0AR.I
#define PWMB HW_PWM_CH0BR.I
#define PWM_SETUPA PWM0_SETUPA
#define PWM_SETUPAOFF PWM0_SETUPAOFF
#define PWM_SETUPB PWM0_SETUPB
#define PWM_SETUPB_SLOW_PWM PWM0_SETUPB_SLOW_PWM
#define PWM_ENABLE_MASK PWM0_ENABLE_MASK
#endif //#ifdef CLCD_8BIT
//these were determined based on the range that is visible

#ifndef LCD_MIN_CONTRAST
#define LCD_MIN_CONTRAST 160
#endif

#ifndef LCD_MAX_CONTRAST
#define LCD_MAX_CONTRAST  210
#endif

#ifndef LCD_STEPSIZE_CONTRAST
#define LCD_STEPSIZE_CONTRAST 1
#endif

#ifndef DEFAULT_CONTRAST
#define DEFAULT_CONTRAST 50
#endif


#ifdef CLCD_8BIT
typedef union               /* GPIO Pin Register Bank 0 */
{
    struct {
        WORD Data  :8;
        WORD Reserved  :16;
    } B;
    int I;
} LCDgpr_type;
#else //#ifdef CLCD_8BIT
typedef union               /* GPIO Pin Register Bank 0 */
{
    struct {
        WORD Data  :16;
        WORD Reserved  :8;
    } B;
    int I;
} LCDgpr_type;
#endif //#ifdef CLCD_8BIT

#define CMD_SET_LOWER_COLUMN_ADDRESS(x)     (x&0xf)
#define CMD_SET_HIGHER_COLUMN_ADDRESS(x)    0x10|((x>>4)&0xf)
#define CMD_SET_PAGE_ADDRESS(x)             0xb0|(x&0xf)
#define CMD_SET_CONTRAST                    0x2A

#ifndef LCD_DATA_DO
#define LCD_DATA_DO      (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOR)).B.Data
#define LCD_DATA_DI      (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDIR)).B.Data
#define LCD_DATA_DOE     (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOER)).B.Data
#define LCD_DATA_EN      (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPENR)).B.Data
#define LCD_DATA_PWR     (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPPWR)).B.Data
#define LCD_DATA_DIR	 (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDIR)).B.Data
#endif

#define _MultiNop() _nop();_nop()

extern INT  g_iContrastPercentage;  // default contrast 50% if settings.dat is not present

_reentrant WORD HALDisplayReadStatus(void);
#endif