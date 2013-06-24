#pragma once
#include "afxwin.h"
#include "cuserimage.h"

class CMemDC : public CDC
{
private:
    CBitmap*    m_bitmap;
    CBitmap*    m_oldBitmap;
    CDC*        m_pDC;
    CRect       m_rcBounds;

public:
    CMemDC(CDC* pDC, const RECT& rcBounds) : CDC()
    {
        CreateCompatibleDC(pDC);
        m_bitmap = new CBitmap;
        m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.right, rcBounds.bottom);
        m_oldBitmap = SelectObject(m_bitmap);
        m_pDC = pDC;
        m_rcBounds = rcBounds;
    }

    ~CMemDC()
    {
        m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
                    this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
        SelectObject(m_oldBitmap);
        if (m_bitmap != NULL)
        {
            delete m_bitmap;
        }
    }

    CMemDC* operator->()
    {
        return this;
    }
};

