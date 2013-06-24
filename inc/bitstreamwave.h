
#ifndef ADPCMTEST

extern _asmfunc char GetByte(void);
extern _asmfunc int GetWord();
extern _asmfunc int GetSWord();
extern _asmfunc long GetDWord();
extern _asmfunc void WriteByte(char byte);
extern _asmfunc void WriteWord(int word);
extern _asmfunc void WriteDWord(long int word);
extern _asmfunc void Wave_Fill_Input_Bitstream_Buffer(int size);
extern _asmfunc void Wave_Flush_Encoded_Bitstream_Buffer();
extern _asmfunc long Get_file_offset();
extern _asmfunc void Seek_data_offset(long offset);
extern _asmfunc void Seek_to_zero();
extern _asmfunc void SkipData(long amount);

#else

char GetByte();
int GetWord();
int GetSWord();
long GetDWord();
void WriteByte(char byte);
void WriteWord(int word);
void WriteDWord(long int word);
void Wave_Fill_Input_Bitstream_Buffer(int size);
void Wave_Flush_Encoded_Bitstream_Buffer();
long int Get_file_offset();
void Seek_data_offset(long int offset);
void Seek_to_zero();
void SkipData(long int amount);

#endif