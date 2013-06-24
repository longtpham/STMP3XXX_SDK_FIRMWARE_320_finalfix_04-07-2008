/**************************************************************************
**  FILE        :  STMP3500.dsc  (LCDexample audio player build)
**
**  DESCRIPTION :  Default locator software part description;
**                 initially, this was for Sigmatel STMP3400 & STMP3410. Now its for 35xx
**
**  NOTE   :  Values for the size of internal memory spaces are
**                taken from the CPU file. By default this file
**                assumes that the ROM data tables have not been enabled.
**************************************************************************/
#define TOTAL_ADDR_SPACE 0x10000

// P memory boundaries
#define INT_P_RAM        0xc000

#define INT_P_VECTOR_ORG 0x0000
#define INT_P_VECTOR_SZ  0x0084

#define INT_P_APP_ORG    (INT_P_VECTOR_ORG+INT_P_VECTOR_SZ) // 0x80
#define INT_P_APP_SZ     0x5edc //0x5Ebc

#define INT_P_MDD_ORG    (INT_P_APP_ORG+INT_P_APP_SZ)
//#define INT_P_MDD_SZ    0xA37 //0xA73
#define INT_P_MDD_SZ      0xA80 //0xA37 //0xA73

#define INT_P_SYS_ORG    (INT_P_MDD_ORG+INT_P_MDD_SZ) //Sum is suggestion to locator but locator actually does a best fit
#define INT_P_SYS_SZ     (0x449d-5) //(0x43e3+104-40+100) //0x41ec //0x43E3 //0x437e //(0x3FEA + 0x210 + 0x5b + 0xD2 + 0x64+0xb7+9)
// P_Funclet_cluster is in end of P_sys and is part of p sys block (before p module).

#define INT_P_MODULE_ORG (INT_P_SYS_ORG+INT_P_SYS_SZ)       // Sum is suggestion to locator but locator does a best fit
#define INT_P_MODULE_SZ  (INT_P_RAM-INT_P_MODULE_ORG)       // P Module size min was 0x5C (april 2005). Actual size may be larger since this is the remainder free area.

#define EXT_P_ORG        INT_P_RAM
#define EXT_P_RAM        TOTAL_ADDR_SPACE-INT_P_RAM

// X memory boundaries
#define INT_X_RAM        0x6000

#define INT_X_BTRM_ORG   0x0000
#define INT_X_BTRM_SZ    0x0

#define INT_X_APP_ORG    (INT_X_BTRM_ORG+INT_X_BTRM_SZ)
#define INT_X_APP_SZ     (0x4908-0xde-7-166-24-50-60-50) //0x4908 //0x496c //0x4a22 //decrease by x100 and increase x_SYS_SZ by x100 so largest player fits. Increase Xapp 0x100 so that we can move stuff from Y to X to accomodate stack growth  //4a22 is 3.05 - 3.0beta then discount 0x600 wma buf growth, x600-x436=x164. Took that from 3.05 number. ; sdk3.1 prelim previous:0x4892 // TOVERIFY LB proj was 4892 (new sect buffers) ;sdk3.0 was 0x4750 & sdk3.05 had 0x4b86. TOVERIFY2 review this. 3.1 not to have g_WmaDecSinkBufSize 1536==x600 word growth.

// Place this in between to reduce wastage.
#define INT_LTABLES_ORG  (INT_X_APP_ORG+INT_X_APP_SZ)
#define INT_LTABLES_SZ   0xCE

#define INT_L_CLSTR_ORG  (INT_LTABLES_ORG+INT_LTABLES_SZ)
#define INT_L_CLSTR_SZ   0xBD

#define INT_X_SYS_ORG    (INT_L_CLSTR_ORG+INT_L_CLSTR_SZ) // TOVERIFY: this is LB proj ver. In sdk3.0 this was hard coded at x4837
#define INT_X_SYS_SZ     (0x1647+7+166+24+50+60+50) //0x1569//0x1505 //0x1450  //(0x1447 + 105 + 3) //increase INT_X_SYS_SZ by x100 & decrease X_APP_SZ by x100 so largest player fits. TOVERIFY

#define INT_X_EXTRA_ORG  (INT_X_SYS_ORG+INT_X_SYS_SZ)
#define INT_X_EXTRA_SZ   (INT_X_RAM-INT_X_EXTRA_ORG)

#define EXT_X_ORG        INT_X_RAM
#define EXT_X_RAM        (TOTAL_ADDR_SPACE-INT_X_RAM-0x1000)

// Y memory boundaries
#define INT_Y_RAM        0x6000

#define INT_Y_SYS_ORG    0x0
#define INT_Y_SYS_SZ     0x1000//(0xfd7 + 0x06+2+5+4+4) //(0xfcc)                   TODO aug22'06: need to try to add 2 more here so its 0xfd7+6+2+2 and enable other TODO line below

#define INT_Y_APP_ORG    (INT_Y_SYS_ORG+INT_Y_SYS_SZ)
#define INT_Y_APP_SZ     (INT_LTABLES_ORG-INT_Y_APP_ORG)

#define INT_Y_EXTRA_ORG  (INT_LTABLES_ORG+INT_LTABLES_SZ)
#define INT_Y_EXTRA_SZ   (INT_Y_RAM-INT_Y_EXTRA_ORG)

#define EXT_Y_ORG        INT_Y_RAM
#define EXT_Y_RAM        (TOTAL_ADDR_SPACE-INT_Y_RAM)

software
{
    //change this to match the entry point for your program
    load_mod start=Exec;
    layout
    {
        space P
        {
            block PModuleOverlayBlock
            {
                cluster P_Module_overlay_clstr
                {
                    amode P_far
                    {
                        label bP_Module_overlay;
                        contiguous
                        {
                            overlay
                            {
                                label ModuleRegion_b;
                                contiguous
                                {
                                    overlay // ***
                                    {
                                        #include "..\..\..\..\..\CPUFiles\P_Modules.dsc"
                                        contiguous
                                        {
                                            section MenuManager_p    attr=i;
                                            overlay
                                            {
                                                #include "..\..\..\..\..\CPUFiles\SystemCodebanks.dsc"
                                                #include "menus.dsc"
                                            }
                                        }
                                        ////////////////////////
                                        // Put your new modules here, they will be overlayed with
                                        // all the other modules. Make sure it is of the form:
                                        // section MyModule MP3_COPY;

                                        ////////////////////////
                                        contiguous //locate semi-statically in player.abs for run-once as reqd. Mk sequence matters
                                        {
                                            section .pbssnandhalinitfunctions ;
                                            section .pdatanandhalinitfunctions;
                                            section .ptextnandhalinitfunctions;
                                            section .ptextnandmediadiscover;
                                            section SYSHWINIT_main_P;
                                        }
                                    }//overlay
                                }//contiguous
//                              label ModuleRegion_e;
                            }//overlay
                        }//contiguous
                        label ModuleRegion_e;
                    }//amode
                }//cluster
           }//PModuleOverlayBlock

            // P address space
            block PSysBlock
            {
                cluster P_sys_cluster
                {
                    amode P_far
                    {
                        label eP_Media_DD_overlay;
                        label bP_sys_cluster;
                        /////////////////////////////////////////////
                        // all P sections not explicitly defined in this
                        //file will end up here in Psys, due to next line: all with writable attribute
                        section selection=w; ///////////////////////////
                        label eP_sys_cluster;
                     }
                }
                cluster P_Funclet_cluster
                {
                    amode P_far
                    {
                        contiguous
                        {
                            label FuncletRegion_b;
                            contiguous
                            {
                                overlay
                                {
                                    section .ptextFuncletEntryPoint;
                                    #include "..\..\..\..\..\CPUFiles\P_FuncletCluster.dsc"
                                }
                            }
                            label FuncletRegion_e;
                        }
                    }
                }
            }
            block PMediaDD {
                #include "..\..\..\..\..\CPUFiles\P_MediaDDCluster.dsc"
            }
            block PAppBlock {
                #include "..\..\..\..\..\CPUFiles\P_AppCluster.dsc"
            }

            block PVectorBlock // P-space memory range
            {
                cluster P_vector
                {
                    section EXEC_Vectors_P addr=0x000000;
                }
            }

            block PExtBlock
            {
                cluster P_placeholder
                {
                    amode P_far
                    {
                        label eP_Module_overlay;
                    }
                }
            }
        }

        space X //-------------------------------------------------------------------------
        {
            // X address space
            block XSysBlock
            {
                cluster X_boot_cluster
                {
                    amode X_far
                    {
                        label bX_const_cluster;
                        overlay
                        {
                            section DECAPP_DecoderMem_mod1_X;
                            section DECWMAMOD_DecoderMem_mod1_X;
                        }
                    }
                }
                cluster X_sys_cluster
                {
                    amode X_far
                    {
                        label bX_sys_cluster;
                        section selection=w;
                    }
                }
            }

            block XAppBlock
            {
                #include "..\..\..\..\..\CPUFiles\X_AppCluster.dsc"
            }

            #include "..\..\..\..\..\CPUFiles\X_LBlock.dsc"

            // TOVERIFY: This block was inserted by LB proj.
            block X_ExtraBlock
            {
                cluster X_Extra
                {
                    amode X_far
                    {
                        label bX_extra_cluster;
                    }
                }
            }  // TOVERIFY  as in merge 3.095.008  XExtBlock which used to be next is deleted.
        }//space X
        space Y   //----------------------------------------------------------------------
        {
            // Y address space
            block YSysBlock
            {
                // Y-space memory range
                cluster Y_sys
                {
                    amode Y_near
                    {
                        section .shortdata address=0x20;
                    }

                    amode Y_far
                    {
                        #include "..\..\..\..\..\CPUFiles\Y_sys.dsc"
                        section .ybssbattery;
                        //section .ybsslicensecleanstore;           // Moved to Y_Extra
                        //section .ybssnandhalcommonwritefunctions; // Moved to Y_Extra
                        section .ybssnandhalreadfunctions;
                        section .ybssnandhalsetnandbusy;
                        section .ybssnandhaltype1writefunctions;
                        section .ybssnandhaltype2writefunctions;
                        section .ybssnandmediaallocate;
                        //section .ybssstfm1000;
                        section .ydatachipcapability; // Move from Y_Extra
                        section .ybsssysvolume; // Move from Y_Extra
                        //section .ydataeqmenu;   // sz fits hole that was here after FS' global bufy sz reduction sept10,'05. Allows for customer globals in y_extra by moving this here.
                        //section .yconstmusiclib_build_function; // in largest+16bit clcd, didn't fit in const area so put it here. TODO: uncomment this line and test it.

                    }
                }
            }
            block YAppBlock
            {
                #include "..\..\..\..\..\CPUFiles\Y_AppCluster.dsc"
            }
            #include "..\..\..\..\..\CPUFiles\Y_LBlock.dsc"

            block Y_ExtraBlock
            {
                cluster Y_Extra
                {
                    amode Y_far
                    {
						//section SYSPLAYER_MsgQueue_display_Y; // june 9 2005 import from y_sys2 (y_app)
                        section .ydataframebuffer;  //  Move this here so that we can fit the stack to larger size 750 //moved to y sys in SDK 2.610
                        section .ybssphilips5767; // aug23'05 sdk3.1 addition so FM_PHILIPS build fits.
                        section .ybssstfm1000;
                        overlay{
                        	section .yovlmusiclib_build_function;
                        	section .yovlmusiclib_playallprev_module;
                        	section .yovlmusiclib_play_module;
                        	section .yovlmusiclib_browse_module;

                        }
						//section .ybssnandhalsetnandbusy;
                        section selection=w;
                        section selection=i;
                        section selection=r;
                    }
                }
            }

            // space for overlay copies Y_settings and P overlays
            block YExtBlock
            {
                cluster Settings_Y
                {
                    fixed addr=INT_Y_RAM; //This lets checkmem know the start addr of x external (non-addressable x).
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
                    attribute s;//don't locate anything here automatically.
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!

                    // this cluster doesn't have a "runtime" area
                    amode Y_far
                    {
                        //!!!! this label must be inside amode of 1st cluster in ExtY
                        label eY_phys;
                        contiguous
                        {
                            #include "settings.dsc"
                            // SYSFUN_SysSettings MUST BE LAST!!
                            section SYSFUN_SysSettings_Commands_Y;
                        }
                    }
                }
                #include "local_copies.dsc"
                #include "..\..\..\..\..\CPUFiles\ycopy.dsc"

            } // YExt block
        } // space Y
        //---------------------- Space L (X concatenated with Y) ---------------
        #include "..\..\..\..\..\CPUFiles\LSpace.dsc"
    }// end layout of all software part spaces
}//end software part. DELFEE: DEscriptive Language For Embedded Environments. Tasking asm24xx appdx.

// include cpu part and memory part  (3410 cpu and mem files used for 35xx SOC)
cpu ..\..\..\..\..\CPUFiles\STMP3410.cpu
memory ..\..\..\..\..\CPUFiles\STMP3410.mem

