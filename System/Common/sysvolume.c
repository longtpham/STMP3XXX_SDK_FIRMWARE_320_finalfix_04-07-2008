

////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2001
//
// File : syscolume.c
// Description : Multi-stage volume controls with EQ Bias adjustment
// Updated 6.20.2002 for SDK 2.105 to add support for a line-in source
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////


// All of the various volume control parameters are defined in project.inc
#include "project.h"
// The Volume Bias from the EQ is defined in system.h
#include "sysmem.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "sysvolume.h"


#pragma optimize 1

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
// Globals

int g_iUserVolume = DEFAULT_VOLUME;                    // Global User Volume
int g_iVolumeMode = VOLUME_MODE_DAC;                   // Global Volume Adjustment Mode 
int g_iVolumeSteps;                                    // Global number of Volume Steps; dependant on volume mode

// Statics
int g_iStage1Switch, g_iStage2Switch, g_iStage3Switch; // Switchover points for each stage; dependant on volume mode
int g_iInternalVolumeMode = -1;                              // Internal volume mode used by the volume controls
int g_InputMinVol, g_InputNomVol, g_InputMaxVol;        // Variable parameters that change based on the Volume Mode
int g_NumInputAttSteps;

#ifdef FUNCLET
    #pragma asm
        extern SysCallFunclet

        ;
        ; Force the org to P so that the resource number appears before the 
        ; code being called, just like a funclet expects.
        ; WARNING this must match the default section for this file!
        ;
        org p,".ptextsysvolume":
        dc      RSRC_FUNCLET_SYSUPDATEVOLUME
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UpdateVolume
//
//   Type:          Function
//
//   Description:   Maps the linear volume into messages for the HW registers.
//                  Also adds g_Volume Bias to the linear volume for PCM autogain
//
//   Inputs:        none
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
void _reentrant SysUpdateVolume_Funclet(void)
#else
void _reentrant SysUpdateVolume(void)
#endif
{
    int iVolume;
    int iMasterVal;
    int iInputVal;
    int temp;
    int iScaledVolumeBias;
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!! CAUTION !!!!!! Since using SysWaitOnEvent() this case can ONLY be executed from a menu
    // !!!!!! the menu manager must be loaded in memory for this function to work
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SysWaitOnEvent(0,0,1);                        // Give the mixer time to empty its queue

    // Check to see if the volume mode has changed
    if (g_iVolumeMode != g_iInternalVolumeMode)
    {
        
        // Set the new volume mode.
        g_iInternalVolumeMode = g_iVolumeMode;
        
        // Adjust the multi-stage switchover points
        switch(g_iVolumeMode)
        {
        case VOLUME_MODE_DAC:
            // Set the stage switchover points
            // End of stage 1
            g_iStage1Switch = NUM_DAC_ATT_STEPS;
            // End of Stage 2
            g_iStage2Switch = NUM_DAC_ATT_STEPS + NUM_MSTR_ATT_STEPS;

            // Set the maximum number of steps
            g_iVolumeSteps = (NUM_DAC_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_DAC_GAIN_STEPS+1);

            // Set the Mixer Input parameters
            g_InputMinVol = MIX_DAC_MIN_VOL;
            g_InputNomVol = MIX_DAC_NOM_VOL;
            g_InputMaxVol = MIX_DAC_MAX_VOL;
            g_NumInputAttSteps = NUM_DAC_ATT_STEPS;

            SysPostMessage(2,MIXER_LINE_MUTE);              // Mute the Line Input            
            SysPostMessage(3,MIXER_POWER_DOWN,MIXER_OFF);   // Mixer Off/ Bypass Mode on
            SysPostMessage(2,MIXER_DAC_UNMUTE);             // Unmute the DAC

            break;

        case VOLUME_MODE_LINE1:
            // Set the stage switchover points
            // End of stage 1
            g_iStage1Switch = NUM_LINE_ATT_STEPS;
            // End of Stage 2
            g_iStage2Switch = NUM_LINE_ATT_STEPS + NUM_MSTR_ATT_STEPS;

            // Set the maximum number of steps
            g_iVolumeSteps = (NUM_LINE_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_LINE_GAIN_STEPS+1);
            
            // Set the Mixer Input parameters
            g_InputMinVol = MIX_LINE_MIN_VOL;
            g_InputNomVol = MIX_LINE_NOM_VOL;
            g_InputMaxVol = MIX_LINE_MAX_VOL;
            g_NumInputAttSteps = NUM_LINE_ATT_STEPS;
            // Change to Line1 Input Mode (Mixer on, DAC Mute)
            HW_MIXPWRDNR.B.PR2 = 0;                         // Mixer On
            SysPostMessage(2,MIXER_LINE_UNMUTE);            // Unmute the Line In
            SysPostMessage(2,MIXER_DAC_MUTE);               // Mute the DAC
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // !!!!!! CAUTION !!!!!! Since using SysWaitOnEvent() this case can ONLY be executed from a menu
            // !!!!!! the menu manager must be loaded in memory for this function to work
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            SysWaitOnEvent(0,0,100);                        // Wait for DAC to Mute
            SysPostMessage(3,MIXER_MASTER_FADE_IN,10);      // Fade in Master Volume
            break;
#ifdef FMTUNER_ON_LINE2_IN
        case VOLUME_MODE_LINE2:
            // Set the stage switchover points
            // End of stage 1
            g_iStage1Switch = NUM_FM_ATT_STEPS;
            // End of Stage 2
            g_iStage2Switch = NUM_FM_ATT_STEPS + NUM_MSTR_ATT_STEPS;

            // Set the maximum number of steps
            g_iVolumeSteps = (NUM_FM_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_FM_GAIN_STEPS+1);
            
            // Set the Mixer Input parameters
            g_InputMinVol = MIX_FM_MIN_VOL;
            g_InputNomVol = MIX_FM_NOM_VOL;
            g_InputMaxVol = MIX_FM_MAX_VOL;
            g_NumInputAttSteps = NUM_FM_ATT_STEPS;
            // Change to Line1 Input Mode (Mixer on, DAC Mute)
            HW_MIXPWRDNR.B.PR2 = 0;                         // Mixer On
            SysPostMessage(2,MIXER_FM_UNMUTE);            // Unmute the Line In
            SysPostMessage(2,MIXER_DAC_MUTE);               // Mute the DAC
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // !!!!!! CAUTION !!!!!! Since using SysWaitOnEvent() this case can ONLY be executed from a menu
            // !!!!!! the menu manager must be loaded in memory for this function to work
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            SysWaitOnEvent(0,0,100);                        // Wait for DAC to Mute
            SysPostMessage(3,MIXER_MASTER_FADE_IN,10);      // Fade in Master Volume
        break;
#endif
        }
    }

    
    // Check to see if the requested volume is out of bounds (high or low)
    if(g_iUserVolume>=g_iVolumeSteps)
        g_iUserVolume = g_iVolumeSteps-1;

    if(g_iUserVolume<0)
        g_iUserVolume = 0;
    
    // Convert the Volume Bias from 0.5dB/step to 1.5dB/step
    iScaledVolumeBias = g_VolumeBias / 3;
    
    // Add the User Volume to the EQ's Normalized Volume Bias, make sure it's within range
    iVolume = g_iUserVolume + iScaledVolumeBias;
    if(iVolume>=g_iVolumeSteps){
        iVolume = g_iVolumeSteps-1;                        // Set the main volume to max      
    }

    //Stage 0 - Mute
    if(iVolume == 0)
    {
        iMasterVal = 0x9F1F;                                // Master at Minimum and Muted
        iInputVal    = ((g_InputMinVol << 8 )| g_InputMinVol);  // Mixer input volume at Minimum
    }

    // Stage 1  - Hold Master at minimum and set Mixer Input attenuation
    else if(iVolume <= g_iStage1Switch)
    {
        iMasterVal = 0x1F1F;                                
        temp       = g_InputMinVol - iVolume +1;          // Mixer Input volume nibble
        iInputVal    = ((temp << 8) | temp);
    
    }

    // Stage 2 - Hold Mixer Input at Nominal and set Master attenuation
    else if (iVolume <= g_iStage2Switch ) // (NUM_DAC_ATT_STEPS + NUM_MSTR_ATT_STEPS))
    {
        iInputVal     = ((g_InputNomVol << 8) | g_InputNomVol);                       
        temp        = 0x1F - (iVolume - (NUM_DAC_ATT_STEPS));
        iMasterVal  = ((temp << 8 ) | temp);
    }
 
    // Stage 3 - Hold Master at Max and set DAC Gain
    else
    {
        iMasterVal  = ((MIX_MSTR_MAX_VOL << 8) | MIX_MSTR_MAX_VOL);
        temp        = g_InputNomVol - (iVolume - (g_NumInputAttSteps + NUM_MSTR_ATT_STEPS));
        iInputVal     = ((temp << 8) | temp);
    }
    
    
    // Update the Master Volume (always)
    SysPostMessage(3,MIXER_MASTER_SETLVL,iMasterVal);

    // Send the new iInputVal to the DAC or Line In
    
    if(g_iVolumeMode == VOLUME_MODE_DAC)
        SysPostMessage(3,MIXER_DAC_SETLVL, iInputVal);

    else if(g_iVolumeMode == VOLUME_MODE_LINE1)
        SysPostMessage(3,MIXER_LINE_SETLVL, iInputVal);
#ifdef FMTUNER_ON_LINE2_IN    
    else if(g_iVolumeMode == VOLUME_MODE_LINE2)
        SysPostMessage(3,MIXER_FM_SETLVL, iInputVal);
#endif  
}

//
// Put the funclet code down here to save a word.
//
#ifdef FUNCLET
    #pragma asm
    
        org p,".ptextsysvolume_funclet_entry":

        FSysUpdateVolume:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list
        CallFunclet RSRC_FUNCLET_SYSUPDATEVOLUME

    #pragma endasm
#endif
