///////////////////////////////////////////////////////////////////////////////
//!
//! \file kernel_settimeout.c
//! \brief
//! This implements the Kernel_SetTimeout function used by the kernel.
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

#pragma optimize 3
///////////////////////////////////////////////////////////////////////////////
// Kernel_SetTimeout
//----------------------------------------------------------------------------
//! Set a timeout value for the current task.  If this is called by the kernel
//! it will do nothing as the kernel has no task storage area.
//!
//! \param wTimeOut
//!     The timeout for the task pointed to by g_pCurrentTask.
//!
//! \retval
//!     None.
//!
///////////////////////////////////////////////////////////////////////////////
_reentrant void Kernel_SetTimeout(INT wTimeout)
{
    long lValue;

    //
    // This should never be called inside the kernel.
    //
    if (g_pCurrentTask != KERNEL_OWNER)
    {
        if(wTimeout<0)
            lValue = 0x7fffffffffff;
        else
            lValue = GetTime()+wTimeout;
        g_pCurrentTask->m_lTimeout = lValue;
    }
}
