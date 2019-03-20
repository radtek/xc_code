
//*****************************************************
//********    CompareInplaceEdit By YaoShuLong 2017.4
//*****************************************************

#include "stdafx.h"
#include "CompareInplaceEdit.h"
#include "CompareEdit.h"

// CCompareInplaceEdit



IMPLEMENT_DYNAMIC(CCompareInplaceEdit, CEdit)

CCompareInplaceEdit::CCompareInplaceEdit()
{
    m_nTextHeight = 0;
    m_nTextOffset = 8;

    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_nItemCountMax = 10;
    m_font.CreateFont(-12,0,0,0,
        FW_BOLD,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("微软雅黑"));
}

CCompareInplaceEdit::~CCompareInplaceEdit()
{
}


BEGIN_MESSAGE_MAP(CCompareInplaceEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_CONTROL_REFLECT(EN_CHANGE, &CCompareInplaceEdit::OnEnChange)
    ON_WM_KEYDOWN()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CCompareInplaceEdit 消息处理程序

HBRUSH CCompareInplaceEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)m_brushBk.GetSafeHandle();  
}

void CCompareInplaceEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    CRect rectWnd, rectClient;

    CFont *pFont = GetFont();
    CRect rectText;
    rectText.SetRectEmpty();

    CDC *pDC = GetDC();
    CFont *pOld = pDC->SelectObject(pFont);
    pDC->DrawText(_T("Ky"), rectText, DT_CALCRECT | DT_LEFT);
    m_nTextHeight = rectText.Height();
    pDC->SelectObject(pOld);
    ReleaseDC(pDC);

    UINT uiCenterOffset = (m_rect.Height() - m_nTextHeight) / 2;

    lpncsp->rgrc[0].top +=uiCenterOffset;
    lpncsp->rgrc[0].bottom -= uiCenterOffset;

    lpncsp->rgrc[0].left += m_nTextOffset;
    lpncsp->rgrc[0].right -= m_nTextOffset;


}

void CCompareInplaceEdit::OnNcPaint()
{
    Default();

    CWindowDC dc(this);
    CBrush Brush(m_clrTextBk);

    CRect rectLeft;
    CRect rectRight;
    rectLeft.SetRect(0, 0, m_nTextOffset, m_rect.Height());
    rectRight.SetRect(m_rect.Width() - m_nTextOffset, 0, m_rect.Width(), m_rect.Height());

    UINT uiCenterOffset = (m_rect.Height() - m_nTextHeight) / 2;
    CRect rectTop = m_rect;
    rectTop.DeflateRect(0, 0, 0, uiCenterOffset + m_nTextHeight );
    CRect rectBottom = m_rect;
    rectBottom.DeflateRect(0, uiCenterOffset + m_nTextHeight, 0, 0);

    dc.FillRect(rectBottom, &Brush);
    dc.FillRect(rectTop, &Brush);

    dc.FillRect(rectLeft, &Brush);
    dc.FillRect(rectRight, &Brush);
    
}



void CCompareInplaceEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}



void CCompareInplaceEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
{
    m_clrTextBk = clrTextBk;
    m_clrText = clrText;
    m_clrHintText = clrHintText;
    if (m_brushBk.GetSafeHandle())
    {
        m_brushBk.DeleteObject();
    }

    m_brushBk.CreateSolidBrush(m_clrTextBk);
}

void CCompareInplaceEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
}

void CCompareInplaceEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
    CRect rect;
    GetClientRect(&rect);
    SetWindowSizeEx(rect);

    SetLimitText(nTextLimit);

    if (hFont != NULL)
    {
        SetFont(CFont::FromHandle(hFont));
    }
    else
    {
        SetFont(&m_font);
    }
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

}


void CCompareInplaceEdit::SetWindowSizeEx(CRect rect)
{
    m_rect = rect;
}


void CCompareInplaceEdit::OnEnChange()
{
    GetParent()->PostMessage(UM_COMPAREDLG, UM_COMPAREDLG_ONENCHANGE, NULL);   
}

void CCompareInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (VK_UP == nChar || VK_DOWN == nChar)
    {
        GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_VK_UPANDDOWN, (LPARAM)nChar);   
        return;
    }
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCompareInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_LISTBOX_OK, NULL);   
    } 

    return CEdit::PreTranslateMessage(pMsg);
}

BOOL CCompareInplaceEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    
    if (zDelta > 0)
    {
        GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_MOUSEWHEELUP, (LPARAM)&pt);   
    } 
    else
    {
        GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_MOUSEWHEELDOWN, (LPARAM)&pt);   
    }

    return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CCompareInplaceEdit::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


