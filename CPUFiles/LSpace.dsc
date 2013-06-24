        space L {
            // L address space
            block L_Block {
                // L-space memory range
                cluster L_clstr {
                        attribute w;
                    amode L_near {
                        section selection=w;
                    }
                    amode L_far {
//  Tasking algorithm places the L cluster at the bottom of RAM. This does not allow us to place a large
//  modulo buffer in X ram. Force it locate else where.
                        contiguous addr=INT_L_CLSTR_ORG{
                            section .Lovl;  
                            section .lovl;              
                            section NormTable;       
                            section WMA_LongVariables;
                            section .lpcCoef;
                            section .lpc;
                        }
                    }
                }
            }
        }
