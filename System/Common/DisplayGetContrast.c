#include "types.h"
#include "project.h"

#ifndef DEFAULT_CONTRAST
#define DEFAULT_CONTRAST 50
#endif

INT  g_iContrastPercentage=DEFAULT_CONTRAST;  // default contrast 50% if settings.dat is not present



WORD _reentrant DisplayGetContrast(void)
{
#ifdef CONTRAST_CALIBRATION    
    // this will return the raw contrast value between 0-3F being sent to the lcd
    // this is useful to display when calibrating the visible contrast range
    return ((LCD_MIN_CONTRAST+ ((g_iContrastPercentage*(LCD_MAX_CONTRAST-LCD_MIN_CONTRAST))/100)));
#else   
    // this will return the % based on the range defined in project.inc
    return (g_iContrastPercentage);
#endif
}


#ifdef STMP_BUILD_PLAYER
// In mass storage class we do not save any settings
#pragma asm
    nolist
        include "sysmacro.asm"
    list

    extern y:Fg_iContrastPercentage

    org y,"Player_settings_Y":

    SaveRange Fg_iContrastPercentage,1

#pragma endasm
#endif