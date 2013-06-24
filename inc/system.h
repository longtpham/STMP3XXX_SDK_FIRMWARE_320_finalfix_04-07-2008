#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "project.h"

//This H file includes common definitions and references that are used throughout the different projects.

#ifndef SCRATCH_USER_Y_SIZE
#define SCRATCH_USER_Y_SIZE 512
#endif

#ifndef SCRATCH_USER_X_SIZE
#define SCRATCH_USER_X_SIZE 512
#endif

extern SyncObject g_Sync_UserScratchY;
extern _Y WORD g_wUserScratchY[SCRATCH_USER_Y_SIZE];

extern SyncObject g_Sync_UserScratchX;
extern _X WORD g_wUserScratchX[SCRATCH_USER_X_SIZE];

#endif