;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regsusb20ip.inc
;;  Description : USB20 IP Register definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

    if !(@def(regsusb20inc))
regsusb20inc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   USB2.0 STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_USB_BASEADDR             equ     ($F200)

;ASM Only
HW_USBCSR                   equ     (HW_USB_BASEADDR)
HW_USBDMAOFF                equ     (HW_USB_BASEADDR+1)
HW_USBARCACCESS             equ     (HW_USB_BASEADDR+2) 
HW_USBARCDATALOW            equ     (HW_USB_BASEADDR+3)
HW_USBARCDATAHIGH           equ     (HW_USB_BASEADDR+4)
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  USB Control Status Register (HW_USBCSR) Bit Definitions
HW_USBCSR_USBEN_BITPOS                  equ     (0)
HW_USBCSR_WAKEUPIRQ_BITPOS              equ     (1)
HW_USBCSR_WAKEUPIE_BITPOS               equ     (2)
HW_USBCSR_VBUSCXIRQ_BITPOS             equ      (3)
HW_USBCSR_VBUSCXIE_BITPOS              equ      (4)
HW_USBCSR_VBUSDISCXIRQ_BITPOS           equ     (5)
HW_USBCSR_VBUSDISCXIE_BITPOS            equ     (6)
HW_USBCSR_CLKOFF_BITPOS                 equ     (7)
HW_USBCSR_SUSP_BITPOS                   equ     (8)
HW_USBCSR_SUSPF_BITPOS                  equ     (9)
HW_USBCSR_UTMITST_BITPOS                equ     (10)
HW_USBCSR_UTMI_EXT_BITPOS               equ     (11)
HW_USBCSR_PLUGGEDIN_EN_BITPOS			equ		(12)
HW_USBCSR_PLUGGEDIN_BITPOS				equ		(13)
HW_USBCSR_HOSTDISCONNECT_BITPOS			equ		(22)
HW_USBCSR_VBUSSENSE_BITPOS              equ     (23)

HW_USBCSR_USBEN_SETMASK                 equ     (1<<HW_USBCSR_USBEN_BITPOS)        
HW_USBCSR_WAKEUPIRQ_SETMASK             equ     (1<<HW_USBCSR_WAKEUPIRQ_BITPOS) 
HW_USBCSR_WAKEUPIE_SETMASK              equ     (1<<HW_USBCSR_WAKEUPIE_BITPOS)  
HW_USBCSR_VBUSCXIRQ_SETMASK             equ     (1<<HW_USBCSR_VBUSCXIRQ_BITPOS)
HW_USBCSR_VBUSCXIE_SETMASK              equ     (1<<HW_USBCSR_VBUSCXIE_BITPOS)
HW_USBCSR_VBUSDISCXIRQ_SETMASK          equ     (1<<HW_USBCSR_VBUSDISCXIRQ_BITPOS)
HW_USBCSR_VBUSDISCXIE_SETMASK           equ     (1<<HW_USBCSR_VBUSDISCXIE_BITPOS)
HW_USBCSR_CLKOFF_SETMASK                equ     (1<<HW_USBCSR_CLKOFF_BITPOS)    
HW_USBCSR_SUSP_SETMASK                  equ     (1<<HW_USBCSR_SUSP_BITPOS)      
HW_USBCSR_SUSPF_SETMASK                 equ     (1<<HW_USBCSR_SUSPF_BITPOS)     
HW_USBCSR_UTMITST_SETMASK               equ     (1<<HW_USBCSR_UTMITST_BITPOS)   
HW_USBCSR_UTMI_EXT_SETMASK              equ     (1<<HW_USBCSR_UTMI_EXT_BITPOS)
HW_USBCSR_VBUSSENSE_SETMASK             equ     (1<<HW_USBCSR_VBUSSENSE_BITPOS)


HW_USBCSR_USBEN_CLRMASK                 equ    (~HW_USBCSR_USBEN_SETMASK)     
HW_USBCSR_WAKEUPIRQ_CLRMASK             equ    (~HW_USBCSR_WAKEUPIRQ_SETMASK) 
HW_USBCSR_WAKEUPIE_CLRMASK              equ    (~HW_USBCSR_WAKEUPIE_SETMASK)  
HW_USBCSR_VBUSCXIRQ_CLRMASK             equ    (~HW_USBCSR_VBUSCXIRQ_SETMASK)
HW_USBCSR_VBUSCXIE_CLRMASK              equ    (~HW_USBCSR_VBUSCXIE_SETMASK)
HW_USBCSR_VBUSDISCXIRQ_CLRMASK          equ    (~HW_USBCSR_VBUSDISCXIRQ_SETMASK)
HW_USBCSR_VBUSDISCXIE_CLRMASK           equ    (~HW_USBCSR_VBUSDISCXIE_SETMASK) 
HW_USBCSR_CLKOFF_CLRMASK                equ    (~HW_USBCSR_CLKOFF_SETMASK)    
HW_USBCSR_SUSP_CLRMASK                  equ    (~HW_USBCSR_SUSP_SETMASK)      
HW_USBCSR_SUSPF_CLRMASK                 equ    (~HW_USBCSR_SUSPF_SETMASK)     
HW_USBCSR_UTMITST_CLRMASK               equ    (~HW_USBCSR_UTMITST_SETMASK)   
HW_USBCSR_UTMI_EXT_CLRMASK              equ    (~HW_USBCSR_UTMI_EXT_SETMASK) 
HW_USBCSR_VBUSSENSE_CLRMASK             equ    (~HW_USBCSR_VBUSSENSE_SETMASK) 

;C insert
;typedef union               
;{
;    struct {
;        int USBEN          :1;
;        int WAKEUPIRQ      :1;
;        int WAKEUPIE       :1;
;        int VBUSCXIRQ      :1;
;        int VBUSCXIE       :1;
;        int VBUSDISCXIRQ   :1;
;        int VBUSDISCXIE    :1;
;        int CLKOFF         :1;
;        int SUSP           :1;
;        int SUSPF          :1;
;        int UTMITST        :1;
;        int ARCCONNECT     :1;
;        int PLUGGEDIN_EN   :1;
;        int PLUGGEDIN      :1;
;        int                :8;
;        int HOSTDISCONNECT :1;
;        int VBUSSENSE      :1;

;    } B;
;    int I;
;} usbcsr_type;

;#define HW_USBCSR      (*(volatile usbcsr_type _X*) (HW_USB_BASEADDR))    /* USB Control / Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB DMA OFFSET register (HW_USBDMAOFF) Bit Definitions
HW_USBDMAOFF_MEM_BITPOS                 equ     (16)

HW_USBDMAOFF_ADD_SETMASK                equ     ($00FFFF)
HW_USBDMAOFF_MEM_SETMASK                equ     (3<<HW_USBDMAOFF_MEM_BITPOS)

HW_USBDMAOFF_ADD_CLRMASK                equ     (~HW_USBDMAOFF_ADD_SETMASK)
HW_USBDMAOFF_MEM_CLRMASK                equ     (~HW_USBDMAOFF_MEM_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int ADD            :16;
;        int MEM            :2;
;        int                :6;
;    } B;
;    int I;
;} usbdmaoff_type;

;#define HW_USBDMAOFF      (*(volatile usbdmaoff_type _X*) (HW_USB_BASEADDR+1))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC ACCESS register (HW_USBARCACCESS) Bit Definitions
HW_USBARCACCESS_RWB_BITPOS              equ     (16)
HW_USBARCACCESS_KICK_BITPOS             equ     (23)

HW_USBARCACCESS_ADD_SETMASK             equ     ($0001FF)
HW_USBARCACCESS_RWB_SETMASK             equ     (1<<HW_USBARCACCESS_RWB_BITPOS)
HW_USBARCACCESS_KICK_SETMASK            equ    (23<<HW_USBDMAOFF_MEM_BITPOS)

HW_USBARCACCESS_ADD_CLRMASK             equ    (~HW_USBARCACCESS_ADD_SETMASK)
HW_USBARCACCESS_RWB_CLRMASK             equ     (~HW_USBARCACCESS_RWB_SETMASK) 
HW_USBARCACCESS_KICK_CLRMASK            equ    (~HW_USBARCACCESS_KICK_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int ADD            :9;
;        int                :7;
;        int RWB            :1;
;        int                :14;
;        int KICK           :1;
;    } B;
;    int I;
;} usbarcaccess_type;

;#define HW_USBARCACCESS      (*(volatile usbarcaccess_type _X*) (HW_USB_BASEADDR+2))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC DATA LOW register (HW_USBARCDATALOW) Bit Definitions
HW_USBARCDATALOW_DATA_SETMASK           equ     ($00FFFF)

HW_USBARCDATALOW_ADD_CLRMASK            equ    (~HW_USBARCDATALOW_DATA_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DATA           :16;
;        int                :8;
;    } B;
;    int I;
;} usbarcdatalow_type;

;#define HW_USBARCDATALOW      (*(volatile usbarcdatalow_type _X*) (HW_USB_BASEADDR+3))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC DATA HIGH register (HW_USBARCDATAHIGH) Bit Definitions
HW_USBARCDATAHIGH_DATA_SETMASK          equ     ($00FFFF)

HW_USBARCDATAHIGH_ADD_CLRMASK           equ    (~HW_USBARCDATAHIGH_DATA_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DATA           :16;
;        int                :8;
;    } B;
;    int I;
;} usbarcdatahigh_type;

;#define HW_USBARCDATAHIGH     (*(volatile usbarcdatahigh_type _X*) (HW_USB_BASEADDR+4))    
;endc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   USB2.0 ARC Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_ARC_BASE_ADDR            equ     ($0000)

HW_ARC_HCSPARAMS            equ     (HW_ARC_BASE_ADDR+$104)
HW_ARC_USBCMD               equ     (HW_ARC_BASE_ADDR+$140)
HW_ARC_USBSTS               equ     (HW_ARC_BASE_ADDR+$144)
HW_ARC_USBINTR              equ     (HW_ARC_BASE_ADDR+$148)
HW_ARC_DEVADDR              equ     (HW_ARC_BASE_ADDR+$154)
HW_ARC_ENDPTLISTADDR        equ     (HW_ARC_BASE_ADDR+$158)
HW_ARC_PORTSC1              equ     (HW_ARC_BASE_ADDR+$184)
HW_ARC_USBMODE              equ     (HW_ARC_BASE_ADDR+$1a8)
HW_ARC_ENDPTSETUPSTAT       equ     (HW_ARC_BASE_ADDR+$1ac)
HW_ARC_ENDPTPRIME           equ     (HW_ARC_BASE_ADDR+$1b0)
HW_ARC_ENDPTFLUSH           equ     (HW_ARC_BASE_ADDR+$1b4)
HW_ARC_ENDPTSTATUS          equ     (HW_ARC_BASE_ADDR+$1b8)
HW_ARC_ENDPTCOMPLETE        equ     (HW_ARC_BASE_ADDR+$1bc)
HW_ARC_ENDPTCTRL0           equ     (HW_ARC_BASE_ADDR+$1c0)
HW_ARC_ENDPTCTRL1           equ     (HW_ARC_BASE_ADDR+$1c4)
HW_ARC_ENDPTCTRL2           equ     (HW_ARC_BASE_ADDR+$1c8)
HW_ARC_ENDPTCTRL3           equ     (HW_ARC_BASE_ADDR+$1cc)
HW_ARC_ENDPTCTRL4           equ     (HW_ARC_BASE_ADDR+$1d0)
HW_ARC_ENDPTCTRL5           equ     (HW_ARC_BASE_ADDR+$1d4)
HW_ARC_ENDPTCTRL6           equ     (HW_ARC_BASE_ADDR+$1d8)
HW_ARC_ENDPTCTRL7           equ     (HW_ARC_BASE_ADDR+$1dc)
HW_ARC_ENDPTCTRL8           equ     (HW_ARC_BASE_ADDR+$1e0)
HW_ARC_ENDPTCTRL9           equ     (HW_ARC_BASE_ADDR+$1e4)
HW_ARC_ENDPTCTRL10          equ     (HW_ARC_BASE_ADDR+$1e8)
HW_ARC_ENDPTCTRL11          equ     (HW_ARC_BASE_ADDR+$1ec)
HW_ARC_ENDPTCTRL12          equ     (HW_ARC_BASE_ADDR+$1f0)
HW_ARC_ENDPTCTRL13          equ     (HW_ARC_BASE_ADDR+$1f4)
HW_ARC_ENDPTCTRL14          equ     (HW_ARC_BASE_ADDR+$1f8)
HW_ARC_ENDPTCTRL15          equ     (HW_ARC_BASE_ADDR+$1fc)

;C insert
;#define HW_ARC_ENDPTCTRL(n) (HW_ARC_ENDPTCTRL0+((n)*4))    
;endc
;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register Host Control Structural Parameters (HW_ARC_HCSPARAMS)

HW_ARC_HCSPARAMS_NPORTS_BITPOS      equ     (0)
HW_ARC_HCSPARAMS_PPC_BITPOS         equ     (4)
HW_ARC_HCSPARAMS_NPCC_BITPOS        equ     (8)
HW_ARC_HCSPARAMS_NCC_BITPOS         equ     (12)
HW_ARC_HCSPARAMS_PI_BITPOS          equ     (16)
HW_ARC_HCSPARAMS_NPTT_BITPOS        equ     (20)
HW_ARC_HCSPARAMS_NTT_BITPOS         equ     (24)

HW_ARC_HCSPARAMS_NPORTS_SETMASK     equ     (15<<HW_ARC_HCSPARAMS_NPORTS_BITPOS)
HW_ARC_HCSPARAMS_PPC_SETMASK        equ     (1<<HW_ARC_HCSPARAMS_PPC_BITPOS)        
HW_ARC_HCSPARAMS_NPCC_SETMASK       equ     (15<<HW_ARC_HCSPARAMS_NPCC_BITPOS)  
HW_ARC_HCSPARAMS_NCC_SETMASK        equ     (15<<HW_ARC_HCSPARAMS_NCC_BITPOS)       
HW_ARC_HCSPARAMS_PI_SETMASK         equ     (1<<HW_ARC_HCSPARAMS_PI_BITPOS)     
HW_ARC_HCSPARAMS_NPTT_SETMASK       equ     (15<<HW_ARC_HCSPARAMS_NPTT_BITPOS)  
HW_ARC_HCSPARAMS_NTT_SETMASK        equ     (15<<HW_ARC_HCSPARAMS_NTT_BITPOS)       

HW_ARC_HCSPARAMS_NPORTS_CLRMASK     equ     (~HW_ARC_HCSPARAMS_NPORTS_SETMASK)
HW_ARC_HCSPARAMS_PPC_CLRMASK        equ     (~HW_ARC_HCSPARAMS_PPC_SETMASK)
HW_ARC_HCSPARAMS_NPCC_CLRMASK       equ     (~HW_ARC_HCSPARAMS_NPCC_SETMASK)
HW_ARC_HCSPARAMS_NCC_CLRMASK        equ     (~HW_ARC_HCSPARAMS_NCC_SETMASK)
HW_ARC_HCSPARAMS_PI_CLRMASK         equ     (~HW_ARC_HCSPARAMS_PI_SETMASK)  
HW_ARC_HCSPARAMS_NPTT_CLRMASK       equ     (~HW_ARC_HCSPARAMS_NPTT_SETMASK)
HW_ARC_HCSPARAMS_NTT_CLRMASK        equ     (~HW_ARC_HCSPARAMS_NTT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int N_PORTS         :4;
;        int PPC             :1;
;        int                 :3;
;        int N_PCC           :4;
;        int N_CC            :4;
;        int PI              :1;
;        int                 :3;
;        int N_PTT           :4;
;        int N_TT            :4;
;        int                 :20;
;    } B;
;    DWORD I;
;} hcsparams_type;

;// #define HW_ARC_HCSPARAMS (*(volatile hcsparams_type _X*) (HW_ARC_BASEADDR))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Command (HW_ARC_USBCMD)

HW_ARC_USBCMD_RS_BITPOS         equ     (0)
HW_ARC_USBCMD_RST_BITPOS        equ     (1)
HW_ARC_USBCMD_FS0_BITPOS        equ     (2)
HW_ARC_USBCMD_FS1_BITPOS        equ     (3)
HW_ARC_USBCMD_PSE_BITPOS        equ     (4)
HW_ARC_USBCMD_ASE_BITPOS        equ     (5)
HW_ARC_USBCMD_IAA_BITPOS        equ     (6)
HW_ARC_USBCMD_LR_BITPOS         equ     (7)
HW_ARC_USBCMD_ASP0_BITPOS       equ     (8)
HW_ARC_USBCMD_ASP1_BITPOS       equ     (9)
HW_ARC_USBCMD_ASPE_BITPOS       equ     (11)
HW_ARC_USBCMD_FS2_BITPOS        equ     (15)
HW_ARC_USBCMD_ITC_BITPOS        equ     (16)

HW_ARC_USBCMD_RS_SETMASK        equ     (1<<HW_ARC_USBCMD_RS_BITPOS)                
HW_ARC_USBCMD_RST_SETMASK       equ     (1<<HW_ARC_USBCMD_RST_BITPOS)   
HW_ARC_USBCMD_FS0_SETMASK       equ     (1<<HW_ARC_USBCMD_FS0_BITPOS)   
HW_ARC_USBCMD_FS1_SETMASK       equ     (1<<HW_ARC_USBCMD_FS1_BITPOS)   
HW_ARC_USBCMD_PSE_SETMASK       equ     (1<<HW_ARC_USBCMD_PSE_BITPOS)   
HW_ARC_USBCMD_ASE_SETMASK       equ     (1<<HW_ARC_USBCMD_ASE_BITPOS)   
HW_ARC_USBCMD_IAA_SETMASK       equ     (1<<HW_ARC_USBCMD_IAA_BITPOS)   
HW_ARC_USBCMD_LR_SETMASK        equ     (1<<HW_ARC_USBCMD_LR_BITPOS)        
HW_ARC_USBCMD_ASP0_SETMASK      equ     (1<<HW_ARC_USBCMD_ASP0_BITPOS)
HW_ARC_USBCMD_ASP1_SETMASK      equ     (1<<HW_ARC_USBCMD_ASP1_BITPOS)
HW_ARC_USBCMD_ASPE_SETMASK      equ     (1<<HW_ARC_USBCMD_ASPE_BITPOS)
HW_ARC_USBCMD_FS2_SETMASK       equ     (1<<HW_ARC_USBCMD_FS2_BITPOS)    
HW_ARC_USBCMD_ITC_SETMASK       equ     (255<<HW_ARC_USBCMD_ITC_BITPOS)

HW_ARC_USBCMD_RS_CLRMASK        equ     (~HW_ARC_USBCMD_RS_SETMASK)     
HW_ARC_USBCMD_RST_CLRMASK       equ     (~HW_ARC_USBCMD_RST_SETMASK)    
HW_ARC_USBCMD_FS0_CLRMASK       equ     (~HW_ARC_USBCMD_FS0_SETMASK)    
HW_ARC_USBCMD_FS1_CLRMASK       equ     (~HW_ARC_USBCMD_FS1_SETMASK)    
HW_ARC_USBCMD_PSE_CLRMASK       equ     (~HW_ARC_USBCMD_PSE_SETMASK)    
HW_ARC_USBCMD_ASE_CLRMASK       equ     (~HW_ARC_USBCMD_ASE_SETMASK)    
HW_ARC_USBCMD_IAA_CLRMASK       equ     (~HW_ARC_USBCMD_IAA_SETMASK)    
HW_ARC_USBCMD_LR_CLRMASK        equ     (~HW_ARC_USBCMD_LR_SETMASK) 
HW_ARC_USBCMD_ASP0_CLRMASK      equ     (~HW_ARC_USBCMD_ASP0_SETMASK)
HW_ARC_USBCMD_ASP1_CLRMASK      equ     (~HW_ARC_USBCMD_ASP1_SETMASK)
HW_ARC_USBCMD_ASPE_CLRMASK      equ     (~HW_ARC_USBCMD_ASPE_SETMASK)
HW_ARC_USBCMD_FS2_CLRMASK       equ     (~HW_ARC_USBCMD_FS2_SETMASK)    
HW_ARC_USBCMD_ITC_CLRMASK       equ     (~HW_ARC_USBCMD_ITC_SETMASK)    

;C insert
;typedef union               
;{
;    struct {
;        int RS              :1;
;        int RST             :1;
;        int FS0             :1;
;        int FS1             :1;
;        int PSE             :1;
;        int ASE             :1;
;        int IAA             :1;
;        int LR              :1;
;        int ASP0            :1;
;        int ASP1            :1;
;        int                 :1;
;        int ASPE            :1;
;        int                 :3;
;        int FS2             :1;
;        int ITC             :8;
;        int                 :24;
;    } B;
;    DWORD I;
;} usbcmd_type;

;//#define HW_ARC_USBCMD ((volatile usbcmd_type _X*) (HW_ARC_BASEADDR+0x140))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Status (HW_ARC_USBSTS)

HW_ARC_USBSTS_UI_BITPOS         equ     (0)
HW_ARC_USBSTS_UEI_BITPOS        equ     (1)
HW_ARC_USBSTS_PCI_BITPOS        equ     (2)
HW_ARC_USBSTS_FRI_BITPOS        equ     (3)
HW_ARC_USBSTS_SEI_BITPOS        equ     (4)
HW_ARC_USBSTS_AAI_BITPOS        equ     (5)
HW_ARC_USBSTS_URI_BITPOS        equ     (6)
HW_ARC_USBSTS_SRI_BITPOS        equ     (7)
HW_ARC_USBSTS_SLI_BITPOS        equ     (8)
HW_ARC_USBSTS_HCH_BITPOS        equ     (12)
HW_ARC_USBSTS_RCL_BITPOS        equ     (13)
HW_ARC_USBSTS_PS_BITPOS         equ     (14)
HW_ARC_USBSTS_AS_BITPOS         equ     (15)

HW_ARC_USBSTS_UI_SETMASK        equ     (1<<HW_ARC_USBSTS_UI_BITPOS)    
HW_ARC_USBSTS_UEI_SETMASK       equ     (1<<HW_ARC_USBSTS_UEI_BITPOS)
HW_ARC_USBSTS_PCI_SETMASK       equ     (1<<HW_ARC_USBSTS_PCI_BITPOS)
HW_ARC_USBSTS_FRI_SETMASK       equ     (1<<HW_ARC_USBSTS_FRI_BITPOS)
HW_ARC_USBSTS_SEI_SETMASK       equ     (1<<HW_ARC_USBSTS_SEI_BITPOS)
HW_ARC_USBSTS_AAI_SETMASK       equ     (1<<HW_ARC_USBSTS_AAI_BITPOS)
HW_ARC_USBSTS_URI_SETMASK       equ     (1<<HW_ARC_USBSTS_URI_BITPOS)
HW_ARC_USBSTS_SRI_SETMASK       equ     (1<<HW_ARC_USBSTS_SRI_BITPOS)
HW_ARC_USBSTS_SLI_SETMASK       equ     (1<<HW_ARC_USBSTS_SLI_BITPOS)
HW_ARC_USBSTS_HCH_SETMASK       equ     (1<<HW_ARC_USBSTS_HCH_BITPOS)
HW_ARC_USBSTS_RCL_SETMASK       equ     (1<<HW_ARC_USBSTS_RCL_BITPOS)
HW_ARC_USBSTS_PS_SETMASK        equ     (1<<HW_ARC_USBSTS_PS_BITPOS)    
HW_ARC_USBSTS_AS_SETMASK        equ     (1<<HW_ARC_USBSTS_AS_BITPOS)    

HW_ARC_USBSTS_UI_CLRMASK        equ     (~HW_ARC_USBSTS_UI_SETMASK)
HW_ARC_USBSTS_UEI_CLRMASK       equ     (~HW_ARC_USBSTS_UEI_SETMASK)
HW_ARC_USBSTS_PCI_CLRMASK       equ     (~HW_ARC_USBSTS_PCI_SETMASK)
HW_ARC_USBSTS_FRI_CLRMASK       equ     (~HW_ARC_USBSTS_FRI_SETMASK)
HW_ARC_USBSTS_SEI_CLRMASK       equ     (~HW_ARC_USBSTS_SEI_SETMASK)
HW_ARC_USBSTS_AAI_CLRMASK       equ     (~HW_ARC_USBSTS_AAI_SETMASK)
HW_ARC_USBSTS_URI_CLRMASK       equ     (~HW_ARC_USBSTS_URI_SETMASK)
HW_ARC_USBSTS_SRI_CLRMASK       equ     (~HW_ARC_USBSTS_SRI_SETMASK)
HW_ARC_USBSTS_SLI_CLRMASK       equ     (~HW_ARC_USBSTS_SLI_SETMASK)
HW_ARC_USBSTS_HCH_CLRMASK       equ     (~HW_ARC_USBSTS_HCH_SETMASK)
HW_ARC_USBSTS_RCL_CLRMASK       equ     (~HW_ARC_USBSTS_RCL_SETMASK)
HW_ARC_USBSTS_PS_CLRMASK        equ     (~HW_ARC_USBSTS_PS_SETMASK)
HW_ARC_USBSTS_AS_CLRMASK        equ     (~HW_ARC_USBSTS_AS_SETMASK)


;C insert
;typedef union               
;{
;    struct {
;        int UI              :1;
;        int UEI             :1;
;        int PCI             :1;
;        int FRI             :1;
;        int SEI             :1;
;        int AAI             :1;
;        int URI             :1;
;        int STI             :1;
;        int SLI             :1;
;        int                 :3;
;        int HCH             :1;
;        int RCL             :1;
;        int PS              :1;
;        int AS              :1;
;        int                 :24;
;    } B;
;    DWORD I;
;} usbsts_type;

;//#define HW_ARC_USBSTS (*(volatile usbsts_type _X*) (HW_ARC_BASEADDR+0x144))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Interrupt Enable (HW_ARC_USBINTR)

HW_ARC_USBINTR_UE_BITPOS        equ     (0)
HW_ARC_USBINTR_UEE_BITPOS       equ     (1)
HW_ARC_USBINTR_PCE_BITPOS       equ     (2)
HW_ARC_USBINTR_FRE_BITPOS       equ     (3)
HW_ARC_USBINTR_SEE_BITPOS       equ     (4)
HW_ARC_USBINTR_AAE_BITPOS       equ     (5)
HW_ARC_USBINTR_URE_BITPOS       equ     (6)
HW_ARC_USBINTR_SRE_BITPOS       equ     (7)
HW_ARC_USBINTR_SLE_BITPOS       equ     (8)

HW_ARC_USBINTR_UE_SETMASK       equ     (1<<HW_ARC_USBINTR_UE_BITPOS)   
HW_ARC_USBINTR_UEE_SETMASK      equ     (1<<HW_ARC_USBINTR_UEE_BITPOS)
HW_ARC_USBINTR_PCE_SETMASK      equ     (1<<HW_ARC_USBINTR_PCE_BITPOS)
HW_ARC_USBINTR_FRE_SETMASK      equ     (1<<HW_ARC_USBINTR_FRE_BITPOS)
HW_ARC_USBINTR_SEE_SETMASK      equ     (1<<HW_ARC_USBINTR_SEE_BITPOS)
HW_ARC_USBINTR_AAE_SETMASK      equ     (1<<HW_ARC_USBINTR_AAE_BITPOS)
HW_ARC_USBINTR_URE_SETMASK      equ     (1<<HW_ARC_USBINTR_URE_BITPOS)
HW_ARC_USBINTR_SRE_SETMASK      equ     (1<<HW_ARC_USBINTR_SRE_BITPOS)
HW_ARC_USBINTR_SLE_SETMASK      equ     (1<<HW_ARC_USBINTR_SLE_BITPOS)

HW_ARC_USBINTR_UE_CLRMASK       equ     (~HW_ARC_USBINTR_UE_SETMASK)
HW_ARC_USBINTR_UEE_CLRMASK      equ     (~HW_ARC_USBINTR_UEE_SETMASK)
HW_ARC_USBINTR_PCE_CLRMASK      equ     (~HW_ARC_USBINTR_PCE_SETMASK)
HW_ARC_USBINTR_FRE_CLRMASK      equ     (~HW_ARC_USBINTR_FRE_SETMASK)
HW_ARC_USBINTR_SEE_CLRMASK      equ     (~HW_ARC_USBINTR_SEE_SETMASK)
HW_ARC_USBINTR_AAE_CLRMASK      equ     (~HW_ARC_USBINTR_AAE_SETMASK)
HW_ARC_USBINTR_URE_CLRMASK      equ     (~HW_ARC_USBINTR_URE_SETMASK)
HW_ARC_USBINTR_SRE_CLRMASK      equ     (~HW_ARC_USBINTR_SRE_SETMASK)
HW_ARC_USBINTR_SLE_CLRMASK      equ     (~HW_ARC_USBINTR_SLE_SETMASK)


;C insert
;typedef union               
;{
;    struct {
;        int UE              :1;
;        int UEE             :1;
;        int PCE             :1;
;        int FRE             :1;
;        int SEE             :1;
;        int AAE             :1;
;        int URE             :1;
;        int STE             :1;
;        int SLE             :1;
;        int                 :39;
;    } B;
;    DWORD I;
;} usbintr_type;

;//#define HW_ARC_USBINTR (*(volatile usbintr_type _X*) (HW_ARC_BASEADDR+0x148))    
;endc


;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Device Controller Device Address (HW_ARC_DEVADDR)

HW_ARC_DEVADDR_ADD_BITPOS       equ     (25)

HW_ARC_DEVADDR_ADD_SETMASK      equ     (127<<HW_ARC_DEVADDR_ADD_BITPOS)    

HW_ARC_DEVEADDR_ADD_CLRMASK     equ     (~HW_ARC_DEVADDR_ADD_SETMASK)   

;C insert
;typedef union               
;{
;    struct {
;        int                 :25;
;        int ADD             :7;
;        int                 :16;
;    } B;
;    DWORD I;
;} devaddr_type;

;//#define HW_ARC_DEVADDR (*(volatile devaddr_type _X*) (HW_ARC_BASEADDR+0x154))    
;endc


;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Device Controller Endpoint List Address (HW_ARC_ENDPTLISTADDR)

HW_ARC_ENDPTLISTADDR_ADD_BITPOS     equ     (11)

HW_ARC_ENDPTLISTADDR_ADD_SETMASK    equ     ($400000<<HW_ARC_ENDPTLISTADDR_ADD_BITPOS) 

HW_ARC_ENDPTLISTADDR_ADD_CLRMASK    equ     (~HW_ARC_ENDPTLISTADDR_ADD_SETMASK) 

;C insert
;typedef union               
;{
;    struct {
;        int                 :10;
;        int ADD             :22;
;        int                 :16;
;    } B;
;    DWORD I;
;} endptlistaddr_type;

;//#define HW_ARC_ENDPTLISTADDR (*(volatile endptlistaddr_type _X*) (HW_ARC_BASEADDR+0x158))    
;endc


;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Port Status Control 1 (HW_ARC_PORTSC1)

HW_ARC_PORTSC1_CCS_BITPOS       equ     (0)
HW_ARC_PORTSC1_CSC_BITPOS       equ     (1)
HW_ARC_PORTSC1_PE_BITPOS        equ     (2)
HW_ARC_PORTSC1_PEC_BITPOS       equ     (3)
HW_ARC_PORTSC1_OCA_BITPOS       equ     (4)
HW_ARC_PORTSC1_OCC_BITPOS       equ     (5)
HW_ARC_PORTSC1_FPR_BITPOS       equ     (6)
HW_ARC_PORTSC1_SUSP_BITPOS      equ     (7)
HW_ARC_PORTSC1_PR_BITPOS        equ     (8)
HW_ARC_PORTSC1_HSP_BITPOS       equ     (9)
HW_ARC_PORTSC1_LS_BITPOS        equ     (10)
HW_ARC_PORTSC1_PP_BITPOS        equ     (12)
HW_ARC_PORTSC1_PO_BITPOS        equ     (13)
HW_ARC_PORTSC1_PIC_BITPOS       equ     (14)
HW_ARC_PORTSC1_PTC_BITPOS       equ     (16)
HW_ARC_PORTSC1_WKCN_BITPOS      equ     (20)
HW_ARC_PORTSC1_WKDS_BITPOS      equ     (21)
HW_ARC_PORTSC1_WKOC_BITPOS      equ     (22)
HW_ARC_PORTSC1_PHCD_BITPOS      equ     (23)
HW_ARC_PORTSC1_PFSC_BITPOS      equ     (24)
HW_ARC_PORTSC1_PSPD_BITPOS      equ     (26)
HW_ARC_PORTSC1_PTW_BITPOS       equ     (29)
HW_ARC_PORTSC1_STS_BITPOS       equ     (30)
HW_ARC_PORTSC1_PTS_BITPOS       equ     (31)

HW_ARC_PORTSC1_CCS_SETMASK      equ     (1<<HW_ARC_PORTSC1_CCS_BITPOS)  
HW_ARC_PORTSC1_CSC_SETMASK      equ     (1<<HW_ARC_PORTSC1_CSC_BITPOS)  
HW_ARC_PORTSC1_PE_SETMASK       equ     (1<<HW_ARC_PORTSC1_PE_BITPOS)   
HW_ARC_PORTSC1_PEC_SETMASK      equ     (1<<HW_ARC_PORTSC1_PEC_BITPOS)  
HW_ARC_PORTSC1_OCA_SETMASK      equ     (1<<HW_ARC_PORTSC1_OCA_BITPOS)  
HW_ARC_PORTSC1_OCC_SETMASK      equ     (1<<HW_ARC_PORTSC1_OCC_BITPOS)  
HW_ARC_PORTSC1_FPR_SETMASK      equ     (1<<HW_ARC_PORTSC1_FPR_BITPOS)  
HW_ARC_PORTSC1_SUSP_SETMASK     equ     (1<<HW_ARC_PORTSC1_SUSP_BITPOS)
HW_ARC_PORTSC1_PR_SETMASK       equ     (1<<HW_ARC_PORTSC1_PR_BITPOS)   
HW_ARC_PORTSC1_HSP_SETMASK      equ     (1<<HW_ARC_PORTSC1_HSP_BITPOS)  
HW_ARC_PORTSC1_LS_SETMASK       equ     (3<<HW_ARC_PORTSC1_LS_BITPOS)   
HW_ARC_PORTSC1_PP_SETMASK       equ     (1<<HW_ARC_PORTSC1_PP_BITPOS)   
HW_ARC_PORTSC1_PO_SETMASK       equ     (1<<HW_ARC_PORTSC1_PO_BITPOS)   
HW_ARC_PORTSC1_PIC_SETMASK      equ     (3<<HW_ARC_PORTSC1_PIC_BITPOS)  
HW_ARC_PORTSC1_PTC_SETMASK      equ     (15<<HW_ARC_PORTSC1_PTC_BITPOS) 
HW_ARC_PORTSC1_WKCN_SETMASK     equ     (1<<HW_ARC_PORTSC1_WKCN_BITPOS)
HW_ARC_PORTSC1_WKDS_SETMASK     equ     (1<<HW_ARC_PORTSC1_WKDS_BITPOS)
HW_ARC_PORTSC1_WKOC_SETMASK     equ     (1<<HW_ARC_PORTSC1_WKOC_BITPOS)
HW_ARC_PORTSC1_PHCD_SETMASK     equ     (1<<HW_ARC_PORTSC1_PHCD_BITPOS)

; We need to equate the following label like this due to a sign extension problem
; if equated like so (1<<HW_ARC_PORTSC1_PFSC_SETMASK)
HW_ARC_PORTSC1_PFSC_SETMASK     equ     ($01000000)

HW_ARC_PORTSC1_PSPD_SETMASK     equ     (3<<HW_ARC_PORTSC1_PSPD_BITPOS)
HW_ARC_PORTSC1_PTW_SETMASK      equ     (1<<HW_ARC_PORTSC1_PTW_BITPOS)  
HW_ARC_PORTSC1_STS_SETMASK      equ     (1<<HW_ARC_PORTSC1_STS_BITPOS)  
HW_ARC_PORTSC1_PTS_SETMASK      equ     (1<<HW_ARC_PORTSC1_PTS_BITPOS)  

HW_ARC_PORTSC1_CCS_CLRMASK      equ     (~HW_ARC_PORTSC1_CCS_SETMASK)   
HW_ARC_PORTSC1_CSC_CLRMASK      equ     (~HW_ARC_PORTSC1_CSC_SETMASK)   
HW_ARC_PORTSC1_PE_CLRMASK       equ     (~HW_ARC_PORTSC1_PE_SETMASK)    
HW_ARC_PORTSC1_PEC_CLRMASK      equ     (~HW_ARC_PORTSC1_PEC_SETMASK)   
HW_ARC_PORTSC1_OCA_CLRMASK      equ     (~HW_ARC_PORTSC1_OCA_SETMASK)   
HW_ARC_PORTSC1_OCC_CLRMASK      equ     (~HW_ARC_PORTSC1_OCC_SETMASK)   
HW_ARC_PORTSC1_FPR_CLRMASK      equ     (~HW_ARC_PORTSC1_FPR_SETMASK)   
HW_ARC_PORTSC1_SUSP_CLRMASK     equ     (~HW_ARC_PORTSC1_SUSP_SETMASK)
HW_ARC_PORTSC1_PR_CLRMASK       equ     (~HW_ARC_PORTSC1_PR_SETMASK)    
HW_ARC_PORTSC1_HSP_CLRMASK      equ     (~HW_ARC_PORTSC1_HSP_SETMASK)   
HW_ARC_PORTSC1_LS_CLRMASK       equ     (~HW_ARC_PORTSC1_LS_SETMASK)    
HW_ARC_PORTSC1_PP_CLRMASK       equ     (~HW_ARC_PORTSC1_PP_SETMASK)    
HW_ARC_PORTSC1_PO_CLRMASK       equ     (~HW_ARC_PORTSC1_PO_SETMASK)    
HW_ARC_PORTSC1_PIC_CLRMASK      equ     (~HW_ARC_PORTSC1_PIC_SETMASK)   
HW_ARC_PORTSC1_PTC_CLRMASK      equ     (~HW_ARC_PORTSC1_PTC_SETMASK)   
HW_ARC_PORTSC1_WKCN_CLRMASK     equ     (~HW_ARC_PORTSC1_WKCN_SETMASK)
HW_ARC_PORTSC1_WKDS_CLRMASK     equ     (~HW_ARC_PORTSC1_WKDS_SETMASK)
HW_ARC_PORTSC1_WKOC_CLRMASK     equ     (~HW_ARC_PORTSC1_WKOC_SETMASK)
HW_ARC_PORTSC1_PHCD_CLRMASK     equ     (~HW_ARC_PORTSC1_PHCD_SETMASK)
HW_ARC_PORTSC1_PFSC_CLRMASK     equ     (~HW_ARC_PORTSC1_PFSC_SETMASK)
HW_ARC_PORTSC1_PSPD_CLRMASK     equ     (~HW_ARC_PORTSC1_PSPD_SETMASK)
HW_ARC_PORTSC1_PTW_CLRMASK      equ     (~HW_ARC_PORTSC1_PTW_SETMASK)   
HW_ARC_PORTSC1_STS_CLRMASK      equ     (~HW_ARC_PORTSC1_STS_SETMASK)   
HW_ARC_PORTSC1_PTS_CLRMASK      equ     (~HW_ARC_PORTSC1_PTS_SETMASK)   

;C insert
;typedef union               
;{
;    struct {
;        int CCS             :1;
;        int CSC             :1;
;        int PE              :1;
;        int PEC             :1;
;        int OCA             :1;
;        int OCC             :1;
;        int FPR             :1;
;        int SUSP            :1;
;        int PR              :1;
;        int HSP             :1;
;        int LS              :2;
;        int PP              :1;
;        int PO              :1;
;        int PIC             :2;
;        int PTC             :4;
;        int WKCN            :1;
;        int WKDS            :1;
;        int WKOC            :1;
;        int PHCD            :1;
;        int PFSC            :1;
;        int                 :1;
;        int PSPD            :2;
;        int                 :1;
;        int PTW             :1;
;        int STS             :1;
;        int PTS             :1;
;        int                 :16;
;    } B;
;    DWORD I;
;} portsc1_type;

;//#define HW_ARC_PORTSC1 (*(volatile portsc1_type _X*) (HW_ARC_BASEADDR+0x184))    
;endc


;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register USB Device Mode (HW_ARC_USBMODE)

HW_ARC_USBMODE_CM_BITPOS        equ     (0)
HW_ARC_USBMODE_ES_BITPOS        equ     (2)

HW_ARC_USBMODE_CM_SETMASK       equ     (3<<HW_ARC_USBMODE_CM_BITPOS)   
HW_ARC_USBMODE_ES_SETMASK       equ     (1<<HW_ARC_USBMODE_ES_BITPOS)   

HW_ARC_USBMODE_CM_CLRMASK       equ     (~HW_ARC_USBMODE_CM_SETMASK) 
HW_ARC_USBMODE_ES_CLRMASK       equ     (~HW_ARC_USBMODE_ES_SETMASK)    

;C insert
;typedef union               
;{
;    struct {
;        int CM              :2;
;        int ES              :1;
;        int                 :46;
;    } B;
;    DWORD I;
;} usbmode_type;

;//#define HW_ARC_USBMODE (*(volatile usbmode_type _X*) (HW_ARC_BASEADDR+0x1a8))    
;endc


;///////////////////////////////////////////////////////////////////////////////
;  The following endpoint equates are common for the following registers

ENDPOINT0_BITPOS        equ     (0)
ENDPOINT1_BITPOS        equ     (1)
ENDPOINT2_BITPOS        equ     (2)
ENDPOINT3_BITPOS        equ     (3)
ENDPOINT4_BITPOS        equ     (4)
ENDPOINT5_BITPOS        equ     (5)
ENDPOINT6_BITPOS        equ     (6)
ENDPOINT7_BITPOS        equ     (7)
ENDPOINT8_BITPOS        equ     (8)
ENDPOINT9_BITPOS        equ     (9)
ENDPOINT10_BITPOS       equ     (10)
ENDPOINT11_BITPOS       equ     (11)
ENDPOINT12_BITPOS       equ     (12)
ENDPOINT13_BITPOS       equ     (13)
ENDPOINT14_BITPOS       equ     (14)
ENDPOINT15_BITPOS       equ     (15)

ENDPOINT0_SETMASK       equ     (1<<ENDPOINT0_BITPOS)
ENDPOINT1_SETMASK       equ     (1<<ENDPOINT1_BITPOS)
ENDPOINT2_SETMASK       equ     (1<<ENDPOINT2_BITPOS)
ENDPOINT3_SETMASK       equ     (1<<ENDPOINT3_BITPOS)
ENDPOINT4_SETMASK       equ     (1<<ENDPOINT4_BITPOS)
ENDPOINT5_SETMASK       equ     (1<<ENDPOINT5_BITPOS)
ENDPOINT6_SETMASK       equ     (1<<ENDPOINT6_BITPOS)
ENDPOINT7_SETMASK       equ     (1<<ENDPOINT7_BITPOS)
ENDPOINT8_SETMASK       equ     (1<<ENDPOINT8_BITPOS)
ENDPOINT9_SETMASK       equ     (1<<ENDPOINT9_BITPOS)
ENDPOINT10_SETMASK      equ     (1<<ENDPOINT10_BITPOS)
ENDPOINT11_SETMASK      equ     (1<<ENDPOINT11_BITPOS)
ENDPOINT12_SETMASK      equ     (1<<ENDPOINT12_BITPOS)
ENDPOINT13_SETMASK      equ     (1<<ENDPOINT13_BITPOS)
ENDPOINT14_SETMASK      equ     (1<<ENDPOINT14_BITPOS)
ENDPOINT15_SETMASK      equ     (1<<ENDPOINT15_BITPOS)

ENDPOINT0_CLRMASK       equ     (~ENDPOINT0_SETMASK)    
ENDPOINT1_CLRMASK       equ     (~ENDPOINT1_SETMASK)    
ENDPOINT2_CLRMASK       equ     (~ENDPOINT2_SETMASK)    
ENDPOINT3_CLRMASK       equ     (~ENDPOINT3_SETMASK)    
ENDPOINT4_CLRMASK       equ     (~ENDPOINT4_SETMASK)    
ENDPOINT5_CLRMASK       equ     (~ENDPOINT5_SETMASK)    
ENDPOINT6_CLRMASK       equ     (~ENDPOINT6_SETMASK)    
ENDPOINT7_CLRMASK       equ     (~ENDPOINT7_SETMASK)    
ENDPOINT8_CLRMASK       equ     (~ENDPOINT8_SETMASK)    
ENDPOINT9_CLRMASK       equ     (~ENDPOINT9_SETMASK)    
ENDPOINT10_CLRMASK      equ     (~ENDPOINT10_SETMASK)
ENDPOINT11_CLRMASK      equ     (~ENDPOINT11_SETMASK)
ENDPOINT12_CLRMASK      equ     (~ENDPOINT12_SETMASK)
ENDPOINT13_CLRMASK      equ     (~ENDPOINT13_SETMASK)
ENDPOINT14_CLRMASK      equ     (~ENDPOINT14_SETMASK)
ENDPOINT15_CLRMASK      equ     (~ENDPOINT15_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int EP0              :1;
;        int EP1              :1;
;        int EP2              :1;
;        int EP3              :1;
;        int EP4              :1;
;        int EP5              :1;
;        int EP6              :1;
;        int EP7              :1;
;        int EP8              :1;
;        int EP9              :1;
;        int EP10             :1;
;        int EP11             :1;
;        int EP12             :1;
;        int EP13             :1;
;        int EP14             :1;
;        int EP15             :1;
;        int                  :32;
;    } B;
;    DWORD I;
;} endpsetupstat_type;
;
;//#define HW_ARC_ENDPTSETUPSTAT (*(volatile endpsetupstat_type _X*) (HW_ARC_BASEADDR+0x1ac))    
;
;typedef union               
;{
;    struct {
;        int EP0              :1;
;        int EP1              :1;
;        int EP2              :1;
;        int EP3              :1;
;        int EP4              :1;
;        int EP5              :1;
;        int EP6              :1;
;        int EP7              :1;
;        int EP8              :1;
;        int EP9              :1;
;        int EP10             :1;
;        int EP11             :1;
;        int EP12             :1;
;        int EP13             :1;
;        int EP14             :1;
;        int EP15             :1;
;        int                  :8;
;    } B;
;    WORD I;
;} endpt_type;
;
;typedef union
;{
;   struct {
;       endpt_type  RX;
;       endpt_type  TX;
;   } W;
;   DWORD DW;
;} endptrxtx_type;
;
;//#define HW_ARC_ENDPTPRIME    (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b0))    
;//#define HW_ARC_ENDPTFLUSH    (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b4))    
;//#define HW_ARC_ENDPTSTAT     (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b8))    
;//#define HW_ARC_ENDPTCOMPLETE (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1bc))    
;
;endc


;///////////////////////////////////////////////////////////////////////////////
;  USB ARC Register Endpoint control (HW_ARC_ENDPTCTRL)

HW_ARC_ENDPTCTRL_RXS_BITPOS     equ     (0)
HW_ARC_ENDPTCTRL_RXD_BITPOS     equ     (1)
HW_ARC_ENDPTCTRL_RXT_BITPOS     equ     (2)
HW_ARC_ENDPTCTRL_RXI_BITPOS     equ     (5)
HW_ARC_ENDPTCTRL_RXR_BITPOS     equ     (6)
HW_ARC_ENDPTCTRL_RXE_BITPOS     equ     (7)
HW_ARC_ENDPTCTRL_TXS_BITPOS     equ     (16)
HW_ARC_ENDPTCTRL_TXD_BITPOS     equ     (17)
HW_ARC_ENDPTCTRL_TXT_BITPOS     equ     (18)
HW_ARC_ENDPTCTRL_TXI_BITPOS     equ     (21)
HW_ARC_ENDPTCTRL_TXR_BITPOS     equ     (22)
HW_ARC_ENDPTCTRL_TXE_BITPOS     equ     (23)

HW_ARC_ENDPTCTRL_RXS_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_RXS_BITPOS)
HW_ARC_ENDPTCTRL_RXD_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_RXD_BITPOS)
HW_ARC_ENDPTCTRL_RXT_SETMASK    equ     (3<<HW_ARC_ENDPTCTRL_RXT_BITPOS)
HW_ARC_ENDPTCTRL_RXI_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_RXI_BITPOS)
HW_ARC_ENDPTCTRL_RXR_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_RXR_BITPOS)
HW_ARC_ENDPTCTRL_RXE_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_RXE_BITPOS)
HW_ARC_ENDPTCTRL_TXS_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXS_BITPOS)
HW_ARC_ENDPTCTRL_TXD_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXD_BITPOS)
HW_ARC_ENDPTCTRL_TXT_SETMASK    equ     (3<<HW_ARC_ENDPTCTRL_TXT_BITPOS)
HW_ARC_ENDPTCTRL_TXI_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXI_BITPOS)
HW_ARC_ENDPTCTRL_TXR_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXR_BITPOS)

; We need to equate the following label like this due to a sign extension problem
; if equated like so (1<<HW_ARC_ENDPTCTRL_TXE_BITPOS)
HW_ARC_ENDPTCTRL_TXE_SETMASK    equ     ($00800000)
;HW_ARC_ENDPTCTRL_TXE_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXE_BITPOS)

HW_ARC_ENDPTCTRL_RXS_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXS_SETMASK)
HW_ARC_ENDPTCTRL_RXD_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXD_SETMASK)
HW_ARC_ENDPTCTRL_RXT_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXT_SETMASK)
HW_ARC_ENDPTCTRL_RXI_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXI_SETMASK)
HW_ARC_ENDPTCTRL_RXR_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXR_SETMASK)
HW_ARC_ENDPTCTRL_RXE_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_RXE_SETMASK)
HW_ARC_ENDPTCTRL_TXS_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXS_SETMASK)
HW_ARC_ENDPTCTRL_TXD_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXD_SETMASK)
HW_ARC_ENDPTCTRL_TXT_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXT_SETMASK)
HW_ARC_ENDPTCTRL_TXI_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXI_SETMASK)
HW_ARC_ENDPTCTRL_TXR_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXR_SETMASK)
HW_ARC_ENDPTCTRL_TXE_CLRMASK    equ     (~HW_ARC_ENDPTCTRL_TXE_SETMASK)


;C insert
;typedef union               
;{
;    struct {
;        int RXS             :1;
;        int RXD             :1;
;        int RXT             :2;
;        int                 :1;
;        int RXI             :1;
;        int RXR             :1;
;        int RXE             :1;
;        int                 :8;
;        int TXS             :1;
;        int TXD             :1;
;        int TXT             :2;
;        int                 :1;
;        int TXI             :1;
;        int TXR             :1;
;        int TXE             :1;
;        int                 :24;
;    } B;
;    DWORD I;
;} endptctrl_type;

;//#define HW_ARC_ENDPTCTRL[n] (*(volatile usbintr_type _X*) (HW_ARC_BASEADDR+0x1c0+((n)*4)))    
;endc

    endif

