/**************************************************************************
**
**  FILE        :  @(#)STMP3500.dsc 
**
**  VERSION     :  00/012/20
**
**  DESCRIPTION :  Default locator software part description;
**                     initially, this is
**                     for Sigmatel STMP35000.
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

#define INT_P_RAM   0xC000
#define INT_X_RAM   0x6000
#define INT_Y_RAM   0x6000

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
                        reserved addr=0x0 length=0x80;
                        section selection=x;
                        section selection=w;
                        label bP_Module_overlay;
                    }
                    amode P_far {
                        section selection=x;
                        section selection=w;
                        label eP_Module_overlay;
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
                        reserved addr=0x0 length=0x200;
                        // near addressing mode:
                        // locate writable sections
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
                    attribute w;
                    section GLOBAL_CONSTANTS_Y addr=0;
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
        }
    }
}


// include cpu part and memory part
cpu ..\..\..\cpufiles\STMPdflt.cpu
memory ..\..\..\cpufiles\STMPdflt.mem
