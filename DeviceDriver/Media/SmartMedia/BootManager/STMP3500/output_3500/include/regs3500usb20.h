//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;  Copyright(C) SigmaTel, Inc. 2002-2003
//;  File        : regs3500usb20.inc
//;  Description : USB20 IP Register definition
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// The following naming conventions are followed in this file.
// All registers are named using the format...
//     HW_<module>_<regname>
// where <module> is the module name which can be any of the following...
//     USB20
// (Note that when there is more than one copy of a particular module, the
// module name includes a number starting from 0 for the first instance of
// that module)
// <regname> is the specific register within that module
// We also define the following...
//     HW_<module>_<regname>_BITPOS
// which defines the starting bit (i.e. LSB) of a multi bit field
//     HW_<module>_<regname>_SETMASK
// which does something else, and
//     HW_<module>_<regname>_CLRMASK
// which does something else.
// Other rules
//     All caps
//     Numeric identifiers start at 0

#if !(defined(regs3500usb20inc))
#define regs3500usb20inc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//   USB2.0 STMP Registers 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define HW_USB_BASEADDR (0xF200)
#define HW_USBPHY_BASEADDR (0xF210)


/////////////////////////////////////////////////////////////////////////////////
//  USB Control Status Register (HW_USBCSR) Bit Definitions
#define HW_USBCSR_USBEN_BITPOS (0)
#define HW_USBCSR_WAKEUPIRQ_BITPOS (1)
#define HW_USBCSR_WAKEUPIE_BITPOS (2)
#define HW_USBCSR_VBUSCXIRQ_BITPOS (3)
#define HW_USBCSR_VBUSCXIE_BITPOS (4)
#define HW_USBCSR_VBUSDISCXIRQ_BITPOS (5)
#define HW_USBCSR_VBUSDISCXIE_BITPOS (6)
#define HW_USBCSR_CLKOFF_BITPOS (7)
#define HW_USBCSR_SUSP_BITPOS (8)
#define HW_USBCSR_SUSPF_BITPOS (9)
#define HW_USBCSR_UTMI_EXT_BITPOS (10)
#define HW_USBCSR_ARCCONNECT_BITPOS (11)
#define HW_USBCSR_PLUGGEDIN_EN_BITPOS (12)
#define HW_USBCSR_PLUGGEDIN_BITPOS (13)
#define HW_USBCSR_HOSTDISCONNECT_BITPOS (22)
#define HW_USBCSR_VBUSSENSE_BITPOS (23)

#define HW_USBCSR_USBEN_SETMASK (1<<HW_USBCSR_USBEN_BITPOS)        
#define HW_USBCSR_WAKEUPIRQ_SETMASK (1<<HW_USBCSR_WAKEUPIRQ_BITPOS) 
#define HW_USBCSR_WAKEUPIE_SETMASK (1<<HW_USBCSR_WAKEUPIE_BITPOS)  
#define HW_USBCSR_VBUSCXIRQ_SETMASK (1<<HW_USBCSR_VBUSCXIRQ_BITPOS)
#define HW_USBCSR_VBUSCXIE_SETMASK (1<<HW_USBCSR_VBUSCXIE_BITPOS)
#define HW_USBCSR_VBUSDISCXIRQ_SETMASK (1<<HW_USBCSR_VBUSDISCXIRQ_BITPOS)
#define HW_USBCSR_VBUSDISCXIE_SETMASK (1<<HW_USBCSR_VBUSDISCXIE_BITPOS)
#define HW_USBCSR_CLKOFF_SETMASK (1<<HW_USBCSR_CLKOFF_BITPOS)    
#define HW_USBCSR_SUSP_SETMASK (1<<HW_USBCSR_SUSP_BITPOS)      
#define HW_USBCSR_SUSPF_SETMASK (1<<HW_USBCSR_SUSPF_BITPOS)     
#define HW_USBCSR_UTMI_EXT_SETMASK (1<<HW_USBCSR_UTMI_EXT_BITPOS)
#define HW_USBCSR_ARCCONNECT_SETMASK (1<<HW_USBCSR_ARCCONNECT_BITPOS)   
#define HW_USBCSR_PLUGGEDIN_EN_SETMASK (1<<HW_USBCSR_PLUGGEDIN_EN_BITPOS)
#define HW_USBCSR_PLUGGEDIN_SETMASK (1<<HW_USBCSR_PLUGGEDIN_BITPOS)
#define HW_USBCSR_HOSTDISCONNECT_SETMASK (1<<HW_USBCSR_HOSTDISCONNECT_BITPOS)
#define HW_USBCSR_VBUSSENSE_SETMASK (1<<HW_USBCSR_VBUSSENSE_BITPOS)

#define HW_USBCSR_USBEN_CLRMASK (~(WORD)HW_USBCSR_USBEN_SETMASK)     
#define HW_USBCSR_WAKEUPIRQ_CLRMASK (~(WORD)HW_USBCSR_WAKEUPIRQ_SETMASK) 
#define HW_USBCSR_WAKEUPIE_CLRMASK (~(WORD)HW_USBCSR_WAKEUPIE_SETMASK)  
#define HW_USBCSR_VBUSCXIRQ_CLRMASK (~(WORD)HW_USBCSR_VBUSCXIRQ_SETMASK)
#define HW_USBCSR_VBUSCXIE_CLRMASK (~(WORD)HW_USBCSR_VBUSCXIE_SETMASK)
#define HW_USBCSR_VBUSDISCXIRQ_CLRMASK (~(WORD)HW_USBCSR_VBUSDISCXIRQ_SETMASK)
#define HW_USBCSR_VBUSDISCXIE_CLRMASK (~(WORD)HW_USBCSR_VBUSDISCXIE_SETMASK) 
#define HW_USBCSR_CLKOFF_CLRMASK (~(WORD)HW_USBCSR_CLKOFF_SETMASK)    
#define HW_USBCSR_SUSP_CLRMASK (~(WORD)HW_USBCSR_SUSP_SETMASK)      
#define HW_USBCSR_SUSPF_CLRMASK (~(WORD)HW_USBCSR_SUSPF_SETMASK)     
#define HW_USBCSR_UTMI_EXT_CLRMASK (~(WORD)HW_USBCSR_UTMI_EXT_SETMASK) 
#define HW_USBCSR_ARCCONNECT_CLRMASK (~(WORD)HW_USBCSR_ARCCONNECT_SETMASK)   
#define HW_USBCSR_PLUGGEDIN_EN_CLRMASK (~(WORD)HW_USBCSR_PLUGGEDIN_EN_SETMASK)
#define HW_USBCSR_PLUGGEDIN_CLRMASK (~(WORD)HW_USBCSR_PLUGGEDIN_SETMASK)
#define HW_USBCSR_HOSTDISCONNECT_CLRMASK (~(WORD)HW_USBCSR_HOSTDISCONNECT_SETMASK)
#define HW_USBCSR_VBUSSENSE_CLRMASK (~(WORD)HW_USBCSR_VBUSSENSE_SETMASK) 

typedef union               
{
    struct {
        int USBEN          :1;
        int WAKEUPIRQ      :1;
        int WAKEUPIE       :1;
        int VBUSCXIRQ      :1;
        int VBUSCXIE       :1;
        int VBUSDISCXIRQ   :1;
        int VBUSDISCXIE    :1;
        int CLKOFF         :1;
        int SUSP           :1;
        int SUSPF          :1;
        int UTMI_EXT       :1;
        int ARCCONNECT     :1;
        int PLUGGEDIN_EN   :1;
        int PLUGGEDIN      :1;
        int                :8;
        int HOSTDISCONNECT :1;
        int VBUSSENSE      :1;
    } B;
    int I;
} usbcsr_type;
#define HW_USBCSR      (*(volatile usbcsr_type _X*) (HW_USB_BASEADDR))    /* USB Control / Status Register */

/////////////////////////////////////////////////////////////////////////////////
//  USB DMA OFFSET register (HW_USBDMAOFF) Bit Definitions
#define HW_USBDMAOFF_MEM_BITPOS (16)

#define HW_USBDMAOFF_ADD_SETMASK (0x00FFFF)
#define HW_USBDMAOFF_MEM_SETMASK (3<<HW_USBDMAOFF_MEM_BITPOS)

#define HW_USBDMAOFF_ADD_CLRMASK (~(WORD)HW_USBDMAOFF_ADD_SETMASK)
#define HW_USBDMAOFF_MEM_CLRMASK (~(WORD)HW_USBDMAOFF_MEM_SETMASK)

typedef union               
{
    struct {
        int ADD            :16;
        int MEM            :2;
        int                :6;
    } B;
    int I;
} usbdmaoff_type;
#define HW_USBDMAOFF      (*(volatile usbdmaoff_type _X*) (HW_USB_BASEADDR+1))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC ACCESS register (HW_USBARCACCESS) Bit Definitions
#define HW_USBARCACCESS_RWB_BITPOS (16)
#define HW_USBARCACCESS_KICK_BITPOS (23)

#define HW_USBARCACCESS_ADD_SETMASK (0x0001FF)
#define HW_USBARCACCESS_RWB_SETMASK (1<<HW_USBARCACCESS_RWB_BITPOS)
#define HW_USBARCACCESS_KICK_SETMASK (23<<HW_USBDMAOFF_MEM_BITPOS)

#define HW_USBARCACCESS_ADD_CLRMASK (~(WORD)HW_USBARCACCESS_ADD_SETMASK)
#define HW_USBARCACCESS_RWB_CLRMASK (~(WORD)HW_USBARCACCESS_RWB_SETMASK) 
#define HW_USBARCACCESS_KICK_CLRMASK (~(WORD)HW_USBARCACCESS_KICK_SETMASK)

typedef union               
{
    struct {
        int ADD            :9;
        int                :7;
        int RWB            :1;
        int                :6;
        int KICK           :1;
    } B;
    int I;
} usbarcaccess_type;
#define HW_USBARCACCESS      (*(volatile usbarcaccess_type _X*) (HW_USB_BASEADDR+2))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC DATA LOW register (HW_USBARCDATALOW) Bit Definitions
#define HW_USBARCDATALOW_DATA_SETMASK (0x00FFFF)

#define HW_USBARCDATALOW_ADD_CLRMASK (~(WORD)HW_USBARCDATALOW_DATA_SETMASK)

typedef union               
{
    struct {
        int DATA           :16;
        int                :8;
    } B;
    int I;
} usbarcdatalow_type;
#define HW_USBARCDATALOW      (*(volatile usbarcdatalow_type _X*) (HW_USB_BASEADDR+3))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC DATA HIGH register (HW_USBARCDATAHIGH) Bit Definitions
#define HW_USBARCDATAHIGH_DATA_SETMASK (0x00FFFF)

#define HW_USBARCDATAHIGH_ADD_CLRMASK (~(WORD)HW_USBARCDATAHIGH_DATA_SETMASK)

typedef union               
{
    struct {
        int DATA           :16;
        int                :8;
    } B;
    int I;
} usbarcdatahigh_type;
#define HW_USBARCDATAHIGH     (*(volatile usbarcdatahigh_type _X*) (HW_USB_BASEADDR+4))    


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//   USB2.0 ARC Registers 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define HW_ARC_BASE_ADDR (0x0000)

#define HW_ARC_HCSPARAMS (HW_ARC_BASE_ADDR+0x104)
#define HW_ARC_USBCMD (HW_ARC_BASE_ADDR+0x140)
#define HW_ARC_USBSTS (HW_ARC_BASE_ADDR+0x144)
#define HW_ARC_USBINTR (HW_ARC_BASE_ADDR+0x148)
#define HW_ARC_DEVADDR (HW_ARC_BASE_ADDR+0x154)
#define HW_ARC_ENDPTLISTADDR (HW_ARC_BASE_ADDR+0x158)
#define HW_ARC_PORTSC1 (HW_ARC_BASE_ADDR+0x184)
#define HW_ARC_USBMODE (HW_ARC_BASE_ADDR+0x1a8)
#define HW_ARC_ENDPTSETUPSTAT (HW_ARC_BASE_ADDR+0x1ac)
#define HW_ARC_ENDPTPRIME (HW_ARC_BASE_ADDR+0x1b0)
#define HW_ARC_ENDPTFLUSH (HW_ARC_BASE_ADDR+0x1b4)
#define HW_ARC_ENDPTSTATUS (HW_ARC_BASE_ADDR+0x1b8)
#define HW_ARC_ENDPTCOMPLETE (HW_ARC_BASE_ADDR+0x1bc)
#define HW_ARC_ENDPTCTRL0 (HW_ARC_BASE_ADDR+0x1c0)
#define HW_ARC_ENDPTCTRL1 (HW_ARC_BASE_ADDR+0x1c4)
#define HW_ARC_ENDPTCTRL2 (HW_ARC_BASE_ADDR+0x1c8)
#define HW_ARC_ENDPTCTRL3 (HW_ARC_BASE_ADDR+0x1cc)
#define HW_ARC_ENDPTCTRL4 (HW_ARC_BASE_ADDR+0x1d0)
#define HW_ARC_ENDPTCTRL5 (HW_ARC_BASE_ADDR+0x1d4)
#define HW_ARC_ENDPTCTRL6 (HW_ARC_BASE_ADDR+0x1d8)
#define HW_ARC_ENDPTCTRL7 (HW_ARC_BASE_ADDR+0x1dc)
#define HW_ARC_ENDPTCTRL8 (HW_ARC_BASE_ADDR+0x1e0)
#define HW_ARC_ENDPTCTRL9 (HW_ARC_BASE_ADDR+0x1e4)
#define HW_ARC_ENDPTCTRL10 (HW_ARC_BASE_ADDR+0x1e8)
#define HW_ARC_ENDPTCTRL11 (HW_ARC_BASE_ADDR+0x1ec)
#define HW_ARC_ENDPTCTRL12 (HW_ARC_BASE_ADDR+0x1f0)
#define HW_ARC_ENDPTCTRL13 (HW_ARC_BASE_ADDR+0x1f4)
#define HW_ARC_ENDPTCTRL14 (HW_ARC_BASE_ADDR+0x1f8)
#define HW_ARC_ENDPTCTRL15 (HW_ARC_BASE_ADDR+0x1fc)

#define HW_ARC_ENDPTCTRL(n) (HW_ARC_ENDPTCTRL0+((n)*4))    
/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register Host Control Structural Parameters (HW_ARC_HCSPARAMS)

#define HW_ARC_HCSPARAMS_NPORTS_BITPOS (0)
#define HW_ARC_HCSPARAMS_PPC_BITPOS (4)
#define HW_ARC_HCSPARAMS_NPCC_BITPOS (8)
#define HW_ARC_HCSPARAMS_NCC_BITPOS (12)
#define HW_ARC_HCSPARAMS_PI_BITPOS (16)
#define HW_ARC_HCSPARAMS_NPTT_BITPOS (20)
#define HW_ARC_HCSPARAMS_NTT_BITPOS (24)

#define HW_ARC_HCSPARAMS_NPORTS_SETMASK (15<<HW_ARC_HCSPARAMS_NPORTS_BITPOS)
#define HW_ARC_HCSPARAMS_PPC_SETMASK (1<<HW_ARC_HCSPARAMS_PPC_BITPOS)        
#define HW_ARC_HCSPARAMS_NPCC_SETMASK (15<<HW_ARC_HCSPARAMS_NPCC_BITPOS)  
#define HW_ARC_HCSPARAMS_NCC_SETMASK (15<<HW_ARC_HCSPARAMS_NCC_BITPOS)       
#define HW_ARC_HCSPARAMS_PI_SETMASK (1<<HW_ARC_HCSPARAMS_PI_BITPOS)     
#define HW_ARC_HCSPARAMS_NPTT_SETMASK (15<<HW_ARC_HCSPARAMS_NPTT_BITPOS)  
#define HW_ARC_HCSPARAMS_NTT_SETMASK (15<<HW_ARC_HCSPARAMS_NTT_BITPOS)       

#define HW_ARC_HCSPARAMS_NPORTS_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_NPORTS_SETMASK)
#define HW_ARC_HCSPARAMS_PPC_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_PPC_SETMASK)
#define HW_ARC_HCSPARAMS_NPCC_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_NPCC_SETMASK)
#define HW_ARC_HCSPARAMS_NCC_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_NCC_SETMASK)
#define HW_ARC_HCSPARAMS_PI_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_PI_SETMASK)  
#define HW_ARC_HCSPARAMS_NPTT_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_NPTT_SETMASK)
#define HW_ARC_HCSPARAMS_NTT_CLRMASK (~(WORD)HW_ARC_HCSPARAMS_NTT_SETMASK)

typedef union               
{
    struct {
        int N_PORTS         :4;
        int PPC             :1;
        int                 :3;
        int N_PCC           :4;
        int N_CC            :4;
        int PI              :1;
        int                 :3;
        int N_PTT           :4;
        int N_TT            :4;
        int                 :20;
    } B;
    DWORD I;
} hcsparams_type;
// #define HW_ARC_HCSPARAMS (*(volatile hcsparams_type _X*) (HW_ARC_BASEADDR))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Command (HW_ARC_USBCMD)

#define HW_ARC_USBCMD_RS_BITPOS (0)
#define HW_ARC_USBCMD_RST_BITPOS (1)
#define HW_ARC_USBCMD_FS0_BITPOS (2)
#define HW_ARC_USBCMD_FS1_BITPOS (3)
#define HW_ARC_USBCMD_PSE_BITPOS (4)
#define HW_ARC_USBCMD_ASE_BITPOS (5)
#define HW_ARC_USBCMD_IAA_BITPOS (6)
#define HW_ARC_USBCMD_LR_BITPOS (7)
#define HW_ARC_USBCMD_ASP0_BITPOS (8)
#define HW_ARC_USBCMD_ASP1_BITPOS (9)
#define HW_ARC_USBCMD_ASPE_BITPOS (11)
#define HW_ARC_USBCMD_FS2_BITPOS (15)
#define HW_ARC_USBCMD_ITC_BITPOS (16)

#define HW_ARC_USBCMD_RS_SETMASK (1<<HW_ARC_USBCMD_RS_BITPOS)                
#define HW_ARC_USBCMD_RST_SETMASK (1<<HW_ARC_USBCMD_RST_BITPOS)   
#define HW_ARC_USBCMD_FS0_SETMASK (1<<HW_ARC_USBCMD_FS0_BITPOS)   
#define HW_ARC_USBCMD_FS1_SETMASK (1<<HW_ARC_USBCMD_FS1_BITPOS)   
#define HW_ARC_USBCMD_PSE_SETMASK (1<<HW_ARC_USBCMD_PSE_BITPOS)   
#define HW_ARC_USBCMD_ASE_SETMASK (1<<HW_ARC_USBCMD_ASE_BITPOS)   
#define HW_ARC_USBCMD_IAA_SETMASK (1<<HW_ARC_USBCMD_IAA_BITPOS)   
#define HW_ARC_USBCMD_LR_SETMASK (1<<HW_ARC_USBCMD_LR_BITPOS)        
#define HW_ARC_USBCMD_ASP0_SETMASK (1<<HW_ARC_USBCMD_ASP0_BITPOS)
#define HW_ARC_USBCMD_ASP1_SETMASK (1<<HW_ARC_USBCMD_ASP1_BITPOS)
#define HW_ARC_USBCMD_ASPE_SETMASK (1<<HW_ARC_USBCMD_ASPE_BITPOS)
#define HW_ARC_USBCMD_FS2_SETMASK (1<<HW_ARC_USBCMD_FS2_BITPOS)    
#define HW_ARC_USBCMD_ITC_SETMASK (255<<HW_ARC_USBCMD_ITC_BITPOS)

#define HW_ARC_USBCMD_RS_CLRMASK (~(WORD)HW_ARC_USBCMD_RS_SETMASK)     
#define HW_ARC_USBCMD_RST_CLRMASK (~(WORD)HW_ARC_USBCMD_RST_SETMASK)    
#define HW_ARC_USBCMD_FS0_CLRMASK (~(WORD)HW_ARC_USBCMD_FS0_SETMASK)    
#define HW_ARC_USBCMD_FS1_CLRMASK (~(WORD)HW_ARC_USBCMD_FS1_SETMASK)    
#define HW_ARC_USBCMD_PSE_CLRMASK (~(WORD)HW_ARC_USBCMD_PSE_SETMASK)    
#define HW_ARC_USBCMD_ASE_CLRMASK (~(WORD)HW_ARC_USBCMD_ASE_SETMASK)    
#define HW_ARC_USBCMD_IAA_CLRMASK (~(WORD)HW_ARC_USBCMD_IAA_SETMASK)    
#define HW_ARC_USBCMD_LR_CLRMASK (~(WORD)HW_ARC_USBCMD_LR_SETMASK) 
#define HW_ARC_USBCMD_ASP0_CLRMASK (~(WORD)HW_ARC_USBCMD_ASP0_SETMASK)
#define HW_ARC_USBCMD_ASP1_CLRMASK (~(WORD)HW_ARC_USBCMD_ASP1_SETMASK)
#define HW_ARC_USBCMD_ASPE_CLRMASK (~(WORD)HW_ARC_USBCMD_ASPE_SETMASK)
#define HW_ARC_USBCMD_FS2_CLRMASK (~(WORD)HW_ARC_USBCMD_FS2_SETMASK)    
#define HW_ARC_USBCMD_ITC_CLRMASK (~(WORD)HW_ARC_USBCMD_ITC_SETMASK)    

typedef union               
{
    struct {
        int RS              :1;
        int RST             :1;
        int FS0             :1;
        int FS1             :1;
        int PSE             :1;
        int ASE             :1;
        int IAA             :1;
        int LR              :1;
        int ASP0            :1;
        int ASP1            :1;
        int                 :1;
        int ASPE            :1;
        int                 :3;
        int FS2             :1;
        int ITC             :8;
        int                 :24;
    } B;
    DWORD I;
} usbcmd_type;
//#define HW_ARC_USBCMD ((volatile usbcmd_type _X*) (HW_ARC_BASEADDR+0x140))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Status (HW_ARC_USBSTS)

#define HW_ARC_USBSTS_UI_BITPOS (0)
#define HW_ARC_USBSTS_UEI_BITPOS (1)
#define HW_ARC_USBSTS_PCI_BITPOS (2)
#define HW_ARC_USBSTS_FRI_BITPOS (3)
#define HW_ARC_USBSTS_SEI_BITPOS (4)
#define HW_ARC_USBSTS_AAI_BITPOS (5)
#define HW_ARC_USBSTS_URI_BITPOS (6)
#define HW_ARC_USBSTS_SRI_BITPOS (7)
#define HW_ARC_USBSTS_SLI_BITPOS (8)
#define HW_ARC_USBSTS_HCH_BITPOS (12)
#define HW_ARC_USBSTS_RCL_BITPOS (13)
#define HW_ARC_USBSTS_PS_BITPOS (14)
#define HW_ARC_USBSTS_AS_BITPOS (15)

#define HW_ARC_USBSTS_UI_SETMASK (1<<HW_ARC_USBSTS_UI_BITPOS)    
#define HW_ARC_USBSTS_UEI_SETMASK (1<<HW_ARC_USBSTS_UEI_BITPOS)
#define HW_ARC_USBSTS_PCI_SETMASK (1<<HW_ARC_USBSTS_PCI_BITPOS)
#define HW_ARC_USBSTS_FRI_SETMASK (1<<HW_ARC_USBSTS_FRI_BITPOS)
#define HW_ARC_USBSTS_SEI_SETMASK (1<<HW_ARC_USBSTS_SEI_BITPOS)
#define HW_ARC_USBSTS_AAI_SETMASK (1<<HW_ARC_USBSTS_AAI_BITPOS)
#define HW_ARC_USBSTS_URI_SETMASK (1<<HW_ARC_USBSTS_URI_BITPOS)
#define HW_ARC_USBSTS_SRI_SETMASK (1<<HW_ARC_USBSTS_SRI_BITPOS)
#define HW_ARC_USBSTS_SLI_SETMASK (1<<HW_ARC_USBSTS_SLI_BITPOS)
#define HW_ARC_USBSTS_HCH_SETMASK (1<<HW_ARC_USBSTS_HCH_BITPOS)
#define HW_ARC_USBSTS_RCL_SETMASK (1<<HW_ARC_USBSTS_RCL_BITPOS)
#define HW_ARC_USBSTS_PS_SETMASK (1<<HW_ARC_USBSTS_PS_BITPOS)    
#define HW_ARC_USBSTS_AS_SETMASK (1<<HW_ARC_USBSTS_AS_BITPOS)    

#define HW_ARC_USBSTS_UI_CLRMASK (~(WORD)HW_ARC_USBSTS_UI_SETMASK)
#define HW_ARC_USBSTS_UEI_CLRMASK (~(WORD)HW_ARC_USBSTS_UEI_SETMASK)
#define HW_ARC_USBSTS_PCI_CLRMASK (~(WORD)HW_ARC_USBSTS_PCI_SETMASK)
#define HW_ARC_USBSTS_FRI_CLRMASK (~(WORD)HW_ARC_USBSTS_FRI_SETMASK)
#define HW_ARC_USBSTS_SEI_CLRMASK (~(WORD)HW_ARC_USBSTS_SEI_SETMASK)
#define HW_ARC_USBSTS_AAI_CLRMASK (~(WORD)HW_ARC_USBSTS_AAI_SETMASK)
#define HW_ARC_USBSTS_URI_CLRMASK (~(WORD)HW_ARC_USBSTS_URI_SETMASK)
#define HW_ARC_USBSTS_SRI_CLRMASK (~(WORD)HW_ARC_USBSTS_SRI_SETMASK)
#define HW_ARC_USBSTS_SLI_CLRMASK (~(WORD)HW_ARC_USBSTS_SLI_SETMASK)
#define HW_ARC_USBSTS_HCH_CLRMASK (~(WORD)HW_ARC_USBSTS_HCH_SETMASK)
#define HW_ARC_USBSTS_RCL_CLRMASK (~(WORD)HW_ARC_USBSTS_RCL_SETMASK)
#define HW_ARC_USBSTS_PS_CLRMASK (~(WORD)HW_ARC_USBSTS_PS_SETMASK)
#define HW_ARC_USBSTS_AS_CLRMASK (~(WORD)HW_ARC_USBSTS_AS_SETMASK)


typedef union               
{
    struct {
        int UI              :1;
        int UEI             :1;
        int PCI             :1;
        int FRI             :1;
        int SEI             :1;
        int AAI             :1;
        int URI             :1;
        int STI             :1;
        int SLI             :1;
        int                 :3;
        int HCH             :1;
        int RCL             :1;
        int PS              :1;
        int AS              :1;
        int                 :24;
    } B;
    DWORD I;
} usbsts_type;
//#define HW_ARC_USBSTS (*(volatile usbsts_type _X*) (HW_ARC_BASEADDR+0x144))    

/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Interrupt Enable (HW_ARC_USBINTR)

#define HW_ARC_USBINTR_UE_BITPOS (0)
#define HW_ARC_USBINTR_UEE_BITPOS (1)
#define HW_ARC_USBINTR_PCE_BITPOS (2)
#define HW_ARC_USBINTR_FRE_BITPOS (3)
#define HW_ARC_USBINTR_SEE_BITPOS (4)
#define HW_ARC_USBINTR_AAE_BITPOS (5)
#define HW_ARC_USBINTR_URE_BITPOS (6)
#define HW_ARC_USBINTR_SRE_BITPOS (7)
#define HW_ARC_USBINTR_SLE_BITPOS (8)

#define HW_ARC_USBINTR_UE_SETMASK (1<<HW_ARC_USBINTR_UE_BITPOS)   
#define HW_ARC_USBINTR_UEE_SETMASK (1<<HW_ARC_USBINTR_UEE_BITPOS)
#define HW_ARC_USBINTR_PCE_SETMASK (1<<HW_ARC_USBINTR_PCE_BITPOS)
#define HW_ARC_USBINTR_FRE_SETMASK (1<<HW_ARC_USBINTR_FRE_BITPOS)
#define HW_ARC_USBINTR_SEE_SETMASK (1<<HW_ARC_USBINTR_SEE_BITPOS)
#define HW_ARC_USBINTR_AAE_SETMASK (1<<HW_ARC_USBINTR_AAE_BITPOS)
#define HW_ARC_USBINTR_URE_SETMASK (1<<HW_ARC_USBINTR_URE_BITPOS)
#define HW_ARC_USBINTR_SRE_SETMASK (1<<HW_ARC_USBINTR_SRE_BITPOS)
#define HW_ARC_USBINTR_SLE_SETMASK (1<<HW_ARC_USBINTR_SLE_BITPOS)

#define HW_ARC_USBINTR_UE_CLRMASK (~(WORD)HW_ARC_USBINTR_UE_SETMASK)
#define HW_ARC_USBINTR_UEE_CLRMASK (~(WORD)HW_ARC_USBINTR_UEE_SETMASK)
#define HW_ARC_USBINTR_PCE_CLRMASK (~(WORD)HW_ARC_USBINTR_PCE_SETMASK)
#define HW_ARC_USBINTR_FRE_CLRMASK (~(WORD)HW_ARC_USBINTR_FRE_SETMASK)
#define HW_ARC_USBINTR_SEE_CLRMASK (~(WORD)HW_ARC_USBINTR_SEE_SETMASK)
#define HW_ARC_USBINTR_AAE_CLRMASK (~(WORD)HW_ARC_USBINTR_AAE_SETMASK)
#define HW_ARC_USBINTR_URE_CLRMASK (~(WORD)HW_ARC_USBINTR_URE_SETMASK)
#define HW_ARC_USBINTR_SRE_CLRMASK (~(WORD)HW_ARC_USBINTR_SRE_SETMASK)
#define HW_ARC_USBINTR_SLE_CLRMASK (~(WORD)HW_ARC_USBINTR_SLE_SETMASK)


typedef union               
{
    struct {
        int UE              :1;
        int UEE             :1;
        int PCE             :1;
        int FRE             :1;
        int SEE             :1;
        int AAE             :1;
        int URE             :1;
        int STE             :1;
        int SLE             :1;
        int                 :39;
    } B;
    DWORD I;
} usbintr_type;
//#define HW_ARC_USBINTR (*(volatile usbintr_type _X*) (HW_ARC_BASEADDR+0x148))    


/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Device Controller Device Address (HW_ARC_DEVADDR)

#define HW_ARC_DEVADDR_ADD_BITPOS (25)

#define HW_ARC_DEVADDR_ADD_SETMASK (127<<HW_ARC_DEVADDR_ADD_BITPOS)    

#define HW_ARC_DEVEADDR_ADD_CLRMASK (~(WORD)HW_ARC_DEVADDR_ADD_SETMASK)   

typedef union               
{
    struct {
        int                 :25;
        int ADD             :7;
        int                 :16;
    } B;
    DWORD I;
} devaddr_type;
//#define HW_ARC_DEVADDR (*(volatile devaddr_type _X*) (HW_ARC_BASEADDR+0x154))    


/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Device Controller Endpoint List Address (HW_ARC_ENDPTLISTADDR)

#define HW_ARC_ENDPTLISTADDR_ADD_BITPOS (11)

#define HW_ARC_ENDPTLISTADDR_ADD_SETMASK (0x400000<<HW_ARC_ENDPTLISTADDR_ADD_BITPOS) 

#define HW_ARC_ENDPTLISTADDR_ADD_CLRMASK (~(WORD)HW_ARC_ENDPTLISTADDR_ADD_SETMASK) 

typedef union               
{
    struct {
        int                 :10;
        int ADD             :22;
        int                 :16;
    } B;
    DWORD I;
} endptlistaddr_type;
//#define HW_ARC_ENDPTLISTADDR (*(volatile endptlistaddr_type _X*) (HW_ARC_BASEADDR+0x158))    


/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Port Status Control 1 (HW_ARC_PORTSC1)

#define HW_ARC_PORTSC1_CCS_BITPOS (0)
#define HW_ARC_PORTSC1_CSC_BITPOS (1)
#define HW_ARC_PORTSC1_PE_BITPOS (2)
#define HW_ARC_PORTSC1_PEC_BITPOS (3)
#define HW_ARC_PORTSC1_OCA_BITPOS (4)
#define HW_ARC_PORTSC1_OCC_BITPOS (5)
#define HW_ARC_PORTSC1_FPR_BITPOS (6)
#define HW_ARC_PORTSC1_SUSP_BITPOS (7)
#define HW_ARC_PORTSC1_PR_BITPOS (8)
#define HW_ARC_PORTSC1_HSP_BITPOS (9)
#define HW_ARC_PORTSC1_LS_BITPOS (10)
#define HW_ARC_PORTSC1_PP_BITPOS (12)
#define HW_ARC_PORTSC1_PO_BITPOS (13)
#define HW_ARC_PORTSC1_PIC_BITPOS (14)
#define HW_ARC_PORTSC1_PTC_BITPOS (16)
#define HW_ARC_PORTSC1_WKCN_BITPOS (20)
#define HW_ARC_PORTSC1_WKDS_BITPOS (21)
#define HW_ARC_PORTSC1_WKOC_BITPOS (22)
#define HW_ARC_PORTSC1_PHCD_BITPOS (23)
#define HW_ARC_PORTSC1_PFSC_BITPOS (24)
#define HW_ARC_PORTSC1_PSPD_BITPOS (26)
#define HW_ARC_PORTSC1_PTW_BITPOS (29)
#define HW_ARC_PORTSC1_STS_BITPOS (30)
#define HW_ARC_PORTSC1_PTS_BITPOS (31)

#define HW_ARC_PORTSC1_CCS_SETMASK (1<<HW_ARC_PORTSC1_CCS_BITPOS)  
#define HW_ARC_PORTSC1_CSC_SETMASK (1<<HW_ARC_PORTSC1_CSC_BITPOS)  
#define HW_ARC_PORTSC1_PE_SETMASK (1<<HW_ARC_PORTSC1_PE_BITPOS)   
#define HW_ARC_PORTSC1_PEC_SETMASK (1<<HW_ARC_PORTSC1_PEC_BITPOS)  
#define HW_ARC_PORTSC1_OCA_SETMASK (1<<HW_ARC_PORTSC1_OCA_BITPOS)  
#define HW_ARC_PORTSC1_OCC_SETMASK (1<<HW_ARC_PORTSC1_OCC_BITPOS)  
#define HW_ARC_PORTSC1_FPR_SETMASK (1<<HW_ARC_PORTSC1_FPR_BITPOS)  
#define HW_ARC_PORTSC1_SUSP_SETMASK (1<<HW_ARC_PORTSC1_SUSP_BITPOS)
#define HW_ARC_PORTSC1_PR_SETMASK (1<<HW_ARC_PORTSC1_PR_BITPOS)   
#define HW_ARC_PORTSC1_HSP_SETMASK (1<<HW_ARC_PORTSC1_HSP_BITPOS)  
#define HW_ARC_PORTSC1_LS_SETMASK (3<<HW_ARC_PORTSC1_LS_BITPOS)   
#define HW_ARC_PORTSC1_PP_SETMASK (1<<HW_ARC_PORTSC1_PP_BITPOS)   
#define HW_ARC_PORTSC1_PO_SETMASK (1<<HW_ARC_PORTSC1_PO_BITPOS)   
#define HW_ARC_PORTSC1_PIC_SETMASK (3<<HW_ARC_PORTSC1_PIC_BITPOS)  
#define HW_ARC_PORTSC1_PTC_SETMASK (15<<HW_ARC_PORTSC1_PTC_BITPOS) 
#define HW_ARC_PORTSC1_WKCN_SETMASK (1<<HW_ARC_PORTSC1_WKCN_BITPOS)
#define HW_ARC_PORTSC1_WKDS_SETMASK (1<<HW_ARC_PORTSC1_WKDS_BITPOS)
#define HW_ARC_PORTSC1_WKOC_SETMASK (1<<HW_ARC_PORTSC1_WKOC_BITPOS)
#define HW_ARC_PORTSC1_PHCD_SETMASK (1<<HW_ARC_PORTSC1_PHCD_BITPOS)

// We need to equate the following label like this due to a sign extension problem
// if equated like so (1<<HW_ARC_PORTSC1_PFSC_SETMASK)
#define HW_ARC_PORTSC1_PFSC_SETMASK (0x01000000)

#define HW_ARC_PORTSC1_PSPD_SETMASK (3<<HW_ARC_PORTSC1_PSPD_BITPOS)
#define HW_ARC_PORTSC1_PTW_SETMASK (1<<HW_ARC_PORTSC1_PTW_BITPOS)  
#define HW_ARC_PORTSC1_STS_SETMASK (1<<HW_ARC_PORTSC1_STS_BITPOS)  
#define HW_ARC_PORTSC1_PTS_SETMASK (1<<HW_ARC_PORTSC1_PTS_BITPOS)  

#define HW_ARC_PORTSC1_CCS_CLRMASK (~(WORD)HW_ARC_PORTSC1_CCS_SETMASK)   
#define HW_ARC_PORTSC1_CSC_CLRMASK (~(WORD)HW_ARC_PORTSC1_CSC_SETMASK)   
#define HW_ARC_PORTSC1_PE_CLRMASK (~(WORD)HW_ARC_PORTSC1_PE_SETMASK)    
#define HW_ARC_PORTSC1_PEC_CLRMASK (~(WORD)HW_ARC_PORTSC1_PEC_SETMASK)   
#define HW_ARC_PORTSC1_OCA_CLRMASK (~(WORD)HW_ARC_PORTSC1_OCA_SETMASK)   
#define HW_ARC_PORTSC1_OCC_CLRMASK (~(WORD)HW_ARC_PORTSC1_OCC_SETMASK)   
#define HW_ARC_PORTSC1_FPR_CLRMASK (~(WORD)HW_ARC_PORTSC1_FPR_SETMASK)   
#define HW_ARC_PORTSC1_SUSP_CLRMASK (~(WORD)HW_ARC_PORTSC1_SUSP_SETMASK)
#define HW_ARC_PORTSC1_PR_CLRMASK (~(WORD)HW_ARC_PORTSC1_PR_SETMASK)    
#define HW_ARC_PORTSC1_HSP_CLRMASK (~(WORD)HW_ARC_PORTSC1_HSP_SETMASK)   
#define HW_ARC_PORTSC1_LS_CLRMASK (~(WORD)HW_ARC_PORTSC1_LS_SETMASK)    
#define HW_ARC_PORTSC1_PP_CLRMASK (~(WORD)HW_ARC_PORTSC1_PP_SETMASK)    
#define HW_ARC_PORTSC1_PO_CLRMASK (~(WORD)HW_ARC_PORTSC1_PO_SETMASK)    
#define HW_ARC_PORTSC1_PIC_CLRMASK (~(WORD)HW_ARC_PORTSC1_PIC_SETMASK)   
#define HW_ARC_PORTSC1_PTC_CLRMASK (~(WORD)HW_ARC_PORTSC1_PTC_SETMASK)   
#define HW_ARC_PORTSC1_WKCN_CLRMASK (~(WORD)HW_ARC_PORTSC1_WKCN_SETMASK)
#define HW_ARC_PORTSC1_WKDS_CLRMASK (~(WORD)HW_ARC_PORTSC1_WKDS_SETMASK)
#define HW_ARC_PORTSC1_WKOC_CLRMASK (~(WORD)HW_ARC_PORTSC1_WKOC_SETMASK)
#define HW_ARC_PORTSC1_PHCD_CLRMASK (~(WORD)HW_ARC_PORTSC1_PHCD_SETMASK)
#define HW_ARC_PORTSC1_PFSC_CLRMASK (~(WORD)HW_ARC_PORTSC1_PFSC_SETMASK)
#define HW_ARC_PORTSC1_PSPD_CLRMASK (~(WORD)HW_ARC_PORTSC1_PSPD_SETMASK)
#define HW_ARC_PORTSC1_PTW_CLRMASK (~(WORD)HW_ARC_PORTSC1_PTW_SETMASK)   
#define HW_ARC_PORTSC1_STS_CLRMASK (~(WORD)HW_ARC_PORTSC1_STS_SETMASK)   
#define HW_ARC_PORTSC1_PTS_CLRMASK (~(WORD)HW_ARC_PORTSC1_PTS_SETMASK)   

typedef union               
{
    struct {
        int CCS             :1;
        int CSC             :1;
        int PE              :1;
        int PEC             :1;
        int OCA             :1;
        int OCC             :1;
        int FPR             :1;
        int SUSP            :1;
        int PR              :1;
        int HSP             :1;
        int LS              :2;
        int PP              :1;
        int PO              :1;
        int PIC             :2;
        int PTC             :4;
        int WKCN            :1;
        int WKDS            :1;
        int WKOC            :1;
        int PHCD            :1;
        int PFSC            :1;
        int                 :1;
        int PSPD            :2;
        int                 :1;
        int PTW             :1;
        int STS             :1;
        int PTS             :1;
        int                 :16;
    } B;
    DWORD I;
} portsc1_type;
//#define HW_ARC_PORTSC1 (*(volatile portsc1_type _X*) (HW_ARC_BASEADDR+0x184))    


/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register USB Device Mode (HW_ARC_USBMODE)

#define HW_ARC_USBMODE_CM_BITPOS (0)
#define HW_ARC_USBMODE_ES_BITPOS (2)

#define HW_ARC_USBMODE_CM_SETMASK (3<<HW_ARC_USBMODE_CM_BITPOS)   
#define HW_ARC_USBMODE_ES_SETMASK (1<<HW_ARC_USBMODE_ES_BITPOS)   

#define HW_ARC_USBMODE_CM_CLRMASK (~(WORD)HW_ARC_USBMODE_CM_SETMASK) 
#define HW_ARC_USBMODE_ES_CLRMASK (~(WORD)HW_ARC_USBMODE_ES_SETMASK)    

typedef union               
{
    struct {
        int CM              :2;
        int ES              :1;
        int                 :46;
    } B;
    DWORD I;
} usbmode_type;
//#define HW_ARC_USBMODE (*(volatile usbmode_type _X*) (HW_ARC_BASEADDR+0x1a8))    


/////////////////////////////////////////////////////////////////////////////////
//  The following endpoint equates are common for the following registers

#define ENDPOINT0_BITPOS (0)
#define ENDPOINT1_BITPOS (1)
#define ENDPOINT2_BITPOS (2)
#define ENDPOINT3_BITPOS (3)
#define ENDPOINT4_BITPOS (4)
#define ENDPOINT5_BITPOS (5)
#define ENDPOINT6_BITPOS (6)
#define ENDPOINT7_BITPOS (7)
#define ENDPOINT8_BITPOS (8)
#define ENDPOINT9_BITPOS (9)
#define ENDPOINT10_BITPOS (10)
#define ENDPOINT11_BITPOS (11)
#define ENDPOINT12_BITPOS (12)
#define ENDPOINT13_BITPOS (13)
#define ENDPOINT14_BITPOS (14)
#define ENDPOINT15_BITPOS (15)

#define ENDPOINT0_SETMASK (1<<ENDPOINT0_BITPOS)
#define ENDPOINT1_SETMASK (1<<ENDPOINT1_BITPOS)
#define ENDPOINT2_SETMASK (1<<ENDPOINT2_BITPOS)
#define ENDPOINT3_SETMASK (1<<ENDPOINT3_BITPOS)
#define ENDPOINT4_SETMASK (1<<ENDPOINT4_BITPOS)
#define ENDPOINT5_SETMASK (1<<ENDPOINT5_BITPOS)
#define ENDPOINT6_SETMASK (1<<ENDPOINT6_BITPOS)
#define ENDPOINT7_SETMASK (1<<ENDPOINT7_BITPOS)
#define ENDPOINT8_SETMASK (1<<ENDPOINT8_BITPOS)
#define ENDPOINT9_SETMASK (1<<ENDPOINT9_BITPOS)
#define ENDPOINT10_SETMASK (1<<ENDPOINT10_BITPOS)
#define ENDPOINT11_SETMASK (1<<ENDPOINT11_BITPOS)
#define ENDPOINT12_SETMASK (1<<ENDPOINT12_BITPOS)
#define ENDPOINT13_SETMASK (1<<ENDPOINT13_BITPOS)
#define ENDPOINT14_SETMASK (1<<ENDPOINT14_BITPOS)
#define ENDPOINT15_SETMASK (1<<ENDPOINT15_BITPOS)

#define ENDPOINT0_CLRMASK (~(WORD)ENDPOINT0_SETMASK)    
#define ENDPOINT1_CLRMASK (~(WORD)ENDPOINT1_SETMASK)    
#define ENDPOINT2_CLRMASK (~(WORD)ENDPOINT2_SETMASK)    
#define ENDPOINT3_CLRMASK (~(WORD)ENDPOINT3_SETMASK)    
#define ENDPOINT4_CLRMASK (~(WORD)ENDPOINT4_SETMASK)    
#define ENDPOINT5_CLRMASK (~(WORD)ENDPOINT5_SETMASK)    
#define ENDPOINT6_CLRMASK (~(WORD)ENDPOINT6_SETMASK)    
#define ENDPOINT7_CLRMASK (~(WORD)ENDPOINT7_SETMASK)    
#define ENDPOINT8_CLRMASK (~(WORD)ENDPOINT8_SETMASK)    
#define ENDPOINT9_CLRMASK (~(WORD)ENDPOINT9_SETMASK)    
#define ENDPOINT10_CLRMASK (~(WORD)ENDPOINT10_SETMASK)
#define ENDPOINT11_CLRMASK (~(WORD)ENDPOINT11_SETMASK)
#define ENDPOINT12_CLRMASK (~(WORD)ENDPOINT12_SETMASK)
#define ENDPOINT13_CLRMASK (~(WORD)ENDPOINT13_SETMASK)
#define ENDPOINT14_CLRMASK (~(WORD)ENDPOINT14_SETMASK)
#define ENDPOINT15_CLRMASK (~(WORD)ENDPOINT15_SETMASK)

typedef union               
{
    struct {
        int EP0              :1;
        int EP1              :1;
        int EP2              :1;
        int EP3              :1;
        int EP4              :1;
        int EP5              :1;
        int EP6              :1;
        int EP7              :1;
        int EP8              :1;
        int EP9              :1;
        int EP10             :1;
        int EP11             :1;
        int EP12             :1;
        int EP13             :1;
        int EP14             :1;
        int EP15             :1;
        int                  :32;
    } B;
    DWORD I;
} endpsetupstat_type;

//#define HW_ARC_ENDPTSETUPSTAT (*(volatile endpsetupstat_type _X*) (HW_ARC_BASEADDR+0x1ac))    

typedef union               
{
    struct {
        int EP0              :1;
        int EP1              :1;
        int EP2              :1;
        int EP3              :1;
        int EP4              :1;
        int EP5              :1;
        int EP6              :1;
        int EP7              :1;
        int EP8              :1;
        int EP9              :1;
        int EP10             :1;
        int EP11             :1;
        int EP12             :1;
        int EP13             :1;
        int EP14             :1;
        int EP15             :1;
        int                  :8;
    } B;
    WORD I;
} endpt_type;

typedef union
{
   struct {
       endpt_type  RX;
       endpt_type  TX;
   } W;
   DWORD DW;
} endptrxtx_type;

//#define HW_ARC_ENDPTPRIME    (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b0))    
//#define HW_ARC_ENDPTFLUSH    (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b4))    
//#define HW_ARC_ENDPTSTAT     (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1b8))    
//#define HW_ARC_ENDPTCOMPLETE (*(volatile endptrxtx_type _X*) (HW_ARC_BASEADDR+0x1bc))    



/////////////////////////////////////////////////////////////////////////////////
//  USB ARC Register Endpoint control (HW_ARC_ENDPTCTRL)

#define HW_ARC_ENDPTCTRL_RXS_BITPOS (0)
#define HW_ARC_ENDPTCTRL_RXD_BITPOS (1)
#define HW_ARC_ENDPTCTRL_RXT_BITPOS (2)
#define HW_ARC_ENDPTCTRL_RXI_BITPOS (5)
#define HW_ARC_ENDPTCTRL_RXR_BITPOS (6)
#define HW_ARC_ENDPTCTRL_RXE_BITPOS (7)
#define HW_ARC_ENDPTCTRL_TXS_BITPOS (16)
#define HW_ARC_ENDPTCTRL_TXD_BITPOS (17)
#define HW_ARC_ENDPTCTRL_TXT_BITPOS (18)
#define HW_ARC_ENDPTCTRL_TXI_BITPOS (21)
#define HW_ARC_ENDPTCTRL_TXR_BITPOS (22)
#define HW_ARC_ENDPTCTRL_TXE_BITPOS (23)

#define HW_ARC_ENDPTCTRL_RXS_SETMASK (1<<HW_ARC_ENDPTCTRL_RXS_BITPOS)
#define HW_ARC_ENDPTCTRL_RXD_SETMASK (1<<HW_ARC_ENDPTCTRL_RXD_BITPOS)
#define HW_ARC_ENDPTCTRL_RXT_SETMASK (3<<HW_ARC_ENDPTCTRL_RXT_BITPOS)
#define HW_ARC_ENDPTCTRL_RXI_SETMASK (1<<HW_ARC_ENDPTCTRL_RXI_BITPOS)
#define HW_ARC_ENDPTCTRL_RXR_SETMASK (1<<HW_ARC_ENDPTCTRL_RXR_BITPOS)
#define HW_ARC_ENDPTCTRL_RXE_SETMASK (1<<HW_ARC_ENDPTCTRL_RXE_BITPOS)
#define HW_ARC_ENDPTCTRL_TXS_SETMASK (1<<HW_ARC_ENDPTCTRL_TXS_BITPOS)
#define HW_ARC_ENDPTCTRL_TXD_SETMASK (1<<HW_ARC_ENDPTCTRL_TXD_BITPOS)
#define HW_ARC_ENDPTCTRL_TXT_SETMASK (3<<HW_ARC_ENDPTCTRL_TXT_BITPOS)
#define HW_ARC_ENDPTCTRL_TXI_SETMASK (1<<HW_ARC_ENDPTCTRL_TXI_BITPOS)
#define HW_ARC_ENDPTCTRL_TXR_SETMASK (1<<HW_ARC_ENDPTCTRL_TXR_BITPOS)

// We need to equate the following label like this due to a sign extension problem
// if equated like so (1<<HW_ARC_ENDPTCTRL_TXE_BITPOS)
#define HW_ARC_ENDPTCTRL_TXE_SETMASK (0x00800000)
//HW_ARC_ENDPTCTRL_TXE_SETMASK    equ     (1<<HW_ARC_ENDPTCTRL_TXE_BITPOS)

#define HW_ARC_ENDPTCTRL_RXS_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXS_SETMASK)
#define HW_ARC_ENDPTCTRL_RXD_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXD_SETMASK)
#define HW_ARC_ENDPTCTRL_RXT_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXT_SETMASK)
#define HW_ARC_ENDPTCTRL_RXI_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXI_SETMASK)
#define HW_ARC_ENDPTCTRL_RXR_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXR_SETMASK)
#define HW_ARC_ENDPTCTRL_RXE_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_RXE_SETMASK)
#define HW_ARC_ENDPTCTRL_TXS_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXS_SETMASK)
#define HW_ARC_ENDPTCTRL_TXD_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXD_SETMASK)
#define HW_ARC_ENDPTCTRL_TXT_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXT_SETMASK)
#define HW_ARC_ENDPTCTRL_TXI_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXI_SETMASK)
#define HW_ARC_ENDPTCTRL_TXR_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXR_SETMASK)
#define HW_ARC_ENDPTCTRL_TXE_CLRMASK (~(WORD)HW_ARC_ENDPTCTRL_TXE_SETMASK)


typedef union               
{
    struct {
        int RXS             :1;
        int RXD             :1;
        int RXT             :2;
        int                 :1;
        int RXI             :1;
        int RXR             :1;
        int RXE             :1;
        int                 :8;
        int TXS             :1;
        int TXD             :1;
        int TXT             :2;
        int                 :1;
        int TXI             :1;
        int TXR             :1;
        int TXE             :1;
        int                 :24;
    } B;
    DWORD I;
} endptctrl_type;
//#define HW_ARC_ENDPTCTRL[n] (*(volatile usbintr_type _X*) (HW_ARC_BASEADDR+0x1c0+((n)*4)))    

/////////////////////////////////////////////////////////////////////////////////
//  USB PHY Powerdown Register (HW_USBPHYPWD) Bit Definitions

#define HW_USBPHYPWD_TXDISCON1500_BITPOS (5)
#define HW_USBPHYPWD_PLLVCOPWD_BITPOS (6)
#define HW_USBPHYPWD_PLLVCPPWD_BITPOS (7)
#define HW_USBPHYPWD_TXPWDFS_BITPOS (10)
#define HW_USBPHYPWD_TXPWDIBIAS_BITPOS (11)
#define HW_USBPHYPWD_TXPWDV2I_BITPOS (12)
#define HW_USBPHYPWD_TXPWDVBG_BITPOS (13)
#define HW_USBPHYPWD_TXPWDCOMP_BITPOS (14)
#define HW_USBPHYPWD_RXPWDDISCONDET_BITPOS (16)
#define HW_USBPHYPWD_RXPWDENV_BITPOS (17)
#define HW_USBPHYPWD_RXPWD1PT1_BITPOS (18)
#define HW_USBPHYPWD_RXPWDDIFF_BITPOS (19)
#define HW_USBPHYPWD_RXPWDRX_BITPOS (20)
#define HW_USBPHYPWD_PWDIBIAS_BITPOS (22)
#define HW_USBPHYPWD_REGRESET_BITPOS (23)

#define HW_USBPHYPWD_TXDISCON1500_SETMASK (1<<HW_USBPHYPWD_TXDISCON1500_BITPOS)
#define HW_USBPHYPWD_PLLVCOPWD_SETMASK (1<<HW_USBPHYPWD_PLLVCOPWD_BITPOS)
#define HW_USBPHYPWD_PLLVCPPWD_SETMASK (1<<HW_USBPHYPWD_PLLVCPPWD_BITPOS)
#define HW_USBPHYPWD_TXPWDFS_SETMASK (1<<HW_USBPHYPWD_TXPWDFS_BITPOS)
#define HW_USBPHYPWD_TXPWDIBIAS_SETMASK (1<<HW_USBPHYPWD_TXPWDIBIAS_BITPOS)
#define HW_USBPHYPWD_TXPWDV2I_SETMASK (1<<HW_USBPHYPWD_TXPWDV2I_BITPOS)
#define HW_USBPHYPWD_TXPWDVBG_SETMASK (1<<HW_USBPHYPWD_TXPWDVBG_BITPOS)
#define HW_USBPHYPWD_TXPWDCOMP_SETMASK (1<<HW_USBPHYPWD_TXPWDCOMP_BITPOS)
#define HW_USBPHYPWD_RXPWDDISCONDET_SETMASK (1<<HW_USBPHYPWD_RXPWDDISCONDET_BITPOS)
#define HW_USBPHYPWD_RXPWDENV_SETMASK (1<<HW_USBPHYPWD_RXPWDENV_BITPOS)
#define HW_USBPHYPWD_RXPWD1PT1_SETMASK (1<<HW_USBPHYPWD_RXPWD1PT1_BITPOS)
#define HW_USBPHYPWD_RXPWDDIFF_SETMASK (1<<HW_USBPHYPWD_RXPWDDIFF_BITPOS)
#define HW_USBPHYPWD_RXPWDRX_SETMASK (1<<HW_USBPHYPWD_RXPWDRX_BITPOS)
#define HW_USBPHYPWD_PWDIBIAS_SETMASK (1<<HW_USBPHYPWD_PWDIBIAS_BITPOS)
#define HW_USBPHYPWD_REGRESET_SETMASK (1<<HW_USBPHYPWD_REGRESET_BITPOS)

#define HW_USBPHYPWD_TXDISCON1500_CLRMASK (~(WORD)HW_USBPHYPWD_TXDISCON1500_SETMASK)
#define HW_USBPHYPWD_PLLVCOPWD_CLRMASK (~(WORD)HW_USBPHYPWD_PLLVCOPWD_SETMASK)
#define HW_USBPHYPWD_PLLVCPPWD_CLRMASK (~(WORD)HW_USBPHYPWD_PLLVCPPWD_SETMASK)
#define HW_USBPHYPWD_TXPWDFS_CLRMASK (~(WORD)HW_USBPHYPWD_TXPWDFS_SETMASK)
#define HW_USBPHYPWD_TXPWDIBIAS_CLRMASK (~(WORD)HW_USBPHYPWD_TXPWDIBIAS_SETMASK)
#define HW_USBPHYPWD_TXPWDV2I_CLRMASK (~(WORD)HW_USBPHYPWD_TXPWDV2I_SETMASK)
#define HW_USBPHYPWD_TXPWDVBG_CLRMASK (~(WORD)HW_USBPHYPWD_TXPWDVBG_SETMASK)
#define HW_USBPHYPWD_TXPWDCOMP_CLRMASK (~(WORD)HW_USBPHYPWD_TXPWDCOMP_SETMASK)
#define HW_USBPHYPWD_RXPWDDISCONDET_CLRMASK (~(WORD)HW_USBPHYPWD_RXPWDDISCONDET_SETMASK)
#define HW_USBPHYPWD_RXPWDENV_CLRMASK (~(WORD)HW_USBPHYPWD_RXPWDENV_SETMASK)
#define HW_USBPHYPWD_RXPWD1PT1_CLRMASK (~(WORD)HW_USBPHYPWD_RXPWD1PT1_SETMASK)
#define HW_USBPHYPWD_RXPWDDIFF_CLRMASK (~(WORD)HW_USBPHYPWD_RXPWDDIFF_SETMASK)
#define HW_USBPHYPWD_RXPWDRX_CLRMASK (~(WORD)HW_USBPHYPWD_RXPWDRX_SETMASK)
#define HW_USBPHYPWD_PWDIBIAS_CLRMASK (~(WORD)HW_USBPHYPWD_PWDIBIAS_SETMASK)
#define HW_USBPHYPWD_REGRESET_CLRMASK (~(WORD)HW_USBPHYPWD_REGRESET_SETMASK)

typedef union               
{
    struct {
        int                :5;
        int TXDISCON1500   :1;
        int PLLVCOPWD      :1;
        int PLLVCPPWD      :1;
        int                :2;
        int TXPWDFS        :1;
        int TXPWDIBIAS     :1;
        int TXPWDV2I       :1;
        int TXPWDVBG       :1;
        int TXPWDCOMP      :1;
        int                :1;
        int RXPWDDISCONDET :1;
        int RXPWDENV       :1;
        int RXPWD1PT1      :1;
        int RXPWDDIFF      :1;
        int RXPWDRX        :1;
        int                :1;
        int PWDIBIAS       :1;
        int REGRESET       :1;
    } B;
    int I;
} usbphypwd_type;
#define HW_USBPHYPWD      (*(volatile usbphypwd_type _X*) (HW_USBPHY_BASEADDR))    /* USB PHY Powerdown Register */

/////////////////////////////////////////////////////////////////////////////////
//  USB PHY Transmit register (HW_USBPHYTX) Bit Definitions
#define HW_USBPHYTX_TXCAL1500_BITPOS (0)
#define HW_USBPHYTX_TXENCAL1500_BITPOS (5)
#define HW_USBPHYTX_TXHSXCVR_BITPOS (6)
#define HW_USBPHYTX_TXCALIBRATE_BITPOS (7)
#define HW_USBPHYTX_TXCAL45DN_BITPOS (8)
#define HW_USBPHYTX_TXENCAL45DN_BITPOS (13)
#define HW_USBPHYTX_TXHSTERM_BITPOS (14)
#define HW_USBPHYTX_TXSKEW_BITPOS (15)
#define HW_USBPHYTX_TXCAL45DP_BITPOS (16)
#define HW_USBPHYTX_TXENCAL45DP_BITPOS (21)
#define HW_USBPHYTX_TXFSHIZ_BITPOS (22)
#define HW_USBPHYTX_TXCOMPOUT_BITPOS (23)

#define HW_USBPHYTX_TXCAL1500_WIDTH (4)
#define HW_USBPHYTX_TXENCAL1500_WIDTH (1)
#define HW_USBPHYTX_TXHSXCVR_WIDTH (1)
#define HW_USBPHYTX_TXCALIBRATE_WIDTH (1)
#define HW_USBPHYTX_TXCAL45DN_WIDTH (4)
#define HW_USBPHYTX_TXENCAL45DN_WIDTH (1)
#define HW_USBPHYTX_TXHSTERM_WIDTH (1)
#define HW_USBPHYTX_TXSKEW_WIDTH (1)
#define HW_USBPHYTX_TXCAL45DP_WIDTH (4)
#define HW_USBPHYTX_TXENCAL45DP_WIDTH (1)
#define HW_USBPHYTX_TXFSHIZ_WIDTH (1)
#define HW_USBPHYTX_TXCOMPOUT_WIDTH (1)

#define HW_USBPHYTX_TXCAL1500_SETMASK (((1<<HW_USBPHYTX_TXCAL1500_WIDTH)-1)<<HW_USBPHYTX_TXCAL1500_BITPOS)
#define HW_USBPHYTX_TXENCAL1500_SETMASK (((1<<HW_USBPHYTX_TXENCAL1500_WIDTH)-1)<<HW_USBPHYTX_TXENCAL1500_BITPOS)
#define HW_USBPHYTX_TXHSXCVR_SETMASK (((1<<HW_USBPHYTX_TXHSXCVR_WIDTH)-1)<<HW_USBPHYTX_TXHSXCVR_BITPOS)
#define HW_USBPHYTX_TXCALIBRATE_SETMASK (((1<<HW_USBPHYTX_TXCALIBRATE_WIDTH)-1)<<HW_USBPHYTX_TXCALIBRATE_BITPOS)
#define HW_USBPHYTX_TXCAL45DN_SETMASK (((1<<HW_USBPHYTX_TXCAL45DN_WIDTH)-1)<<HW_USBPHYTX_TXCAL45DN_BITPOS)
#define HW_USBPHYTX_TXENCAL45DN_SETMASK (((1<<HW_USBPHYTX_TXENCAL45DN_WIDTH)-1)<<HW_USBPHYTX_TXENCAL45DN_BITPOS)
#define HW_USBPHYTX_TXHSTERM_SETMASK (((1<<HW_USBPHYTX_TXHSTERM_WIDTH)-1)<<HW_USBPHYTX_TXHSTERM_BITPOS)
#define HW_USBPHYTX_TXSKEW_SETMASK (((1<<HW_USBPHYTX_TXSKEW_WIDTH)-1)<<HW_USBPHYTX_TXSKEW_BITPOS)
#define HW_USBPHYTX_TXCAL45DP_SETMASK (((1<<HW_USBPHYTX_TXCAL45DP_WIDTH)-1)<<HW_USBPHYTX_TXCAL45DP_BITPOS)
#define HW_USBPHYTX_TXENCAL45DP_SETMASK (((1<<HW_USBPHYTX_TXENCAL45DP_WIDTH)-1)<<HW_USBPHYTX_TXENCAL45DP_BITPOS)
#define HW_USBPHYTX_TXFSHIZ_SETMASK (((1<<HW_USBPHYTX_TXFSHIZ_WIDTH)-1)<<HW_USBPHYTX_TXFSHIZ_BITPOS)
#define HW_USBPHYTX_TXCOMPOUT_SETMASK (((1<<HW_USBPHYTX_TXCOMPOUT_WIDTH)-1)<<HW_USBPHYTX_TXCOMPOUT_BITPOS)

#define HW_USBPHYTX_TXCAL1500_CLRMASK (~(WORD)HW_USBPHYTX_TXCAL1500_SETMASK)
#define HW_USBPHYTX_TXENCAL1500_CLRMASK (~(WORD)HW_USBPHYTX_TXENCAL1500_SETMASK)
#define HW_USBPHYTX_TXHSXCVR_CLRMASK (~(WORD)HW_USBPHYTX_TXHSXCVR_SETMASK)
#define HW_USBPHYTX_TXCALIBRATE_CLRMASK (~(WORD)HW_USBPHYTX_TXCALIBRATE_SETMASK)
#define HW_USBPHYTX_TXCAL45DN_CLRMASK (~(WORD)HW_USBPHYTX_TXCAL45DN_SETMASK)
#define HW_USBPHYTX_TXENCAL45DN_CLRMASK (~(WORD)HW_USBPHYTX_TXENCAL45DN_SETMASK)
#define HW_USBPHYTX_TXHSTERM_CLRMASK (~(WORD)HW_USBPHYTX_TXHSTERM_SETMASK)
#define HW_USBPHYTX_TXSKEW_CLRMASK (~(WORD)HW_USBPHYTX_TXSKEW_SETMASK)
#define HW_USBPHYTX_TXCAL45DP_CLRMASK (~(WORD)HW_USBPHYTX_TXCAL45DP_SETMASK)
#define HW_USBPHYTX_TXENCAL45DP_CLRMASK (~(WORD)HW_USBPHYTX_TXENCAL45DP_SETMASK)
#define HW_USBPHYTX_TXFSHIZ_CLRMASK (~(WORD)HW_USBPHYTX_TXFSHIZ_SETMASK)
#define HW_USBPHYTX_TXCOMPOUT_CLRMASK (~(WORD)HW_USBPHYTX_TXCOMPOUT_SETMASK)

typedef union               
{
    struct {
        int TXCAL1500          :4;
        int RSVD0              :1;
        int TXENCAL1500        :1;
        int TXHSXCVR           :1;
        int TXCALIBRATE        :1;
        int TXCAL45DN          :4;
        int RSVD1              :1;
        int TXENCAL45DN        :1;
        int TXHSTERM           :1;
        int TXSKEW             :1;
        int TXCAL45DP          :4;
        int RSVD2              :1;
        int TXENCAL45DP        :1;
        int TXFSHIZ            :1;
        int TXCOMPOUT          :1;
    } B;
    int I;
} usbphytx_type;
#define HW_USBPHYTX      (*(volatile usbphytx_type _X*) (HW_USBPHY_BASEADDR+1))    

/////////////////////////////////////////////////////////////////////////////////
//  USB PHY PLL register (HW_USBPHYPLL) Bit Definitions
#define HW_USBPHYPLL_PLLV2ISEL_BITPOS (0)
#define HW_USBPHYPLL_PLLCPDBLIP_BITPOS (5)
#define HW_USBPHYPLL_PLLVCOCLK2_BITPOS (6)
#define HW_USBPHYPLL_PLLVCOCLK24_BITPOS (7)
#define HW_USBPHYPLL_PLLCPNSEL_BITPOS (8)
#define HW_USBPHYPLL_PLLCLKDIVSEL_BITPOS (12)
#define HW_USBPHYPLL_PLLPFDRST_BITPOS (20)
#define HW_USBPHYPLL_PLLCPSHORTLFR_BITPOS (21)
#define HW_USBPHYPLL_PLLVCOKSTART_BITPOS (22)
#define HW_USBPHYPLL_PLLCLKDIVRSTZ_BITPOS (23)

#define HW_USBPHYPLL_PLLV2ISEL_WIDTH (4)
#define HW_USBPHYPLL_PLLCPDBLIP_WIDTH (1)
#define HW_USBPHYPLL_PLLVCOCLK2_WIDTH (1)
#define HW_USBPHYPLL_PLLVCOCLK24_WIDTH (1)
#define HW_USBPHYPLL_PLLCPNSEL_WIDTH (4)
#define HW_USBPHYPLL_PLLCLKDIVSEL_WIDTH (4)
#define HW_USBPHYPLL_PLLPFDRST_WIDTH (1)
#define HW_USBPHYPLL_PLLCPSHORTLFR_WIDTH (1)
#define HW_USBPHYPLL_PLLVCOKSTART_WIDTH (1)
#define HW_USBPHYPLL_PLLCLKDIVRSTZ_WIDTH (1)

#define HW_USBPHYPLL_PLLV2ISEL_SETMASK (((1<<HW_USBPHYPLL_PLLV2ISEL_WIDTH)-1)<<HW_USBPHYPLL_PLLV2ISEL_BITPOS)
#define HW_USBPHYPLL_PLLCPDBLIP_SETMASK (((1<<HW_USBPHYPLL_PLLCPDBLIP_WIDTH)-1)<<HW_USBPHYPLL_PLLCPDBLIP_BITPOS)
#define HW_USBPHYPLL_PLLVCOCLK2_SETMASK (((1<<HW_USBPHYPLL_PLLVCOCLK2_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOCLK2_BITPOS)
#define HW_USBPHYPLL_PLLVCOCLK24_SETMASK (((1<<HW_USBPHYPLL_PLLVCOCLK24_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOCLK24_BITPOS)
#define HW_USBPHYPLL_PLLCPNSEL_SETMASK (((1<<HW_USBPHYPLL_PLLCPNSEL_WIDTH)-1)<<HW_USBPHYPLL_PLLCPNSEL_BITPOS)
#define HW_USBPHYPLL_PLLCLKDIVSEL_SETMASK (((1<<HW_USBPHYPLL_PLLCLKDIVSEL_WIDTH)-1)<<HW_USBPHYPLL_PLLCLKDIVSEL_BITPOS)
#define HW_USBPHYPLL_PLLPFDRST_SETMASK (((1<<HW_USBPHYPLL_PLLPFDRST_WIDTH)-1)<<HW_USBPHYPLL_PLLPFDRST_BITPOS)
#define HW_USBPHYPLL_PLLCPSHORTLFR_SETMASK (((1<<HW_USBPHYPLL_PLLCPSHORTLFR_WIDTH)-1)<<HW_USBPHYPLL_PLLCPSHORTLFR_BITPOS)
#define HW_USBPHYPLL_PLLVCOKSTART_SETMASK (((1<<HW_USBPHYPLL_PLLVCOKSTART_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOKSTART_BITPOS)
#define HW_USBPHYPLL_PLLCLKDIVRSTZ_SETMASK (((1<<HW_USBPHYPLL_PLLCLKDIVRSTZ_WIDTH)-1)<<HW_USBPHYPLL_PLLCLKDIVRSTZ_BITPOS)

#define HW_USBPHYPLL_PLLV2ISEL_CLRMASK (~(WORD)HW_USBPHYPLL_PLLV2ISEL_SETMASK)
#define HW_USBPHYPLL_PLLCPDBLIP_CLRMASK (~(WORD)HW_USBPHYPLL_PLLCPDBLIP_SETMASK)
#define HW_USBPHYPLL_PLLVCOCLK2_CLRMASK (~(WORD)HW_USBPHYPLL_PLLVCOCLK2_SETMASK)
#define HW_USBPHYPLL_PLLVCOCLK24_CLRMASK (~(WORD)HW_USBPHYPLL_PLLVCOCLK24_SETMASK)
#define HW_USBPHYPLL_PLLCPNSEL_CLRMASK (~(WORD)HW_USBPHYPLL_PLLCPNSEL_SETMASK)
#define HW_USBPHYPLL_PLLCLKDIVSEL_CLRMASK (~(WORD)HW_USBPHYPLL_PLLCLKDIVSEL_SETMASK)
#define HW_USBPHYPLL_PLLPFDRST_CLRMASK (~(WORD)HW_USBPHYPLL_PLLPFDRST_SETMASK)
#define HW_USBPHYPLL_PLLCPSHORTLFR_CLRMASK (~(WORD)HW_USBPHYPLL_PLLCPSHORTLFR_SETMASK)
#define HW_USBPHYPLL_PLLVCOKSTART_CLRMASK (~(WORD)HW_USBPHYPLL_PLLVCOKSTART_SETMASK)
#define HW_USBPHYPLL_PLLCLKDIVRSTZ_CLRMASK (~(WORD)HW_USBPHYPLL_PLLCLKDIVRSTZ_SETMASK)

typedef union               
{
    struct {
        int PLLV2ISEL        :4;
        int RSVD0            :1;
        int PLLCPDBLIP       :1;
        int PLLVCOCLK2       :1;
        int PLLVCOCLK24      :1;
        int PLLCPNSEL        :4;
        int PLLCLKDIVSEL     :4;
        int RSVD1            :4;
        int PLLPFDRST        :1;
        int PLLCPSHORTLFR    :1;
        int PLLVCOKSTART     :1;
        int PLLCLKDIVRSTZ    :1;
    } B;
    int I;
} usbphypll_type;
#define HW_USBPHYPLL      (*(volatile usbphypll_type _X*) (HW_USBPHY_BASEADDR+2))    

/////////////////////////////////////////////////////////////////////////////////
//  USB PHY PLL register (HW_USBPHYRX) Bit Definitions
#define HW_USBPHYRX_ENVADJ_BITPOS (0)
#define HW_USBPHYRX_DISCONADJ_BITPOS (4)
#define HW_USBPHYRX_DEBUGMODE_BITPOS (8)
#define HW_USBPHYRX_PLLLKTIMECTL_BITPOS (12)
#define HW_USBPHYRX_PLLCKDIVCTL_BITPOS (16)
#define HW_USBPHYRX_HOSTMODETEST_BITPOS (20)
#define HW_USBPHYRX_FSCKSOURCESEL_BITPOS (21)
#define HW_USBPHYRX_REGRXDBYPASS_BITPOS (22)
#define HW_USBPHYRX_PLLLOCKED_BITPOS (23)

#define HW_USBPHYRX_ENVADJ_WIDTH (4)
#define HW_USBPHYRX_DISCONADJ_WIDTH (4)
#define HW_USBPHYRX_DEBUGMODE_WIDTH (4)
#define HW_USBPHYRX_PLLLKTIMECTL_WIDTH (4)
#define HW_USBPHYRX_PLLCKDIVCTL_WIDTH (4)
#define HW_USBPHYRX_HOSTMODETEST_WIDTH (1)
#define HW_USBPHYRX_FSCKSOURCESEL_WIDTH (1)
#define HW_USBPHYRX_REGRXDBYPASS_WIDTH (1)
#define HW_USBPHYRX_PLLLOCKED_WIDTH (1)

#define HW_USBPHYRX_ENVADJ_SETMASK (((1<<HW_USBPHYRX_ENVADJ_WIDTH)-1)<<HW_USBPHYRX_ENVADJ_BITPOS)
#define HW_USBPHYRX_DISCONADJ_SETMASK (((1<<HW_USBPHYRX_DISCONADJ_WIDTH)-1)<<HW_USBPHYRX_DISCONADJ_BITPOS)
#define HW_USBPHYRX_DEBUGMODE_SETMASK (((1<<HW_USBPHYRX_DEBUGMODE_WIDTH)-1)<<HW_USBPHYRX_DEBUGMODE_BITPOS)
#define HW_USBPHYRX_PLLLKTIMECTL_SETMASK (((1<<HW_USBPHYRX_PLLLKTIMECTL_WIDTH)-1)<<HW_USBPHYRX_PLLLKTIMECTL_BITPOS)
#define HW_USBPHYRX_PLLCKDIVCTL_SETMASK (((1<<HW_USBPHYRX_PLLCKDIVCTL_WIDTH)-1)<<HW_USBPHYRX_PLLCKDIVCTL_BITPOS)
#define HW_USBPHYRX_HOSTMODETEST_SETMASK (((1<<HW_USBPHYRX_HOSTMODETEST_WIDTH)-1)<<HW_USBPHYRX_HOSTMODETEST_BITPOS)
#define HW_USBPHYRX_FSCKSOURCESEL_SETMASK (((1<<HW_USBPHYRX_FSCKSOURCESEL_WIDTH)-1)<<HW_USBPHYRX_FSCKSOURCESEL_BITPOS)
#define HW_USBPHYRX_REGRXDBYPASS_SETMASK (((1<<HW_USBPHYRX_REGRXDBYPASS_WIDTH)-1)<<HW_USBPHYRX_REGRXDBYPASS_BITPOS)
#define HW_USBPHYRX_PLLLOCKED_SETMASK (((1<<HW_USBPHYRX_PLLLOCKED_WIDTH)-1)<<HW_USBPHYRX_PLLLOCKED_BITPOS)

#define HW_USBPHYRX_ENVADJ_CLRMASK (~(WORD)HW_USBPHYRX_ENVADJ_SETMASK)
#define HW_USBPHYRX_DISCONADJ_CLRMASK (~(WORD)HW_USBPHYRX_DISCONADJ_SETMASK)
#define HW_USBPHYRX_DEBUGMODE_CLRMASK (~(WORD)HW_USBPHYRX_DEBUGMODE_SETMASK)
#define HW_USBPHYRX_PLLLKTIMECTL_CLRMASK (~(WORD)HW_USBPHYRX_PLLLKTIMECTL_SETMASK)
#define HW_USBPHYRX_PLLCKDIVCTL_CLRMASK (~(WORD)HW_USBPHYRX_PLLCKDIVCTL_SETMASK)
#define HW_USBPHYRX_HOSTMODETEST_CLRMASK (~(WORD)HW_USBPHYRX_HOSTMODETEST_SETMASK)
#define HW_USBPHYRX_FSCKSOURCESEL_CLRMASK (~(WORD)HW_USBPHYRX_FSCKSOURCESEL_SETMASK)
#define HW_USBPHYRX_REGRXDBYPASS_CLRMASK (~(WORD)HW_USBPHYRX_REGRXDBYPASS_SETMASK)
#define HW_USBPHYRX_PLLLOCKED_CLRMASK (~(WORD)HW_USBPHYRX_PLLLOCKED_SETMASK)

typedef union               
{
    struct {
     int ENVADJ_WIDTH         :4;
     int DISCONADJ_WIDTH      :4;
     int DEBUGMODE_WIDTH      :4;
     int PLLLKTIMECTL_WIDTH   :4;
     int PLLCKDIVCTL_WIDTH    :4;
     int HOSTMODETEST_WIDTH   :1;
     int FSCKSOURCESEL_WIDTH  :1;
     int REGRXDBYPASS_WIDTH   :1;
     int PLLLOCKED_WIDTH      :1;
    } B;
    int I;
} usbphyrx_type;
#define HW_USBPHYRX      (*(volatile usbphyrx_type _X*) (HW_USBPHY_BASEADDR+3))    

#endif


