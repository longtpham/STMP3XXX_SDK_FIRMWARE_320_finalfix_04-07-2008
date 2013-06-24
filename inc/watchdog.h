#ifndef WATCHDOG_H
#define WATCHDOG_H

#define SOFT_WATCHDOG_TICKLED_VALUE  0xDEAD

#define LONG_WATCHDOG_TIMOUT_VALUE      10000  

#define NORMAL_WATCHDOG_TIMOUT_VALUE    5000
    extern WORD g_wSoftWatchdog;
    extern WORD g_wWatchdogTimeoutValue;

    int SetWatchdogTimeout(int);
    void TickleWatchdog(void);

#endif
