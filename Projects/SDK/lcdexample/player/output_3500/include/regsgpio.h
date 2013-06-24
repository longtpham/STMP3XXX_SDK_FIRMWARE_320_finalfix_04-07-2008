#if !(defined(__REGS_GPIO_INC))
#define __REGS_GPIO_INC 1

#include "types.h"

/////////////////////////////////////////////////////////////////////////////////
//  Interrupt Collector Registers
/////////////////////////////////////////////////////////////////////////////////

#define HW_GPIO_BASEADDR 0xF400

#define HW_GPB0_BASEADDR HW_GPIO_BASEADDR
#define HW_GPB1_BASEADDR HW_GPIO_BASEADDR+0x10
#define HW_GPB2_BASEADDR HW_GPIO_BASEADDR+0x20
#define HW_GPB3_BASEADDR HW_GPIO_BASEADDR+0x30

#define HW_GPB0_BLOCKNUM 0
#define HW_GPB1_BLOCKNUM 1
#define HW_GPB2_BLOCKNUM 2
#define HW_GPB3_BLOCKNUM 3

#define HW_GPB_GPENR 0
#define HW_GPB_GPDOR 1
#define HW_GPB_GPDIR 2
#define HW_GPB_GPDOER 3
#define HW_GPB_GPIPENR 4
#define HW_GPB_GPIENR 5
#define HW_GPB_GPILVLR 6
#define HW_GPB_GPIPOLR 7
#define HW_GPB_GPISTATR 8
#define HW_GPB_GPPWR 9
#define HW_GPB_GP8MA 10



/////////////////////////////////////////////////////////////////////////////////
//  GPIO Register Bit Positions
typedef union               /* GPIO Pin Register Bank 0 */
{
    struct Bitfield B;      // Bitfield is generic structure B0 - B23 in types.h
    unsigned int I;
    unsigned int U;
} gpr_type;

#define HW_GP_B0_BITPOS 0
#define HW_GP_B1_BITPOS 1
#define HW_GP_B2_BITPOS 2
#define HW_GP_B3_BITPOS 3
#define HW_GP_B4_BITPOS 4
#define HW_GP_B5_BITPOS 5
#define HW_GP_B6_BITPOS 6
#define HW_GP_B7_BITPOS 7
#define HW_GP_B8_BITPOS 8
#define HW_GP_B9_BITPOS 9
#define HW_GP_B10_BITPOS 10
#define HW_GP_B11_BITPOS 11
#define HW_GP_B12_BITPOS 12
#define HW_GP_B13_BITPOS 13
#define HW_GP_B14_BITPOS 14
#define HW_GP_B15_BITPOS 15
#define HW_GP_B16_BITPOS 16
#define HW_GP_B17_BITPOS 17
#define HW_GP_B18_BITPOS 18
#define HW_GP_B19_BITPOS 19
#define HW_GP_B20_BITPOS 20
#define HW_GP_B21_BITPOS 21
#define HW_GP_B22_BITPOS 22
#define HW_GP_B23_BITPOS 23

#define HW_GP_B0_SETMASK (1<<HW_GP_B0_BITPOS)
#define HW_GP_B1_SETMASK (1<<HW_GP_B1_BITPOS)
#define HW_GP_B2_SETMASK (1<<HW_GP_B2_BITPOS)
#define HW_GP_B3_SETMASK (1<<HW_GP_B3_BITPOS)
#define HW_GP_B4_SETMASK (1<<HW_GP_B4_BITPOS)
#define HW_GP_B5_SETMASK (1<<HW_GP_B5_BITPOS)
#define HW_GP_B6_SETMASK (1<<HW_GP_B6_BITPOS)
#define HW_GP_B7_SETMASK (1<<HW_GP_B7_BITPOS)
#define HW_GP_B8_SETMASK (1<<HW_GP_B8_BITPOS)
#define HW_GP_B9_SETMASK (1<<HW_GP_B9_BITPOS)
#define HW_GP_B10_SETMASK (1<<HW_GP_B10_BITPOS)
#define HW_GP_B11_SETMASK (1<<HW_GP_B11_BITPOS)
#define HW_GP_B12_SETMASK (1<<HW_GP_B12_BITPOS)
#define HW_GP_B13_SETMASK (1<<HW_GP_B13_BITPOS)
#define HW_GP_B14_SETMASK (1<<HW_GP_B14_BITPOS)
#define HW_GP_B15_SETMASK (1<<HW_GP_B15_BITPOS)
#define HW_GP_B16_SETMASK (1<<HW_GP_B16_BITPOS)
#define HW_GP_B17_SETMASK (1<<HW_GP_B17_BITPOS)
#define HW_GP_B18_SETMASK (1<<HW_GP_B18_BITPOS)
#define HW_GP_B19_SETMASK (1<<HW_GP_B19_BITPOS)
#define HW_GP_B20_SETMASK (1<<HW_GP_B20_BITPOS)
#define HW_GP_B21_SETMASK (1<<HW_GP_B21_BITPOS)
#define HW_GP_B22_SETMASK (1<<HW_GP_B22_BITPOS)
#define HW_GP_B23_SETMASK (1<<HW_GP_B23_BITPOS)

#define HW_GP_B0_CLRMASK (~(WORD)HW_GP_B0_SETMASK)
#define HW_GP_B1_CLRMASK (~(WORD)HW_GP_B1_SETMASK)
#define HW_GP_B2_CLRMASK (~(WORD)HW_GP_B2_SETMASK)
#define HW_GP_B3_CLRMASK (~(WORD)HW_GP_B3_SETMASK)
#define HW_GP_B4_CLRMASK (~(WORD)HW_GP_B4_SETMASK)
#define HW_GP_B5_CLRMASK (~(WORD)HW_GP_B5_SETMASK)
#define HW_GP_B6_CLRMASK (~(WORD)HW_GP_B6_SETMASK)
#define HW_GP_B7_CLRMASK (~(WORD)HW_GP_B7_SETMASK)
#define HW_GP_B8_CLRMASK (~(WORD)HW_GP_B8_SETMASK)
#define HW_GP_B9_CLRMASK (~(WORD)HW_GP_B9_SETMASK)
#define HW_GP_B10_CLRMASK (~(WORD)HW_GP_B10_SETMASK)
#define HW_GP_B11_CLRMASK (~(WORD)HW_GP_B11_SETMASK)
#define HW_GP_B12_CLRMASK (~(WORD)HW_GP_B12_SETMASK)
#define HW_GP_B13_CLRMASK (~(WORD)HW_GP_B13_SETMASK)
#define HW_GP_B14_CLRMASK (~(WORD)HW_GP_B14_SETMASK)
#define HW_GP_B15_CLRMASK (~(WORD)HW_GP_B15_SETMASK)
#define HW_GP_B16_CLRMASK (~(WORD)HW_GP_B16_SETMASK)
#define HW_GP_B17_CLRMASK (~(WORD)HW_GP_B17_SETMASK)
#define HW_GP_B18_CLRMASK (~(WORD)HW_GP_B18_SETMASK)
#define HW_GP_B19_CLRMASK (~(WORD)HW_GP_B19_SETMASK)
#define HW_GP_B20_CLRMASK (~(WORD)HW_GP_B20_SETMASK)
#define HW_GP_B21_CLRMASK (~(WORD)HW_GP_B21_SETMASK)
#define HW_GP_B22_CLRMASK (~(WORD)HW_GP_B22_SETMASK)
#define HW_GP_B23_CLRMASK (~(WORD)HW_GP_B23_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  GPIO 8mA Register Bit Positions
#define HW_GP8MA_B7_B0_BITPOS 0
#define HW_GP8MA_B15_B8_BITPOS 1
#define HW_GP8MA_B23_B16_BITPOS 2
#define HW_GP8MA_CLK_GATE_BITPOS 23


/////////////////////////////////////////////////////////////////////////////////
//  Logical GPIO numbers
#define HW_GPIO_000 0
#define HW_GPIO_001 1
#define HW_GPIO_002 2
#define HW_GPIO_003 3
#define HW_GPIO_004 4
#define HW_GPIO_005 5
#define HW_GPIO_006 6
#define HW_GPIO_007 7
#define HW_GPIO_008 8
#define HW_GPIO_009 9
#define HW_GPIO_010 10
#define HW_GPIO_011 11
#define HW_GPIO_012 12
#define HW_GPIO_013 13
#define HW_GPIO_014 14
#define HW_GPIO_015 15
#define HW_GPIO_016 16
#define HW_GPIO_017 17
#define HW_GPIO_018 18
#define HW_GPIO_019 19
#define HW_GPIO_020 20
#define HW_GPIO_021 21
#define HW_GPIO_022 22
#define HW_GPIO_023 23
#define HW_GPIO_024 24
#define HW_GPIO_025 25
#define HW_GPIO_026 26
#define HW_GPIO_027 27
#define HW_GPIO_028 28
#define HW_GPIO_029 29
#define HW_GPIO_030 30
#define HW_GPIO_031 31
#define HW_GPIO_032 32
#define HW_GPIO_033 33
#define HW_GPIO_034 34
#define HW_GPIO_035 35
#define HW_GPIO_036 36
#define HW_GPIO_037 37
#define HW_GPIO_038 38
#define HW_GPIO_039 39
#define HW_GPIO_040 40
#define HW_GPIO_041 41
#define HW_GPIO_042 42
#define HW_GPIO_043 43
#define HW_GPIO_044 44
#define HW_GPIO_045 45
#define HW_GPIO_046 46
#define HW_GPIO_047 47
#define HW_GPIO_048 48
#define HW_GPIO_049 49
#define HW_GPIO_050 50
#define HW_GPIO_051 51
#define HW_GPIO_052 52
#define HW_GPIO_053 53
#define HW_GPIO_054 54
#define HW_GPIO_055 55
#define HW_GPIO_056 56
#define HW_GPIO_057 57
#define HW_GPIO_058 58
#define HW_GPIO_059 59
#define HW_GPIO_060 60
#define HW_GPIO_061 61
#define HW_GPIO_062 62
#define HW_GPIO_063 63
#define HW_GPIO_064 64
#define HW_GPIO_065 65
#define HW_GPIO_066 66
#define HW_GPIO_067 67
#define HW_GPIO_068 68
#define HW_GPIO_069 69
#define HW_GPIO_070 70
#define HW_GPIO_071 71
#define HW_GPIO_072 72
#define HW_GPIO_073 73
#define HW_GPIO_074 74
#define HW_GPIO_075 75
#define HW_GPIO_076 76
#define HW_GPIO_077 77
#define HW_GPIO_078 78
#define HW_GPIO_079 79
#define HW_GPIO_080 80
#define HW_GPIO_081 81
#define HW_GPIO_082 82
#define HW_GPIO_083 83
#define HW_GPIO_084 84
#define HW_GPIO_085 85
#define HW_GPIO_086 86
#define HW_GPIO_087 87
#define HW_GPIO_088 88
#define HW_GPIO_089 89
#define HW_GPIO_090 90
#define HW_GPIO_091 91
#define HW_GPIO_092 92
#define HW_GPIO_093 93
#define HW_GPIO_094 94
#define HW_GPIO_095 95
#define HW_GPIO_LAST HW_GPIO_095

#define HW_GP0ENR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPENR))  /* GPIO 0 Enable Register   */
#define HW_GP0DOR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOR)) /* GPIO 0 Data Out Register */
#define HW_GP0DIR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDIR)) /* GPIO 0 Dait In Register  */
#define HW_GP0DOER     (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOER)) /* GPIO 0 Dait Out Enable Register  */
#define HW_GP0IPENR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 0 Interrupt Pin Enable Register */
#define HW_GP0IENR     (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIENR)) /* GPIO 0 Interrupt Enable Register */
#define HW_GP0ILVLR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 0 Interrupt Level Register  */
#define HW_GP0IPOLR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 0 Interrupt Polarity Register   */
#define HW_GP0ISTATR   (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 0 Interrupt Status Register */
#define HW_GP0PWR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
#define HW_GP08MA      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GP8MA)) 
#define HW_GP1ENR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPENR))     /* GPIO 1 Enable Register   */
#define HW_GP1DOR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDOR)) /* GPIO 1 Data Out Register */
#define HW_GP1DIR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDIR)) /* GPIO 1 Dait In Register  */
#define HW_GP1DOER     (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDOER)) /* GPIO 1 Dait Out Enable Register  */
#define HW_GP1IPENR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 1 Interrupt Pin Enable Register */
#define HW_GP1IENR     (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIENR)) /* GPIO 1 Interrupt Enable Register */
#define HW_GP1ILVLR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 1 Interrupt Level Register  */
#define HW_GP1IPOLR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 1 Interrupt Polarity Register   */
#define HW_GP1ISTATR   (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 1 Interrupt Status Register */
#define HW_GP1PWR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
#define HW_GP18MA      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GP8MA)) 
#define HW_GP2ENR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPENR))     /* GPIO 2 Enable Register   */
#define HW_GP2DOR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDOR)) /* GPIO 2 Data Out Register */
#define HW_GP2DIR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDIR)) /* GPIO 2 Dait In Register  */
#define HW_GP2DOER     (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDOER)) /* GPIO 2 Dait Out Enable Register  */
#define HW_GP2IPENR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 2 Interrupt Pin Enable Register */
#define HW_GP2IENR     (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIENR)) /* GPIO 2 Interrupt Enable Register */
#define HW_GP2ILVLR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 2 Interrupt Level Register  */
#define HW_GP2IPOLR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 2 Interrupt Polarity Register   */
#define HW_GP2ISTATR   (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 2 Interrupt Status Register */
#define HW_GP2PWR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
#define HW_GP28MA      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GP8MA)) 
#define HW_GP3ENR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPENR))     /* GPIO 2 Enable Register   */
#define HW_GP3DOR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDOR)) /* GPIO 2 Data Out Register */
#define HW_GP3DIR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDIR)) /* GPIO 2 Dait In Register  */
#define HW_GP3DOER     (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDOER)) /* GPIO 2 Dait Out Enable Register  */
#define HW_GP3IPENR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 2 Interrupt Pin Enable Register */
#define HW_GP3IENR     (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIENR)) /* GPIO 2 Interrupt Enable Register */
#define HW_GP3ILVLR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 2 Interrupt Level Register  */
#define HW_GP3IPOLR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 2 Interrupt Polarity Register   */
#define HW_GP3ISTATR   (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 2 Interrupt Status Register */
#define HW_GP3PWR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
#define HW_GP38MA      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GP8MA)) 

#endif

