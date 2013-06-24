;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; Encoder Module       
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(encoderequ))
encoderequ   equ     1

ADPCM_BLOCK_SIZE            equ     512
ADPCM_SOURCE_BUF_BLOCK_SIZE equ     2*ADPCM_BLOCK_SIZE
ADPCM_SOURCE_BUF_SIZE       equ     2048
ADPCM_SOURCE_BUF_MODULO     equ     ADPCM_SOURCE_BUF_SIZE-1

ADPCM_SINK_BUF_BLOCK_SIZE   equ     512
ADPCM_SINK_BUF_SIZE         equ     512
ADPCM_SINK_BUF_MODULO       equ     ADPCM_SINK_BUF_SIZE-1

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

 if !@def(NUM_ADPCM_BUFS)
NUM_ADPCM_BUFS      equ 20 
NUM_ADPCM_BUFS_M1   equ (NUM_ADPCM_BUFS-1)
 endif

ADPCM_NSAMPLESPERSEC_8000   equ     8000
ADPCM_NSAMPLESPERSEC_11025  equ     11025
ADPCM_NSAMPLESPERSEC_16000  equ     16000
ADPCM_NSAMPLESPERSEC_22050  equ     22050
ADPCM_NSAMPLESPERSEC_32000  equ     32000
ADPCM_NSAMPLESPERSEC_44100  equ     44100
ADPCM_NSAMPLESPERSEC_48000  equ     48000


                            ; Calcuate the ADCSRR (A/D sample rate) setting like this:
                            ;   ADCSRR = $010000 * ((Fanalog/Fsamples) - 1)
                            ;            ($010000 is 2^16)
                            ; where Fanalog is the analog sampling rate (e.g. 1.024MHz or higher)
                            ; and Fsamples is the digital sampling rate WITHOUT DECIMATION
                            ; (Note: To properly reduce sigma-delta noise, the digital sampling rate
                            ; should be 8X the desired final sampling rate; the data from the A/D
                            ; should be fed through an 8X filter-decimator.  In the current implementation
                            ; of ADPCM, for voice record, the filter-decimator is absent, so Fsamples
                            ; is at the final rate, and there is significant noise in the signal.
                            ; **THIS WILL CHANGE** when we add a software decimation filter.)
    if @def('DEVICE_3410')
    ; For the 3410, Xtal=24.576MHz and CCR_ADIV=24 so Fanalog=1.024MHz
    ;  CCR_ADIV is set in AdcDriverEnablePort
ADPCM_ADCSRR_8000           equ     $0F0000         ; 8X oversampling
ADPCM_ADCSRR_11025          equ     $0A9C27         ; 8X
ADPCM_ADCSRR_16000          equ     $070000         ; 8X
ADPCM_ADCSRR_22050          equ     $04CE14         ; 8X
ADPCM_ADCSRR_32000          equ     $030000         ; 8X
ADPCM_ADCSRR_44100          equ     $01E70A         ; 8X
ADPCM_ADCSRR_48000          equ     $01AAAB         ; 8X
    endif
    if @def('DEVICE_3500')
    ; For the 3500, Xtal=24.0MHz and CCR_ADIV=4 so Fanalog=6.0MHz.
    ;  CCR_ADIV is set in AdcDriverEnablePort

; OSR Values corresponding to FAnalog = 1MHz
ADPCM_ADCSRR_8000           equ     $0EA000         ; 8X oversampling
ADPCM_ADCSRR_11025          equ     $0A567E         ; 8X
ADPCM_ADCSRR_16000          equ     $06D000         ; 8X

; OSR Values corresponding to FAnalog = 6MHz
ADPCM_ADCSRR_22050          equ     $21037c			; 8X
ADPCM_ADCSRR_32000          equ     $167000			; 8X
ADPCM_ADCSRR_44100          equ     $1001BD         ; 8X
ADPCM_ADCSRR_48000          equ     $0EA000         ; 8X

    endif
    
; Scaling factors for ADC data.  These are mostly empirical values.
; Full-scale from the ADC is supposed to be 160*(OSR)^2
; NOTE: Low pass filter after Sigma Delta has a gain of 18dB. If LPF is enabled 
;		following ADC gains need to be adjusted accordingly.

; Scalefactors corresponding to FAnalog = 1MHz and with LPF 
ADPCM_ADCGAIN_8000           equ     13	
ADPCM_ADCGAIN_11025          equ     26
ADPCM_ADCGAIN_16000          equ     53

; Scalefactors corresponding to FAnalog = 6MHz and with LPF
ADPCM_ADCGAIN_22050          equ     3		
ADPCM_ADCGAIN_32000          equ     7		
ADPCM_ADCGAIN_44100          equ     12	
ADPCM_ADCGAIN_48000          equ     16	

; Calculate the InvSampleRateScale like this:
;   InvSampleRateScale = 4096/nsamplespersec * 2^23
ADPCM_INVSAMPLERATESCALE_8000   equ     $418937
ADPCM_INVSAMPLERATESCALE_11025  equ     $2F8DF2
ADPCM_INVSAMPLERATESCALE_16000  equ     $20C49B
ADPCM_INVSAMPLERATESCALE_22050  equ     $17C6F9
ADPCM_INVSAMPLERATESCALE_32000  equ     $10624D
ADPCM_INVSAMPLERATESCALE_44100  equ     $0BE37C
ADPCM_INVSAMPLERATESCALE_48000  equ     $0AEC34


ADPCM_NAVGBYTESPERSEC_8000  equ     4000
ADPCM_NAVGBYTESPERSEC_11025 equ     5513
ADPCM_NAVGBYTESPERSEC_16000 equ     8000
ADPCM_NAVGBYTESPERSEC_22050 equ     11025
ADPCM_NAVGBYTESPERSEC_32000 equ     16000
ADPCM_NAVGBYTESPERSEC_44100 equ     22050
ADPCM_NAVGBYTESPERSEC_48000 equ     24000


                            ; Calculate the ClusterTimeScale like this:
                            ;   ClusterTimeScale = (wsamplesperblock/nblockalign/nsamplespersec)*(2^32)
                            ; where
                            ;   wsamplesperblock = ADPCM_BLOCK_SIZE*2-8+1
                            ;   nblockalign = number of bytes per block (typically=512)
ADPCM_CLUSTERTIMESCALE_8000     equ     $1045A2
ADPCM_CLUSTERTIMESCALE_11025    equ     $0BCEAE
ADPCM_CLUSTERTIMESCALE_16000    equ     $0822D1
ADPCM_CLUSTERTIMESCALE_22050    equ     $05E757
ADPCM_CLUSTERTIMESCALE_32000    equ     $041168
ADPCM_CLUSTERTIMESCALE_44100    equ     $02F3AC
ADPCM_CLUSTERTIMESCALE_48000    equ     $02B646



DEFAULT_ADPCM_ADCSRR                equ     ADPCM_ADCSRR_8000
DEFAULT_ADPCM_ADCGAIN               equ     ADPCM_ADCGAIN_8000
DEFAULT_ADPCM_NSAMPLESPERSEC        equ     ADPCM_NSAMPLESPERSEC_8000
DEFAULT_ADPCM_INVSAMPLERATESCALE    equ     ADPCM_INVSAMPLERATESCALE_8000
DEFAULT_ADPCM_NAVGBYTESPERSEC       equ     ADPCM_NAVGBYTESPERSEC_8000
DEFAULT_ADPCM_CLUSTERTIMESCALE      equ     ADPCM_CLUSTERTIMESCALE_8000

    ENDIF   ; IF (!@def(encoderequ))
