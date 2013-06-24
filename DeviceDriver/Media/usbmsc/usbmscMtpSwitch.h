#ifndef USBMSC_MTP_SWITCH_H

#define USBMSC_MTP_SWITCH_H
#ifdef MTP_BUILD
//increased the size of this buffer to 1.5K words to enable faster transfers
extern WORD _X Device1CommandOutBuffer[TRANSFER_BUFFER_SIZE*3];
#else
extern WORD _X Device1CommandOutBuffer[TRANSFER_BUFFER_SIZE];
#endif
extern WORD _X Device1CommandInBuffer[TRANSFER_BUFFER_SIZE];

extern WORD MTPDetected;

#endif 
