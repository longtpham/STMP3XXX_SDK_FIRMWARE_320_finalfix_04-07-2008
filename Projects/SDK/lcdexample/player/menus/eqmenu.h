#ifndef _EQ_H
#define _EQ_H

#include "types.h"

// Order of EQ selection
// must match order of struct MenuItem defined in Eqmenu.c
#define FIRST_EQ    0
#define EQ_NORMAL   0
#define EQ_ROCK     1
#define EQ_JAZZ     2   
#define EQ_CLASSIC  3   
#define EQ_POP      4
#define EQ_CUSTOM   5
#define LAST_EQ     5
#define EQMENU_COUNT   LAST_EQ+1

// Band Pass Filter Center Frequencies  (SDK2.520)
#define CENTER_FREQ1  80    // 80 Hz  (see 'CenterFreq' defined in Geqmem.asm)
#define CENTER_FREQ2  250
#define CENTER_FREQ3  1000
#define CENTER_FREQ4  4000
#define CENTER_FREQ5  12000

#define EQ_BAND_FIRST 0
#define EQ_BAND1      0
#define EQ_BAND2      1
#define EQ_BAND3      2
#define EQ_BAND4      3
#define EQ_BAND5      4
#define EQ_BAND_LAST  4
#define NUM_EQ_BANDS EQ_BAND_LAST + 1

#define EQ_ROCK_GAIN1    18
#define EQ_ROCK_GAIN2   -4
#define EQ_ROCK_GAIN3   -2
#define EQ_ROCK_GAIN4    14
#define EQ_ROCK_GAIN5    14

#define EQ_JAZZ_GAIN1    12
#define EQ_JAZZ_GAIN2   -2
#define EQ_JAZZ_GAIN3   -4
#define EQ_JAZZ_GAIN4    0
#define EQ_JAZZ_GAIN5    8

#define EQ_CLASS_GAIN1   0
#define EQ_CLASS_GAIN2   0
#define EQ_CLASS_GAIN3   0
#define EQ_CLASS_GAIN4  -7
#define EQ_CLASS_GAIN5  -10

#define EQ_POP_GAIN1    -4
#define EQ_POP_GAIN2     8
#define EQ_POP_GAIN3     8
#define EQ_POP_GAIN4    -2
#define EQ_POP_GAIN5     0

#define EQ_NORM_GAIN1    0
#define EQ_NORM_GAIN2    0
#define EQ_NORM_GAIN3    0
#define EQ_NORM_GAIN4    0
#define EQ_NORM_GAIN5    0

// dB gain level:  number of 0.5dB units
#define EQ_MAX_GAIN      28    // +14dB  (see 'dBTable' defined in Geqmem.asm)
#define EQ_ZERO_GAIN     0     //  0 dB  (see 'ZerodBPoint')
#define EQ_MIN_GAIN     -28    // -14dB  (divide gain by 2 to get dB)

extern INT g_iEqSetting;

#endif
