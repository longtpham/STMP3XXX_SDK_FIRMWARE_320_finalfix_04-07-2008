        page 255,255

AdcBlockSize          equ     $40             ; The size of the block we process at each interrupt
AdcBufSize            equ     $100            ; The buffer size dedicated to the ADC port
AdcBufModulo          equ     AdcBufSize-1    ; The modulo of the ADC buffer 
AdcFlt2NTaps		  equ	  20	
AdcFlt3NTaps		  equ	  70	
