#pragma once
#include "cuserimage.h"

#define FC_DRAWNORMAL   0x00000001
#define FC_DRAWFOCUS    0x00000002
#define FC_DRAWPRESSED  0x00000004
#define FC_DRAWDISABLE  0x00000008

#define TIMER_COMBO_TOUCH     1

class CImgComboBox : public CComboBox
{
public:
    CImgComboBox();
private:
    CUserImage m_imgOutlineNormal, m_imgOutlineFocus, m_imgOutlineDisable;
    CUserImage m_imgBtnNormal, m_imgBtnPress, m_imgBtnFocus, m_imgBtnDisable;
    CUserImage m_imgBkSelect, m_imgBkNormal, m_imgBkDisable, m_imgFrSelect, m_imgFrNormal, m_imgFrDisable;
    BOOL m_btnPress;

protected:
    //{{AFX_MSG(CImgComboBox)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

    void DrawOutline( DWORD dwStyle );
    void SetWindowSize( int x, int y, int nWidth, int nHeight );
    void InitImageOutline( const char *pcOutlineNormal, const char *pcOutlineDisable, const char *pcOutlineFocus );
    void InitImageItem( const char *pcBkNormal, const char *pcBkSelect, const char *pcBkDisable, const char *pcFrNormal, const char *pcFrSelect, const char *pcFrDisable );
    void InitImageBtn( const char *pcBtnNormal, const char *pcBtnPress, const char *pcBtnFocus, const char *pcBtnDisable );
};

