/**************************************************************************
**
**  FILE        :  @(#)testdflt.dsc 
**
**  VERSION     :  00/012/20
**
**  DESCRIPTION :  Default locator software part description;
**                     this is
**                     for Sigmatel STMP35xx.
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file. By default this file
**             assumes that the ROM data tables have not been 
**             enabled.
**
**
**************************************************************************/

#define TOTAL_ADDR_SPACE 0x10000

#define INT_P_RAM   0x10000
#define INT_X_RAM   0x2000
#define INT_Y_RAM   0x6000

#define INT_P_VECTOR_ORG 0x0000
#define INT_P_VECTOR_SZ  0x0080

#define INT_P_OVLY_ORG   0x80
#define INT_P_OVLY_SZ    0x1680 // 0x22d9 historic value  // sdk2.6.025 had 0x1629 locates. TOVERIFY, is x1680 enough rsvd for bootmanager? 
#define INT_P_MDD_ORG    (INT_P_OVLY_ORG+INT_P_OVLY_SZ)
#define INT_P_MDD_SZ     (0x1900-0xA00) //0x1900  //TOVERIFY    sdk3.0 & base ver had 0x1900. 2.6 ver reduced it to 0x1070

#define INT_P_SYS_ORG    (INT_P_MDD_ORG+INT_P_MDD_SZ)
#define INT_P_SYS_SZ	 (INT_P_RAM-INT_P_SYS_ORG)

#define INT_Y_BLOCK_ORG 0
#define INT_Y_BLOCK_SZ  0x3000   // TOVERIFY   0x3000 is 2.6 ver (a reduction from base).  sdk3.0 and base ver was 0x3100
#define INT_Y_COPY_ORG  (INT_Y_BLOCK_ORG+INT_Y_BLOCK_SZ)
#define INT_Y_COPY_SZ   (INT_Y_RAM-INT_Y_COPY_ORG)

#define MP3_COPY   attr=i
#define EXT_P_RAM   (TOTAL_ADDR_SPACE-INT_P_RAM)
#define EXT_X_RAM   (TOTAL_ADDR_SPACE-INT_X_RAM)
#define EXT_Y_RAM   (TOTAL_ADDR_SPACE-INT_Y_RAM)

software {
//change this to match the entry point for your program
  load_mod start=Exec;
  layout {
    space P 
    {
        // P address space
        block PVectorBlock 
        {
            // P-space memory range 
            cluster P_vector 
            {
                amode P_sjmp 
                {
                    // reserve vector space in case using interrupts
	    
                    section selection=w;
            
                }

                amode P_far 
                {
                    section EXEC_Vectors_P addr=0x000000;
                }
            }
        }

        block POvlyBlock 
        {
            cluster P_evictBootMng 
            {

                amode P_far 
                {

                    contiguous addr = INT_P_OVLY_ORG 
                    {

                        label p_OVLY_start;
                        overlay
                        {
                            contiguous
                            {
                                section .ptextmmchalspiwrite attr=i;
                                section .ptextmmchaldetectpresence attr=i;
// Mar 28 2007 (remove these sections so that they all fall nto the P_clstr)+
//                                section .ptextmmcprocesscsd attr=i;

//                                section .ptextmmcprocesscid2 attr=i;
//                                section .ptextmmcprocesscid attr=i;
//                                section .ptextmmcresetdevice attr=i;
//                                section .ptextmmcserialnumberinit attr=i;
//                                section .ptextmmcenumerate attr=i;
// Mar 28 2007 (remove these sections so that they all fall nto the P_clstr)-
                                section .ptextnddcloseupdate attr=i;
                                section .ptextdisplaydrv attr=i;
                                section .ptextreaddevicerecord attr=i;
                    
                                section .ptextfirstfreeandallocate attr=i;
                            }

                            //These sections are unused in the fat32test	
// Mar 28 2007 (overlaying dummy sections)+
//                            contiguous
//                            {
//                              // this is overlaid starting in sdk2.6 ver. 3.0 ver was in contig block above without overlay.
//                              section .ptextnanddatadrivemultiwritecache attr=i;
//                            }
                            overlay
                            {
                                // this is overlaid starting in sdk2.6 ver. 3.0 ver was in contig block above without overlay.
                                section .ptextnanddatadrivemultiwritecache attr=i;

                                section .ptextnanddatadrivemultiwrite MP3_COPY;            
                                section .ptextnanddatadrivemultiwritedumb MP3_COPY;
                                section .ptextnandmediaallocate MP3_COPY;
                                section .ptextnandsystemdrivemultiwrite MP3_COPY;
                                section .ptextnandsystemdrivesetinfo MP3_COPY;
                                section .ptextnanddatadrivesetinfo MP3_COPY;
                                section .ptextnanddatadriveerase MP3_COPY;
                                section .ptextnandsystemdriveerase MP3_COPY;
                                section .ptextnandmediaerase MP3_COPY;
                                section .ptextnandsystemdrivewritesector MP3_COPY;
                            }
//Mar 28 2007 (overlaying dummy sections)-
                        }//overlay
                    } //contiguous
                }//amode p_far
            } //cluster
        } // end of POvlyBlock 

        block P_Block 
        {
            cluster P_clstr 
            {
                amode P_far 
                {
                    label eP_Module_overlay;
                    section selection=w;
                }

// Mar 28 2007 (add the sections below for resolving symbols)+
                amode P_far 
                {
                        contiguous
                        {
                            label p_usbmsc_lcd_battery_start;
                            overlay 
                            {
                                // DBCS does not overlay in FAT32test 
                                //section .ptextdbcs_usbmsc_overlay attr=i;
                                section .ptextMMCProcessCid2_usbmsc_overlay attr=i;
                                section .ptextMMCProcessCsd_usbmsc_overlay attr=i;
                            }
                        }
                }
// Mar 28 2007 (add the sections below for resolving symbols)-
            }
        }

        block PMediaDD 
        {
            cluster P_Media_DD_Read_cluster 
            {
                amode P_far 
                {
                    fixed addr = INT_P_MDD_ORG;
                    contiguous //addr = INT_P_MDD_ORG 
                    {//these seemingly extra contiguous/overlay brackets are here because the locator has a habit
                        overlay 
                        {//of putting the labels after the section at the wrong spot unless you have them.
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
                                                section .ptextnanddatadrivereadsector MP3_COPY;     // Called through SysCallFunction
                                                section .ptextnanddatadrivewritesector MP3_COPY;    // Called through SysCallFunction
                                                section .ptextnanddatadrivegetinfo MP3_COPY;        // Called through SysCallFunction
                                                section .ptextnddconvertlogicaltorelative MP3_COPY;
                                                section .ptextnddlookupublock MP3_COPY;
                                                section .ptextnddlookupusector MP3_COPY;
                                                section .ptextnddprotectublocks MP3_COPY;
                                                section .ptextnddconvertrelativetophysical MP3_COPY;
                                                section .ptextnddlinconvertrelativetophys MP3_COPY;
                                            }
                                        }
        
                                        section .ptextnddgetusectorsmap MP3_COPY;
                                        section .ptextnddlookuprblock MP3_COPY;
                                        section .ptextnddsearchbbtblforphysblock MP3_COPY;
                                        section .ptextnddaddblktobbtbl MP3_COPY;
                                        section .ptextnddublkutils MP3_COPY;
                                        section .ptextnddflush MP3_COPY;
                                    }
        
                                    contiguous  
                                    {
                                        section .ptextnandmediainit;
                                        section .ptextnandsystemdriveinit;
                                        section .ptextnandmediadiscover;
                                        section .ptextnandmediagetinfo;
                                        section .ptextnandsystemdrivegetinfo;
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
                    } // contiguous
        
                    label MediaRegion_e;
                    label bP_Module_overlay;
        
// Mar 28 2007 (move these dummy sections to P_evictBootMng)+
//                    contiguous
//                    {
//                        overlay //addr=INT_P_DUMMY_ORG
//                        {
//                            section .ptextnanddatadrivemultiwrite MP3_COPY;
//        
//                            section .ptextnanddatadrivemultiwritedumb MP3_COPY;
//                            section .ptextnandmediaallocate MP3_COPY;
//                            section .ptextnandsystemdrivemultiwrite MP3_COPY;
//                            section .ptextnandsystemdrivesetinfo MP3_COPY;
//                            section .ptextnanddatadrivesetinfo MP3_COPY;
//                            section .ptextnanddatadriveerase MP3_COPY;
//                            section .ptextnandsystemdriveerase MP3_COPY;
//                            section .ptextnandmediaerase MP3_COPY;
//                            section .ptextnandsystemdrivewritesector MP3_COPY;
//                        }
//                    }
// Mar 28 2007(move these dummy sections to  P_evictBootMng)-
        
                }  // amode far
            }  // cluster

//                cluster P_Media_Dummy_cluster 
//                {
//                    amode P_far 
//                    {
//                        overlay addr=INT_P_MDD_ORG
//                        {
//                            section .ptextnanddatadrivemultiwrite MP3_COPY;
//                            section .ptextnanddatadrivemultiwritedumb MP3_COPY;
//                            section .ptextnandmediaallocate MP3_COPY;
//                            section .ptextnandsystemdrivemultiwrite MP3_COPY;
//                            section .ptextnandsystemdrivesetinfo MP3_COPY;
//                            section .ptextnanddatadrivesetinfo MP3_COPY;
//                            section .ptextnanddatadriveerase MP3_COPY;
//                            section .ptextnandsystemdriveerase MP3_COPY;
//                            section .ptextnandmediaerase MP3_COPY;
//                            section .ptextnandsystemdrivewritesector MP3_COPY;
//                        }
//                    }
//                }

        }  // block



    }
    space L {
      // L address space
      block L_block {
        // L-space memory range
        cluster L_clstr {
          attribute w;
          amode L_near {
            section selection=w;
          }
          amode L_far {
            section selection=w;
          }
        }
      }
    }
    space X {
      // X address space


      block X_block {

        // X-space memory range
        cluster X_clstr {
          attribute w;
          amode X_near {
            // near addressing mode:
            // locate writable sections
            reserved addr=0x0 length=0x200;
            section selection=w;
          }
          amode X_far {
            reserved addr=0x0 length=0x400;
            section selection=w;
          }
          amode X_io {

            // io addressing mode:
            // locate writable sections
            section selection=w;
          }
        }
      }
    }

    space Y {
      // Y address space
      block Y_block {
        // Y-space memory range

        cluster Y_clstr {
          section GLOBAL_CONSTANTS_Y addr=0;
          attribute w;
          amode Y_near {
            section selection=w;
          }
          amode Y_far {
            section selection=w;
          }
        }

        cluster Y_copyClstr{
          amode Y_far {
            contiguous {
              contiguous {

                label y_evictBootCopybegin;

                copy .ptextmmchalspiwrite;
                copy .ptextmmchaldetectpresence;
// Mar 28 2007 (remove these sections so that they all fall nto the P_clstr)+
//                copy .ptextmmcprocesscsd;

//                copy .ptextmmcprocesscid2;
//                copy .ptextmmcprocesscid;
//                copy .ptextmmcresetdevice;
//                copy .ptextmmcserialnumberinit;
//                copy .ptextmmcenumerate;
// Mar 28 2007 (remove these sections so that they all fall nto the P_clstr)-

                copy .ptextnddcloseupdate;
                copy .ptextdisplaydrv;
                copy .ptextreaddevicerecord;

                copy .ptextfirstfreeandallocate;

                label y_evictBootFirstCopyend;

// Mar 28 2007 (add labels below to resolve symbols)+
                label y_MMCProcessCid2_usbmsc_OVLY_copy_start;
                copy .ptextMMCProcessCid2_usbmsc_overlay;
                label y_MMCProcessCid2_usbmsc_OVLY_copy_end;

                label y_Usbmsc_lcd_OVLY_copy_start; // dummy label for Usbmsc_lcd section start
                label y_MMCProcessCsd_usbmsc_OVLY_copy_start;
                copy .ptextMMCProcessCsd_usbmsc_overlay;
                label y_MMCProcessCsd_usbmsc_OVLY_copy_end;
                label y_Usbmsc_lcd_OVLY_copy_end; //  dummy label for Usbmsc_lcd section end
// Mar 28 2007 (add labels below to resolve symbols)-
              }
            }
          }
        }
      }


      block Y_copy
      {
        cluster Y_Copy {
          //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
          attribute s;//don't locate anything here automatically.  
          //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
          amode Y_far {
            copy;
          }
        }

      }
    } // Space_Y
  }
}


// include cpu part and memory part

// Note paths are relative to build directory
cpu .\stmpdfltFAT.cpu
memory ..\..\..\CPUFiles\stmpdflt.mem

