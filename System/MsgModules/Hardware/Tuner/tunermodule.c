/*//////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel Inc. 2003
// Filename: tunermodule.c
// Description: This contains the Executive module's two entry points for the FM tuner. 
// This includes the init function and the main FM tuner event processing function.
///////////////////////////////////////////////////////////////////////////////*/
#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "project.h"
#include "tunerdriver.h"


#pragma optimize 1

/* global variables */
/* variable timer signal depending on tuner module. in milliseconds */ 
#ifdef TUNER_STFM1000
int _X g_iTunerProcessIntervalMs = 50;	//
#else
#ifdef TUNER_LV24000 	
int _X g_iTunerProcessIntervalMs = 10;   //
#else
int _X g_iTunerProcessIntervalMs = 30;   //
#endif
#endif
struct ModuleEntry *g_pTunerModuleEntry=NULL;

#ifdef TUNER_STFM1000 
extern BOOL _X g_bEnable100msTimer;     //
INT         error=TUNER_SUCCESS;
#endif

#ifdef TUNER_STFM1000
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          bIsTunerModulePowered          
//
//   Type:          Function
//
//   Description:   Check if tuning is on or off
//
//   Inputs:        none
//
//   Outputs:       TRUE: power-on/FALSE: power-off
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant BOOL bIsTunerModulePowered(void)
{
    return ( !(g_wFMTunerError.m_bPOWERED_OFF));
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetSensitivity          
//
//   Type:          Function
//
//   Description:   Set tuner sensitivity, i.e.RSSI after decoded, the value is 0-70 
//
//   Inputs:        iSensitivity: sensitivity level from 0-100
//
//   Outputs:       return iTuningStatus 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT TunerDriverSetSensitivity(INT iSensitivity )
{
    //s_iFMTunerSensitivity =  (iSensitivity*70)/100;	   
    //StfmTuneData.sd_ctl_TuneRssiTh_w = s_iFMTunerSensitivity;    
    return TUNER_SUCCESS;
}
#endif
//******************************************************************************
// Description: Exercises the tuner driver's init function
// Inputs:      Pointer to the tuner module's ModuleTableEntry
// Outputs:     int module's wait value to store in ModuleTable tell what events 
//              to call module.
// Notes:
//******************************************************************************
int _reentrant TunerModuleInit(struct ModuleEntry *pEntry)
{
    union WaitMask returnMask;  

    g_pTunerModuleEntry = pEntry;
    TunerDriverInit();

    //Tell ModuleTableEntry.wait to call this module's process function next when it has a msg    
    returnMask.I=0x000000 | 2;/* Initialize it to signal us if we have a message */
    return returnMask.I;
}


#if FM_WATCHDOG_ENABLE
#define FM_WATCHDOG_INTERVAL_MS 300
unsigned long ulFMStateMachineCheckTimeMs=0; // must be 0 initially
#endif
/*//////////////////////////////////////////////////////////////////////////////
// Function:    TunerModuleProcessQueue()
// Description: Handle any timer messages and refresh this module's callback time. 
// Inputs:      (union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
// Outputs:     int returnMask (requests next call on these events)
// Notes:       FM watchdog inserted to catch and remove invalid FM state-machine states.
// See FM_WATCHDOG_ENABLE in project.inc which auto-generates project.h
//////////////////////////////////////////////////////////////////////////////*/
extern INT _X iBattlevel;
extern BOOL g_bEraseComplete;
extern WORD _X s_wTimeCount;
static INT LastBattLevel;
static INT battlevel;
_reentrant WORD IsSystemDriveDirty(void);
_reentrant WORD SysBatteryGetLevel(void);

int _reentrant TunerModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    union WaitMask returnMask; 
    struct CMessage  Msg;
    unsigned long    ulTimeout;
#ifndef TUNER_STFM1000 
    INT              error=TUNER_SUCCESS;
#endif
    int i;

    returnMask.I = 0x000000 | 2;/*Initialize it to signal us next if we have a message */
#if FM_WATCHDOG_ENABLE
    if( SysGetCurrentTime() > ulFMStateMachineCheckTimeMs )
    {   if( (g_wFMTunerError.m_bTUNER_BUSY == 0) && 
             (g_wFMTunerError.m_bSEARCHING_UP || g_wFMTunerError.m_bSEARCHING_DOWN || g_wFMTunerError.m_bSCANNING_STATION))
        {   
            g_wFMTunerError.m_bSEARCHING_UP     = 0; /* unlock the deadlock state */
            g_wFMTunerError.m_bSEARCHING_DOWN   = 0;
            g_wFMTunerError.m_bSCANNING_STATION = 0;
        }
        ulFMStateMachineCheckTimeMs = SysGetCurrentTime() + FM_WATCHDOG_INTERVAL_MS;
    }
#endif
	/* Don't cut off the module timer if the tuner is searching */
	if ((g_wFMTunerError.m_bSEARCHING_UP==TRUE)||(g_wFMTunerError.m_bSEARCHING_DOWN==TRUE))
    {
	   returnMask.B.m_bTimer=TRUE;
    }

    if(Signals.B.m_bTimer)
    {
        // Monitor flash dirty
        if ((!g_bEraseComplete) && (IsSystemDriveDirty()))
        {
            SysPostMessage( 2, MENU_TUNER_DIRTY_FLASH );
        }
        else
        {
            if(g_bEraseComplete && (IsSystemDriveDirty()))
            {
                SysPostMessage( 2, MENU_TUNER_DIRTY_FLASH );
            }
        }

        // Monitor Battery
        if( s_wTimeCount++ >= 20 )
        {
            if( (iBattlevel=((iBattlevel+SysBatteryGetLevel())/2)) < 10 )
            {
                battlevel = 0;
            }        
            else if( iBattlevel >= 100 )
            {
                battlevel = 10;
            }
            else 
            {
                battlevel = iBattlevel/10;
            }

            if ( battlevel != LastBattLevel )
            {
                LastBattLevel = battlevel;
                SysPostMessage( 2, MENU_BATTERY_CHNG );
            }

            s_wTimeCount = 0;
        }

#ifdef TUNER_STFM1000
         if((!(Signals.B.m_bMessage)) && ( g_wFMTunerError.m_bTUNER_BUSY != TRUE) &&(g_bEnable100msTimer) &&(g_wFMTunerError.m_bSCANNING_STATION==FALSE) && (error != E_TUNER_BUSY))
         {
            if(bIsTunerModulePowered())
			{
               	error = TunerDriverGetTunerstate();
			}
         
         }
        // The timer could be E_TUNER_BUSY by tuning or optimizing channel and monitoring
        // the signal quality. In the last two cases, the tuner should be not in busy state
        // In those cases, if a message comes, we should ignor timer and handle the message
		else if(((!(Signals.B.m_bMessage)) || ( g_wFMTunerError.m_bTUNER_BUSY == TRUE))/* && (error == E_TUNER_BUSY)*/)
        {
#endif          

            error = TunerGetStatus();

	        // if there is message on the queue, throw it away
		    if((Signals.B.m_bMessage) && 
            (g_wFMTunerError.m_bSEARCHING_UP==TRUE) || 
            (g_wFMTunerError.m_bSEARCHING_DOWN==TRUE) ||
            (g_wFMTunerError.m_bSCANNING_STATION == TRUE))
		    {
                while(GetMessage(pQueue,&Msg))		// ignore all other messages besides the power control message
		        {
        	        if (Msg.m_uMsg[0]==TUNER_POWER_OFF)
			        {
				        g_wFMTunerError.m_bSEARCHING_UP = FALSE;
				        g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
				        g_wFMTunerError.m_bSCANNING_STATION = FALSE;
				        error = TunerDriverSetTunerStandby(TRUE);
			        }
                }
            }
#ifdef TUNER_STFM1000
         }
#endif
         		
    }
#ifdef TUNER_STFM1000
    if(Signals.B.m_bMessage)
#else
    else if(Signals.B.m_bMessage)
#endif
    {
        while(GetMessage(pQueue,&Msg))
        {
#ifdef TUNER_STFM1000 
            if(error != E_TUNER_BUSY)
            {
                /* default error is tuner powered down only if we have no other message
                   which enables the timer - The fix for defect 9204 */
                error = E_TUNER_POWERED_DOWN; 
            }
#else
                error = E_TUNER_POWERED_DOWN; // default error is tuner powered down
#endif

            if( Msg.m_uMsg[0] == TUNER_POWER_ON )
            {
                error = TunerDriverSetTunerStandby(FALSE);
            }
            else
            {   
                if(bIsTunerModulePowered())             
                {
                    switch(Msg.m_uMsg[0])
                    {
                        case TUNER_TUNE_FREQUENCY:
                            error = TunerDriverTuneToFrequency(Msg.m_uMsg[1]);
                            break;
                        case TUNER_TUNE_MANUAL_UP:
                            error = TunerDriverIncrementUp();
                            break;
                        case TUNER_TUNE_MANUAL_DOWN:
                            error = TunerDriverIncrementDown();
                            break;
                        case TUNER_TUNE_SEARCH_UP:
                            error = TunerDriverSearchUp();
                            break;
                        case TUNER_TUNE_SEARCH_DOWN:
                            error = TunerDriverSearchDown();
                            break;
                        case TUNER_PRESET_STATION:
                            error = TunerDriverScanStations();
                            break;
                        case TUNER_TUNE_TO_PRESET:
                            error = TunerDriverSetToPresetStations(Msg.m_uMsg[1]);
                            break;
                        case TUNER_SET_PRESET:
                            error = TunerDriverSetPresetStations(Msg.m_uMsg[1]);
                            break;
                        case TUNER_ERASE_PRESET:
                            error = TunerDriverErasePresetStations(Msg.m_uMsg[1]);
                            break;
                        case TUNER_FORCED_MONO:
                            error = TunerDriverSetMonoStereoMode(Msg.m_uMsg[1]==TRUE?TUNER_MODE_MONO:TUNER_MODE_STEREO);
                            break;
                        case TUNER_RESET:
                            break;
                        case TUNER_POWER_OFF:
                            error = TunerDriverSetTunerStandby(TRUE);
                            break;
                        case TUNER_SET_SENSITIVITY:
                            error = TunerDriverSetSensitivity(Msg.m_uMsg[1]);
                            break;
                        case TUNER_GET_STATES:
                            if(g_wFMTunerError.m_bTUNER_BUSY==FALSE)
                            {
                                error = TunerDriverGetTunerstate();
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    if(error == E_TUNER_BUSY)
    {
        returnMask.B.m_bTimer=TRUE;
        ulTimeout= SysGetCurrentTime() + g_iTunerProcessIntervalMs;
        g_pTunerModuleEntry->m_uTimeOutHigh = ulTimeout>>24;
        g_pTunerModuleEntry->m_uTimeOutLow = ulTimeout&0xffffff;
    }
#ifdef TUNER_STFM1000
    else
    {
        // we only trigger 100ms timer when tuner is idle
        if(/*(!(Signals.B.m_bMessage)) && ( g_wFMTunerError.m_bTUNER_BUSY != TRUE) &&*/(g_bEnable100msTimer)) 
        {
            returnMask.B.m_bTimer=TRUE;
            ulTimeout= SysGetCurrentTime() + 100;
            g_pTunerModuleEntry->m_uTimeOutHigh = ulTimeout>>24;
            g_pTunerModuleEntry->m_uTimeOutLow = ulTimeout&0xffffff;
        }   
    }
#endif
    return returnMask.I;
}

