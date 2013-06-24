#ifndef PL5_BROWSEMENU_H
#define PL5_BROWSEMENU_H


#define MENU_ITEM_HEIGHT            8
#define BROWSE_VIEW_X_POS           0
#define BROWSE_VIEW_Y_POS           (MENU_ITEM_HEIGHT*2)
#define BROWSE_TITLE_X_POS          BROWSE_VIEW_X_POS
#define BROWSE_TITLE_Y_POS          BROWSE_VIEW_Y_POS
#define BROWSE_PAGE_X_POS           BROWSE_VIEW_X_POS
#define BROWSE_PAGE_Y_POS           (BROWSE_TITLE_Y_POS+MENU_ITEM_HEIGHT)


int _reentrant BrowseMenu(int a, int b, int *pPtr);

void _reentrant DisplayLine (int x, int y, PL5_BR_VIEW * view, PL5_BR_QUEUE * queue, int offset);
void _reentrant DisplayPage(int x, int y, PL5_BROWSE * browse);
void _reentrant DisplayViewTitle(int x, int y, PL5_BROWSE * browse);
void _reentrant DisplayView(int x, int y, PL5_BROWSE * browse);
_reentrant BOOL InValidRange(PL5_BR_QUEUE * queue, int offset);
void _reentrant BrowseView_Down(PL5_BROWSE* browse);
void _reentrant BrowseView_Up(PL5_BROWSE * browse);
#endif //PL5_BROWSEMENU_H