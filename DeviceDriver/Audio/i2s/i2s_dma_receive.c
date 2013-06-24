////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: i2s_dma_receive.c
// Description: Driver for the CDSync/I2S DMA module
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hwequ.h"
#include "i2s_dma_receive.h"

// Not included in hwequ.h :(
#include "regscd.h"

    #pragma asm
    global g_I2SDMAReceiveIsrClear
    #pragma endasm

// This function must be implemented for the application
extern _asmfunc void g_I2SDMAReceiveIsr(void);

// This buffer must be defined by the application
extern volatile _circ _X void *g_I2SDMATargetMBuffer;

// This size variable must be defined by the application
// I2S data will be DMA'd in g_I2SDMATargetMBufferSize/2 chunk sizes
extern volatile int g_I2SDMATargetMBufferSize;

// Global vars
volatile unsigned int g_I2SDMAOverflowCount = 0;
volatile unsigned int g_I2SDMADone = 0;
volatile int g_DACsrrCorrection = 1024; // Correction factor for DACSRR

static void I2S_DMA_Recv_Int_Init (void);
static void I2S_DMA_Recv_Init (void);
static void I2S_Recv_CSR_Init (void);
static void I2S_Gpio_Enable (void);
static void _long_interrupt((0x68/2)) CDSyncRxExceptionIsr(void);

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          I2S DMA Receive Interrupt Init
//
//   Type:          Function
//
//   Description:   Sets up the interrupts for DMA done and DMA overflow.
//
//   Inputs:        
//                                                                       
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
static void I2S_DMA_Recv_Int_Init (void)
{

    #pragma asm
    ; init the vector addresses
    move    #>$bf080,x0             ; opcode for JSR
    move    #$0066,r1
    move    #>g_I2SDMAReceiveIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)

    move    #$0068,r1
    move    #>FCDSyncRxExceptionIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)
    #pragma endasm

    // Enable DMA IRQ
    HW_CDSYNCCSR.B.DMAIRQEN = 1;

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void I2S_DMA_Recv_Init (void)
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//                  
//                  
//
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
static void I2S_DMA_Recv_Init (void) 
{

// Enable WCLK, VDDD, and VDDIO as they are controlled by external FETs (Set GPIO to low)   
    HW_GP1DOR.B.B15 = 0;    // GPIO_39: STMP 9222 & STMP 9222 (SD_VDDIO, I2S_WCLK, SD_VDDD)  

    // Bring DMA block out of reset
    HW_CDSYNCCSR.B.RESET = 0;

    // Reset the start of the DMA buffer
    HW_CDSYNCCPR.I = 0;

    // Set the modulo to the size of the output buffer
    HW_CDSYNCMR.I = g_I2SDMATargetMBufferSize-1;

    // Setup channel 0 DMA buffer
    #pragma asm
    move    #g_I2SDMATargetMBuffer,x0
    move    x0,x:$F605          ; set base address to dmabuffer
    #pragma endasm

    // Number of words to read into the DMA buffer
    HW_CDSYNCWCR.I = g_I2SDMATargetMBufferSize>>1;

    // Configuration register setup
    HW_CDSYNCCSR.B.INWRDLEN = 0;    // 16-bit words
    HW_CDSYNCCSR.B.OUTWRLEN = 2;   // 2 bytes per word ???
    HW_CDSYNCCSR.B.INPUTMSB = 1;   // Bits 23:8 are valid
    HW_CDSYNCCSR.B.MODE1 = 0;   // Assume mode = 01 ???
    HW_CDSYNCCSR.B.SYNCEN = 0;   // Disable SYNC-detector, DMA I2S data as is
    HW_CDSYNCCSR.B.DSCRAM = 0;   // Disable de-scrambling, DMA I2S data as is
    HW_CDSYNCCSR.B.CRC = 0;   // Disable CRC calc, DMA I2S data as is
    HW_CDSYNCCSR.B.EN = 1;    // Enable DMA transfers from I2S
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void I2S_Recv_CSR_Init (void)
//
//   Type:          Function
//
//   Description:   Initializes HW_SAIRCSR register for I2S receive from stfm1000
//
//   Inputs:        none        
//
//   Outputs:       none
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
static void I2S_Recv_CSR_Init (void) 
{

    // Receiver Data Word Justification
    HW_SAIRCSR.B.RDWJ = 1;  
    
    // Receiver Relative Timing
    HW_SAIRCSR.B.RREL = 0;  
    
    // Receiver Clock Polarity
    HW_SAIRCSR.B.RCKP = 0;       
    
    // Receiver Left Right Selection
    HW_SAIRCSR.B.RLRS = 0;    
    
    // Receiver Data Shift Direction
    HW_SAIRCSR.B.RDIR = 0;    
    
    // Receiver Word Length Control
    HW_SAIRCSR.B.RWL = 0;    
    
    // Receiver Master Mode Enable
    HW_SAIRCSR.B.RMME = 0;    
    
    // DATAI0_EN
    HW_SAIRCSR.B.REN0 = 1;      // Enable channel 0
    
    // DATAI1_EN
    HW_SAIRCSR.B.REN1 = 0;      // Disable channel 1
    
    // DATAI2_EN
    HW_SAIRCSR.B.REN2 = 0;      // Disable channel 2
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CDSYNC_RX_ISR (CDSYNC Receive Interrupt Sub-Routine)
//
//   Type:          Interrupt
//
//   Description:   
//
//   Inputs:        
//                  
//                  
//
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////

// NOTE: This ISR is implemented in dec_stfm_nonoverlay.asm
////////////////////////////////////////////////////////////
// CDSync Rx interrupt service routine
// IRQ is at P:$0066 = decimal 102  interrupt # = decimal 31
////////////////////////////////////////////////////////////
void g_I2SDMAReceiveClearInt(void) 
{
    unsigned int csr = HW_CDSYNCCSR.I & ~((unsigned int)HW_CDSYNCCSR_DMAOF_SETMASK);

    if( csr & HW_CDSYNCCSR_DMADONE_SETMASK )
    {
        g_I2SDMADone++;

        // Restart the DMA transfer
        // Number of words to read into the DMA buffer
        HW_CDSYNCWCR.I = g_I2SDMATargetMBufferSize>>1;
    }
    HW_CDSYNCCSR.I = csr;
}    
     
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CDSYNC_RX_EXCEPTION_ISR
//
//   Type:          Interrupt
//
//   Description:   Handles overflows.  
//
//   Inputs:        
//                  
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// CDSync Rx exception interrupt service routine
// IRQ is at P:$0068 = decimal 104  interrupt # = decimal 32
////////////////////////////////////////////////////////////
static void _long_interrupt((0x68/2)) CDSyncRxExceptionIsr(void) {

    unsigned int csr = HW_CDSYNCCSR.I & ~((unsigned int)HW_CDSYNCCSR_DMADONE_SETMASK);

    if( csr & HW_CDSYNCCSR_DMAOF_SETMASK /*HW_CDSYNCCSR.B.DMAOF*/ )
    {
        g_I2SDMAOverflowCount++;
//         HW_GP1DOR.B.B12 = 1;   //DEBUG ONLY
//         HW_GP1DOR.B.B12 = 0;   //DEBUG ONLY
//         HW_GP1DOR.B.B12 = 1;   //DEBUG ONLY
         
//         HW_DACSRR.I = HW_DACSRR.I - g_DACsrrCorrection;   // Speed up DAC sample rate so I2S overrun will occur less often
//         g_DACsrrCorrection = g_DACsrrCorrection/2;        // Cut in half to allow convergence by DACIsr
        // Corresponding slowing of DAC sample rate in DacIsr.asm (by + 32)
    }

    // These next 3 exceptions should never happen in our system
    if( HW_CDSYNCCSR.B.EDC || HW_CDSYNCCSR.B.LOS || HW_CDSYNCCSR.B.SYNC )
    {
        #pragma asm //Break into debugger
            debug
        #pragma endasm
    }

    // Clear exception bits, not the DMA done bit
    HW_CDSYNCCSR.I = csr;
}    

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void I2S_Gpio_Enable (void)
//
//   Type:          Function
//
//   Description:   Enables pins for I2S mode.  
//
//   Inputs:        none      
//                                              
//   Outputs:       none
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
static void I2S_Gpio_Enable (void)  
{
    // Make sure pins are powered
    HW_GP0PWR.B.B0 = 1;    
    HW_GP0PWR.B.B3 = 1;    
    HW_GP0PWR.B.B4 = 1;      

    HW_GP1PWR.B.B15 = 1;    // GPIO_39: STMP 9222 & STMP 9222 (SD_VDDIO, I2S_WCLK, SD_VDDD)  
   
    // Enable I2S mode for pins
    HW_GP0ENR.B.B0 = 0;     // I2S_DataI0
    HW_GP0ENR.B.B3 = 0;     // I2S_WCLK
    HW_GP0ENR.B.B4 = 0;     // I2S_BCLK

// Set as GPIO
    HW_GP1ENR.B.B15 = 1;    // GPIO_39: STMP 9222 & STMP 9222 (SD_VDDIO, I2S_WCLK, SD_VDDD)  
    
// Set as Output Pins   
    HW_GP1DOER.B.B15 = 1;  	// GPIO_39: STMP 9222 & STMP 9222 (SD_VDDIO, I2S_WCLK, SD_VDDD)  
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void I2SDMAReceiveStart(void)
//
//   Type:          Function
//
//   Description:   Starts the I2S DMA receiver
//
//   Inputs:        
//                  
//                  
//
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////

void I2SDMAReceiveStart(void)
{
    // Setup to receive DMA I2S data
    I2S_DMA_Recv_Init();

    // Now setup and capture the data from the i/f
    I2S_Recv_CSR_Init();

    //Initialize gpio's used for clocks and data input
    I2S_Gpio_Enable();

    // Setup DMA interrupt
    I2S_DMA_Recv_Int_Init();
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void I2SDMAReceiveStop(void)
//
//   Type:          Function
//
//   Description:   Stops I2S DMA receiver
//
//   Inputs:        
//                  
//                  
//
//   Outputs:       
//
//   Notes:         
//                  
//                  
//<
////////////////////////////////////////////////////////////////////////////////
void I2SDMAReceiveStop(void)
{

    // Disable the Interrupts for DMA done and DMA overflow
    HW_ICLENABLE1R.B.SEN31 = 0;    // Disable source 31 (CDSync Int)
    HW_ICLENABLE1R.B.SEN32 = 0;    // Disable source 32 (CDSync Exception)
    
    // DATAI0_EN
    HW_SAIRCSR.B.REN0 = 0;      // Disable channel 0
    
    // Disable DMA block
    HW_CDSYNCCSR.I = HW_CDSYNCCSR_RESET_SETMASK;
    
    //**************************************************************************
    // GPIO_39 is the power control of STFM1000 and it should not be disabled. 
    // I2S pins would multiplex with GPIO_00, GPIO_03, and GPIO_04. If STMP1000
    // is not turned on, the ports on STMP1000 would be indeterministic.
    //**************************************************************************
    // Disable WCLK, VDDD, and VDDIO as they are controlled by external FETs (Set GPIO to high)   
    //HW_GP1DOR.B.B15 = 1;    // GPIO_39: STMP 9222 & STMP 9222 (SD_VDDIO, I2S_WCLK, SD_VDDD) 

    // Make sure pins are powered down
    HW_GP0PWR.B.B0 = 0;    
    HW_GP0PWR.B.B3 = 0;    
    HW_GP0PWR.B.B4 = 0;      
     

	// clear I2S CSR
	HW_SAIRCSR.I = 0;

}

