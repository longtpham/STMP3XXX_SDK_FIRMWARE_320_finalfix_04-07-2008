                        section GLOBAL_CONSTANTS_Y addr=0;
                        section SYSFUN_SysFunclet_Y;
                        section ReadFAT12_y;
                        section BUTMOD_Button_mod_Y;
                        section NANDHAL_ECCUTILS_y;
                        section .ybssnanddatadrivemultiwritedumb;
                        section MenuManager_Stack_y;
                        section SYSFUN_StackMem_Y;
//						section .ybssfsproj;	
                        //section .ybssplayerlib;
                        //section BUTMOD_Button_Y;
						section .yovlnddgetusectorsmap;
						section .ybssmetadata;
                        section SYSPLAYER_MsgQueue_menu_Y;
                        section SYSPLAYER_MsgQueue_geq_Y;
                        section SYSPLAYER_MsgQueue_decoder_Y;
                        section SYSPLAYER_MsgQueue_FMTuner_Y;
                        section SYSPLAYER_MsgQueue_SoftTimer_Y;
                        section SYSPLAYER_MsgQueue_mixer_Y;
                        section SYSPLAYER_MsgQueue_system_Y;
                        section SYSPLAYER_MsgQueue_encoder_Y;
                        section SYSPLAYER_MsgQueue_Playlist5_Playback_Y;
                        section SYSPLAYER_MsgQueue_Playlist5_Browse_Y;
                        section SYSPLAYER_ModuleTable_Y;
                        section SYSFUN_SysMem_Y;
						//section .ydataddiproject;
						section .ybsschipserialnumberinit; // Moved from Y_extra
                        section .ydatafsddread ;
                        section .ydatamemorysetup;
                        section .ydatanandmediadefaults;
                        section .yovlnandmediadiscover;
                        section SYSFUN_SysSpeed_Y;
						section .YlicData;
												section .ydatanandhalreadfunctions;
//                        section .yovlmiscproject;   // from Y_Extra
                        section .ybssvbr;           // from Y_Extra
                        section .ynanddatadriveapi; // from Y_Extra
                        section .ynandsystemdriveapi;   // from Y_Extra
						section .ybsscreatedirectoryrecord;
						section .ybsslowresolutionadc;
						//section .ydataplaylist3; //move to Y_Extra
			            //section .ydataframebuffer; //  Remove to increase stack size to 750. import here currently. for 3.1 may not be able to keep this here. TOVERIFY TOVERIFY2 critical sdk3.05 did not have this ln here but 3.1prelim did. Fit? maybe not.
						//section .ybssmmcmediainit;
						//section .ybssfsutilsc;
