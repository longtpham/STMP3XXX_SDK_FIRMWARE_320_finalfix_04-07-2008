/*;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description: 
;***********************************************************************/

#ifdef ADPCMTEST
#include <stdio.h>
#endif
#include "defs.h"
#include "wavedefs.h"
#include "bitstreamwave.h"
#include "wavedata.h"
#include "FileSystem.h"


#ifdef ADPCMTEST
extern int _X BitBuf[];
extern int _Y SampleBuf[];
extern FILE *in,*out;
#endif

void ClearSourceBuffer(void);
void Reset_ADPCM_States(void);
void Increment_Counters(void);
extern WORD _X  EncoderSinkBuf[];
extern WORD _Y  EncoderSourceBuf[];


#ifdef MS_ADPCM
extern int _Y first_block_flag;
extern int _Y iDeltaEnc[];
void Encode_MS_ADPCM_header(void);
extern _asmfunc void Encode_MS_ADPCM_Block(int num_channels, WORD _X *EncoderSinkBuf, WORD _Y *EncoderSourceBuf);
#endif

#ifdef IMA_ADPCM
void Encode_IMA_ADPCM_header(void);
extern _asmfunc void Encode_IMA_ADPCM_Block(int num_channels, WORD _X *EncoderSinkBuf, WORD _Y *EncoderSourceBuf);
#endif

#ifdef WINDOWS_PCM
void Encode_PCM_header(void);
extern _asmfunc void Encode_PCM_Block(int num_channels, WORD _X *EncoderSinkBuf, WORD _Y *EncoderSourceBuf);
#endif


void Reset_ADPCM_States()
{
   int i;
#ifdef IMA_ADPCM
    Init_IMA_ADPCM_States();
#endif
#ifdef MS_ADPCM
    for(i = 0; i<2; i++)
        iDeltaEnc[i] = 0;
#endif    
}


void ClearSourceBuffer()
{
    int j;
    for (j = 0; j < ADPCM_SOURCE_BUF_SIZE; j++)
        EncoderSourceBuf[j] = 0;
}




/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Write_Wave_Header
;
;    Type:	Function
;
;       void Write_Wave_Header()
;
;    Description:	Writes the Wave header to the file.
;
;    Inputs:		
;		void
;           
;    Outputs: void
;    Notes: Used to write the wave header at the beginning and end of encoding
;////////////////////////////////////////////////////////////////////////////// 
*/
void Write_Wave_Header()
{
    int i;
	int ret_val;
	WriteDWord(ckidlist[RIFF_CKID]);

	WriteDWord(g_Wave_riffsize);

	WriteDWord(ckidlist[WAVE_CKID]);

	WriteDWord(ckidlist[FMT_CKID]);

	WriteDWord(g_Wave_wavesize);

	WriteWord(g_Wave_wFormatTag);
	
	WriteWord(g_Wave_wChannels);
	
	WriteDWord(g_Wave_dwSamplesPerSec);
	
	WriteDWord(g_Wave_dwAvgBytesPerSec);
if (g_Wave_wFormatTag != WAVE_FORMAT_PCM)
	WriteWord(g_Wave_wBlockAlign);
else
    WriteWord((g_Wave_wBitsPerSample*g_Wave_wChannels) >> 3); 

#ifdef MS_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_ADPCM)
			Encode_MS_ADPCM_header();
		else
#endif
#ifdef IMA_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
			Encode_IMA_ADPCM_header();
		else
#endif
#ifdef G721_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_G721_ADPCM)
			Encode_G721_ADPCM_header();
		else
#endif
#ifdef WINDOWS_PCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
			Encode_PCM_header();
		else
#endif
			ret_val = -1;	// Unknown algo.
    if (g_Wave_wFormatTag != WAVE_FORMAT_PCM)
    {
        // No fact chunk for Windows PCM
    	WriteDWord(ckidlist[FACT_CKID]);
    	WriteDWord(4);			// Size of Fact chunk is fixed
    	WriteDWord(g_Wave_dwWavDuration);
    }
    // Now add Sigmatel LIST chunk.
/*
;       "LIST"              4 bytes
;       chunk size          4 bytes
;       "INFO"              4 bytes
;       "ISFT"              4 bytes
;       chunk size          4 bytes
;       "SIGMATEL",0,0      10 bytes
*/
    WriteDWord(list_ckid);
    WriteDWord(22); // Size of LIST chubk
    WriteDWord(info_ckid); 
    WriteDWord(isft_ckid);
    WriteDWord(10); // Size of ISFT chunk
    for(i = 0; i < 5; i++)
        WriteWord(sigmatel[i]);
    

	// Prepare Data chunk prior to encoding.
	WriteDWord(ckidlist[DATA_CKID]);
	WriteDWord(g_Wave_datasize);
}

/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_MS_ADPCM_header
;
;    Type:	Function
;
;       void Encode_MS_ADPCM_header()
;
;    Description:	
;
;    Inputs:		
;		void
;           
;    Outputs: void
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef MS_ADPCM
void Encode_MS_ADPCM_header()
{
	int i;
	WriteWord(g_Wave_wBitsPerSample);
	WriteWord(g_Wave_wcbSize);
	WriteWord(g_Wave_wnSamplesPerBlock);
	WriteWord(g_Wave_wnNumCoef);
	for (i = 0; i < g_Wave_wnNumCoef; i++)
	{
		WriteWord(g_Wave_iCoef1Array[i]);
		WriteWord(g_Wave_iCoef2Array[i]);
	}
    first_block_flag = 1;       // For first block we need to estimate Delta.
}
#endif


/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_IMA_ADPCM_header
;
;    Type:	Function
;
;       void Encode_IMA_ADPCM_header()
;
;    Description:	
;
;    Inputs:		
;		void
;           
;    Outputs: void
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef IMA_ADPCM
void Encode_IMA_ADPCM_header()
{
	WriteWord(g_Wave_wBitsPerSample);
	WriteWord(g_Wave_wcbSize);
	WriteWord(g_Wave_wnSamplesPerBlock);
}
#endif

/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_PCM_header
;
;    Type:	Function
;
;       void Encode_PCM_header()
;
;    Description:	
;
;    Inputs:		
;		void
;           
;    Outputs: void
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef WINDOWS_PCM
void Encode_PCM_header()
{
	WriteWord(g_Wave_wBitsPerSample);
}
#endif


/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Initialize_Wave_Header
;
;    Type:	Function
;
;       int Initialize_Wave_Header()
;
;    Description:	
;       Called at the beginning of Encoding to prepare the wave header
;    Inputs:		
;		void
;           
;    Outputs: returns 0 on Success. -1 or non zero on Failure
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
int Initialize_Wave_Header(int formattag, int num_channels, int SamplingRate,int NumberOfBits)
{
	int ret_val = 0;
	g_Wave_dwWavDuration = 0;
	g_Wave_wChannels = num_channels;		
	g_Wave_dwSamplesPerSec = SamplingRate;
	g_Wave_wFormatTag = formattag;
    g_Wave_datasize = 0;
    
    ClearSourceBuffer();
    
    // If you add more chunks change this.
if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
    g_Wave_riffsize = 20+30; // WAVE(4)+fmt (4)+cksize(4)+data(4)+cksize(4)
else
    g_Wave_riffsize = 32+30; // WAVE(4)+fmt (4)+cksize(4)+fact(4)+cksize(4)+factdata(4)+data(4)+cksize(4)
// Additional 30 for "Sigmatel" info chunk.
/*
;       "LIST"              4 bytes
;       chunk size          4 bytes
;       "INFO"              4 bytes
;       "ISFT"              4 bytes
;       chunk size          4 bytes
;       "SIGMATEL",0,0      10 bytes
*/

#ifdef MS_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_ADPCM)
        {
        	g_Wave_wBlockAlign = 512; // Hardcode this, because we allocate buffers beforehand.
            if (num_channels == 1)
                g_Wave_wnSamplesPerBlock = 1012; // Based on Hard-code block align
            else 
                g_Wave_wnSamplesPerBlock = 500;
            g_Wave_dwAvgBytesPerSec = (int)((long)SamplingRate*g_Wave_wBlockAlign/g_Wave_wnSamplesPerBlock);
            g_Wave_wBitsPerSample = 4; // Fix this to four. Our algo will support 4 bits per sample only.
            g_Wave_wcbSize = 32; // We will standard structure
            g_Wave_wnNumCoef = 7; // We support standard number of coefficients
            g_Wave_wavesize =  50; // All of the above structure.
            g_Wave_riffsize += 50;
        }
        else 
#endif
#ifdef IMA_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
        {
            g_Wave_wBlockAlign = 512; // Hardcode this, because we allocate buffers beforehand.
            if (num_channels == 1)
                g_Wave_wnSamplesPerBlock = 1017; // Based on Hard-code block align
            else 
                g_Wave_wnSamplesPerBlock = 505;
            g_Wave_dwAvgBytesPerSec = (int)((long)SamplingRate*g_Wave_wBlockAlign/g_Wave_wnSamplesPerBlock);
            g_Wave_wBitsPerSample = 4; // Fix this to four. Our algo will support this only.
            g_Wave_wcbSize = 2; 
            g_Wave_wavesize =  20; // All of the above structure.
            g_Wave_riffsize += 20;
        }
   		else 
#endif
#ifdef WINDOWS_PCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
        {
            // The following is the correct value of wBlockAlign
            // g_Wave_wBlockAlign = (num_channels*NumberOfBits) >> 3; 
            // But we process large blocks, so fudge it temporarily
            g_Wave_wBlockAlign = 504; 

            g_Wave_dwAvgBytesPerSec = (SamplingRate*g_Wave_wChannels*NumberOfBits)>>3;
            g_Wave_wBitsPerSample = NumberOfBits; 
            if (g_Wave_wBitsPerSample == 16)
                g_Wave_wnSamplesPerBlock        = (252>>(g_Wave_wChannels-1));
            else if (g_Wave_wBitsPerSample == 24)
                g_Wave_wnSamplesPerBlock        = (168>>(g_Wave_wChannels-1));
            else
                g_Wave_wnSamplesPerBlock        = (504>>(g_Wave_wChannels-1)); // Got to be 8 bit PCM
            g_Wave_wavesize =  16; // All of the above structure.
            g_Wave_riffsize += 16;
            // Not supported high data rates.
            g_Wave_wMegicWord = ((g_Wave_dwAvgBytesPerSec) >> 1) + ((g_Wave_dwSamplesPerSec) << 1);
            if(g_Wave_wMegicWord > 190000)
                return -1;
            
        }
   		else 
#endif
#ifdef G721_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_G721_ADPCM)
        {
        // To be done latter.
        }
		else
#endif
			ret_val = -1;	// Unknown algo.

    	TotalSamplesPerBlock = g_Wave_wnSamplesPerBlock*num_channels;
		return ret_val;
}

/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Increment_Counters
;
;    Type:	Function
;
;       void Increment_Counters()
;
;    Description:	
;       Called after every block to set the appropriate counters
;    Inputs:		
		
;		void
;           
;    Outputs: void
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
void Increment_Counters()
{
    // Set the following data. Fact chunk duration, Riff chunk size, Data chunk size
    g_Wave_riffsize += g_Wave_wBlockAlign;
    g_Wave_datasize += g_Wave_wBlockAlign;
    g_Wave_dwWavDuration += TotalSamplesPerBlock;
}
                                  


int EncodeADPCMFrame(int num_channels, WORD _X *EncoderSinkBuf, WORD _Y *EncoderSourceBuf, int handle)
{
#ifdef MS_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_ADPCM)
    		Encode_MS_ADPCM_Block(num_channels, EncoderSinkBuf, EncoderSourceBuf);
        else 
#endif
#ifdef IMA_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
    		Encode_IMA_ADPCM_Block(num_channels, EncoderSinkBuf, EncoderSourceBuf);
        else
#endif
#ifdef WINDOWS_PCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
    		Encode_PCM_Block(num_channels, EncoderSinkBuf, EncoderSourceBuf);
        else
#endif
        return -1;
    FSFileWrite(g_Wave_wBlockAlign, handle, MEM_SPACE_X, (INT)(-1),(WORD * _X)EncoderSinkBuf);         
    return 0;
}


void GetSamplestoFade(int handle, WORD *sample, int ch)
{
int i;
WORD temp[2];

#ifdef MS_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_ADPCM)
        {
            // Skip predictor
            FSFileRead(1*ch, handle, MEM_SPACE_Y, (INT)(-1), &temp[0]);
            // Skip Delta
            FSFileRead(2*ch, handle, MEM_SPACE_Y, (INT)(-1), &temp[0]);
            // Read Samples
            FSFileRead(2, handle, MEM_SPACE_Y, (INT)(-1), &sample[0]);
            FSFileRead(2, handle, MEM_SPACE_Y, (INT)(-1), &sample[1]);
            sample[0] = sample[0]<<8;
            sample[1] = sample[1]<<8;
    	}	
        else 
#endif
#ifdef IMA_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
        {
            for (i = 0; i < ch; i++)
            {
                FSFileRead(2, handle, MEM_SPACE_Y, (INT)(-1), &sample[i]);// Samle
                FSFileRead(1, handle, MEM_SPACE_Y, (INT)(-1), &temp[0]); // Skip index
                FSFileRead(1, handle, MEM_SPACE_Y, (INT)(-1), &temp[0]); // Skip reserved
            }
            sample[0] = sample[0]<<8;
            sample[1] = sample[1]<<8;
        }
        else
#endif
#ifdef WINDOWS_PCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
        {
            sample[0] = 0;
            sample[1] = 0;
        }
        else
#endif
        return;
}


int FadeoutPreviousFrames(int _Y *phandle, _packed BYTE *pname, int devicenum)
{
    int handle = *phandle;
    DWORD fposition;
    int bytestorewind, blockstoskip;
    DWORD bytestoskip, newposition;
    WORD sample[2];
    int i,j,ch;

    Reset_ADPCM_States();

    FSFileClose(handle);


    handle = FSFileOpen(pname,READ,devicenum);

    fposition = FSFileSize(handle);

    // Find bytes to rewind.    
    bytestorewind = g_Wave_dwAvgBytesPerSec/10;
    
    blockstoskip = bytestorewind/g_Wave_wBlockAlign;
    
    if (blockstoskip == 0)
        blockstoskip = 1;
        
    bytestoskip = blockstoskip*g_Wave_wBlockAlign;
    
    if ( (newposition = (fposition - bytestoskip) ) <= 0)
        return 0;
    
    fseek(handle,newposition,SEEK_SET);
    
    GetSamplestoFade(handle,&sample[0],g_Wave_wChannels);
    
    FSFileClose(handle);
    
    handle = FSFileOpen(pname,WRITE,devicenum);
    
    fseek(handle,newposition,SEEK_SET);

    j = 0;

    for ( i = 0; i < 24; i++)
    {
        for (ch = 0; ch < g_Wave_wChannels; ch++)
        {
            EncoderSourceBuf[j] = sample[ch];
            sample[ch] = sample[ch] >> 1;
            j++;
        }
    }
    
    // Clear rest of the buffer    
    for (j = 24*g_Wave_wChannels; j < 2*g_Wave_wBlockAlign; j++)
        EncoderSourceBuf[j] = 0;
    
    EncodeADPCMFrame(g_Wave_wChannels, EncoderSinkBuf, EncoderSourceBuf, handle);
    
    Reset_ADPCM_States();    

    ClearSourceBuffer();
    
    blockstoskip--;    
    
    for (i = 0; i < blockstoskip; i++)
    {
        EncodeADPCMFrame(g_Wave_wChannels, EncoderSinkBuf, EncoderSourceBuf, handle);
    }
    
    FSFileClose(handle);
    
    Reset_ADPCM_States();    
    
    handle = FSFileAppend(pname,devicenum);
    
    //fseek(handle,fposition,SEEK_SET);
    
    *phandle = handle;
    
    
    return 0;
}
    
        

                                  

#ifdef ADPCMTEST
int Fill_input_PCM_buffer(int numsamples)
{
	return fread(SampleBuf,1,numsamples,in);
}

/* 
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Increment_Counters
;
;    Type:	Function
;
;       void Encode_ADPCM(int FormatTag, int num_channels)
;
;    Description:	
;       Used by test code. Equivalent of module ISR.
;    Inputs:		
;		void
;           
;    Outputs: void
;    Notes: 
;////////////////////////////////////////////////////////////////////////////// 
*/
void Encode_ADPCM(int FormatTag, int num_channels)
{
#ifdef IMA_ADPCM
    Init_IMA_ADPCM_States();
#endif
	while(Fill_input_PCM_buffer(TotalSamplesPerBlock)== TotalSamplesPerBlock)
	{
#ifdef MS_ADPCM
        if (FormatTag == WAVE_FORMAT_ADPCM)
    		Encode_MS_ADPCM_Block(num_channels);
        else 
#endif
#ifdef IMA_ADPCM
        if (FormatTag == WAVE_FORMAT_IMA_ADPCM)
    		Encode_IMA_ADPCM_Block(num_channels);
        else
#endif
#ifdef WINDOWS_PCM
        if (FormatTag == WAVE_FORMAT_PCM)
    		Encode_PCM_Block(num_channels);
        else
#endif
        return;

		Wave_Flush_Encoded_Bitstream_Buffer(g_Wave_wBlockAlign);
        Increment_Counters();
	}
}


#endif

