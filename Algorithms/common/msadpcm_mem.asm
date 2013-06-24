;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; File : msadpcm_mem.asm
; Description : MS ADPCM specific memory
;///////////////////////////////////////////////////////////////////////////////
    
    if @def('MS_ADPCM')
    section MSADPCM_Mem        
;///////////////////////////////////////////////////////////////////////////////
;       Globals
;///////////////////////////////////////////////////////////////////////////////

        global first_block_flag,Ffirst_block_flag
        global iDeltaEnc,FiDeltaEnc       
        global AdaptionTable
        global iSamp2,iSamp1,iCoef2,iCoef1,iPredSample,iDelta     

;///////////////////////////////////////////////////////////////////////////////
;       Y Memory
;///////////////////////////////////////////////////////////////////////////////

        org y,"MSADPCM_Mem_Y":
Ffirst_block_flag
first_block_flag        ds              1

; Do not change the order of the following six variables.    
iSamp2                  ds          2
iSamp1                  ds          2
iCoef2                  ds          2
iCoef1                  ds          2
iPredSample             ds          2
iDelta                  ds          2

FiDeltaEnc
iDeltaEnc               ds              2
AdaptionTable
                        dc              230
                        dc              230
                        dc              230
                        dc              230
                        dc              307
                        dc              409
                        dc              512
                        dc              614
                        dc              768
                        dc              614
                        dc              512
                        dc              409
                        dc              307
                        dc              230
                        dc              230
                        dc              230

    endsec
    endif
