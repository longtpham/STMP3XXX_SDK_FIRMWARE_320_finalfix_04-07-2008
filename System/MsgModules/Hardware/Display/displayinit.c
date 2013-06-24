#include "kernel.h"
#include "displaydriver.h"
#include "displaydrv.h"
#include "sal\display_sal.h"
#include "resource.h"

void _reentrant DisplayInit(void)
{
    SALDisplayInit();
    HALDisplayInit();
    DisplaySetFont(0);
}
