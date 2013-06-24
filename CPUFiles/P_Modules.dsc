section .ptextsofttimers                    MP3_COPY;

contiguous
{
    section .ptextdisplaymodule             MP3_COPY;
    section .ptextdisplaymodule_color       MP3_COPY;
    section .ptextdisplaydrv                MP3_COPY;
    section .ptextdisplaydrv_color          MP3_COPY;
    section .ptextsed15xx                   MP3_COPY;
    section .ptextsed15xxinit               MP3_COPY;
    section .ptexts6b1713                   MP3_COPY;
    section .ptexts6b1713init               MP3_COPY;
    section .ptextlds514					MP3_COPY;
    section .ptextlds514init				MP3_COPY;
    section .ptextili516					MP3_COPY;
    section .ptextili516init				MP3_COPY;
    section .ptextssd1332					MP3_COPY;
    section .ptextssd1332init				MP3_COPY;
    section .ptexts6b33b0a		            MP3_COPY;
    section .ptexts6b33b0a_65k	            MP3_COPY;
    section .ptexts6b33b0a_262k	            MP3_COPY;
    section .ptexts6b33b0ainit	            MP3_COPY;
    overlay
    {
        section .ptextframebuffer           MP3_COPY;
        section .ptextdirecttolcd           MP3_COPY;
        section .ptextcolorlcdsal	        MP3_COPY;
    }
}
contiguous //(CHANGE TO MP3_COPY? but would also need that for menus.dsc)
{
    section .ptexttunermodule               MP3_COPY;
    overlay
    {
        section .ptextphilips5767           MP3_COPY;
        section .ptextphilips5757           MP3_COPY;
        section .ptextstfm1000              MP3_COPY;
        section .ptextsanyo24000            MP3_COPY; // size 0 if not compiled
    }
}

// WMA decoder module consists of 2 sections
contiguous
{
    section DECWMAMOD_DecoderOverlay_overlay_P   MP3_COPY;
    section .wmadecinit                          MP3_COPY;
}
contiguous
{
    section .ptextsystemmodule              MP3_COPY;
    section SystemApi_P_overlay             MP3_COPY;
}

// the rest of the modules only have 1 section
section LEDMOD_Led_overlay_p                MP3_COPY;
section GeqOverlay_overlay_P                MP3_COPY;
section MIXMOD_Mixer_overlay_P              MP3_COPY;
section SYSMOD_SystemApi_overlay_P          MP3_COPY;
section DECMOD_Stfm_overlay_P           MP3_COPY;
section DECMOD_AdpcmIma_overlay_P           MP3_COPY;
section DECMOD_AdpcmSmv_overlay_P           MP3_COPY;
section DECMOD_DecoderOverlay_overlay_P     MP3_COPY;
section ENCMOD_AdpcmIma_overlay_P           MP3_COPY;
contiguous
{
    section ENCMOD_Mp3_overlay_P            MP3_COPY;
    section .ptextmp3filename               MP3_COPY;
}
contiguous
{
    overlay
    {
        contiguous
        {
            section .ptextmusiclib_browse_function_common   MP3_COPY;
            overlay
            {
                contiguous
                {
                    section .ptextmusiclib_browse_extra MP3_COPY;
                    overlay
                    {
                        section .ptextmusiclib_browse_module MP3_COPY;
                        section .ptextmusiclib_voice_module MP3_COPY;
                    }
                }
                contiguous
                {
                    section .ptextmusiclib_browse_play  MP3_COPY;
                    overlay
                    {
                        contiguous
                        {
                            section .ptextmusiclib_browse_play_extra    MP3_COPY;
                            overlay
                            {
                                section .ptextmusiclib_play_module MP3_COPY;
                                section .ptextmusiclib_shuffle_module MP3_COPY;
                            }
                        }
                        contiguous
                        {
                            section .ptextmusiclib_browse_play_extra2   MP3_COPY;
                            overlay
                            {
                                section .ptextmusiclib_playallnext_module   MP3_COPY;
                                section .ptextmusiclib_playallprev_module   MP3_COPY;
                            }
                        }
                        section .ptextmusiclib_voiceplay_module MP3_COPY;
                    }
                }
            }
        }
        contiguous
        {
            section .ptextmusiclib_merge_module MP3_COPY;
            section .ptextmusiclib_merge_module_flash MP3_COPY;
        }
    }
}
contiguous
{
    overlay
    {
        contiguous {
            section .ptextplaylist5_browse_module           MP3_COPY;
            section .ptextplaylist5_browse_common           MP3_COPY;
        }
        contiguous {
            section .ptextplaylist5_playback_module         MP3_COPY;
            section .ptextplaylist5_playback_common         MP3_COPY;
        }
    }
    // locate common functions here
//    section .ptextfindfirst                     MP3_COPY;
//    section .ptextfindnext                      MP3_COPY;
}
section Audible_DECMOD_DecoderOverlay_overlay_P MP3_COPY;
contiguous
{
    section ENCMOD_Mp3_overlay_P MP3_COPY;
    section .ptextmp3filename MP3_COPY;
}
section DECACELPMOD_DecoderOverlay_overlay_P MP3_COPY;

overlay
{
	section .ptextFuncletEntryPoint2;
	section SYSFUNCLET_CREATEDIRECTORY_P				  MP3_COPY;
	section SYSFUNCLET_CreateDirRecord_P				  MP3_COPY;
	section SYSFUNCLET_DELETECONTENT_P					  MP3_COPY;
	section SYSFUNCLET_FASTOPEN_P						  MP3_COPY;
	section SYSFUNCLET_FOPENW_P							  MP3_COPY;
	section SYSFUNCLET_FOPEN_P							  MP3_COPY;
	section SYSFUNCLET_READDEVICERECORD_P				  MP3_COPY;
	section SYSFUNCLET_RMDIR_P							  MP3_COPY;
    section SYSFUNCLET_DisplayPrintBitmapFile_P           MP3_COPY;
	section SYSFUNCLET_RMDIRW_P							  WMA_COPY;
	section SYSFUNCLET_SEARCHDIRECTORY_P				  WMA_COPY;
	section SYSFUNCLET_SETHANDLEFORSEARCH_P				  WMA_COPY;
	section SYSFUNCLET_SETSHORTFILENAME_P				  WMA_COPY;
	section SYSFUNCLET_CHDIR_P							  WMA_COPY;

	contiguous
	{
		section .ptextskiponewordforsteppingvoltage        WMA_COPY;
		section .ptextsteppingvoltageset				   WMA_COPY;
	}

	section SYSFUNCLET_ConverToShortname_P				  WMA_COPY;
	section SYSFUNCLET_GETSHORTFILENAME_P                 WMA_COPY;
	section SYSFUNCLET_LONGDIRMATCH_P                     WMA_COPY;
	section SYSFUNCLET_Shortdirmatch_P                    WMA_COPY;
	section SYSFUNCLET_ArrangeLongFileName_P			  WMA_COPY;

	//DRM funclets moved into module space.
    section SYSFUNCLET_AGGREGATE_P                        WMDRM_COPY1;
    section SYSFUNCLET_CREATE_MODULUS_P         PK_COPY;
    section SYSFUNCLET_DIVIDE_PRECONDITION_1_P          PK_COPY;
	section SYSFUNCLET_DIVIDE_IMMEDIATE_P           PK_COPY;
	section SYSFUNCLET_DRM_ASD_GETLICENSEAGGREGATEDATA_P            WMDRM_COPY1;
	section SYSFUNCLET_DRM_ASD_PARSEV2LICENSE_P         WMDRM_COPY1;
	section SYSFUNCLET_DRM_B64_DECODEW_INPLACE_P        WMDRM_COPY1;
	section SYSFUNCLET_DRM_B64_DECODEW_P            WMDRM_COPY2;
	section SYSFUNCLET_DRM_B64_ENCODEW_P            WMDRM_COPY2;
	section SYSFUNCLET_DRM_CLK_INITCHECKSECURECLOCK_P        WMDRM_COPY1;
	section SYSFUNCLET_DRM_CPHR_INIT_P          WMDRM_COPY2;
	section SYSFUNCLET_DRM_DCP_GETATTRIBUTE_P            WMDRM_COPY1;
	section SYSFUNCLET_DRM_DCP_LOADPROPERTIESCACHE_P            WMDRM_COPY1;
	section SYSFUNCLET_DRM_EXPR_EVALUATEEXPRESSION_P            WMDRM_COPY2;
	section SYSFUNCLET_DRM_LEVL_PERFORMOPERATIONS_P         WMDRM_COPY1;
	section SYSFUNCLET_DRM_LIC_COMPLETELICENSECHAIN_P           WMDRM_COPY2;
	section SYSFUNCLET_DRM_LIC_GETENABLINGBITS_P           WMDRM_COPY1;
	section SYSFUNCLET_DRM_LIC_VERIFYSIGNATURE_P            WMDRM_COPY1;
	section SYSFUNCLET_DRM_MGR_BIND_P           WMDRM_COPY1;
	section SYSFUNCLET_DRM_PK_DECRYPT_P         PK_COPY;
	section SYSFUNCLET_DRM_PK_ENCRYPT_P         PK_COPY;
	section SYSFUNCLET_DRM_PK_SIGN_P            PK_COPY;
	section SYSFUNCLET_DRM_PK_VERIFY_P          PK_COPY;
	section SYSFUNCLET_DRM_SST_SETTOKENVALUE_P          WMDRM_COPY1;
	section SYSFUNCLET_ECAFFINE_ADDITION_P          PK_COPY;
	section SYSFUNCLET_ECAFFINE_EXPONENTIATION_TABULAR_P            PK_COPY;
	section SYSFUNCLET_ECAFFINE_TABLE_CONSTRUCTION_P            PK_COPY;
	section SYSFUNCLET_FUNCTIONGETVALUE_FN_DATEDIFF_P        WMDRM_COPY1;
	section SYSFUNCLET_FUNCTIONGETVALUE_P           WMDRM_COPY2;
	section SYSFUNCLET_FUNCTIONGETVALUE_PART1_P        WMDRM_COPY1;
	section SYSFUNCLET_GENDEVICECERTIFICATE_P        WMDRM_COPY1;
	section SYSFUNCLET_GETNEXTLICENSE_P         WMDRM_COPY1;
	section SYSFUNCLET_GETPRIVKEY_P        WMDRM_COPY1;
	section SYSFUNCLET_MOD_EXP2000_P            PK_COPY;
	section SYSFUNCLET_MOD_LUCASUV_P            PK_COPY;
	section SYSFUNCLET_MOD_SQRT_P           PK_COPY;
	section SYSFUNCLET_MP_GCDEX_P          PK_COPY;
	section SYSFUNCLET_MP_GCDEX_SPLIT1_P          PK_COPY;
	section SYSFUNCLET_PERFORMOPERATION_PART5_P         WMDRM_COPY2;
	section SYSFUNCLET__DECRYPTCONTENTKEY_P         WMDRM_COPY2;
	section SYSFUNCLET__GETLICENSEINFOANDSETUP_P            WMDRM_COPY2;
	section SYSFUNCLET__HDSADJUSTCHILDPAYLOAD_P             WMDRM_COPY2;
	section SYSFUNCLET__HDSALLOCBLOCK_P             WMDRM_COPY2;
	section SYSFUNCLET__HDSREMOVESLOT_P             WMDRM_COPY2;
	section SYSFUNCLET__HDSSLOTENUMNEXT_P           WMDRM_COPY1;
	section SYSFUNCLET__OPENANDVERIFYSLOT_P         WMDRM_COPY1;
	section SYSFUNCLET__SCANNODEFORATTRIBUTEW_P         WMDRM_COPY1;
	section SYSFUNCLET__SETUPLICEVALOBJECTTOSHARE_P         WMDRM_COPY1;
	section SYSFUNCLET_EVAL_P        					WMDRM_COPY1;
	section SYSFUNCLET_PERFORMOPERATION_PART2_P         WMDRM_COPY2;
	section SYSFUNCLET_PERFORMOPERATION_PART3_P         WMDRM_COPY2;
	section SYSFUNCLET__LOADPRORITIZEDLIST_P            WMDRM_COPY1;
	section SYSFUNCLET__UPDATEATTRIBUTETOKENINSLOT_P            WMDRM_COPY1;
	section SYSFUNCLET_WORDS_TO_ECAFFINE_P          PK_COPY;
	section SYSFUNCLET__HDSALLOCBLOCKBUFFER_P               WMDRM_COPY2;
	section SYSFUNCLET_MKTIME_P        WMDRM_COPY1;
    section SYSFUNCLET__HDSPREALLOC_P        WMDRM_COPY1;
    section SYSFUNCLET_TUNER_DRIVER_INIT_P                 WMA_COPY;
    section SYSFUNCLET_STFM1000_REG_POWER_UP1_P            WMA_COPY;
    section SYSFUNCLET_STFM1000_REG_POWER_UP2_P            WMA_COPY;
    //section SYSFUNCLET_STFM1000_POWER_UP_P                 WMA_COPY;
    section SYSFUNCLET_STFM1000_FIND_RSSI_THRESHOLD_P      WMA_COPY;
    section SYSFUNCLET_STFM1000_DATA_PATH_OFF_P            WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_INC_UP_P               WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_INC_DOWN_P             WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_SET_STANDBY_P          WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_REPLACE_PRESET_P       WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_ERASE_PRESET_P         WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_TUNE_TO_PRESET_P       WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_SEARCH_DOWN_P          WMA_COPY;
    //section SYSFUNCLET_STFM1000_TUNER_MONITOR_QUALITY_P    WMA_COPY;
    section SYSFUNCLET_STFM1000_SET_LNA_BIAS_P     WMA_COPY;
    section SYSFUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL_P   WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_SCAN_STATION_P         WMA_COPY;
    section SYSFUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE_P WMA_COPY;
    section SYSFUNCLET_TUNER_SAFE_POWER_DOWN_P             WMA_COPY;
    section SYSFUNCLET_STFM1000_LOOK_FOR_PILOT_P           WMA_COPY;
    section SYSFUNCLET_STFM1000_TUNER_SEARCH_ENDED_P       WMA_COPY;
//    section SYSFUNCLET_LoadEncoderCommon_P                 WMA_COPY;
    section SYSFUNCLET_STFM1000_TEST_PRESENCE_P            WMA_COPY;
    section SYSFUNCLET_STFM1000_I2C_INIT_P                 WMA_COPY;
    section SYSFUNCLET_I2C_WRITE_TUNER_REGISTER_P          WMA_COPY;
    section SYSFUNCLET_I2C_RESET_P                         WMA_COPY;
    section SYSFUNCLET_STFM1000_CONTINUE_SEARCHING_P       WMA_COPY;
    section SYSFUNCLET_GET_LRC_DIR_NAME_PL5_P              WMA_COPY;
    section SYSFUNCLET_PL5_PL_QUEUEPUSHBACK_P							 WMA_COPY;
    section SYSFUNCLET_PL5_PL_QUEUEPUSHFRONT_P						 WMA_COPY;    
}
