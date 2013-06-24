// smv_transcode.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "global.h"
#include "smv.h"
#include "cuserimage.h"
#include "imgbutton.h"
#include "imgcheck.h"
#include "imgcombo.h"
#include "imglistctrl.h"
#include "imgslider.h"
#include "imgstatic.h"
#include "imgprogress.h"
#include "pubvars.h"
#include "BrowseDialog.h"

// Csmv_transcodeApp:
// See smv_transcode.cpp for the implementation of this class
//

class Csmv_transcodeApp : public CWinApp
{
public:
    Csmv_transcodeApp();

// Overrides
    public:
    virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern Csmv_transcodeApp theApp;