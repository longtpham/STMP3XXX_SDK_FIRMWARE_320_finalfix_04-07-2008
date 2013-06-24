#ifndef SED15xx_H
#define SED15xx_H

//DanhNguyen reviews: For X8iTF OLED LCD

//USBSUSPEND GPIO37
#ifndef USB_SUSPEND_GPD0
#define USB_SUSPEND_GPDO      HW_GP1DOR.B.B13
#define USB_SUSPEND_GPDOE     HW_GP1DOER.B.B13
#define USB_SUSPEND_GPEN      HW_GP1ENR.B.B13
#define USB_SUSPEND_GPPWR     HW_GP1PWR.B.B13
#endif 

//GPIO42 control PWRON pin of ltc345
#ifndef PWR_ON_GPD0
#define PWR_ON_GPDO      HW_GP1DOR.B.B18
#define PWR_ON_GPDOE     HW_GP1DOER.B.B18
#define PWR_ON_GPEN      HW_GP1ENR.B.B18
#define PWR_ON_GPPWR     HW_GP1PWR.B.B18
#endif 

#ifndef LCD_BL_GPDO
#define LCD_BL_GPDO         HW_GP1DOER.B.B15
#define LCD_BL_GPDOE        HW_GP1DOER.B.B15
#define LCD_BL_GPPWR        HW_GP1PWR.B.B15
#define LCD_BL_GPEN         HW_GP1ENR.B.B15
#endif	//LCD_command/data_select

//GPIO39 turn on off VCC oled
#ifndef LCD_VCC_GPDO
#define LCD_VCC_GPDO      HW_GP1DOR.B.B15
#define LCD_VCC_GPDOE     HW_GP1DOER.B.B15
#define LCD_VCC_GPEN      HW_GP1ENR.B.B15
#define LCD_VCC_GPPWR     HW_GP1PWR.B.B15
#endif

#ifndef LCD_RESET_GPDO
#define LCD_RESET_GPDO      HW_GP1DOR.B.B20
#define LCD_RESET_GPDOE     HW_GP1DOER.B.B20
#define LCD_RESET_GPEN      HW_GP1ENR.B.B20
#define LCD_RESET_GPPWR     HW_GP1PWR.B.B20
#endif 

#ifndef LCD_RDE_GPD0
#define LCD_RDE_GPDO        HW_GP0DOR.B.B11
#define LCD_RDE_GPDOE       HW_GP0DOER.B.B11
#define LCD_RDE_GPEN        HW_GP0ENR.B.B11
#define LCD_RDE_GPPWR       HW_GP0PWR.B.B11
#endif


#ifndef LCD_CS1_GPDO         
#define LCD_CS1_GPDO         HW_GP0DOR.B.B8
#define LCD_CS1_GPDOE        HW_GP0DOER.B.B8
#define LCD_CS1_GPEN         HW_GP0ENR.B.B8
#define LCD_CS1_GPPWR        HW_GP0PWR.B.B8
#endif

#ifndef LCD_RW_GPDO
#define LCD_RW_GPDO         HW_GP0DOER.B.B9
#define LCD_RW_GPDOE        HW_GP0DOER.B.B9
#define LCD_RW_GPPWR        HW_GP0PWR.B.B9
#define LCD_RW_GPEN         HW_GP0ENR.B.B9
#endif


#ifndef LCD_DC_GPDO
#define LCD_DC_GPDO         HW_GP0DOR.B.B10
#define LCD_DC_GPDOE        HW_GP0DOER.B.B10
#define LCD_DC_GPPWR        HW_GP0PWR.B.B10
#define LCD_DC_GPEN         HW_GP0ENR.B.B10
#endif

// LCD_D0   GPIO_19
#ifndef LCD_DATA_D00 
#define LCD_DATA_D00           HW_GP0DOR.B.B19
#define LCD_DATA_D00_GPDOE     HW_GP0DOER.B.B19
#define LCD_DATA_D00_GPEN      HW_GP0ENR.B.B19
#define LCD_DATA_D00_GPPWR     HW_GP0PWR.B.B19
#endif

// LCD_D3   GPIO_33
#ifndef LCD_DATA_D03 
#define LCD_DATA_D03           HW_GP1DOR.B.B9
#define LCD_DATA_D03_GPDOE     HW_GP1DOER.B.B9
#define LCD_DATA_D03_GPEN      HW_GP1ENR.B.B9
#define LCD_DATA_D03_GPPWR     HW_GP1PWR.B.B9
#endif

// LCD_D4   GPIO_51
#ifndef LCD_DATA_D04 
#define LCD_DATA_D04           HW_GP2DOR.B.B3
#define LCD_DATA_D04_GPDOE     HW_GP2DOER.B.B3
#define LCD_DATA_D04_GPEN      HW_GP2ENR.B.B3
#define LCD_DATA_D04_GPPWR     HW_GP2PWR.B.B3
#endif


#ifndef LCD_MIN_CONTRAST
#define LCD_MIN_CONTRAST 0
#endif

#ifndef LCD_MAX_CONTRAST
#define LCD_MAX_CONTRAST  250
#endif

#ifndef LCD_STEPSIZE_CONTRAST
#define LCD_STEPSIZE_CONTRAST 20
#endif

#ifndef DEFAULT_CONTRAST
#define DEFAULT_CONTRAST 80
#endif


typedef union               /* GPIO Pin Register Bank 0 */
{
    struct {
        WORD Data  :8;
        WORD Reserved  :16;
    } B;
    int I;
} LCDgpr_type;

#define CMD_SET_LOWER_COLUMN_ADDRESS(x)     (x&0xf)
#define CMD_SET_HIGHER_COLUMN_ADDRESS(x)    0x10|((x>>4)&0xf)
#define CMD_SET_PAGE_ADDRESS(x)             0xb0|(x&0xf)
#define CMD_SET_CONTRAST                    0x81

#ifndef LCD_DATA_DO
#define LCD_DATA_DO      (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOR)).B.Data
#define LCD_DATA_DOE     (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOER)).B.Data
#define LCD_DATA_EN      (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPENR)).B.Data
#define LCD_DATA_PWR     (*(volatile LCDgpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPPWR)).B.Data
#endif

#define _MultiNop() _nop();_nop();_nop()

extern INT  g_iContrastPercentage;  // default contrast 50% if settings.dat is not present

#endif
