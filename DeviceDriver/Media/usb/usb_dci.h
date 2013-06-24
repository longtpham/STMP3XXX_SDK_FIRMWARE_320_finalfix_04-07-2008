#ifndef _USB_DCI_H
#define _USB_DCI_H

#include "types.h"
#include "usb_internal.h"
#include "usb_api.h"


BYTE _reentrant usb_dci_init                (Dev_State_Struct _USB_MEM * pStDeviceState);
#define usb_dci_chip_init(pStDeviceState) /* Do Nothing */
void _reentrant usb_dci_process_reset       (Dev_State_Struct _USB_MEM * pStDeviceState);
void _reentrant usb_dci_process_tr_complete (Dev_State_Struct _USB_MEM * pStDeviceState);
void _reentrant usb_dci_process_suspend     (Dev_State_Struct _USB_MEM * pStDeviceState);
void _reentrant usb_dci_process_port_change (Dev_State_Struct _USB_MEM * pStDeviceState);
BYTE _reentrant usb_dci_init_endpoint       (xd_struct _USB_MEM * pStXD);
USB_SetupData *_reentrant usb_dci_get_setup_data(BYTE btEndpoint);
BYTE _reentrant usb_dci_get_endpoint_status(BYTE btEndpoint);
void _reentrant usb_dci_set_endpoint_status(BYTE btEndpoint, USHORT usStatus);
void _reentrant usb_dci_set_address(BYTE btAddress);
BYTE _reentrant usb_dci_get_transfer_status(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_dci_stall_endpoint(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_dci_host_stall_endpoint(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_dci_unstall_endpoint(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_dci_init_data_toggle_endpoint(BYTE btEndpoint);
BYTE _reentrant usb_dci_transfer_data(xd_struct _USB_MEM * pStXd);
void _reentrant usb_dci_finish_transaction(BYTE btEndpoint);
void _reentrant usb_dci_update_connection_status(void);
void _reentrant usb_dci_shutdown(void);
void _reentrant usb_dci_set_test_mode(USHORT usTest); 

void _reentrant usb_dci_chip_enable (void);
#endif