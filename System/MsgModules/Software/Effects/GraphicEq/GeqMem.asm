;;***********************************************************************
;SigmaTel, Inc
;$Archive:  $
;$Revision: 3 $
;$Date:  $
;Description: 
;Notes:	Graphic Eq coefficients, dBTables. etc.
;		
;***********************************************************************/
	Section	GeqMem

;*******************************************************************************
;  Include files
;*******************************************************************************
        include "Geqequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
		global	FilterCoefs
		global	dBTable,ZerodBPoint
		global  currentdBgainbase,currentgainbase,TargetdBGainBase	
		global  x_filter_state_base_odd,x_filter_state_base_even
		global	y_filter_state_base_odd,y_filter_state_base_even 	
        global Geq_Quadrant  
        global Geq_sn        
        global Geq_cs        
        global Geq_alpha_by_4
        global Filter_1_by_2Q
        global CenterFreq




;*******************************************************************************
;  X memory
;*******************************************************************************


	org y,"GeqCoefmem":
; The filter coefficients for each of the bandpass filters. These values are obtained
; using filtdes.c. Sampling Frequency = 48000 Hz.
;-------------------------------------
;  Type			Center freq		   Q
;-------------------------------------
; bandpass        80              1	       
; bandpass        250             1          
; bandpass        1000            1          
; bandpass        4000            1          
; bandpass        12000           1          

        buffer  M,NumberOfBands*3
FilterCoefs
			DC		0.49495327			; a2
			DC		0.00252335			; b0
			DC		0.99492043			; a1

			DC		0.47996041
			DC		0.01001981
			DC		0.97943568

			DC		0.42457429
			DC		0.03771286
			DC		0.91666448

			DC		0.26190478
			DC		0.11904762
			DC		0.65982884

			DC		0.14881733
			DC		0.17559133
			DC		-0.32440868
 	endbuf

currentgainbase					DSM			NumberOfBands

currentdBgainbase				DS			NumberOfBands

TargetdBGainBase	
		 	DC			ZerodBPoint
		 	DC			ZerodBPoint
		 	DC			ZerodBPoint
		 	DC			ZerodBPoint
		 	DC			ZerodBPoint


; dBTable: Converts dB to actual gain. Entries are in steps of 0.5 dB.
; The first entry corresponds to cut of -14 dB
; The last entry corresponds to a boost of +14 dB.
; Zero dB point corresponds to zero gain, for the filter, giving a flat response.
; dB table computation
; for i=1:57,
;	A(i)=(10^((i-29)/40)-1)/4;
; end;					

; Explaination:
; A(i)=(10^((i-29)/40)-1)/4;
;                     ^   ^   
; Output is scaled by 4 in the code.
; Subtraction by 1 to compensate for feedforward path.

		org x,".GEqdBTable":
dBTable
		dc -0.200118442126		 ;  	-14.0	 dB
		dc -0.197162774004		 ; 		-13.5	 dB
		dc -0.194031971536		 ; 		-13.0	 dB
		dc -0.190715657358		 ; 		-12.5	 dB
		dc -0.187202839212		 ; 		-12.0	 dB
		dc -0.183481873505		 ; 		-11.5	 dB
		dc -0.179540426718		 ; 		-11.0	 dB
		dc -0.175365434527		 ; 		-10.5	 dB
		dc -0.170943058496		 ; 		-10.0	 dB
		dc -0.166258640211		 ; 		-9.5	 dB
		dc -0.161296652692		 ; 		-9.0	 dB
		dc -0.156040648928		 ; 		-8.5	 dB
		dc -0.150473207362		 ; 		-8.0	 dB
		dc -0.144575874143		 ; 		-7.5	 dB
		dc -0.138329101962		 ; 		-7.0	 dB
		dc -0.131712185260		 ; 		-6.5	 dB
		dc -0.124703191593		 ; 		-6.0	 dB
		dc -0.117278888942		 ; 		-5.5	 dB
		dc -0.109414668702		 ; 		-5.0	 dB
		dc -0.101084464118		 ; 		-4.5	 dB
		dc -0.092260663880		 ; 		-4.0	 dB
		dc -0.082914020608		 ; 		-3.5	 dB
		dc -0.073013553904		 ; 		-3.0	 dB
		dc -0.062526447667		 ; 		-2.5	 dB
		dc -0.051417941319		 ; 		-2.0	 dB
		dc -0.039651214589		 ; 		-1.5	 dB
		dc -0.027187265467		 ; 		-1.0	 dB
		dc -0.013984780929		 ; 		-0.5	 dB
ZerodBPoint						 
		dc 	0					 ; 		  0		 dB
		dc 	0.014813431294		 ; 		 0.5	 dB
		dc 	0.030504613575		 ; 		 1.0	 dB
		dc 	0.047125556859		 ; 		 1.5	 dB
		dc 	0.064731352949		 ; 		 2.0	 dB
		dc 	0.083380358041		 ; 		 2.5	 dB
		dc 	0.103134386156		 ; 		 3.0	 dB
		dc 	0.124058914024		 ; 		 3.5	 dB
		dc 	0.146223298115		 ; 		 4.0	 dB
		dc 	0.169701004531		 ; 		 4.5	 dB
		dc 	0.194569852510		 ; 		 5.0	 dB
		dc 	0.220912272372		 ; 		 5.5	 dB
		dc 	0.248815578742		 ; 		 6.0	 dB
		dc 	0.278372259959		 ; 		 6.5	 dB
		dc 	0.309680284642		 ; 		 7.0	 dB
		dc 	0.342843426415		 ; 		 7.5	 dB
		dc 	0.377971607877		 ; 		 8.0	 dB
		dc 	0.415181264950		 ; 		 8.5	 dB
		dc 	0.454595732816		 ; 		 9.0	 dB
		dc 	0.496345654729		 ; 		 9.5	 dB
		dc 	0.540569415042		 ; 		 10.0	 dB
		dc 	0.587413597895		 ; 		 10.5	 dB
		dc 	0.637033473084		 ; 		 11.0	 dB
		dc 	0.689593510721		 ; 		 11.5	 dB
		dc 	0.745267926384		 ; 		 12.0	 dB
		dc 	0.804241258571		 ; 		 12.5	 dB
		dc 	0.866708980377		 ; 		 13.0	 dB
		dc 	0.932878147404		 ; 		 13.5	 dB
		dc 	$7fffff				 ; 	   ~ 14.0	 dB

    ; Other GeQ data
; These values are over-ridden by GEQ_SET_PARAMS message.
; Corresponding Q for each filter.
Filter_1_by_2Q      dc          0.5         ; Q = 1
                    dc          0.5
                    dc          0.5
                    dc          0.5
                    dc          0.5


; Band Pass Filter Center Frequencies
CenterFreq          dc          80
                    dc          250
                    dc          1000
                    dc          4000
                    dc          12000

Geq_Quadrant        ds          1
Geq_sn              ds          1
Geq_cs              ds          1
Geq_alpha_by_4      ds          1



	org x,"GeqFilterStateMem":

x_filter_state_base_odd			DSM			2*MAX_CHANNELS
x_filter_state_base_even		DSM			2*MAX_CHANNELS
y_filter_state_base_odd 		DSM			2*NumberOfBands*MAX_CHANNELS
y_filter_state_base_even 		DSM			2*NumberOfBands*MAX_CHANNELS

	EndSec


