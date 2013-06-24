    if !(@def(RESGSSPARE_INC))
RESGSSPARE_INC equ 1

HW_SPARER_BASEADDR             			equ     ($FA16)

;ASM Only
HW_SPARER                               equ     HW_SPARER_BASEADDR     ; Spare register
;endasm

HW_SPARER_I2SS_BITPOS                   equ     0
HW_SPARER_USB_SELECT_BITPOS             equ     1
HW_SPARER_USB_PLUGIN_BITPOS             equ     8
HW_SPARER_PSWITCH_BITPOS                equ     9

HW_SPARER_I2SS_SETMASK                  equ     1<<HW_SPARER_I2SS_BITPOS
HW_SPARER_USB_SELECT_SETMASK            equ     1<<HW_SPARER_USB_SELECT_BITPOS

HW_SPARER_I2SS_CLRMASK                  equ     ~HW_SPARER_I2SS_SETMASK
HW_SPARER_USB_SELECT_CLRMASK            equ     ~HW_SPARER_USB_SELECT_SETMASK



;C Insert
;typedef union               
;{
;    struct {
;        int      I2SS       :1;     
;        int      USBSELECT  :1;     
;        unsigned            :6;
;        int      USBPLUGIN  :1;     
;        int      PSWITCH    :1;     
;    } B;
;
;    int I;
;
;} spare_type;
;#define HW_SPARER      (*(volatile spare_type _X*) (HW_SPARER_BASEADDR))
;endc



    endif 