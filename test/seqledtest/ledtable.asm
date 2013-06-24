
























;///////////////////////////////////////////////////////////////////////////////
    nolist
        include "hwequ.inc"
    list


    org   y,"LEDModule.y":
    GLOBAL LedTable
    GLOBAL LED_NUMBER
    GLOBAL LED_TABLE_LENGTH

LedTable
Led0            dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000080            ; ON mask  to be used with OR op
                dc      $FFFF7F            ; OFF mask to be used with AND op
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time

LED_TABLE_LENGTH equ *-LedTable            ; Determine the length of a table entry!                          

Led1            dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000040            ; ON mask
                dc      $FFFFBF            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time

Led2            dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000020            ; ON mask
                dc      $FFFFDf            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time


;added on February 11th 2002...for the ENG_BOARD Rev B
Led3            dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000010            ; ON mask
                dc      $FFFFef            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time

Led4            dc      HW_GP0ENR          ; GPIO Bank 0 Enable Register
                dc      HW_GP0DOER         ; GPIO Bank 0 Direction Register
                dc      HW_GP0DOR          ; GPIO Bank 0 Data Register
                dc      $000008            ; ON mask
                dc      $FFFFf7            ; OFF mask
                dc      0                  ; On Time
                dc      0                  ; Off Time
                dc      0                  ; current on time
                dc      0                  ; current off time

LED_NUMBER      equ     5           	   ; total number of LEDs on board
