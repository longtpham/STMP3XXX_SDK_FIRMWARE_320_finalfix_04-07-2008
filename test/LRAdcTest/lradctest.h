#ifndef _LRADCTEST_H
#define _LRADCTEST_H

#include "types.h"

#define PLAY    4
#define ADC_VALUE_X_COORD	70

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

void _reentrant PowerUpHeadPhones (void);
WORD _reentrant GetKey(void);

#endif
