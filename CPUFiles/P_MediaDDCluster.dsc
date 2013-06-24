cluster P_Media_DD_Read_cluster
{
    amode P_far 
    {
        fixed addr = INT_P_MDD_ORG;
        contiguous 
        {//these seemingly extra contiguous/overlay brackets are here because the locator has a habit
            overlay 
            {//of putting the labels after the section at the wrong spot unless you have them.
               label eP_app_cluster;
               label MediaRegion_b;
                contiguous 
                {
                   overlay
                   {
                        contiguous
                        {
                            overlay
                            {
                                section .ptextnanddatadriveinit MP3_COPY;
                                contiguous
                                {
                                    section .ptextnanddatadrivereadsector       MP3_COPY;
                                    section .ptextnanddatadrivewritesector      MP3_COPY;
                                    section .ptextnanddatadrivegetinfo          MP3_COPY;
                                    section .ptextnddconvertlogicaltorelative MP3_COPY;
                                    section .ptextnddlookupublock MP3_COPY;
                                    section .ptextnddlookupusector MP3_COPY;
                                    section .ptextnddprotectublocks MP3_COPY;
                                    section .ptextnddconvertrelativetophysical MP3_COPY;
                                    section .ptextnddlinconvertrelativetophys MP3_COPY;
                                    }
                                }
                            
                            section .ptextnddlookuprblock MP3_COPY;
                            section .ptextnddgetusectorsmap MP3_COPY;
                            section .ptextnddublkutils MP3_COPY;
                            section .ptextnddaddblktobbtbl MP3_COPY;
                            section .ptextnddsearchbbtblforphysblock MP3_COPY;

                            section .ptextnddflush MP3_COPY;
                        }
                        contiguous 
                        {
                            overlay 
                            {
                                section .ptextnandmediagetinfo MP3_COPY;
                                section .ptextnandsystemdrivegetinfo MP3_COPY;
                            }
                        }
                        // ********  SEMI-STATIC SECTIONS IN OVERLAY: Must be only sections 
                        // with player.abs build blank attr in the overlay else locator fails silently.
                        contiguous  
                        {
                            section .ptextnandmediainit;
                            section .ptextnandsystemdriveinit;
                        }
                    } // overlay
                } // contiguous	
                contiguous
                {
                    section .ptextmmchalfunctions MP3_COPY;
                    section .ptextmmchalspi MP3_COPY;			
                    section .ptextspihal MP3_COPY;
                    section SPI_HAL_ASM_p MP3_COPY; 
                    overlay 
                    {
                        contiguous 
                        {
                            section .ptextmmcfunctions MP3_COPY;
                            overlay 
                            {
                                contiguous 
                                {
                                    section .ptextmmcmediainit MP3_COPY;
                                    section .ptextmmcmediaallocate MP3_COPY;        
                                    section .ptextmmcmediadiscover MP3_COPY; 
                                    section .ptextmmcdatadriveinit MP3_COPY;
                                    section .ptextmmcdatadriveflush MP3_COPY;
                                    section .ptextmmchalinit MP3_COPY;
                                }                           
                                contiguous 
                                {
                                    section .ptextmmcmediagetmediatable MP3_COPY;         
                                    section .ptextmmcmediagetinfo MP3_COPY;
                                    section .ptextmmcdatadrivesetinfo MP3_COPY;
                                    section .ptextmmcdatadrivegetinfo MP3_COPY;
                                    section .ptextmmcdetectmedia MP3_COPY; 
                                }
                            }
                        }
                        contiguous
                        {
                             section .ptextmmcmediaerase MP3_COPY;
                             section .ptextmmcdatadriveerase MP3_COPY;
                             section .ptextmmcerasefunctions MP3_COPY;
                        }
                        contiguous 
                        {
                            section .ptextmmcdatadrivereadsector MP3_COPY;     
                            section .ptextmmcdatadrivewritesector MP3_COPY;
                            section .ptextmmcdatadrivemultiwrite MP3_COPY;  
                        }                
                    } // overlay
                } // contiguous
            }
        }
        fixed addr = INT_P_MDD_ORG+INT_P_MDD_SZ;
        label MediaRegion_e;
    }  // amode far
}  // cluster
