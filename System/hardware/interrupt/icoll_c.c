////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// File : icoll.c
// Description : 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "icoll.h"
#include "regsicoll.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////

// This array maps directly to the ICOLL_SOURCE defs found in icoll.h
WORD wIcollVectors[ICOLL_NUM_SOURCES] = 
{
    HW_IVECDACE,
    HW_IVECDACUF,
    HW_IVECADCF,
    HW_IVECADCOF,
    HW_IVECFD,
    HW_IVECCF,
    HW_IVECSMTO,
    HW_IVECSMIP,
    HW_IVECCFNC,
    HW_IVECCFSC,
    HW_IVECGPIO0,
    HW_IVECGPIO1,
    HW_IVECGPIO2,
    HW_IVECTIMER0,
    HW_IVECTIMER1,
    HW_IVECTIMER2,
    HW_IVECTIMER3,
    HW_IVECGPIO3,
    HW_IVECSDRAM,
    HW_IVECCDI,
#if (defined(DEVICE_3500))
    HW_IVECVDD5VCONN,                        
    HW_IVECUSBCTLR,                          
    HW_IVECUSBWAKEUP,                        
    HW_IVECVDD5VDISC,                        
    HW_IVECESPI,                             
    HW_IVECFILCO,                            
    HW_IVECLRADC1,                           
    HW_IVECRTCALARM,                         
    HW_IVECLRADC2,                           
    HW_IVECHWECC,
    0x0064,                 // Unused in 3500                            
#else
    HW_IVECUSBCR,
    HW_IVECUSRSIR,
    HW_IVECUSBHR,
    HW_IVECUSBEP0,
    HW_IVECUSBEP1,
    HW_IVECUSBEP2,
    HW_IVECUSBEP3,
    HW_IVECUSBEP4,
    HW_IVECUSBEP5,
    HW_IVECUSBEP6,
    HW_IVECUSBEP7,    
#endif
    HW_IVECCDSYNC,
    HW_IVECCDSYNCEX,
    HW_IVECRS,
    HW_IVECI2CRXDR,
    HW_IVECI2CRXOF,
    HW_IVECI2CTXDE,
    HW_IVECI2CTXUF,
    HW_IVECSPI,
    HW_IVECI2SRXOF,
    HW_IVECI2STXUF,
    HW_IVECI2SRXDF,
    HW_IVECI2STXDE,
    HW_IVECRESET,
    HW_IVECSTERR,
    HW_IVECTRAC,
    HW_IVECSWI,
    HW_IVECIRQA,
    HW_IVECIRQB,
    HW_IVECILI
};

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollShutDown
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant IcollShutDown(void)
{

    HW_ICLENABLE0R.I = 0;
    HW_ICLENABLE1R.I = 0;
    HW_ICLSTATUS0R.I = 0;
    HW_ICLSTATUS1R.I = 0;
    HW_ICLPRIOR0R.I = 0;
    HW_ICLPRIOR1R.I = 0;
    HW_ICLPRIOR2R.I = 0;
    HW_ICLPRIOR3R.I = 0;
    HW_ICLSTEER0R.I = 0;
    HW_ICLSTEER1R.I = 0;
    HW_ICLSTEER2R.I = 0;
    
    // TODO:  Shutdown all other "sources"

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollInterruptEnable
//
//   Type:          Function
//
//   Description:   dwSource            Icoll Source (use defs in icoll.h)
//
//   Inputs:        RETCODE
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant IcollEnable(DWORD dwSource)
{

    if(dwSource <= ICOLL_ICLENABLE_REG_0_LAST_SRC)
    {
        HW_ICLENABLE0R.I |= (1 << ((WORD)(dwSource)));
        return SUCCESS;
    }
    else if(dwSource <= ICOLL_END_OF_TRUE_SOURCES)
    {
        dwSource -= 24;
        HW_ICLENABLE1R.I |= (1 << ((WORD)(dwSource)));
        return SUCCESS;
    }
    else if(dwSource <= ICOLL_SOURCE_END)
    {
        // TODO:  Handle the non-icoll "sources"
        #pragma asm
            debug
        #pragma endasm
    }
    else
        return ICOLL_ERROR_INVALID_SOURCE;
    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollInterruptDisable
//
//   Type:          Function
//
//   Description:   dwSource            Icoll Source (use defs in icoll.h)
//
//   Inputs:        RETCODE
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant IcollDisable(DWORD dwSource)
{

    if(dwSource <= ICOLL_ICLENABLE_REG_0_LAST_SRC)
    {
        HW_ICLENABLE0R.I &= ~(1 << ((WORD)(dwSource)));
        return SUCCESS;
    }
    else if(dwSource <= ICOLL_END_OF_TRUE_SOURCES)
    {
        dwSource -= 24;
        HW_ICLENABLE1R.I &= ~(1 << ((WORD)(dwSource)));
        return SUCCESS;
    }
    else if(dwSource <= ICOLL_SOURCE_END)
    {
        // TODO:  Handle the non-icoll "sources"
        SystemHalt();
    }
    else
        return ICOLL_ERROR_INVALID_SOURCE;
    
    // Make the compiler happy
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollGetStatus
//
//   Type:          Function
//
//   Description:   dwSource            Icoll Source (use defs in icoll.h)
//
//   Inputs:        BOOL                TRUE => interrupt pending
//                                      FALSE => interrupt not pending
//
//   Outputs:       none
//
//   Notes:         Invalid dwSource returns FALSE
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant IcollGetStatus(DWORD dwSource)
{

    if(dwSource <= ICOLL_ICLENABLE_REG_0_LAST_SRC)
    {
        if(HW_ICLSTATUS0R.I & (1 << ((WORD)(dwSource))))
            return TRUE;
        else
            return FALSE;    
    }
    else if(dwSource <= ICOLL_END_OF_TRUE_SOURCES)
    {
        dwSource -= 24;
        if(HW_ICLSTATUS1R.I & (~(1 << ((WORD)(dwSource)))))
            return TRUE;
        else
            return FALSE;    
    }
    else if(dwSource <= ICOLL_SOURCE_END)
    {
        // TODO:  Handle the non-icoll "sources"
        SystemHalt();
    }
    else
        return FALSE;
    
    // Make the compiler happy
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollSetPriority
//
//   Type:          Function
//
//   Description:   dwSource            Icoll Source (use defs in icoll.h)
//
//   Inputs:        RETCODE
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant IcollSetPriority(DWORD dwSource, WORD wPriority)
{
    WORD wPriorityRegister;
    
    if(dwSource < 8)
    {
        HW_ICLPRIOR0R.I &= ~(7<<((WORD)(dwSource)*3));
        HW_ICLPRIOR0R.I |= wPriority<<((WORD)(dwSource)*3);
    }
    else if(dwSource < 16)
    {
        dwSource -= 8;
        HW_ICLPRIOR1R.I &= ~(7<<((WORD)(dwSource)*3));
        HW_ICLPRIOR1R.I |= wPriority<<((WORD)(dwSource)*3);
    }
    else if(dwSource < 24)
    {
        dwSource -= 16;
        HW_ICLPRIOR2R.I &= ~(7<<((WORD)(dwSource)*3));
        HW_ICLPRIOR2R.I |= wPriority<<((WORD)(dwSource)*3);
    }
    else if(dwSource < 32)
    {
        dwSource -= 24;
        HW_ICLPRIOR3R.I &= ~(7<<((WORD)(dwSource)*3));
        HW_ICLPRIOR3R.I |= wPriority<<((WORD)(dwSource)*3);
    }
    else if(dwSource <= ICOLL_END_OF_TRUE_SOURCES)
    {
        dwSource -= 32;
        HW_ICLPRIOR4R.I &= ~(7<<((WORD)(dwSource)*3));
        HW_ICLPRIOR4R.I |= wPriority<<((WORD)(dwSource)*3);
    }
    else
        return ICOLL_ERROR_INVALID_SOURCE;        
    
    return SUCCESS;    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollSetSteering
//
//   Type:          Function
//
//   Description:   dwSource            Icoll Source (use defs in icoll.h)
//                  wSteering           
//
//   Inputs:        RETCODE
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant IcollSetSteering(DWORD dwSource, WORD wSteering)
{
    
    if(dwSource < 12)
    {
        HW_ICLSTEER0R.I &= ~(3<<((WORD)(dwSource)*2));
        HW_ICLSTEER0R.I |= wSteering<<((WORD)(dwSource)*2);
    }
    else if(dwSource < 24)
    {
        dwSource -= 12;
        HW_ICLSTEER1R.I &= ~(3<<((WORD)(dwSource)*2));
        HW_ICLSTEER1R.I |= wSteering<<((WORD)(dwSource)*2);
    }
    else if(dwSource <= ICOLL_END_OF_TRUE_SOURCES)
    {
        dwSource -= 24;
        HW_ICLSTEER2R.I &= ~(3<<((WORD)(dwSource)*2));
        HW_ICLSTEER2R.I |= wSteering<<((WORD)(dwSource)*2);
    }
    else
        return ICOLL_ERROR_INVALID_SOURCE;        
    
    return SUCCESS;    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          IcollInstallIsrVector
//
//   Type:          Function
//
//   Description:   
//                  
//   Inputs:        pVector             Address for ISR vector function
//                  wSource             Icoll Source (use defs in icoll.h)
//
//   Outputs:       RETCODE             (always SUCCESS for now)
//
//   Notes:         No error checking on pSource
//                  Not portable C code
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant IcollInstallIsrVector(ISR_VECTOR pVector, WORD wSource)
{
    #pragma asm
        move    a1,n1                   ; get the offset to the source
        move    #FwIcollVectors,r1      ; get the vector address table
        move    #>$0BF080,x0            ; get the jsr instruction opcode
        move    y:(r1+n1),r1            ; get the vector address
        nop
        move    x0,p:(r1)+              ; write the jsr opcode
        move    r0,p:(r1)-              ; write the vector address
    #pragma endasm
    
    return SUCCESS;
}

