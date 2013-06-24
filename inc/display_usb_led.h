#ifndef _DISPLAY_USB_LED_H
#define _DISPLAY_USB_LED_H

#ifndef USB_LED_GPDO
#define USB_LED_GPDO            HW_GP0DOR.B.B7
#define USB_LED_GPDOE           HW_GP0DOER.B.B7
#define USB_LED_GPEN            HW_GP0ENR.B.B7
#define USB_LED_GPPWR           HW_GP0PWR.B.B7
#endif 

#ifndef EXTRA_LED_GPDO
#define EXTRA_LED_GPDO          HW_GP0DOR.B.B5
#define EXTRA_LED_GPDOE         HW_GP0DOER.B.B5
#define EXTRA_LED_GPEN          HW_GP0ENR.B.B5
#define EXTRA_LED_GPPWR         HW_GP0PWR.B.B5
#endif

void USBLedInit(void);
void USBLedState(int wState);


#endif