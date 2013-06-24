////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: icoll.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _ICOLL_H
#define _ICOLL_H

///////////////////////////////////////////////////////////////////////////////
// Defs
///////////////////////////////////////////////////////////////////////////////
                                                                                          
#define ICOLL_SOURCE_DAC_REFILL                                 (DWORD)0             
#define ICOLL_SOURCE_DAC_UNDERFLOW                              (DWORD)1             
#define ICOLL_SOURCE_ADC_REFILL                                 (DWORD)2             
#define ICOLL_SOURCE_ADC_OVERFLOW                               (DWORD)3             
#define ICOLL_SOURCE_FLASH_DONE                                 (DWORD)4              
#define ICOLL_SOURCE_COMPACTFLASH_CARD_IRQ                      (DWORD)5              
#define ICOLL_SOURCE_SMARTMEDIA_TIMEOUT                         (DWORD)6              
#define ICOLL_SOURCE_SMARTMEDIA_INTERFACE_INVALID_PROGRAMMING   (DWORD)7              
#define ICOLL_SOURCE_COMPACTFLASH_NO_CARD                       (DWORD)8               
#define ICOLL_SOURCE_COMPACTFLASH_STATUS_CHANGE                 (DWORD)9               
#define ICOLL_SOURCE_GPIO_0                                     (DWORD)10               
#define ICOLL_SOURCE_GPIO_1                                     (DWORD)11               
#define ICOLL_SOURCE_GPIO_2                                     (DWORD)12                
#define ICOLL_SOURCE_TIMER_0                                    (DWORD)13                
#define ICOLL_SOURCE_TIMER_1                                    (DWORD)14                
#define ICOLL_SOURCE_TIMER_2                                    (DWORD)15                
#define ICOLL_SOURCE_TIMER_3                                    (DWORD)16                 
#define ICOLL_SOURCE_GPIO_3                                     (DWORD)17                 
#define ICOLL_SOURCE_SDRAM                                      (DWORD)18                 
#define ICOLL_SOURCE_CDI                                        (DWORD)19                 

#if (defined(DEVICE_3410) || defined(DEVICE_3310))

#define ICOLL_SOURCE_USB_SOF                                    (DWORD)20                  
#define ICOLL_SOURCE_USB_SET_INTERFACE_REQUEST                  (DWORD)21                  
#define ICOLL_SOURCE_USB_HOST_RESET                             (DWORD)22                  
#define ICOLL_SOURCE_USB_ENDPOINT_START                         (DWORD)23
#define ICOLL_SOURCE_USB_ENDPOINT_0                             (DWORD)23                  
#define ICOLL_SOURCE_USB_ENDPOINT_1                             (DWORD)24                  
#define ICOLL_SOURCE_USB_ENDPOINT_2                             (DWORD)25                  
#define ICOLL_SOURCE_USB_ENDPOINT_3                             (DWORD)26                  
#define ICOLL_SOURCE_USB_ENDPOINT_4                             (DWORD)27                  
#define ICOLL_SOURCE_USB_ENDPOINT_5                             (DWORD)28                    
#define ICOLL_SOURCE_USB_ENDPOINT_6                             (DWORD)29                    
#define ICOLL_SOURCE_USB_ENDPOINT_7                             (DWORD)30                    

#else

#define ICOLL_SOURCE_VDD_5V_CONNECTED                           (DWORD)20                  
#define ICOLL_SOURCE_USB_CONTROLLER                             (DWORD)21                  
#define ICOLL_SOURCE_USB_WAKEUP                                 (DWORD)22                  
#define ICOLL_SOURCE_VDD_5V_DISCONNECTED                        (DWORD)23
#define ICOLL_SOURCE_ESPI                                       (DWORD)24                  
#define ICOLL_SOURCE_FILCO                                      (DWORD)25                  
#define ICOLL_SOURCE_LRADC1                                     (DWORD)26                  
#define ICOLL_SOURCE_RTC_ALARM                                  (DWORD)27                  
#define ICOLL_SOURCE_LRADC2                                     (DWORD)28                  
#define ICOLL_SOURCE_FLASH_ECC                                  (DWORD)29                    

#endif

#define ICOLL_SOURCE_CDSYNC                                     (DWORD)31                    
#define ICOLL_SOURCE_CDSYNC_EXCEPTION                           (DWORD)32                     
#define ICOLL_SOURCE_REED_SOLOMON                               (DWORD)33
#define ICOLL_END_OF_TRUE_SOURCES                                ICOLL_SOURCE_REED_SOLOMON
// The rest of these "sources" are not really ICOLL sources.  They are not
//  routed through the ICOLL but are included here to make interrupt abstraction
//  as simple as possible.  The ICOLL functions special-case these.
#define ICOLL_SOURCE_I2C_RX_READY                               (DWORD)34
#define ICOLL_SOURCE_I2C_RX_OVERFLOW                            (DWORD)35
#define ICOLL_SOURCE_I2C_TX_EMPTY                               (DWORD)36
#define ICOLL_SOURCE_I2C_TX_UNDERFLOW                           (DWORD)37
#define ICOLL_SOURCE_SPI_COMPLETE                               (DWORD)38
#define ICOLL_SOURCE_I2S_RX_OVERFLOW                            (DWORD)39
#define ICOLL_SOURCE_I2S_TX_UNDERFLOW                           (DWORD)40
#define ICOLL_SOURCE_I2S_RX_READY                               (DWORD)41
#define ICOLL_SOURCE_I2S_TX_EMPTY                               (DWORD)42
#define ICOLL_SOURCE_HARDWARE_RESET                             (DWORD)43
#define ICOLL_SOURCE_STACK_ERROR                                (DWORD)44
#define ICOLL_SOURCE_TRACE                                      (DWORD)45
#define ICOLL_SOURCE_SWI                                        (DWORD)46
#define ICOLL_SOURCE_IRQA                                       (DWORD)47
#define ICOLL_SOURCE_BROWNOUT                                   (DWORD)48
#define ICOLL_SOURCE_ILLEGAL_INSTRUCTION                        (DWORD)49
#define ICOLL_SOURCE_END                                        ICOLL_SOURCE_ILLEGAL_INSTRUCTION
#define ICOLL_NUM_SOURCES                                       ICOLL_SOURCE_END+1

#define ICOLL_ICLENABLE_REG_0_LAST_SRC                          (DWORD)23

#define ICOLL_PRIORITY_000              0x000000
#define ICOLL_PRIORITY_001              0x000001
#define ICOLL_PRIORITY_010              0x000002
#define ICOLL_PRIORITY_011              0x000003
#define ICOLL_PRIORITY_100              0x000004
#define ICOLL_PRIORITY_101              0x000005
#define ICOLL_PRIORITY_110              0x000006
#define ICOLL_PRIORITY_111              0x000007

#define ICOLL_STEERING_IVL1             0x000000
#define ICOLL_STEERING_IVL2             0x000001
#define ICOLL_STEERING_IVL3             0x000002
#define ICOLL_STEERING_IVL6             0x000003

///////////////////////////////////////////////////////////////////////////////
// ERRORS
///////////////////////////////////////////////////////////////////////////////
                                                                                          
#define ICOLL_ERROR_INVALID_SOURCE              (RETCODE)0x000001

///////////////////////////////////////////////////////////////////////////////
// Structure definitions
///////////////////////////////////////////////////////////////////////////////

typedef void (*ISR_VECTOR)(void);

typedef struct
{
    WORD wSource;
    WORD wPriority;
    WORD wSteering;
} ICOLL_ELEMENT;

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

void _reentrant IcollShutDown(void);
RETCODE _reentrant IcollEnable(DWORD dwSource);
RETCODE _reentrant IcollDisable(DWORD dwSource);
BOOL _reentrant IcollGetStatus(DWORD dwSource);
RETCODE _reentrant IcollSetPriority(DWORD dwSource, WORD wPriority);
RETCODE _reentrant IcollSetSteering(DWORD dwSource, WORD wSteering);
RETCODE _reentrant IcollInstallIsrVector(ISR_VECTOR pISR, WORD wSource);
void _asmfunc IcollResetVectorTable(void);
#endif // #ifndef _ICOLL_H
