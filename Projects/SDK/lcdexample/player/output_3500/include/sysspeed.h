/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
// Speed defs
// NOTE: sysspeed.H is generated from sysspeed.INC. Only modify inc version.
// Speed clients: See SPEED_CLIENT_ defines and non 3400 Speed Indeces
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(SYSSPEED_INC))
#define SYSSPEED_INC 1

// PLL Frequency Divider (Assume DDIV=0)
// PLL Output Frequency = (33+PDIV)*(XTAL/20)   XTAL=24 MHz
// PDIV = (Speed * 20 / 24) - 33  (if DDIV=0)
// Digital Clock Post Divider 
// Digital Clock = PLL Output Fequency / 2^DDIV   (Speed is in MHz)
// GP Flash Timing Registers
// x:HW_GPFLASH_TIMING1R
//      TSU = 0ns 
//      TH = 10ns Min
// Use Default Settings since max speed is 75.6 MHz or 13.2275 ns
// Add Register if needed
// x:HW_GPFLASH_TIMING2R 
//      TDS = 25ns Min  data setup time
//      TDH = 15ns Min  data hold  time
//      TDS + TDH must be > 50 nS
// Speed Indexes (into the speed table)
//!!!!! ** MUST BE ORDERED FROM LOWEST TO HIGHEST FREQ ** !!!!!
// NOTE: Executive speed client must only use speed index 1 or 0 (IDLE).
#define SPEED_IDLE 0 
#define SPEED_AUDIBLE_MP3 1
#define SPEED_AUDIBLE_ACELP_SR8KHz 2
#define SPEED_FM 3
#define SPEED_FM_EQ 4
#define SPEED_AUDIBLE_ACELP_SR16KHz 5
#define SPEED_MP3 6
#define SPEED_ADPCM 7
#define SPEED_MIXER 8
#define SPEED_ADCBASE 9
#define SPEED_MAX 10
#define SPEED_WMA 11
#define SPEED_MP3ENCODE 12
#define SPEED_PEAK 13
#define SPEED_LAST SPEED_PEAK
#define SPEED_UNINITIALIZED SPEED_LAST+1
// SPEED_UNINITIALIZED must be the highest index value

// Idle speed is 10 MHz  (98.04nS)
#define IDLE_SPEED 10              
// (1+33)*(24/20) = 40.8
#define IDLE_PDIV 1               
// 40.5504 / (2^2) = 10.2 MHz
#define IDLE_DDIV 2               
// 1.37v
#define IDLE_VOLTAGE 0x00000B         
// 1.28v 
#define IDLE_BOVOLTAGE 0x000800         


// tdh and tds: hold and setup times in n dclks are 6-bit fields that are right justified byte aligned
// 60MHz MSC uses the following clock setup: 
// NAND1     $000203 -> (2+3)/60MHz =  83.3ns   2 dclk hold and 3 dclk setup
// NAND2     $000304 -> (3+4)/60MHz = 116.7ns
// NAND3     $000305 -> (3+5)/60MHz = 133.3ns
// NAND4     $000305 -> (3+5)/60MHz = 133.3ns
// So, for 10MHz idle speed:
// NAND1,2,4 may use $000101 
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define IDLE_GPSPEED 0x000101         
#endif
//---------    

// speed is 15 MHz(66.6nS)
#define AUDIBLE_MP3_SPEED 15              
// PLL O/P freq = (33 + PDIV)*(24/20) = (33 + 17)*(24/20) = 60MHz
#define AUDIBLE_MP3_PDIV 17              
// DCLK = 40.8MHz/(2^DDIV) = 15MHz
#define AUDIBLE_MP3_DDIV 2               
// 1.37V
#define AUDIBLE_MP3_VOLTAGE 0x00000B         
// 1.28V 
#define AUDIBLE_MP3_BOVOLTAGE 0x000800         

// NAND1     15MHz *  83.3ns = 1.249 -> $000101
// NAND2     15MHz * 116.7ns = 1.750 -> $000101
// NAND3|4   15MHz * 133.3ns = 1.999 -> $000101

#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_MP3_GPSPEED 0x000101         
#endif
//---------   

// speed is 15 MHz(66.6nS)
#define AUDIBLE_ACELP_SR8KHz_SPEED 15              
// PLL O/P freq = (33 + PDIV)*(24/20) = (33 + 17)*(24/20) = 60MHz
#define AUDIBLE_ACELP_SR8KHz_PDIV 17              
// DCLK = 40.8MHz/(2^DDIV) = 15MHz
#define AUDIBLE_ACELP_SR8KHz_DDIV 2               
// 1.37V
#define AUDIBLE_ACELP_SR8KHz_VOLTAGE 0x00000B         
// 1.28V 
#define AUDIBLE_ACELP_SR8KHz_BOVOLTAGE 0x000800         

// NAND1     15MHz *  83.3ns = 1.249 -> $000101
// NAND2     15MHz * 116.7ns = 1.750 -> $000101
// NAND3|4   15MHz * 133.3ns = 1.999 -> $000101

#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR8KHz_GPSPEED 0x000101         
#endif
//--------- 
// Changed speed from 18Mhz to 24Mhz stmp10415
// stmp10415   was 18              ; FM speed is 18 MHz, which is as low as possible for Steely Dan Tuner in sdk 2.610. In 3.110 we need >22Mhz so we use 24Mhz. 
#define FM_SPEED 24 
// stmp10415   was 27              ; (27+33)*(24/20) = 72 in sdk2.610.  In sdk 3.110 we use (7+33)*(24/20) = 48.
#define FM_PDIV 7  
// stmp10415   was  2              ; 72 / (2^2) = 18 MHz in sdk 2.610.    In 3.110 we use 48/2^1 = 24Mhz. 
#define FM_DDIV 1  

// 1.44V	- Steely Dan spec at 1.35V
#define FM_VOLTAGE 0x00000d         
// 1.28v
#define FM_BOVOLTAGE 0x000800         
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_GPSPEED 0x000203         
#endif
//---------
// Increase to 30 MHz to support equalizer
#define FM_EQ_SPEED 30  
// (17+33)*(24/20) = 60
#define FM_EQ_PDIV 17  
//  60 / (2^1) = 30 MHz  
#define FM_EQ_DDIV 1   

// Higher voltages when using equalizer and TA2 STFM1000 to prevent lockup when changing EQ settings rapidly
//1.44v  - Steely Dan spec at 1.35V
#define FM_EQ_VOLTAGE 0x00000D   
//1.31v     
#define FM_EQ_BOVOLTAGE 0x000900   

#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
#define FM_EQ_GPSPEED 0x000203         
#endif
//---------


// Audible Acelp speed is 36 MHz(27.78nS)
#define AUDIBLE_ACELP_SR16KHz_SPEED 36            
// PLL O/P freq = (33 + PDIV)*(24/20)	= (33+27)*(24/20) = 72MHz
#define AUDIBLE_ACELP_SR16KHz_PDIV 27	           
// DCLK = 60MHz/(2^DDIV) = 36MHz
#define AUDIBLE_ACELP_SR16KHz_DDIV 1             
// 1.37V
#define AUDIBLE_ACELP_SR16KHz_VOLTAGE 0x00000B       
// 1.28V 
#define AUDIBLE_ACELP_SR16KHz_BOVOLTAGE 0x000800       

// NAND1     36MHz *  83.3ns = 2.999 -> $000102
// NAND2     36MHz * 116.7ns = 4.201 -> $000203
// NAND4     36MHz * 133.3ns = 4.788 -> $000203
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define AUDIBLE_ACELP_SR16KHz_GPSPEED 0x000102         
#endif
//---------   




// MP3 speed is 36 MHz  (27.78nS)
#define MP3_SPEED 36              
// (27+33)*(24/20) = 72
#define MP3_PDIV 27              
// 72 / (2^1) = 36 MHz
#define MP3_DDIV 1               
// 1.37v
#define MP3_VOLTAGE 0x00000B         
// 1.28v
#define MP3_BOVOLTAGE 0x000800         

// NAND1     36MHz *  83.3ns = 2.999 -> $000102
// NAND2     36MHz * 116.7ns = 4.201 -> $000203
// NAND4     36MHz * 133.3ns = 4.788 -> $000203
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define MP3_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MP3_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MP3_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MP3_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define MP3_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define MP3_GPSPEED 0x000102         
#endif
//---------
// ADPCM speed is 36 MHz  (27.78nS)
#define ADPCM_SPEED 36              
// (27+33)*(24/20) = 72
#define ADPCM_PDIV 27              
// 72 / (2^1) = 36 MHz
#define ADPCM_DDIV 1               
// 1.37v
#define ADPCM_VOLTAGE 0x00000B         
// 1.28v
#define ADPCM_BOVOLTAGE 0x000800         

// NAND1     36MHz *  83.3ns = 2.999 -> $000102
// NAND2     36MHz * 116.7ns = 4.201 -> $000203
// NAND3|4   36MHz * 133.3ns = 4.788 -> $000203
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADPCM_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADPCM_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADPCM_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADPCM_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADPCM_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADPCM_GPSPEED 0x000102         
#endif
//---------
// Mixer use requires a min vddd. Reason for this: 
// ************
// Stmp00004930: Field failure: Distortion from the mixer. Very worst case 
//           conditions (at cold) will cause the ADC to not provide any samples.
//           All mixer/adc functions must run VDDD>=1.43V. This includes encoding
//           and Fmtuner  operation (uses line-in via mixer) in the SDK. 
// ************
// same as Idle speed defined above
#define MIXER_SPEED IDLE_SPEED      
// same as idle case defined above
#define MIXER_PDIV IDLE_PDIV       
// same as idle case defined above
#define MIXER_DDIV IDLE_DDIV       
// 1.44v
#define MIXER_VOLTAGE 0x00000D         
// 1.34v as in the MAX_BOVOLTAGE case below which has same Vddd.
#define MIXER_BOVOLTAGE 0x000A00         

// HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
#define MIXER_GPSPEED IDLE_GPSPEED    
//---------
// ADCBASE speed is 36 MHz  (27.78nS)
#define ADCBASE_SPEED 36              
// (27+33)*(24/20) = 72
#define ADCBASE_PDIV 27              
// 72 / (2^1) = 36 MHz
#define ADCBASE_DDIV 1               
// 1.44v Min vddd voltage to operate high res ADC @ low temp.
#define ADCBASE_VOLTAGE 0x00000D         
// 1.34v Voltages same as max case. Rest matches ADPCM case.
#define ADCBASE_BOVOLTAGE 0x000A00         

// NAND1     36MHz *  83.3ns = 2.999 -> $000102
// NAND2     36MHz * 116.7ns = 4.201 -> $000203
// NAND4     36MHz * 133.3ns = 4.788 -> $000203
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADCBASE_GPSPEED 0x000102         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADCBASE_GPSPEED 0x000203         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADCBASE_GPSPEED 0x000203         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define ADCBASE_GPSPEED 0x000203         
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADCBASE_GPSPEED 0x000102         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
#define ADCBASE_GPSPEED 0x000102         
#endif
//---------
// MAX speed is 48 MHz (20.83ns)
#define MAX_SPEED 48              
// (7+33)*(24/20) = 48
#define MAX_PDIV 7               
// 47.9232 / (2^0) = 48 MHz
#define MAX_DDIV 0               
// 1.44v
#define MAX_VOLTAGE 0x00000D         
// 1.34v
#define MAX_BOVOLTAGE 0x000A00         

// NAND1     48MHz *  83.3ns = 3.998 -> $000202 -> $000203 required (setup needs 3dclk)
// NAND2     48MHz * 116.7ns = 5.602 -> $000303
// NAND3|4   48MHz * 133.3ns = 6.398 -> $000304
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MAX_GPSPEED 0x000203         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
#define MAX_GPSPEED 0x000303         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define MAX_GPSPEED 0x000304         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define MAX_GPSPEED 0x000304         
#else 
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MAX_GPSPEED 0x000203         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define MAX_GPSPEED 0x000203         
#endif
//---------
// WMA speed is 60MHz   (16.67nS)
#define WMA_SPEED 60              
// (17+33)*(24/20) = 60
#define WMA_PDIV 17              
// 60 / (2^0) = 60 MHz
#define WMA_DDIV 0               
// 1.63V
#define WMA_VOLTAGE 0x000013         
// 1.54V
#define WMA_BOVOLTAGE 0x001000         

// Matching values for MSC (see note above for IDLE_GPSPEED)
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define WMA_GPSPEED 0x000203         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define WMA_GPSPEED 0x000304         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 5 for TDS
#define WMA_GPSPEED 0x000305         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 5 for TDS
#define WMA_GPSPEED 0x000305         
#else 
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define WMA_GPSPEED 0x000203         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
#define WMA_GPSPEED 0x000203         
#endif
//---------
// MP3ENCODE speed is 65MHz   (15.35nS) but see next line
#define MP3ENCODE_SPEED 65              
// (22+33)*(24/20) = 65; but   (20+33)*(24/20)=63.6 (T=15.723 ns)
#define MP3ENCODE_PDIV 20              
// 65.1264 / (2^0) = 65 MHz;   63.6 / 2^0 = 63.6 MHz
#define MP3ENCODE_DDIV 0               
// 1.82V
#define MP3ENCODE_VOLTAGE 0x000019         
// 1.76 v 
#define MP3ENCODE_BOVOLTAGE 0x001700         

// NAND1     65MHz *  83.3ns = 5.415 -> $000303
// NAND2     65MHz * 116.7ns = 7.586 -> $000404
// NAND3|4   65MHz * 133.3ns = 8.665 -> $000405
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
#define MP3ENCODE_GPSPEED 0x000303         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 4 for TDS
#define MP3ENCODE_GPSPEED 0x000404         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS
#define MP3ENCODE_GPSPEED 0x000405         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS
#define MP3ENCODE_GPSPEED 0x000405         
#else 
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
#define MP3ENCODE_GPSPEED 0x000303         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
#define MP3ENCODE_GPSPEED 0x000303         
#endif
//---------
// x:HW_GPFLASH_TIMING2R 
//      TDS = 25ns Min data setup time. Note usbmsc says 45 for min. 20ns is a typical nand flash.
//      TDH = 15ns Min data hold  time. Note usbmsc says 30 for min. But has min (tds+tdh)>50ns.
//      TDS + TDH must be > 50 nS
// PEAK speed is 75.6MHz T=13.2275 ns  
#define PEAK_SPEED 75              
// (29+33)*(24/20) = 75.6          
#define PEAK_PDIV 30              
// 75.6 / (2^0) = 75.6 MHz     
#define PEAK_DDIV 0               
// 1.92V 
#define PEAK_VOLTAGE 0x00001c         
// 1.82V                        
#define PEAK_BOVOLTAGE 0x001900         

// DEBUG builds have different setup&hold due to engineering board's capacitance. RETAIL BUILDS USE ONE VALUE. 
// ns times below are from usbmsc's total (tds + tdh) = setup+hold. USBMSC runs at 60 MHz.
// NAND1     75.6 MHz *  83.3ns = 6.247 DCLKs so 7 -> $000304      
// NAND2     75.6 MHz * 116.7ns = 8.823 DCLKs so 9 -> $000405      
// NAND3|4   75.6 MHz * 133.3ns = 10.078 DCLKs     -> $000505 If issues try 000506 here (TDS=6).     
#if (defined(DEBUG))
#if (defined(NAND1))
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define PEAK_GPSPEED 0x000304         
#else 
#if (defined(NAND2))
// HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS; nand1 settings work on multinand if good layout.
#define PEAK_GPSPEED 0x000405         
#else 
#if (defined(NAND3))
// HW_GPFLASH_TIMING2R - 5dclk/access time for TDH and 5 for TDS
#define PEAK_GPSPEED 0x000505         
#else 
#if (defined(NAND4))
// HW_GPFLASH_TIMING2R - 5dclk/access time for TDH and 5 for TDS
#define PEAK_GPSPEED 0x000505         
#else 
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define PEAK_GPSPEED 0x000304         
#endif
#endif
#endif
#endif
#else 
// HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
#define PEAK_GPSPEED 0x000304         
#endif

//------------------------------------------
//> 
// Multi-Client Clock Speed Adjustment API:  
// Prototypes here are exported from sysspeed.inc to sysspeed.h by incToC util
_reentrant INT SysGetSpeed(void);
//             or asm input params: none
//             Returns the CurrentSpeedIndex
_reentrant INT SysSpeedIncrease(int SpeedIndex, int clockUserId); 
//             or asm input parms:            x1,              y0
//       Returns resulting SpeedIndex from SysGetSpeed
_reentrant INT SysSpeedClockFree(int clockUserId);  
//                   or asm input:              y0
//       Returns resulting SpeedIndex from SysGetSpeed
// Generally, new clock adjusting modules must insert a bit equate below 
// (speed ordered) and use the API calls:
// SysSpeedIncrease(SpeedIndex, MY_BIT) and SysSpeedClockFree(MY_BIT) 
// passing in the client's clockUserId below for these calls
// which manipulate a corresponding speed client bit in clockUserFlag.
// SysSpeedClockFree(MY_BIT) reduces clock speed to that of highest active client
// or IDLE. Each bit here is a client active indicator managed via this speed API.
// clockUserFlag  dc      0      ; Module private var defined in sysspeed.asm
// ORDER MUST BE LOWEST TO HIGHEST FREQ so insert your new speed client ID and renumber.
// Exec turns up the clock on button events. 
#define SPEED_CLIENT_EXECUTIVE 0 
#define SPEED_CLIENT_AUDIBLE_MP3 1
#define SPEED_CLIENT_STFM_DEC 2
#define SPEED_CLIENT_AUDIBLE_ACELP 3
#define SPEED_CLIENT_STFM_EQ 4
#define SPEED_CLIENT_MP3_DEC 5
#define SPEED_CLIENT_ADPCM_DEC 6
#define SPEED_CLIENT_ADPCM_ENC 7
#define SPEED_CLIENT_MIXER 8
#define SPEED_CLIENT_MP3_DEC_SYNC 9
// Music and voice parser share this client
#define SPEED_CLIENT_PARSER 10 
// Same speed as parser.asm or higher. C client. Rename if using.
#define SPEED_CLIENT_MENU_WOW 11 
#define SPEED_CLIENT_MVIDEO 12
#define SPEED_CLIENT_METADATA 13
#define SPEED_CLIENT_PLAYERLIB 14
#define SPEED_CLIENT_PLAYLIST1 15 
#define SPEED_CLIENT_WMA_DEC 16
// Move line when used to maintain speed order
#define SPEED_CLIENT_MP3_ENC 17 
#define SPEED_CLIENT_JPEG_DEC 18 
// change value to match high bit pos above
#define SPEED_CLIENT_HIGHEST_USER 18 

// Above equates are exported to incToC utility generated sysspeed.h file.
// <
//-----------------------------------------


#endif // IF (!@def(SYSSPEED_INC))



