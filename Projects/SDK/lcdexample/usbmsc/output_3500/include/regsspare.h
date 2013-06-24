#if !(defined(RESGSSPARE_INC))
#define RESGSSPARE_INC 1

#define HW_SPARER_BASEADDR (0xFA16)


#define HW_SPARER_I2SS_BITPOS 0
#define HW_SPARER_USB_SELECT_BITPOS 1
#define HW_SPARER_USB_PLUGIN_BITPOS 8
#define HW_SPARER_PSWITCH_BITPOS 9

#define HW_SPARER_I2SS_SETMASK 1<<HW_SPARER_I2SS_BITPOS
#define HW_SPARER_USB_SELECT_SETMASK 1<<HW_SPARER_USB_SELECT_BITPOS

#define HW_SPARER_I2SS_CLRMASK ~(WORD)HW_SPARER_I2SS_SETMASK
#define HW_SPARER_USB_SELECT_CLRMASK ~(WORD)HW_SPARER_USB_SELECT_SETMASK



typedef union               
{
    struct {
        int      I2SS       :1;     
        int      USBSELECT  :1;     
        unsigned            :6;
        int      USBPLUGIN  :1;     
        int      PSWITCH    :1;     
    } B;

    int I;

} spare_type;
#define HW_SPARER      (*(volatile spare_type _X*) (HW_SPARER_BASEADDR))



#endif
