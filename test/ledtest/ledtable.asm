    nolist
        include "hwequ.inc"
    list


    org   y,"LEDModule.y":
    GLOBAL LedTable
    GLOBAL LED_NUMBER
    GLOBAL LED_TABLE_LENGTH

LedTable
                dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000040            ; ON mask
                dc      $FFFFbf            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time
LED_TABLE_LENGTH equ *-LedTable                                      
                dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000080            ; ON mask
                dc      $FFFF7f            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time


LED_NUMBER      equ     2           ; number of LEDs on board


