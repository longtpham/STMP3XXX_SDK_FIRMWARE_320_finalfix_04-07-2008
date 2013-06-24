///////////////////////////////////////////////////////////////////////////////
//!
//! \file kernel_mem.c
//! \brief
//! Some of the internal memory allocated for the kernel.
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

#define KERNEL_STACK_SIZE   300

WORD g_KernelStack[KERNEL_STACK_SIZE];

//!
//! Initialize the current task to a known value.  When this variable is set to
//! KERNEL_OWNER, the system is running in the kernel's context. 
//!  
TaskEntry *g_pCurrentTask = KERNEL_OWNER;

WORD *Kernel_pTempStack;
