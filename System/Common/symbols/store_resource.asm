;;/////////////////////////////////////////////////////////////////////////////
;;!
;;! \file store_resource.asm
;;! \brief This file contains the global references for the store resources.
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
    
    GLOBAL RSRC_FUNCLET_STOR_GETNUMOBJECTS
    GLOBAL RSRC_FUNCLET_STOR_ISOBJECTVALID
    GLOBAL RSRC_FUNCLET_STOR_OPENFRIT
    GLOBAL RSRC_FUNCLET_STOR_CLOSEFRIT
    GLOBAL RSRC_FUNCLET_STOR_OPENOHIT
    GLOBAL RSRC_FUNCLET_STOR_CLOSEOHIT
    GLOBAL RSRC_FUNCLET_STOR_ADDOBJECTTOINDEXTABLE
    GLOBAL RSRC_FUNCLET_STOR_DELETEOBJECTFROMINDEXTABLE
    GLOBAL RSRC_FUNCLET_STOR_CHDIRTOPARENTOF
    GLOBAL RSRC_FUNCLET_STOR_FLUSH
