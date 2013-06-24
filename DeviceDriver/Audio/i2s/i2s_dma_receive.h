////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: i2s_dma_receive.h
// Description: Driver for the CDSync/I2S DMA module
////////////////////////////////////////////////////////////////////////////////

#ifndef _I2S_DMA_RECEIVE_H
#define _I2S_DMA_RECEIVE_H

#include "types.h"

// Start/Stop I2S DMA
void I2SDMAReceiveStart(void);
void I2SDMAReceiveStop(void);
void g_I2SDMAReceiveClearInt(void);

#endif // #ifndef _I2S_DMA_RECEIVE_H
