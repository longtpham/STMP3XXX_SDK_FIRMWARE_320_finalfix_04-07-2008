///////////////////////////////////////////////////////////////////////////////
//!
//! \file P_MtpCluster.dsc
//! \brief 
//!   This file describes the memory layout of the MTP application cluster.
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
cluster P_MTP_cluster
{
    amode P_far 
    {
        contiguous addr=INT_P_MTP_ORG 
        {
            overlay{
		            contiguous 
					{
			            label b_MMD5_DRM_SHA1;
	                    section  .ptextdrmmd5              TASK_COPY;
	                    section  .ptextdrmsha1          TASK_COPY;
			            label e_MMD5_DRM_SHA1;
					}
		            contiguous 
					{
			            label MTPOVL_b;
						section .ptextstobjects			TASK_COPY;
						section .ptextdatabase			TASK_COPY;
						section .ptextdbgetstring		TASK_COPY;
			            label MTPOVL_e;
					}
			}
			section .ptextprivate_key;
            label MTPRegion_b;
            contiguous 
            {
                overlay 
                {
                    contiguous 
                    {
                        label b_JanusSupport_P;
                        section .psysloadTables	       TASK_COPY;
                        section .pBitManip_Write       TASK_COPY;
						section .ptextMTPOptfunc  	   TASK_COPY;	
	                    section .pRTC_asm      	       TASK_COPY;
	                    section .pWMDRMBitManipulation TASK_COPY;                         
	                    section .ptextgetinfo          TASK_COPY;
	                    section .poemfile              TASK_COPY;  
	                    section .ptextoemtime          TASK_COPY;  
	                    section .ptextdrmint64_common  TASK_COPY;
                        label e_JanusSupport_P;
						overlay{
		    	            section .ptextdrmhds_impl      TASK_COPY;
							contiguous{
								label b_AES_DEC_P;
//		                        section .ptextprivate_key      TASK_COPY;
                        section .paes_dec_key1         TASK_COPY;
                        section .paes_enc_key1         TASK_COPY;
//		                        section .ptextMtp_Initfunc     TASK_COPY;
                        section .ptextfsinitjanus      TASK_COPY;
                        section .ptextjanus_mtp_init   TASK_COPY;
                        section .ptextdecryptdata      TASK_COPY;
                        section .paes_dec_blk          TASK_COPY;
								label e_AES_DEC_P;
							}
						}
                    }
                    contiguous
                    {
                        section .ptextaamtpaccess                   TASK_COPY;
                        section .ptextaamtpsongposition             TASK_COPY;                        
                        overlay
                        {
                            section .ptextmtp_audiblemetadata           TASK_COPY;
                            section .ptextaudibleobjectprophandler      TASK_COPY;
                        }
                    }
                    contiguous
                    {
                        section .ptextaaactivationrecords			TASK_COPY;
                        section .ptextaudiblesetdevicepropvalue     TASK_COPY;
                        section .ptextaudibledevicepropertyhandler  TASK_COPY;
                    }
                    section .ptextmtp_metadata                      TASK_COPY;
                    section .ptextmtp_mp3metadata                   TASK_COPY;
                    section .ptextmtp_wmametadata                   TASK_COPY;
                    section .ptextmtp_wavmetadata                   TASK_COPY;
                    section .ptextid3v2                             TASK_COPY;
                    section .ptextmtp_vbr                           TASK_COPY;
                    section .ptextmtpinitdevice                     TASK_COPY;
                    section .ptextmtpjanuscleandatastore            TASK_COPY;
                    section .ptextmtpjanusgetmeterchallenge         TASK_COPY;
                    section .ptextmtpjanusgetsecuretimechallenge    TASK_COPY;
                    section .ptextmtpjanusgetsynclist               TASK_COPY;
                    section .ptextmtpjanussendmeterchallengequery   TASK_COPY;
                    section .ptextmtpjanussetdevcertresponse        TASK_COPY;
                    section .ptextmtpjanussetlicenseresponse        TASK_COPY;
                    section .ptextmtpjanussetmeterresponse          TASK_COPY;
                    section .ptextmtpjanussetsecuretimeresponse     TASK_COPY;
                    section .ptextmtpjanusgetdevcertchallenge       TASK_COPY;
                    section .ptextmtpjanusgetlicensestate           TASK_COPY;
                    section .ptextmtpjanussendwmdrmpdcommand        TASK_COPY;
                    section .ptextmtpjanussendwmdrmpdrequest        TASK_COPY;
                    section .ptextmtpformatstore                    TASK_COPY;
				    // This section is used to restore the hostrsrc.bin file when
			        // it gets corrupted.
			        section .ptextrestoredrivefrombackupmtp                TASK_COPY;

                    contiguous
                    {
                        contiguous
                        {
                            section .ptextststore                       TASK_COPY;
                            section .pdataststore                       TASK_COPY;
                        }
                        overlay
                        {
                            contiguous
                            {
                                section .ptextconvstmp2hoststr              TASK_COPY;
                                overlay 
                                {
                                    contiguous 
                                    {
    								    section .ptextststoreenum                   TASK_COPY;
	    		                        section .ptextmtpopensession                TASK_COPY;
	    		                    }
                                    contiguous 
                                    {
                                        section .ptextmtpsendobject                 TASK_COPY;
                                        section .ptextstwriteobjectdata             TASK_COPY;
                                    }
	    						}
                            }
                            contiguous
                            {
                                section .ptextststorclose                   TASK_COPY;
			                    section .ptextmtpclosesession               TASK_COPY;
							}
                            contiguous
                            {
                                section .ptextmtpgetnumobjects              TASK_COPY;
                            }
                            contiguous
                            {
								section .ptextmtpgetobjectinfo                  TASK_COPY;
							}
                            contiguous
                            {
			                    section .ptextmtpsetobjectprotection            TASK_COPY;
							}
                            contiguous
                            {
                                section .ptextmtpgetobjecthandles           TASK_COPY;
                                section .ptextstgetobjectlist               TASK_COPY;
                            }
                            contiguous
                            {
                                section .ptextmtpgetobject                  TASK_COPY;
                                section .ptextstreadobjectdata              TASK_COPY;
                            }
                            contiguous
                            {
                                section .ptextstdeleteobject                TASK_COPY;
                                overlay {
                                    contiguous
                                    {
                                        section .ptextmtpsendobjectinfo             TASK_COPY;
                                        section .ptextstaddnewobject                TASK_COPY;
                                    }
                                    contiguous
                                    {
                                        section .ptextmtpdeleteobject               TASK_COPY;
                                    }
                                }
                            }
                            contiguous
                            {
                                section .ptextststorinfo                        TASK_COPY;
                                overlay 
                                {
                                    contiguous 
                                    {
                                        section .ptextmtpgetstorageids              TASK_COPY;
                                        section .ptextmtpgetdeviceinfo              TASK_COPY;
                                        section .ptextmtpdeviceinfodata             TASK_COPY;
                                    }
                                    contiguous
                                    {
                                        section .ptextmtpgetstorageinfo             TASK_COPY;
                                    }
                                }
                            }
                            contiguous
                            {
                                section .ptextmtppowerdown                  TASK_COPY;
                                section .ptextmtpresetdevice                TASK_COPY;
                                section .ptextmtpselftest                   TASK_COPY;
                            }
                            contiguous
                            {
                                section .ptextmtpgetdevicepropdesc          TASK_COPY;
                                section .ptextmtpgetdevicepropvalue         TASK_COPY;
                                section .ptextmtpsetdevicepropvalue         TASK_COPY;
                                section .ptextmtpresetdevicepropvalue       TASK_COPY;
                                section .ptextmtpjanuspropertyhandler       TASK_COPY;
                           }
                        }
                    }
                    //
                    // The section for the Display Task code.
                    //
                    contiguous 
                    {
                        section .ptextdisplaytask                   TASK_COPY;
                        section .ptextdisplaytask_color             TASK_COPY;
                        section .ptextdisplay                       TASK_COPY;
                        section .ptextdisplayinit                   TASK_COPY;
                        section .ptextdisplayinit_color             TASK_COPY;
                        section .ptextsed15xx                       TASK_COPY;
                        section .ptextsed15xxinit                   TASK_COPY;
                        section .ptextframebuffer                   TASK_COPY;
                        section .ptextcolorlcdsal	                TASK_COPY;
                        section .ptextdisplaydrv                    TASK_COPY;
                        section .ptextdisplaydrv_color              TASK_COPY;
						section .ptexts6b33b0a						TASK_COPY;
						section .ptexts6b33b0a_65k					TASK_COPY;
						section .ptexts6b33b0ainit					TASK_COPY;
                    }
					contiguous 
					{
						section .ptextmtpobjectproperty TASK_COPY;
						overlay 
						{
							section .ptextmtpgetobjectpropsupported TASK_COPY;
							section .ptextmtpgetobjectpropdesc TASK_COPY;
							contiguous {
								section .ptextmtpobjprop_getpropval TASK_COPY;
								overlay {
									section .ptextmtpgetobjectpropvalue TASK_COPY;
									section .ptextmtpgetobjectproplist TASK_COPY;
								}
							}
							contiguous {
								section .ptextmtpobjprop_setpropval TASK_COPY;
								overlay {
									section .ptextmtpsendobjectproplist TASK_COPY;
									section .ptextmtpsetobjectpropvalue TASK_COPY;
									section .ptextmtpsetobjectproplist TASK_COPY;
								}
							}
						}
					}
                    //
                    // Include the database overlay section.
                    //
                    #include "..\..\..\..\..\CPUFiles\database.dsc"
                    contiguous
                    {
                        section .ptextsystem                        TASK_COPY;
                        section .ptextmtp_lcd                       TASK_COPY;
                        section .ptextusbbattery                    TASK_COPY;
                    }
                    contiguous 
                    {
                        section .ptextmtphandler                    TASK_COPY;
                        section .ptextmtpresponse                   TASK_COPY;
                        section .ptextmtpproject                    TASK_COPY;
                        section .ptextusbmscmtpswitch               TASK_COPY;						
                        section .ptextmtp                           TASK_COPY;
                        section .ptextmtpsendresponse               TASK_COPY;
                    } // contiguous
                    //
                    // These are init-time only functions that can be overlaid
                    // once the system is running tasks.
                    //
                    contiguous
                    {
                        section .pbssnandhalinitfunctions;
                        section .pdatanandhalinitfunctions;
                        section .ptextnandhalinitfunctions;
                        section .ptextmain;
                        section .ptextpowerinit;
                        section .ptextflashtiming;
                        section .ptexticoll_c;
                    }
                } // overlay
            } // contiguous
            label MTPRegion_e;
        }
    } /* amode P_far */
}
