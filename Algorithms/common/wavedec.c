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

#ifdef ADPCMTEST
extern int _X BitBuf[];
extern int _Y SampleBuf[];
extern FILE *in,*out;
#endif

//----------------------------------------------------------------------
// Function Declarations for C
int ProcessChunk(int ckIDval, long int *cksize);
int GetckID(void);
void Process_Common_Fmt_Header(void);
extern _asmfunc long GetDWord(void);
extern _asmfunc int GetWord(void);
extern _asmfunc int GetSWord(void);
extern _asmfunc long Get_file_offset(void);
int waveparser(void);

#ifdef MS_ADPCM
void Decode_MS_ADPCM_header(long int *cksize);
#endif

#ifdef IMA_ADPCM
int Decode_IMA_ADPCM_header(void);
#endif

#ifdef WINDOWS_PCM
int Decode_PCM_header(void);
#endif

//---------------------------------------------------------------------
// Function Declarations for assembly code.
#ifdef MS_ADPCM
extern _asmfunc void Decode_MS_ADPCM_Block();
#endif

#ifdef IMA_ADPCM
extern _asmfunc void Decode_MS_ADPCM_Block();
#endif

#ifdef WINDOWS_PCM
extern _asmfunc void Decode_PCM_Block();
#endif



/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  waveparser
;
;    Type:	Function
;
;       int waveparser()
;
;    Description:	Parses up to Data chunk.
;
;    Inputs:		
;			void
;           
;    Outputs:		
;			retval is 0, if Success
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
int waveparser()
{
	int ret_val;
	int ckIDval=0;
	long int ckSize;
	int chunklevel = 0;
    riff_file_flag=0;
	while((ckIDval = GetckID())>=0)		// Returns -1 on error
	{
		ckSize = GetDWord();
		// Subtract this chunk size from the current level
		DataSize[chunklevel] -= (ckSize + 8);
		// Skip unknown chunks
		if (ckIDval == UNKNOWN_CKID)
		{
			SkipData(ckSize);	// Align the file reads correctly.
		}
		else
		{
			ret_val = ProcessChunk(ckIDval, &ckSize);
			if(ret_val == 1)
			{
				// Contains nested chunks. Go one level up.
				chunklevel++;
				DataSize[chunklevel] = ckSize;
			}
            else if(ret_val == 2)
            {
    			return 0;       // Found Data chunk. Our header ends here.
            }
			else if (ret_val < 0)
				return 1; // Failed,  Inform the top level module.
		}
		
		// Check whether all the data at this level is exhausted
		// chunklevel 0 will always contain negative value.
		if(DataSize[chunklevel] <= 0)
		{
			// Go down one level
			chunklevel--;
			if (chunklevel<=0)
            {
                Seek_data_offset(g_Wave_data_offset);
				return 0;			// Done!
            }
		}
	}
    // The file is completely screwed up. Could'nt find the riff chunk
    if (riff_file_flag == 0)
        return -1;
    
    
        
    return 0;    
}


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  ProcessChunk
;
;    Type:	Function
;
;       int ProcessChunk(int ckIDval, long int *cksize)
;
;    Description:	Processes a chunk
;
;    Inputs:		
;			chunk id value, chunk size
;           
;    Outputs:		
;			0 = if there are no nested chunks.
;           1 = if there are nested chunks
;           2 = if it is a Data chunk
;           -1 = FAILED
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/

int ProcessChunk(int ckIDval, long int *cksize)
{
int i;
int ret_val = 0;
	switch(ckIDval)
	{
	case RIFF_CKID:
        // This is an extended Ckid
		// check if 'W','A','V','E' is present.
		// We suppport only wave files.
		ckIDval = GetckID();
		if (ckIDval == WAVE_CKID)
		{
			*cksize -= 4;
			ret_val = 1;
            riff_file_flag = 1;
		}
		else
			ret_val = -1;
		break;

	case FMT_CKID:
		Process_Common_Fmt_Header();
        *cksize -= 14;
#ifdef MS_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_ADPCM)
			Decode_MS_ADPCM_header(cksize);
   		else 
#endif
#ifdef IMA_ADPCM
        if (g_Wave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
        {
			ret_val = Decode_IMA_ADPCM_header();
            *cksize -= 6;
        }
		else
#endif
#ifdef G721_ADPCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_G721_ADPCM)
			Decode_G721_ADPCM_header();
		else
#endif
#ifdef WINDOWS_PCM
		if (g_Wave_wFormatTag == WAVE_FORMAT_PCM)
        {
			ret_val = Decode_PCM_header();
            *cksize -= 2;
        }
		else
#endif

			ret_val = -1;	// Unknown algo.
        
        if (*cksize > 0)
        {   
            for(i = 0; i < *cksize; i++)
            {
                GetByte();
            }
        }
		break;

	case FACT_CKID:
		g_Wave_dwWavDuration = GetDWord();
		ret_val = 0;
		break;

	case DATA_CKID:
		g_Wave_data_offset = Get_file_offset();	// Store the offset to be passed to the algo.
		g_Wave_datachunksize = *cksize;
		ret_val = 2;
		break;
	default:
		ret_val = 0;
		break;

	}
	return ret_val;
}



/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  GetckID
;
;    Type:	Function
;
;       int GetckID()
;
;    Description:	Find the chunk id
;
;    Inputs:		
;			void
;           
;    Outputs: Returns ckid
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
int GetckID()
{
	int i;
	long int word = GetDWord();
	// Default to unknown
	int ckid = UNKNOWN_CKID;
	for (i = 0; i < TOTAL_CKID; i++)
	{
		if (word == ckidlist[i])
		{
			ckid = i;
			break;
		}
	}
	return ckid;
}

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Process_Common_Fmt_Header
;
;    Type:	Function
;
;       void Process_Common_Fmt_Header()
;
;    Description:	Processes Fmt header (common part)
;
;    Inputs:		
;			void
;           
;    Outputs:		
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/

void Process_Common_Fmt_Header()
{
	g_Wave_wFormatTag			= GetWord();
	g_Wave_wChannels			= GetWord();
	g_Wave_dwSamplesPerSec	= GetDWord();
	g_Wave_dwAvgBytesPerSec	= GetDWord();
	g_Wave_wBlockAlign		= GetWord();
}


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_MS_ADPCM_header
;
;    Type:	Function
;
;       void Decode_MS_ADPCM_header()
;
;    Description:	Processes MS ADPCM header 
;
;    Inputs:		
;			void
;           
;    Outputs:		
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef MS_ADPCM
void Decode_MS_ADPCM_header(long int *cksize)
{
	int i;
	g_Wave_wBitsPerSample			= GetWord();
	g_Wave_wcbSize					= GetWord();
	g_Wave_wnSamplesPerBlock		= GetWord();
	g_Wave_wnNumCoef				= GetWord();
    *cksize -= 8;
	for (i = 0; i < g_Wave_wnNumCoef; i++)
	{
		g_Wave_wCoef1[i]	= GetSWord();
		g_Wave_wCoef2[i]	= GetSWord();
        *cksize -= 4;
	}
    
}       

#endif


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_IMA_ADPCM_header
;
;    Type:	Function
;
;       void Decode_IMA_ADPCM_header()
;
;    Description:	Processes IMA ADPCM header 
;
;    Inputs:		
;			void
;           
;    Outputs:		
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef IMA_ADPCM
int Decode_IMA_ADPCM_header()
{
	g_Wave_wBitsPerSample			= GetWord();
	g_Wave_wcbSize					= GetWord();
	g_Wave_wnSamplesPerBlock		= GetWord();
    if(g_Wave_wBitsPerSample == 4)
       return 0;
    else
       return -1;
}
#endif

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_G721_ADPCM_header
;
;    Type:	Function
;
;       void Decode_G721_ADPCM_header()
;
;    Description:	Processes G721 ADPCM header 
;
;    Inputs:		
;			void
;           
;    Outputs:		
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef G721_ADPCM
void Decode_G721_ADPCM_header()
{
	g_Wave_wBitsPerSample			= GetWord();
	g_Wave_wcbSize					= GetWord();
	g_Wave_wnAuxBlockSize			= GetWord();
}
#endif

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_PCM_header
;
;    Type:	Function
;
;       void Decode_PCM_header()
;
;    Description:	Processes PCM Wave header 
;
;    Inputs:		
;			void
;           
;    Outputs:		
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
#ifdef WINDOWS_PCM
int Decode_PCM_header()
{
	g_Wave_wBitsPerSample			= GetWord();
    // Fudge the Block Align to process more samples, simultaneously, so rest of the processing is unchanged
    g_Wave_wBlockAlign              = 504; // This ensures that we can process both 24 bit and 16 bit sample blocks
    // Dummy variable below for processing blocks of data.
    if (g_Wave_wBitsPerSample == 16)
        g_Wave_wnSamplesPerBlock        = (252>>(g_Wave_wChannels-1));
    else if (g_Wave_wBitsPerSample == 24)
        g_Wave_wnSamplesPerBlock        = (168>>(g_Wave_wChannels-1));
    else
        g_Wave_wnSamplesPerBlock        = (504>>(g_Wave_wChannels-1)); // Got to be 8 bit PCM

    // Not supported high data rates.
    g_Wave_wMegicWord = ((g_Wave_dwAvgBytesPerSec) >> 1) + ((g_Wave_dwSamplesPerSec) << 1);
    if(g_Wave_wMegicWord > 190000)
        return -1;
        
    return 0;

}
#endif


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  GetSamplestoDecode
;
;    Type:	Function
;
;       int GetSamplestoDecode()
;
;    Description:	Gets Samples to Process for a normal block
;
;    Inputs:		
;			Format Tag
;           
;    Outputs: Samples to Process
;    Notes: Can be overlaid. NOT USED
;////////////////////////////////////////////////////////////////////////////// 
*/
int GetSamplestoDecode(int FormatTag)
{
    int ret_val = 0;
    if (FormatTag == WAVE_FORMAT_ADPCM)
    {
        ret_val =  g_Wave_wnSamplesPerBlock - 2;
    }
    return ret_val;
}


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  GetSamplestoDecodeLastBlock
;
;    Type:	Function
;
;       int GetSamplestoDecodeLastBlock()
;
;    Description:	Gets Samples to Process for the last block
;
;    Inputs:		
;			Format Tag
;           
;    Outputs: Samples to Process
;    Notes: Can be overlaid
;////////////////////////////////////////////////////////////////////////////// 
*/
int GetSamplestoDecodeLastBlock(int FormatTag, long int g_Wave_datachunksize)
{
    int SamplestoDecode = 0;
    if (FormatTag == WAVE_FORMAT_ADPCM)
    {
        if (g_Wave_wChannels == 1)
            SamplestoDecode = 2*(g_Wave_wBlockAlign + g_Wave_datachunksize) - 12;
        else // Two channels
			SamplestoDecode = g_Wave_wBlockAlign + g_Wave_datachunksize - 12;
    }
    else if (FormatTag == WAVE_FORMAT_IMA_ADPCM)
    {
        if (g_Wave_wChannels == 1)
            SamplestoDecode = 2*(g_Wave_wBlockAlign + g_Wave_datachunksize) - 7;
        else // Two channels
			SamplestoDecode = g_Wave_wBlockAlign + g_Wave_datachunksize - 7;
    }
#ifdef WINDOWS_PCM
    else if (FormatTag == WAVE_FORMAT_PCM)
    {
         SamplestoDecode = g_Wave_wBlockAlign + g_Wave_datachunksize;
        if (g_Wave_wBitsPerSample == 16)
            SamplestoDecode        = (SamplestoDecode>>g_Wave_wChannels);
        else if (g_Wave_wBitsPerSample == 24)
            SamplestoDecode        = ((SamplestoDecode/3)>>(g_Wave_wChannels-1));
        else
            SamplestoDecode        = (SamplestoDecode>>(g_Wave_wChannels-1)); // Got to be 8 bit PCM
    }
#endif
    return SamplestoDecode;
}

                                  

#ifdef ADPCMTEST
void Flush_OutPut_PCM_Buffer(int size)
{
	fwrite(SampleBuf,1,size,out);
}

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_ADPCM
;
;    Type:	Function
;
;       int Decode_ADPCM()
;
;    Description:	Decodes for stand-alone version of the decoder
;
;    Inputs:		
;			Format Tag
;           
;    Outputs: void
;    Notes: Used by the Test code.
;////////////////////////////////////////////////////////////////////////////// 
*/

void Decode_ADPCM(int FormatTag)
{
	int Last_block_flag = 0;
	
	// Default processing sizes.
	int SamplestoDecode = g_Wave_wnSamplesPerBlock;
	int	inputbufsize = g_Wave_wBlockAlign;
#ifdef IMA_ADPCM
    Init_IMA_ADPCM_States();
#endif
	// Start real time processing now. Set the file pointers correctly.
	Seek_data_offset(g_Wave_data_offset);
	while(g_Wave_datachunksize>0)
	{
		// Subtract block align and move forward into the buffer. so padding is avoided
		g_Wave_datachunksize -= g_Wave_wBlockAlign;	
		if (g_Wave_datachunksize <= 0)
		{
			Last_block_flag = 1;
            
			SamplestoDecode = GetSamplestoDecodeLastBlock(FormatTag, g_Wave_datachunksize);
			inputbufsize = g_Wave_wBlockAlign + g_Wave_datachunksize;
		}
		
		Wave_Fill_Input_Bitstream_Buffer(inputbufsize);
		
#ifdef MS_ADPCM
        if (FormatTag == WAVE_FORMAT_ADPCM)
    		Decode_MS_ADPCM_Block(SamplestoDecode);
        else 
#endif
#ifdef IMA_ADPCM
        if (FormatTag == WAVE_FORMAT_IMA_ADPCM)
        {
    		Decode_IMA_ADPCM_Block(SamplestoDecode);
        }
	    else
#endif
#ifdef G721_ADPCM
        if (FormatTag == WAVE_FORMAT_G721_ADPCM)
    		Decode_G721_ADPCM_Block(SamplestoDecode);
	    else
#endif
#ifdef WINDOWS_PCM
        if (FormatTag == WAVE_FORMAT_PCM)
        {
    		Decode_PCM_Block(SamplestoDecode);
        }
	    else
#endif
        return;
	    	
		Flush_OutPut_PCM_Buffer(SamplestoDecode*g_Wave_wChannels); // Just push this out to the DAC.
		
		// Check if no more data to process
		if (Last_block_flag == 1)
			return;
	}
}
#endif


