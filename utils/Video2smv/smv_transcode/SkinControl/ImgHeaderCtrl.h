#pragma once

#include "cuserimage.h"

class CImgHeaderCtrl : public CHeaderCtrl
{
private:
    CUserImage m_imgTitle, m_imgSeparator, m_imgTextColor;

protected:
    //{{AFX_MSG(CImgHeaderCtrl)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    CImgHeaderCtrl();
    ~CImgHeaderCtrl();

    void InitImage( const char *pcTitle, const char *pcSeparator, const char *pcTextColor );

protected:
    virtual void OnDrawItem( CDC *pDC, int iItem, CRect rect );
};