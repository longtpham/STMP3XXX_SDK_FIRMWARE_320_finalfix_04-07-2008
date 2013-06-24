#pragma once
#include "cuserimage.h"

#define SLIDER_HEADER_RANGE       8
#define TIMER_SLIDER_SECQUENCE    1

class CImgStatic : public CStatic
{
public:
    void InitImage( const char *InitImage );

private:
    CUserImage m_imgNormal;

protected:
    //{{AFX_MSG(CImgComboBox)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

