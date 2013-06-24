
                cluster Y_app {
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s(pecial) line!!!!
                    attribute w;//don't locate anything here automatically. (w)ritable
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
                    amode Y_far {
                        fixed addr = INT_Y_SYS_SZ;
                        label bY_app_cluster;
                        contiguous {

                            overlay  {
                                contiguous
                                {
									label b_MusicLibBuild_Y;
                                    label b_MusicLibMerge_Y;
                                    section .ybssmusiclib_basic_function_common MP3_COPY;
                                    overlay
                                    {
                                        contiguous
                                        {
                                            section .ybssmusiclib_build_function MP3_COPY;
                                            section .ybsssdmd MP3_COPY;
        									section .yovlmusiclib_build_function        MP3_COPY;
                                            section .yovlmusiclib_build_function_flash  MP3_COPY;
                                            section .ybssmusiclib_build_genre_table     MP3_COPY;
                                            label e_MusicLibBuild_Y;
								        }
                                        contiguous
                                        {
                                            section .ybssmusiclib_merge_function MP3_COPY;
                                            section .yovlmusiclib_merge_function MP3_COPY;
                                            section .yovlmusiclib_merge_function_flash  MP3_COPY;
                                            label e_MusicLibMerge_Y;
                                        }
                                    }
                                }

								contiguous {
									label b_MusicLibVoice_Y;
									section .ybssmusiclib_voice_function MP3_COPY;
									section .yovlmusiclib_voice_function        MP3_COPY;
                                    section .yovlmusiclib_voice_function_flash  MP3_COPY;
									label e_MusicLibVoice_Y;
								}

                                contiguous {        // WMA Overlay
                                    label b_WMA_Y;
                                    overlay{
                                        contiguous{
                                            section WMA_Y_mem WMA_COPY;
                                            section WmaDecoderMem_Y WMA_COPY;
                                            section .WMA_ParsePayloadHeader WMA_COPY;
                                            section .stackdata WMA_COPY;
                                            section .ydata WMA_COPY;
                                            section .yovl WMA_COPY;
                                            section .WMA_LoadAudioObject WMA_COPY;
                                            section .ybuf WMA_COPY;
                                            section .sincosdata WMA_COPY;
                                            section DECWMAMOD_DecoderMem_Y WMA_COPY;
                                            section .InBuf WMA_COPY;
                                            section .MaskTable WMA_COPY;
                                        }
                                        section .ybsspkcrypto;
                                    }
                                    section .prvGetBandWeightLowRate WMA_COPY;
                                    overlay{
                                        section .piPrevOutput WMA_COPY;
                                        contiguous{
    		                                section .ybssjanus_init;
      		                                section .yovljanus_init;
                                            section .ybssjanusdefis;
    		                                section .yovlpseudo;
    					                    section .ybsspseudo;
									    	section .ybsssecureclockrequest;
                                            section .ybssoemfileio;
                                            section .ybssdevcert;
                                            section .ybssdrmlicstore;
                                            overlay{
                                                section .ybssdrmhds_impl;
                                                section .ybssoemimpl;
                                            }
                                            section .ybssdrmsecstore;
											section  .ybssdrmutilities;
                                            overlay{
                                                section .yovlprivate_key;
                                                contiguous{
                                                    label b_CONST_PKCRYPTO_Y;
                                                    section .yconstpkcrypto WMDRM_COPY;
                                                    section .yconstkinitpr  WMDRM_COPY;
                                                    section .yconstmodexp   WMDRM_COPY;
                                                    section .ydatapkcrypto  WMDRM_COPY;
                                                    label e_CONST_PKCRYPTO_Y;
                                                }
                                            }
                                            section .ybssdrmlicparser;
                                            section .ybssencdata;
                                            section .ybssdecryptdata;
                                            section .ybssbase64table;
                                            section .ybssdrmexpreval;
                                            section .ybssdrmlicparser;
                                            section .ybssdrmviewrights;
                                            section .ybssdrmcrt;
                                            section .ybssdrmxmlparser;
                                            section .yovlfsinitjanus;
                                            section .yovljanusdrmstubs;
                                            section .yovloemtime;
                                            section .yovldrmint64_common;
                                            section .yovldrmint64;
                                            section .yovldecryptdata;
                                            section .yovlencdata;

                                            overlay{
                                                section .ydataaeskeytables  WMDRM_COPY;
                                                section .ydataaesenctables  WMDRM_COPY;
                                                contiguous{
                                                    label b_CONST_Y;
                                                    section .yconstprivate_key         WMDRM_COPY;
                                                    section .yconstdrmsecstore         WMDRM_COPY;
                                                    section .yconstdrmhds              WMDRM_COPY;
                                                    section .yconstdrmoutputlevels     WMDRM_COPY;
                                                    section .yconstbase64table         WMDRM_COPY;
                                                    section .yconstsecureclockrequest  WMDRM_COPY;
                                                    section .yconstdrmlicstore         WMDRM_COPY;
                                                    section .yconstoemtime             WMDRM_COPY;
                                                    section .yconstoemfileio           WMDRM_COPY;
                                                    section .yconstdrmconstants        WMDRM_COPY;
						                            section .yconstjanus_init          WMDRM_COPY;
                                                    section .yconstdrmhds_impl         WMDRM_COPY;
                                                    section .yconstdrmsyncstore        WMDRM_COPY;
                                                    section .yconstpseudo              WMDRM_COPY;
                                                    label e_CONST_Y;

                                                }
                                            }
                                        }
                                    }
                                    section .NoiseSubTables WMA_COPY;
                                    label e_WMA_Y;
                                    section .ybssdrmchain;
                                    overlay{
                                        section .HuffTables;
                                        section .huff16Ob WMA_COPY;
                                        section .huff44Qb WMA_COPY;
                                        section .huff44Ob WMA_COPY;
                                        contiguous{
                                            section .yovlJanus;
                                            section .ydataaesdectables  WMDRM_COPY;
											overlay{
                                                section .ybsswmdrmpdmem;
    											contiguous{
	    											section .ydataoem_data			WMDRM_COPY2;
		    										section .ybssdecrypt_gpk;
												}
											}
                                        }
                                    }

                                    section .yovlblackbox_runtime;
                                    section .yovlrc4;
                                    section .yovlgetinfo;
                                    section .yovldrmcipher_runtime;
                                    section .yovldrmcbc_runtime;

                                    overlay{
                                        section .DRM_DES_Cipher;
                                        section DRM_DES_Cipher;
                                        section .yovlwmdrmsupport;
                                    }
                                    section .ybssjanusutils;
                                    label b_DATA_JANUS_Y;
                                    section .ydatajanus_init        WMDRM_COPY;
                                    section .ydataoeminfo           WMDRM_COPY;
                                    section .ydatadrmutilities      WMDRM_COPY;
                                    section .ydataoemfileio         WMDRM_COPY;
                                    section .ydatawmdrmsupport      WMDRM_COPY;
                                    section .ydatadrmexprfunc       WMDRM_COPY;
                                    section .ydatadrmexprvariable   WMDRM_COPY;
                                    section .ydatadrmhdrparser      WMDRM_COPY;
                                    section .ydatadrmsyncstore      WMDRM_COPY;
                                    section .ydataoemtime           WMDRM_COPY;
                                    section .ydatadrmmanager_init   WMDRM_COPY;
                                    section janusInitWrapper        WMDRM_COPY;
                                    section .yInitADC               WMDRM_COPY;
                                    section .ydatajanusutils        WMDRM_COPY;
                                    section .ydatadrmcbc_runtime    WMDRM_COPY;
                                    label e_DATA_JANUS_Y;
                                }
								//jpeg y memory
    							contiguous{
    								overlay {
    								    contiguous{
    								        label b_Jpeg_Y_1;
            							    section .ybssglobals.Jpeg_Dec_input_buffer_array    MP3_COPY;
            						        section .ybssjpegdecodedisplay.InputBuffer          MP3_COPY;
            								section .yconstjpegdecodedisplay                    MP3_COPY;
            								section .ydataglobals                               MP3_COPY;
            								section .yovldjmain                                 MP3_COPY;
            								section .ybssdjinit                                 MP3_COPY;

            								section .ybssjpegdecodedisplay                      MP3_COPY;
            								section .yovljpegdecodedisplay                      MP3_COPY;
            								section .ydatadjcolor                               MP3_COPY;

            								section .yconstdjmain                       MP3_COPY;

            								section .yconstdjinit                       MP3_COPY;
            								section .yconstglobals                      MP3_COPY;
            								section .ydatadjidct                        MP3_COPY;
            								section .yovldjcommon                       MP3_COPY;
            								section .ydatadjmain                        MP3_COPY;
            								section .ybssglobals                        MP3_COPY;
            								label e_Jpeg_Y_1;
            								label b_Jpeg_Y_2;
            								overlay{
            								    contiguous{
            										section .yovldjinit                 MP3_COPY;
            										section .ydatadjhuff                MP3_COPY;
            										label e_Jpeg_Y_2;
            									}
            									contiguous{
            										section .ybssdjbmp                  MP3_COPY;
            										section .yovldjbmp                  MP3_COPY;
            										label e_BMP_Y_2;
            									}
            								}
            							}
    								    contiguous{
    								        label b_SmvJpeg_Y;
            							    section .ybsssmvglobals.SmvJpeg_Dec_input_buffer_array  MP3_COPY;
            						        section .ybsssmvjpegdecodedisplay.SmvInputBuffer        MP3_COPY;
            								section .yconstsmvjpegdecodedisplay                     MP3_COPY;
            								section .ydatasmvglobals                                MP3_COPY;
            								section .yovlsmvdjmain                                  MP3_COPY;
            								section .ybsssmvdjinit                                  MP3_COPY;

            								section .ybsssmvjpegdecodedisplay                       MP3_COPY;
            								section .yovlsmvjpegdecodedisplay                       MP3_COPY;
            								section .ydatasmvdjcolor                                MP3_COPY;

                                            // ********************************************
                                            // NOTE: Read the notes about this buffer in
                                            // dec_smvadpcm_mem.asm and dec_smvadpcmequ.inc.
                                            // Locating the modulo buffer required some
                                            // fakery, which may break if this moves around
                                            // too much.
                                            // ********************************************
            								section DECAPP_DecSmvAdpcmMem_mod_Y;

            								section DECAPP_DecSmvAdpcmMem_Y                 MP3_COPY;

            					            section .yovlsmvwavedec                         MP3_COPY;

            								section	SMVADPCM_Data_Y                         MP3_COPY;
            	                            section SMV_Bitstream_Wave_Y                    MP3_COPY;
                	                        section .ydatasmvwave                           MP3_COPY;
                    	                    section .ybsssmvwave                            MP3_COPY;

            								section .yconstsmvdjmain                    MP3_COPY;

            								section .yconstsmvdjinit                    MP3_COPY;
            								section .yconstsmvglobals                   MP3_COPY;
            								section .ydatasmvdjidct                     MP3_COPY;
            								section .yovlsmvdjcommon                    MP3_COPY;
            								section .ydatasmvdjmain                     MP3_COPY;
            								section .ybsssmvglobals                     MP3_COPY;
            								section .yovlsmvdjinit                      MP3_COPY;
            								section .ydatasmvdjhuff                     MP3_COPY;
            								label e_SmvJpeg_Y;
            							}
            						}
    							}

							    contiguous{
								    overlay{
                                        //contiguous
                                        //{//STFM DECODER SPECIFIC
                                        //    label b_STFM_Y;
                                            // ********************************************
                                            // NOTE: Read the notes about this buffer in
                                            // dec_stfm_mem.asm and dec_stfmequ.inc.
                                            // Locating the modulo buffer required some
                                            // fakery, which may break if this moves around
                                            // too much.
                                            // ********************************************
                                        //}
                                        contiguous
                                        { //COMMON STUFF BETWEEN ADPCM ENCODER AND DEDODER
                                            label b_ADPCM_Y;
                                            section IMAADPCM_Data_Y                             WMA_COPY;
                                            section MSADPCM_Mem_Y                               WMA_COPY;
                                            section Bitstream_Wave_Y                            WMA_COPY;
                                            section .ydatawave                                  WMA_COPY;
                                            section .ybsswave                                   WMA_COPY;
                                            overlay
                                            {
                                                contiguous
                                                { //  ADPCM ENCODER SPECIFIC
                                                    section ENCAPP_EncAdpcmMem_Y                WMA_COPY;
                                                    section ENCAPP_EncAdpcmImaNonOverlay_Y      WMA_COPY;
                                                    section .yovlwaveenc                        WMA_COPY;
                                                    label e_ADPCM_Enc_Y;
                                                    label b_STFM_Y;
                                                    section DECAPP_DecStfmMem_Y                WMA_COPY;
                                                    label e_STFM_Dec_Y;
                                                    section ENCAPP_EncAdpcmMem_mod_Y;
                                                    section DECAPP_DecStfmMem_mod_Y;
                                                }

                                                contiguous
                                                {//ADPCM DECODER SPECIFIC
                                                    // ********************************************
                                                    // NOTE: Read the notes about this buffer in
                                                    // dec_adpcm_mem.asm and dec_adpcmequ.inc.
                                                    // Locating the modulo buffer required some
                                                    // fakery, which may break if this moves around
                                                    // too much.
                                                    // ********************************************
                                                    section DECAPP_DecAdpcmMem_mod_Y;

                                                    section DECAPP_DecAdpcmMem_Y                WMA_COPY;
                                                    section .yovlwavedec                        WMA_COPY;
                                                    //section DECADPCMMOD_DecAdpcmMem_2_Y MP3_COPY;

                                                    label e_ADPCM_Dec_Y;
                                                }
                                            }
                                        }

									    contiguous{		//MP3 ENCODER
                                            section ENCAPP_EncMp3Mem_mod_Y;
                                            section .yMp3EncFftMem;

                                            label b_MP3ENC_Y_buf;
                                            section ENCAPP_EncMp3Mem_Y              MP3ENC_COPY;
                                            section .yMp3EncChooseTableMem          MP3ENC_COPY;
                                            section .ydataencodeaudio               MP3ENC_COPY;
                                            section .ydataiterationloop             MP3ENC_COPY;
                                            section .yovlformatbitstream            MP3ENC_COPY;
                                            section .yovlinitbitstream              MP3ENC_COPY;
                                            section .yIterationLoopMem              MP3ENC_COPY;
                                            section .ydataformatbitstream           MP3ENC_COPY;
                                            section .yovlencodeaudio                MP3ENC_COPY;
                                            section .yovlinitializegetbitstream     MP3ENC_COPY;
                                            section .yovlinitializeputbitstream     MP3ENC_COPY;
                                            section .ydatamp3mem                    MP3ENC_COPY;
                                            section .yovlcomputemaindatabegin       MP3ENC_COPY;
                                            section .yovlparseparameterstoheader    MP3ENC_COPY;
                                            section .yovlpackframe                  MP3ENC_COPY;
                                            section .yovlencodemaindata             MP3ENC_COPY;
                                            section .yovlinitmp3encoder             MP3ENC_COPY;
                                            section .yovliterationloop              MP3ENC_COPY;
                                            section .yovlencodesideinfo             MP3ENC_COPY;
                                            section .yovlcopyhuffmancodedbits       MP3ENC_COPY;
                                            section .yOuter_Loop                    MP3ENC_COPY;
                                            section .yMp3EncHuffManCodeBitsMem      MP3ENC_COPY;
                                            section .yMp3EncTables                  MP3ENC_COPY;
                                            section .ydatabitmasktable              MP3ENC_COPY;
                                            section .ydatamp3table                  MP3ENC_COPY;
                                            section .ydatahufftables                MP3ENC_COPY;
										    label e_MP3ENC_Y_buf;

                                            section .ybsspackframe;
                                            section .ybssiterationloop;
                                            section .ybssencodeaudio;
                                            section .ybssformatbitstream;
                                            section .ybssmp3mem;
                                            section .ybssmp3encoderram;
                                            label b_ADC_Y;
									    }
					    			}
								    section   AdcmemY   WMA_COPY;
							    }

                                contiguous{
                                    overlay {
									    section DECAPP_Mp3DecoderMem_mod_Y ;
                                        section DECAPP_AudibleMp3DecoderMem_mod_Y ;
                                    }
                                    section .ybssmpegmem;
	                                label b_MP3_Y_buf;
	                                section .yinvmdctl MP3_COPY;
                                    section .yiployphal_m MP3_COPY;
                                    section .yiployphar_m MP3_COPY;
			                        section .yihybrid MP3_COPY;
			                        section .yx43tab MP3_COPY;
			                        section .ychanctl MP3_COPY;
			                        section yOVLN MP3_COPY;
			                        section .yinvmdctl_2 MP3_COPY;
	                                section .ydatalayer2tables    MP3_COPY;
	                                section .ydatatables   	      MP3_COPY;
                                    contiguous
                                    {
                                        overlay
                                        {
                                            contiguous {
			                                    section DECAPP_Mp3DecoderMem_Y MP3_COPY;
			                                    section DECAPP_DecoderMem_Y MP3_COPY;
                                            }

                                            contiguous {
                                                section DECAPP_AudibleMp3DecoderMem_Y	MP3_COPY;
                                                section DECAPP_AudibleDecoderMem_Y		MP3_COPY;
                                                section .ybssaudiblemp3dec_c			MP3_COPY;
                                                section .yconstaudiblemp3overlay MP3_COPY;
                                                section .ydataaudiblemp3overlay MP3_COPY;
                                                section .ydataaudiblemp3common MP3_COPY;
                                                section .ydataaudiblemp3dec_c MP3_COPY;
                                                section .yovlaudiblemp3dec_c MP3_COPY;
                                                section .yovlaudiblemp3common MP3_COPY;
                                                section .yovlaudiblemp3nonoverlay MP3_COPY;
                                                section .yovlaudiblemp3overlay MP3_COPY;
                                            }
                                        }
                                    }
	                                label e_MP3_Y_buf;
                                }
                                contiguous {
                                    label b_ACELP_Y;
                                    section DECAPP_AcelpDecoderMem_mod_Y MP3_COPY;
                                    section DECAPP_AcelpDecoderMem_Y MP3_COPY;
                                    section DECACELPMOD_DecoderMem_Y MP3_COPY;
                                    section .ybssaudibleacelpdec_c MP3_COPY;
                                    section .yovlconv2416 MP3_COPY;
                                    section .yovlinputstreamimplcstand MP3_COPY;
                                    section .yovlacelpcodecimpl MP3_COPY;
                                    section .yovlaudcstandallocation MP3_COPY;
                                    section .ydataaudcstandallocation MP3_COPY;
                                    section .ydatainputstreamimplcstand MP3_COPY;
                                    section .yovlaaopenfile MP3_COPY;
                                    section .yovlaudcstandimpgeneric MP3_COPY;
                                    section .yovlreadaafilesectionsheader MP3_COPY;
                                    section .yovlfillaudiblefileinfo MP3_COPY;
                                    section .yovlcircularbuffer MP3_COPY;
                                    section .yovlaafilemischelpers MP3_COPY;
                                    section .yovlaafileaccess_nav MP3_COPY;
                                    section .ydataconv2416 MP3_COPY;
                                    section .ydataacelpcodecimpl MP3_COPY;
                                    section .yovlcoreencryption MP3_COPY;
                                    section .yovlaafiledecode MP3_COPY;
                                    section .yovlaagetaudio MP3_COPY;
                                    section .yovlaafileaccessimpl MP3_COPY;
                                    section .yovldecryptheader MP3_COPY;
                                    section .yconstfillaudiblefileinfo MP3_COPY;
                                    section .yconstinputstreamimplcstand MP3_COPY;
                                    section .ydataaudibleacelpdec_c MP3_COPY;
                                    section .ydataaafilemischelpers MP3_COPY;
                                    section .yovlaudibleacelpdec_c MP3_COPY;
                                    label e_ACELP_Y;
                                }

                                contiguous{
	                                label b_chkdsk_Y_buf;
                                    section .yovlchkdsk WMA_COPY;
			                        section .ydatachkdsk WMA_COPY;
									section .ydatabitio WMA_COPY;
	                                label e_chkdsk_Y_buf;
	                                section .ybsschkdsk;
                                }
                                contiguous{
                                    section .ybssaudiblemetadata;
                                    section .ybssaudiblesongposition;
                                    section .ybsschecktargeting;
                                    section .ybsssha;
                                    label b_AUDIBLE_METADATA_Y;
//									section .ydataaudiblemetadata		MP3_COPY;
                                    section .yconstkeys					MP3_COPY;
                                    section .yconstdsaverify			MP3_COPY;
                                    section .yovlaudiblesongposition	MP3_COPY;
                                    section .yovlronsdsa				MP3_COPY;
                                    section .yovlsha					MP3_COPY;
                                    section .yovldsaverify				MP3_COPY;
                                    section .ydatametafillaudiblefileinfo	MP3_COPY;
                                    section .yovlmetafillaudiblefileinfo	MP3_COPY;
                                    section .yovlactivationrecords MP3_COPY;
                                    section .yconstaaactivationrecords	MP3_COPY;
                                    section .yconstaaactivationfilename	MP3_COPY;
                                    label e_AUDIBLE_METADATA_Y;
                                }
								section .ybssplaylist2sortmem;
                            }

                            contiguous{
                                label b_wow_coeff_Y;
                                overlay{
                                    section .yconstwow11k WMA_COPY;
                                    section .yconstwow16k WMA_COPY;
                                    section .yconstwow24k WMA_COPY;
                                    section .yconstwow44k WMA_COPY;
                                    section .yconstwow48k WMA_COPY;
                                    section .yconstwow8k  WMA_COPY;
                                    section .yconstwow22k WMA_COPY;
                                    section .yconstwow32k WMA_COPY;
                                }
                            }
                            label e_wow_coeff_Y;

                            section .ydatafsutilsc; //here this section is for getting offset.don't remove this section from here.
							overlay{
                            	contiguous{
                                    section .ymmcoverlayscratch; // Moved these here to make room in y_extra for increased BBTable for MLC. Don't put ybssmmcmediainit here.
									//section .ybssplayerlib;
									//section .ybsskeydefis;
                                    //section .ybssplaylist2mem;
								}
								contiguous{
									//section .ydataoem_data			WMDRM_COPY2;
									section .yovldecrypt_gpk;
									section .ydatast_priv			WMDRM_COPY2;

									//section .ybssdecrypt_gpk;
								}
							}
                        }
                        label eY_app_cluster;
						//The following are really system sections. i.e they never get swapped out
						// VERY IMPORTANT: must be put under eY_app_cluster or else
						// the L2 table will be erased by SysFlushCoderMemory
						section .ybssmusiclib_browse_app;
						section .ybssmlmmc_browse_app;    // additional sections for MMC musiclib : Feb 5 2007
						section .ybssbrowsemenu;
						section SYSPLAYER_MsgQueue_MusicLib_Play_Y; //This is really a system section. i.e they never get swapped out. Placing here becaues y memory is not easily re-mapped.
                    }
                }


                cluster Y_Sys2 { //here's 1k of memory we need to fill with something.
                    amode Y_far {
                        section .yconstnandsystemdrivereadsector;
						section GeqCoefmem;
                        section FgndModuleTable;
                        //section .ybssphilips5767;
                        section .ydataaudiblemetadata;
                        section SYSFUN_Brownout_Y;
                        //section .ybssplaylist2;   //move to Y_EXTRA
                        //section .ydataplaylist2;  //move to Y_EXTRA
//                        section .ybsskeydefis;
						section .ybssddiproject;
 						section .ydataddiproject;
						section .yovlsteppingvoltageset;
//						section .ydataframebuffer;
//						section .ydatanandhaltype1descriptors;
//						section .ybssplayerlib;
						section .ybsssetcwdhandle;
                        section .yconstdisplay;
                        section .yconstfsutilsc;
                        section .yconstvbr;
                        section .ydatamicbias;
                        section .ybsssettimemenu;
                        section .ybssdetectsilence;
                        section .yovlstfm1000testpresence;
                        fixed addr = INT_LTABLES_ORG;

                    }
                }










