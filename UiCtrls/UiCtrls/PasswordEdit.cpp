
//*****************************************************
//********    PasswordEdit By YaoShuLong 2017.2
//*****************************************************

// PasswordEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "PasswordEdit.h"


// CPasswordEdit



IMPLEMENT_DYNAMIC(CPasswordEdit, CEdit)

CPasswordEdit::CPasswordEdit()
{
   // RegisterWindowClass();
    m_bUseDefBrush = TRUE;
    m_bHintMode = TRUE;
    m_nTextHeight = 0;
    m_nTextOffset = 8;
    m_nTextLimit = 0;
    m_bEnable = TRUE;

    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBkDef.CreateSolidBrush(m_clrTextBk);
    m_font.CreateFont(-14,0,0,0,
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

CPasswordEdit::~CPasswordEdit()
{

}


BEGIN_MESSAGE_MAP(CPasswordEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_ERASEBKGND()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CPasswordEdit 消息处理程序

BOOL CPasswordEdit::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, _T("CPasswordEdit"), &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("CPasswordEdit");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

HBRUSH CPasswordEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor( m_bHintMode ? m_clrHintText : (m_bEnable ? m_clrText : m_clrHintText));   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)(m_bUseDefBrush ? m_brushBkDef.GetSafeHandle() : m_brushBkSet.GetSafeHandle());  
}

void CPasswordEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
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

void CPasswordEdit::OnNcPaint()
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

void CPasswordEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    if (m_bHintMode)
    {
        m_strWindowText.Empty();
        SetWindowText(m_strWindowText);
        m_bHintMode = FALSE;

    }

}

void CPasswordEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    GetWindowText(m_strWindowText);
    if (m_strWindowText.IsEmpty())
    {
        CEdit::SetWindowText(m_strWindowHint);
        m_bHintMode = TRUE;
    }
    else
    {
        m_bHintMode = FALSE;
    }

}

void CPasswordEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CPasswordEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
    m_rect.SetRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
    m_rect.OffsetRect(-m_rect.left, -m_rect.top);
    CEdit::MoveWindow(lpRect, bRepaint);
}

void CPasswordEdit::GetWindowText(CString& string)
{
    if (m_bHintMode)
    {
        string.Empty();
    }
    else
    {
        string = m_strWindowText;
    }

}

void CPasswordEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
{
    m_bUseDefBrush = FALSE;
    m_clrTextBk = clrTextBk;
    m_clrText = clrText;
    m_clrHintText = clrHintText;
    if (m_brushBkSet.GetSafeHandle())
    {
        m_brushBkSet.DeleteObject();
    }
    m_brushBkSet.CreateSolidBrush(m_clrTextBk);
    Invalidate(FALSE);
}

void CPasswordEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
}

void CPasswordEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
    CRect rect;
    GetClientRect(&rect);
    SetWindowSize(rect);
    m_nTextLimit = nTextLimit;
    SetLimitText(nTextLimit);

    if (hFont != NULL)
    {
        SetFont(CFont::FromHandle(hFont));
    }
    else
    {
        SetFont(&m_font);
    }

    m_strWindowHint = strHint;
    CEdit::SetWindowText(m_strWindowHint);
    m_bHintMode = TRUE;

    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

}

void CPasswordEdit::SetWindowText(CString str)
{
    if (m_bHintMode)
    {
        m_bHintMode = FALSE;
    }
    m_strWindowText = str;
    CString strPSW = _T("******************************");
    int nLenth = str.GetLength();
    strPSW = strPSW.Mid(0, nLenth);
    CEdit::SetWindowText(strPSW);   
}

void CPasswordEdit::SetWindowSize(CRect rect)
{
    m_rect = rect;
}

void CPasswordEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar > 0x80 || nChar == 1 || nChar == 2 || nChar == 3 || nChar == 4 || nChar == 6 || nChar == 19 || nChar == 22 || nChar == 24 || nChar == 25 || nChar == 26)
    {
        return;
    }

    int nStartChar = 0;
    int nEndChar = 0;
    GetSel(nStartChar, nEndChar);

    if (8 == nChar)
    {
        if (nStartChar == nEndChar && nStartChar != 0)
        {
            m_strWindowText.Delete(nStartChar - 1, 1);
        } 
        else
        {
            m_strWindowText.Delete(nStartChar,nEndChar - nStartChar);
        }

    } 
    else
    {
        CString strAdd;
        strAdd = (wchar_t)nChar;
        if (nStartChar == nEndChar)
        {
            if (m_strWindowText.GetLength() == m_nTextLimit)
            {
                return;
            } 
            m_strWindowText.Insert(nStartChar, strAdd);
        } 
        else
        {
            m_strWindowText.Delete(nStartChar,nEndChar - nStartChar);
            m_strWindowText.Insert(nStartChar, strAdd);
        }

         nChar = '*';  

    }

    CEdit::DefWindowProc(WM_CHAR,nChar,MAKELONG(nRepCnt,nFlags));
    
}

BOOL CPasswordEdit::PreTranslateMessage(MSG* pMsg)
{
    if (!m_bEnable)
    {
        if (pMsg->message == WM_PAINT)
        {
            return CEdit::PreTranslateMessage(pMsg);
        }
        else if (pMsg->message == WM_KILLFOCUS)
        {
            return CEdit::PreTranslateMessage(pMsg);
        }
        return TRUE;
    }
    else
    {
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
        {
            NMHDR hdr;
            hdr.idFrom = GetDlgCtrlID();
            hdr.hwndFrom = GetSafeHwnd();
            hdr.code = NM_RETURN;
            GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        }
        return CEdit::PreTranslateMessage(pMsg);
    }

}

void CPasswordEdit::EnableWindow(BOOL bEnable/* = TRUE*/)
{
    if (IsWindowEnabled() == bEnable)
    {
        return;
    }
    m_bEnable = bEnable;
    Invalidate(FALSE);
}

BOOL CPasswordEdit::IsWindowEnabled()
{
    return m_bEnable;
}

BOOL CPasswordEdit::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CPasswordEdit::SetHintText(CString str)
{
    m_strWindowHint = str;
    if (m_bHintMode)
    {
        CEdit::SetWindowText(m_strWindowHint);
    }
    
}

void CPasswordEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
}


