#pragma once
#include "cuserimage.h"

#define SLIDER_HEADER_RANGE       8
#define TIMER_SLIDER_SECQUENCE    1

class CImgProgressCtrl : public CStatic
{
public:
    CImgProgressCtrl();
    void InitImage( const char *pcBack, const char *pcSelect );
    void SetRange( UINT min, UINT max );
    void SetPos( UINT pos );

private:
    CUserImage m_imgBack, m_imgSelect;
    UINT m_min, m_max, m_current;

protected:
    //{{AFX_MSG(CImgComboBox)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

