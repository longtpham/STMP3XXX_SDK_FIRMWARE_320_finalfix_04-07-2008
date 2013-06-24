#ifndef DBCS_H
#define DBCS_H


_reentrant void DBCSToUnicode(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength);
_reentrant void DBCSToUnicodeDstXSrcY(_packed unsigned char *pDBCS, WORD _X * _X pUnicode, INT iLength);

#endif