;;/////////////////////////////////////////////////////////////////////////////
;;!
;;! \file database_resource.asm
;;! \brief This file contains the global references for the database resources.
;;!     These are needed so that the resource numbers can be defined outside of 
;;!     the library.
;;!
;;! Copyright (c) SigmaTel, Inc. Unpublished
;;!
;;! SigmaTel, Inc.
;;! Proprietary  Confidential
;;!
;;! This source code and the algorithms implemented therein constitute
;;! confidential information and may comprise trade secrets of SigmaTel, Inc.
;;! or its associates, and any use thereof is subject to the terms and
;;! conditions of the Confidential Disclosure Agreement pursuant to which this
;;! source code was originally received.
;;!
;;/////////////////////////////////////////////////////////////////////////////
    include "resource.inc"

    GLOBAL RSRC_DATABASE_CLOSE
    GLOBAL RSRC_DATABASE_ACCESS
    GLOBAL RSRC_DATABASE_DELETE
    GLOBAL RSRC_DATABASE_OPEN
