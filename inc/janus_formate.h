
#include "types.h"
/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: janusdefi.c
/////////////////////////////////////////////////////////////////////////////////

//#define ASSUME_JANUS_DRIVE_PRESENT
//#define NEGLECT_JANUS_DRIVE


#define   JANUSSIGNATURE_1      0x80071119
#define   JANUSSIGNATURE_2      0x19082879


#define   JANUS_FORMATE    		        100
#define   JANUS_FORMATE_OFFSET          24

#define   JANUSSIGNATURE_1_OFFSET  	    16
#define   JANUSSIGNATURE_2_OFFSET  	    20

#define   JANUSTOTALSECTORS_OFFSET      28
#define   JANUSBOOTSECTOR_OFFSET  	    26

#define   JANUSTOTALSECTORS          0x800
#define   JANUSBOOTSECTOR  	            20  
