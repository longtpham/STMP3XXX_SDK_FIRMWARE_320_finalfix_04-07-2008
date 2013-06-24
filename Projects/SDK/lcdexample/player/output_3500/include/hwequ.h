/////////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2001
//  File        : hwequ.inc
//  Description : STMP Hardware Constants
/////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////
// hwequ is being eliminated for SDK3XXX.  For SDK2.5XX it will now reside in the   
// local folder instead of ..\inc.  All register includes are converted to .h by 
// registers.mk placed in the ..\lcdexample\player\output_XXXX folder.
// ////////////////////////////////////////////////////////////////////////////////

#if (!defined(HWEQU_INC))
#define HWEQU_INC 1

#include "types.h"
#include "regsclkctrl.h"
#include "regscore.h"
#include "regscodec.h"
#include "regsdcdc.h"
#include "regsemc.h"
#include "regsgpio.h"
#include "regsi2c.h"
#include "regsi2s.h"
#include "regsicoll.h"
#include "regslradc.h"
#include "regspwm.h"
#include "regsrevision.h"
#include "regsrtc.h"
#include "regsspare.h"
#include "regsspi.h"
#include "regsswizzle.h"
#include "regssdram.h"
#include "regstb.h"
#include "regstimer.h"
#include "regsusb20.h"
#include "regsusb20phy.h"


#endif // if (!@def(hwequ))

