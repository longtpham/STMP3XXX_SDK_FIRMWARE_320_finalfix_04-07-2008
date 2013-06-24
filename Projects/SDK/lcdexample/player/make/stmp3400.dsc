/**************************************************************************
**
**  FILE        :  @(#)STMP3400.dsc 
**
**  VERSION     :  00/012/20
**
**  DESCRIPTION :  Default locator software part description;
**                     initially, this is
**                     for Sigmatel STMP34000.
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file. By default this file
**             assumes that the ROM data tables have not been 
**             enabled.
**
**
**************************************************************************/

#define TOTAL_ADDR_SPACE 0x10000

// P memory boundaries
#define INT_P_RAM        0x6000

#define INT_P_VECTOR_ORG 0x0000
#define INT_P_VECTOR_SZ  0x0080

#define INT_P_APP_ORG    (INT_P_VECTOR_ORG+INT_P_VECTOR_SZ) // 0x80
#define INT_P_APP_SZ     0x203f                                    

#define INT_P_MDD_ORG    (INT_P_APP_ORG+INT_P_APP_SZ)       // 0x234a
#define INT_P_MDD_SZ     0x10B0

#define INT_P_SYS_ORG    (INT_P_MDD_ORG+INT_P_MDD_SZ)       // 0x336d
#define INT_P_SYS_SZ     0x2860

#define INT_P_MODULE_ORG (INT_P_SYS_ORG+INT_P_SYS_SZ)       // 0x593d
#define INT_P_MODULE_SZ  (INT_P_RAM-INT_P_MODULE_ORG)

#define EXT_P_ORG        INT_P_RAM
#define EXT_P_RAM        TOTAL_ADDR_SPACE-INT_P_RAM

// X memory boundaries
#define INT_X_RAM        0x2000

#define INT_X_APP_ORG    0x0000
#define INT_X_APP_SZ     0x1359

#define INT_X_SYS_ORG    (INT_X_APP_ORG+INT_X_APP_SZ)
#define INT_X_SYS_SZ     0x0b65

#define EXT_X_ORG        INT_X_RAM
#define EXT_X_RAM        (TOTAL_ADDR_SPACE-INT_X_RAM)

// Y memory boundaries
#define INT_Y_RAM        0x2000
#define INT_Y_SYS_ORG    0x0000
#define INT_Y_SYS_SZ     0x0f3a

#define INT_Y_APP_ORG    (INT_Y_SYS_ORG+INT_Y_SYS_SZ)
#define INT_Y_APP_SZ     (INT_Y_RAM-INT_Y_APP_ORG)

#define EXT_Y_ORG        INT_Y_RAM
#define EXT_Y_RAM        (TOTAL_ADDR_SPACE-INT_Y_RAM)

software {
//change this to match the entry point for your program
        load_mod start=Exec;
    layout {
        space P {
            block PVectorBlock {
                // P-space memory range 
                cluster P_vector {
                        section EXEC_Vectors_P addr=0x000000;
                }
            }
            // P address space
           block PSysBlock {
                cluster P_sys_cluster {
                    amode P_far {
                     label eP_Media_DD_overlay;
                     label bP_sys_cluster;
                     section selection=w;
                     label eP_sys_cluster;                       
                     label bP_space_funclet;
                     overlay {
                        section SYSFUNCLET_SysResetAllInt_P attr=i;
                        section SYSFUNCLET_SysSetupDecVec_P attr=i;
                        section SYSFUNCLET_SysSetupEncVec_P attr=i;
                        section SYSFUNCLET_SysTimeInit_P attr=i;
                        section SYSFUNCLET_SysSetPlaybackMode_P attr=i;
                        section SYSFUNCLET_AnalogInit_P attr=i;
                        section SYSFUNCLET_UsbConnected_P attr=i;
                        section SYSFUNCLET_InitVoiceNumber_P attr=i;
                        section SYSFUNCLET_ButtonInit_P attr=i;
                        section SYSFUNCLET_ChangeDir_P attr=i;
                        section SYSFUNCLET_PowerUpHeadPhones_P attr=i;
                        section SYSFUNCLET_PowerDownHeadPhones_P attr=i;
                        section SYSFUNCLET_StartProject_P attr=i;
                        section SYSFUNCLET_ModuleSetTimer_P attr=i;
                        section SYSFUNCLET_InitVoiceNumber_P attr=i;
                        section SYSFUNCLET_FileSysSetDir_P attr=i;
                        section SYSFUNCLET_SearchDir_P attr=i;
                        section SYSFUNCLET_FatCountCx_P attr=i;
                        section SYSFUNCLET_FileOpen_P attr=i;
                        section SYSFUNCLET_FSOpen_P attr=i;
                        section SYSFUNCLET_FileClose_P attr=i;
                        section SYSFUNCLET_HndlFree_P attr=i;
                        section SYSFUNCLET_MediaDetection_P attr=i;
                        section SYSFUNCLET_SysStopDecoder_P attr=i;
                        section SYSFUNCLET_SysStopEncoder_P attr=i;
                        section SYSFUNCLET_SysSetDecoder_P attr=i;
                        section SYSFUNCLET_SysSetEncoder_P attr=i;
                        section SYSFUNCLET_ProcessPT_P attr=i;
                        section SYSFUNCLET_FileSystemPresent_P attr=i;
                        section SYSFUNCLET_FileSystemType_P attr=i;
                        section SYSFUNCLET_FileSystemIsReady_P attr=i;
                        section SYSFUNCLET_MEDIAReadBootSector_P attr=i;
                        section SYSFUNCLET_MEDIAUpdate_P attr=i;
                        section SYSFUNCLET_FatCountCxFat12_P attr=i;
                        section SYSFUNCLET_FatCountCxFat16_P attr=i;
                        section SYSFUNCLET_ParseDataBootSector_P attr=i;
                        section SYSFUNCLET_MmcEnumeratePhysicalDevice_P attr=i;
                        section SYSFUNCLET_ResetDevice_P attr=i;
                        section SYSFUNCLET_ProcessCsd_P attr=i;
                        section SYSFUNCLET_ProcessPartition_P attr=i;
                        section SYSFUNCLET_PartitionEntryCopy_P attr=i;
                        section SYSFUNCLET_SysSetSpeed_P attr=i;
                        section SYSFUNCLET_SysGetVolume_P attr=i;
                        section SYSFUNCLET_DisplaySplashScreen_P attr=i;
						section SYSFUNCLET_MixerMasterFadeOut_P attr=i;
						section SYSFUNCLET_MixerMasterFadeIn_P attr=i;
						section SYSFUNCLET_EncSetProperties_P attr=i;
                     }                       
                    reserved label=eP_space_funclet length=1;
                    }
                }
              }
           block PMediaDD {
                cluster P_Media_DD_cluster {
                    amode P_sjmp {
                    }  // amode P_sjmp
                    amode P_far {
                        label bP_Media_DD_overlay;
                        overlay addr=INT_P_MDD_ORG {
                            contiguous {
                               section SMDD_Header_p;
                               section SMDD_High_p;
                            }
                            contiguous {
                               section MMCDD_Header_p attr=i;
                               section .ptextspihal attr=i;
                               section .ptextmmcsal attr=i;
                               section .ptextmmchalspi attr=i;
                               section SPI_HAL_ASM_p attr=i;
							   section .ptextmmcsalwrite attr=i;
							   section .ptextmmchalspiwrite attr=i; 
                            }
                        } // overlay
                    }  // amode far
                }  // cluster
            }  // block

           block PAppBlock {
                cluster P_app_cluster {
                    amode P_sjmp {
                        overlay addr=INT_P_VECTOR_SZ {
                            label bP_app_cluster;
                            contiguous { 
                                section .pnear_huffdec attr=i;
                                section .pnear_huffdec2 attr=i;
                                section .ph_recfdom attr=i;
                            }  //contig
                        }  //overlay
                    }  // amode P_sjmp

                    amode P_far {
                        overlay addr=0x1e0 {
                            contiguous {                         
                                section .pdivmod attr=i;       
                                section .pdivunint attr=i;     
                                section .pupdptr attr=i;       
                                section .perr_func attr=i;
                                section .pdiv_sig attr=i;      
                                section .pasmwrap attr=i;      
                                section .pbttrfly attr=i;      
                                section .pconfig attr=i;       
                                section .pexp2infc attr=i;     
                                section .pdescalyy attr=i;     
                                section .pfreqidx attr=i;      
                                section .pcrc_mpeg attr=i;     
                                section .phuffdec2 attr=i;     
                                section .pdeemph attr=i;       
                                section .ptools attr=i;        
                                section .pchanctl attr=i;      
                                section .psfbwidth attr=i;     
                                section .pinvquant attr=i;     
                                section .panc_data attr=i;     
                                section .pihybrid attr=i;      
                                section .pset_ssl attr=i;      
                                section .pinvstero attr=i;     
                                section .pscalefac attr=i;     
                                section .pl3_core attr=i;      
                                section .phuffdec attr=i;      
                                section .pl3_sync attr=i;      
                                section .pdec_l3 attr=i;       
                                section .precfdom attr=i;      
                                section .pinvmdctl attr=i;     
                                section .pipolypha attr=i;     
                                section .pl3_head attr=i;
                                section .pmext attr=i;
                                section .pPTROFBSS_C attr=i;   
                                section .pRegisterBitPtr2BitNr attr=i;
                                section .pPTROFBSS_asm attr=i;
                                section .pBIT_DIS_asm  attr=i; 
                                section .pskip_bit_asm  attr=i;
                                section .pDIV_24_C  attr=i;    
                                section .pBSS2RegisterBitPtr attr=i;
                                section .prd_f_bit_asm attr=i;
                                section .prd_v_bit_asm attr=i;
                                section .pwr_v_bit_asm attr=i;
                                section .pCopyB2B_asm attr=i; 
                                section DECAPP_DecoderNonOverlay_P attr=i;                          
                                section DECAPP_Mp3Dec_P attr=i;
                            } // mp3 contiguous 
                        }   // mp3 overlay
                        overlay addr=INT_P_VECTOR_SZ {
                            // using auto section locator label since only 1 section, because when memory full
							//     F_lc_b_DECAPP_DecAdpcmImaNonOverlay_P
							//     F_lc_e_DECAPP_DecAdpcmImaNonOverlay_P
							// the end label gets moved to the beginning of memory
							// must add labels back if another section name is added -- and change code
                            //label   b_IMAADPCM_P_buf;                         
                            section DECAPP_DecAdpcmImaNonOverlay_P attr=i;
                            //label   e_IMAADCM_P_buf;

                            contiguous{
                                section WriteFAT12_p attr=i;
                                overlay {
                                    section ENCAPP_EncAdpcmImaNonOverlay_P attr=i;
                                    contiguous{
                                        label bP_directory;                       
                                        section DIRAPP_SysDir_P attr=i;
                                        section DIRAPP_SysVoice_P attr=i;
                                        label eP_directory;
                                        section SYSAPP_SysSettings_Init_P attr=i;
                                        section SYSAPP_SysSettings_Shutdown_P attr=i;
                                    } // contiguous
                                }  // encoder/directory overlay
                            } // writefat/encoder-dir contiguous
                        }  // non-mp3 overlay
                    }  // amode far
                }  // cluster
            }  // block
           block PModuleOverlayBlock {
                cluster P_Module_overlay_clstr {
                    amode P_far {
                        label bP_Module_overlay;
                        overlay {
                            contiguous {
                                section SoftTimers_ID_p  attr=i;
                                section .ptextsofttimers attr=i;
                            }
                            contiguous {
                                overlay
                                    {
                                    section MainMenu_ID_p               attr=i;
                                    section MusicMenu_ID_p              attr=i;
                                    section PlayerStateMachine_ID_p     attr=i;
                                    section VoiceMenu_ID_p              attr=i;
                                    section RecorderStateMachine_ID_p   attr=i;
                                    section FMTunerMenu_ID_p            attr=i;
                                    section Display_ID_p                attr=i;
                                    section DisplayLists_ID_p           attr=i;
                                    section ContrastMenu_ID_p           attr=i;
                                    section EqMenu_ID_p                 attr=i;
                                    section PlayModeMenu_ID_p           attr=i;
                                    section PwrMenu_ID_p                attr=i;
                                    section SettingsMenu_ID_p           attr=i;                                                                         
                                    section AboutMenu_ID_p              attr=i;
                                    section DeleteMenu_ID_p             attr=i;
                                    }
                                section MenuManager_p    attr=i;
                                overlay {
                                    section .ptextmainmenu              attr=i;
                                    section .ptextplayerstatemachine    attr=i;
                                    section .ptextdisplay               attr=i;
                                    section .ptextvoicemenu             attr=i;
                                    section .ptextfmtunermenu           attr=i;
                                    contiguous
                                    {   section .ptextmusicmenu         attr=i;
                                        section .ptextmusicmenu_extra   attr=i;
                                    }
                                    section .ptextrecorderstatemachine  attr=i;
                                    section .ptextdisplaylists          attr=i;
                                    section .ptextcontrastmenu          attr=i;
                                    section .ptexteqmenu                attr=i;
                                    section .ptextsettingsmenu          attr=i;
                                    section .ptextpwrsettingsmenu       attr=i;
                                    section .ptextplaymodemenu          attr=i;
                                    section .ptextdeletemenu            attr=i;
                                    section .ptextaboutmenu             attr=i;
                                }
                            }
                         section FMTMOD_Fmt_overlay_p attr=i;
                         contiguous {                                              
                             section LCDMOD_Lcd_overlay_p attr=i;
                             section .ptextlcdprintstring attr=i;
                             section .ptextdbcs attr=i;
                         }
                         section PARMOD_Parser_overlay_P attr=i;                        
                         section PARVOIMOD_Parser_overlay_P attr=i;
                         section MIXMOD_Mixer_overlay_P attr=i;
                         section SYSMOD_SystemApi_overlay_P attr=i;
                         section DECMOD_AdpcmIma_overlay_P attr=i;
                         section DECMOD_DecoderOverlay_overlay_P attr=i;
                         section ENCMOD_AdpcmIma_overlay_P attr=i;
                         section GeqOverlay_overlay_P attr=i;

                        }
                    }

                } //end overlay cluster


            }
        
            block PExtBlock {
                cluster P_placeholder {
                   amode P_far {
                        label eP_Module_overlay;
                    }
                }
            }
        }

          
        space X {
            // X address space
           block XAppBlock  {
                cluster X_app_cluster {
                    amode X_far {
                        label bX_app_cluster;
                        overlay {
                            contiguous {
		                        section DECAPP_Mp3DecoderMem_X; 
                                section .xerr_func attr=i;
                                section .xWorkBuffer attr=i;
                                section .xlevel attr=i;
                                section .xconfig attr=i;
                                section .xtools attr=i;
                                section .xcrc_mpeg attr=i;
                                section .xFMP3_HostFuncTable attr=i;
                                section .xrecfdom attr=i;
                                section .xmata attr=i;
                                section .xsectionB2B_asm attr=i;
                                section .xfreqidx attr=i;
                                section .xBitrate_MPEG1 attr=i;
                                section .xBitrate_MPEG2 attr=i;
                                section .xBitrate_MPEG25 attr=i;
                                section .xFReset_to_zero attr=i;
                                section .xexp2infc attr=i;
                                section .xtonectr attr=i;
                                section .xanc_data_x attr=i;
                                section .xl3_core attr=i;
                                section .xchanctl attr=i;
                                section .xinvquant attr=i;
                                section .xl_huffdec attr=i;
                                section .xasm_wrap_x attr=i;
                                section .xinvstero attr=i;
                                section .xhplp_bt attr=i;
                                section .xFMP3_HostParmTable attr=i;
                                section .xdeemph attr=i;
                                section .xl3_head attr=i;
                                section .xmtabs attr=i;
                                section .xinvmdctl attr=i;
                                section .xdec_l3 attr=i;
                            }
                            contiguous {
                                label   b_IMAADPCMDEC_X_buf;
                                section DECAPP_DecAdpcmMem_X attr=i;
                                section DECAPP_DecAdpcmMem_mod_X;
                                label   e_IMAADPCMDEC_X_buf;
                            }
                            contiguous{
                                section WriteFAT12_x attr=i;
								overlay{
								// if don't force overlay label will go with WriteFAT12_X
                                label   b_IMAADPCMENC_X_buf;
                                section ENCAPP_EncAdpcmMem_mod_X;
								}
                                section ENCAPP_EncAdpcmMem_X attr=i;
                                section ENCAPP_EncAdpcmImaNonOverlay_X attr=i;
                                label   e_IMAADPCMENC_X_buf;
                            }
                        }   // x_app overlay
                        section .xscalefac attr=i;
                        section xOVLN attr=i;
                        section .xsfbwidth attr=i;
                        section .xipolypha attr=i;
                        section .xhct__std attr=i;
                        section .xCopyB2B_asm attr=i;
                        section DECAPP_Mp3DecoderMem_2_X attr=i;   
                        section DECAPP_Mp3WrapperMem_X attr=i;
                        section DECAPP_DecoderMem_X attr=i;
                        section .xbss;
                        label eX_app_cluster;
                    }  // amode
                }   // cluster
            }   // block
            block XSysBlock {
                cluster X_sys_cluster {
                    amode X_far {
                        label bX_sys_cluster;
                        section selection=w;
                    }
                }
            }
            block XExtBlock {
                cluster X_copy {
                    attribute i;
                    amode X_far;
                }
            }  // Ext X block
        }  //  X space

        space Y {
        // Y address space
            block YSysBlock {
            // Y-space memory range
                cluster Y_sys {
                    amode Y_far {
                        section GLOBAL_CONSTANTS_Y addr=0x00;
                        section selection=w;
                    }
                }
            }
            block YAppBlock{ 
                cluster Y_app {
                    amode Y_far {
                        label bY_app_cluster;
                        overlay addr=INT_Y_SYS_SZ {
                            contiguous {
                                 section WriteFAT12_y attr=i;

								 // this label must include sections: 
								 // ENCAPP_EncAdpcmMem_Y,ENCAPP_EncAdpcmMem_mod_Y,
								 // and ENCAPP_EncAdpcmImaNonOverlay_Y}
								 label b_IMAADPCMENC_Y_buf;
                                 section ENCAPP_EncAdpcmMem_Y attr=i;
                                 section ENCAPP_EncAdpcmImaNonOverlay_Y attr=i;
                            } // adpcm enc contig

                            section .yinvmdctl attr=i;

				    		// this labeling should include both sections
				    		// DECAPP_DecAdpcmMem_Y and DECAPP_DecAdpcmMem_mod_Y
							label b_IMAADPCMDEC_Y_buf;
                            section DECAPP_DecAdpcmMem_Y attr=i;                    

                        } // end overlay

                        overlay {
                            // all 0x800 wide buffers
                            section DECAPP_DecAdpcmMem_mod_Y;
                            section DECAPP_Mp3DecoderMem_mod_Y;                                     
 						    contiguous {
							// required to get end label right
                            section ENCAPP_EncAdpcmMem_mod_Y;
			                label e_IMAADPCMDEC_Y_buf;
					        label e_IMAADPCMENC_Y_buf;
						    }
                        } // end 2nd overlay

                        section  .yiploypha attr=i;
                        section  .yihybrid attr=i;
                        section  .yx43tab attr=i;
                        section  .ychanctl attr=i;
                        section  yOVLN attr=i;
                        section  .yinvmdctl_2 attr=i;
                        section  DECAPP_Mp3DecoderMem_Y attr=i;
                        section  DECAPP_DecoderMem_Y attr=i;
                    }
                }
            }
            // space for overlay copies Y_settings and P overlays
            block YExtBlock {
                cluster Settings_Y {
                    // this cluster doesn't have a "runtime" area
                    amode Y_far {
                        //!!!! this label must be inside amode of 1st cluster in ExtY
                        label eY_phys;
                        label eY_app_cluster;
                        section MIXMOD_Mixer_settings_Y;
                        section LCDMOD_Lcd_settings_Y;
                        section Player_settings_Y;
                        // SYSFUN_SysSettings MUST BE LAST!!
                        section SYSFUN_SysSettings_Commands_Y;
                    } 
                }
                cluster Y_Copy {
                    amode Y_far {
                        copy;
                    }
                }
            } // block
        } // space Y

        space L {
            // L address space
            block LBlock {
                // L-space memory range
                cluster L_clstr {
                        attribute w;
                    amode L_near {
                        section selection=w;
                    }
                    amode L_far {
						section .lovl addr=0x1fee;
                        section selection=w;
                    }
                }
            }
        }
    }
}


// include cpu part and memory part

cpu ..\..\..\..\..\CPUFiles\STMP3400.cpu
memory ..\..\..\..\..\CPUFiles\STMP3400.mem

