/**********************************************************************************
/*                                                       
/* SigmaTel, Inc                                         
/* $Archive:  $ 
/* $Revision: 2 $
/* $Date: 1/24/04 12:14p $
/* Description: detectsilence.c
/* Notes: TBD - Move DCOffset code into a separate file
/*                                                       
/***********************************************************************************/
#include "types.h"
#include "fsapi.h"
#include "sysmem.h"
#include "silencedetection.h"
#include "sysequ.h"
#include "encoderproperties.h"

int SilenceSR; 
int SilencePtr;   
int SilenceCount;
int MaxSilenceCnt;
int MaxDelay;

/*//////////////////////////////////////////////////////////////////////////////

/*  Name:  ComputeDCOffsetInit

    Type:	void ComputeDCOffsetInit(SamplingRate,*SourceBufDesc_Ptr)

    Description: initializes silence variables

    Inputs:		
			1)  sampling rate 
            2)  pointer to the source buffer descriptor
    Outputs:		
			Nil
    Notes: 
			
//////////////////////////////////////////////////////////////////////////////*/
static int DCOffsetPtr;       
static int DCOffsetPwr;             // Blocks desired (power of 2)
static int DCOffsetCnt;             // = 2^DCOffsetPwr;  
static int DCOffsetBlocksProcessed; // Blocks processed
static int DCOffsetSetback;         // Delay timer to start measurements to let ADC/MIXER voltage stabilize
static  _fract DCOffset[2];        

// Max ofset is aprox 2% of full scale
#define MAX_DC_OFFSET 02

extern _fract _asmfunc FindDCOffset(int BlockSize,int Ptr, int NumChannels,int modulo);
extern _P EncoderSampleRateParms parmsTbl[];

_reentrant void ComputeDCOffsetInit(int SamplingRate, int *SourceBufDesc_Ptr)
{  
    
    DCOffsetPtr =  *( SourceBufDesc_Ptr + BD_BufferBaseAddress );
// Use a power of 2 to simplify normalization
// e.g. for 6:  2^6 = 64; 64 * 64 = 4096 ; sampling time is from 4096/48000 = 85ms to  4096/32000 = 128ms   
// e.g. for 5:  2^5 = 32; 64 * 64 = 2048 ; sampling time is from 2048/48000 = 43ms to  2048/32000 = 64ms
// e.g. for 4:  2^4 = 16; 16 * 64 = 1024 ; sampling time is from 1024/48000 = 21ms to  1024/32000 = 32ms  
   
    DCOffsetPwr = 4;   // 32 ms
    DCOffsetCnt = 16;  // (1 << DCOffsetPwr);     // = 2^DCOffsetPwr;  
    DCOffsetSetback = 8; // 50% 0f DCOffsetCnt  (16ms)
     
    if(32000 != SamplingRate) 
    {
        DCOffsetPwr = 5;   // 43ms @48K; 46ms @ 44.1K
        DCOffsetCnt = 32;  // (1 << DCOffsetPwr);     // = 2^DCOffsetPwr;  
//        DCOffsetSetback = 8; // 25% 0f DCOffsetCnt (10.75ms to 16ms)
    }
    
    DCOffsetBlocksProcessed = 0; 
    DCOffset[0] = DCOffset[1] = 0;   
}
/*//////////////////////////////////////////////////////////////////////////////

/*  Name:  ComputeDCOffset

    Type:	void ComputeDCOffset(*SourceBufDesc_Ptr,NumChannels)

    Description: finds DC bias for the number of samples available

    Inputs:		
			1)  pointer to the source buffer descriptor
            2)  number Of Channels
    Outputs:		
			returns 0 for complete
			returns 1 for more to do  
    Notes: Made a funclet to save P_ memory. But, performance may be an issue.
           MP3 recording fails so, un-made funclet and made a regular function
			
//////////////////////////////////////////////////////////////////////////////*/
_reentrant INT ComputeDCOffset(int *SourceBufDesc_Ptr, int NumChannels)  // stmp6964
{
    int RetValue = AUDIO_DETECTED;
    int BlockSize,NumSamples,Ptr;
    int i;
    int idx = g_wEncSamplingRateIdx;
    _fract CurDCOffset;   
    _fract TempResult;
 
    if ( DCOffsetCnt <=  DCOffsetBlocksProcessed ) return 0;   // Already computed DC bias for this recording
    
    // number of samples available to process
    NumSamples = *( SourceBufDesc_Ptr + BD_HdPtr ) - DCOffsetPtr;                
    
    if ( NumSamples < 0 )
    {
        NumSamples = NumSamples + *( SourceBufDesc_Ptr + BD_BufferSize );
    }
       
    // BlockSize 64 for single channel 128 for dual channel
    BlockSize = 64 * NumChannels;                                      
  
    while ( ( BlockSize < NumSamples) && ( DCOffsetBlocksProcessed <  DCOffsetCnt ) )
    {
        if ( 0 < --DCOffsetSetback) break;  // Wait for ADC / MIXER voltages become stable
        DCOffsetBlocksProcessed++;        
        Ptr = DCOffsetPtr;      
        for ( i = 0 ; i < NumChannels ; i++)
        {   // finds DC bias for the block of 64 samples 
            TempResult = FindDCOffset(Ptr, 64 , NumChannels, *( SourceBufDesc_Ptr + BD_BufferModulo ));
            TempResult =  TempResult >> 6;    // TempResult / 64;
            DCOffset[i] +=  TempResult ;     
            Ptr = DCOffsetPtr + 1;
        } 
        //  increment the DCOffset pointer by the number of samples processed
        DCOffsetPtr = ( DCOffsetPtr + BlockSize ) ;   

        //  change for modulo buffer    
        if ( DCOffsetPtr >=  ( *( SourceBufDesc_Ptr + BD_BufferBaseAddress ) + 
                               *( SourceBufDesc_Ptr + BD_BufferSize ) ) )
        {
            DCOffsetPtr =  DCOffsetPtr - ( *( SourceBufDesc_Ptr + BD_BufferSize ) );     
        }
        NumSamples = NumSamples -  BlockSize;
    }

    if ( DCOffsetCnt <=  DCOffsetBlocksProcessed)
    {    
        for ( i = 0 ; i < NumChannels ; i++)
        {
            CurDCOffset = (DCOffset[i] >> DCOffsetPwr); 
            CurDCOffset += DCOffsetADC[i][idx];      
            if ( (-MAX_DC_OFFSET < CurDCOffset) && (CurDCOffset < MAX_DC_OFFSET))   // limit check
            {
                DCOffsetADC[i][idx] = CurDCOffset;           // UpdateDC bias  
            }
            else SystemHalt();  // debug only
        }
        g_wEncADCLChOffset = DCOffsetADC[0][idx];         // L-Chan DC Offset
        g_wEncADCRChOffset = DCOffsetADC[1][idx];         // R-Chan DC Offset
        return 0;
    }
   
    return 1;   // more to do 
  
 }   

  
/*//////////////////////////////////////////////////////////////////////////////

/*  Name:  DetectSilenceInit

    Type:	void DetectSilenceInit(SamplingRate,*SourceBufDesc_Ptr)

    Description: initializes silence variables

    Inputs:		
			1)  sampling rate 
            2)  pointer to the source buffer descriptor
    Outputs:		
			Nil
    Notes: 
			
//////////////////////////////////////////////////////////////////////////////*/
void DetectSilenceInit(int SamplingRate, int *SourceBufDesc_Ptr)
{
    SilenceSR = 0;

    SilencePtr =  *( SourceBufDesc_Ptr + BD_BufferBaseAddress );
       
    SilenceCount = 0;

    MaxSilenceCnt = (SamplingRate*SILENCE_PERIOD)/64;                   // SILENCE_PERIOD = 4.5 secs

    MaxDelay = (SamplingRate/64)*IDLE_SILENCE_PERIOD;                                    // MaxDelay = 1 Min
    
}

/*//////////////////////////////////////////////////////////////////////////////

/*  Name:  DetectSilence

    Type:	void DetectSilence(*SourceBufDesc_Ptr,NumChannels)

    Description: finds silence or audio for the number of samples available

    Inputs:		
			1)  pointer to the source buffer descriptor
            2)  number Of Channels
    Outputs:		
			returns AUDIO_DETECTED or SILENCE_DETECTED 
    Notes: 
			
//////////////////////////////////////////////////////////////////////////////*/
int _reentrant DetectSilence(int *SourceBufDesc_Ptr, int NumChannels)
{
    int RetValue = AUDIO_DETECTED;
    int BlockSize,NumSamples,Ptr;
    int i;
    int Energy[2];

    // number of samples available to process
    NumSamples = *( SourceBufDesc_Ptr + BD_HdPtr ) - SilencePtr;                
    
    if ( NumSamples < 0 )
        NumSamples = NumSamples + *( SourceBufDesc_Ptr + BD_BufferSize );
        
    // BlockSize 64 for single channel 128 for dual channel
    BlockSize = 64*NumChannels;                                      
        
    while ( NumSamples > BlockSize )
    {
        Ptr = SilencePtr;

        for ( i = 0 ; i < NumChannels ; i++)
        {
            // finds energy for the block of 64 samples 
            Energy[i] = FindEnergy(Ptr,64,NumChannels,*( SourceBufDesc_Ptr + BD_BufferModulo )); 
            Ptr = SilencePtr + 1;
        } 
        
        // if energy for the block is below THRESHOLD_ENERGY then increment the SilenceCount
        if ( NumChannels == 2 )
        {
            if ( (Energy[0] < THRESHOLD_ENERGY ) && ( Energy[1] <  THRESHOLD_ENERGY) )
            {
                SilenceCount++;
            }
            else 
            {
                // if audio is detected then reset SilenceCount to zero
                SilenceCount = 0;
            }    
        }
        else
        {
            if ( Energy[0] < THRESHOLD_ENERGY )
            {
                SilenceCount++;
            }
            else 
            {
                SilenceCount = 0;
            }    
        }        

        //  increment the silence pointer by the number of samples processed
        SilencePtr = ( SilencePtr + BlockSize ) ;   

        //  change for modulo buffer    
        if ( SilencePtr >=  ( *( SourceBufDesc_Ptr + BD_BufferBaseAddress ) + *( SourceBufDesc_Ptr + BD_BufferSize ) ) )
        {
            SilencePtr =  SilencePtr - ( *( SourceBufDesc_Ptr + BD_BufferSize ) );     
        }
        NumSamples = NumSamples -  BlockSize;

    }
    // if SilenceCount is greater than MaxSilenceCnt ( equivalent to 4.5 seconds ) then return SILENCE_DETECTED
    if ( SilenceCount >= MaxSilenceCnt )
    {
        RetValue = SILENCE_DETECTED;
    }
    //  else return AUDIO_DETECTED 
    return  RetValue;
}
/*//////////////////////////////////////////////////////////////////////////////

/*  Name:  CheckSilenceBlockCount

    Type:	void DetectSilence(void)

    Description: determines whether end of album reached

    Inputs:		
            Nil
    Outputs:		
		    returns 1 for end of album reached and 0 for end of album not reached
    Notes: 
			
//////////////////////////////////////////////////////////////////////////////*/
int CheckSilenceBlockCount()
{
    if ( SilenceCount > MaxDelay )
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
