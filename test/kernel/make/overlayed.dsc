/**************************************************************************
**
**	FILE        :  @(#)STMP3400.dsc	
**
**	VERSION     :  00/012/20
**
**	DESCRIPTION :  Default locator software part description;
**                     initially, this is
**                     for Sigmatel STMP34000.
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file. By default this file
**		       assumes that the ROM data tables have not been 
**		       enabled.
**
**
**************************************************************************/

#define TOTAL_ADDR_SPACE 0x10000

#define INT_P_RAM	0x6000
#define INT_X_RAM	0x2000
#define INT_Y_RAM	0x2000

#define EXT_P_RAM   (TOTAL_ADDR_SPACE-INT_P_RAM)
#define EXT_X_RAM   (TOTAL_ADDR_SPACE-INT_X_RAM)
#define EXT_Y_RAM   (TOTAL_ADDR_SPACE-INT_Y_RAM)

software {
//change this to match the entry point for your program
        load_mod start=Exec;
	layout {
		space P {
			// P address space
			block P_block {
				// P-space memory range				 
				cluster P_clstr {
				    section EXEC_Vectors_P addr=0x000000;

					amode P_sjmp {
                        // reserve vector space in case using interrupts
                        label bP_Module_overlay;
						section selection=w;
					}
					amode P_far {
                        reserved addr=0x80 length=0x1000-0x80;
  						section selection=w;
                        contiguous {
                            label Region1_b;
                        contiguous {
                            overlay {
                                section .ptexttask0 attr=i;
                                section .ptexttask1 attr=i;

                                section .ptexttask2 attr=i;
                                }
                            }                        
                            label Region1_e;
                            label Region2_b;
                        contiguous {
                            overlay {
                                section .ptexttask3 attr=i;
                                section .ptexttask4 attr=i;
                                }
                            }
                            label Region2_e;
                        }
					}
				}
			}
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
					amode Y_io {
  						section selection=w;
					}
				}
			}
		} // Space_Y
	}
}


// include cpu part and memory part

// Note paths are relative to build directory
cpu ..\..\..\CPUFiles\stmpdflt.cpu
memory ..\..\..\CPUFiles\stmpdflt.mem

