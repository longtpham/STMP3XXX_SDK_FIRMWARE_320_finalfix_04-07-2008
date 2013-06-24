    if !@def(__REGS_ICOLL_INC)
__REGS_ICOLL_INC equ 1

;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Registers
;///////////////////////////////////////////////////////////////////////////////

HW_ICOLL_BASEADDR                       equ     $F300

;ASM Only
HW_ICLENABLE0R                          equ     HW_ICOLL_BASEADDR
HW_ICLENABLE1R                          equ     HW_ICOLL_BASEADDR+1
HW_ICLSTATUS0R                          equ     HW_ICOLL_BASEADDR+2
HW_ICLSTATUS1R                          equ     HW_ICOLL_BASEADDR+3
HW_ICLPRIOR0R                           equ     HW_ICOLL_BASEADDR+4
HW_ICLPRIOR1R                           equ     HW_ICOLL_BASEADDR+5
HW_ICLPRIOR2R                           equ     HW_ICOLL_BASEADDR+6
HW_ICLPRIOR3R                           equ     HW_ICOLL_BASEADDR+7
HW_ICLSTEER0R                           equ     HW_ICOLL_BASEADDR+8
HW_ICLSTEER1R                           equ     HW_ICOLL_BASEADDR+9
HW_ICLSTEER2R                           equ     HW_ICOLL_BASEADDR+10
HW_ICLFORCE0R                           equ     HW_ICOLL_BASEADDR+11
HW_ICLFORCE1R                           equ     HW_ICOLL_BASEADDR+12
HW_ICLFENABLE0R                         equ     HW_ICOLL_BASEADDR+13
HW_ICLFENABLE1R                         equ     HW_ICOLL_BASEADDR+14
HW_ICLOBSVZ0R                           equ     HW_ICOLL_BASEADDR+15
HW_ICLOBSVZ1R                           equ     HW_ICOLL_BASEADDR+16
HW_ICLPRIOR4R                           equ     HW_ICOLL_BASEADDR+17
HW_IPR                                  equ     $FFFF                 ; Interrupt Priority register
;endasm


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector 0 Enable Register (HW_ICLENABLE0R) Bit Positions

;C Insert
;typedef union
;{
;    struct {
;        int SEN0        :1;
;        int SEN1        :1;
;        int SEN2        :1;
;        int SEN3        :1;
;        int SEN4        :1;
;        int SEN5        :1;
;        int SEN6        :1;
;        int SEN7        :1;
;        int SEN8        :1;
;        int SEN9        :1;
;        int SEN10       :1;
;        int SEN11       :1;
;        int SEN12       :1;
;        int SEN13       :1;
;        int SEN14       :1;
;        int SEN15       :1;
;        int SEN16       :1;
;        int SEN17       :1;
;        int SEN18       :1;
;        int SEN19       :1;
;        int SEN20       :1;
;        int SEN21       :1;
;        int SEN22       :1;
;        int SEN23       :1;
;    } B;
;    int I;
;} iclenable0_type;
;#define HW_ICLENABLE0R  (*(volatile iclenable0_type _X*) (HW_ICOLL_BASEADDR)) /* Interrupt Priority Register Core   */
;endc


HW_ICLENABLE0R_SEN0_BITPOS              equ     0
HW_ICLENABLE0R_SEN1_BITPOS              equ     1
HW_ICLENABLE0R_SEN2_BITPOS              equ     2
HW_ICLENABLE0R_SEN3_BITPOS              equ     3
HW_ICLENABLE0R_SEN4_BITPOS              equ     4
HW_ICLENABLE0R_SEN5_BITPOS              equ     5
HW_ICLENABLE0R_SEN6_BITPOS              equ     6
HW_ICLENABLE0R_SEN7_BITPOS              equ     7
HW_ICLENABLE0R_SEN8_BITPOS              equ     8
HW_ICLENABLE0R_SEN9_BITPOS              equ     9
HW_ICLENABLE0R_SEN10_BITPOS             equ     10
HW_ICLENABLE0R_SEN11_BITPOS             equ     11
HW_ICLENABLE0R_SEN12_BITPOS             equ     12
HW_ICLENABLE0R_SEN13_BITPOS             equ     13
HW_ICLENABLE0R_SEN14_BITPOS             equ     14
HW_ICLENABLE0R_SEN15_BITPOS             equ     15
HW_ICLENABLE0R_SEN16_BITPOS             equ     16
HW_ICLENABLE0R_SEN17_BITPOS             equ     17
HW_ICLENABLE0R_SEN18_BITPOS             equ     18
HW_ICLENABLE0R_SEN19_BITPOS             equ     19
HW_ICLENABLE0R_SEN20_BITPOS             equ     20
HW_ICLENABLE0R_SEN21_BITPOS             equ     21
HW_ICLENABLE0R_SEN22_BITPOS             equ     22
HW_ICLENABLE0R_SEN23_BITPOS             equ     23

HW_ICLENABLE0R_SEN0_SETMASK             equ     1<<HW_ICLENABLE0R_SEN0_BITPOS
HW_ICLENABLE0R_SEN1_SETMASK             equ     1<<HW_ICLENABLE0R_SEN1_BITPOS
HW_ICLENABLE0R_SEN2_SETMASK             equ     1<<HW_ICLENABLE0R_SEN2_BITPOS
HW_ICLENABLE0R_SEN3_SETMASK             equ     1<<HW_ICLENABLE0R_SEN3_BITPOS
HW_ICLENABLE0R_SEN4_SETMASK             equ     1<<HW_ICLENABLE0R_SEN4_BITPOS
HW_ICLENABLE0R_SEN5_SETMASK             equ     1<<HW_ICLENABLE0R_SEN5_BITPOS
HW_ICLENABLE0R_SEN6_SETMASK             equ     1<<HW_ICLENABLE0R_SEN6_BITPOS
HW_ICLENABLE0R_SEN7_SETMASK             equ     1<<HW_ICLENABLE0R_SEN7_BITPOS
HW_ICLENABLE0R_SEN8_SETMASK             equ     1<<HW_ICLENABLE0R_SEN8_BITPOS
HW_ICLENABLE0R_SEN9_SETMASK             equ     1<<HW_ICLENABLE0R_SEN9_BITPOS
HW_ICLENABLE0R_SEN10_SETMASK            equ     1<<HW_ICLENABLE0R_SEN10_BITPOS
HW_ICLENABLE0R_SEN11_SETMASK            equ     1<<HW_ICLENABLE0R_SEN11_BITPOS
HW_ICLENABLE0R_SEN12_SETMASK            equ     1<<HW_ICLENABLE0R_SEN12_BITPOS
HW_ICLENABLE0R_SEN13_SETMASK            equ     1<<HW_ICLENABLE0R_SEN13_BITPOS
HW_ICLENABLE0R_SEN14_SETMASK            equ     1<<HW_ICLENABLE0R_SEN14_BITPOS
HW_ICLENABLE0R_SEN15_SETMASK            equ     1<<HW_ICLENABLE0R_SEN15_BITPOS
HW_ICLENABLE0R_SEN16_SETMASK            equ     1<<HW_ICLENABLE0R_SEN16_BITPOS
HW_ICLENABLE0R_SEN17_SETMASK            equ     1<<HW_ICLENABLE0R_SEN17_BITPOS
HW_ICLENABLE0R_SEN18_SETMASK            equ     1<<HW_ICLENABLE0R_SEN18_BITPOS
HW_ICLENABLE0R_SEN19_SETMASK            equ     1<<HW_ICLENABLE0R_SEN19_BITPOS
HW_ICLENABLE0R_SEN20_SETMASK            equ     1<<HW_ICLENABLE0R_SEN20_BITPOS
HW_ICLENABLE0R_SEN21_SETMASK            equ     1<<HW_ICLENABLE0R_SEN21_BITPOS
HW_ICLENABLE0R_SEN22_SETMASK            equ     1<<HW_ICLENABLE0R_SEN22_BITPOS
HW_ICLENABLE0R_SEN23_SETMASK            equ     1<<HW_ICLENABLE0R_SEN23_BITPOS

HW_ICLENABLE0R_SEN0_CLRMASK             equ     ~HW_ICLENABLE0R_SEN0_SETMASK
HW_ICLENABLE0R_SEN1_CLRMASK             equ     ~HW_ICLENABLE0R_SEN1_SETMASK
HW_ICLENABLE0R_SEN2_CLRMASK             equ     ~HW_ICLENABLE0R_SEN2_SETMASK
HW_ICLENABLE0R_SEN3_CLRMASK             equ     ~HW_ICLENABLE0R_SEN3_SETMASK
HW_ICLENABLE0R_SEN4_CLRMASK             equ     ~HW_ICLENABLE0R_SEN4_SETMASK
HW_ICLENABLE0R_SEN5_CLRMASK             equ     ~HW_ICLENABLE0R_SEN5_SETMASK
HW_ICLENABLE0R_SEN6_CLRMASK             equ     ~HW_ICLENABLE0R_SEN6_SETMASK
HW_ICLENABLE0R_SEN7_CLRMASK             equ     ~HW_ICLENABLE0R_SEN7_SETMASK
HW_ICLENABLE0R_SEN8_CLRMASK             equ     ~HW_ICLENABLE0R_SEN8_SETMASK
HW_ICLENABLE0R_SEN9_CLRMASK             equ     ~HW_ICLENABLE0R_SEN9_SETMASK
HW_ICLENABLE0R_SEN10_CLRMASK            equ     ~HW_ICLENABLE0R_SEN10_SETMASK
HW_ICLENABLE0R_SEN11_CLRMASK            equ     ~HW_ICLENABLE0R_SEN11_SETMASK
HW_ICLENABLE0R_SEN12_CLRMASK            equ     ~HW_ICLENABLE0R_SEN12_SETMASK
HW_ICLENABLE0R_SEN13_CLRMASK            equ     ~HW_ICLENABLE0R_SEN13_SETMASK
HW_ICLENABLE0R_SEN14_CLRMASK            equ     ~HW_ICLENABLE0R_SEN14_SETMASK
HW_ICLENABLE0R_SEN15_CLRMASK            equ     ~HW_ICLENABLE0R_SEN15_SETMASK
HW_ICLENABLE0R_SEN16_CLRMASK            equ     ~HW_ICLENABLE0R_SEN16_SETMASK
HW_ICLENABLE0R_SEN17_CLRMASK            equ     ~HW_ICLENABLE0R_SEN17_SETMASK
HW_ICLENABLE0R_SEN18_CLRMASK            equ     ~HW_ICLENABLE0R_SEN18_SETMASK
HW_ICLENABLE0R_SEN19_CLRMASK            equ     ~HW_ICLENABLE0R_SEN19_SETMASK
HW_ICLENABLE0R_SEN20_CLRMASK            equ     ~HW_ICLENABLE0R_SEN20_SETMASK
HW_ICLENABLE0R_SEN21_CLRMASK            equ     ~HW_ICLENABLE0R_SEN21_SETMASK
HW_ICLENABLE0R_SEN22_CLRMASK            equ     ~HW_ICLENABLE0R_SEN22_SETMASK
HW_ICLENABLE0R_SEN23_CLRMASK            equ     ~HW_ICLENABLE0R_SEN23_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector 1 Enable Register (HW_ICLENABLE1R) Bit Positions
;C Insert
;typedef union
;{
;    struct {
;        
;        int SEN24       :1;
;        int SEN25       :1;
;        int SEN26       :1;
;        int SEN27       :1;
;        int SEN28       :1;
;        int SEN29       :1;
;        int SEN30       :1;
;        int SEN31       :1;
;        int SEN32       :1;
;        int SEN33       :1;
;    } B;
;    int I;
;} iclenable1_type;
;
;#define HW_ICLENABLE1R  (*(volatile iclenable1_type _X*) (HW_ICOLL_BASEADDR+1)) /* Interrupt Priority Register Core    */
;endC

HW_ICLENABLE1R_SEN24_BITPOS             equ     0
HW_ICLENABLE1R_SEN25_BITPOS             equ     1
HW_ICLENABLE1R_SEN26_BITPOS             equ     2
HW_ICLENABLE1R_SEN27_BITPOS             equ     3
HW_ICLENABLE1R_SEN28_BITPOS             equ     4
HW_ICLENABLE1R_SEN29_BITPOS             equ     5
HW_ICLENABLE1R_SEN30_BITPOS             equ     6
HW_ICLENABLE1R_SEN31_BITPOS             equ     7
HW_ICLENABLE1R_SEN32_BITPOS             equ     8
HW_ICLENABLE1R_SEN33_BITPOS             equ     9

HW_ICLENABLE1R_SEN24_SETMASK            equ     1<<HW_ICLENABLE1R_SEN24_BITPOS
HW_ICLENABLE1R_SEN25_SETMASK            equ     1<<HW_ICLENABLE1R_SEN25_BITPOS
HW_ICLENABLE1R_SEN26_SETMASK            equ     1<<HW_ICLENABLE1R_SEN26_BITPOS
HW_ICLENABLE1R_SEN27_SETMASK            equ     1<<HW_ICLENABLE1R_SEN27_BITPOS
HW_ICLENABLE1R_SEN28_SETMASK            equ     1<<HW_ICLENABLE1R_SEN28_BITPOS
HW_ICLENABLE1R_SEN29_SETMASK            equ     1<<HW_ICLENABLE1R_SEN29_BITPOS
HW_ICLENABLE1R_SEN30_SETMASK            equ     1<<HW_ICLENABLE1R_SEN30_BITPOS
HW_ICLENABLE1R_SEN31_SETMASK            equ     1<<HW_ICLENABLE1R_SEN31_BITPOS
HW_ICLENABLE1R_SEN32_SETMASK            equ     1<<HW_ICLENABLE1R_SEN32_BITPOS
HW_ICLENABLE1R_SEN33_SETMASK            equ     1<<HW_ICLENABLE1R_SEN33_BITPOS

HW_ICLENABLE1R_SEN24_CLRMASK            equ     ~HW_ICLENABLE1R_SEN24_SETMASK
HW_ICLENABLE1R_SEN25_CLRMASK            equ     ~HW_ICLENABLE1R_SEN25_SETMASK
HW_ICLENABLE1R_SEN26_CLRMASK            equ     ~HW_ICLENABLE1R_SEN26_SETMASK
HW_ICLENABLE1R_SEN27_CLRMASK            equ     ~HW_ICLENABLE1R_SEN27_SETMASK
HW_ICLENABLE1R_SEN28_CLRMASK            equ     ~HW_ICLENABLE1R_SEN28_SETMASK
HW_ICLENABLE1R_SEN29_CLRMASK            equ     ~HW_ICLENABLE1R_SEN29_SETMASK
HW_ICLENABLE1R_SEN30_CLRMASK            equ     ~HW_ICLENABLE1R_SEN30_SETMASK
HW_ICLENABLE1R_SEN31_CLRMASK            equ     ~HW_ICLENABLE1R_SEN31_SETMASK
HW_ICLENABLE1R_SEN32_CLRMASK            equ     ~HW_ICLENABLE1R_SEN32_SETMASK
HW_ICLENABLE1R_SEN33_CLRMASK            equ     ~HW_ICLENABLE1R_SEN33_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Status 0 Register (HW_ICLSTATUS0R) Bit Positions
;C Insert
;typedef union
;{
;    struct {
;        int SST0        :1;
;        int SST1        :1;
;        int SST2        :1;
;        int SST3        :1;
;        int SST4        :1;
;        int SST5        :1;
;        int SST6        :1;
;        int SST7        :1;
;        int SST8        :1;
;        int SST9        :1;
;        int SST10       :1;
;        int SST11       :1;
;        int SST12       :1;
;        int SST13       :1;
;        int SST14       :1;
;        int SST15       :1;
;        int SST16       :1;
;        int SST17       :1;
;        int SST18       :1;
;        int SST19       :1;
;        int SST20       :1;
;        int SST21       :1;
;        int SST22       :1;
;        int SST23       :1;
;    } B;
;    int I;
;} iclstatus0_type;
;#define HW_ICLSTATUS0R  (*(volatile iclstatus0_type _X*) (HW_ICOLL_BASEADDR+2)) /* Interrupt Priority Register Core */
;endC
HW_ICLSTATUS0R_SST0_BITPOS              equ     0
HW_ICLSTATUS0R_SST1_BITPOS              equ     1
HW_ICLSTATUS0R_SST2_BITPOS              equ     2
HW_ICLSTATUS0R_SST3_BITPOS              equ     3
HW_ICLSTATUS0R_SST4_BITPOS              equ     4
HW_ICLSTATUS0R_SST5_BITPOS              equ     5
HW_ICLSTATUS0R_SST6_BITPOS              equ     6
HW_ICLSTATUS0R_SST7_BITPOS              equ     7
HW_ICLSTATUS0R_SST8_BITPOS              equ     8
HW_ICLSTATUS0R_SST9_BITPOS              equ     9
HW_ICLSTATUS0R_SST10_BITPOS             equ     10
HW_ICLSTATUS0R_SST11_BITPOS             equ     11
HW_ICLSTATUS0R_SST12_BITPOS             equ     12
HW_ICLSTATUS0R_SST13_BITPOS             equ     13
HW_ICLSTATUS0R_SST14_BITPOS             equ     14
HW_ICLSTATUS0R_SST15_BITPOS             equ     15
HW_ICLSTATUS0R_SST16_BITPOS             equ     16
HW_ICLSTATUS0R_SST17_BITPOS             equ     17
HW_ICLSTATUS0R_SST18_BITPOS             equ     18
HW_ICLSTATUS0R_SST19_BITPOS             equ     19
HW_ICLSTATUS0R_SST20_BITPOS             equ     20
HW_ICLSTATUS0R_SST21_BITPOS             equ     21
HW_ICLSTATUS0R_SST22_BITPOS             equ     22
HW_ICLSTATUS0R_SST23_BITPOS             equ     23

HW_ICLSTATUS0R_SST0_SETMASK             equ     1<<HW_ICLSTATUS0R_SST0_BITPOS
HW_ICLSTATUS0R_SST1_SETMASK             equ     1<<HW_ICLSTATUS0R_SST1_BITPOS
HW_ICLSTATUS0R_SST2_SETMASK             equ     1<<HW_ICLSTATUS0R_SST2_BITPOS
HW_ICLSTATUS0R_SST3_SETMASK             equ     1<<HW_ICLSTATUS0R_SST3_BITPOS
HW_ICLSTATUS0R_SST4_SETMASK             equ     1<<HW_ICLSTATUS0R_SST4_BITPOS
HW_ICLSTATUS0R_SST5_SETMASK             equ     1<<HW_ICLSTATUS0R_SST5_BITPOS
HW_ICLSTATUS0R_SST6_SETMASK             equ     1<<HW_ICLSTATUS0R_SST6_BITPOS
HW_ICLSTATUS0R_SST7_SETMASK             equ     1<<HW_ICLSTATUS0R_SST7_BITPOS
HW_ICLSTATUS0R_SST8_SETMASK             equ     1<<HW_ICLSTATUS0R_SST8_BITPOS
HW_ICLSTATUS0R_SST9_SETMASK             equ     1<<HW_ICLSTATUS0R_SST9_BITPOS
HW_ICLSTATUS0R_SST10_SETMASK            equ     1<<HW_ICLSTATUS0R_SST10_BITPOS
HW_ICLSTATUS0R_SST11_SETMASK            equ     1<<HW_ICLSTATUS0R_SST11_BITPOS
HW_ICLSTATUS0R_SST12_SETMASK            equ     1<<HW_ICLSTATUS0R_SST12_BITPOS
HW_ICLSTATUS0R_SST13_SETMASK            equ     1<<HW_ICLSTATUS0R_SST13_BITPOS
HW_ICLSTATUS0R_SST14_SETMASK            equ     1<<HW_ICLSTATUS0R_SST14_BITPOS
HW_ICLSTATUS0R_SST15_SETMASK            equ     1<<HW_ICLSTATUS0R_SST15_BITPOS
HW_ICLSTATUS0R_SST16_SETMASK            equ     1<<HW_ICLSTATUS0R_SST16_BITPOS
HW_ICLSTATUS0R_SST17_SETMASK            equ     1<<HW_ICLSTATUS0R_SST17_BITPOS
HW_ICLSTATUS0R_SST18_SETMASK            equ     1<<HW_ICLSTATUS0R_SST18_BITPOS
HW_ICLSTATUS0R_SST19_SETMASK            equ     1<<HW_ICLSTATUS0R_SST19_BITPOS
HW_ICLSTATUS0R_SST20_SETMASK            equ     1<<HW_ICLSTATUS0R_SST20_BITPOS
HW_ICLSTATUS0R_SST21_SETMASK            equ     1<<HW_ICLSTATUS0R_SST21_BITPOS
HW_ICLSTATUS0R_SST22_SETMASK            equ     1<<HW_ICLSTATUS0R_SST22_BITPOS
HW_ICLSTATUS0R_SST23_SETMASK            equ     1<<HW_ICLSTATUS0R_SST23_BITPOS

HW_ICLSTATUS0R_SST0_CLRMASK             equ     ~HW_ICLSTATUS0R_SST0_SETMASK
HW_ICLSTATUS0R_SST1_CLRMASK             equ     ~HW_ICLSTATUS0R_SST1_SETMASK
HW_ICLSTATUS0R_SST2_CLRMASK             equ     ~HW_ICLSTATUS0R_SST2_SETMASK
HW_ICLSTATUS0R_SST3_CLRMASK             equ     ~HW_ICLSTATUS0R_SST3_SETMASK
HW_ICLSTATUS0R_SST4_CLRMASK             equ     ~HW_ICLSTATUS0R_SST4_SETMASK
HW_ICLSTATUS0R_SST5_CLRMASK             equ     ~HW_ICLSTATUS0R_SST5_SETMASK
HW_ICLSTATUS0R_SST6_CLRMASK             equ     ~HW_ICLSTATUS0R_SST6_SETMASK
HW_ICLSTATUS0R_SST7_CLRMASK             equ     ~HW_ICLSTATUS0R_SST7_SETMASK
HW_ICLSTATUS0R_SST8_CLRMASK             equ     ~HW_ICLSTATUS0R_SST8_SETMASK
HW_ICLSTATUS0R_SST9_CLRMASK             equ     ~HW_ICLSTATUS0R_SST9_SETMASK
HW_ICLSTATUS0R_SST10_CLRMASK            equ     ~HW_ICLSTATUS0R_SST10_SETMASK
HW_ICLSTATUS0R_SST11_CLRMASK            equ     ~HW_ICLSTATUS0R_SST11_SETMASK
HW_ICLSTATUS0R_SST12_CLRMASK            equ     ~HW_ICLSTATUS0R_SST12_SETMASK
HW_ICLSTATUS0R_SST13_CLRMASK            equ     ~HW_ICLSTATUS0R_SST13_SETMASK
HW_ICLSTATUS0R_SST14_CLRMASK            equ     ~HW_ICLSTATUS0R_SST14_SETMASK
HW_ICLSTATUS0R_SST15_CLRMASK            equ     ~HW_ICLSTATUS0R_SST15_SETMASK
HW_ICLSTATUS0R_SST16_CLRMASK            equ     ~HW_ICLSTATUS0R_SST16_SETMASK
HW_ICLSTATUS0R_SST17_CLRMASK            equ     ~HW_ICLSTATUS0R_SST17_SETMASK
HW_ICLSTATUS0R_SST18_CLRMASK            equ     ~HW_ICLSTATUS0R_SST18_SETMASK
HW_ICLSTATUS0R_SST19_CLRMASK            equ     ~HW_ICLSTATUS0R_SST19_SETMASK
HW_ICLSTATUS0R_SST20_CLRMASK            equ     ~HW_ICLSTATUS0R_SST20_SETMASK
HW_ICLSTATUS0R_SST21_CLRMASK            equ     ~HW_ICLSTATUS0R_SST21_SETMASK
HW_ICLSTATUS0R_SST22_CLRMASK            equ     ~HW_ICLSTATUS0R_SST22_SETMASK
HW_ICLSTATUS0R_SST23_CLRMASK            equ     ~HW_ICLSTATUS0R_SST23_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Status 1 Register (HW_ICLSTATUS1R) Bit Positions
;C Insert
;typedef union
;{
;    struct {
;        int SST24       :1;
;        int SST25       :1;
;        int SST26       :1;
;        int SST27       :1;
;        int SST28       :1;
;        int SST29       :1;
;        int SST30       :1;
;        int SST31       :1;
;        int SST32       :1;
;        int SST33       :1;
;    } B;
;    int I;
;} iclstatus1_type;
;#define HW_ICLSTATUS1R  (*(volatile iclstatus1_type _X*) (HW_ICOLL_BASEADDR+3)) /* Interrupt Priority Register Core */
;endC
HW_ICLSTATUS1R_SST24_BITPOS             equ     0
HW_ICLSTATUS1R_SST25_BITPOS             equ     1
HW_ICLSTATUS1R_SST26_BITPOS             equ     2
HW_ICLSTATUS1R_SST27_BITPOS             equ     3
HW_ICLSTATUS1R_SST28_BITPOS             equ     4
HW_ICLSTATUS1R_SST29_BITPOS             equ     5
HW_ICLSTATUS1R_SST30_BITPOS             equ     6
HW_ICLSTATUS1R_SST31_BITPOS             equ     7
HW_ICLSTATUS1R_SST32_BITPOS             equ     8
HW_ICLSTATUS1R_SST33_BITPOS             equ     9

HW_ICLSTATUS1R_SST24_SETMASK            equ     1<<HW_ICLSTATUS1R_SST24_BITPOS
HW_ICLSTATUS1R_SST25_SETMASK            equ     1<<HW_ICLSTATUS1R_SST25_BITPOS
HW_ICLSTATUS1R_SST26_SETMASK            equ     1<<HW_ICLSTATUS1R_SST26_BITPOS
HW_ICLSTATUS1R_SST27_SETMASK            equ     1<<HW_ICLSTATUS1R_SST27_BITPOS
HW_ICLSTATUS1R_SST28_SETMASK            equ     1<<HW_ICLSTATUS1R_SST28_BITPOS
HW_ICLSTATUS1R_SST29_SETMASK            equ     1<<HW_ICLSTATUS1R_SST29_BITPOS
HW_ICLSTATUS1R_SST30_SETMASK            equ     1<<HW_ICLSTATUS1R_SST30_BITPOS
HW_ICLSTATUS1R_SST31_SETMASK            equ     1<<HW_ICLSTATUS1R_SST31_BITPOS
HW_ICLSTATUS1R_SST32_SETMASK            equ     1<<HW_ICLSTATUS1R_SST32_BITPOS
HW_ICLSTATUS1R_SST33_SETMASK            equ     1<<HW_ICLSTATUS1R_SST33_BITPOS

HW_ICLSTATUS1R_SST24_CLRMASK            equ     ~HW_ICLSTATUS1R_SST24_SETMASK
HW_ICLSTATUS1R_SST25_CLRMASK            equ     ~HW_ICLSTATUS1R_SST25_SETMASK
HW_ICLSTATUS1R_SST26_CLRMASK            equ     ~HW_ICLSTATUS1R_SST26_SETMASK
HW_ICLSTATUS1R_SST27_CLRMASK            equ     ~HW_ICLSTATUS1R_SST27_SETMASK
HW_ICLSTATUS1R_SST28_CLRMASK            equ     ~HW_ICLSTATUS1R_SST28_SETMASK
HW_ICLSTATUS1R_SST29_CLRMASK            equ     ~HW_ICLSTATUS1R_SST29_SETMASK
HW_ICLSTATUS1R_SST30_CLRMASK            equ     ~HW_ICLSTATUS1R_SST30_SETMASK
HW_ICLSTATUS1R_SST31_CLRMASK            equ     ~HW_ICLSTATUS1R_SST31_SETMASK
HW_ICLSTATUS1R_SST32_CLRMASK            equ     ~HW_ICLSTATUS1R_SST32_SETMASK
HW_ICLSTATUS1R_SST33_CLRMASK            equ     ~HW_ICLSTATUS1R_SST33_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority Defs
;C Insert
;typedef union
;{
;    struct {
;        unsigned S0P    :3;
;        unsigned S1P    :3;
;        unsigned S2P    :3;
;        unsigned S3P    :3;
;        unsigned S4P    :3;
;        unsigned S5P    :3;
;        unsigned S6P    :3;
;        unsigned S7P    :3;
;    } B;
;    int I;
;
;} iclprior0_type;
;
;#define HW_ICLPRIOR0R   (*(volatile iclprior0_type _X*) (HW_ICOLL_BASEADDR+4)) /* Interrupt Collector Register 0 Priority   */
;endC

HW_ICLPRIORR_SP_0                       equ     0
HW_ICLPRIORR_SP_1                       equ     1
HW_ICLPRIORR_SP_2                       equ     2
HW_ICLPRIORR_SP_3                       equ     3
HW_ICLPRIORR_SP_4                       equ     4
HW_ICLPRIORR_SP_5                       equ     5
HW_ICLPRIORR_SP_6                       equ     6
HW_ICLPRIORR_SP_7                       equ     7


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority 0 Register (HW_ICLPRIOR0R) Bit Positions
HW_ICLPRIOR0R_S0P_BITPOS                equ     0
HW_ICLPRIOR0R_S1P_BITPOS                equ     3
HW_ICLPRIOR0R_S2P_BITPOS                equ     6
HW_ICLPRIOR0R_S3P_BITPOS                equ     9
HW_ICLPRIOR0R_S4P_BITPOS                equ     12
HW_ICLPRIOR0R_S5P_BITPOS                equ     15
HW_ICLPRIOR0R_S6P_BITPOS                equ     18
HW_ICLPRIOR0R_S7P_BITPOS                equ     21

HW_ICLPRIOR0R_S0P_SETMASK               equ     7<<HW_ICLPRIOR0R_S0P_BITPOS
HW_ICLPRIOR0R_S1P_SETMASK               equ     7<<HW_ICLPRIOR0R_S1P_BITPOS
HW_ICLPRIOR0R_S2P_SETMASK               equ     7<<HW_ICLPRIOR0R_S2P_BITPOS
HW_ICLPRIOR0R_S3P_SETMASK               equ     7<<HW_ICLPRIOR0R_S3P_BITPOS
HW_ICLPRIOR0R_S4P_SETMASK               equ     7<<HW_ICLPRIOR0R_S4P_BITPOS
HW_ICLPRIOR0R_S5P_SETMASK               equ     7<<HW_ICLPRIOR0R_S5P_BITPOS
HW_ICLPRIOR0R_S6P_SETMASK               equ     7<<HW_ICLPRIOR0R_S6P_BITPOS
HW_ICLPRIOR0R_S7P_SETMASK               equ     7<<HW_ICLPRIOR0R_S7P_BITPOS

HW_ICLPRIOR0R_S0P_CLRMASK               equ     ~HW_ICLPRIOR0R_S0P_SETMASK
HW_ICLPRIOR0R_S1P_CLRMASK               equ     ~HW_ICLPRIOR0R_S1P_SETMASK
HW_ICLPRIOR0R_S2P_CLRMASK               equ     ~HW_ICLPRIOR0R_S2P_SETMASK
HW_ICLPRIOR0R_S3P_CLRMASK               equ     ~HW_ICLPRIOR0R_S3P_SETMASK
HW_ICLPRIOR0R_S4P_CLRMASK               equ     ~HW_ICLPRIOR0R_S4P_SETMASK
HW_ICLPRIOR0R_S5P_CLRMASK               equ     ~HW_ICLPRIOR0R_S5P_SETMASK
HW_ICLPRIOR0R_S6P_CLRMASK               equ     ~HW_ICLPRIOR0R_S6P_SETMASK
HW_ICLPRIOR0R_S7P_CLRMASK               equ     ~HW_ICLPRIOR0R_S7P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority 1 Register (HW_ICLPRIOR1R) Bit Positions
;C Insert
;typedef union
;{
;    struct {
;        unsigned S8P    :3;
;        unsigned S9P    :3;
;        unsigned S10P   :3;
;        unsigned S11P   :3;
;        unsigned S12P   :3;
;        unsigned S13P   :3;
;        unsigned S14P   :3;
;        unsigned S15P   :3;
;    } B;
;    int I;
;} iclprior1_type;
;
;#define HW_ICLPRIOR1R   (*(volatile iclprior1_type _X*) (HW_ICOLL_BASEADDR+5)) /* Interrupt Collector Register 1 Priority   */
;endC

HW_ICLPRIOR1R_S8P_BITPOS                equ     0
HW_ICLPRIOR1R_S9P_BITPOS                equ     3
HW_ICLPRIOR1R_S10P_BITPOS               equ     6
HW_ICLPRIOR1R_S11P_BITPOS               equ     9
HW_ICLPRIOR1R_S12P_BITPOS               equ     12
HW_ICLPRIOR1R_S13P_BITPOS               equ     15
HW_ICLPRIOR1R_S14P_BITPOS               equ     18
HW_ICLPRIOR1R_S15P_BITPOS               equ     21

HW_ICLPRIOR1R_S8P_SETMASK               equ     7<<HW_ICLPRIOR1R_S8P_BITPOS
HW_ICLPRIOR1R_S9P_SETMASK               equ     7<<HW_ICLPRIOR1R_S9P_BITPOS
HW_ICLPRIOR1R_S10P_SETMASK              equ     7<<HW_ICLPRIOR1R_S10P_BITPOS
HW_ICLPRIOR1R_S11P_SETMASK              equ     7<<HW_ICLPRIOR1R_S11P_BITPOS
HW_ICLPRIOR1R_S12P_SETMASK              equ     7<<HW_ICLPRIOR1R_S12P_BITPOS
HW_ICLPRIOR1R_S13P_SETMASK              equ     7<<HW_ICLPRIOR1R_S13P_BITPOS
HW_ICLPRIOR1R_S14P_SETMASK              equ     7<<HW_ICLPRIOR1R_S14P_BITPOS
HW_ICLPRIOR1R_S15P_SETMASK              equ     7<<HW_ICLPRIOR1R_S15P_BITPOS

HW_ICLPRIOR1R_S8P_CLRMASK               equ     ~HW_ICLPRIOR1R_S8P_SETMASK
HW_ICLPRIOR1R_S9P_CLRMASK               equ     ~HW_ICLPRIOR1R_S9P_SETMASK
HW_ICLPRIOR1R_S10P_CLRMASK              equ     ~HW_ICLPRIOR1R_S10P_SETMASK
HW_ICLPRIOR1R_S11P_CLRMASK              equ     ~HW_ICLPRIOR1R_S11P_SETMASK
HW_ICLPRIOR1R_S12P_CLRMASK              equ     ~HW_ICLPRIOR1R_S12P_SETMASK
HW_ICLPRIOR1R_S13P_CLRMASK              equ     ~HW_ICLPRIOR1R_S13P_SETMASK
HW_ICLPRIOR1R_S14P_CLRMASK              equ     ~HW_ICLPRIOR1R_S14P_SETMASK
HW_ICLPRIOR1R_S15P_CLRMASK              equ     ~HW_ICLPRIOR1R_S15P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority 2 Register (HW_ICLPRI2R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Priority 2 Register      */
;{
;    struct {
;        unsigned S16P   :3;
;        unsigned S17P   :3;
;        unsigned S18P   :3;
;        unsigned S19P   :3;
;        unsigned S20P   :3;
;        unsigned S21P   :3;
;        unsigned S22P   :3;
;        unsigned S23P   :3;
;    } B;
;    int I;
;} iclprior2_type;
;#define HW_ICLPRIOR2R   (*(volatile iclprior2_type _X*) (HW_ICOLL_BASEADDR+6)) /* Interrupt Collector Register 2 Priority   */
;endC
HW_ICLPRIOR2R_S16P_BITPOS               equ     0
HW_ICLPRIOR2R_S17P_BITPOS               equ     3
HW_ICLPRIOR2R_S18P_BITPOS               equ     6
HW_ICLPRIOR2R_S19P_BITPOS               equ     9
HW_ICLPRIOR2R_S20P_BITPOS               equ     12
HW_ICLPRIOR2R_S21P_BITPOS               equ     15
HW_ICLPRIOR2R_S22P_BITPOS               equ     18
HW_ICLPRIOR2R_S23P_BITPOS               equ     21

HW_ICLPRIOR2R_S16P_SETMASK              equ     7<<HW_ICLPRIOR2R_S16P_BITPOS
HW_ICLPRIOR2R_S17P_SETMASK              equ     7<<HW_ICLPRIOR2R_S17P_BITPOS
HW_ICLPRIOR2R_S18P_SETMASK              equ     7<<HW_ICLPRIOR2R_S18P_BITPOS
HW_ICLPRIOR2R_S19P_SETMASK              equ     7<<HW_ICLPRIOR2R_S19P_BITPOS
HW_ICLPRIOR2R_S20P_SETMASK              equ     7<<HW_ICLPRIOR2R_S20P_BITPOS
HW_ICLPRIOR2R_S21P_SETMASK              equ     7<<HW_ICLPRIOR2R_S21P_BITPOS
HW_ICLPRIOR2R_S22P_SETMASK              equ     7<<HW_ICLPRIOR2R_S22P_BITPOS
HW_ICLPRIOR2R_S23P_SETMASK              equ     7<<HW_ICLPRIOR2R_S23P_BITPOS

HW_ICLPRIOR2R_S16P_CLRMASK              equ     ~HW_ICLPRIOR2R_S16P_SETMASK
HW_ICLPRIOR2R_S17P_CLRMASK              equ     ~HW_ICLPRIOR2R_S17P_SETMASK
HW_ICLPRIOR2R_S18P_CLRMASK              equ     ~HW_ICLPRIOR2R_S18P_SETMASK
HW_ICLPRIOR2R_S19P_CLRMASK              equ     ~HW_ICLPRIOR2R_S19P_SETMASK
HW_ICLPRIOR2R_S20P_CLRMASK              equ     ~HW_ICLPRIOR2R_S20P_SETMASK
HW_ICLPRIOR2R_S21P_CLRMASK              equ     ~HW_ICLPRIOR2R_S21P_SETMASK
HW_ICLPRIOR2R_S22P_CLRMASK              equ     ~HW_ICLPRIOR2R_S22P_SETMASK
HW_ICLPRIOR2R_S23P_CLRMASK              equ     ~HW_ICLPRIOR2R_S23P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority 3 Register (HW_ICLPRI3R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Priority 3 Register      */
;{
;    struct {
;        unsigned S24P   :3;
;        unsigned S25P   :3;
;        unsigned S26P   :3;
;        unsigned S27P   :3;
;        unsigned S28P   :3;
;        unsigned S29P   :3;
;        unsigned S30P   :3;
;        unsigned S31P   :3;
;    } B;
;    int I;
;} iclprior3_type;
;#define HW_ICLPRIOR3R   (*(volatile iclprior3_type _X*) (HW_ICOLL_BASEADDR+7)) /* Interrupt Collector Register 3 Priority   */
;endC

HW_ICLPRIOR3R_S24P_BITPOS               equ     0
HW_ICLPRIOR3R_S25P_BITPOS               equ     3
HW_ICLPRIOR3R_S26P_BITPOS               equ     6
HW_ICLPRIOR3R_S27P_BITPOS               equ     9
HW_ICLPRIOR3R_S28P_BITPOS               equ     12
HW_ICLPRIOR3R_S29P_BITPOS               equ     15
HW_ICLPRIOR3R_S30P_BITPOS               equ     18
HW_ICLPRIOR3R_S31P_BITPOS               equ     21

HW_ICLPRIOR3R_S24P_SETMASK              equ     7<<HW_ICLPRIOR3R_S24P_BITPOS
HW_ICLPRIOR3R_S25P_SETMASK              equ     7<<HW_ICLPRIOR3R_S25P_BITPOS
HW_ICLPRIOR3R_S26P_SETMASK              equ     7<<HW_ICLPRIOR3R_S26P_BITPOS
HW_ICLPRIOR3R_S27P_SETMASK              equ     7<<HW_ICLPRIOR3R_S27P_BITPOS
HW_ICLPRIOR3R_S28P_SETMASK              equ     7<<HW_ICLPRIOR3R_S28P_BITPOS
HW_ICLPRIOR3R_S29P_SETMASK              equ     7<<HW_ICLPRIOR3R_S29P_BITPOS
HW_ICLPRIOR3R_S30P_SETMASK              equ     7<<HW_ICLPRIOR3R_S30P_BITPOS
HW_ICLPRIOR3R_S31P_SETMASK              equ     7<<HW_ICLPRIOR3R_S31P_BITPOS

HW_ICLPRIOR3R_S24P_CLRMASK              equ     ~HW_ICLPRIOR3R_S24P_SETMASK
HW_ICLPRIOR3R_S25P_CLRMASK              equ     ~HW_ICLPRIOR3R_S25P_SETMASK
HW_ICLPRIOR3R_S26P_CLRMASK              equ     ~HW_ICLPRIOR3R_S26P_SETMASK
HW_ICLPRIOR3R_S27P_CLRMASK              equ     ~HW_ICLPRIOR3R_S27P_SETMASK
HW_ICLPRIOR3R_S28P_CLRMASK              equ     ~HW_ICLPRIOR3R_S28P_SETMASK
HW_ICLPRIOR3R_S29P_CLRMASK              equ     ~HW_ICLPRIOR3R_S29P_SETMASK
HW_ICLPRIOR3R_S30P_CLRMASK              equ     ~HW_ICLPRIOR3R_S30P_SETMASK
HW_ICLPRIOR3R_S31P_CLRMASK              equ     ~HW_ICLPRIOR3R_S31P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Priority 4 Register (HW_ICLPRI4R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Priority 3 Register      */
;{
;    struct {
;        unsigned S32P   :3;
;        unsigned S33P   :3;
;    } B;
;    int I;
;} iclprior4_type;
;#define HW_ICLPRIOR4R (*(volatile iclprior4_type _X*) (HW_ICOLL_BASEADDR+17)) /* Interrupt Collector Register 4 Priority   */
;endC

HW_ICLPRIOR4R_S32P_BITPOS               equ     0
HW_ICLPRIOR4R_S33P_BITPOS               equ     3

HW_ICLPRIOR4R_S32P_SETMASK              equ     7<<HW_ICLPRIOR4R_S32P_BITPOS
HW_ICLPRIOR4R_S33P_SETMASK              equ     7<<HW_ICLPRIOR4R_S33P_BITPOS

HW_ICLPRIOR4R_S32P_CLRMASK              equ     ~HW_ICLPRIOR4R_S32P_SETMASK
HW_ICLPRIOR4R_S33P_CLRMASK              equ     ~HW_ICLPRIOR4R_S33P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Steering 0 Register (HW_ICLSTEER0R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Steering 0 Register      */
;{
;    struct {
;        unsigned S0S    :2;
;        unsigned S1S    :2;
;        unsigned S2S    :2;
;        unsigned S3S    :2;
;        unsigned S4S    :2;
;        unsigned S5S    :2;
;        unsigned S6S    :2;
;        unsigned S7S    :2;
;        unsigned S8S    :2;
;        unsigned S9S    :2;
;        unsigned S10S   :2;
;        unsigned S11S   :2;
;    } B;
;    int I;
;} iclsteer0_type;
;#define HW_ICLSTEER0R   (*(volatile iclsteer0_type _X*) (HW_ICOLL_BASEADDR+8)) /* Interrupt Collector Steering Register 0   */
;endC

HW_ICLSTEER0R_S0P_BITPOS                equ     0
HW_ICLSTEER0R_S1P_BITPOS                equ     2
HW_ICLSTEER0R_S2P_BITPOS                equ     4
HW_ICLSTEER0R_S3P_BITPOS                equ     6
HW_ICLSTEER0R_S4P_BITPOS                equ     8
HW_ICLSTEER0R_S5P_BITPOS                equ     10
HW_ICLSTEER0R_S6P_BITPOS                equ     12
HW_ICLSTEER0R_S7P_BITPOS                equ     14
HW_ICLSTEER0R_S8P_BITPOS                equ     16
HW_ICLSTEER0R_S9P_BITPOS                equ     18
HW_ICLSTEER0R_S10P_BITPOS               equ     20
HW_ICLSTEER0R_S11P_BITPOS               equ     22

HW_ICLSTEER0R_S0P_SETMASK               equ     3<<HW_ICLSTEER0R_S0P_BITPOS
HW_ICLSTEER0R_S1P_SETMASK               equ     3<<HW_ICLSTEER0R_S1P_BITPOS
HW_ICLSTEER0R_S2P_SETMASK               equ     3<<HW_ICLSTEER0R_S2P_BITPOS
HW_ICLSTEER0R_S3P_SETMASK               equ     3<<HW_ICLSTEER0R_S3P_BITPOS
HW_ICLSTEER0R_S4P_SETMASK               equ     3<<HW_ICLSTEER0R_S4P_BITPOS
HW_ICLSTEER0R_S5P_SETMASK               equ     3<<HW_ICLSTEER0R_S5P_BITPOS
HW_ICLSTEER0R_S6P_SETMASK               equ     3<<HW_ICLSTEER0R_S6P_BITPOS
HW_ICLSTEER0R_S7P_SETMASK               equ     3<<HW_ICLSTEER0R_S7P_BITPOS
HW_ICLSTEER0R_S8P_SETMASK               equ     3<<HW_ICLSTEER0R_S8P_BITPOS
HW_ICLSTEER0R_S9P_SETMASK               equ     3<<HW_ICLSTEER0R_S9P_BITPOS
HW_ICLSTEER0R_S10P_SETMASK              equ     3<<HW_ICLSTEER0R_S10P_BITPOS
HW_ICLSTEER0R_S11P_SETMASK              equ     3<<HW_ICLSTEER0R_S11P_BITPOS

HW_ICLSTEER0R_S0P_CLRMASK               equ     ~HW_ICLSTEER0R_S0P_SETMASK
HW_ICLSTEER0R_S1P_CLRMASK               equ     ~HW_ICLSTEER0R_S1P_SETMASK
HW_ICLSTEER0R_S2P_CLRMASK               equ     ~HW_ICLSTEER0R_S2P_SETMASK
HW_ICLSTEER0R_S3P_CLRMASK               equ     ~HW_ICLSTEER0R_S3P_SETMASK
HW_ICLSTEER0R_S4P_CLRMASK               equ     ~HW_ICLSTEER0R_S4P_SETMASK
HW_ICLSTEER0R_S5P_CLRMASK               equ     ~HW_ICLSTEER0R_S5P_SETMASK
HW_ICLSTEER0R_S6P_CLRMASK               equ     ~HW_ICLSTEER0R_S6P_SETMASK
HW_ICLSTEER0R_S7P_CLRMASK               equ     ~HW_ICLSTEER0R_S7P_SETMASK
HW_ICLSTEER0R_S8P_CLRMASK               equ     ~HW_ICLSTEER0R_S8P_SETMASK
HW_ICLSTEER0R_S9P_CLRMASK               equ     ~HW_ICLSTEER0R_S9P_SETMASK
HW_ICLSTEER0R_S10P_CLRMASK              equ     ~HW_ICLSTEER0R_S10P_SETMASK
HW_ICLSTEER0R_S11P_CLRMASK              equ     ~HW_ICLSTEER0R_S11P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Steering 1 Register (HW_ICLSTEER1R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Steering 1 Register      */
;{
;    struct {
;        unsigned S12S   :2;
;        unsigned S13S   :2;
;        unsigned S14S   :2;
;        unsigned S15S   :2;
;        unsigned S16S   :2;
;        unsigned S17S   :2;
;        unsigned S18S   :2;
;        unsigned S19S   :2;
;        unsigned S20S   :2;
;        unsigned S21S   :2;
;        unsigned S22S   :2;
;        unsigned S23S   :2;
;    } B;
;    int I;
;} iclsteer1_type;
;#define HW_ICLSTEER1R   (*(volatile iclsteer1_type _X*) (HW_ICOLL_BASEADDR+9)) /* Interrupt Collector Steering Register 1   */
;endC
HW_ICLSTEER1R_S12P_BITPOS               equ     0
HW_ICLSTEER1R_S13P_BITPOS               equ     2
HW_ICLSTEER1R_S14P_BITPOS               equ     4
HW_ICLSTEER1R_S15P_BITPOS               equ     6
HW_ICLSTEER1R_S16P_BITPOS               equ     8
HW_ICLSTEER1R_S17P_BITPOS               equ     10
HW_ICLSTEER1R_S18P_BITPOS               equ     12
HW_ICLSTEER1R_S19P_BITPOS               equ     14
HW_ICLSTEER1R_S20P_BITPOS               equ     16
HW_ICLSTEER1R_S21P_BITPOS               equ     18
HW_ICLSTEER1R_S22P_BITPOS               equ     20
HW_ICLSTEER1R_S23P_BITPOS               equ     22

HW_ICLSTEER1R_S12P_SETMASK              equ     3<<HW_ICLSTEER1R_S12P_BITPOS
HW_ICLSTEER1R_S13P_SETMASK              equ     3<<HW_ICLSTEER1R_S13P_BITPOS
HW_ICLSTEER1R_S14P_SETMASK              equ     3<<HW_ICLSTEER1R_S14P_BITPOS
HW_ICLSTEER1R_S15P_SETMASK              equ     3<<HW_ICLSTEER1R_S15P_BITPOS
HW_ICLSTEER1R_S16P_SETMASK              equ     3<<HW_ICLSTEER1R_S16P_BITPOS
HW_ICLSTEER1R_S17P_SETMASK              equ     3<<HW_ICLSTEER1R_S17P_BITPOS
HW_ICLSTEER1R_S18P_SETMASK              equ     3<<HW_ICLSTEER1R_S18P_BITPOS
HW_ICLSTEER1R_S19P_SETMASK              equ     3<<HW_ICLSTEER1R_S19P_BITPOS
HW_ICLSTEER1R_S20P_SETMASK              equ     3<<HW_ICLSTEER1R_S20P_BITPOS
HW_ICLSTEER1R_S21P_SETMASK              equ     3<<HW_ICLSTEER1R_S21P_BITPOS
HW_ICLSTEER1R_S22P_SETMASK              equ     3<<HW_ICLSTEER1R_S22P_BITPOS
HW_ICLSTEER1R_S23P_SETMASK              equ     3<<HW_ICLSTEER1R_S23P_BITPOS

HW_ICLSTEER1R_S12P_CLRMASK              equ     ~HW_ICLSTEER1R_S12P_SETMASK
HW_ICLSTEER1R_S13P_CLRMASK              equ     ~HW_ICLSTEER1R_S13P_SETMASK
HW_ICLSTEER1R_S14P_CLRMASK              equ     ~HW_ICLSTEER1R_S14P_SETMASK
HW_ICLSTEER1R_S15P_CLRMASK              equ     ~HW_ICLSTEER1R_S15P_SETMASK
HW_ICLSTEER1R_S16P_CLRMASK              equ     ~HW_ICLSTEER1R_S16P_SETMASK
HW_ICLSTEER1R_S17P_CLRMASK              equ     ~HW_ICLSTEER1R_S17P_SETMASK
HW_ICLSTEER1R_S18P_CLRMASK              equ     ~HW_ICLSTEER1R_S18P_SETMASK
HW_ICLSTEER1R_S19P_CLRMASK              equ     ~HW_ICLSTEER1R_S19P_SETMASK
HW_ICLSTEER1R_S20P_CLRMASK              equ     ~HW_ICLSTEER1R_S20P_SETMASK
HW_ICLSTEER1R_S21P_CLRMASK              equ     ~HW_ICLSTEER1R_S21P_SETMASK
HW_ICLSTEER1R_S22P_CLRMASK              equ     ~HW_ICLSTEER1R_S22P_SETMASK
HW_ICLSTEER1R_S23P_CLRMASK              equ     ~HW_ICLSTEER1R_S23P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Steering 2 Register (HW_ICLSTEER2R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Steering 2 Register      */
;{
;    struct {
;        unsigned S24S   :2;
;        unsigned S25S   :2;
;        unsigned S26S   :2;
;        unsigned S27S   :2;
;        unsigned S28S   :2;
;        unsigned S29S   :2;
;        unsigned S30S   :2;
;        unsigned S31S   :2;
;        unsigned S32S   :2;
;        unsigned S33S   :2;
;    } B;
;    int I;
;} iclsteer2_type;
;#define HW_ICLSTEER2R   (*(volatile iclsteer2_type _X*) (HW_ICOLL_BASEADDR+10)) /* Interrupt Collector Steering Register 2  */
;endC

HW_ICLSTEER2R_S24P_BITPOS               equ     0
HW_ICLSTEER2R_S25P_BITPOS               equ     2
HW_ICLSTEER2R_S26P_BITPOS               equ     4
HW_ICLSTEER2R_S27P_BITPOS               equ     6
HW_ICLSTEER2R_S28P_BITPOS               equ     8
HW_ICLSTEER2R_S29P_BITPOS               equ     10
HW_ICLSTEER2R_S30P_BITPOS               equ     12
HW_ICLSTEER2R_S31P_BITPOS               equ     14
HW_ICLSTEER2R_S32P_BITPOS               equ     16
HW_ICLSTEER2R_S33P_BITPOS               equ     18

HW_ICLSTEER2R_S24P_SETMASK              equ     3<<HW_ICLSTEER2R_S24P_BITPOS
HW_ICLSTEER2R_S25P_SETMASK              equ     3<<HW_ICLSTEER2R_S25P_BITPOS
HW_ICLSTEER2R_S26P_SETMASK              equ     3<<HW_ICLSTEER2R_S26P_BITPOS
HW_ICLSTEER2R_S27P_SETMASK              equ     3<<HW_ICLSTEER2R_S27P_BITPOS
HW_ICLSTEER2R_S28P_SETMASK              equ     3<<HW_ICLSTEER2R_S28P_BITPOS
HW_ICLSTEER2R_S29P_SETMASK              equ     3<<HW_ICLSTEER2R_S29P_BITPOS
HW_ICLSTEER2R_S30P_SETMASK              equ     3<<HW_ICLSTEER2R_S30P_BITPOS
HW_ICLSTEER2R_S31P_SETMASK              equ     3<<HW_ICLSTEER2R_S31P_BITPOS
HW_ICLSTEER2R_S32P_SETMASK              equ     3<<HW_ICLSTEER2R_S32P_BITPOS
HW_ICLSTEER2R_S33P_SETMASK              equ     3<<HW_ICLSTEER2R_S33P_BITPOS

HW_ICLSTEER2R_S24P_CLRMASK              equ     ~HW_ICLSTEER2R_S24P_SETMASK
HW_ICLSTEER2R_S25P_CLRMASK              equ     ~HW_ICLSTEER2R_S25P_SETMASK
HW_ICLSTEER2R_S26P_CLRMASK              equ     ~HW_ICLSTEER2R_S26P_SETMASK
HW_ICLSTEER2R_S27P_CLRMASK              equ     ~HW_ICLSTEER2R_S27P_SETMASK
HW_ICLSTEER2R_S28P_CLRMASK              equ     ~HW_ICLSTEER2R_S28P_SETMASK
HW_ICLSTEER2R_S29P_CLRMASK              equ     ~HW_ICLSTEER2R_S29P_SETMASK
HW_ICLSTEER2R_S30P_CLRMASK              equ     ~HW_ICLSTEER2R_S30P_SETMASK
HW_ICLSTEER2R_S31P_CLRMASK              equ     ~HW_ICLSTEER2R_S31P_SETMASK
HW_ICLSTEER2R_S32P_CLRMASK              equ     ~HW_ICLSTEER2R_S32P_SETMASK
HW_ICLSTEER2R_S33P_CLRMASK              equ     ~HW_ICLSTEER2R_S33P_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Debug Force 0 Register (HW_ICLFORCE0R) Bit Positions
;C Insert 
;typedef union               /* Interrupt Collector Debug Force 0 Register       */
;{
;    struct {
;        int S0FV        :1;
;        int S1FV        :1;
;        int S2FV        :1;
;        int S3FV        :1;
;        int S4FV        :1;
;        int S5FV        :1;
;        int S6FV        :1;
;        int S7FV        :1;
;        int S8FV        :1;
;        int S9FV        :1;
;        int S10FV       :1;
;        int S11FV       :1;
;        int S12FV       :1;
;        int S13FV       :1;
;        int S14FV       :1;
;        int S15FV       :1;
;        int S16FV       :1;
;        int S17FV       :1;
;        int S18FV       :1;
;        int S19FV       :1;
;        int S20FV       :1;
;        int S21FV       :1;
;        int S22FV       :1;
;        int S23FV       :1;
;    } B;
;    int I;
;} iclforce0_type;
;#define HW_ICLFORCE0R   (*(volatile iclforce0_type _X*) (HW_ICOLL_BASEADDR+11)) /* Interrupt Collector Debug Force Register 0   */
;endC
HW_ICLFORCE0R_S0FV_BITPOS               equ     0
HW_ICLFORCE0R_S1FV_BITPOS               equ     1
HW_ICLFORCE0R_S2FV_BITPOS               equ     2
HW_ICLFORCE0R_S3FV_BITPOS               equ     3
HW_ICLFORCE0R_S4FV_BITPOS               equ     4
HW_ICLFORCE0R_S5FV_BITPOS               equ     5
HW_ICLFORCE0R_S6FV_BITPOS               equ     6
HW_ICLFORCE0R_S7FV_BITPOS               equ     7
HW_ICLFORCE0R_S8FV_BITPOS               equ     8
HW_ICLFORCE0R_S9FV_BITPOS               equ     9
HW_ICLFORCE0R_S10FV_BITPOS              equ     10
HW_ICLFORCE0R_S11FV_BITPOS              equ     11
HW_ICLFORCE0R_S12FV_BITPOS              equ     12
HW_ICLFORCE0R_S13FV_BITPOS              equ     13
HW_ICLFORCE0R_S14FV_BITPOS              equ     14
HW_ICLFORCE0R_S15FV_BITPOS              equ     15
HW_ICLFORCE0R_S16FV_BITPOS              equ     16
HW_ICLFORCE0R_S17FV_BITPOS              equ     17
HW_ICLFORCE0R_S18FV_BITPOS              equ     18
HW_ICLFORCE0R_S19FV_BITPOS              equ     19
HW_ICLFORCE0R_S20FV_BITPOS              equ     20
HW_ICLFORCE0R_S21FV_BITPOS              equ     21
HW_ICLFORCE0R_S22FV_BITPOS              equ     22
HW_ICLFORCE0R_S23FV_BITPOS              equ     23

HW_ICLFORCE0R_S0FV_SETMASK              equ     1<<HW_ICLFORCE0R_S0FV_BITPOS
HW_ICLFORCE0R_S1FV_SETMASK              equ     1<<HW_ICLFORCE0R_S1FV_BITPOS
HW_ICLFORCE0R_S2FV_SETMASK              equ     1<<HW_ICLFORCE0R_S2FV_BITPOS
HW_ICLFORCE0R_S3FV_SETMASK              equ     1<<HW_ICLFORCE0R_S3FV_BITPOS
HW_ICLFORCE0R_S4FV_SETMASK              equ     1<<HW_ICLFORCE0R_S4FV_BITPOS
HW_ICLFORCE0R_S5FV_SETMASK              equ     1<<HW_ICLFORCE0R_S5FV_BITPOS
HW_ICLFORCE0R_S6FV_SETMASK              equ     1<<HW_ICLFORCE0R_S6FV_BITPOS
HW_ICLFORCE0R_S7FV_SETMASK              equ     1<<HW_ICLFORCE0R_S7FV_BITPOS
HW_ICLFORCE0R_S8FV_SETMASK              equ     1<<HW_ICLFORCE0R_S8FV_BITPOS
HW_ICLFORCE0R_S9FV_SETMASK              equ     1<<HW_ICLFORCE0R_S9FV_BITPOS
HW_ICLFORCE0R_S10FV_SETMASK             equ     1<<HW_ICLFORCE0R_S10FV_BITPOS
HW_ICLFORCE0R_S11FV_SETMASK             equ     1<<HW_ICLFORCE0R_S11FV_BITPOS
HW_ICLFORCE0R_S12FV_SETMASK             equ     1<<HW_ICLFORCE0R_S12FV_BITPOS
HW_ICLFORCE0R_S13FV_SETMASK             equ     1<<HW_ICLFORCE0R_S13FV_BITPOS
HW_ICLFORCE0R_S14FV_SETMASK             equ     1<<HW_ICLFORCE0R_S14FV_BITPOS
HW_ICLFORCE0R_S15FV_SETMASK             equ     1<<HW_ICLFORCE0R_S15FV_BITPOS
HW_ICLFORCE0R_S16FV_SETMASK             equ     1<<HW_ICLFORCE0R_S16FV_BITPOS
HW_ICLFORCE0R_S17FV_SETMASK             equ     1<<HW_ICLFORCE0R_S17FV_BITPOS
HW_ICLFORCE0R_S18FV_SETMASK             equ     1<<HW_ICLFORCE0R_S18FV_BITPOS
HW_ICLFORCE0R_S19FV_SETMASK             equ     1<<HW_ICLFORCE0R_S19FV_BITPOS
HW_ICLFORCE0R_S20FV_SETMASK             equ     1<<HW_ICLFORCE0R_S20FV_BITPOS
HW_ICLFORCE0R_S21FV_SETMASK             equ     1<<HW_ICLFORCE0R_S21FV_BITPOS
HW_ICLFORCE0R_S22FV_SETMASK             equ     1<<HW_ICLFORCE0R_S22FV_BITPOS
HW_ICLFORCE0R_S23FV_SETMASK             equ     1<<HW_ICLFORCE0R_S23FV_BITPOS

HW_ICLFORCE0R_S0FV_CLRMASK              equ     ~HW_ICLFORCE0R_S0FV_SETMASK
HW_ICLFORCE0R_S1FV_CLRMASK              equ     ~HW_ICLFORCE0R_S1FV_SETMASK
HW_ICLFORCE0R_S2FV_CLRMASK              equ     ~HW_ICLFORCE0R_S2FV_SETMASK
HW_ICLFORCE0R_S3FV_CLRMASK              equ     ~HW_ICLFORCE0R_S3FV_SETMASK
HW_ICLFORCE0R_S4FV_CLRMASK              equ     ~HW_ICLFORCE0R_S4FV_SETMASK
HW_ICLFORCE0R_S5FV_CLRMASK              equ     ~HW_ICLFORCE0R_S5FV_SETMASK
HW_ICLFORCE0R_S6FV_CLRMASK              equ     ~HW_ICLFORCE0R_S6FV_SETMASK
HW_ICLFORCE0R_S7FV_CLRMASK              equ     ~HW_ICLFORCE0R_S7FV_SETMASK
HW_ICLFORCE0R_S8FV_CLRMASK              equ     ~HW_ICLFORCE0R_S8FV_SETMASK
HW_ICLFORCE0R_S9FV_CLRMASK              equ     ~HW_ICLFORCE0R_S9FV_SETMASK
HW_ICLFORCE0R_S10FV_CLRMASK             equ     ~HW_ICLFORCE0R_S10FV_SETMASK
HW_ICLFORCE0R_S11FV_CLRMASK             equ     ~HW_ICLFORCE0R_S11FV_SETMASK
HW_ICLFORCE0R_S12FV_CLRMASK             equ     ~HW_ICLFORCE0R_S12FV_SETMASK
HW_ICLFORCE0R_S13FV_CLRMASK             equ     ~HW_ICLFORCE0R_S13FV_SETMASK
HW_ICLFORCE0R_S14FV_CLRMASK             equ     ~HW_ICLFORCE0R_S14FV_SETMASK
HW_ICLFORCE0R_S15FV_CLRMASK             equ     ~HW_ICLFORCE0R_S15FV_SETMASK
HW_ICLFORCE0R_S16FV_CLRMASK             equ     ~HW_ICLFORCE0R_S16FV_SETMASK
HW_ICLFORCE0R_S17FV_CLRMASK             equ     ~HW_ICLFORCE0R_S17FV_SETMASK
HW_ICLFORCE0R_S18FV_CLRMASK             equ     ~HW_ICLFORCE0R_S18FV_SETMASK
HW_ICLFORCE0R_S19FV_CLRMASK             equ     ~HW_ICLFORCE0R_S19FV_SETMASK
HW_ICLFORCE0R_S20FV_CLRMASK             equ     ~HW_ICLFORCE0R_S20FV_SETMASK
HW_ICLFORCE0R_S21FV_CLRMASK             equ     ~HW_ICLFORCE0R_S21FV_SETMASK
HW_ICLFORCE0R_S22FV_CLRMASK             equ     ~HW_ICLFORCE0R_S22FV_SETMASK
HW_ICLFORCE0R_S23FV_CLRMASK             equ     ~HW_ICLFORCE0R_S23FV_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Debug Force 1 Register (HW_ICLFORCE1R) Bit Positions
;C Insert
;typedef union               /* Interrupt Debug Force 1 Register     */
;{
;    struct {
;        int S24FV       :1;
;        int S25FV       :1;
;        int S26FV       :1;
;        int S27FV       :1;
;        int S28FV       :1;
;        int S29FV       :1;
;        int S30FV       :1;
;        int S31FV       :1;
;        int S32FV       :1;
;        int S33FV       :1;
;    } B;
;    int I;
;} iclforce1_type;
;#define HW_ICLFORCE1R   (*(volatile iclforce1_type _X*) (HW_ICOLL_BASEADDR+12)) /* Interrupt Collector Debug Force Register 1   */
;endC

HW_ICLFORCE1R_S24FV_BITPOS              equ     0
HW_ICLFORCE1R_S25FV_BITPOS              equ     1
HW_ICLFORCE1R_S26FV_BITPOS              equ     2
HW_ICLFORCE1R_S27FV_BITPOS              equ     3
HW_ICLFORCE1R_S28FV_BITPOS              equ     4
HW_ICLFORCE1R_S29FV_BITPOS              equ     5
HW_ICLFORCE1R_S30FV_BITPOS              equ     6
HW_ICLFORCE1R_S31FV_BITPOS              equ     7
HW_ICLFORCE1R_S32FV_BITPOS              equ     8
HW_ICLFORCE1R_S33FV_BITPOS              equ     9

HW_ICLFORCE1R_S24FV_SETMASK             equ     1<<HW_ICLFORCE1R_S24FV_BITPOS
HW_ICLFORCE1R_S25FV_SETMASK             equ     1<<HW_ICLFORCE1R_S25FV_BITPOS
HW_ICLFORCE1R_S26FV_SETMASK             equ     1<<HW_ICLFORCE1R_S26FV_BITPOS
HW_ICLFORCE1R_S27FV_SETMASK             equ     1<<HW_ICLFORCE1R_S27FV_BITPOS
HW_ICLFORCE1R_S28FV_SETMASK             equ     1<<HW_ICLFORCE1R_S28FV_BITPOS
HW_ICLFORCE1R_S29FV_SETMASK             equ     1<<HW_ICLFORCE1R_S29FV_BITPOS
HW_ICLFORCE1R_S30FV_SETMASK             equ     1<<HW_ICLFORCE1R_S30FV_BITPOS
HW_ICLFORCE1R_S31FV_SETMASK             equ     1<<HW_ICLFORCE1R_S31FV_BITPOS
HW_ICLFORCE1R_S32FV_SETMASK             equ     1<<HW_ICLFORCE1R_S32FV_BITPOS
HW_ICLFORCE1R_S33FV_SETMASK             equ     1<<HW_ICLFORCE1R_S33FV_BITPOS

HW_ICLFORCE1R_S24FV_CLRMASK             equ     ~HW_ICLFORCE1R_S24FV_SETMASK
HW_ICLFORCE1R_S25FV_CLRMASK             equ     ~HW_ICLFORCE1R_S25FV_SETMASK
HW_ICLFORCE1R_S26FV_CLRMASK             equ     ~HW_ICLFORCE1R_S26FV_SETMASK
HW_ICLFORCE1R_S27FV_CLRMASK             equ     ~HW_ICLFORCE1R_S27FV_SETMASK
HW_ICLFORCE1R_S28FV_CLRMASK             equ     ~HW_ICLFORCE1R_S28FV_SETMASK
HW_ICLFORCE1R_S29FV_CLRMASK             equ     ~HW_ICLFORCE1R_S29FV_SETMASK
HW_ICLFORCE1R_S30FV_CLRMASK             equ     ~HW_ICLFORCE1R_S30FV_SETMASK
HW_ICLFORCE1R_S31FV_CLRMASK             equ     ~HW_ICLFORCE1R_S31FV_SETMASK
HW_ICLFORCE1R_S32FV_CLRMASK             equ     ~HW_ICLFORCE1R_S32FV_SETMASK
HW_ICLFORCE1R_S33FV_CLRMASK             equ     ~HW_ICLFORCE1R_S33FV_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Force Enable 0 Register (HW_ICLFENABLE0R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Force Enable 0 Register      */
;{
;    struct {
;        int S0FE        :1;
;        int S1FE        :1;
;        int S2FE        :1;
;        int S3FE        :1;
;        int S4FE        :1;
;        int S5FE        :1;
;        int S6FE        :1;
;        int S7FE        :1;
;        int S8FE        :1;
;        int S9FE        :1;
;        int S10FE       :1;
;        int S11FE       :1;
;        int S12FE       :1;
;        int S13FE       :1;
;        int S14FE       :1;
;        int S15FE       :1;
;        int S16FE       :1;
;        int S17FE       :1;
;        int S18FE       :1;
;        int S19FE       :1;
;        int S20FE       :1;
;        int S21FE       :1;
;        int S22FE       :1;
;        int S23FE       :1;
;    } B;
;    int I;
;} iclfenable0_type;
;#define HW_ICLFENABLE0R (*(volatile iclfenable0_type _X*) (HW_ICOLL_BASEADDR+13)) /* Interrupt Collector Force Enable Register 0    */
;endC

HW_ICLFENABLE0R_S0FE_BITPOS             equ     0
HW_ICLFENABLE0R_S1FE_BITPOS             equ     1
HW_ICLFENABLE0R_S2FE_BITPOS             equ     2
HW_ICLFENABLE0R_S3FE_BITPOS             equ     3
HW_ICLFENABLE0R_S4FE_BITPOS             equ     4
HW_ICLFENABLE0R_S5FE_BITPOS             equ     5
HW_ICLFENABLE0R_S6FE_BITPOS             equ     6
HW_ICLFENABLE0R_S7FE_BITPOS             equ     7
HW_ICLFENABLE0R_S8FE_BITPOS             equ     8
HW_ICLFENABLE0R_S9FE_BITPOS             equ     9
HW_ICLFENABLE0R_S10FE_BITPOS            equ     10
HW_ICLFENABLE0R_S11FE_BITPOS            equ     11
HW_ICLFENABLE0R_S12FE_BITPOS            equ     12
HW_ICLFENABLE0R_S13FE_BITPOS            equ     13
HW_ICLFENABLE0R_S14FE_BITPOS            equ     14
HW_ICLFENABLE0R_S15FE_BITPOS            equ     15
HW_ICLFENABLE0R_S16FE_BITPOS            equ     16
HW_ICLFENABLE0R_S17FE_BITPOS            equ     17
HW_ICLFENABLE0R_S18FE_BITPOS            equ     18
HW_ICLFENABLE0R_S19FE_BITPOS            equ     19
HW_ICLFENABLE0R_S20FE_BITPOS            equ     20
HW_ICLFENABLE0R_S21FE_BITPOS            equ     21
HW_ICLFENABLE0R_S22FE_BITPOS            equ     22
HW_ICLFENABLE0R_S23FE_BITPOS            equ     23

HW_ICLFENABLE0R_S0FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S0FE_BITPOS
HW_ICLFENABLE0R_S1FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S1FE_BITPOS
HW_ICLFENABLE0R_S2FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S2FE_BITPOS
HW_ICLFENABLE0R_S3FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S3FE_BITPOS
HW_ICLFENABLE0R_S4FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S4FE_BITPOS
HW_ICLFENABLE0R_S5FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S5FE_BITPOS
HW_ICLFENABLE0R_S6FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S6FE_BITPOS
HW_ICLFENABLE0R_S7FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S7FE_BITPOS
HW_ICLFENABLE0R_S8FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S8FE_BITPOS
HW_ICLFENABLE0R_S9FE_SETMASK            equ     1<<HW_ICLFENABLE0R_S9FE_BITPOS
HW_ICLFENABLE0R_S10FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S10FE_BITPOS
HW_ICLFENABLE0R_S11FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S11FE_BITPOS
HW_ICLFENABLE0R_S12FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S12FE_BITPOS
HW_ICLFENABLE0R_S13FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S13FE_BITPOS
HW_ICLFENABLE0R_S14FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S14FE_BITPOS
HW_ICLFENABLE0R_S15FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S15FE_BITPOS
HW_ICLFENABLE0R_S16FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S16FE_BITPOS
HW_ICLFENABLE0R_S17FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S17FE_BITPOS
HW_ICLFENABLE0R_S18FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S18FE_BITPOS
HW_ICLFENABLE0R_S19FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S19FE_BITPOS
HW_ICLFENABLE0R_S20FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S20FE_BITPOS
HW_ICLFENABLE0R_S21FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S21FE_BITPOS
HW_ICLFENABLE0R_S22FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S22FE_BITPOS
HW_ICLFENABLE0R_S23FE_SETMASK           equ     1<<HW_ICLFENABLE0R_S23FE_BITPOS

HW_ICLFENABLE0R_S0FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S0FE_SETMASK
HW_ICLFENABLE0R_S1FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S1FE_SETMASK
HW_ICLFENABLE0R_S2FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S2FE_SETMASK
HW_ICLFENABLE0R_S3FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S3FE_SETMASK
HW_ICLFENABLE0R_S4FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S4FE_SETMASK
HW_ICLFENABLE0R_S5FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S5FE_SETMASK
HW_ICLFENABLE0R_S6FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S6FE_SETMASK
HW_ICLFENABLE0R_S7FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S7FE_SETMASK
HW_ICLFENABLE0R_S8FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S8FE_SETMASK
HW_ICLFENABLE0R_S9FE_CLRMASK            equ     ~HW_ICLFENABLE0R_S9FE_SETMASK
HW_ICLFENABLE0R_S10FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S10FE_SETMASK
HW_ICLFENABLE0R_S11FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S11FE_SETMASK
HW_ICLFENABLE0R_S12FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S12FE_SETMASK
HW_ICLFENABLE0R_S13FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S13FE_SETMASK
HW_ICLFENABLE0R_S14FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S14FE_SETMASK
HW_ICLFENABLE0R_S15FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S15FE_SETMASK
HW_ICLFENABLE0R_S16FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S16FE_SETMASK
HW_ICLFENABLE0R_S17FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S17FE_SETMASK
HW_ICLFENABLE0R_S18FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S18FE_SETMASK
HW_ICLFENABLE0R_S19FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S19FE_SETMASK
HW_ICLFENABLE0R_S20FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S20FE_SETMASK
HW_ICLFENABLE0R_S21FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S21FE_SETMASK
HW_ICLFENABLE0R_S22FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S22FE_SETMASK
HW_ICLFENABLE0R_S23FE_CLRMASK           equ     ~HW_ICLFENABLE0R_S23FE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Force Enable 1 Register (HW_ICLFENABLE1R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Force Enable 1 Register      */
;{
;    struct {
;        int S24FE       :1;
;        int S25FE       :1;
;        int S26FE       :1;
;        int S27FE       :1;
;        int S28FE       :1;
;        int S29FE       :1;
;        int S30FE       :1;
;        int S31FE       :1;
;        int S32FE       :1;
;        int S33FE       :1;
;    } B;
;    int I;
;} iclfenable1_type;
;#define HW_ICLFENABLE1R (*(volatile iclfenable1_type _X*) (HW_ICOLL_BASEADDR+14)) /* Interrupt Collector Force Enable Register 1    */
;endC
HW_ICLFENABLE1R_S24FE_BITPOS            equ     0
HW_ICLFENABLE1R_S25FE_BITPOS            equ     1
HW_ICLFENABLE1R_S26FE_BITPOS            equ     2
HW_ICLFENABLE1R_S27FE_BITPOS            equ     3
HW_ICLFENABLE1R_S28FE_BITPOS            equ     4
HW_ICLFENABLE1R_S29FE_BITPOS            equ     5
HW_ICLFENABLE1R_S30FE_BITPOS            equ     6
HW_ICLFENABLE1R_S31FE_BITPOS            equ     7
HW_ICLFENABLE1R_S32FE_BITPOS            equ     8
HW_ICLFENABLE1R_S33FE_BITPOS            equ     9

HW_ICLFENABLE1R_S24FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S24FE_BITPOS
HW_ICLFENABLE1R_S25FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S25FE_BITPOS
HW_ICLFENABLE1R_S26FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S26FE_BITPOS
HW_ICLFENABLE1R_S27FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S27FE_BITPOS
HW_ICLFENABLE1R_S28FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S28FE_BITPOS
HW_ICLFENABLE1R_S29FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S29FE_BITPOS
HW_ICLFENABLE1R_S30FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S30FE_BITPOS
HW_ICLFENABLE1R_S31FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S31FE_BITPOS
HW_ICLFENABLE1R_S32FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S32FE_BITPOS
HW_ICLFENABLE1R_S33FE_SETMASK           equ     1<<HW_ICLFENABLE1R_S33FE_BITPOS

HW_ICLFENABLE1R_S24FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S24FE_SETMASK
HW_ICLFENABLE1R_S25FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S25FE_SETMASK
HW_ICLFENABLE1R_S26FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S26FE_SETMASK
HW_ICLFENABLE1R_S27FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S27FE_SETMASK
HW_ICLFENABLE1R_S28FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S28FE_SETMASK
HW_ICLFENABLE1R_S29FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S29FE_SETMASK
HW_ICLFENABLE1R_S30FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S30FE_SETMASK
HW_ICLFENABLE1R_S31FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S31FE_SETMASK
HW_ICLFENABLE1R_S32FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S32FE_SETMASK
HW_ICLFENABLE1R_S33FE_CLRMASK           equ     ~HW_ICLFENABLE1R_S33FE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Observation Register 0 Register (HW_ICLOBSVZ0R) Bit Positions
;C Insert
;typedef union               /* Interrupt Collector Observation Register 0       */
;{
;    struct {
;        unsigned RQ     :7;
;        unsigned IVA    :7;
;        unsigned IVB    :7;
;    } B;
;    int I;
;} iclobsvz0_type;
;#define HW_ICLOBSVZ0R   (*(volatile iclobsvz0_type _X*) (HW_ICOLL_BASEADDR+15)) /* Interrupt Collector Observation Register 0   */
;endC

HW_ICLOBSVZ0R_RQ_BITPOS                 equ     0
HW_ICLOBSVZ0R_IVA_BITPOS                equ     7
HW_ICLOBSVZ0R_IVB_BITPOS                equ     14

HW_ICLOBSVZ0R_RQ_SETMASK                equ     $3F<<HW_ICLOBSVZ0R_RQ_BITPOS
HW_ICLOBSVZ0R_IVA_SETMASK               equ     $3F<<HW_ICLOBSVZ0R_IVA_BITPOS
HW_ICLOBSVZ0R_IVB_SETMASK               equ     $3F<<HW_ICLOBSVZ0R_IVB_BITPOS

HW_ICLOBSVZ0R_RQ_CLRMASK                equ     ~HW_ICLOBSVZ0R_RQ_SETMASK
HW_ICLOBSVZ0R_IVA_CLRMASK               equ     ~HW_ICLOBSVZ0R_IVA_SETMASK
HW_ICLOBSVZ0R_IVB_CLRMASK               equ     ~HW_ICLOBSVZ0R_IVB_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Observation Register 1 Register (HW_ICL1BSVZ0R) Bit Positions
HW_ICL1BSVZ0R_IVC_BITPOS                equ     0
HW_ICL1BSVZ0R_IVD_BITPOS                equ     7

HW_ICL1BSVZ0R_IVC_SETMASK               equ     $3F<<HW_ICL1BSVZ0R_IVC_BITPOS
HW_ICL1BSVZ0R_IVD_SETMASK               equ     $3F<<HW_ICL1BSVZ0R_IVD_BITPOS

HW_ICL1BSVZ0R_IVC_CLRMASK               equ     ~HW_ICL1BSVZ0R_IVC_SETMASK
HW_ICL1BSVZ0R_IVD_CLRMASK               equ     ~HW_ICL1BSVZ0R_IVD_SETMASK




;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Vectors
;///////////////////////////////////////////////////////////////////////////////
HW_IVECRESET                            equ     $0000           ; Reset Vector
HW_IVECSTERR                            equ     $0002           ; Stack Error
HW_IVECTRAC                             equ     $0004           ; Trace
HW_IVECSWI                              equ     $0006           ; SWI
HW_IVECIRQA                             equ     $0008           ; ~IRQA
HW_IVECIRQB                             equ     $000A           ; ~IRQB - BROWNOUT
HW_IVECERROR                            equ     $000C           ; Fatal Error
HW_IVECSPI                              equ     $000E           ; SPI
HW_IVECI2STXDE                          equ     $0010           ; I2S TX Data Empty
HW_IVECI2STXUF                          equ     $0012           ; I2S TX Underflow
HW_IVECI2SRXDF                          equ     $0014           ; I2S RX Data Full
HW_IVECI2SRXOF                          equ     $0016           ; I2S RX Overflow
;                                       equ     $0018           ; Error, nothing vectors here
;                                       equ     $001A           ; Error, nothing vectors here
;                                       equ     $001C           ; Error, nothing vectors here
;                                       equ     $001E           ; Non-Maskable Interrupt
HW_IVECGPIO1                            equ     $0020           ; GPIO1
HW_IVECGPIO2                            equ     $0022           ; GPIO2
HW_IVECGPIO0                            equ     $0024           ; GPIO0
HW_IVECTIMER0                           equ     $0026           ; TIMER0
HW_IVECTIMER1                           equ     $0028           ; TIMER1
HW_IVECTIMER2                           equ     $002A           ; TIMER2
;                                       equ     $002C           ; Error, nothing vectors here
;                                       equ     $002E           ; Error, nothing vectors here
HW_IVECI2CRXDR                          equ     $0030           ; I2C RX Data Ready
HW_IVECI2CRXOF                          equ     $0032           ; I2C RX Overflow
HW_IVECI2CTXDE                          equ     $0034           ; I2C TX Data Empty
HW_IVECI2CTXUF                          equ     $0036           ; I2C TX Underflow
HW_IVECILI                              equ     $0038           ; Illegal Instruction
;                                       equ     $003A           ; Error, nothing vectors here
HW_IVECDACE                             equ     $003C           ; DAC Empty ISR (DAC Request to Fill Buffer)
HW_IVECDACUF                            equ     $003E           ; DAC Underflow ISR
;                                       equ     $0040           ; Error, nothing vectors here
HW_IVECADCF                             equ     $0042           ; ADC Full ISR
HW_IVECADCOF                            equ     $0044           ; ADC Overflow ISR
;                                       equ     $0046           ; Error, nothing vectors here
HW_IVECTIMER3                           equ     $0048           ; TIMER3
HW_IVECGPIO3                            equ     $004A           ; GPIO3
HW_IVECSDRAM                            equ     $004C           ; SDRAM
;                                       equ     $004E           ; Error, nothing vectors here
HW_IVECVDD5VCONN                        equ     $0050           ; 5 volt power connected
HW_IVECUSBCTLR                          equ     $0052           ; USB Controller
HW_IVECUSBWAKEUP                        equ     $0054           ; USB Wakeup 
HW_IVECVDD5VDISC                        equ     $0056           ; 5 volt power disconnected
HW_IVECESPI                             equ     $0058           ; enhanced SPI
HW_IVECFILCO                            equ     $005A           ; filter coprocessor
HW_IVECLRADC1                           equ     $005C           ; low res ADC #1
HW_IVECRTCALARM                         equ     $005E           ; real time clock alarm
HW_IVECLRADC2                           equ     $0060           ; low res ADC #2
HW_IVECHWECC                            equ     $0062           ; flash hardware ECC
;                                       equ     $0064           ; Error, nothing vectors here
HW_IVECCDSYNC                           equ     $0066           ; CDSYNC Interrupt
HW_IVECCDSYNCEX                         equ     $0068           ; CDSYNC Exception
HW_IVECRS                               equ     $006A           ; RS
;                                       equ     $006C           ; Error, nothing vectors here
HW_IVECFD                               equ     $006E           ; Flash Done ISR
HW_IVECCF                               equ     $0070           ; CompactFlash ISR
HW_IVECSMTO                             equ     $0072           ; SmartMedia Timeout ISR
HW_IVECSMIP                             equ     $0074           ; SmartMedia Invalid Programming
HW_IVECCFNC                             equ     $0076           ; CompactFlash No Card ISR
HW_IVECCFSC                             equ     $0078           ; CompactFlash Status Change ISR
;                                       equ     $007A           ; Error, nothing vectors here
;                                       equ     $007C           ; Error, nothing vectors here
HW_IVECCDI                              equ     $007E           ; CDI

;C Insert
;/////////////////////////////////////////////////////////////////////////////////
;//  Interrupt Vectors
;/////////////////////////////////////////////////////////////////////////////////
;// macro to allow setting vectors from C. Hex const below is jsr opcode.
;#define VECTOR(address,isr) \
;    (*(volatile int _P *)(address))=0x0BF080; \
;    (*(volatile int _P *)(address+1))=(isr)
;
;endC    

;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Priority Register (HW_IPR) Bit Positions
HW_IPR_IRQA_BITPOS                      equ     0
HW_IPR_IRQA_ET_BITPOS                   equ     2
HW_IPR_IRQB_BITPOS                      equ     3
HW_IPR_IRQB_ET_BITPOS                   equ     5
HW_IPR_L0P_BITPOS                       equ     10
HW_IPR_L1P_BITPOS                       equ     12
HW_IPR_L2P_BITPOS                       equ     14
HW_IPR_L3P_BITPOS                       equ     16
HW_IPR_L4P_BITPOS                       equ     18
HW_IPR_L5P_BITPOS                       equ     20
HW_IPR_L6P_BITPOS                       equ     22

HW_IPR_LP_0_SETMASK                     equ     0   ; Interrupt Disabled
HW_IPR_LP_1_SETMASK                     equ     1   ; Interrupt Priority Level 0
HW_IPR_LP_2_SETMASK                     equ     2   ; Interrupt Priority Level 1
HW_IPR_LP_3_SETMASK                     equ     3   ; Interrupt Priority Level 2

HW_IPR_IRQA_SETMASK                     equ     3<<HW_IPR_IRQA_BITPOS
HW_IPR_IRQA_ET_SETMASK                  equ     1<<HW_IPR_IRQA_ET_BITPOS
HW_IPR_IRQB_SETMASK                     equ     3<<HW_IPR_IRQB_BITPOS
HW_IPR_IRQB_ET_SETMASK                  equ     1<<HW_IPR_IRQB_ET_BITPOS
HW_IPR_L0P_SETMASK                      equ     3<<HW_IPR_L0P_BITPOS
HW_IPR_L1P_SETMASK                      equ     3<<HW_IPR_L1P_BITPOS
HW_IPR_L2P_SETMASK                      equ     3<<HW_IPR_L2P_BITPOS
HW_IPR_L3P_SETMASK                      equ     3<<HW_IPR_L3P_BITPOS
HW_IPR_L4P_SETMASK                      equ     3<<HW_IPR_L4P_BITPOS
HW_IPR_L5P_SETMASK                      equ     3<<HW_IPR_L5P_BITPOS
HW_IPR_L6P_SETMASK                      equ     3<<HW_IPR_L6P_BITPOS

HW_IPR_IRQA_CLRMASK                     equ     ~HW_IPR_IRQA_SETMASK
HW_IPR_IRQA_ET_CLRMASK                  equ     ~HW_IPR_IRQA_ET_SETMASK
HW_IPR_IRQB_CLRMASK                     equ     ~HW_IPR_IRQB_SETMASK
HW_IPR_IRQB_ET_CLRMASK                  equ     ~HW_IPR_IRQB_ET_SETMASK
HW_IPR_L0P_CLRMASK                      equ     ~HW_IPR_L0P_SETMASK
HW_IPR_L1P_CLRMASK                      equ     ~HW_IPR_L1P_SETMASK
HW_IPR_L2P_CLRMASK                      equ     ~HW_IPR_L2P_SETMASK
HW_IPR_L3P_CLRMASK                      equ     ~HW_IPR_L3P_SETMASK
HW_IPR_L4P_CLRMASK                      equ     ~HW_IPR_L4P_SETMASK
HW_IPR_L5P_CLRMASK                      equ     ~HW_IPR_L5P_SETMASK
HW_IPR_L6P_CLRMASK                      equ     ~HW_IPR_L6P_SETMASK

;C Insert
;// Interrupt Priority register
;typedef union               
;{
;    struct {
;        unsigned int IRQAP  :2; /* IRQ A Priority: 00 disable, 01 10 11 enable  */
;        unsigned int IRQAT  :1; /* IRQ A Type: 0 level, 1 negative edge         */
;        unsigned int IRQBP  :2; /* IRQ B Priority: 00 disable, 01 10 11 enable  */
;        unsigned int IRQBT  :1; /* IRQ B Type: 0 level, 1 negative edge.        */
;        int                 :4; /* Reserved */
;        unsigned int L0P    :2; /* Interrupt Priority for priority 0 (SAI)      */
;        unsigned int L1P    :2; /* Interrupt Priority for priority 1 (IColl)    */
;        unsigned int L2P    :2; /* Interrupt Priority for priority 2 (IColl)    */
;        unsigned int L3P    :2; /* Interrupt Priority for priority 3 (IColl)    */
;        unsigned int L4P    :2; /* Interrupt Priority for priority 4 (I2C)      */
;        unsigned int L5P    :2; /* Interrupt Priority for priority 5 (SPI)      */
;        unsigned int L6P    :2; /* Interrupt Priority for priority 6 (IColl)    */
;    } B;
;
;    int I;
;
;} ipr_type;
;#define HW_IPR         (*(volatile ipr_type _X*) 0x00FFFF)
;endc

 ;*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
 ;
 ; The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
 ; to update the actual files. Only the defines needed to build SDK2.400 were added.

HW_IPR_L1P0_BITPOS          equ     12
HW_IPR_L2P0_BITPOS          equ     14
HW_IPR_L3P0_BITPOS          equ     16
HW_IPR_L6P0_BITPOS          equ     22

;///////////////////////////////////////////////////////////////////////////////
;  Interrupt Collector Steering 0 Register (HW_ICLSTEER0R) Bit Positions
HW_ICLSTEERR_SS_1                         equ     0
HW_ICLSTEERR_SS_2                         equ     1
HW_ICLSTEERR_SS_3                         equ     2
HW_ICLSTEERR_SS_6                         equ     3

    endif
