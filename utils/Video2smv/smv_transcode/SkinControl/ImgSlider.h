#pragma once
#include "cuserimage.h"

#define SLIDER_HEADER_RANGE       8
#define TIMER_SLIDER_SECQUENCE    1

class CImgSlider : public CSliderCtrl
{
public:
    void InitImage( const char *pcAxis, const char *pcSelAxis, const char *pcSlider );
    void SetRange( int min, int max );
    void SetSelection( int min, int max );

private:
    CUserImage m_imgAxis, m_imgSelAxis, m_imgSlider;
    CDC *m_pDC;
    LPNMCUSTOMDRAW lpCustDraw;

protected:
    //{{AFX_MSG(CImgComboBox)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown( UINT nFlags,CPoint point );
    afx_msg void OnLButtonUp( UINT nFlags,CPoint point );
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void DrawAxis( void );
    void DrawSlider( void );
};

