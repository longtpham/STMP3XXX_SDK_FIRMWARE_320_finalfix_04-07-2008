#pragma once

#include "cuserimage.h"

#define BUTTON_FRAME_SIGN   0

#if BUTTON_FRAME_SIGN
#define IMG_CHECK_HAS_TEXT  1
#else
#define IMG_CHECK_HAS_TEXT  0
#endif

#if BUTTON_FRAME_SIGN
#define CHECK_IMAGE_SIZE     15
#endif

#if IMG_CHECK_HAS_TEXT
#define CHECK_FONT_HEIGHT    13
#define CHECK_STRING_DIFF_X  ( CHECK_IMAGE_SIZE + 3 )
#define CHECK_STRING_DIFF_Y  ( CHECK_IMAGE_SIZE + 1 - CHECK_FONT_HEIGHT )/2
#endif

#define TIMER_CHECK_TOUCH  1

class CImageCheckBox : public CButton
{
private:
    CToolTipCtrl m_Tooltip;
    CUserImage m_imgNormal, m_imgCheck, m_imgNormalFocus, m_imgCheckFocus;
    CUserImage m_imgDisable, m_imgCheckDisable;

    BOOL blCheck;

#if BUTTON_FRAME_SIGN
    COLORREF m_TextColor;
    BOOL m_UserColor;
#endif

public:
    void InitImage( const char *pcNormal, const char *pcNormalFocus, const char *pcCheck, const char *pcCheckFocus,
        const char *pcDisable, const char *pcCheckDisable );
    void DrawItemImg(LPDRAWITEMSTRUCT lpDIS);
    BOOL GetCheck();
    void SetCheck( BOOL state );

#if IMG_CHECK_HAS_TEXT
    CImageCheckBox();
    void GetClientRect( LPRECT lpRect );
    void SetTextColor( COLORREF color );
#endif

protected:
    //{{AFX_MSG(CButtonBox)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    void SetWindowTip(LPCTSTR lpszTip);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
};

