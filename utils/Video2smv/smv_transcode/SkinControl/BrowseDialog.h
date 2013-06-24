#pragma once
#include "afxwin.h"

class CBrowseDialog
{
    public:
        CBrowseDialog();
        virtual ~CBrowseDialog();
        BOOL DoModal();
        CString m_Path;
        CString m_InitDir;
        CString m_SelDir;
        CString m_Title;
        HWND hwndOwner;
};

