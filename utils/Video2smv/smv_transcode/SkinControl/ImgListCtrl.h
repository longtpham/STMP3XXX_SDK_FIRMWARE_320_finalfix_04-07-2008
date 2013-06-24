#pragma once

#include "imgheaderctrl.h"

class CImgListCtrl : public CListCtrl
{
private:
    CImgHeaderCtrl m_lstHeader;

protected:
    //{{AFX_MSG(CImgListCtrl)
    afx_msg void OnRButtonDown( UINT nFlags,CPoint point );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    virtual void PreSubclassWindow();

public:
    CImgListCtrl();
    ~CImgListCtrl();

    void InitHeaderImage( const char *pcTitle, const char *pcSeparator, const char *pcTextColor );
    void InitItemImage( const char *pcBack, const char *pcText );
    int GetFocusItem( void );
};