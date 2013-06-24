//
// This should be set to the maximum size of any decoder.  It is used by
// the FGndModules sections to move them to the end of the last decoder and
// still overlay with the Janus code.
//
#define MAX_MP3_WAV_WMA_DECODER_SZ  0x53F7 //0x50F6 //0x549E //0x53DE //0x563C //0x53DE //0x53CF
#define MAX_ALL_DECODER_SZ          0x5B25 //0x5824 //0x5B35 //0x5A75 //0x5CD3 //0x5A75 //0x5A66
cluster P_app_cluster
{
    amode P_sjmp
    {
        overlay addr=INT_P_VECTOR_SZ
        {
            contiguous
            {
                label b_MP3_P_buf;
                section .pnear_huffdec                              MP3_COPY;
                section .pnear_huffdec2                             MP3_COPY;
                section .ph_recfdom                                 MP3_COPY;
            }  //contig
        }  //overlay
    }  // amode P_sjmp
    amode P_far
    {
        overlay addr=INT_P_VECTOR_SZ
        {
            label bP_app_cluster;
        }
        overlay addr=0x1e5
        {
            // MP3 overlay
            contiguous
            {
                section .pdivmod                                    MP3_COPY;
                section .pdivunint                                  MP3_COPY;
                section .pupdptr                                    MP3_COPY;
                section .perr_func                                  MP3_COPY;
                section .pdiv_sig                                   MP3_COPY;
                section .pasmwrap                                   MP3_COPY;
                section .pbttrfly                                   MP3_COPY;
                section .pconfig                                    MP3_COPY;
                section .pexp2infc                                  MP3_COPY;
                section .pdescalyy                                  MP3_COPY;
                section .pfreqidx                                   MP3_COPY;
                section .pcrc_mpeg                                  MP3_COPY;
                section .phuffdec2                                  MP3_COPY;
                section .pdeemph                                    MP3_COPY;
                section .ptools                                     MP3_COPY;
                section .pchanctl                                   MP3_COPY;
                section .psfbwidth                                  MP3_COPY;
                section .pinvquant                                  MP3_COPY;
                section .panc_data                                  MP3_COPY;
                section .pihybrid                                   MP3_COPY;
                section .pset_ssl                                   MP3_COPY;
                section .pinvstero                                  MP3_COPY;
                section .pscalefac                                  MP3_COPY;
                section .pl3_core                                   MP3_COPY;
                section .phuffdec                                   MP3_COPY;
                section .pl3_sync                                   MP3_COPY;
                section .pdec_l3                                    MP3_COPY;
                section .precfdom                                   MP3_COPY;
                section .pinvmdctl                                  MP3_COPY;
                section .pipolypha                                  MP3_COPY;
                section .pl3_head                                   MP3_COPY;

                section .pPTROFBSS_C                                MP3_COPY;
                section .pRegisterBitPtr2BitNr                      MP3_COPY;
                section .pPTROFBSS_asm                              MP3_COPY;
                section .pBIT_DIS_asm                               MP3_COPY;
                section .pskip_bit_asm                              MP3_COPY;
                section .pDIV_24_C                                  MP3_COPY;
                section .pBSS2RegisterBitPtr                        MP3_COPY;
                section .prd_f_bit_asm                              MP3_COPY;
                section .prd_v_bit_asm                              MP3_COPY;
                section .pwr_v_bit_asm                              MP3_COPY;
                section .pCopyB2B_asm                               MP3_COPY;

                section .ptextdec_play                              MP3_COPY;
                section .pSubbandSynthesisLoop                      MP3_COPY;
                section .ptextcommon                                MP3_COPY;
                section .pGetSamples_Layer2                         MP3_COPY;
                section .ptextmpegmain                              MP3_COPY;
                section .ptextlayer1                                MP3_COPY;
                section .ptextbitstreams                            MP3_COPY;
                section .ptextlayerallo                             MP3_COPY;
                section .Layer12BitFunc                             MP3_COPY;
                contiguous {
                    overlay {
                        contiguous {
                            section DECAPP_DecoderNonOverlay_P                  MP3_COPY;
                            section DECAPP_Mp3Dec_P                             MP3_COPY;
                        }

                        contiguous {
                            section DECAPP_AudibleDecoderNonOverlay_P	MP3_COPY;
                            section DECAPP_AudibleMp3Dec_P				MP3_COPY;
                            section .ptextaudiblemp3dec_c				MP3_COPY;
                            section .ptextaudiblemp3nonoverlay			MP3_COPY;
                            section .ptextaudiblemp3common				MP3_COPY;
                            section .ptextaudiblemp3overlay				MP3_COPY;
                        }
                    }
                }
                label e_MP3_P_buf;
            } // mp3 contiguous
        }   // mp3 overlay

        overlay addr=INT_P_VECTOR_SZ {	//Jpeg decoder and control code.
            label b_Jpeg_P;
            contiguous{
                label b_SMVADPCM_Dec_P;
                section .ptextsmvwave                   MP3_COPY;
                section SMV_ADPCM_Wave_BitStream_P      MP3_COPY;
                section SMVADPCM_BitStream_Decoder_P    MP3_COPY;
                section .ptextsmvwavedec                MP3_COPY;
                section DECAPP_DecAdpcmSmvNonOverlay_P  MP3_COPY;
                section Decode_SMV_ADPCM_Block_P        MP3_COPY;
        		section .ptextsmvjpegdecodedisplay		MP3_COPY;
        		section .ptextsmvdjcommon				MP3_COPY;
        		section .ptextsmvdjmain					MP3_COPY;
        		section .ptextsmvglobals				MP3_COPY;
        		section .ptextsmvdjhuff                 MP3_COPY;
        		section .ptextsmvdjidct                 MP3_COPY;
        		section .ptextsmvdjcolor                MP3_COPY;
        		section .ptextsmvdjinit                 MP3_COPY;
                label e_SMVADPCM_Dec_P;
            }
            contiguous{
                section .ptextjpegdecodedisplay		    MP3_COPY;
                section .ptextdjcommon					MP3_COPY;
                section .ptextdjmain					MP3_COPY;
                section .ptextglobals					MP3_COPY;
                overlay {
                    contiguous {
                        section .ptextdjhuff            MP3ENC_COPY;
                        section .ptextdjidct            MP3ENC_COPY;
                        section .ptextdjcolor           MP3ENC_COPY;
                        section .ptextdjinit            MP3ENC_COPY;
                        label e_Jpeg_P;
                    }
                    contiguous {
                        section .ptextdjbmp             MP3ENC_COPY;
                        label e_BMP_P;
                    }
                }
            }
        }
        //
        // These are actually never extracted in the player mode.
        //
        overlay addr=INT_P_VECTOR_SZ
        {
            overlay
            {
                section .ptextnanddatadrivemultiwritecache          WMDRM_COPY2;
                section .ptextnanddatadrivemultiwritedumb           WMDRM_COPY2;
            }
            section .ptextnandmediaallocate                         WMDRM_COPY2;
            section .ptextnandsystemdrivemultiwrite                 WMDRM_COPY2;
            section .ptextnandsystemdrivesetinfo                    WMDRM_COPY2;
            section .ptextnanddatadrivesetinfo                      WMDRM_COPY2;
            section .ptextnanddatadriveerase                        WMDRM_COPY2;
            section .ptextnandsystemdriveerase                      WMDRM_COPY2;
            section .ptextnandmediaerase                            WMDRM_COPY2;
            section .ptextnandsystemdrivewritesector                WMDRM_COPY2;
        } // nand routines
        overlay addr=INT_P_VECTOR_SZ
        {
            // Encoder/writefat overlay
            contiguous
            {
                overlay
                {
	                contiguous
	                {
		                label bFATWrite_P;
		                section .ptextnddcopysectors                        WMDRM_COPY;
		                section .ptextnddfindfreeublock                     WMDRM_COPY;
		                section .ptextnddstartnewupdate                     WMDRM_COPY;
		                section .ptextfsddwrite                             WMDRM_COPY;
		                section .ptextnddkillusectorsmap                    WMDRM_COPY;
		//                section .ptextnandwritesync                         WMDRM_COPY; // TOVERIFY3  do we need this line? doubt it. sz may be 0 anyway.
		                section .ptextnddrecoverbadhblock                   WMDRM_COPY;
		                section .ptextnddrecoverbadrblock                   WMDRM_COPY;
		                section .ptextnddlingetfreeerasedrblock             WMDRM_COPY;
		                section .ptextnddgetrecycledublock                  WMDRM_COPY;
		                section .ptextnddfreshenupdate                      WMDRM_COPY;
		                section .ptextnddfggc                               WMDRM_COPY;
		                section .ptextnddaddusector                         WMDRM_COPY;
		                section .ptextnddrecoverbadublock                   WMDRM_COPY;
		                section .ptextnddgetfreeerasedublock                WMDRM_COPY;
		                section .ptextnddwritesector                        WMDRM_COPY;
		                section .ptextnddkillusector                        WMDRM_COPY;
		                section .ptextnddcloseupdate                        WMDRM_COPY;
		                section .ptextnandhaltype1writefunctions            WMDRM_COPY;
		                section .ptextnandhaltype2writefunctions            WMDRM_COPY;
		//                section .ptextnandhaltype4functions                 WMDRM_COPY; // remove ancient line.
		                section .ptextnandhaltype5functions                 WMDRM_COPY;
		                section .ptextnandhalcommonwritefunctions           WMDRM_COPY;
		                section .ptextnandra_utils_write                    WMDRM_COPY;
		                section .ptextnandhalstartcomputeecc                WMDRM_COPY;
		                section .ptextnandhalsetnandbusy                    WMDRM_COPY;
		                section .ptextnandhalcomputeeccgetstatus            WMDRM_COPY;
		                section  NANDHIL_Write_P                            WMDRM_COPY;
		                section .ptextnddgetfreeerasedrblock                WMDRM_COPY;
		                section .ptextnandcalculateeccsync                  WMDRM_COPY;
		                section .ptextnandhalmarkblockbadinra               WMDRM_COPY;
		                section .ptextnddkillupdate                         WMDRM_COPY;
		                section .ptextfflush                                WMDRM_COPY;
		                section .ptextflushsector                           WMDRM_COPY;
		//                section .ptextfindnextfreecurrentcluster            WMDRM_COPY; // sz 0 item. remove
		                section .ptextupdatefilesize                        WMDRM_COPY;
		                section .ptextgetnewcluster                         WMDRM_COPY;
		                section .ptextwritefatentry                         WMDRM_COPY;
		                section .ptextwritesector                           WMDRM_COPY;
		                section .ptextfwrite                                WMDRM_COPY;
		                section .ptextfirstfreeandallocate                  WMDRM_COPY;
		                section .ptexttotalfreecluster                      WMDRM_COPY;
		                section .pStmpTotalfreecx                           WMDRM_COPY;
		                section .ptextcomputefreeclusters                   WMDRM_COPY;
		                section .ptextmmchalspiwrite                        WMDRM_COPY; // only need for mmc builds
		                section .ptextndd_pin_allocation                    WMDRM_COPY;
		                section .ptextndd_write_sector_in_place             WMDRM_COPY;
		                label eFATWrite_P;
		              // sdk3.11 : this next overlay and contiguous keyword solves janus playback 
		              // defects by forcing code below to be AFTER eFATWrite_P since code below needs to write to media.                                 
					}
		            contiguous
		            {
		                label b_JanusCommon_P;
		                section .psysloadpkcrypto                           WMDRM_COPY;
		                section .ptextjanusutils                            WMDRM_COPY;
		                section .ptextJanusfunc                     		WMDRM_COPY;
		                section .pJanusWrap                         		WMDRM_COPY;
		                section .pDRM_RC4_Cipher_X                  		WMDRM_COPY;
		                section .pWMDRMMP_C_STEP                    		WMDRM_COPY;
		                section .pDRM_DES_Cipher                    		WMDRM_COPY;
		                section .ptextdrmmanager                   			WMDRM_COPY;
		                section .ptextblackbox                     			WMDRM_COPY;
		                section .ptextdrmcbc                       			WMDRM_COPY;
		                section .ptextdrmcbc_runtime                        WMDRM_COPY;
		                section .ptextdrmcipher                    			WMDRM_COPY;
		                section .ptextdrmcipher_runtime                     WMDRM_COPY;
		                section .ptextrc4                          			WMDRM_COPY;
		                section .ptextoemimpl                               WMDRM_COPY;
		                section .ptextfield1                        		WMDRM_COPY;
		                section .ptextkmuladd                        		WMDRM_COPY;
		                section .ptextmprand                        		WMDRM_COPY;
		                section .pAsmBignum                         		WMDRM_COPY;
		                label e_JanusCommon_P;
		                overlay 
		                {
		                    contiguous
		                    {    //  doplay section
		                        label b_doplay_P;
		                        section  .pDRM_RC4_Cipher                   WMDRM_COPY;
		                        section  .ptextdrmsyncstore                 WMDRM_COPY;
		                        section  .ptextdrmint64                     WMDRM_COPY;
		                        section  .ptextdrmhmac                      WMDRM_COPY;
		                        section  .ptextdrmmetering                  WMDRM_COPY;
		                        section  .ptextdrmhdrparser                 WMDRM_COPY;
		                        section  .ptextdrmcrt                       WMDRM_COPY;
		                        section  .ptextoeminfo                      WMDRM_COPY;
		                        section  .pWMDRMdes                         WMDRM_COPY;
		                        section  .ptextdrmoutputlevels              WMDRM_COPY;
		                        section  .ptextdrmliceval                   WMDRM_COPY;
		                        section  .ptextdevicedevcert                WMDRM_COPY;
		                        section  .ptextdrmbase64                    WMDRM_COPY;
		                        section  .ptextdrmlicstore                  WMDRM_COPY;
		                        section  .ptextdevcert                      WMDRM_COPY;
		                        section  .ptextdrmexprvariable              WMDRM_COPY;
		                        section  .ptextdrmexprfunc                  WMDRM_COPY;
		                        section  .ptextdrmlicparser                 WMDRM_COPY;
		                        section  .ptextdrmutilities                 WMDRM_COPY;
		                        section  .ptextdrmchain                     WMDRM_COPY;
		                        section  .ptextdrmhds                       WMDRM_COPY;
		                        section  .ptextdrmviewrights                WMDRM_COPY;
		                        section  .ptextdrmsecstore                  WMDRM_COPY;
		                        section  .ptextdrmsecstore_ovl              WMDRM_COPY;
		                        section  .ptextdrmutilities_ovl             WMDRM_COPY;
		                        section  .ptextdrmxmlparser                 WMDRM_COPY;
		                        section  .ptextdrmexpreval                  WMDRM_COPY;
		                        section  .ptextprivate_key                  WMDRM_COPY;
		                        section  .ptextblackbox_init                WMDRM_COPY;
		                        section  .ptextdevcert_init                 WMDRM_COPY;
		                        section  .ptextdrmhds_init                  WMDRM_COPY;
		                        section  .ptextdrmmanager_init              WMDRM_COPY;
		                        section  .ptextgendevcert                   WMDRM_COPY;
		                        section  .ptextsecureclockrequest           WMDRM_COPY;
		                        label e_doplay_P;
		                    }
		                    contiguous
		                    {   // pkcrypto section
		                        label b_Pkcrypto_P;
		                        section .ptextpkcryptofunc                  PK_COPY;
		                        section  .ptextmodsqrt                      PK_COPY;
		                        section  .ptextmpinit                       PK_COPY;
		                        section  .ptextkdiv                         PK_COPY;
		                        section  .ptextlucas                        PK_COPY;
		                        section  .ptextmodexp                       PK_COPY;
		                        section  .ptextbigerrno                     PK_COPY;
		                        section  .ptextbigdefs                      PK_COPY;
		                        section  .ptextpkcrypto                     PK_COPY;
		                        section  .ptextkinvert                      PK_COPY;
		                        section  .ptextmpmul22                      PK_COPY;
		                        section  .ptextbignum                       PK_COPY;
		                        section  .ptextmodmulch1                    PK_COPY;
		                        section  .ptextecurve                       PK_COPY;
		                        section  .ptextkinitpr                      PK_COPY;
		                        section  .ptextmodular                      PK_COPY;
		                        section  .ptextfield                        PK_COPY;
		                        section  .ptextdivide1                      PK_COPY;
		                        section  .ptextmpaddsubcmp                  PK_COPY;
		                        label e_Pkcrypto_P;
		                    }
		                }
					}
				}
                section .pWmaWrapdummy;
				overlay{
					contiguous{
                        label b_drmhdsimplsec_p;
	                    section .ptextdrmhds_impl                   WMDRM_COPY;
	                    section .ptextdrmmd5                        WMDRM_COPY;
                        label e_drmhdsimplsec_p;
		                label b_hdsimplcommon_P;
		                section .ptextdrmsha1                       WMDRM_COPY;
		                label e_hdsimplcommon_P;
		                label b_wmasupport_P;
		                section .ptextdrmstkalloc         WMDRM_COPY;
		                section .pWMDRMBitManipulation    WMDRM_COPY;
		                section .ptextgetinfo             WMDRM_COPY;
		                section .ptextoemtime			  WMDRM_COPY;
		                section .ptextdrmint64_common     WMDRM_COPY;
		                section .ptextwmdrmsupportfunc    WMDRM_COPY;
		                section .psysloadDoplay           WMDRM_COPY;
		                section .ptextjanusdrmstubs       WMDRM_COPY;
		                section .poemfile                 WMDRM_COPY;
		                section .ptextwmdrmsupport        WMDRM_COPY;
		                section .psysloadTables           WMDRM_COPY;
		                section .ptextoemfileio           WMDRM_COPY;
		                section .ptextdecryptdata         WMDRM_COPY;
		                section .paes_dec_blk             WMDRM_COPY;
		                section .ptextOptfunc             WMDRM_COPY;
		                section .pRTC_asm	              WMDRM_COPY;				
		                label e_wmasupport_P;
	            	}
					contiguous{
                        label b_Pkcrypto_Ovl_P;
                        section  .ptextmpgcdex                      PK_COPY;
                        section  .ptextecexpon                      PK_COPY;
                        label e_Pkcrypto_Ovl_P;
     					label b_DecryptGPK_P;
						section .ptextdecrypt_gpk				  PK_COPY;
     					label e_DecryptGPK_P;
					}
				}
                overlay
                {
                    // these commented lines are to have the database store in player for abstract playlists (not needed for sdk3.11 yet).
//					contiguous
//					{
//						label b_stor_enum_P;
//						section .ptextststoreenum                          MP3_COPY;
//						label e_stor_enum_P;
//					}
                    contiguous
                    {
                        label b_MusicLibBuild_P;
                        label b_MusicLibMerge_P;
                        section .ptextmusiclib_basic_function_common MP3_COPY;
                        overlay
                        {
        			        contiguous
        			        {
                        		section .ptextmusiclib_build_function MP3_COPY;
                				section .ptextmusiclib_build_function_flash MP3_COPY;
                				section .ptextplaylist3init_static MP3_COPY;
                        		label e_MusicLibBuild_P;
                    		}
                    		contiguous
                    		{                        		
                        		section .ptextmusiclib_merge_function MP3_COPY;
                				section .ptextmusiclib_merge_function_flash MP3_COPY;
                        		label e_MusicLibMerge_P;
                    		}
						}
					}
                    contiguous
                    {
                        label b_EncoderCommon_P;
                        section .pAdcDrv                                    WMA_COPY;  // Add Adc driver here.
                        section SilenceProcess                              WMA_COPY;
                        section .ptextdetectsilence                         WMA_COPY;
                        label e_EncoderCommon_P;

                        label b_ADPCM_P;
                        section .ptextwave                                  WMA_COPY;
                        section ADPCM_Wave_BitStream_P                      WMA_COPY;
                        overlay
                        {
                            contiguous
                            {
                                overlay
                                {
                                    contiguous
                                    {
                                        section DECAPP_DecAdpcmImaNonOverlay_P      WMA_COPY;
                                        section .ptextwavedec                       WMA_COPY;
                                        section Decode_IMA_ADPCM_Block_P            WMA_COPY;
                                        section Decode_MS_ADPCM_Block_P             WMA_COPY;
                                        section Decode_PCM_Block_P                  WMA_COPY;
                                        section ADPCM_BitStream_Decoder_P           WMA_COPY;
                                        label e_ADPCM_Dec_P;
                                    }
                                    contiguous
                                    {
                                        section ENCAPP_EncAdpcmImaNonOverlay_P      WMA_COPY;
                                        section Encode_IMA_ADPCM_Block_P            WMA_COPY;
                                        section Encode_MSADPCM_Block_P              WMA_COPY;
                                        section Encode_PCM_Block_P                  WMA_COPY;
                                        section ADPCM_BitStream_Encoder_P           WMA_COPY;
                                        section .ptextwaveenc                       WMA_COPY;
                                        label e_ADPCM_Enc_P;

                                    }
                                    contiguous
                                    {
                                        label b_MusicLibVoice_P;
                                        section .ptextmusiclib_voice_function MP3_COPY;
										section .ptextmusiclib_voice_function_basic MP3_COPY;
										section .ptextmusiclib_voice_function_flash MP3_COPY;
                                        label e_MusicLibVoice_P;
                                    }

                                }
                                contiguous
                                {
                                    // steely dan decoder
                                    label b_STFM_P;
                                    section DECAPP_DecStfmNonOverlay_P          WMA_COPY;
                                    section Decode_STFM_Block_P                 WMA_COPY;
                                    label e_STFM_Dec_P;
                                    contiguous    
                                    {
                                        //   we can use the memory in the app space for STFM1000 in order to reduce
                                        //NAND access in the FM mode to improve the sensitivity
                                        label b_FM_TUNER_SECOND_RSRC_P_buf;
                                        section .pSTFM1000_NO_OVERLAY_P       WMA_COPY;
                                        label e_FM_TUNER_SECOND_RSRC_P_buf;
                                    }
                                }
                            }

                            contiguous
                            {
                                label b_MP3ENC_P_buf;
                                section  ENCAPP_EncMp3NonOverlay_P          MP3ENC_COPY;
                                section .pMp3EncCacl_GranualEnergy          MP3ENC_COPY;
                                section .pMp3EncFind_ABS                    MP3ENC_COPY;
                                section .pMp3EncNegCoef                     MP3ENC_COPY;
                                section .ptextformatbitstream               MP3ENC_COPY;
                                section .pMp3EncResvFrameBegin              MP3ENC_COPY;
                                section .pMp3EncCalc_scfsi                  MP3ENC_COPY;
                                section .ptextinitializegetbitstream        MP3ENC_COPY;
                                section .pMp3EncLoop_Break                  MP3ENC_COPY;
                                section .pMp3EncSTMP_x3_by_4                MP3ENC_COPY;
                                section .pMp3EncSTMP_x4_by_3                MP3ENC_COPY;
                                section .pMp3EncResvMaxBits                 MP3ENC_COPY;
                                section .pMp3EncInner_Loop                  MP3ENC_COPY;
                                section .pGetBits                           MP3ENC_COPY;
                                section .ptextinitializeputbitstream        MP3ENC_COPY;
                                section .pMp3EncSubbandAnalysis             MP3ENC_COPY;
                                section .pMp3EncBin_Search_StepSize         MP3ENC_COPY;
                                section .pMp3EncPart2Length                 MP3ENC_COPY;
                                section .pPutBits                           MP3ENC_COPY;
                                section .pMp3EncResvFrameEnd                MP3ENC_COPY;
                                section .pMp3EncMdct                        MP3ENC_COPY;
                                section .pMp3EncSubbandAnalysisLoop         MP3ENC_COPY;
                                section .pMp3EncSubDivide                   MP3ENC_COPY;
                                section .pMp3EncScale_BitCount              MP3ENC_COPY;
                                section .ptextinitbitstream                 MP3ENC_COPY;
                                section .pMp3Encbigv_tab_select_count       MP3ENC_COPY;

                                section .ptextcomputemaindatabegin          MP3ENC_COPY;
                                section .pMp3EncHuffmanCode                 MP3ENC_COPY;
                                section .pMp3EncCalcrunlen                  MP3ENC_COPY;
                                section .pMp3EncPreemphasis                 MP3ENC_COPY;
                                section .pMp3EncMdct_Sub                    MP3ENC_COPY;
                                section .pMp3EncChooseTable                 MP3ENC_COPY;
                                section .pMp3EncCountBits                   MP3ENC_COPY;
                                section .pMp3EncOuter_Loop                  MP3ENC_COPY;
                                section .pMp3EncAmp_Scalefac_Bands          MP3ENC_COPY;
                                section .pMp3EncCalc_noise                  MP3ENC_COPY;
                                section .ptextcopyhuffmancodedbits          MP3ENC_COPY;
                                section .pMp3EncQuantize                    MP3ENC_COPY;
                                section .ptextencodeaudio                   MP3ENC_COPY;
                                section .ptextpackframe                     MP3ENC_COPY;
                                section .ptextparseparameterstoheader       MP3ENC_COPY;
                                section .ptextinitmp3encoder                MP3ENC_COPY;
                                section .pMp3EncHuffmanCodeBits             MP3ENC_COPY;
                                section .ptextiterationloop                 MP3ENC_COPY;
                                section .ptextencodesideinfo                MP3ENC_COPY;
                                section .ptextencodemaindata                MP3ENC_COPY;
                                label e_MP3ENC_P_buf;
                            }
                            contiguous
                            {
                                //  Checkdisk
                                label b_CheckdiskOvl;
                                section .ptextchkdsk                        WMA_COPY;
                                section .ptextcwrapfunc                     WMA_COPY;
                                section .ptextfatutils                      WMA_COPY;
                                section .ptextbitio                         WMA_COPY;
                                label e_CheckdiskOvl;
                            }
                            contiguous
                            {
                                label b_AES_ENC_P;
                                section .paes_dec_key                       WMDRM_COPY;
                                section .paes_enc_key                       WMDRM_COPY;
                                section .paes_enc_blk                       WMDRM_COPY;
                                section .ptextencdata                       WMDRM_COPY;
                                label e_AES_ENC_P;

                                label b_janus_p;
                                section .ptextjanus_init                    WMDRM_COPY2;
                                section .ptextgetseed                       WMDRM_COPY2;
                                section .ptextfsinitjanus                   WMDRM_COPY2;
                                section .ppseudo_shuffle                    WMDRM_COPY2;
                                section .ptextpseudo                        WMDRM_COPY2;
                                section .pInitADC                           WMDRM_COPY2;
                                section JanusInit_p                         WMDRM_COPY2;
                                label e_janus_p;
                            }
                            // These 3 sections are used to restore the resource.bin file when
                            // it gets corrupted.  The restoredrivefromback up code reboots
                            // after it complets so it does not matter where this section
                            // get placed.
                            //
                           contiguous
                           {
                               label bRestoreSysDrive_P;
                               section .ptextnandsystemdrivewritesector            MP3_COPY;
                               section .ptextrestoredrivefrombackup                MP3_COPY;
                               section .ptextnandsystemdriveerase                  MP3_COPY;
                               label eRestoreSysDrive_P;
                           }
                        } // Overlay portions of encoders/decoders
                    } // contiguous portions of encoders/decoders
                } //overlay portions of st-store and encoder/decoders
            }  // Contiguous adding fatwrite and Common ADPCM portions
        }  // Encoder/writefat overlay

        overlay addr=0x4000
        {
            section ENCAPP_EncMp3Mem_mod_P;
        } // mp3 encoder.
        overlay addr=INT_P_VECTOR_SZ {
            contiguous {
                label b_ACELP_P;
                section DECMODACELP_DecoderNonOverlay_P MP3_COPY;
                section .ptextaudibleacelpdec_c MP3_COPY;
                section .ptextaudcstandallocation MP3_COPY;
                section .ptextplaybackposition MP3_COPY;
                section .ptextaafiledecode MP3_COPY;
                section .ptextaafileaccessimpl MP3_COPY;
                section .ptextinputstreamimplcstand MP3_COPY;
                section .ptextaudcstandimpgeneric MP3_COPY;
                section .ptextdecryptheader MP3_COPY;
                section .ptextcircularbuffer MP3_COPY;
                section .ptextaaopenfile MP3_COPY;
                section .ptextaafileaccess_nav MP3_COPY;
                section .ptextacelpcodecimpl MP3_COPY;
                section .ptextaafilemischelpers MP3_COPY;
                section .ptextcoreencryption MP3_COPY;
                section .ptextfillaudiblefileinfo MP3_COPY;
                section .ptextreadaafilesectionsheader MP3_COPY;
                section .ptextconv2416 MP3_COPY;
                section .ptextaagetaudio MP3_COPY;
                section .ptextutil		 MP3_COPY;
                section .ptextreorderw	 MP3_COPY;
                section .ptextscale  	 MP3_COPY;
                section .ptextrandom 	 MP3_COPY;
                section .ptextpit_shrp	 MP3_COPY;
                section .ptextweight_a	 MP3_COPY;
                section .ptextint_lpcw	 MP3_COPY;
                section .ptextpred_ltw	 MP3_COPY;
                section .ptextpow2		 MP3_COPY;
                section .ptextreorder	 MP3_COPY;
                section .ptextfind_exc	 MP3_COPY;
                section .ptextsynflt32	 MP3_COPY;
                section .ptextpred_lt	 MP3_COPY;
                section .ptextlog2		 MP3_COPY;
                section .ptextint_lpc	 MP3_COPY;
                section .ptextinv_sqrt	 MP3_COPY;
                section .ptextfind_f	 MP3_COPY;
                section .ptextresidu	 MP3_COPY;
                section .ptextcopybits	 MP3_COPY;
                section .ptextsyn_filt	 MP3_COPY;
                section .ptextsyn_flt2	 MP3_COPY;
                section .ptextpreemph5	 MP3_COPY;
                section .ptextbasic_op	 MP3_COPY;
                section .ptextdec_lag3	 MP3_COPY;
                section .ptextdec45_85	 MP3_COPY;
                section .ptextharmmcode	 MP3_COPY;
                section .ptexthp_2nd	 MP3_COPY;
                section .ptextexcfilt	 MP3_COPY;
                section .ptextpreemph	 MP3_COPY;
                section .ptextd_acb5	 MP3_COPY;
                section .ptextdlag5		 MP3_COPY;
                section .ptextagc		 MP3_COPY;
                section .ptextd_acb		 MP3_COPY;
                section .ptextdlag85	 MP3_COPY;
                section .ptextd_isp		 MP3_COPY;
                section .ptextisp2a		 MP3_COPY;
                section .ptextd_gains	 MP3_COPY;
                section .ptextd_gain2	 MP3_COPY;
                section .ptextdec_swt	 MP3_COPY;
                section .ptextacelpdec_	 MP3_COPY;
                section .ptextdec_5k	 MP3_COPY;
                section .ptextd_isp_5	 MP3_COPY;
                section .ptextdec_wb	 MP3_COPY;
                label e_ACELP_P;
            }
        }
        overlay addr=INT_P_VECTOR_SZ
        {
            //  WMA decoder

            contiguous
            {
                label b_DrmpdCommon_P;
                section .pw002                              DRM_COPY;
                section .pw011                              DRM_COPY;
                section .pw012                                      DRM_COPY;
                section .pw014                                      DRM_COPY;
                section .pw015                                      DRM_COPY;
                section .pw018                                      DRM_COPY;
                section .pdrmpdWrap                         DRM_COPY;
                label e_DrmpdCommon_P;
                overlay
                {
                    contiguous
                    {   //  drmpd lib section
                        label b_WmaLicenseInit;
                        section .ptextwmafilelicenseinit            DRM_COPY;
                        section .pw001                              DRM_COPY;
                        section .pw003                              DRM_COPY;
                        section .pw004                              DRM_COPY;
                        section .pw005                              DRM_COPY;
                        section .pw006                              DRM_COPY;
                        section .pw007                              DRM_COPY;
                        section .pw008                              DRM_COPY;
                        section .pw009                              DRM_COPY;
                        section .pw010                              DRM_COPY;
                        section .pw013                              DRM_COPY;
                        section .pw016                              DRM_COPY;
                        section .pw017                              DRM_COPY;
                        label e_WmaLicenseInit;
                    }
                    contiguous
                    {   // wma decoder section
                        label b_WMANonOvl;
                        section DECMODWMA_DecoderNonOverlay_P       WMA_COPY;
                        section DECWMAMOD_WMADec_P                  WMA_COPY;
                        section .dacdrv                             WMA_COPY;
                        section .pCommon                            WMA_COPY;
                        label e_WMANonOvl;
                        overlay
                        {
                            section .pInit                          WMA_COPY;
                            section .pCore                          WMA_COPY;
                        }
                        // Following Overlay arrangement for min overall WMA code memory.
                        section .pMidLow WMA_COPY;
                        overlay
                        {
                            contiguous
                            {
                                section .pLow WMA_COPY;
                            }
                            contiguous
                            {
                                section .pHighMid WMA_COPY;
                                overlay
                                {
                                    section .pHigh WMA_COPY;
                                    section .pMid WMA_COPY;
                                }
                            }
                        }
                    }
                }
            }
        } // WMA Decoder
        overlay addr=INT_P_VECTOR_SZ
        {   
            contiguous addr=MAX_MP3_WAV_WMA_DECODER_SZ+INT_P_VECTOR_SZ
            {   // The sufficient condition for allocating this segment code is
                // after the Foreground modules
                label b_MusicLibBrowse_P;
                section .ptextmusiclib_browse_app MP3_COPY;
                section .ptextmusiclib_browse_app_basic MP3_COPY;
				section .ptextmusiclib_browse_app_flash MP3_COPY;
                label e_MusicLibBrowse_P;

                // FGndModules
                label b_FGndModules_P;
                section .dacdriver    MP3_COPY;
                //section GraphicEq     MP3_COPY;
                section .FgndExec     MP3_COPY;
                section .Copybuffer   MP3_COPY;
                label e_FGndModules_P;
                section .START        MP3_COPY;
                section .TEXT1        MP3_COPY;
                section .TEXT2        MP3_COPY;
                section .TEXT4        MP3_COPY;
                section .WOW_Process  MP3_COPY;
                label e_srs_FGndModules_P;
            }

//            contiguous addr=MAX_MP3_WAV_WMA_DECODER_SZ+INT_P_VECTOR_SZ
//            {   // The sufficient condition for allocating this segment code is
//                // after the Foreground modules
//                label b_MusicLibBrowse_P;
//                section .ptextmusiclib_browse_app MP3_COPY;
//                section .ptextmlmmc_browse_app MP3_COPY;    // additional sections for MMC musiclib : Feb 5 2007
//                label e_MusicLibBrowse_P;
//
//            }
//            contiguous addr=MAX_ALL_DECODER_SZ+INT_P_VECTOR_SZ
//            {   // FGndModules
//                label b_FGndModules_P;
//                section .dacdriver    MP3_COPY;
//                //section GraphicEq     MP3_COPY;
//                section .FgndExec     MP3_COPY;
//                section .Copybuffer   MP3_COPY;
//                label e_FGndModules_P;
//                section .START        MP3_COPY;
//                section .TEXT1        MP3_COPY;
//                section .TEXT2        MP3_COPY;
//                section .TEXT4        MP3_COPY;
//                section .WOW_Process  MP3_COPY;
//                label e_srs_FGndModules_P;
//            }
        } // Janus
    } // amode far
}  // cluster
