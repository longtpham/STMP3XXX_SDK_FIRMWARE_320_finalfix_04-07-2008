#include "types.h"
#include "exec.h"
#include "messages.h"
#include "resource.h"
#include "playlist5.h"

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern int g_PL5_BrowseTmpBfrIdx;

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant int PL5_BR_CdBrowsing_Fast (PL5_BROWSE * browse);
_reentrant int PL5_Chdir (_packed char *filepath, INT RecordNumber);
_reentrant void PL5_BR_Do_Search (PL5_BROWSE* browse);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
struct ModuleEntry * g_pPL5BrowseModuleEntry;
PL5_BROWSE * g_PL5_browse;
int g_PL5_BR_LastState;

////////////////////////////////////////////////////////////
// Module Functions
// DON'T MOVE BELOW TWO MODULE FUNCTIONS
// The first must be module init function
// The second must be module message queue process function
////////////////////////////////////////////////////////////
int _reentrant PL5BrowseModuleInit(struct ModuleEntry *pModuleTableEntry)
{
    union WaitMask returnMask;

    g_PL5_browse = NULL;
    g_pPL5BrowseModuleEntry = pModuleTableEntry;

    returnMask.I=0x000000 | 2;          // EVENT_MESSAGE
    return returnMask.I;
}


int _reentrant PL5BrowseModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    union WaitMask returnMask;
    struct CMessage  Msg;
    LONG ulTimeout;
    BOOL bStop = FALSE;
    BOOL pause = TRUE;

    returnMask.I=0x000000 | 2;          //Initialize it to signal us if we have a message

    //handle incoming messages
    if(Signals.B.m_bMessage)
    {
        while(GetMessage(pQueue,&Msg))
        {
            if ((g_PL5_browse = (PL5_BROWSE*)(Msg.m_uMsg[1])) == NULL)
                break;

            switch(Msg.m_uMsg[0])
            {
                case PL5_BR_MSG_RESET:
                    g_PL5_browse = NULL;
                    g_PL5_BrowseTmpBfrIdx = 0;
                    break;
                case PL5_BR_MSG_CDBROWSING:
                    *((int*)Msg.m_uMsg[2]) = PL5_BR_CdBrowsing_Fast(g_PL5_browse);
                    break;
                case PL5_BR_MSG_START:
                    g_PL5_browse->search->status = PL5_STATE_RUN;
                    PL5_BR_Do_Search(g_PL5_browse);
                    break;
                case PL5_BR_MSG_STOP:
                    g_PL5_browse->search->status = PL5_STATE_IDLE;
                    break;
                case PL5_BR_MSG_PAUSE:
                    g_PL5_BR_LastState = g_PL5_browse->search->status;
                    g_PL5_browse->search->status = PL5_STATE_PAUSE;
                    break;
                case PL5_BR_MSG_RESUME:
                    if (g_PL5_browse->search->status == PL5_STATE_PAUSE)
                    {
                        g_PL5_browse->search->status = g_PL5_BR_LastState;
                    }
                    break;
                case PL5_BR_MSG_DELETE:

                    break;
                default:
                    __asm("debug");         // Unknown message
                    g_PL5_browse = NULL;
                    return returnMask.I;
            }
        }
    }
    else if(Signals.B.m_bTimer)
    {
        if ((g_PL5_browse != NULL) && (g_PL5_browse->search->status == PL5_STATE_RUN))
        {
            PL5_BR_Do_Search(g_PL5_browse);
        }
    }


    if ((g_PL5_browse != NULL) && (g_PL5_browse->search->status == PL5_STATE_RUN))
    {
        // Search has not complete, we trigger a timer to continue search next time.
        returnMask.B.m_bTimer=TRUE;
        ulTimeout= SysGetCurrentTime() + PL5_BR_SEARCH_INTERVAL;
        g_pPL5BrowseModuleEntry->m_uTimeOutHigh = ulTimeout>>24;
        g_pPL5BrowseModuleEntry->m_uTimeOutLow = ulTimeout&0xffffff;
    }


    return returnMask.I;
}