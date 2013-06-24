#ifndef EXEC_H
#define EXEC_H


void _reentrant SysPostMessage(int iLength,...);
int _asmfunc GetMessage(struct MessageQueueDescriptor*,struct CMessage * );
long _asmfunc SysGetCurrentTime(void);


#endif