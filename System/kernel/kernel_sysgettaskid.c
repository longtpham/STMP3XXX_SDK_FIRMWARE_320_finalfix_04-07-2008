///////////////////////////////////////////////////////////////////////////////
//!
//! \file kernel_sysgettaskid.c
//! \brief
//! This file holds the implementation of the SysGetTaskID kernel API.
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
// SysGetTaskID
//----------------------------------------------------------------------------
//! This function simply returns the task id for the currently running task.
//!
//! \param
//!     None.
//!
//! \return
//!     The task ID for the current task.
//!
///////////////////////////////////////////////////////////////////////////////
_reentrant WORD SysGetTaskID(void)
{
    assert(g_pCurrentTask != KERNEL_OWNER);
    return g_pCurrentTask->m_wTaskID;
}

