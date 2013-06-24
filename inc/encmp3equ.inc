;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel Inc
;
; MP3 Encoder Module       
;/////////////////////////////////////////////////////////////////////////////// 

MP3ENCODE_BLOCK_SIZE            equ     1152
MP3ENCODE_SOURCE_BUF_BLOCK_SIZE equ     2*MP3ENCODE_BLOCK_SIZE
MP3ENCODE_SOURCE_BUF_SIZE       equ     2048*2
MP3ENCODE_SOURCE_BUF_MODULO     equ     MP3ENCODE_SOURCE_BUF_SIZE-1

MP3ENCODE_SINK_BUF_BLOCK_SIZE   equ     512
MP3ENCODE_SINK_BUF_SIZE         equ     8192
MP3ENCODE_SINK_BUF_MODULO       equ     MP3ENCODE_SINK_BUF_SIZE-1


ENC_ISR_STATE_STOPPED       equ     0
ENC_ISR_STATE_RECORDING     equ     1
ENC_ISR_STATE_PAUSED        equ     2


 if !@def(ENC_MIN_CLUSTERS)
ENC_MIN_CLUSTERS            equ     8
 endif

; Made this for the value 2 that was already in use. 
; NOTE: Total rec time remaining needs to exclude these reserved clusters in its calculation.
; As of Apr 11, 2002, ver tot time left does not seem to factor in these reserved clusters.
 if !@def(ENC_MIN_FREE_CLUSTER_LIM)
ENC_MIN_FREE_CLUSTER_LIM    equ     2
 endif


; With bytes/cluster of 16,384. /4000 bytes/sec adpcm data =  4.096 sec/cluster
; (at 32kbps)
;  A 3 cluster threshhold warning above record free cluster limit gives ~12 sec warning.    
;  Note: Alternate approach checks remaining record bytes to get seconds but is probably not worth the system performance  
;        hit as 'warning needed' is checked in the high frequency periodic timer ISR. Just check the free clusters.

 if !@def(ENC_FREE_CLUSTERS_BEFORE_WARNING)
ENC_FREE_CLUSTERS_BEFORE_WARNING equ 3
 endif

 if !@def(ENC_FREE_CLUSTERS_WARN_THRESHOLD)
ENC_FREE_CLUSTERS_WARN_THRESHOLD equ ENC_MIN_FREE_CLUSTER_LIM+ENC_FREE_CLUSTERS_BEFORE_WARNING
 endif
