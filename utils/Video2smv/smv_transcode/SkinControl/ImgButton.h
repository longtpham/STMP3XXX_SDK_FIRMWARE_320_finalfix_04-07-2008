#pragma once

#include "cuserimage.h"

#define IMG_BUTTON_HAS_TEXT     0

#if IMG_BUTTON_HAS_TEXT
#define IMG_BUTTON_TEXT_HEIGHT 12
#endif

#define TIMER_BUTTON_TOUCH     1

class CImgButton : public CButton
{
private:
    int m_diff_y;
    CUserImage m_imgNormal, m_imgDisable, m_imgFocus, m_imgPress;
    CUserImage m_imgBackGround;
    BOOL m_btnPress;
    CToolTipCtrl m_Tooltip;

#if IMG_BUTTON_HAS_TEXT
    COLORREF m_TextColor;
    BOOL m_UserColor;
#endif


protected:
    //{{AFX_MSG(CImgButton)
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    CImgButton();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
    void InitImage( const char *pcNormal, const char *pcDisable, const char *pcFocus, const char *pcPress );
    void InitBackground( const char *pcBack );
    void SetWindowTip(LPCTSTR lpszTip);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

#if IMG_BUTTON_HAS_TEXT
    void SetTextColor( COLORREF color );
    void SetWindowText( int diff_y, char *str );
    void SetWindowText( char *str );
#endif
};

