// Plafform API

extern void EnterNonReentrantSection(void);
extern void LeaveNonReentrantSection(void);

extern INT FSGetByte(void *buffer, INT Offset,INT MemoryType);
extern INT FSGetWord(void *buffer, INT Offset,INT MemoryType);
extern LONG FSGetDWord(void *buffer, INT Offset,INT MemoryType);
extern void FsCopyBuffer(void *Dest_Buffer,INT Dest_Offset,void *Source_Buffer,INT Source_Offset,INT Size,INT Source_MemoryType,INT Dest_MemoryType,INT Dest_modulo,INT SourceModule);
extern void PutByte(void *buffer, INT byte,INT Offset ,INT MemoryType);
extern void PutWord(void *buffer, INT word, INT Offset,INT MemoryType);
extern void PutDword(void *buffer,  LONG dword,INT Offset,INT MemoryType);
extern LONG GetDWordY(void *buffer, INT Offset,INT MemoryType);
