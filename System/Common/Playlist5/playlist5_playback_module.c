#include "types.h"
#include "exec.h"
#include "messages.h"
#include "playlist5.h"


////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern int g_PL5_PlaybackTmpBfrIdx;

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant INT PL5_PL_CdSearching_Fast(void);
_reentrant void PL5_PL_Do_Search (void);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
struct ModuleEntry * g_pPL5PlaybackModuleEntry;
int g_PL5_PL_LastState;
BOOL g_PL5_PL_bNotCount = FALSE;

////////////////////////////////////////////////////////////
// Module Functions
// DON'T MOVE BELOW TWO MODULE FUNCTIONS
// The first must be module init function
// The second must be module message queue process function
////////////////////////////////////////////////////////////
int _reentrant PL5PlaybackModuleInit(struct ModuleEntry *pModuleTableEntry)
{
    union WaitMask returnMask;

    g_pPL5PlaybackModuleEntry = pModuleTableEntry;

    g_PL5_PL_search.status = PL5_STATE_IDLE;

    returnMask.I=0x000000 | 2; // EVENT_MESSAGE
    return returnMask.I;
}


int _reentrant PL5PlaybackModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    union WaitMask returnMask;
    struct CMessage  Msg;
    LONG ulTimeout;
    int offset, i;
    PL5_PL_FOLDER tmp;

    returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

    //handle incoming messages
    if(Signals.B.m_bMessage)
    {
        while(GetMessage(pQueue,&Msg))
        {
            switch(Msg.m_uMsg[0])
            {
                case PL5_PL_MSG_RESET:
                    g_PL5_PL_search.status = PL5_STATE_IDLE;
                    g_PL5_PlaybackTmpBfrIdx = 0;
                    break;
                case PL5_PL_MSG_CDSEARCHING:
                    *((int*)Msg.m_uMsg[1]) = PL5_PL_CdSearching_Fast();
                    break;
                case PL5_PL_MSG_START:
                    g_PL5_PL_search.status = PL5_STATE_RUN;
                    PL5_PL_Do_Search();
                    break;
                case PL5_PL_MSG_STOP:
                    g_PL5_PL_search.status = PL5_STATE_IDLE;
                    break;
                case PL5_PL_MSG_PAUSE:
                    g_PL5_PL_LastState = g_PL5_PL_search.status;
                    g_PL5_PL_search.status = PL5_STATE_PAUSE;
                    break;
                case PL5_PL_MSG_RESUME:
                    if (g_PL5_PL_search.status == PL5_STATE_PAUSE)
                    {
                        g_PL5_PL_search.status = g_PL5_PL_LastState;
                    }

                    break;
                case PL5_PL_MSG_DELETE:

                    break;
                case PL5_PL_MSG_PLAY:

                    break;
                case PL5_PL_MSG_DISCNT:
                    g_PL5_PL_bNotCount = TRUE;
                    break;
                case PL5_PL_MSG_ENACNT:
                    g_PL5_PL_bNotCount = FALSE;
                    break;
                default:
                    __asm("debug");    // Unknown message
                    return returnMask.I;
            }
        }
    }
    else if(Signals.B.m_bTimer)
    {
         if ((g_PL5_PL_search.status == PL5_STATE_RUN) &&
             ((g_PL5_PL_search.phase != PL5_COUNT) || ((g_PL5_PL_search.phase == PL5_COUNT) && (!g_PL5_PL_bNotCount))))
            PL5_PL_Do_Search();
    }

    if ((g_PL5_PL_search.status == PL5_STATE_RUN))
    {
        // we only trigger 100ms timer when search status is not idle or total count is not finished
        returnMask.B.m_bTimer=TRUE;
        ulTimeout= SysGetCurrentTime() + PL5_PL_SEARCH_INTERVAL;
        g_pPL5PlaybackModuleEntry->m_uTimeOutHigh = ulTimeout>>24;
        g_pPL5PlaybackModuleEntry->m_uTimeOutLow = ulTimeout&0xffffff;
    }

    return returnMask.I;
}