/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000/2001
// Encoder Module       
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(encoderequ))
#define encoderequ 1

#define ADPCM_BLOCK_SIZE 512
#define ADPCM_SOURCE_BUF_BLOCK_SIZE 2*ADPCM_BLOCK_SIZE
#define ADPCM_SOURCE_BUF_SIZE 2048
#define ADPCM_SOURCE_BUF_MODULO ADPCM_SOURCE_BUF_SIZE-1

#define ADPCM_SINK_BUF_BLOCK_SIZE 512
#define ADPCM_SINK_BUF_SIZE 512
#define ADPCM_SINK_BUF_MODULO ADPCM_SINK_BUF_SIZE-1

#define ENC_ISR_STATE_STOPPED 0
#define ENC_ISR_STATE_RECORDING 1
#define ENC_ISR_STATE_PAUSED 2


#if !defined(ENC_MIN_CLUSTERS)
#define ENC_MIN_CLUSTERS 8
#endif

// Made this for the value 2 that was already in use. 
// NOTE: Total rec time remaining needs to exclude these reserved clusters in its calculation.
// As of Apr 11, 2002, ver tot time left does not seem to factor in these reserved clusters.
#if !defined(ENC_MIN_FREE_CLUSTER_LIM)
#define ENC_MIN_FREE_CLUSTER_LIM 2
#endif


// With bytes/cluster of 16,384. /4000 bytes/sec adpcm data =  4.096 sec/cluster
// (at 32kbps)
//  A 3 cluster threshhold warning above record free cluster limit gives ~12 sec warning.    
//  Note: Alternate approach checks remaining record bytes to get seconds but is probably not worth the system performance  
//        hit as 'warning needed' is checked in the high frequency periodic timer ISR. Just check the free clusters.

#if !defined(ENC_FREE_CLUSTERS_BEFORE_WARNING)
#define ENC_FREE_CLUSTERS_BEFORE_WARNING 3
#endif

#if !defined(ENC_FREE_CLUSTERS_WARN_THRESHOLD)
#define ENC_FREE_CLUSTERS_WARN_THRESHOLD ENC_MIN_FREE_CLUSTER_LIM+ENC_FREE_CLUSTERS_BEFORE_WARNING
#endif

#if !defined(NUM_ADPCM_BUFS)
#define NUM_ADPCM_BUFS 20 
#define NUM_ADPCM_BUFS_M1 (NUM_ADPCM_BUFS-1)
#endif

#define ADPCM_NSAMPLESPERSEC_8000 8000
#define ADPCM_NSAMPLESPERSEC_11025 11025
#define ADPCM_NSAMPLESPERSEC_16000 16000
#define ADPCM_NSAMPLESPERSEC_22050 22050
#define ADPCM_NSAMPLESPERSEC_32000 32000
#define ADPCM_NSAMPLESPERSEC_44100 44100
#define ADPCM_NSAMPLESPERSEC_48000 48000


// Calcuate the ADCSRR (A/D sample rate) setting like this:
//   ADCSRR = $010000 * ((Fanalog/Fsamples) - 1)
//            ($010000 is 2^16)
// where Fanalog is the analog sampling rate (e.g. 1.024MHz or higher)
// and Fsamples is the digital sampling rate WITHOUT DECIMATION
// (Note: To properly reduce sigma-delta noise, the digital sampling rate
// should be 8X the desired final sampling rate; the data from the A/D
// should be fed through an 8X filter-decimator.  In the current implementation
// of ADPCM, for voice record, the filter-decimator is absent, so Fsamples
// is at the final rate, and there is significant noise in the signal.
// **THIS WILL CHANGE** when we add a software decimation filter.)
#if defined(DEVICE_3410)
// For the 3410, Xtal=24.576MHz and CCR_ADIV=24 so Fanalog=1.024MHz
//  CCR_ADIV is set in AdcDriverEnablePort
// 8X oversampling
#define ADPCM_ADCSRR_8000 0x0F0000         
// 8X
#define ADPCM_ADCSRR_11025 0x0A9C27         
// 8X
#define ADPCM_ADCSRR_16000 0x070000         
// 8X
#define ADPCM_ADCSRR_22050 0x04CE14         
// 8X
#define ADPCM_ADCSRR_32000 0x030000         
// 8X
#define ADPCM_ADCSRR_44100 0x01E70A         
// 8X
#define ADPCM_ADCSRR_48000 0x01AAAB         
#endif
#if defined(DEVICE_3500)
// For the 3500, Xtal=24.0MHz and CCR_ADIV=4 so Fanalog=6.0MHz.
//  CCR_ADIV is set in AdcDriverEnablePort

// OSR Values corresponding to FAnalog = 1MHz
// 8X oversampling
#define ADPCM_ADCSRR_8000 0x0EA000         
// 8X
#define ADPCM_ADCSRR_11025 0x0A567E         
// 8X
#define ADPCM_ADCSRR_16000 0x06D000         

// OSR Values corresponding to FAnalog = 6MHz
// 8X
#define ADPCM_ADCSRR_22050 0x21037c			
// 8X
#define ADPCM_ADCSRR_32000 0x167000			
// 8X
#define ADPCM_ADCSRR_44100 0x1001BD         
// 8X
#define ADPCM_ADCSRR_48000 0x0EA000         

#endif

// Scaling factors for ADC data.  These are mostly empirical values.
// Full-scale from the ADC is supposed to be 160*(OSR)^2
// NOTE: Low pass filter after Sigma Delta has a gain of 18dB. If LPF is enabled 
//		following ADC gains need to be adjusted accordingly.

// Scalefactors corresponding to FAnalog = 1MHz and with LPF 
#define ADPCM_ADCGAIN_8000 13	
#define ADPCM_ADCGAIN_11025 26
#define ADPCM_ADCGAIN_16000 53

// Scalefactors corresponding to FAnalog = 6MHz and with LPF
#define ADPCM_ADCGAIN_22050 3		
#define ADPCM_ADCGAIN_32000 7		
#define ADPCM_ADCGAIN_44100 12	
#define ADPCM_ADCGAIN_48000 16	

// Calculate the InvSampleRateScale like this:
//   InvSampleRateScale = 4096/nsamplespersec * 2^23
#define ADPCM_INVSAMPLERATESCALE_8000 0x418937
#define ADPCM_INVSAMPLERATESCALE_11025 0x2F8DF2
#define ADPCM_INVSAMPLERATESCALE_16000 0x20C49B
#define ADPCM_INVSAMPLERATESCALE_22050 0x17C6F9
#define ADPCM_INVSAMPLERATESCALE_32000 0x10624D
#define ADPCM_INVSAMPLERATESCALE_44100 0x0BE37C
#define ADPCM_INVSAMPLERATESCALE_48000 0x0AEC34


#define ADPCM_NAVGBYTESPERSEC_8000 4000
#define ADPCM_NAVGBYTESPERSEC_11025 5513
#define ADPCM_NAVGBYTESPERSEC_16000 8000
#define ADPCM_NAVGBYTESPERSEC_22050 11025
#define ADPCM_NAVGBYTESPERSEC_32000 16000
#define ADPCM_NAVGBYTESPERSEC_44100 22050
#define ADPCM_NAVGBYTESPERSEC_48000 24000


// Calculate the ClusterTimeScale like this:
//   ClusterTimeScale = (wsamplesperblock/nblockalign/nsamplespersec)*(2^32)
// where
//   wsamplesperblock = ADPCM_BLOCK_SIZE*2-8+1
//   nblockalign = number of bytes per block (typically=512)
#define ADPCM_CLUSTERTIMESCALE_8000 0x1045A2
#define ADPCM_CLUSTERTIMESCALE_11025 0x0BCEAE
#define ADPCM_CLUSTERTIMESCALE_16000 0x0822D1
#define ADPCM_CLUSTERTIMESCALE_22050 0x05E757
#define ADPCM_CLUSTERTIMESCALE_32000 0x041168
#define ADPCM_CLUSTERTIMESCALE_44100 0x02F3AC
#define ADPCM_CLUSTERTIMESCALE_48000 0x02B646



#define DEFAULT_ADPCM_ADCSRR ADPCM_ADCSRR_8000
#define DEFAULT_ADPCM_ADCGAIN ADPCM_ADCGAIN_8000
#define DEFAULT_ADPCM_NSAMPLESPERSEC ADPCM_NSAMPLESPERSEC_8000
#define DEFAULT_ADPCM_INVSAMPLERATESCALE ADPCM_INVSAMPLERATESCALE_8000
#define DEFAULT_ADPCM_NAVGBYTESPERSEC ADPCM_NAVGBYTESPERSEC_8000
#define DEFAULT_ADPCM_CLUSTERTIMESCALE ADPCM_CLUSTERTIMESCALE_8000

#endif // IF (!@def(encoderequ))

