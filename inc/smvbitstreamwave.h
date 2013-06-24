////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : smvbitstreamwave.h
// Description : header file
////////////////////////////////////////////////////////////////////////////////
#ifndef ADPCMTEST

extern _asmfunc char SmvGetByte(void);
extern _asmfunc int SmvGetWord();
extern _asmfunc int SmvGetSWord();
extern _asmfunc long SmvGetDWord();
extern _asmfunc void SmvWriteByte(char byte);
extern _asmfunc void SmvWriteWord(int word);
extern _asmfunc void SmvWriteDWord(long int word);
extern _asmfunc void Smv_Wave_Fill_Input_Bitstream_Buffer(int size);
extern _asmfunc void Smv_Wave_Flush_Encoded_Bitstream_Buffer();
extern _asmfunc long SmvGet_file_offset();
extern _asmfunc void SmvSeek_data_offset(long offset);
extern _asmfunc void SmvSeek_to_zero();
extern _asmfunc void SmvSkipData(long amount);

#else

char SmvGetByte();
int SmvGetWord();
int SmvGetSWord();
long SmvGetDWord();
void SmvWriteByte(char byte);
void SmvWriteWord(int word);
void SmvWriteDWord(long int word);
void Smv_Wave_Fill_Input_Bitstream_Buffer(int size);
void Smv_Wave_Flush_Encoded_Bitstream_Buffer();
long int SmvGet_file_offset();
void SmvSeek_data_offset(long int offset);
void SmvSeek_to_zero();
void SmvSkipData(long int amount);

#endif