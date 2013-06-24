#ifndef __NIMH_H
#define __NIMH_H

#define NIMH_BATTERY_VOLTAGE_1_2 1200
#define NIMH_BATTERY_VOLTAGE_1_5 1500
#define NIMH_BATTERY_VOLTAGE_1_8 1800

// Some states may not be used for a given state machine.
#define NIMH_UNKNOWN               0
#define NIMH_CONDITIONING          1
#define NIMH_CHARGING              2
#define NIMH_WAITING_FOR_FINISH    3
#define NIMH_CHARGED               4


#endif