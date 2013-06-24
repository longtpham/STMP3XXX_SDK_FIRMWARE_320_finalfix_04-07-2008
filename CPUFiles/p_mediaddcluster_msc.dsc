cluster P_Media_DD_Read_cluster 
{
    amode P_far 
    {
        contiguous addr=INT_P_MEDIADD_ORG 
        { //these seemingly extra contiguous/overlay brackets are here because the locator has a habit
            overlay // Media region overlays 
            { //of putting the labels after the section at the wrong spot unless you have them.
                label MediaRegion_b;
                contiguous {    // MMC overlays
                    section .ptextmmchalspi                                     MEDIA_DD_COPY;	
                    section .ptextspihal                                        MEDIA_DD_COPY;
                    section SPI_HAL_ASM_p                                       MEDIA_DD_COPY; 
                    overlay {
                        contiguous {
                            section .ptextmmcfunctions                          MEDIA_DD_COPY;
                            overlay {
                                contiguous {
                                    section .ptextmmcmediainit                  MEDIA_DD_COPY;
                                    section .ptextmmcmediaallocate              MEDIA_DD_COPY;
                                    section .ptextmmcmediadiscover              MEDIA_DD_COPY; 
                                    section .ptextmmcdatadriveinit              MEDIA_DD_COPY;
                                    section .ptextmmcdatadriveflush             MEDIA_DD_COPY;
                                } // contiguous
			                    contiguous {
			                        section .ptextmmcmediagetmediatable         MEDIA_DD_COPY;
				                    section .ptextmmcmediagetinfo               MEDIA_DD_COPY;
				                    section .ptextmmcdatadrivesetinfo           MEDIA_DD_COPY;
				                    section .ptextmmcdatadrivegetinfo           MEDIA_DD_COPY;
                                    section .ptextmmcdetectmedia                MEDIA_DD_COPY; 
				                } // contiguous
                            } // overlay
                        } // contiguous
                        contiguous {
				            section .ptextmmcmediaerase                         MEDIA_DD_COPY;
				            section .ptextmmcdatadriveerase                     MEDIA_DD_COPY;
				            section .ptextmmcerasefunctions                     MEDIA_DD_COPY;
				        }
			            contiguous {
				            section .ptextmmcdatadrivereadsector                MEDIA_DD_COPY;     
				            section .ptextmmcdatadrivewritesector               MEDIA_DD_COPY;
				            section .ptextmmcdatadrivemultiwrite                MEDIA_DD_COPY;  
				        } // contiguous
			        } // overlay
		        } // contiguous MMC overlays
                contiguous // NAND overlays
                {
                    overlay 
                    {
                        contiguous 
                        {
                            section .ptextnddgetusectorsmap                 MEDIA_DD_COPY;
                            section .ptextnddlookuprblock                   MEDIA_DD_COPY;
                            section .ptextnddsearchbbtblforphysblock        MEDIA_DD_COPY;
                            section .ptextnddaddblktobbtbl                  MEDIA_DD_COPY;
                            section .ptextnddublkutils                      MEDIA_DD_COPY;
                            section .ptextnddflush                          MEDIA_DD_COPY;
                            overlay 
                            {
                                contiguous 
                                {
                                    section .ptextnanddatadriveinit             MEDIA_DD_COPY;
                                    section .ptextnanddatadrivesetinfo          MEDIA_DD_COPY;
                                    section .ptextnanddatadriveerase            MEDIA_DD_COPY;
                                }
                                contiguous 
                                {
                                    section .ptextnanddatadrivereadsector       MEDIA_DD_COPY;     // Called through SysCallFunction
                                    section .ptextnanddatadrivewritesector      MEDIA_DD_COPY;     // Called through SysCallFunction
                                    section .ptextnddconvertlogicaltorelative   MEDIA_DD_COPY;
                                    section .ptextnddlookupublock               MEDIA_DD_COPY;
                                    section .ptextnddlookupusector              MEDIA_DD_COPY;
                                    section .ptextnddprotectublocks             MEDIA_DD_COPY;
                                    section .ptextnddconvertrelativetophysical  MEDIA_DD_COPY;
                                    section .ptextnddlinconvertrelativetophys   MEDIA_DD_COPY;
				    section .ptextndd_pin_allocation 		MEDIA_DD_COPY;
				    section .ptextndd_write_sector_in_place 	MEDIA_DD_COPY;
                                    section .ptextnanddatadrivegetinfo          MEDIA_DD_COPY;
                                }
                                contiguous 
                                {
                                    section .ptextnandsystemdrivesetinfo        MEDIA_DD_COPY;
                                    section .ptextnandsystemdrivewritesector    MEDIA_DD_COPY;
                                    section .ptextnandsystemdrivemultiwrite     MEDIA_DD_COPY;
                                    section .ptextnandsystemdriveerase          MEDIA_DD_COPY;
                                    section .ptextnandsystemdriveflush          MEDIA_DD_COPY;
                                }
                            } // overlay
                        } //contiguous
                        contiguous
                        {
                            section .ptextnandmediagetinfo;
                            section .ptextnandsystemdrivegetinfo;
                            section .ptextnandmediainit;                            
                            section .ptextnandsystemdriveinit;
                            section .ptextnandmediadiscover;
                            section .ptextnandmediaerase;
                            section .ptextnandmediagetmediatable;
                        } // contiguous
			            // These sections have code that is actually never used in MTP
			            // but need to be compiled
			            contiguous
            			{
                        	section .ptextnandmediaallocate						MEDIA_DD_COPY;
			            } // contiguous
                    } // overlay
                } // contiguous NAND overlays
            } // overlay Media region overlays  
        } // contiguous addr=INT_P_MEDIADD_ORG 
        label MediaRegion_e;
   } // amode far
} // DD_read cluster
