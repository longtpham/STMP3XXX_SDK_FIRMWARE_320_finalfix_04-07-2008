                cluster X_app_cluster 
                {
                    amode X_far 
                    {
                        label bX_app_cluster;
                        label b_MP3_X_buf;
                        overlay 
                        {
							contiguous {
								section .xbssmusiclib_build_function MP3_COPY;
								section .xbsssdmd MP3_COPY;
								section .xdatamusiclib_build_function MP3_COPY;
								label b_MusicLibBuild_X;
                                label b_MusicLibBuildFlushMem_X;
                                overlay
                                {
                                    contiguous
                                    {
                                        section .xbssmusiclib_build_function_flush  MP3_COPY;
                                        label e_MusicLibBuildFlushMem_X;
                                    }
                                    contiguous
                                    {
                                        section .xbssmusiclib_build_genre_table     MP3_COPY;
                                        section .xdatamusiclib_build_genre_table    MP3_COPY;
										section .xbssplaylist3init MP3_COPY;
										section .xdataplaylist3init	MP3_COPY;
                                        label e_MusicLibBuild_X;
                                    }
                                }
							}
				
							contiguous {
								section .xbssmusiclib_voice_function MP3_COPY;
							}
				
							contiguous {
								section .xbssmusiclib_merge_function MP3_COPY;
							}
				
                            contiguous 
                            {
		                        overlay
                                {
		                        section DECAPP_Mp3DecoderMem_X; 
                                    section DECAPP_AudibleMp3DecoderMem_X;
                                }
                                section .xerr_func MP3_COPY;
                                section .xWorkBuffer MP3_COPY;
                                section .xconfig MP3_COPY;
                                section .xtools MP3_COPY;
                                section .xcrc_mpeg MP3_COPY;
                                section .xFMP3_HostFuncTable MP3_COPY;

                                section .xrecfdom MP3_COPY;
                                section .xsectionB2B_asm MP3_COPY;
                                section .xfreqidx MP3_COPY;
                                section .xBitrate_MPEG1 MP3_COPY;
                                section .xBitrate_MPEG2 MP3_COPY;
                                section .xBitrate_MPEG25 MP3_COPY;
                                section .xFReset_to_zero MP3_COPY;
                                section .xexp2infc MP3_COPY;
                                section .xtonectr MP3_COPY;
                                section .xanc_data_x MP3_COPY;
                                section .xl3_core MP3_COPY;
                                section .xchanctl MP3_COPY;
                                section .xinvquant MP3_COPY;
                                section .xl_huffdec MP3_COPY;
                                section .xasm_wrap_x MP3_COPY;
                                section .xinvstero MP3_COPY;
                                section .xhplp_bt MP3_COPY;
                                section .xFMP3_HostParmTable MP3_COPY;
                                section .xdeemph MP3_COPY;
                                section .xl3_head MP3_COPY;
                                section .xmtabs MP3_COPY;
                                section .xinvmdctl MP3_COPY;
                                section .xdec_l3 MP3_COPY;
		                        section .xscalefac MP3_COPY;
		                        section xOVLN MP3_COPY;
		                        section .xsfbwidth MP3_COPY;
		                        section .xipolypha MP3_COPY;
		                        section .xhct__std MP3_COPY;
		                        section .xCopyB2B_asm MP3_COPY;

                                section .xdatalayer2tables   MP3_COPY;
                                section .xBitrate      	     MP3_COPY;
                                section .xdatalayerallo	     MP3_COPY;
                                section .xdatampegmain 	     MP3_COPY;
                                section .xdatacommon   MP3_COPY;
                                section .xdatampegmem  MP3_COPY;
                                section .xdatabitstreams MP3_COPY;
                                section .xdatatables   	 MP3_COPY;

                                section .xdatadec_play 	            MP3_COPY;
                                section .xovlbitstreams	            MP3_COPY;
                                section .xovlcommon    	            MP3_COPY;
                                section .xovldec_play  	            MP3_COPY;
                                section .xovlmpegmain  	            MP3_COPY;
                                section .xovllayer1    	            MP3_COPY;
                                section .xovllayerallo 	            MP3_COPY;
								section DECAPP_Mp3WrapperMem_X		MP3_COPY;
								overlay {
                                    contiguous {
                                section DECAPP_DecoderNonOverlay_X  MP3_COPY;
		                        section DECAPP_Mp3DecoderMem_2_X MP3_COPY;   
		                        section DECAPP_DecoderMem_X MP3_COPY;
                                }

                                        contiguous {
                                        section DECAPP_AudibleDecoderNonOverlay_X	MP3_COPY;
                                        section DECAPP_AudibleMp3DecoderMem_2_X		MP3_COPY;   
                                        section DECAPP_AudibleDecoderMem_X			MP3_COPY;
                                    }
                                }
                                section .xbsslayerallo;
                                section .xbssmpegmem;
                                section .xbssmpegmain;
                                section .xbssdec_play;

                                label e_MP3_X_buf;		                         
                            } // end mp3

                            label b_Jpeg_X;
							contiguous{
							    overlay{
    								contiguous{
        								section .xbssglobals						MP3_COPY;
        								section .xdataglobals						MP3_COPY;
        								section .xdatadjhuff						MP3_COPY;
        								label e_Jpeg_X;
        							}
        							contiguous{
        								section .xbsssmvglobals                     MP3_COPY;
        								section .xdatasmvglobals                    MP3_COPY;
        								section .xdatasmvdjhuff                     MP3_COPY;
        								label e_SmvJpeg_X;
        							}
        						}
								contiguous{
        							label b_SMVADPCM_Dec_X;
    								section SMVADPCM_Tables_X                   MP3_COPY;
    								section DECAPP_DecSmvAdpcmMem_X             MP3_COPY;
    								label e_SMVADPCM_Dec_X;
    								section DECAPP_DecSmvAdpcmMem_mod_X;
    						    }
							}

							contiguous
                            {
								overlay
                                {
									contiguous
                                    {		//MP3 ENCODER
										section ENCAPP_EncMp3Mem_mod_X;
                                        section .xSubbandAnalysisMem;
                                        section .xSubbandAnalysisMem_1;
                                        section .xMp3EncMdct_SubMem;
                                        section .xMp3EncMdctMem;
                                        section .xOuter_LoopMem;
                                        section .xIterationLoopMem;
										label b_MP3ENC_X_buf;
                                        section .xdataiterationloop MP3ENC_COPY;
										label e_MP3ENC_X_buf;                               
                                        section .xbssiterationloop;
                                        section .xbssbitmasktable;
                                        section .xbssencodeaudio;
									}
                                    contiguous
                                    {
                                        label b_ADPCM_X;
                                        section IMAADPCM_Tables_X                       WMA_COPY;
                                        overlay
                                        {
                                            contiguous
                                            { //  ADPCM ENCODER
                                                section ENCAPP_EncAdpcmMem_mod_X;
                                                section ENCAPP_EncAdpcmImaNonOverlay_X  WMA_COPY;                                        
                                                label e_ADPCM_Enc_X;
												    // steely dan decoder
												    section DECAPP_DecStfmMem_mod_X;
												    label b_STFM_X;
                                                section DECAPP_DecStfmMem_X            WMA_COPY;
                                                label e_STFM_Dec_X;

                                            }
                                            contiguous 
                                            {//ADPCM DECODER
                                                section DECAPP_DecAdpcmMem_X            WMA_COPY;
                                                label e_ADPCM_Dec_X;
                                                section DECAPP_DecAdpcmMem_mod_X;
                                            }
                                        }
                                        
                                    }
								}
							    section   AdcmemX   WMA_COPY;

							}



                            contiguous {
                               overlay{
                                 contiguous{
                                 section WmaDecoderMem_mod_X;    // No need to copy this one.
                                    section WMA_X_mem_ovl;
                                    label b_WMA_CONST_X;
                                    section WMA_X_mem WMA_COPY;
                                    section .drmdata WMA_COPY;
                                    section .Midrate WMA_COPY;
                                    section .data WMA_COPY;
                                    label e_WMA_CONST_X;
                                 }
                                 contiguous{
                                    section .xjanusdata; // moved here by AJ in 3.101 and also in 3.111 so we can increase wma out buf sz.                                     
                                    section .xbsswmdrmpdmem;
                                section .xbssdrmliceval WMDRM_COPY;
                                    section .xbssdrmxmlparser WMDRM_COPY;   
                                section .xbssdrmutilities WMDRM_COPY;
                                    section .xconstdrmconstants WMDRM_COPY;
                                    overlay{
                                        section .xbsssecstore_mem;
                                        contiguous {	 //janus section
                                            label b_janus_X_buf;
                                            section .xbssjanus_init WMDRM_COPY2; 
                                            section .xdatajanus_init WMDRM_COPY2; 
		                                    section .xInitADC   WMDRM_COPY2;
                                            label e_janus_X_buf;
                                        }// end Janus section
                                     }
                                  }
                               }
                                section .xdummy1;
                                overlay{
                                    contiguous{
                                        section .rgiCoefReconOrig;
//                                        section WMA_X_mem_ovl;
                                            //section .OtherInputData_ovl;  // moved a few lines down at 3.111 (as in 3.101.013 by AJ)
                                        section .Midrate_ovl;
                                    }
                                    contiguous{
                                        overlay{
	                                        section .OtherInputData_ovl; 
                                            section .xdevecerttemplate WMDRM_COPY;
                                            section .xdevcert WMDRM_COPY;
                                            section .xOverlayBuffer;
                                        }
//                                        section .xjanusdata; // removed from here at 3.111 (as in 3.101.013) see above comment on wma buf sz. Moved up 33 lines earlier.
                                    }
                                }
                                section .xdummy2;
                                overlay{
                                    contiguous{
                                        label b_TABLE_X;
                                        section .xconst DRM_COPY;
                                        label e_TABLE_X;
                                    }
                                    contiguous{
                                        label b_TABLE1_X;
                                            section .xDES_Key WMDRM_COPY;
                                            section .xDES_Key_Setup WMDRM_COPY;
                                        label e_TABLE1_X;
                                    }
                                }
                                section .xdummy3;

                                label b_WMA_X1;
                                section WmaDecoderMem_X WMA_COPY;
                                section .rgcValidBarkBand WMA_COPY;
                                section .rgiMaskQ WMA_COPY;         
                                section .rgiBarkIndexOrig WMA_COPY;
                                section DECWMAMOD_DecoderMem_X WMA_COPY;                                
                                section .licdata WMA_COPY;
                                section .OtherInputData WMA_COPY;
                                section .xdatawmafilelicenseinit WMA_COPY;
                                section .xwata WMA_COPY;
                                label e_WMA_X1;
                                
                                section .xbsswmafilelicenseinit;
                                section .xAesMem;
                                section .xdummy5;                                
                                

                                
                                overlay
                                {
                                    contiguous
                                    {
                                label b_JANUSMEM_X;
                                section .xdatadrmlicparser  WMDRM_COPY;
                                section .xdatabase64table   WMDRM_COPY;
                                section .xconstbase64table  WMDRM_COPY;
                                section .xconstoeminfo  WMDRM_COPY;
                                section .xconstdrmmd5   WMDRM_COPY;
                                label e_JANUSMEM_X;
                                    
                                        section .xbssdevcert_init;
                                        section .xbssdrmmanager_init;
                                        section .xbsswmdrmsupport;
                                        section .xbssjanusdefis;
                                        section .xbssdecryptdata;
                                        section .xbssencdata;
                                section .xbssdrmcrt;
                                        overlay
                                        {
                                            section .xbssdrmexprvariable;    
                                            section .xbssblackbox;    
                                        }
                                        overlay
                                        {
                                            contiguous
                                            {
                                                section .xbssdrmmanager;
                                section .xbssdrmexprfunc;
                                section .xbssdevcert;
                                section .xbssdrmchain;
                                section .xbssdrmexpreval;
                                section .xbssdrmhdrparser;
                                section .xbssdrmviewrights;
                                section .xbssdevicedevcert;
                                section .xbssdrmoutputlevels;
                                section .xbssdrmlicparser;
                                            }
                                            section .xbsspseudo;
                                        }
                                    }
                                    //Place Fgnd Modules and other effects buffers here
                                    contiguous
                                    {
                                            section Dacmem_Mod_X        MP3_COPY;   	
                                            section FgndMemoryX         MP3_COPY;
                                            section GeqFilterStateMem   MP3_COPY;
                                            label  b_Srs_Effects_X;
                                            section wow_local_buffer    WMA_COPY;
                                            label  e_Srs_Effects_X;                                    }
                                }
                            }
                            
				            contiguous {
                                label b_ACELP_X;
                                section DECAPP_AcelpDecoderMem_2_X MP3_COPY;
								section .xbssacelpcodecimpl		MP3_COPY;
								section .xovlpit_shrp    		MP3_COPY;
								section .xdatabasic_op   		MP3_COPY;
								section .xovlcopybits    		MP3_COPY;
								section .xovldlag85      		MP3_COPY;
								section .xovlpred_lt     		MP3_COPY;
								section .xovldec_lag3    		MP3_COPY;
								section .xovldlag5       		MP3_COPY;
								section .xovlinv_sqrt    		MP3_COPY;
								section .xovlpreemph5    		MP3_COPY;
								section .xovlresidu      		MP3_COPY;
								section .xdatad_isp      		MP3_COPY;
								section .xovld_acb       		MP3_COPY;
								section .xovld_acb5      		MP3_COPY;
								section .xovlfind_exc    		MP3_COPY;
								section .xovlharmmcode   		MP3_COPY;
								section .xovllog2        		MP3_COPY;
								section .xovldec45_85    		MP3_COPY;
								section .xovlhp_2nd      		MP3_COPY;
								section .xdatadec_5k     		MP3_COPY;
								section .xdatadec_swt    		MP3_COPY;
								section .xovld_gains     		MP3_COPY;
								section .xovld_gain2     		MP3_COPY;
								section .xovlint_lpc     		MP3_COPY;
								section .xovlsynflt32    		MP3_COPY;
								section .xovld_isp_5     		MP3_COPY;
								section .xovlint_lpcw    		MP3_COPY;
								section .xovlfind_f      		MP3_COPY;
								section .xdatadec_wb     		MP3_COPY;
								section .xdatalog2       		MP3_COPY;
								section .xdatapow2       		MP3_COPY;
								section .xovld_isp       		MP3_COPY;
								section .xdatainv_sqrt   		MP3_COPY;
								section .xovlacelpdec_   		MP3_COPY;
								section .xdatad_gains    		MP3_COPY;
								section .xovlisp2a       		MP3_COPY;
								section .xovlagc         		MP3_COPY;
								section .xdataacelpdec_  		MP3_COPY;
								section .xovlpreemph     		MP3_COPY;
								section .xovldec_wb      		MP3_COPY;
								section .xovldec_swt     		MP3_COPY;
								section .xovldec_5k      		MP3_COPY;
								section .xovlexcfilt     		MP3_COPY;
								section .xdatad_gain2    		MP3_COPY;
								section .xovlsyn_filt    		MP3_COPY;
								section .xovlsyn_flt2    		MP3_COPY;
								section .xdatad_isp_5    		MP3_COPY;
								section .xconstpred_ltw  		MP3_COPY;
								section .xconstd_isp     		MP3_COPY;
                                section .xdatahp_2nd  			MP3_COPY;
								section .xconstpred_lt 			MP3_COPY;
								section .xbssacelpdec_   		MP3_COPY;
                                label e_ACELP_X;
                            }
                            
                            // Checkdisk X overlay
                            contiguous {
								section .XscratchSpace;
                                section .xbsschkdsk;                                
                            }
                        }   // x_app overlay

                        // free space from x_sys (default) by moving this here. sz=0xb0=176 words
                        section .xbssfsddinit;
                        //section .xbsslyrics_static;
                        //section .xdatalyrics_static;
                        section .xbsswow_init;
                        //section .xbssstfm1000; 
                        //section .xbssnandhalmarkblockbadinra; 
                        //section .xbssfmtunermenu; 
                    }  // amode X_FAR
                }   // cluster

