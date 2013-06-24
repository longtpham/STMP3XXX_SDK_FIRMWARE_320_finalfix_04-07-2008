;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename: regstb.inc
; Description: Register definitions for Trace Buffer
;///////////////////////////////////////////////////////////////////////////////
; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     USB20
; (Note that when there is more than one copy of a particular module, the
; module name includes a number starting from 0 for the first instance of
; that module)
;
; <regname> is the specific register within that module
;
; We also define the following...
;
;     HW_<module>_<regname>_BITPOS
; which defines the starting bit (i.e. LSB) of a multi bit field
;
;     HW_<module>_<regname>_SETMASK
; which does something else, and
;
;     HW_<module>_<regname>_CLRMASK
; which does something else.
;
; Other rules
;     All caps
;     Numeric identifiers start at 0
    if !(@def(regstbinc))
regstbinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   Trace Buffer STMP Registers 
;   Last Updated 6.30.2003 D. Baker
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_TB_BASEADDR             equ     ($F080)

 

;ASM Only

 
HW_TB_CFG                    equ     (HW_TB_BASEADDR+0)
HW_TB_BAR                    equ     (HW_TB_BASEADDR+1)
HW_TB_MOD                    equ     (HW_TB_BASEADDR+2)
HW_TB_CIR                    equ     (HW_TB_BASEADDR+3)
HW_TB_OBC                    equ     (HW_TB_BASEADDR+4)
HW_TB_TCS                    equ     (HW_TB_BASEADDR+16)
HW_TB_TVR                    equ     (HW_TB_BASEADDR+24)

;endasm

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Configuration Register (HW_TB_CFG) Bit Definitions

HW_TB_CFG_CLK_ENABLE_BITPOS    equ     (0)
HW_TB_CFG_ENABLE_BITPOS        equ     (1)
HW_TB_CFG_DONE_BITPOS          equ     (3)
HW_TB_CFG_DMA_ASEL_BITPOS      equ     (4)
HW_TB_CFG_TRIG_EVENT_BITPOS    equ     (6)

HW_TB_CFG_CLK_ENABLE_WIDTH     equ     (1)
HW_TB_CFG_ENABLE_WIDTH         equ     (1)
HW_TB_CFG_RSVD1_WIDTH          equ     (1)
HW_TB_CFG_DONE_WIDTH           equ     (1)
HW_TB_CFG_DMA_ASEL_WIDTH       equ     (2)
HW_TB_CFG_TRIG_EVENT_WIDTH     equ     (1)
HW_TB_CFG_RSVD2_WIDTH          equ     (17)
   
HW_TB_CFG_CLK_ENABLE_SETMASK   equ     (((1<<HW_TB_CFG_CLK_ENABLE_WIDTH)-1)<<HW_TB_CFG_CLK_ENABLE_BITPOS) 
HW_TB_CFG_ENABLE_SETMASK       equ     (((1<<HW_TB_CFG_ENABLE_WIDTH)-1)<<HW_TB_CFG_ENABLE_BITPOS) 
HW_TB_CFG_DONE_SETMASK         equ     (((1<<HW_TB_CFG_DONE_WIDTH)-1)<<HW_TB_CFG_DONE_BITPOS) 
HW_TB_CFG_DMA_ASEL_SETMASK     equ     (((1<<HW_TB_CFG_DMA_ASEL_WIDTH)-1)<<HW_TB_CFG_DMA_ASEL_BITPOS) 
HW_TB_CFG_TRIG_EVENT_SETMASK   equ     (((1<<HW_TB_CFG_TRIG_EVENT_WIDTH)-1)<<HW_TB_CFG_TRIG_EVENT_BITPOS) 
  
HW_TB_CFG_CLK_ENABLE_CLRMASK   equ     (~HW_TB_CFG_CLK_ENABLE_SETMASK)
HW_TB_CFG_ENABLE_CLRMASK       equ     (~HW_TB_CFG_ENABLE_SETMASK)
HW_TB_CFG_DONE_CLRMASK         equ     (~HW_TB_CFG_DONE_SETMASK)
HW_TB_CFG_DMA_ASEL_CLRMASK     equ     (~HW_TB_CFG_DMA_ASEL_SETMASK)
HW_TB_CFG_TRIG_EVENT_CLRMASK   equ     (~HW_TB_CFG_TRIG_EVENT_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int CLK_ENABLE      : HW_TB_CFG_CLK_ENABLE_WIDTH;
;	 int ENABLE 	     : HW_TB_CFG_ENABLE_WIDTH;
;        int rsvd1           : HW_TB_CFG_RSVD1_WIDTH;
;	 int DONE 	     : HW_TB_CFG_DONE_WIDTH;
;	 int DMA_ASEL 	     : HW_TB_CFG_DMA_ASEL_WIDTH;
;	 int TRIG_EVENT      : HW_TB_CFG_TRIG_EVENT_WIDTH;
;        int rsvd2           : HW_TB_CFG_RSVD2_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_cfg_type;

 

;#define HW_TB_CFG      (*(volatile tb_cfg_type _X*) (HW_TB_BASEADDR+0))    /* Trace Buffer Configuration Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Base Address Register (HW_TB_BAR) Bit Definitions

HW_TB_BAR_ADDRESS_BITPOS    equ     (0)

HW_TB_BAR_ADDRESS_WIDTH     equ     (16)        
HW_TB_BAR_RSVD_WIDTH        equ     (8)
   
HW_TB_BAR_ADDRESS_SETMASK   equ     (((1<<HW_TB_BAR_ADDRESS_WIDTH)-1)<<HW_TB_BAR_ADDRESS_BITPOS) 
  
HW_TB_BAR_ADDRESS_CLRMASK   equ     (~HW_TB_BAR_ADDRESS_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int ADDRESS      : HW_TB_BAR_ADDRESS_WIDTH;
;        int reserved     : HW_TB_BAR_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_bar_type;

 

;#define HW_TB_BAR      (*(volatile tb_bar_type _X*) (HW_TB_BASEADDR+1))    /* Trace Buffer Base Address Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Modulo Register (HW_TB_MOD) Bit Definitions

HW_TB_MOD_MODULUS_BITPOS    equ     (0)

HW_TB_MOD_MODULUS_WIDTH     equ     (14)        
HW_TB_MOD_RSVD_WIDTH           equ     (10)
   
HW_TB_MOD_MODULUS_SETMASK   equ     (((1<<HW_TB_MOD_MODULUS_WIDTH)-1)<<HW_TB_MOD_MODULUS_BITPOS) 
  
HW_TB_MOD_MODULUS_CLRMASK   equ     (~HW_TB_MOD_MODULUS_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int MODULUS      : HW_TB_MOD_MODULUS_WIDTH;
;        int reserved        : HW_TB_MOD_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_mod_type;

 

;#define HW_TB_MOD      (*(volatile tb_mod_type _X*) (HW_TB_BASEADDR+2))    /* Trace Buffer Modulus Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Current Index Register (HW_TB_CIR) Bit Definitions

HW_TB_CIR_INDEX_BITPOS    equ     (0)

HW_TB_CIR_INDEX_WIDTH     equ     (14)        
HW_TB_CIR_RSVD_WIDTH      equ     (10)
   
HW_TB_CIR_INDEX_SETMASK   equ     (((1<<HW_TB_CIR_INDEX_WIDTH)-1)<<HW_TB_CIR_INDEX_BITPOS) 
  
HW_TB_CIR_INDEX_CLRMASK   equ     (~HW_TB_CIR_INDEX_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int INDEX        : HW_TB_CIR_INDEX_WIDTH;
;        int reserved     : HW_TB_CIR_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_cir_type;

 

;#define HW_TB_CIR      (*(volatile tb_cir_type _X*) (HW_TB_BASEADDR+3))    /* Trace Buffer Current Index Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer One Byte Code Register (HW_TB_OBC) Bit Definitions

HW_TB_OBC_CODE_BITPOS    equ     (0)

HW_TB_OBC_CODE_WIDTH     equ     (8)        
HW_TB_OBC_RSVD_WIDTH           equ     (16)
   
HW_TB_OBC_CODE_SETMASK   equ     (((1<<HW_TB_OBC_CODE_WIDTH)-1)<<HW_TB_OBC_CODE_BITPOS) 
  
HW_TB_OBC_CODE_CLRMASK   equ     (~HW_TB_OBC_CODE_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int CODE        : HW_TB_OBC_CODE_WIDTH;
;        int reserved    : HW_TB_OBC_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_obc_type;

 

;#define HW_TB_OBC      (*(volatile tb_obc_type _X*) (HW_TB_BASEADDR+4))    /* Trace Buffer one byte code Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Trigger Command Register (HW_TB_TCS) Bit Definitions

HW_TB_TCS_TRG_STYLE_BITPOS     equ    (0)
HW_TB_TCS_CAP_CLASS_BITPOS    equ     (1)
HW_TB_TCS_TRG_CLASS_BITPOS    equ     (3)
HW_TB_TCS_FREEZE_BITPOS       equ     (5)

HW_TB_TCS_TRG_STYLE_WIDTH      equ    (1)        
HW_TB_TCS_CAP_CLASS_WIDTH     equ     (2)        
HW_TB_TCS_TRG_CLASS_WIDTH     equ     (2)        
HW_TB_TCS_FREEZE_WIDTH        equ     (1)        
HW_TB_TCS_RSVD_WIDTH          equ     (18)
   
HW_TB_TCS_TRG_STYLE_SETMASK    equ    (((1<<HW_TB_TCS_TRG_STYLE_WIDTH)-1)<<HW_TB_TCS_TRG_STYLE_BITPOS) 
HW_TB_TCS_CAP_CLASS_SETMASK   equ     (((1<<HW_TB_TCS_CAP_CLASS_WIDTH)-1)<<HW_TB_TCS_CAP_CLASS_BITPOS) 
HW_TB_TCS_TRG_CLASS_SETMASK   equ     (((1<<HW_TB_TCS_TRG_CLASS_WIDTH)-1)<<HW_TB_TCS_TRG_CLASS_BITPOS) 
HW_TB_TCS_FREEZE_SETMASK      equ     (((1<<HW_TB_TCS_FREEZE_WIDTH)-1)<<HW_TB_TCS_FREEZE_BITPOS) 
  
HW_TB_TCS_TRG_STYLE_CLRMASK    equ    (~HW_TB_TCS_TRG_STYLE_SETMASK)
HW_TB_TCS_CAP_CLASS_CLRMASK   equ     (~HW_TB_TCS_CAP_CLASS_SETMASK)
HW_TB_TCS_TRG_CLASS_CLRMASK   equ     (~HW_TB_TCS_TRG_CLASS_SETMASK)
HW_TB_TCS_FREEZE_CLRMASK      equ     (~HW_TB_TCS_FREEZE_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int TRG_STYLE       : HW_TB_TCS_TRG_STYLE_WIDTH;
;	 int CAP_CLASS       : HW_TB_TCS_CAP_CLASS_WIDTH;
;	 int TRG_CLASS       : HW_TB_TCS_TRG_CLASS_WIDTH;
;	 int FREEZE          : HW_TB_TCS_FREEZE_WIDTH;
;        int reserved        : HW_TB_TCS_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_tcs_type;

 

;#define HW_TB_TCS      (*(volatile tb_tcs_type _X*) (HW_TB_BASEADDR+16))    /* Trace Buffer Trigger Command Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Trace Buffer Trigger Value Register (HW_TB_TVR) Bit Definitions

HW_TB_TVR_MATCH_ADDR_BITPOS    equ     (0)

HW_TB_TVR_MATCH_ADDR_WIDTH     equ     (16)        
HW_TB_TVR_RSVD_WIDTH           equ     (8)
   
HW_TB_TVR_MATCH_ADDR_SETMASK   equ     (((1<<HW_TB_TVR_MATCH_ADDR_WIDTH)-1)<<HW_TB_TVR_MATCH_ADDR_BITPOS) 
  
HW_TB_TVR_MATCH_ADDR_CLRMASK   equ     (~HW_TB_TVR_MATCH_ADDR_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int MATCH_ADDR      : HW_TB_TVR_MATCH_ADDR_WIDTH;
;        int reserved        : HW_TB_TVR_RSVD_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} tb_tvr_type;

 

;#define HW_TB_TVR      (*(volatile tb_tvr_type _X*) (HW_TB_BASEADDR+24))    /* Trace Buffer Trigger Value Register */

;endc



    endif
















