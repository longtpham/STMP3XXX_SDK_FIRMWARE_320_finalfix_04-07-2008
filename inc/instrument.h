//  Event   Pin       GP      Function       Bank  Bit
//    00     22       42      Button         GPIO1 18
//    01     40       46      Button         GPIO1 22
//    02     41       47      Button         GPIO1 23
//    03     42       48      Button         GPIO2  0
//    04      1       14      SPI MOSI       GPIO0 14
//    05      2       13      SPI MISO       GPIO0 13
//    06      3       12      SPI SCK        GPIO0 12
//    07    100       15      SPI SS         GPIO0 15

#ifndef _INSTRUMENT_H
#define _INSTRUMENT_H

#ifdef INSTRUMENT_ENABLE

#include "regsgpio.h"

#define EventSetup00();  HW_GP1DOER.B.B18=1; HW_GP1DOR.B.B18=0; HW_GP1ENR.B.B18=1;
#define EventSetup01();  HW_GP1DOER.B.B22=1; HW_GP1DOR.B.B22=0; HW_GP1ENR.B.B22=1;
#define EventSetup02();  HW_GP1DOER.B.B23=1; HW_GP1DOR.B.B23=0; HW_GP1ENR.B.B23=1;
#define EventSetup03();  HW_GP2DOER.B.B0=1;  HW_GP2DOR.B.B0=0;  HW_GP2ENR.B.B0=1;
#define EventSetup04();  HW_GP0DOER.B.B14=1; HW_GP0DOR.B.B14=0; HW_GP0ENR.B.B14=1;
#define EventSetup05();  HW_GP0DOER.B.B13=1; HW_GP0DOR.B.B13=0; HW_GP0ENR.B.B13=1;
#define EventSetup06();  HW_GP0DOER.B.B12=1; HW_GP0DOR.B.B12=0; HW_GP0ENR.B.B12=1;
#define EventSetup07();  HW_GP0DOER.B.B15=1; HW_GP0DOR.B.B15=0; HW_GP0ENR.B.B15=1;

#define EventSet00();   HW_GP1DOR.B.B18=1;
#define EventSet01();   HW_GP1DOR.B.B22=1;
#define EventSet02();   HW_GP1DOR.B.B23=1;
#define EventSet03();   HW_GP2DOR.B.B0=1;
#define EventSet04();   HW_GP0DOR.B.B14=1;
#define EventSet05();   HW_GP0DOR.B.B13=1;
//#define EventSet04();
//#define EventSet05();
#define EventSet06();   HW_GP0DOR.B.B12=1;
#define EventSet07();   HW_GP0DOR.B.B15=1;
                        
#define EventClear00(); HW_GP1DOR.B.B18=0;
#define EventClear01(); HW_GP1DOR.B.B22=0;
#define EventClear02(); HW_GP1DOR.B.B23=0;
#define EventClear03(); HW_GP2DOR.B.B0=0;
#define EventClear04(); HW_GP0DOR.B.B14=0;
#define EventClear05(); HW_GP0DOR.B.B13=0;
//#define EventClear04();
//#define EventClear05();
#define EventClear06(); HW_GP0DOR.B.B12=0;
#define EventClear07(); HW_GP0DOR.B.B15=0;
                        
#else // #ifdef INSTRUMENT_ENABLE

#define EventSetup00();
#define EventSetup01();
#define EventSetup02();
#define EventSetup03();
#define EventSetup04();
#define EventSetup05();
#define EventSetup06();
#define EventSetup07();

#define EventSet00();
#define EventSet01();
#define EventSet02();
#define EventSet03();
#define EventSet04();
#define EventSet05();
#define EventSet06();
#define EventSet07();
                        
#define EventClear00();
#define EventClear01();
#define EventClear02();
#define EventClear03();
#define EventClear04();
#define EventClear05();
#define EventClear06();
#define EventClear07();

#endif // #ifdef INSTRUMENT_ENABLE

#endif // #ifndef _INSTRUMENT_H
