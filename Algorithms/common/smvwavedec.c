////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : smvwavedec.c
// Description : smv wave decoder
////////////////////////////////////////////////////////////////////////////////
#include "defs.h"
#include "wavedefs.h"
#include "smvbitstreamwave.h"
#include "smvwavedata.h"

//----------------------------------------------------------------------
// Function Declarations for C
int SmvProcessChunk(int ckIDval, long int *cksize);
int SmvGetckID(void);
void SmvProcess_Common_Fmt_Header(void);
extern _asmfunc long SmvGetDWord(void);
extern _asmfunc int SmvGetWord(void);
extern _asmfunc int SmvGetSWord(void);
extern _asmfunc long SmvGet_file_offset(void);
int smvwaveparser(void);

int SMV_Decode_IMA_ADPCM_header(void);

//---------------------------------------------------------------------
// Function Declarations for assembly code.
extern _asmfunc void Decode_SMV_ADPCM_Block();


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  smvwaveparser
;
;    Type:	Function
;
;       int smvwaveparser()
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
int smvwaveparser()
{
	int ret_val;
	int ckIDval=0;
	long int ckSize;
	int chunklevel = 0;
    smv_riff_file_flag=0;
	while((ckIDval = SmvGetckID())>=0)		// Returns -1 on error
	{
		ckSize = SmvGetDWord();
		// Subtract this chunk size from the current level
		SmvDataSize[chunklevel] -= (ckSize + 8);
		// Skip unknown chunks
		if (ckIDval == UNKNOWN_CKID)
		{
			SmvSkipData(ckSize);	// Align the file reads correctly.
		}
		else
		{
			ret_val = SmvProcessChunk(ckIDval, &ckSize);
			if(ret_val == 1)
			{
				// Contains nested chunks. Go one level up.
				chunklevel++;
				SmvDataSize[chunklevel] = ckSize;
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
		if(SmvDataSize[chunklevel] <= 0)
		{
			// Go down one level
			chunklevel--;
			if (chunklevel<=0)
            {
                SmvSeek_data_offset(g_SmvWave_data_offset);
				return 0;			// Done!
            }
		}
	}
    // The file is completely screwed up. Could'nt find the riff chunk
    if (smv_riff_file_flag == 0)
        return -1;
    
    
        
    return 0;    
}


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvProcessChunk
;
;    Type:	Function
;
;       int SmvProcessChunk(int ckIDval, long int *cksize)
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

int SmvProcessChunk(int ckIDval, long int *cksize)
{
int i;
int ret_val = 0;
	switch(ckIDval)
	{
	case RIFF_CKID:
        // This is an extended Ckid
		// check if 'W','A','V','E' is present.
		// We suppport only wave files.
		ckIDval = SmvGetckID();
		if (ckIDval == WAVE_CKID)
		{
			*cksize -= 4;
			ret_val = 1;
            smv_riff_file_flag = 1;
		}
		else
			ret_val = -1;
		break;

	case FMT_CKID:
		SmvProcess_Common_Fmt_Header();
        *cksize -= 14;
        if (g_SmvWave_wFormatTag == WAVE_FORMAT_IMA_ADPCM)
        {
			ret_val = SMV_Decode_IMA_ADPCM_header();
            *cksize -= 6;
        }
		else
			ret_val = -1;	// Unknown algo.
        
        if (*cksize > 0)
        {   
            for(i = 0; i < *cksize; i++)
            {
                SmvGetByte();
            }
        }
		break;

	case FACT_CKID:
		g_SmvWave_dwWavDuration = SmvGetDWord();
		ret_val = 0;
		break;

	case DATA_CKID:
		g_SmvWave_data_offset = SmvGet_file_offset();	// Store the offset to be passed to the algo.
		g_SmvWave_datachunksize = *cksize;
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
;>   Name:  SmvGetckID
;
;    Type:	Function
;
;       int SmvGetckID()
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
int SmvGetckID()
{
	int i;
	long int word = SmvGetDWord();
	// Default to unknown
	int ckid = UNKNOWN_CKID;
	for (i = 0; i < TOTAL_CKID; i++)
	{
		if (word == smv_ckidlist[i])
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
;>   Name:  SmvProcess_Common_Fmt_Header
;
;    Type:	Function
;
;       void SmvProcess_Common_Fmt_Header()
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

void SmvProcess_Common_Fmt_Header()
{
	g_SmvWave_wFormatTag			= SmvGetWord();
	g_SmvWave_wChannels			= SmvGetWord();
	g_SmvWave_dwSamplesPerSec	= SmvGetDWord();
	g_SmvWave_dwAvgBytesPerSec	= SmvGetDWord();
	g_SmvWave_wBlockAlign		= SmvGetWord();
}

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SMV_Decode_IMA_ADPCM_header
;
;    Type:	Function
;
;       void SMV_Decode_IMA_ADPCM_header()
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
int SMV_Decode_IMA_ADPCM_header()
{
	g_SmvWave_wBitsPerSample			= SmvGetWord();
	g_SmvWave_wcbSize					= SmvGetWord();
	g_SmvWave_wnSamplesPerBlock		= SmvGetWord();
    if(g_SmvWave_wBitsPerSample == 4)
       return 0;
    else
       return -1;
}

/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetSamplestoDecode
;
;    Type:	Function
;
;       int SmvGetSamplestoDecode()
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
int SmvGetSamplestoDecode(int FormatTag)
{
    int ret_val = 0;
    if (FormatTag == WAVE_FORMAT_ADPCM)
    {
        ret_val =  g_SmvWave_wnSamplesPerBlock - 2;
    }
    return ret_val;
}


/* 
///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetSamplestoDecodeLastBlock
;
;    Type:	Function
;
;       int SmvGetSamplestoDecodeLastBlock()
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
int SmvGetSamplestoDecodeLastBlock(int FormatTag, long int g_SmvWave_datachunksize)
{
    int SmvSamplestoDecode = 0;
    if (FormatTag == WAVE_FORMAT_ADPCM)
    {
        if (g_SmvWave_wChannels == 1)
            SmvSamplestoDecode = 2*(g_SmvWave_wBlockAlign + g_SmvWave_datachunksize) - 12;
        else // Two channels
			SmvSamplestoDecode = g_SmvWave_wBlockAlign + g_SmvWave_datachunksize - 12;
    }
    else if (FormatTag == WAVE_FORMAT_IMA_ADPCM)
    {
        if (g_SmvWave_wChannels == 1)
            SmvSamplestoDecode = 2*(g_SmvWave_wBlockAlign + g_SmvWave_datachunksize) - 7;
        else // Two channels
			SmvSamplestoDecode = g_SmvWave_wBlockAlign + g_SmvWave_datachunksize - 7;
    }
    return SmvSamplestoDecode;
}
