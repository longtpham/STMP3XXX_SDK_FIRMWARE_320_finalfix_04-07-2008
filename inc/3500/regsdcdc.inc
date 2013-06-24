    if !(@def(regsdcdcinc))

regssysteminc            equ         1

 

;C insert

;#include "types.h"

;endc

 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   SYSTEM STMP Registers 
;	Last Edited 2.19.2003 M. May
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_DCDC_BASEADDR             equ     ($FA0C)

 

;ASM Only
HW_DCDC1_CTRL0                   equ     (HW_DCDC_BASEADDR)			 ;FA0C
HW_DCDC1_CTRL1                   equ     (HW_DCDC_BASEADDR+1)
HW_DCDC_VDDIO                    equ     (HW_DCDC_BASEADDR+2)
HW_DCDC_VDDD                     equ     (HW_DCDC_BASEADDR+3)
HW_DCDC_VDDA                     equ     (HW_DCDC_BASEADDR+4)
HW_DCDC2_CTRL0                   equ     (HW_DCDC_BASEADDR+5)
HW_DCDC2_CTRL1                   equ     (HW_DCDC_BASEADDR+6)
HW_SPEED                         equ     (HW_DCDC_BASEADDR+7)
HW_DCDC_TBR                      equ     (HW_DCDC_BASEADDR+8)
HW_DCDC_PERSIST					 equ     (HW_DCDC_BASEADDR+15)
HW_VDD5V_PWR_CHARGE				 equ     (HW_DCDC_BASEADDR+17) 		;FA1D

;endasm

 

;///////////////////////////////////////////////////////////////////////////////

;  DcDc#1 Control Register (HW_DCDC1_CTRL0) Bit Definitions

HW_DCDC1_CTRL0_PLEVBU_WIDTH                 equ     (5)
HW_DCDC1_CTRL0_RSVD0_WIDTH                  equ     (3)
HW_DCDC1_CTRL0_PLEVBO_WIDTH                 equ     (5)
HW_DCDC1_CTRL0_RSVD1_WIDTH                  equ     (3)
HW_DCDC1_CTRL0_NLEV_WIDTH                   equ     (5)

HW_DCDC1_CTRL0_RSVD2_WIDTH                  equ     (3)



HW_DCDC1_CTRL0_PLEVBU_BITPOS                equ     (0)
HW_DCDC1_CTRL0_PLEVBO_BITPOS                equ     (8)
HW_DCDC1_CTRL0_NLEV_BITPOS                  equ     (16)

 

HW_DCDC1_CTRL0_PLEVBU_SETMASK               equ     (((1<<HW_DCDC1_CTRL0_PLEVBU_WIDTH)-1)<<HW_DCDC1_CTRL0_PLEVBU_BITPOS)        
HW_DCDC1_CTRL0_PLEVBO_SETMASK               equ     (((1<<HW_DCDC1_CTRL0_PLEVBO_WIDTH)-1)<<HW_DCDC1_CTRL0_PLEVBO_BITPOS) 
HW_DCDC1_CTRL0_NLEV_SETMASK                 equ     (((1<<HW_DCDC1_CTRL0_NLEV_WIDTH)-1)<<HW_DCDC1_CTRL0_NLEV_BITPOS)  

 

HW_DCDC1_CTRL0_PLEVBU_CLRMASK               equ    (~HW_DCDC1_CTRL0_PLEVBU_SETMASK)     
HW_DCDC1_CTRL0_PLEVBO_CLRMASK               equ    (~HW_DCDC1_CTRL0_PLEVBO_SETMASK) 
HW_DCDC1_CTRL0_NLEV_CLRMASK                 equ    (~HW_DCDC1_CTRL0_NLEV_SETMASK)  

 

 

;C insert
;typedef union               
;{
;    struct {
;        unsigned int PLEVBU           : HW_DCDC1_CTRL0_PLEVBU_WIDTH;
;        unsigned int RSVD0            : HW_DCDC1_CTRL0_RSVD0_WIDTH;
;        unsigned int PLEVBO           : HW_DCDC1_CTRL0_PLEVBO_WIDTH;
;        unsigned int RSVD1            : HW_DCDC1_CTRL0_RSVD1_WIDTH;
;        unsigned int NLEV             : HW_DCDC1_CTRL0_NLEV_WIDTH;
;        unsigned int RSVD2            : HW_DCDC1_CTRL0_RSVD2_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc1_ctrl0_type;

 
;#define HW_DCDC1_CTRL0      (*(volatile dcdc1_ctrl0_type _X*) (HW_DCDC_BASEADDR))    /* DcDc#1 Limit Level Register */

;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  DCDc#1 Control Register1 (HW_DCDC1_CTRL1) Bit Definitions

HW_DCDC1_CTRL1_C_WIDTH                      equ     (4)
HW_DCDC1_CTRL1_R_WIDTH                      equ     (4)
HW_DCDC1_CTRL1_FFOR_WIDTH                   equ     (3)
HW_DCDC1_CTRL1_RSVD0_WIDTH                  equ     (1)
HW_DCDC1_CTRL1_PFMCTRL_WIDTH                equ     (12)



HW_DCDC1_CTRL1_C_BITPOS                     equ     (0)
HW_DCDC1_CTRL1_R_BITPOS                     equ     (4)
HW_DCDC1_CTRL1_FFOR_BITPOS                  equ     (8)
HW_DCDC1_CTRL1_PFMCTRL_BITPOS               equ     (12) 

HW_DCDC1_CTRL1_C_SETMASK                    equ     (((1<<HW_DCDC1_CTRL1_C_WIDTH)-1)<<HW_DCDC1_CTRL1_C_BITPOS)        
HW_DCDC1_CTRL1_R_SETMASK                    equ     (((1<<HW_DCDC1_CTRL1_R_WIDTH)-1)<<HW_DCDC1_CTRL1_R_BITPOS) 
HW_DCDC1_CTRL1_FFOR_SETMASK                 equ     (((1<<HW_DCDC1_CTRL1_FFOR_WIDTH)-1)<<HW_DCDC1_CTRL1_FFOR_BITPOS)  
HW_DCDC1_CTRL1_PFMCTRL_SETMASK              equ     (((1<<HW_DCDC1_CTRL1_PFMCTRL_WIDTH)-1)<<HW_DCDC1_CTRL1_PFMCTRL_BITPOS)   

HW_DCDC1_CTRL1_C_CLRMASK                    equ    (~HW_DCDC1_CTRL1_C_SETMASK)     
HW_DCDC1_CTRL1_R_CLRMASK                    equ    (~HW_DCDC1_CTRL1_R_SETMASK) 
HW_DCDC1_CTRL1_FFOR_CLRMASK                 equ    (~HW_DCDC1_CTRL1_FFOR_SETMASK)  
HW_DCDC1_CTRL1_PFMCTRL_CLRMASK              equ    (~HW_DCDC1_CTRL1_PFMCTRL_SETMASK)  

 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int C       : HW_DCDC1_CTRL1_C_WIDTH;
;        unsigned int R                : HW_DCDC1_CTRL1_R_WIDTH;
;        unsigned int FFOR             : HW_DCDC1_CTRL1_FFOR_WIDTH;
;        unsigned int RSVD0            : HW_DCDC1_CTRL1_RSVD0_WIDTH;
;        unsigned int PFMCTRL          : HW_DCDC1_CTRL1_FFOR_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc1_ctrl1_type;
;#define HW_DCDC1_CTRL1      (*(volatile dcdc1_ctrl1_type _X*) (HW_DCDC_BASEADDR+1))    /* DcDc#1 Ctrl #1 Register */

;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  DcDc VDDIO Register (HW_DCDC_VDDIO) Bit Definitions

HW_DCDC_VDDIO_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDIO_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDIO_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDIO_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDIO_OK_WIDTH					   equ     (1)
HW_DCDC_VDDIO_RSVD3_WIDTH                  equ     (2)



HW_DCDC_VDDIO_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS       equ     (16)
HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS       equ     (20)
HW_DCDC_VDDIO_OK_BITPOS					   equ     (21)
 

HW_DCDC_VDDIO_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDIO_VOLTAGE_LEVEL_WIDTH)-1)<<HW_DCDC_VDDIO_VOLTAGE_LEVEL_BITPOS)        
HW_DCDC_VDDIO_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_LEVEL_WIDTH)-1)<<HW_DCDC_VDDIO_BROWNOUT_LEVEL_BITPOS) 
HW_DCDC_VDDIO_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_ENABLE_WIDTH)-1)<<HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS)  
HW_DCDC_VDDIO_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_STATUS_WIDTH)-1)<<HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS)  

 

HW_DCDC_VDDIO_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDIO_VOLTAGE_LEVEL_SETMASK)     
HW_DCDC_VDDIO_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDIO_BROWNOUT_LEVEL_SETMASK) 
HW_DCDC_VDDIO_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDIO_BROWNOUT_ENABLE_SETMASK)  
HW_DCDC_VDDIO_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDIO_BROWNOUT_STATUS_SETMASK)  

 
;C insert

;typedef union               
;{
;    struct {
;        unsigned int VOLTAGE_LEVEL    : HW_DCDC_VDDIO_VOLTAGE_LEVEL_WIDTH;
;        unsigned int RSVD0            : HW_DCDC_VDDIO_RSVD0_WIDTH;
;        unsigned int BROWNOUT_LEVEL   : HW_DCDC_VDDIO_BROWNOUT_LEVEL_WIDTH;
;        unsigned int RSVD1            : HW_DCDC_VDDIO_RSVD1_WIDTH;
;        unsigned int BROWNOUT_ENABLE  : HW_DCDC_VDDIO_BROWNOUT_ENABLE_WIDTH;
;        unsigned int RSVD2            : HW_DCDC_VDDIO_RSVD2_WIDTH;
;        unsigned int BROWNOUT_STATUS  : HW_DCDC_VDDIO_BROWNOUT_STATUS_WIDTH;
;		 unsigned int VDDIO_OK	      : HW_DCDC_VDDIO_OK_WIDTH;
;        unsigned int RSVD3            : HW_DCDC_VDDIO_RSVD3_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc_vddio_type;

 

;#define HW_DCDC_VDDIO      (*(volatile dcdc_vddio_type _X*) (HW_DCDC_BASEADDR+2))    /* DcDc VDDIO Register */

;endc
 

 
;///////////////////////////////////////////////////////////////////////////////
;  DcDc VDDD Register (HW_DCDC_VDDD) Bit Definitions

HW_DCDC_VDDD_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDD_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDD_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDD_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDD_OK_WIDTH			          equ     (1)
HW_DCDC_VDDD_RSVD3_WIDTH                  equ     (2)

HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDD_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS       equ     (16)
HW_DCDC_VDDD_BROWNOUT_STATUS_BITPOS       equ     (20)
HW_DCDC_VDDD_OK_BITPOS					  equ     (21)
HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDD_VOLTAGE_LEVEL_WIDTH)-1)<<HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS)        
HW_DCDC_VDDD_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDD_BROWNOUT_LEVEL_WIDTH)-1)<<HW_DCDC_VDDD_BROWNOUT_LEVEL_BITPOS) 
HW_DCDC_VDDD_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDD_BROWNOUT_ENABLE_WIDTH)-1)<<HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS)  
HW_DCDC_VDDD_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDD_BROWNOUT_STATUS_WIDTH)-1)<<HW_DCDC_VDDD_BROWNOUT_STATUS_BITPOS)  
 
HW_DCDC_VDDD_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK)     
HW_DCDC_VDDD_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDD_BROWNOUT_LEVEL_SETMASK) 
HW_DCDC_VDDD_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDD_BROWNOUT_ENABLE_SETMASK)  
HW_DCDC_VDDD_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDD_BROWNOUT_STATUS_SETMASK)  
 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int VOLTAGE_LEVEL    : HW_DCDC_VDDD_VOLTAGE_LEVEL_WIDTH;
;        unsigned int RSVD0            : HW_DCDC_VDDD_RSVD0_WIDTH;
;        unsigned int BROWNOUT_LEVEL   : HW_DCDC_VDDD_BROWNOUT_LEVEL_WIDTH;
;        unsigned int RSVD1            : HW_DCDC_VDDD_RSVD1_WIDTH;
;        unsigned int BROWNOUT_ENABLE  : HW_DCDC_VDDD_BROWNOUT_ENABLE_WIDTH;
;        unsigned int RSVD2            : HW_DCDC_VDDD_RSVD2_WIDTH;
;        unsigned int BROWNOUT_STATUS  : HW_DCDC_VDDD_BROWNOUT_STATUS_WIDTH;
;		 unsigned int VDD_OK           : HW_DCDC_VDDD_OK_WIDTH;
;        unsigned int RSVD3            : HW_DCDC_VDDD_RSVD3_WIDTH;
;    } B;
;   unsigned int I;
;	unsigned U;
;} dcdc_vddd_type;

;#define HW_DCDC_VDDD      (*(volatile dcdc_vddd_type _X*) (HW_DCDC_BASEADDR+3))    /* DcDc VDDD Register */
;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  DcDc Vdda Register (HW_DCDC_VDDA) Bit Definitions

HW_DCDC_VDDA_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDA_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDA_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDA_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDA_OK_WIDTH					  equ     (1)
HW_DCDC_VDDA_RSVD3_WIDTH                  equ     (2)



HW_DCDC_VDDA_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDA_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDA_BROWNOUT_ENABLE_BITPOS       equ     (16)
HW_DCDC_VDDA_BROWNOUT_STATUS_BITPOS       equ     (20)
HW_DCDC_VDDA_OK_BITPOS					  equ     (21)
 

HW_DCDC_VDDA_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDA_VOLTAGE_LEVEL_WIDTH)-1)<<HW_DCDC_VDDA_VOLTAGE_LEVEL_BITPOS)        
HW_DCDC_VDDA_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDA_BROWNOUT_LEVEL_WIDTH)-1)<<HW_DCDC_VDDA_BROWNOUT_LEVEL_BITPOS) 
HW_DCDC_VDDA_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDA_BROWNOUT_ENABLE_WIDTH)-1)<<HW_DCDC_VDDA_BROWNOUT_ENABLE_BITPOS)  
HW_DCDC_VDDA_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDA_BROWNOUT_STATUS_WIDTH)-1)<<HW_DCDC_VDDA_BROWNOUT_STATUS_BITPOS)  

 

HW_DCDC_VDDA_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDA_VOLTAGE_LEVEL_SETMASK)     
HW_DCDC_VDDA_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDA_BROWNOUT_LEVEL_SETMASK) 
HW_DCDC_VDDA_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDA_BROWNOUT_ENABLE_SETMASK)  
HW_DCDC_VDDA_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDA_BROWNOUT_STATUS_SETMASK)  

 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int VOLTAGE_LEVEL    : HW_DCDC_VDDA_VOLTAGE_LEVEL_WIDTH;
;        unsigned int RSVD0            : HW_DCDC_VDDA_RSVD0_WIDTH;
;        unsigned int BROWNOUT_LEVEL   : HW_DCDC_VDDA_BROWNOUT_LEVEL_WIDTH;
;        unsigned int RSVD1            : HW_DCDC_VDDA_RSVD1_WIDTH;
;        unsigned int BROWNOUT_ENABLE  : HW_DCDC_VDDA_BROWNOUT_ENABLE_WIDTH;
;        unsigned int RSVD2            : HW_DCDC_VDDA_RSVD2_WIDTH;
;        unsigned int BROWNOUT_STATUS  : HW_DCDC_VDDA_BROWNOUT_STATUS_WIDTH;
;	     unsigned int VDDA_OK          : HW_DCDC_VDDA_OK_WIDTH;
;        unsigned int RSVD3            : HW_DCDC_VDDA_RSVD3_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc_vdda_type;

;#define HW_DCDC_VDDA      (*(volatile dcdc_vdda_type _X*) (HW_DCDC_BASEADDR+4))    /* DcDc VDDA Register */

;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  DcDc#2 Control Register 0 (HW_DCDC2_CTRL0) Bit Definitions

HW_DCDC2_CTRL0_PLEVBU_WIDTH                 equ     (5)
HW_DCDC2_CTRL0_RSVD0_WIDTH                  equ     (3)
HW_DCDC2_CTRL0_PLEVBO_WIDTH                 equ     (5)
HW_DCDC2_CTRL0_RSVD1_WIDTH                  equ     (3)
HW_DCDC2_CTRL0_NLEV_WIDTH                   equ     (5)
HW_DCDC2_CTRL0_RSVD2_WIDTH                  equ     (3)



HW_DCDC2_CTRL0_PLEVBU_BITPOS                equ     (0)
HW_DCDC2_CTRL0_PLEVBO_BITPOS                equ     (8)
HW_DCDC2_CTRL0_NLEV_BITPOS                  equ     (16)

HW_DCDC2_CTRL0_PLEVBU_SETMASK               equ     (((1<<HW_DCDC2_CTRL0_PLEVBU_WIDTH)-1)<<HW_DCDC2_CTRL0_PLEVBU_BITPOS)        
HW_DCDC2_CTRL0_PLEVBO_SETMASK               equ     (((1<<HW_DCDC2_CTRL0_PLEVBO_WIDTH)-1)<<HW_DCDC2_CTRL0_PLEVBO_BITPOS) 
HW_DCDC2_CTRL0_NLEV_SETMASK                 equ     (((1<<HW_DCDC2_CTRL0_NLEV_WIDTH)-1)<<HW_DCDC2_CTRL0_NLEV_BITPOS)  

HW_DCDC2_CTRL0_PLEVBU_CLRMASK               equ    (~HW_DCDC2_CTRL0_PLEVBU_SETMASK)     
HW_DCDC2_CTRL0_PLEVBO_CLRMASK               equ    (~HW_DCDC2_CTRL0_PLEVBO_SETMASK) 
HW_DCDC2_CTRL0_NLEV_CLRMASK                 equ    (~HW_DCDC2_CTRL0_NLEV_SETMASK)  


;C insert
;typedef union               
;{
;    struct {
;        unsigned int PLEVBU           : HW_DCDC2_CTRL0_PLEVBU_WIDTH;
;        unsigned int RSVD0            : HW_DCDC2_CTRL0_RSVD0_WIDTH;
;        unsigned int PLEVBO           : HW_DCDC2_CTRL0_PLEVBO_WIDTH;
;        unsigned int RSVD1            : HW_DCDC2_CTRL0_RSVD1_WIDTH;
;        unsigned int NLEV             : HW_DCDC2_CTRL0_NLEV_WIDTH;
;        unsigned int RSVD2            : HW_DCDC2_CTRL0_RSVD2_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc2_ctrl0_type; 

;#define HW_DCDC2_CTRL0      (*(volatile dcdc2_ctrl0_type _X*) (HW_DCDC_BASEADDR+5))    /* DcDc#2 Limit Level Register */

;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  DcDc#2 Control Register #1 (HW_DCDC2_CTRL1) Bit Definitions

HW_DCDC2_CTRL1_C_WIDTH                      equ     (4)
HW_DCDC2_CTRL1_R_WIDTH                      equ     (4)
HW_DCDC2_CTRL1_FFOR_WIDTH                   equ     (3)
HW_DCDC2_CTRL1_RSVD0_WIDTH                  equ     (1)
HW_DCDC2_CTRL1_PFMCTRL_WIDTH                equ     (12)


HW_DCDC2_CTRL1_C_BITPOS                     equ     (0)
HW_DCDC2_CTRL1_R_BITPOS                     equ     (4)
HW_DCDC2_CTRL1_FFOR_BITPOS                  equ     (8)
HW_DCDC2_CTRL1_PFMCTRL_BITPOS               equ     (12)

 

HW_DCDC2_CTRL1_C_SETMASK                    equ     (((1<<HW_DCDC2_CTRL1_C_WIDTH)-1)<<HW_DCDC2_CTRL1_C_BITPOS)        
HW_DCDC2_CTRL1_R_SETMASK                    equ     (((1<<HW_DCDC2_CTRL1_R_WIDTH)-1)<<HW_DCDC2_CTRL1_R_BITPOS) 
HW_DCDC2_CTRL1_FFOR_SETMASK                 equ     (((1<<HW_DCDC2_CTRL1_FFOR_WIDTH)-1)<<HW_DCDC2_CTRL1_FFOR_BITPOS)  
HW_DCDC2_CTRL1_PFMCTRL_SETMASK              equ     (((1<<HW_DCDC2_CTRL1_PFMCTRL_WIDTH)-1)<<HW_DCDC2_CTRL1_PFMCTRL_BITPOS)  

 

HW_DCDC2_CTRL1_C_CLRMASK                    equ    (~HW_DCDC2_CTRL1_C_SETMASK)     
HW_DCDC2_CTRL1_R_CLRMASK                    equ    (~HW_DCDC2_CTRL1_R_SETMASK) 
HW_DCDC2_CTRL1_FFOR_CLRMASK                 equ    (~HW_DCDC2_CTRL1_FFOR_SETMASK)  
HW_DCDC2_CTRL1_PFMCTRL_CLRMASK              equ    (~HW_DCDC2_CTRL1_PFMCTRL_SETMASK)  

 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int C                : HW_DCDC2_CTRL1_C_WIDTH;
;        unsigned int R                : HW_DCDC2_CTRL1_R_WIDTH;
;        unsigned int FFOR             : HW_DCDC2_CTRL1_FFOR_WIDTH;
;        unsigned int RSVD0            : HW_DCDC2_CTRL1_RSVD0_WIDTH;
;        unsigned int PFMCTRL          : HW_DCDC2_CTRL1_FFOR_WIDTH;
;    } B;
;    unsigned int I;
;} dcdc2_ctrl1_type;

;#define HW_DCDC2_CTRL1      (*(volatile dcdc2_ctrl1_type _X*) (HW_DCDC_BASEADDR+6))    /* DcDc#2 Ctrl Register #1 */

;endc

 

 
;///////////////////////////////////////////////////////////////////////////////

;  Speed Monitoring Register (HW_SPEED) Bit Definitions

HW_SPEED_OSC1_PWRUP_WIDTH             equ     (1)
HW_SPEED_OSC1_START_COUNT_WIDTH       equ     (1)
HW_SPEED_RSVD0_WIDTH                  equ     (2)
HW_SPEED_RINGOSC1_WIDTH               equ     (7)
HW_SPEED_RSVD1_WIDTH                  equ     (1)
HW_SPEED_OSC2_PWRUP_WIDTH             equ     (1)
HW_SPEED_OSC2_START_COUNT_WIDTH       equ     (1)
HW_SPEED_RSVD2_WIDTH                  equ     (2)
HW_SPEED_RINGOSC2_WIDTH               equ     (7)
HW_SPEED_RSVD3_WIDTH                  equ     (1)



HW_SPEED_OSC1_PWRUP_BITPOS            equ     (0)
HW_SPEED_OSC1_START_COUNT_BITPOS      equ     (1)
HW_SPEED_RINGOSC1_BITPOS              equ     (4)
HW_SPEED_OSC2_PWRUP_BITPOS            equ     (12)
HW_SPEED_OSC2_START_COUNT_BITPOS      equ     (13)
HW_SPEED_RINGOSC2_BITPOS              equ     (16)


HW_SPEED_OSC1_PWRUP_SETMASK           equ     (((1<<HW_SPEED_OSC1_PWRUP_WIDTH)-1)<<HW_SPEED_OSC1_PWRUP_BITPOS)        
HW_SPEED_OSC1_START_COUNT_SETMASK     equ     (((1<<HW_SPEED_OSC1_START_COUNT_WIDTH)-1)<<HW_SPEED_OSC1_START_COUNT_BITPOS)        
HW_SPEED_RINGOSC1_SETMASK             equ     (((1<<HW_SPEED_RINGOSC1_WIDTH)-1)<<HW_SPEED_RINGOSC1_BITPOS) 
HW_SPEED_OSC2_PWRUP_SETMASK           equ     (((1<<HW_SPEED_OSC2_PWRUP_WIDTH)-1)<<HW_SPEED_OSC2_PWRUP_BITPOS)        

HW_SPEED_OSC2_START_COUNT_SETMASK     equ     (((1<<HW_SPEED_OSC2_START_COUNT_WIDTH)-1)<<HW_SPEED_OSC2_START_COUNT_BITPOS)        
HW_SPEED_RINGOSC2_SETMASK             equ     (((1<<HW_SPEED_RINGOSC2_WIDTH)-1)<<HW_SPEED_RINGOSC2_BITPOS) 



HW_SPEED_OSC1_PWRUP_CLRMASK        equ    (~HW_SPEED_OSC1_PWRUP_SETMASK)     
HW_SPEED_OSC1_START_COUNT_CLRMASK  equ    (~HW_SPEED_OSC1_START_COUNT_SETMASK)     
HW_SPEED_RINGOSC1_CLRMASK          equ    (~HW_SPEED_RINGOSC1_SETMASK) 
HW_SPEED_OSC2_PWRUP_CLRMASK        equ    (~HW_SPEED_OSC2_PWRUP_SETMASK)   
HW_SPEED_OSC2_START_COUNT_CLRMASK  equ    (~HW_SPEED_OSC2_START_COUNT_SETMASK)     
HW_SPEED_RINGOSC2_CLRMASK          equ    (~HW_SPEED_RINGOSC2_SETMASK) 

 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int OSC1_PWRUP          : HW_SPEED_OSC1_PWRUP_WIDTH;
;        unsigned int OSC1_START_COUNT    : HW_SPEED_OSC1_START_COUNT_WIDTH;
;        unsigned int RSVD0               : HW_SPEED_RSVD0_WIDTH;
;        unsigned int RINGOSC1            : HW_SPEED_RINGOSC1_WIDTH;
;        unsigned int RSVD1               : HW_SPEED_RSVD1_WIDTH;
;        unsigned int OSC2_PWRUP          : HW_SPEED_OSC2_PWRUP_WIDTH;
;        unsigned int OSC2_START_COUNT    : HW_SPEED_OSC2_START_COUNT_WIDTH;
;        unsigned int RSVD2               : HW_SPEED_RSVD2_WIDTH;
;        unsigned int RINGOSC2            : HW_SPEED_RINGOSC2_WIDTH;
;        unsigned int RSVD3               : HW_SPEED_RSVD3_WIDTH;
;    } B;
;    unsigned int I;
;} speed_type;
;#define HW_SPEED      (*(volatile speed_type _X*) (HW_DCDC_BASEADDR+7))    /* Speed Measurement Register */
;endc

 

 

 

; DC DC Test Bit Register (HW_DCDCTBR) Definitions
HW_DCDC_TBR_DCDC1_ADJ_TN_WIDTH                   equ 4
HW_DCDC_TBR_DCDC1_DIS_5BIT_WIDTH                 equ 1
HW_DCDC_TBR_DCDC1_BAT_ADJ_WIDTH                  equ 1
HW_DCDC_TBR_DCDC1_CLK4X_WIDTH                    equ 1
HW_DCDC_TBR_DCDC1_CLK2X_WIDTH                    equ 1
HW_DCDC_TBR_DCDC1_NOZERO_WIDTH                   equ 1
HW_DCDC_TBR_DCDC1_VDD5V_ACTIVE_WIDTH             equ 1
HW_DCDC_TBR_DCDC1_PFM_WIDTH                      equ 1
HW_DCDC_TBR_DCDC1_STOPCLK_WIDTH                  equ 1
HW_DCDC_TBR_PWRUP_VDDIO_BRNOUT_WIDTH             equ 1
HW_DCDC_TBR_DCDC_ANA_LESSI_WIDTH                 equ 1
HW_DCDC_TBR_DCDC_MORE_CAP_WIDTH                  equ 1
HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_WIDTH              equ 1
HW_DCDC_TBR_DCDC1_NEW_SCHEME_WIDTH               equ 1
HW_DCDC_TBR_DCDC1_HALF_FETS_WIDTH                equ 1
HW_DCDC_TBR_DCDC2_DIS_5BIT_WIDTH                 equ 1
HW_DCDC_TBR_DCDC2_CLK4X_WIDTH                    equ 1
HW_DCDC_TBR_DCDC2_CLK2X_WIDTH                    equ 1
HW_DCDC_TBR_DCDC2_VDD5V_ACTIVE_WIDTH             equ 1
HW_DCDC_TBR_DCDC2_PFM_WIDTH                      equ 1
HW_DCDC_TBR_DCDC2_STOPCLK_WIDTH                  equ 1

HW_DCDC_TBR_DCDC1_ADJ_TN_BITPOS              equ    (0)
HW_DCDC_TBR_DCDC1_DIS_5BIT_BITPOS            equ 	(4)
HW_DCDC_TBR_DCDC1_BAT_ADJ_BITPOS             equ    (5)
HW_DCDC_TBR_DCDC1_CLK4X_BITPOS               equ 	(6)
HW_DCDC_TBR_DCDC1_CLK2X_BITPOS               equ 	(7)
HW_DCDC_TBR_DCDC1_NOZERO_BITPOS              equ 	(8)
HW_DCDC_TBR_DCDC1_VDD5V_ACTIVE_BITPOS        equ 	(9)
HW_DCDC_TBR_DCDC1_PFM_BITPOS                 equ 	(10)
HW_DCDC_TBR_DCDC1_STOPCLK_BITPOS             equ 	(11)
HW_DCDC_TBR_PWRUP_VDDIO_BRNOUT_BITPOS        equ 	(12)
HW_DCDC_TBR_DCDC_ANA_LESSI_BITPOS            equ 	(13)
HW_DCDC_TBR_DCDC_MORE_CAP_BITPOS             equ 	(14)
HW_DCDC_TBR_DCDC_DCDC_ANA_BGR_BIAS_BITPOS    equ    (15)
HW_DCDC_TBR_DCDC1_NEW_SCHEME_BITPOS          equ 	(16)
HW_DCDC_TBR_DCDC1_HALF_FETS_BITPOS           equ 	(17)
HW_DCDC_TBR_DCDC2_DIS_5BIT_BITPOS            equ 	(18)
HW_DCDC_TBR_DCDC2_CLK4X_BITPOS               equ 	(19)
HW_DCDC_TBR_DCDC2_CLK2X_BITPOS               equ 	(20)
HW_DCDC_TBR_DCDC2_VDD5V_ACTIVE_BITPOS        equ 	(21)
HW_DCDC_TBR_DCDC2_PFM_BITPOS                 equ 	(22)
HW_DCDC_TBR_DCDC2_STOPCLK_BITPOS             equ 	(23)

HW_DCDC_TBR_DCDC1_ADJ_TN_SETMASK             equ    (((1<<HW_DCDC_TBR_DCDC1_ADJ_TN_WIDTH)-1)<<HW_DCDC_TBR_DCDC1_ADJ_TN_BITPOS)
HW_DCDC_TBR_DCDC1_BAT_ADJ_SETMASK            equ    (((1<<HW_DCDC_TBR_DCDC1_BAT_ADJ_WIDTH)-1)<<HW_DCDC_TBR_DCDC1_BAT_ADJ_BITPOS)
HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_SETMASK   equ    (((1<<HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_WIDTH)-1)<<HW_DCDC_TBR_DCDC_DCDC_ANA_BGR_BIAS_BITPOS)

HW_DCDC_TBR_DCDC1_ADJ_TN_CLRMASK             equ    (~HW_DCDC_TBR_DCDC1_ADJ_TN_SETMASK)
HW_DCDC_TBR_DCDC1_BAT_ADJ_CLRMASK            equ    (~HW_DCDC_TBR_DCDC1_BAT_ADJ_SETMASK)
HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_CLRMASK   equ    (~HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_SETMASK)

; Defines for What needs to change to be able to use a higher Core voltage and when the change should happen
HW_DCDC_VDDD_CHANGE_TBR_CUTOFF_VALUE         equ    ((18<<HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS)&HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK)
HW_DCDC_TBR_VDDD_HI_VALUE                    equ    ((1<<HW_DCDC_TBR_DCDC_DCDC_ANA_BGR_BIAS_BITPOS)&HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_SETMASK)
HW_DCDC_TBR_VDDD_LO_VALUE                    equ    (((1<<HW_DCDC_TBR_DCDC_DCDC_ANA_BGR_BIAS_BITPOS)&HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_SETMASK)+((2<<HW_DCDC_TBR_DCDC1_ADJ_TN_BITPOS)&HW_DCDC_TBR_DCDC1_ADJ_TN_SETMASK)+((1<<HW_DCDC_TBR_DCDC1_BAT_ADJ_BITPOS)&HW_DCDC_TBR_DCDC1_BAT_ADJ_SETMASK))

;C insert
;typedef union               
;{
;    struct {
;        unsigned int DCDC1_ADJ_TN               : HW_DCDC_TBR_DCDC1_ADJ_TN_WIDTH;
;        unsigned int DCDC1_DIS_5BIT             : HW_DCDC_TBR_DCDC1_DIS_5BIT_WIDTH;
;        unsigned int DCDC1_BAT_ADJ              : HW_DCDC_TBR_DCDC1_BAT_ADJ_WIDTH;
;        unsigned int DCDC1_CLK4X                : HW_DCDC_TBR_DCDC1_CLK4X_WIDTH;
;        unsigned int DCDC1_CLK2X                : HW_DCDC_TBR_DCDC1_CLK2X_WIDTH;
;        unsigned int DCDC1_NOZERO               : HW_DCDC_TBR_DCDC1_NOZERO_WIDTH;
;        unsigned int DCDC1_VDD5V_ACTIVE         : HW_DCDC_TBR_DCDC1_VDD5V_ACTIVE_WIDTH;
;        unsigned int DCDC1_PFM                  : HW_DCDC_TBR_DCDC1_PFM_WIDTH;
;        unsigned int DCDC1_STOPCLK              : HW_DCDC_TBR_DCDC1_STOPCLK_WIDTH;
;        unsigned int PWRUP_VDDIO_BRNOUT         : HW_DCDC_TBR_PWRUP_VDDIO_BRNOUT_WIDTH;
;        unsigned int DCDC_ANA_LESSI             : HW_DCDC_TBR_DCDC_ANA_LESSI_WIDTH;
;        unsigned int DCDC_MORE_CAP              : HW_DCDC_TBR_DCDC_MORE_CAP_WIDTH;
;        unsigned int DCDC_ANA_BGR_BIAS          : HW_DCDC_TBR_DCDC_ANA_BGR_BIAS_WIDTH;
;        unsigned int DCDC1_NEW_SCHEME           : HW_DCDC_TBR_DCDC1_NEW_SCHEME_WIDTH;
;        unsigned int DCDC1_HALF_FETS            : HW_DCDC_TBR_DCDC1_HALF_FETS_WIDTH;
;        unsigned int DCDC2_DIS_5BIT             : HW_DCDC_TBR_DCDC2_DIS_5BIT_WIDTH;
;        unsigned int DCDC2_CLK4X                : HW_DCDC_TBR_DCDC2_CLK4X_WIDTH;
;        unsigned int DCDC2_CLK2X                : HW_DCDC_TBR_DCDC2_CLK2X_WIDTH;
;        unsigned int DCDC2_VDD5V_ACTIVE         : HW_DCDC_TBR_DCDC2_VDD5V_ACTIVE_WIDTH;
;        unsigned int DCDC2_PFM                  : HW_DCDC_TBR_DCDC2_PFM_WIDTH;
;        unsigned int DCDC2_STOPCLK              : HW_DCDC_TBR_DCDC2_STOPCLK_WIDTH;
;    } B;
;    unsigned int I;
;} usb_dcdctbr_type;

;#define HW_DCDC_TBR                     (*(volatile usb_dcdctbr_type _X*) (HW_DCDC_BASEADDR+8))    /* Analog test bit register*/
;endc


;///////////////////////////////////////////////////////////////////////////////

;  Analog Persistent Config (HW_VDD5V_PWR_CHARGE) Bit Definitions

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_WIDTH       equ     (6)
HW_VDD5V_PWR_CHARGE_RSVD0_WIDTH              equ     (2)
HW_VDD5V_PWR_CHARGE_PWD_WIDTH                equ     (1)
HW_VDD5V_PWR_CHARGE_RES_WIDTH                equ     (1)
HW_VDD5V_PWR_CHARGE_NIMH_WIDTH               equ     (1)
HW_VDD5V_PWR_CHARGE_LI_TYPE_WIDTH            equ     (1)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_WIDTH     equ     (1)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_WIDTH   equ     (1)
HW_VDD5V_PWR_CHARGE_DCANA_LP_WIDTH           equ     (1)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_WIDTH       equ     (1)
HW_VDD5V_PWR_CHARGE_DRV_BATT_WIDTH           equ     (1)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_WIDTH         equ     (1)
HW_VDD5V_PWR_CHARGE_RSVD1_WIDTH              equ     (2)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_WIDTH      equ     (1)
HW_VDD5V_PWR_CHARGE_BATT_INFO_WIDTH          equ     (2)
HW_VDD5V_PWR_CHARGE_RSVD2_WIDTH              equ     (3)

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_BITPOS      equ     (0)
HW_VDD5V_PWR_CHARGE_PWD_BITPOS               equ     (8)
HW_VDD5V_PWR_CHARGE_RES_BITPOS               equ     (9)
HW_VDD5V_PWR_CHARGE_NIMH_BITPOS              equ     (10)

HW_VDD5V_PWR_CHARGE_LI_TYPE_BITPOS           equ     (11)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS    equ     (12)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS  equ     (13)
HW_VDD5V_PWR_CHARGE_DCANA_LP_BITPOS          equ     (14)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_BITPOS      equ     (15)
HW_VDD5V_PWR_CHARGE_DRV_BATT_BITPOS          equ     (16)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_BITPOS        equ     (17)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_BITPOS     equ     (20)
HW_VDD5V_PWR_CHARGE_BATT_INFO_BITPOS         equ     (21)

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_SETMASK     equ     (((1<<HW_VDD5V_PWR_CHARGE_BATT_CURRENT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_BATT_CURRENT_BITPOS)        
HW_VDD5V_PWR_CHARGE_PWD_SETMASK              equ     (((1<<HW_VDD5V_PWR_CHARGE_PWD_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_PWD_BITPOS)        
HW_VDD5V_PWR_CHARGE_RES_SETMASK              equ     (((1<<HW_VDD5V_PWR_CHARGE_RES_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_RES_BITPOS) 
HW_VDD5V_PWR_CHARGE_NIMH_SETMASK             equ     (((1<<HW_VDD5V_PWR_CHARGE_NIMH_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_NIMH_BITPOS) 
HW_VDD5V_PWR_CHARGE_LI_TYPE_SETMASK          equ     (((1<<HW_VDD5V_PWR_CHARGE_LI_TYPE_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_LI_TYPE_BITPOS) 
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_SETMASK   equ     (((1<<HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS)        
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_SETMASK equ     (((1<<HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS)        
HW_VDD5V_PWR_CHARGE_DCANA_LP_SETMASK         equ     (((1<<HW_VDD5V_PWR_CHARGE_DCANA_LP_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_DCANA_LP_BITPOS) 
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_SETMASK     equ     (((1<<HW_VDD5V_PWR_CHARGE_TEST_USBREGS_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_TEST_USBREGS_BITPOS) 
HW_VDD5V_PWR_CHARGE_DRV_BATT_SETMASK         equ     (((1<<HW_VDD5V_PWR_CHARGE_DRV_BATT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_DRV_BATT_BITPOS) 
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_SETMASK       equ     (((1<<HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_BITPOS) 
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_SETMASK    equ     (((1<<HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_BITPOS) 
HW_VDD5V_PWR_CHARGE_BATT_INFO_SETMASK        equ     (((1<<HW_VDD5V_PWR_CHARGE_BATT_INFO_WIDTH)-1)<<HW_VDD5V_PWR_CHARGE_BATT_INFO_BITPOS) 


HW_VDD5V_PWR_CHARGE_BATT_CURRENT_CLRMASK     equ    (~HW_VDD5V_PWR_CHARGE_BATT_CURRENT_SETMASK)     
HW_VDD5V_PWR_CHARGE_PWD_CLRMASK              equ    (~HW_VDD5V_PWR_CHARGE_PWD_SETMASK)     
HW_VDD5V_PWR_CHARGE_RES_CLRMASK              equ    (~HW_VDD5V_PWR_CHARGE_RES_SETMASK) 
HW_VDD5V_PWR_CHARGE_NIMH_CLRMASK             equ    (~HW_VDD5V_PWR_CHARGE_NIMH_SETMASK) 
HW_VDD5V_PWR_CHARGE_LI_TYPE_CLRMASK          equ    (~HW_VDD5V_PWR_CHARGE_LI_TYPE_SETMASK) 
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_CLRMASK   equ    (~HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_SETMASK)     
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_CLRMASK equ    (~HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_SETMASK)     
HW_VDD5V_PWR_CHARGE_DCANA_LP_CLRMASK         equ    (~HW_VDD5V_PWR_CHARGE_DCANA_LP_SETMASK) 
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_CLRMASK     equ    (~HW_VDD5V_PWR_CHARGE_TEST_USBREGS_SETMASK) 
HW_VDD5V_PWR_CHARGE_DRV_BATT_CLRMASK         equ    (~HW_VDD5V_PWR_CHARGE_DRV_BATT_SETMASK) 
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_CLRMASK       equ    (~HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_SETMASK) 
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_CLRMASK    equ    (~HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_SETMASK) 
HW_VDD5V_PWR_CHARGE_BATT_INFO_CLRMASK        equ    (~HW_VDD5V_PWR_CHARGE_BATT_INFO_SETMASK) 
 
;C insert
;typedef union               
;{
;    struct {
;        unsigned int BATT_CURRENT               : HW_VDD5V_PWR_CHARGE_BATT_CURRENT_WIDTH;
;        unsigned int RSVD0                      : HW_VDD5V_PWR_CHARGE_RSVD0_WIDTH;
;        unsigned int PWD                        : HW_VDD5V_PWR_CHARGE_PWD_WIDTH;
;        unsigned int RES                        : HW_VDD5V_PWR_CHARGE_RES_WIDTH;
;        unsigned int NIMH                       : HW_VDD5V_PWR_CHARGE_NIMH_WIDTH;
;        unsigned int LI_TYPE                    : HW_VDD5V_PWR_CHARGE_LI_TYPE_WIDTH;
;        unsigned int DISABLE_ILIMIT             : HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_WIDTH;
;        unsigned int PWDN_ON_IOBRNOUT           : HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_WIDTH;
;        unsigned int DCANA_LP                   : HW_VDD5V_PWR_CHARGE_DCANA_LP_WIDTH;
;        unsigned int TEST_USBREGS               : HW_VDD5V_PWR_CHARGE_TEST_USBREGS_WIDTH;
;        unsigned int DRV_BATT                   : HW_VDD5V_PWR_CHARGE_DRV_BATT_WIDTH;
;        unsigned int SWCHRG_BAT                 : HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_WIDTH;
;        unsigned int RSVD1                      : HW_VDD5V_PWR_CHARGE_RSVD1_WIDTH;
;        unsigned int VDD5V_PRESENT              : HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_WIDTH;
;        unsigned int RSVD2                      : HW_VDD5V_PWR_CHARGE_RSVD2_WIDTH;
;    } B;
;    unsigned int I;
;} usb_pwr_charge_type;
 
;#define HW_VDD5V_PWR_CHARGE      (*(volatile usb_pwr_charge_type _X*) (HW_DCDC_BASEADDR+17))    /* Analog Persistent Config Register */
;endc

HW_DCDC_PERSIST_AUTO_RESTART_STAT_WIDTH             equ 1
HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_STAT_WIDTH    equ 1
HW_DCDC_PERSIST_UPDATE_WIDTH                        equ 1
HW_DCDC_PERSIST_AUTO_RESTART_WIDTH                  equ 1
HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_WIDTH         equ 1
HW_DCDC_PERSIST_LOW_BATT_TYPE_WIDTH                 equ 1
HW_DCDC_PERSIST_LOW_BATTERY_ENABLE_WIDTH            equ 1
HW_DCDC_PERSIST_SLEEP_XTAL_ENABLE_WIDTH             equ 1
HW_DCDC_PERSIST_XTAL_TRIM1_WIDTH                    equ 1
HW_DCDC_PERSIST_XTAL_TRIM0_WIDTH                    equ 1
HW_DCDC_PERSIST_XTAL_BIAS_DOWN1_WIDTH               equ 1
HW_DCDC_PERSIST_XTAL_BIAS_DOWN0_WIDTH               equ 1
HW_DCDC_PERSIST_XTAL_TRM_ENABLE_WIDTH               equ 1
HW_DCDC_PERSIST_RSRVD_WIDTH                         equ 2

HW_DCDC_PERSIST_AUTO_RESTART_STAT_BITPOS             equ 12
HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_STAT_BITPOS    equ 11
HW_DCDC_PERSIST_UPDATE_BITPOS                        equ 10
HW_DCDC_PERSIST_AUTO_RESTART_BITPOS                  equ 9
HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_BITPOS         equ 8 
HW_DCDC_PERSIST_LOW_BATT_TYPE_BITPOS                 equ 7
HW_DCDC_PERSIST_LOW_BATTERY_ENABLE_BITPOS            equ 6
HW_DCDC_PERSIST_SLEEP_XTAL_ENABLE_BITPOS             equ 5
HW_DCDC_PERSIST_XTAL_TRIM1_BITPOS                    equ 4
HW_DCDC_PERSIST_XTAL_TRIM0_BITPOS                    equ 3
HW_DCDC_PERSIST_XTAL_BIAS_DOWN1_BITPOS               equ 2
HW_DCDC_PERSIST_XTAL_BIAS_DOWN0_BITPOS               equ 1
HW_DCDC_PERSIST_XTAL_TRM_ENABLE_BITPOS               equ 0
 
;C insert
;typedef union               
;{
;    struct {       
;       int XTAL_TRM_ENABLE             : HW_DCDC_PERSIST_XTAL_TRM_ENABLE_WIDTH;
;       int XTAL_BIAS_DOWN0             : HW_DCDC_PERSIST_XTAL_BIAS_DOWN0_WIDTH;
;       int XTAL_BIAS_DOWN1             : HW_DCDC_PERSIST_XTAL_BIAS_DOWN1_WIDTH;
;       int XTAL_TRIM0                  : HW_DCDC_PERSIST_XTAL_TRIM0_WIDTH;
;       int XTAL_TRIM1                  : HW_DCDC_PERSIST_XTAL_TRIM1_WIDTH;
;       int SLEEP_XTAL_ENABLE           : HW_DCDC_PERSIST_SLEEP_XTAL_ENABLE_WIDTH;
;       int LOW_BATTERY_ENABLE          : HW_DCDC_PERSIST_LOW_BATTERY_ENABLE_WIDTH;
;       int LOW_BATT_TYPE               : HW_DCDC_PERSIST_LOW_BATT_TYPE_WIDTH;
;       int DELAY_5V_AUTO_RESTART       : HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_WIDTH;
;       int AUTO_RESTART                : HW_DCDC_PERSIST_AUTO_RESTART_WIDTH;
;       int UPDATE                      : HW_DCDC_PERSIST_UPDATE_WIDTH;
;       int DELAY_5V_AUTO_RESTART_STAT  : HW_DCDC_PERSIST_DELAY_5V_AUTO_RESTART_STAT_WIDTH;
;       int AUTO_RESTART_STAT           : HW_DCDC_PERSIST_AUTO_RESTART_STAT_WIDTH;
;       int RSRVD                       : HW_DCDC_PERSIST_RSRVD_WIDTH;
;    } B;
;    int I;
;} usb_dcdcpersist_type;


;#define HW_DCDC_PERSIST                (*(volatile usb_dcdcpersist_type _X*) (HW_DCDC_BASEADDR+15))    /* Analog test bit register*/
;endc



    endif

 
