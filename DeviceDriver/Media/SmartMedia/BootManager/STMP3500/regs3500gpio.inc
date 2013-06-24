;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regs3500gpio.inc
;;  Description : GPIO IP Register definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     GPIO
; (Note that when there is more than one copy of a particular module, the
; module name includes a number starting from 0 for the first instance of
; that module)
;
; <regname> is the specific register within that module
;
; We also define the following...
;
;     HW_<module>_<regname>_BITPOS
; which defines the starting bit (i.e. LSB) of a multi bit field
;
;     HW_<module>_<regname>_SETMASK
; which does something else, and
;
;     HW_<module>_<regname>_CLRMASK
; which does something else.
;
; Other rules
;     All caps
;     Numeric identifiers start at 0

    if !(@def(regs3500gpioinc))
regs3500gpioinc equ 1

;C Insert
;#include "types.h"
;endc

;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Registers
;///////////////////////////////////////////////////////////////////////////////

HW_GPIO_BASEADDR                        equ     $F400
HW_GPB0_BASEADDR                        equ     HW_GPIO_BASEADDR
HW_GPB1_BASEADDR                        equ     HW_GPIO_BASEADDR+$10
HW_GPB2_BASEADDR                        equ     HW_GPIO_BASEADDR+$20
HW_GPB3_BASEADDR                        equ     HW_GPIO_BASEADDR+$30

HW_GPB0_BLOCKNUM                        equ     0
HW_GPB1_BLOCKNUM                        equ     1
HW_GPB2_BLOCKNUM                        equ     2
HW_GPB3_BLOCKNUM                        equ     3

HW_GPB_GPENR                            equ     0
HW_GPB_GPDOR                            equ     1
HW_GPB_GPDIR                            equ     2
HW_GPB_GPDOER                           equ     3
HW_GPB_GPIPENR                          equ     4
HW_GPB_GPIENR                           equ     5
HW_GPB_GPILVLR                          equ     6
HW_GPB_GPIPOLR                          equ     7
HW_GPB_GPISTATR                         equ     8
HW_GPB_GPPWR                            equ     9
HW_GPB_GP8MA                            equ     10

;ASM Only
HW_GP0ENR                               equ     HW_GPB0_BASEADDR+HW_GPB_GPENR
HW_GP0DOR                               equ     HW_GPB0_BASEADDR+HW_GPB_GPDOR
HW_GP0DIR                               equ     HW_GPB0_BASEADDR+HW_GPB_GPDIR
HW_GP0DOER                              equ     HW_GPB0_BASEADDR+HW_GPB_GPDOER
HW_GP0IPENR                             equ     HW_GPB0_BASEADDR+HW_GPB_GPIPENR
HW_GP0IENR                              equ     HW_GPB0_BASEADDR+HW_GPB_GPIENR
HW_GP0ILVLR                             equ     HW_GPB0_BASEADDR+HW_GPB_GPILVLR
HW_GP0IPOLR                             equ     HW_GPB0_BASEADDR+HW_GPB_GPIPOLR
HW_GP0ISTATR                            equ     HW_GPB0_BASEADDR+HW_GPB_GPISTATR
HW_GP0PWR                               equ     HW_GPB0_BASEADDR+HW_GPB_GPPWR
HW_GP08MA                               equ     HW_GPB0_BASEADDR+HW_GPB_GP8MA
HW_GP1ENR                               equ     HW_GPB1_BASEADDR+HW_GPB_GPENR
HW_GP1DOR                               equ     HW_GPB1_BASEADDR+HW_GPB_GPDOR
HW_GP1DIR                               equ     HW_GPB1_BASEADDR+HW_GPB_GPDIR
HW_GP1DOER                              equ     HW_GPB1_BASEADDR+HW_GPB_GPDOER
HW_GP1IPENR                             equ     HW_GPB1_BASEADDR+HW_GPB_GPIPENR
HW_GP1IENR                              equ     HW_GPB1_BASEADDR+HW_GPB_GPIENR
HW_GP1ILVLR                             equ     HW_GPB1_BASEADDR+HW_GPB_GPILVLR
HW_GP1IPOLR                             equ     HW_GPB1_BASEADDR+HW_GPB_GPIPOLR
HW_GP1ISTATR                            equ     HW_GPB1_BASEADDR+HW_GPB_GPISTATR
HW_GP1PWR                               equ     HW_GPB1_BASEADDR+HW_GPB_GPPWR
HW_GP18MA                               equ     HW_GPB1_BASEADDR+HW_GPB_GP8MA
HW_GP2ENR                               equ     HW_GPB2_BASEADDR+HW_GPB_GPENR
HW_GP2DOR                               equ     HW_GPB2_BASEADDR+HW_GPB_GPDOR
HW_GP2DIR                               equ     HW_GPB2_BASEADDR+HW_GPB_GPDIR
HW_GP2DOER                              equ     HW_GPB2_BASEADDR+HW_GPB_GPDOER
HW_GP2IPENR                             equ     HW_GPB2_BASEADDR+HW_GPB_GPIPENR
HW_GP2IENR                              equ     HW_GPB2_BASEADDR+HW_GPB_GPIENR
HW_GP2ILVLR                             equ     HW_GPB2_BASEADDR+HW_GPB_GPILVLR
HW_GP2IPOLR                             equ     HW_GPB2_BASEADDR+HW_GPB_GPIPOLR
HW_GP2ISTATR                            equ     HW_GPB2_BASEADDR+HW_GPB_GPISTATR
HW_GP2PWR                               equ     HW_GPB2_BASEADDR+HW_GPB_GPPWR
HW_GP28MA                               equ     HW_GPB2_BASEADDR+HW_GPB_GP8MA
HW_GP3ENR                               equ     HW_GPB3_BASEADDR+HW_GPB_GPENR
HW_GP3DOR                               equ     HW_GPB3_BASEADDR+HW_GPB_GPDOR
HW_GP3DIR                               equ     HW_GPB3_BASEADDR+HW_GPB_GPDIR
HW_GP3DOER                              equ     HW_GPB3_BASEADDR+HW_GPB_GPDOER
HW_GP3IPENR                             equ     HW_GPB3_BASEADDR+HW_GPB_GPIPENR
HW_GP3IENR                              equ     HW_GPB3_BASEADDR+HW_GPB_GPIENR
HW_GP3ILVLR                             equ     HW_GPB3_BASEADDR+HW_GPB_GPILVLR
HW_GP3IPOLR                             equ     HW_GPB3_BASEADDR+HW_GPB_GPIPOLR
HW_GP3ISTATR                            equ     HW_GPB3_BASEADDR+HW_GPB_GPISTATR
HW_GP3PWR                               equ     HW_GPB3_BASEADDR+HW_GPB_GPPWR
HW_GP38MA                               equ     HW_GPB3_BASEADDR+HW_GPB_GP8MA
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  GPIO Register Bit Positions
;C Insert
;typedef union               /* GPIO Pin Register Bank 0 */
;{
;    struct Bitfield B;      // Bitfield is generic structure B0 - B23 in types.h
;    int I;
;} gpr_type;
;endc

HW_GP_B0_BITPOS                         equ     0
HW_GP_B1_BITPOS                         equ     1
HW_GP_B2_BITPOS                         equ     2
HW_GP_B3_BITPOS                         equ     3
HW_GP_B4_BITPOS                         equ     4
HW_GP_B5_BITPOS                         equ     5
HW_GP_B6_BITPOS                         equ     6
HW_GP_B7_BITPOS                         equ     7
HW_GP_B8_BITPOS                         equ     8
HW_GP_B9_BITPOS                         equ     9
HW_GP_B10_BITPOS                        equ     10
HW_GP_B11_BITPOS                        equ     11
HW_GP_B12_BITPOS                        equ     12
HW_GP_B13_BITPOS                        equ     13
HW_GP_B14_BITPOS                        equ     14
HW_GP_B15_BITPOS                        equ     15
HW_GP_B16_BITPOS                        equ     16
HW_GP_B17_BITPOS                        equ     17
HW_GP_B18_BITPOS                        equ     18
HW_GP_B19_BITPOS                        equ     19
HW_GP_B20_BITPOS                        equ     20
HW_GP_B21_BITPOS                        equ     21
HW_GP_B22_BITPOS                        equ     22
HW_GP_B23_BITPOS                        equ     23

HW_GP_B0_SETMASK                        equ     1<<HW_GP_B0_BITPOS
HW_GP_B1_SETMASK                        equ     1<<HW_GP_B1_BITPOS
HW_GP_B2_SETMASK                        equ     1<<HW_GP_B2_BITPOS
HW_GP_B3_SETMASK                        equ     1<<HW_GP_B3_BITPOS
HW_GP_B4_SETMASK                        equ     1<<HW_GP_B4_BITPOS
HW_GP_B5_SETMASK                        equ     1<<HW_GP_B5_BITPOS
HW_GP_B6_SETMASK                        equ     1<<HW_GP_B6_BITPOS
HW_GP_B7_SETMASK                        equ     1<<HW_GP_B7_BITPOS
HW_GP_B8_SETMASK                        equ     1<<HW_GP_B8_BITPOS
HW_GP_B9_SETMASK                        equ     1<<HW_GP_B9_BITPOS
HW_GP_B10_SETMASK                       equ     1<<HW_GP_B10_BITPOS
HW_GP_B11_SETMASK                       equ     1<<HW_GP_B11_BITPOS
HW_GP_B12_SETMASK                       equ     1<<HW_GP_B12_BITPOS
HW_GP_B13_SETMASK                       equ     1<<HW_GP_B13_BITPOS
HW_GP_B14_SETMASK                       equ     1<<HW_GP_B14_BITPOS
HW_GP_B15_SETMASK                       equ     1<<HW_GP_B15_BITPOS
HW_GP_B16_SETMASK                       equ     1<<HW_GP_B16_BITPOS
HW_GP_B17_SETMASK                       equ     1<<HW_GP_B17_BITPOS
HW_GP_B18_SETMASK                       equ     1<<HW_GP_B18_BITPOS
HW_GP_B19_SETMASK                       equ     1<<HW_GP_B19_BITPOS
HW_GP_B20_SETMASK                       equ     1<<HW_GP_B20_BITPOS
HW_GP_B21_SETMASK                       equ     1<<HW_GP_B21_BITPOS
HW_GP_B22_SETMASK                       equ     1<<HW_GP_B22_BITPOS
HW_GP_B23_SETMASK                       equ     1<<HW_GP_B23_BITPOS

HW_GP_B0_CLRMASK                        equ     ~HW_GP_B0_SETMASK
HW_GP_B1_CLRMASK                        equ     ~HW_GP_B1_SETMASK
HW_GP_B2_CLRMASK                        equ     ~HW_GP_B2_SETMASK
HW_GP_B3_CLRMASK                        equ     ~HW_GP_B3_SETMASK
HW_GP_B4_CLRMASK                        equ     ~HW_GP_B4_SETMASK
HW_GP_B5_CLRMASK                        equ     ~HW_GP_B5_SETMASK
HW_GP_B6_CLRMASK                        equ     ~HW_GP_B6_SETMASK
HW_GP_B7_CLRMASK                        equ     ~HW_GP_B7_SETMASK
HW_GP_B8_CLRMASK                        equ     ~HW_GP_B8_SETMASK
HW_GP_B9_CLRMASK                        equ     ~HW_GP_B9_SETMASK
HW_GP_B10_CLRMASK                       equ     ~HW_GP_B10_SETMASK
HW_GP_B11_CLRMASK                       equ     ~HW_GP_B11_SETMASK
HW_GP_B12_CLRMASK                       equ     ~HW_GP_B12_SETMASK
HW_GP_B13_CLRMASK                       equ     ~HW_GP_B13_SETMASK
HW_GP_B14_CLRMASK                       equ     ~HW_GP_B14_SETMASK
HW_GP_B15_CLRMASK                       equ     ~HW_GP_B15_SETMASK
HW_GP_B16_CLRMASK                       equ     ~HW_GP_B16_SETMASK
HW_GP_B17_CLRMASK                       equ     ~HW_GP_B17_SETMASK
HW_GP_B18_CLRMASK                       equ     ~HW_GP_B18_SETMASK
HW_GP_B19_CLRMASK                       equ     ~HW_GP_B19_SETMASK
HW_GP_B20_CLRMASK                       equ     ~HW_GP_B20_SETMASK
HW_GP_B21_CLRMASK                       equ     ~HW_GP_B21_SETMASK
HW_GP_B22_CLRMASK                       equ     ~HW_GP_B22_SETMASK
HW_GP_B23_CLRMASK                       equ     ~HW_GP_B23_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  GPIO 8mA Register Bit Positions
HW_GP8MA_B7_B0_BITPOS                   equ     0
HW_GP8MA_B15_B8_BITPOS                  equ     1
HW_GP8MA_B23_B16_BITPOS                 equ     2
HW_GP8MA_CLK_GATE_BITPOS                equ     23


;///////////////////////////////////////////////////////////////////////////////
;  Logical GPIO numbers
HW_GPIO_000                             equ     0
HW_GPIO_001                             equ     1
HW_GPIO_002                             equ     2
HW_GPIO_003                             equ     3
HW_GPIO_004                             equ     4
HW_GPIO_005                             equ     5
HW_GPIO_006                             equ     6
HW_GPIO_007                             equ     7
HW_GPIO_008                             equ     8
HW_GPIO_009                             equ     9
HW_GPIO_010                             equ     10
HW_GPIO_011                             equ     11
HW_GPIO_012                             equ     12
HW_GPIO_013                             equ     13
HW_GPIO_014                             equ     14
HW_GPIO_015                             equ     15
HW_GPIO_016                             equ     16
HW_GPIO_017                             equ     17
HW_GPIO_018                             equ     18
HW_GPIO_019                             equ     19
HW_GPIO_020                             equ     20
HW_GPIO_021                             equ     21
HW_GPIO_022                             equ     22
HW_GPIO_023                             equ     23
HW_GPIO_024                             equ     24
HW_GPIO_025                             equ     25
HW_GPIO_026                             equ     26
HW_GPIO_027                             equ     27
HW_GPIO_028                             equ     28
HW_GPIO_029                             equ     29
HW_GPIO_030                             equ     30
HW_GPIO_031                             equ     31
HW_GPIO_032                             equ     32
HW_GPIO_033                             equ     33
HW_GPIO_034                             equ     34
HW_GPIO_035                             equ     35
HW_GPIO_036                             equ     36
HW_GPIO_037                             equ     37
HW_GPIO_038                             equ     38
HW_GPIO_039                             equ     39
HW_GPIO_040                             equ     40
HW_GPIO_041                             equ     41
HW_GPIO_042                             equ     42
HW_GPIO_043                             equ     43
HW_GPIO_044                             equ     44
HW_GPIO_045                             equ     45
HW_GPIO_046                             equ     46
HW_GPIO_047                             equ     47
HW_GPIO_048                             equ     48
HW_GPIO_049                             equ     49
HW_GPIO_050                             equ     50
HW_GPIO_051                             equ     51
HW_GPIO_052                             equ     52
HW_GPIO_053                             equ     53
HW_GPIO_054                             equ     54
HW_GPIO_055                             equ     55
HW_GPIO_056                             equ     56
HW_GPIO_057                             equ     57
HW_GPIO_058                             equ     58
HW_GPIO_059                             equ     59
HW_GPIO_060                             equ     60
HW_GPIO_061                             equ     61
HW_GPIO_062                             equ     62
HW_GPIO_063                             equ     63
HW_GPIO_064                             equ     64
HW_GPIO_065                             equ     65
HW_GPIO_066                             equ     66
HW_GPIO_067                             equ     67
HW_GPIO_068                             equ     68
HW_GPIO_069                             equ     69
HW_GPIO_070                             equ     70
HW_GPIO_071                             equ     71
HW_GPIO_072                             equ     72
HW_GPIO_073                             equ     73
HW_GPIO_074                             equ     74
HW_GPIO_075                             equ     75
HW_GPIO_076                             equ     76
HW_GPIO_077                             equ     77
HW_GPIO_078                             equ     78
HW_GPIO_079                             equ     79
HW_GPIO_080                             equ     80
HW_GPIO_081                             equ     81
HW_GPIO_082                             equ     82
HW_GPIO_083                             equ     83
HW_GPIO_084                             equ     84
HW_GPIO_085                             equ     85
HW_GPIO_086                             equ     86
HW_GPIO_087                             equ     87
HW_GPIO_088                             equ     88
HW_GPIO_089                             equ     89
HW_GPIO_090                             equ     90
HW_GPIO_091                             equ     91
HW_GPIO_092                             equ     92
HW_GPIO_093                             equ     93
HW_GPIO_094                             equ     94
HW_GPIO_095                             equ     95
HW_GPIO_LAST                            equ     HW_GPIO_095

;C Insert
;#define HW_GP0ENR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPENR))  /* GPIO 0 Enable Register   */
;#define HW_GP0DOR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOR)) /* GPIO 0 Data Out Register */
;#define HW_GP0DIR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDIR)) /* GPIO 0 Dait In Register  */
;#define HW_GP0DOER     (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPDOER)) /* GPIO 0 Dait Out Enable Register  */
;#define HW_GP0IPENR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 0 Interrupt Pin Enable Register */
;#define HW_GP0IENR     (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIENR)) /* GPIO 0 Interrupt Enable Register */
;#define HW_GP0ILVLR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 0 Interrupt Level Register  */
;#define HW_GP0IPOLR    (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 0 Interrupt Polarity Register   */
;#define HW_GP0ISTATR   (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 0 Interrupt Status Register */
;#define HW_GP0PWR      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
;#define HW_GP08MA      (*(volatile gpr_type _X*) (HW_GPB0_BASEADDR+HW_GPB_GP8MA)) 

;#define HW_GP1ENR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPENR))     /* GPIO 1 Enable Register   */
;#define HW_GP1DOR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDOR)) /* GPIO 1 Data Out Register */
;#define HW_GP1DIR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDIR)) /* GPIO 1 Dait In Register  */
;#define HW_GP1DOER     (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPDOER)) /* GPIO 1 Dait Out Enable Register  */
;#define HW_GP1IPENR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 1 Interrupt Pin Enable Register */
;#define HW_GP1IENR     (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIENR)) /* GPIO 1 Interrupt Enable Register */
;#define HW_GP1ILVLR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 1 Interrupt Level Register  */
;#define HW_GP1IPOLR    (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 1 Interrupt Polarity Register   */
;#define HW_GP1ISTATR   (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 1 Interrupt Status Register */
;#define HW_GP1PWR      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
;#define HW_GP18MA      (*(volatile gpr_type _X*) (HW_GPB1_BASEADDR+HW_GPB_GP8MA)) 

;#define HW_GP2ENR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPENR))     /* GPIO 2 Enable Register   */
;#define HW_GP2DOR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDOR)) /* GPIO 2 Data Out Register */
;#define HW_GP2DIR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDIR)) /* GPIO 2 Dait In Register  */
;#define HW_GP2DOER     (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPDOER)) /* GPIO 2 Dait Out Enable Register  */
;#define HW_GP2IPENR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 2 Interrupt Pin Enable Register */
;#define HW_GP2IENR     (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIENR)) /* GPIO 2 Interrupt Enable Register */
;#define HW_GP2ILVLR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 2 Interrupt Level Register  */
;#define HW_GP2IPOLR    (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 2 Interrupt Polarity Register   */
;#define HW_GP2ISTATR   (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 2 Interrupt Status Register */
;#define HW_GP2PWR      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
;#define HW_GP28MA      (*(volatile gpr_type _X*) (HW_GPB2_BASEADDR+HW_GPB_GP8MA)) 

;#define HW_GP3ENR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPENR))     /* GPIO 2 Enable Register   */
;#define HW_GP3DOR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDOR)) /* GPIO 2 Data Out Register */
;#define HW_GP3DIR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDIR)) /* GPIO 2 Dait In Register  */
;#define HW_GP3DOER     (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPDOER)) /* GPIO 2 Dait Out Enable Register  */
;#define HW_GP3IPENR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIPENR)) /* GPIO 2 Interrupt Pin Enable Register */
;#define HW_GP3IENR     (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIENR)) /* GPIO 2 Interrupt Enable Register */
;#define HW_GP3ILVLR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPILVLR)) /* GPIO 2 Interrupt Level Register  */
;#define HW_GP3IPOLR    (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPIPOLR)) /* GPIO 2 Interrupt Polarity Register   */
;#define HW_GP3ISTATR   (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPISTATR)) /* GPIO 2 Interrupt Status Register */
;#define HW_GP3PWR      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GPPWR)) /* GPIO 0 Power Register */
;#define HW_GP38MA      (*(volatile gpr_type _X*) (HW_GPB3_BASEADDR+HW_GPB_GP8MA)) 
;endc

    endif
