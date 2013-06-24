#ifndef _WOW_MENU_H
#define _WOW_MENU_H

// menus in wow submenu
// defines order of selection
#define WOWMENU_FIRST       0
#define MENU_BRIGHTNESS		0
#define MENU_TRUBASS        1
#define MENU_WOWVOLUME      2
#define WOWMENU_EXIT        3
#define WOWMENU_LAST        3
#define WOWMENU_COUNT  WOWMENU_LAST+1

// Menus in wow brightness menu
// match enum TBrightness
#define WOWBRIGHT_FIRST     0
#define WOWBRIGHT_LOW     	0
#define WOWBRIGHT_MID       1
#define WOWBRIGHT_HIGH      2
#define WOWBRIGHT_LAST      2
#define WOWBRIGHT_COUNT  WOWBRIGHT_LAST+1

// combo menu function
#define TRUBASS             0
#define VOLUME              1

#define WOWTRUBASS_MIN      0 
#define WOWTRUBASS_MAX      0x7fffff 
#define WOWTRUBASS_STEPSIZE (WOWTRUBASS_MAX/10)

#define WOWVOLUME_MIN       0 
#define WOWVOLUME_MAX       0x7fffff
#define WOWVOLUME_STEPSIZE (WOWVOLUME_MAX/10)

#define DISABLE             0
#define ENABLE              1

// codebank callable
int _reentrant WowMenu(int a, int b, int *pPtr);
int _reentrant WowInit(int iUnused1,int iUnused2, int *pPtr);
int _reentrant WowSetProperties(int iUnused1, int iUnused2, int *pPtr);
// NOT codebank callable
int _reentrant WOWBrightnessMenu (void);
int _reentrant NextBrightness(int iDirection);
int _reentrant WOWControl (int type);
extern _P int _lc_u_e_wow_ctrl_overlay_P[];
extern _P int _lc_u_b_wow_ctrl_overlay_P[];
extern _Y int _lc_u_e_wow_coeff_Y[];
extern _Y int _lc_u_b_wow_coeff_Y[];
extern _P int _lc_u_e_wow_overlay_P[];
extern _P int _lc_u_b_wow_overlay_P[];

#endif