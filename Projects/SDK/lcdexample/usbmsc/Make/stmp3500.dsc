/**************************************************************************
**
**  FILE        :  @(#)STMP3500.dsc
**
**  VERSION     :  00/012/20
**
**  DESCRIPTION :  Default locator software part description;
**                 initially, this is for Sigmatel STMP3500.
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file. By default this file
**             assumes that the ROM data tables have not been
**             enabled.
**
**             Defines to change the default memory layout
**             (-em<define> on the locator command line):
**             MEM_Y,
**             MEM_P,
**                     MEM_L : Y/P/L used as default memory (instead of X)
**             NSTK_L: do not put stack in L memory
**                 (only applicable for default X or Y memory)
**             STACKSIZE: set stack size (other than default)
**             HEAPSIZE : set heap size (other than default)
**             NEARXYSIZE : set near X/Y variable area size
**
**************************************************************************/
#define TOTAL_ADDR_SPACE 0x10000

#define INT_P_RAM   0x10000
#define INT_X_RAM   0x2000
#define INT_Y_RAM   0x6000

#define INT_X_ORG   (0)
#define EXT_X_ORG   (INT_X_RAM)

#define INT_Y_ORG   (0)
#define EXT_Y_ORG   (INT_Y_RAM)

#define EXT_P_RAM   (TOTAL_ADDR_SPACE-INT_P_RAM)
#define EXT_X_RAM   (TOTAL_ADDR_SPACE-INT_X_RAM)
#define EXT_Y_RAM   (TOTAL_ADDR_SPACE-INT_Y_RAM)


software 
{
//change this to match the entry point for your program
    load_mod start=Exec;
    layout 
    {
        space P 
        {
            // P address space
            block P_block 
            {
                // P-space memory range
                cluster P_OVLY_clstr 
                {
                    amode P_far 
                    {
                        contiguous addr=0x0080
                        {
                            label p_OVLY_start;
                            overlay
                            {
                                contiguous
                                {
                                    // Start of boot_mmc is label __lc_u_boot_mmc
                                    section .ptextmmcprocesscid attr=i;
                                    section .ptextmmcprocesscsd attr=i;
                                    section .ptextmmcerasefunctions attr=i;
                                    section .ptextmmcmediainit attr=i;
                                    section .ptextmmcfunctions attr=i;
                                    section .ptextmmcdatadrivegetinfo attr=i;
                                    section .ptextmmcresetdevice attr=i;
                                    section .ptextmmcenumerate attr=i;
                                    section .ptextmmcmediagetinfo attr=i;
                                    section .ptextmmcdatadrivereadsector attr=i;
                                    section .ptextmmcdatadriveerase attr=i;
                                    section SPI_HAL_ASM_p attr=i;
                                    section .ptextmmchaldetectpresence attr=i;
                                    section .ptextmmcdatadriveflush attr=i;
                                    section .ptextmmcmediaallocate attr=i;
                                    section .ptextmmcdetectmedia attr=i;
                                    section .ptextmmcmediaerase attr=i;
                                    section .ptextmmchalfunctions attr=i;
                                    section .ptextmmcmediadiscover attr=i;
                                    section .ptextmmcdatadriveinit attr=i;
                                    // moved 2/23/04 because of Boot EEPROM.
                                    section .ptextmmchalspiwrite attr=i;
                                    section .ptextmmchalspi attr=i;
                                    section .ptextmmcdatadrivewritesector attr=i;
                                    section .ptextmmcdatadrivemultiwrite attr=i;
                                    section .ptextspihal attr=i;
                                    section .ptextlowresolutionadcinit attr=i;
                                    section .ptextchipcapability attr=i;
                                    section .ptextchipserialnumberinit attr=i;
                                    section .ptextbatterychargetask attr=i;
                                    section .ptextnandhalsetnandbusy attr=i;
                                    section .ptextusbbattery attr=i;
                                    section .ptextversion attr=i;
                                    section .ptextunicode attr=i;
							        section .ptextunicode_color attr=i;
                                    section .ptextrammediainit attr=i;
                                    section .ptextmiscproject attr=i;
                                    section .ptextdisplaytask attr=i;
									section .ptextdisplaytask_color attr=i;
                                    section .ptextnandra_utils attr=i;
                                }
                                //
                                // These must be unused in updater mode.
                                //
                                contiguous
                                {
                                    section .ptextndd_pin_allocation        attr=i;
                                    section .ptextndd_write_sector_in_place attr=i;
                                }
                            } // overlay
                        }
                    }         // end of amode
                }       // end of P_OVLY_cluster.
                cluster P_clstr 
                {
                    amode P_far 
                    {   
                        contiguous {
                            label p_Disp_start;
                            overlay {
                              contiguous {
                                  section .ptexts6b33b0ainit attr=i;
                                  section .ptextdisplayinit attr=i;
                                  section .ptextdisplayinit_color attr=i;
                                  section .ptextstfm1000testpresence attr=i;    //move here since stfm1000 test only being called in display driver initialization
                              }
                              contiguous {
                                  section .ptexts6b33b0a attr=i;
                                  section .ptexts6b33b0a_65k attr=i;
                                  section .ptexts6b33b0a_262k attr=i;
                                  label p_usbmsc_lcd_battery_start;
                                  overlay {
                                    section .ptextusbmsc_lcd attr=i;
                                    section .ptextusbmsc_lcd_battery attr=i;
                                    section .ptextdbcs_usbmsc_overlay attr=i;
                                    section .ptexts6b33b0a_65k_usbmsc_overlay attr=i;
                                    section .ptextMMCProcessCid2_usbmsc_overlay attr=i;
                                    section .ptextMMCProcessCsd_usbmsc_overlay attr=i;
                                    section .ptextRAMMediaInit_usbmsc_overlay attr=i;
                                  }
                              }
                            }
                        }
                    }
                    amode P_sjmp 
                    {
                        reserved addr=0x2 length=0x7e;
                        section selection=x;
                        section selection=w;
                        label bP_Module_overlay;
                    } // end of amode P_sjmp
                    amode P_far 
                    {
                      // Save space for the bootmanager.
                      reserved addr=0x80 length=0x2380;
                      section selection=x;
                      section selection=w;
                      // Due to above, all P far addressing mode executable (and writable) sections locate here by default.
                    // I don't think these are used in USBMSC.
        //                        label eP_Module_overlay;
        //                        label bP_Media_DD_overlay;
        //                        label eP_Media_DD_overlay;
                    }   // end of amode P_far
                }   // end of P_clstr
            }       // end of P_block
        }       // end of P_space
        space L 
        {
            // L address space
            block L_block 
            {
                // L-space memory range
                cluster L_clstr 
                {
                    attribute w;
                    amode L_near 
                    {
                        section selection=w;
                    }
                    amode L_far 
                    {
                        section selection=w;
                    }
                }
            }
        }
        space X 
        {
            // X address space
            block XInt_Block 
            {
                // X-space memory range
                cluster X_clstr 
                {
                    attribute w;
                    amode X_near 
                    {
                        reserved addr=0x0 length=0x200;
                        // near addressing mode:
                        // locate writable sections
                        section selection=w;
                    }
                    amode X_far 
                    {
                        reserved addr=0x0 length=0x400;
                        section USB20_Device_Driver;
                        section selection=w;
                    }       // end of amode X_far
                    amode X_io 
                    {
                        // io addressing mode:
                        // locate writable sections
                        section selection=w;
                    }
                }
            }
            block XExt_Block 
            {
                // X-space memory range
                cluster X_Copy_clstr 
                {
                    amode X_far 
                    {
                        fixed addr=EXT_X_ORG;
                        //
                        // These are never extrated but are located here.
                        //
                        copy .ptextndd_pin_allocation;
                        copy .ptextndd_write_sector_in_place;
                    }
                }
            }
        }
        space Y 
        {
            // Y address space
            block Y_block 
            {
                // Y-space memory range
                cluster Y_clstr 
                {
                    attribute w;
                    section GLOBAL_CONSTANTS_Y addr=0;
                    amode Y_near 
                    {
                        section selection=w;
                    }
                    amode Y_far 
                    {
                        contiguous
                        {
                            overlay 
                            {
                                contiguous 
                                {
                                    label y_MMC_bssclear_start;
                                    section .ybssnanddatadrivemultiwritedumb attr=-bf;
                                    section .ybssnandmediadefaults attr=-bf;
                                    section .ybssnanddatadrivemultiwritecache attr=-bf;
                                    section .ybssnandsystemdriveinit attr=-bf;
                                    section .ybssmmcmediainit attr=-bf;
                                    section .ybssmmcfunctions attr=-bf;
                                    section .ybssnandhalinitfunctions attr=-bf;
                                    section .ybssnandmediainit attr=-bf;
                                    // The next one is riskier.
                                    section .ybssusb_api attr=-bf;
                                    label y_MMC_bssclear_end;
                                }
                                contiguous 
                                {  // Contiguous bootmanager overlay
                                    contiguous 
                                    {
                                        label y_OVLY_copy_start;
                                        copy .ptextmmcprocesscid;
                                        copy .ptextmmcprocesscsd;
                                        copy .ptextmmcerasefunctions;
                                        copy .ptextmmcmediainit;
                                        copy .ptextmmcfunctions;
                                        copy .ptextmmcdatadrivegetinfo;
                                        copy .ptextmmcresetdevice;
                                        copy .ptextmmcenumerate;
                                        copy .ptextmmcmediagetinfo;
                                        copy .ptextmmcdatadrivereadsector;
                                        copy .ptextmmcdatadriveerase;
                                        copy SPI_HAL_ASM_p;
                                        copy .ptextmmchaldetectpresence;
                                        copy .ptextmmcdatadriveflush;
                                        copy .ptextmmcmediaallocate;
                                        copy .ptextmmcdetectmedia;
                                        copy .ptextmmcmediaerase;
                                        copy .ptextmmchalfunctions;
                                        copy .ptextmmcmediadiscover;
                                        copy .ptextmmcdatadriveinit;
                                        // moved 2/23/04 because of Boot EEPROM.
                                        copy .ptextmmchalspiwrite;
                                        copy .ptextmmchalspi;
                                        copy .ptextmmcdatadrivewritesector;
                                        copy .ptextmmcdatadrivemultiwrite;
                                        copy .ptextspihal;
                                        copy .ptextlowresolutionadcinit;
                                        copy .ptextchipcapability;
                                        copy .ptextchipserialnumberinit;
                                        copy .ptextbatterychargetask;
                                        copy .ptextnandhalsetnandbusy;
                                        copy .ptextusbbattery;
                                        copy .ptextversion;
                                        copy .ptextunicode;
									    copy .ptextunicode_color;
                                        copy .ptextrammediainit;
                                        copy .ptextmiscproject;
                                        copy .ptextdisplaytask;
										copy .ptextdisplaytask_color;
                                        copy .ptextnandra_utils;
                                        label y_OVLY_copy_end;
                                        label y_Disp_Init_OVLY_copy_start;
                                        copy .ptexts6b33b0ainit;
                                        copy .ptextdisplayinit;
                                        copy .ptextdisplayinit_color;
                                        copy .ptextstfm1000testpresence;
                                        label y_Disp_Init_OVLY_copy_end;
                                        label y_Disp_Code_OVLY_copy_start;
                                        copy .ptexts6b33b0a;
                                        copy .ptexts6b33b0a_65k;
                                        copy .ptexts6b33b0a_262k;
                                        label y_Usbmsc_lcd_OVLY_copy_start;
                                        copy .ptextusbmsc_lcd;
                                        label y_Usbmsc_lcd_OVLY_copy_end;
                                        label y_Disp_Code_OVLY_copy_end;
                                        label y_Usbmsc_lcd_battery_OVLY_copy_start;
                                        copy .ptextusbmsc_lcd_battery;
                                        label y_Usbmsc_lcd_battery_OVLY_copy_end;
                                        label y_dbcs_usbmsc_OVLY_copy_start;
                                        copy .ptextdbcs_usbmsc_overlay;
                                        label y_dbcs_usbmsc_OVLY_copy_end;
                                        label y_s6b33b0a_65k_usbmsc_OVLY_copy_start;
                                        copy .ptexts6b33b0a_65k_usbmsc_overlay;
                                        label y_s6b33b0a_65k_usbmsc_OVLY_copy_end;
                                        label y_MMCProcessCid2_usbmsc_OVLY_copy_start;
                                        copy .ptextMMCProcessCid2_usbmsc_overlay;
                                        label y_MMCProcessCid2_usbmsc_OVLY_copy_end;
                                        label y_MMCProcessCsd_usbmsc_OVLY_copy_start;
                                        copy .ptextMMCProcessCsd_usbmsc_overlay;
                                        label y_MMCProcessCsd_usbmsc_OVLY_copy_end;
                                        label y_RAMMediaInit_usbmsc_OVLY_copy_start;
                                        copy .ptextRAMMediaInit_usbmsc_overlay;
                                        label y_RAMMediaInit_usbmsc_OVLY_copy_end;
                                    }   // end of contiguous
                                }   // End of Contiguous bootmanager overlay
                            }   // end of overlay
                            // Put rest of sections not referenced in here.
                            section selection=w;
                        }       // end of contiguous
                    }       // end of amode Y_far
                    amode Y_io 
                    {
                        section selection=w;
                    }
                }
            }
        }
    }
}


// include cpu part and memory part
cpu ..\..\..\..\..\cpufiles\usbmsc.cpu
memory ..\..\..\..\..\cpufiles\STMPdflt.mem
