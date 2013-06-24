/**************************************************************************
**
**  FILE        :  @(#)STMP3500.dsc 
**
**  VERSION     :  02/11/14
**
**  DESCRIPTION :  Boot ROM locator software part description for STMP3500
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file.
**
**             Defines to change the default memory layout 
**             (-em<define> on the locator command line):
**             MEM_Y,
**             MEM_P,
**             MEM_L : Y/P/L used as default memory (instead of X)
**             NSTK_L: do not put stack in L memory 
**                 (only applicable for default X or Y memory)
**             STACKSIZE: set stack size (other than default)
**             HEAPSIZE : set heap size (other than default)
**             NEARXYSIZE : set near X/Y variable area size
**
**************************************************************************/
#define TOTAL_ADDR_SPACE    0x10000
#define INT_L_RAM           TOTAL_ADDR_SPACE

#define INT_P_VECTOR_ORG    0x0000
#define INT_P_VECTOR_SZ     0x0080
#define INT_P_APP_ORG       (INT_P_VECTOR_ORG+INT_P_VECTOR_SZ) // 0x80
#define INT_P_APP_SZ        (TOTAL_ADDR_SPACE-INT_P_APP_ORG)                                    

#define INT_X_RAM        0x6000
#define INT_X_BTRM_ORG   0x0000
#define INT_X_BTRM_SZ    0x0210
#define INT_X_USB20_ORG  INT_X_BTRM_SZ
#define INT_X_USB20_SZ   0x2000
#define INT_X_APP_ORG    (INT_X_USB20_ORG+INT_X_USB20_SZ)
#define INT_X_APP_SZ     (INT_X_RAM-INT_X_BTRM_SZ-INT_X_USB20_SZ)

// Y memory boundaries
#define INT_Y_RAM        0x6000
#define INT_Y_APP_ORG    0x0000
#define INT_Y_APP_SZ     INT_Y_RAM

software {
//change this to match the entry point for your program
        load_mod start=Exec;

    layout {

        space P {
            // P address space

            block PVectorBlock {
                // P-space memory range 
                cluster P1_clstr {
                    section EXEC_Vectors_P    addr=0x000000;
                    section VDDIO_BO_Vector_P addr=0x00000A;
                    section USB_Vector_P      addr=0x000052;
                    amode P_sjmp {
                        reserved addr=0x0 length=0x80;
                        section selection=x;
                        section selection=w;
                        label bP_Module_overlay;
                        
                    }               // amode 
                }                   // cluster
           }                        // block    

            block PAppBlock {
                // P-space memory range 
                cluster P2_clstr {
                    amode P_far {
                        section selection=x;
                        section selection=w;
                        label eP_Module_overlay;
                        section .ptextnandra_utils;
                        section .ptextnandhalcorrectecc;
                    }               // amode 
                }                   // cluster
           }                        // block    

        }                           // space


        space X {
            // X address space
            block XBootBlock {
                // X-space memory range
                cluster X_boot_cluster {
                    amode X_near {
                        // near addressing mode:
                        // locate writable sections
                        section selection=w;
                    }
                    amode X_io {
                        // io addressing mode:
                        // locate writable sections
                                   section selection=w;
                    }
                    amode X_far {
                        label eX_BOOT_MEM_START;
                        // Flash buffer is split into 2 pieces Flash__X and
                        // Flash_Buf_X.
                        section Flash__X addr=INT_X_BTRM_ORG;
                        overlay {
                        // This assumes the Flash buffer is located at the end
                          // of Flash__X.  Since Flash and USB are exclusive, we
                          // can overlay.  TOVERIFY  review this                        
                          section Flash_Buf_X;                         
                          section .xbssusb20devapi;
                        }
                    }
                }           
            }

            block XUsb20Block  {
                 cluster X_app_cluster {
                     amode X_far {
                         label eX_USB20_BASE_ADDRESS;
                         section USB20_Device_Driver addr=INT_X_USB20_ORG;
                         section USB20_Buffer;
                         section selection=w;                    
                     }  // amode
                 }   // cluster
             }   // block
		}//space

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
            }//block
		}//space
    }
}


// include cpu part and memory part
cpu ..\..\..\..\..\..\cpufiles\STMP3500.cpu
memory ..\..\..\..\..\..\cpufiles\stmp3500.mem
