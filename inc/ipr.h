#ifndef __IPR_H
#define __IPR_H



#define IVL_DISABLED    0
#define IVL_PRIORITY_0  1
#define IVL_PRIORITY_1  2
#define IVL_PRIORITY_2  3

#define IVL_LINE_0     10
#define IVL_LINE_1     12
#define IVL_LINE_2     14
#define IVL_LINE_3     16
#define IVL_LINE_4     18
#define IVL_LINE_5     20
#define IVL_LINE_6     22


_asmfunc WORD IPRSetIVLPriority(WORD wIVL, WORD wPriority);

#endif 