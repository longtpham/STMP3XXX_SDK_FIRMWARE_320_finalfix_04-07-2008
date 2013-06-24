#ifndef SED15xx_H
#define SED15xx_H

#ifndef LCD_RESET_GPDO
#define LCD_RESET_GPDO      HW_GP1DOR.B.B20		//GPIO Data Out Register
#define LCD_RESET_GPDOE     HW_GP1DOER.B.B20	//GPIO Data Out Enable Register
#define LCD_RESET_GPEN      HW_GP1ENR.B.B20		//GPIO Enable Register
#define LCD_RESET_GPPWR     HW_GP1PWR.B.B20		//GPIO Power Register
#endif	//LCD_enable 
#ifndef LCD_E_GPD0
#define LCD_E_GPDO        HW_GP0DOR.B.B10
#define LCD_E_GPDOE       HW_GP0DOER.B.B10
#define LCD_E_GPEN        HW_GP0ENR.B.B10
#define LCD_E_GPPWR       HW_GP0PWR.B.B10
#endif	//LCD_chip_select
#ifndef LCD_CS_GPDO         
#define LCD_CS_GPDO         HW_GP0DOR.B.B8
#define LCD_CS_GPDOE        HW_GP0DOER.B.B8
#define LCD_CS_GPEN         HW_GP0ENR.B.B8
#define LCD_CS_GPPWR        HW_GP0PWR.B.B8
#endif	//LVD_back_light
#ifndef LCD_BL_GPDO
#define LCD_BL_GPDO         HW_GP0DOER.B.B9
#define LCD_BL_GPDOE        HW_GP0DOER.B.B9
#define LCD_BL_GPPWR        HW_GP0PWR.B.B9
#define LCD_BL_GPEN         HW_GP0ENR.B.B9
#endif	//LCD_command/data_select
#ifndef LCD_RS_GPDO
#define LCD_RS_GPDO         HW_GP0DOR.B.B11
#define LCD_RS_GPDOE        HW_GP0DOER.B.B11
#define LCD_RS_GPPWR        HW_GP0PWR.B.B11
#define LCD_RS_GPEN         HW_GP0ENR.B.B11
#endif

#ifndef LCD_RESET_GPDO
#define LCD_RESET_GPDO      HW_GP1DOR.B.B20
#define LCD_RESET_GPDOE     HW_GP1DOER.B.B20
#define LCD_RESET_GPEN      HW_GP1ENR.B.B20
#define LCD_RESET_GPPWR     HW_GP1PWR.B.B20
#endif 

#ifndef LCD_RDE_GPD0
#define LCD_RDE_GPDO        HW_GP0DOR.B.B10
#define LCD_RDE_GPDOE       HW_GP0DOER.B.B10
#define LCD_RDE_GPEN        HW_GP0ENR.B.B10
#define LCD_RDE_GPPWR       HW_GP0PWR.B.B10
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
#define LCD_DC_GPDO         HW_GP0DOR.B.B11
#define LCD_DC_GPDOE        HW_GP0DOER.B.B11
#define LCD_DC_GPPWR        HW_GP0PWR.B.B11
#define LCD_DC_GPEN         HW_GP0ENR.B.B11
#endif
/////////////////////////////////////////////////////LCD Command Set
#define	LCD_RESET				0xE2
#define LCD_DISPLAY_ON			0xAF
#define	LCD_DISPLAY_OFF			0xAE
#define	LCD_POWER_CONTROL		0x2F
#define LCD_DISPLAY_START_LINE	0x40
#define LCD_DISPLAY_MODE		0xA6
#define LCD_ENTIRE_DISPLAY		0xA4
#define LCD_REG_RES_RATIO		0x25
#define LCD_FLIP_VERTICAL		0xC0
#define LCD_FLIP_HOTIZONTAL		0xA2
#define LCD_ICON_PAGE			0xB8

#define LCD_ICON_PAGE			0xB8
#define LCD_SEG_MASK    		0x0f
#define LCD_LSEG_CMD    		0x00
#define LCD_USEG_CMD    		0x10
//////////////////////////////////////////////////////
#ifndef LCD_MIN_CONTRAST
#define LCD_MIN_CONTRAST 0
#endif

#ifndef LCD_MAX_CONTRAST
#define LCD_MAX_CONTRAST  64
#endif

#ifndef LCD_STEPSIZE_CONTRAST
#define LCD_STEPSIZE_CONTRAST 1
#endif

#ifndef DEFAULT_CONTRAST
#define DEFAULT_CONTRAST 50
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

#define _MultiNop() _nop();_nop()

extern INT  g_iContrastPercentage;  // default contrast 50% if settings.dat is not present


#endif