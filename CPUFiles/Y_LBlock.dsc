            block Y_LBlock {
                cluster L_TablesY {
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
                    attribute s;//don't locate anything here automatically.  
                    //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
                    amode Y_far {
                        label b_WMA_LY;
                        section .RLCOffsetTableY;
                        section .lpcconstsY;         
                        section .lpctablesY;
                        reserved label=e_WMA_LY length=1;      
                    }
                }
            }
