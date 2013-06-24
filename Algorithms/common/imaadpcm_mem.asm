;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; File : imaadpcm_mem.asm
; Description : IMA ADPCM specific memory
;///////////////////////////////////////////////////////////////////////////////
    
    if @def('IMA_ADPCM')
    section IMA_ADPCM_Mem        
;///////////////////////////////////////////////////////////////////////////////
;       Globals
;///////////////////////////////////////////////////////////////////////////////

    global IMA_pred0,IMA_index0,IMAIndexTable,IMAStepSizeTable
    global FIMA_pred0,FIMA_index0,FIMA_pred1,FIMA_index1

;///////////////////////////////////////////////////////////////////////////////
;       Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"IMAADPCM_Data_Y":
; Do not change the order of the following four variables.    
; Channel 0
FIMA_pred0
IMA_pred0                 ds         1
FIMA_index0
IMA_index0                ds         1
; Channel 1
FIMA_pred1
IMA_pred1                 ds         1
FIMA_index1
IMA_index1                ds         1


    org x,"IMAADPCM_Tables_X":
IMAIndexTable
                        DC              -1
                        DC              -1
                        DC              -1
                        DC              -1
                        DC              2
                        DC              4
                        DC              6
                        DC              8
                        DC              -1
                        DC              -1
                        DC              -1
                        DC              -1
                        DC              2
                        DC              4
                        DC              6
                        DC              8


IMAStepSizeTable
                        DC              $7
                        DC              $8
                        DC              $9
                        DC              $a
                        DC              $b
                        DC              $c
                        DC              $d
                        DC              $e
                        DC              $10 
                        DC              $11 
                        DC              $13 
                        DC              $15 
                        DC              $17 
                        DC              $19 
                        DC              $1c 
                        DC              $1f 
                        DC              $22 
                        DC              $25 
                        DC              $29 
                        DC              $2d 
                        DC              $32 
                        DC              $37 
                        DC              $3c 
                        DC              $42 
                        DC              $49 
                        DC              $50 
                        DC              $58 
                        DC              $61 
                        DC              $6b 
                        DC              $76 
                        DC              $82 
                        DC              $8f 
                        DC              $9d 
                        DC              $ad 
                        DC              $be 
                        DC              $d1 
                        DC              $e6 
                        DC              $fd 
                        DC              $117 
                        DC              $133 
                        DC              $151 
                        DC              $173 
                        DC              $198 
                        DC              $1c1 
                        DC              $1ee 
                        DC              $220 
                        DC              $256 
                        DC              $292 
                        DC              $2d4 
                        DC              $31c 
                        DC              $36c 
                        DC              $3c3 
                        DC              $424 
                        DC              $48e 
                        DC              $502 
                        DC              $583 
                        DC              $610 
                        DC              $6ab 
                        DC              $756 
                        DC              $812 
                        DC              $8e0 
                        DC              $9c3 
                        DC              $abd 
                        DC              $bd0 
                        DC              $cff 
                        DC              $e4c 
                        DC              $fba 
                        DC              $114c
                        DC              $1307
                        DC              $14ee
                        DC              $1706
                        DC              $1954
                        DC              $1bdc
                        DC              $1ea5
                        DC              $21b6
                        DC              $2515
                        DC              $28ca
                        DC              $2cdf
                        DC              $315b
                        DC              $364b
                        DC              $3bb9
                        DC              $41b2
                        DC              $4844
                        DC              $4f7e
                        DC              $5771
                        DC              $602f
                        DC              $69ce
                        DC              $7462
                        DC              $7fff

    endsec
    endif
