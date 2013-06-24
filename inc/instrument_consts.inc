INSTRUMENT_CONTEXT_DEVICE_DRIVER_READ           equ             $01
INSTRUMENT_CONTEXT_DEVICE_DRIVER_READ_READ_SECTOR equ       $01



INSTRUMENT_CONTEXT_SYSLOADRESOURCE              equ             $02
INSTRUMENT_CONTEXT_SYSLOADRESOURCE_OVERHEAD     equ         $01
INSTRUMENT_CONTEXT_SYSLOADRESOURCE_SECTOR_READ  equ         $02
INSTRUMENT_CONTEXT_SYSLOADRESOURCE_COPY         equ         $03

INSTRUMENT_CONTEXT_SYSTEMDRIVE_READ             equ                 $03
INSTRUMENT_CONTEXT_SYSTEMDRIVE_READ_OVERHEAD    equ         $01
INSTRUMENT_CONTEXT_SYSTEMDRIVE_READ_HAL         equ         $02


INSTRUMENT_CONTEXT_HAL                          equ                 $04
INSTRUMENT_CONTEXT_HAL_OVERHEAD                 equ         $01
INSTRUMENT_CONTEXT_HAL_READ_FROM_PAGE           equ         $02

INSTRUMENT_CONTEXT_HIL                          equ                 $05

CONTEXT_USB_ISR                                 equ                 $06
CONTEXT_USB_ISR_ENTRY                           equ         $01


MULTI_WRITE_ACTIVE                              equ         $00
