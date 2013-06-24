#ifndef STRINGLIB_H
#define STRINGLIB_H
_asmfunc int pack_string(UCS3 _Y * _Y pUnpacked, _packed BYTE _Y * _Y pPacked);
_asmfunc int unpack_string( _packed BYTE _Y * _Y pPacked,  UCS3 _Y * _Y pUnpacked);
_asmfunc int unpack_data( _packed BYTE _Y * _Y pPacked,  BYTE _Y * _Y pUnpacked, INT iCount);

_asmfunc BYTE GetByteFromArrayInX( void _X * p,int n);
_asmfunc void PutByteInArrayInX( void _X * p,int n, int newchar);
_asmfunc BYTE GetByteFromArrayInY( void _Y * p,int n);
_asmfunc void PutByteInArrayInY( void _Y * p,int n, int newchar);

_asmfunc BYTE packed_get( _packed BYTE _Y * p,int n);
_asmfunc void packed_set( _packed BYTE _Y * p,int n, int newchar);

_asmfunc int packed_strlen( _packed BYTE _Y * _Y p);

_asmfunc _packed BYTE _Y * packed_strncpy( _packed BYTE _Y * _Y pDest,_packed BYTE _Y * _Y pSrc, int iCount);
_asmfunc _packed BYTE _Y * packed_strcpy ( _packed BYTE _Y * _Y pDest,_packed BYTE _Y * _Y pSrc);


_asmfunc int packed_strncmp( _packed BYTE _Y * _Y pLeft,_packed BYTE _Y * _Y pRight, int iCount);
_asmfunc int packed_strcmp ( _packed BYTE _Y * _Y pLeft,_packed BYTE _Y * _Y pRight);

_asmfunc int packed_strncat( _packed BYTE _Y * _Y pDest,_packed BYTE _Y * _Y pSource, int iCount);
_asmfunc int packed_strcat ( _packed BYTE _Y * _Y pDest,_packed BYTE _Y * _Y pSource);


_asmfunc int strlen(UCS3 _Y * _Y p);

_asmfunc UCS3 *strchr(UCS3 _Y * _Y pUnpacked, UCS3 CharToFind);
_asmfunc UCS3 *strrchr(UCS3 _Y * _Y pUnpacked, UCS3 CharToFind);

_asmfunc void strncpy(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource, BYTE iCount);
_asmfunc void strcpy(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource);

_asmfunc char *strncat(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource, int iCount);
_asmfunc char *strcat(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource);

_asmfunc int strncmp(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource, int iCount);
_asmfunc int strcmp(UCS3 _Y * _Y pDest, UCS3 _Y * _Y pSource);

#endif
