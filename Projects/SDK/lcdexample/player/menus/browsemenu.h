#ifndef BROWSEMENU_H
#define BROWSEMENU_H

#include "musiclib_ghdr.h"
#ifdef _FOLDER_BROWSE_
#define BROWSE_MAX_LEVEL    8
#else
#define BROWSE_MAX_LEVEL    3
#endif  // _FOLDER_BROWSE_

typedef struct _disp_year_list_arg
{
    int                 iHighLightedItem;
    FLASH_GROUP_NAME_T* browse_item_name;
    int                 num_of_item;
    int                 window_offset;
    BOOL                bHavePlayAll;
} DISP_YEAR_LIST_ARG_T;

typedef struct _browsemenu_init_arg
{
    INT     browse_type;
    INT     browse_lv;
    INT*    pbrowse_max_lv;
    INT*    num_of_item;
    BOOL*   bHaveNext;
} BROWSEMENU_INIT_ARG_T;

extern BOOL g_bDeleteBrowse;
extern BOOL bPlayVoice;
extern BOOL bFavoritesUpdate;


#endif  /* BROWSEMENU_H */
