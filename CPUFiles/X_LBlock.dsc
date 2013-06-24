            block X_LBlock {
                cluster L_TablesX {
                    amode X_far {
                        label eX_app_cluster;
                        label b_WMA_LX;
                        section .RLCOffsetTableX;
                        section .lpcconstsX;         
                        section .lpctablesX;
                        reserved label=e_WMA_LX length=1;       // Stupid label
                    }
                }
            }
