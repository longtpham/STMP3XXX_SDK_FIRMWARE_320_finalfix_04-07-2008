///////////////////////////////////////////////////////////////////////////////
//!
//! \file kernel_syswait.c
//! \brief
//!   This implements the kernel SysWait API.
//!
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "kernel_internal.h"

///////////////////////////////////////////////////////////////////////////////
// SysWait
//----------------------------------------------------------------------------
//! This implements the kernel SysWait API.
//!
//! \param
//! wTimeout 
//!     This holds the timeout that will expire before the task pointed to by
//!     g_pCurrentTask will be run again.    
//!
//! \retval
//! EVENT_TIMEOUT
//!     The timeout has expired.
//!
///////////////////////////////////////////////////////////////////////////////
_reentrant WORD SysWait(INT wTimeout)
{
    //
    // This should never be called in the kernel.
    //
    assert(KERNEL_OWNER != g_pCurrentTask);
    
    g_pCurrentTask->m_wState = EVENT_TIMEOUT;
    Kernel_SetTimeout(wTimeout);
    Kernel_ContextSwitch();        
    return EVENT_TIMEOUT;
}
