
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

// SimpleEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SimpleEdit.h"

HFONT CSimpleEdit::m_hFont = NULL;
// CSimpleEdit

IMPLEMENT_DYNAMIC(CSimpleEdit, CEdit)

CSimpleEdit::CSimpleEdit()
{
    m_bHintMode = TRUE;
    m_nTextOffset = 8;

    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_bEnable = TRUE;
    m_bSetSel = FALSE;
    m_bFloat = FALSE;
    m_bUintOnly = FALSE;
    m_NumberControl = TRUE;
	m_bEnbaleButtonEdit = FALSE;

    //m_font.CreateFont(-12,0,0,0,
    //    FW_BOLD,
    //    FALSE,FALSE,
    //    FALSE, 
    //    DEFAULT_CHARSET,
    //    OUT_DEFAULT_PRECIS,
    //    CLIP_DEFAULT_PRECIS,
    //    PROOF_QUALITY,
    //    DEFAULT_PITCH, 
    //    _T("微软雅黑"));
}

CSimpleEdit::~CSimpleEdit()
{
}

BEGIN_MESSAGE_MAP(CSimpleEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
	ON_WM_MOUSEACTIVATE()
    ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

HBRUSH CSimpleEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor( m_bHintMode ? m_clrHintText : (m_bEnable ? m_clrText : m_clrHintText));   
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(m_clrTextBk);
	return HBRUSH(m_brushBk.GetSafeHandle());
}

void CSimpleEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (m_rect.Height() == 0)
	{
		return;
	}

	UINT uiCenterOffset = (m_rect.Height() - GetTextHeight()) / 2;

    lpncsp->rgrc[0].top +=uiCenterOffset;
    lpncsp->rgrc[0].bottom -= uiCenterOffset;

    lpncsp->rgrc[0].left += m_nTextOffset;
    lpncsp->rgrc[0].right -= m_nTextOffset;
}

void CSimpleEdit::OnNcPaint()
{
    Default();

	if (m_rect.Height() == 0)
	{
		return;
	}

    CWindowDC dc(this);
    CBrush Brush(m_clrTextBk);

    CRect rectLeft;
    CRect rectRight;
    rectLeft.SetRect(0, 0, m_nTextOffset, m_rect.Height());
    rectRight.SetRect(m_rect.Width() - m_nTextOffset, 0, m_rect.Width(), m_rect.Height());

	int nTextHeight = GetTextHeight();
	UINT uiCenterOffset = (m_rect.Height() - nTextHeight) / 2;
    CRect rectTop = m_rect;
	rectTop.DeflateRect(0, 0, 0, uiCenterOffset + nTextHeight);
    CRect rectBottom = m_rect;
	rectBottom.DeflateRect(0, uiCenterOffset + nTextHeight, 0, 0);

    dc.FillRect(rectBottom, &Brush);
    dc.FillRect(rectTop, &Brush);

    dc.FillRect(rectLeft, &Brush);
    dc.FillRect(rectRight, &Brush);   
}

int CSimpleEdit::GetTextHeight()
{
	CDC *pDC = GetDC();
	CFont *pOld = pDC->SelectObject(GetFont());
	CRect rectText;
	pDC->DrawText(_T("Ky"), rectText, DT_CALCRECT | DT_LEFT);
	pDC->SelectObject(pOld);
	ReleaseDC(pDC);
	return rectText.Height();
}

void CSimpleEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);
    if (m_bHintMode)
    {
        m_strWindowText.Empty();
        SetWindowText("");
        m_bHintMode = FALSE;
    }
    else
    {
        if (m_bSetSel)
        {
            GetParent()->PostMessage(UM_SETCUL, GetDlgCtrlID(), NULL);
        }    
    }
}

void CSimpleEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    CEdit::GetWindowText(m_strWindowText);
    m_strWindowText.Trim();
    if (m_strWindowText.IsEmpty())
    {
        SetWindowText(m_strWindowHint);
        m_bHintMode = TRUE;
    }
    else
    {
        m_bHintMode = FALSE;
    }
	if (m_bEnbaleButtonEdit)
	{
		GetParent()->SendMessage(WM_KILLFOCUS, 0, 0);
	}
	
}

BOOL CSimpleEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_rect = rect;
	m_rect.OffsetRect(-m_rect.left, -m_rect.top);
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

void CSimpleEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CSimpleEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
    m_rect.SetRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
    m_rect.OffsetRect(-m_rect.left, -m_rect.top);
    CEdit::MoveWindow(lpRect, bRepaint);
}

void CSimpleEdit::GetWindowText(CString& str)
{
    if (m_bHintMode)
    {
		str.Empty();
    }
    else
    {
		CEdit::GetWindowText(str);
    }
}

void CSimpleEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
{
    m_clrTextBk = clrTextBk;
    m_clrText = clrText;
    m_clrHintText = clrHintText;
    if (m_brushBk.GetSafeHandle())
    {
		m_brushBk.DeleteObject();
    }
	m_brushBk.CreateSolidBrush(m_clrTextBk);
    Invalidate(FALSE);
}

void CSimpleEdit::SetBkColor(COLORREF clr)
{
    m_clrTextBk = clr;
    if (m_brushBk.GetSafeHandle())
    {
        m_brushBk.DeleteObject();
    }
	m_brushBk.CreateSolidBrush(m_clrTextBk);
    Invalidate(FALSE);
}

void CSimpleEdit::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

void CSimpleEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
    Invalidate(FALSE);    
}

void CSimpleEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
    SetLimitText(nTextLimit);

    //if (hFont != NULL)
    //{
    //    SetFont(CFont::FromHandle(hFont));
    //}
    //else
    //{
    //    SetFont(&m_font);
    //}
	CEdit::SetFont(CFont::FromHandle(m_hFont));

    m_strWindowHint = strHint;
    CEdit::SetWindowText(m_strWindowHint);
    m_bHintMode = TRUE;

    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

void CSimpleEdit::SetWindowText(CString str)
{
    if (m_bHintMode)
    {
        m_bHintMode = FALSE;
    }
    CEdit::SetWindowText(str);   
}

BOOL CSimpleEdit::PreTranslateMessage(MSG* pMsg)
{
    if (!m_bEnable)
    {
        if (pMsg->message == WM_PAINT )
        {
            return CEdit::PreTranslateMessage(pMsg);
        }
        return TRUE;
    }
    else
    {
        if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_RETURN)
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

void CSimpleEdit::EnableWindow(BOOL bEnable/* = TRUE*/)
{   
	if (m_bEnable != bEnable)
	{
		if (!bEnable)
		{
			GetParent()->SetFocus();
		}
		m_bEnable = bEnable;
		Invalidate(FALSE);
		CEdit::EnableWindow(bEnable);
	}	
}

BOOL CSimpleEdit::IsWindowEnabled()
{
    return m_bEnable;
}

void CSimpleEdit::EnableSetSelOnFocus(BOOL bSetSel)
{
    m_bSetSel = bSetSel;
}

void CSimpleEdit::SetHintText(CString str)
{
    m_strWindowHint = str;
    if (m_bHintMode)
    {
        CEdit::SetWindowText(m_strWindowHint);
    }
}

int CSimpleEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (GetFocus() == this)  // 防止 DockTabPane 中 SetFocus
		return MA_ACTIVATE;

	return CEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);	
}

void CSimpleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (m_bFloat)
    {      
        if ((nChar < 48 || nChar > 57)  //0~9数字
            && nChar != 46 //小数点.
            && nChar != 8 //Backspace
            && nChar != 26 //Ctrl + Z
            && nChar != 24 //Ctrl + X
            && nChar != 3 //Ctrl + C
            && nChar != 22 //Ctrl + V
            && nChar != 25 //Ctrl + Y
            && (m_bUintOnly ? 1 : nChar != 45)
            )
        {
            return;
        }
        int nStartChar = 0;
        int nEndChar = 0;
        GetSel(nStartChar, nEndChar);
        CString str;
        GetWindowText(str);
        CString strAdd;
        strAdd = (wchar_t)nChar;
        if (0 == nStartChar && 0 == nEndChar && ("." == strAdd || ("0" == strAdd && !str.IsEmpty())))
        {
            return;
        }
        else if (0 == nStartChar && nEndChar < str.GetLength() && ("0" == strAdd || "." == strAdd))
        {
            return;
        }
        else if (str.Find(".") >= 0 && "." == strAdd)
        {
            return;
        }
        else if ("0" == str)
        {
            if (1 == nStartChar && 1 == nEndChar && "." != strAdd && 8 != nChar)
            {
                return;
            }           
        }     
        else if (!m_bUintOnly && 45 == nChar)
        {
            if (nStartChar > 0 || (0 == nStartChar && 0 == nEndChar && str.Find("-") >= 0))
            {
                return;
            }
        }
    }
    else if(GetStyle() & ES_NUMBER)
    {
		if ((nChar < 48 || nChar > 57)  //0~9数字
			&& nChar != 8 //Backspace
			&& nChar != 26 //Ctrl + Z
			&& nChar != 24 //Ctrl + X
			&& nChar != 3 //Ctrl + C
			&& nChar != 22 //Ctrl + V
			&& nChar != 25 //Ctrl + Y
			)
		{
			return;
		}
        if (m_NumberControl)
        {
            int nStartChar = 0;
            int nEndChar = 0;
            GetSel(nStartChar, nEndChar);
            CString str;
            GetWindowText(str);
            CString strAdd;
            strAdd = (wchar_t)nChar;
            if (0 == nStartChar && 0 == nEndChar && "0" == strAdd && !str.IsEmpty())
            {
                return;
            }
            else if (0 == nStartChar && nEndChar < str.GetLength() && "0" == strAdd)
            {
                return;
            }
            if ("0" == str)
            {
                if (1 == nStartChar && 1 == nEndChar && 8 != nChar)
                {
                    return;
                }           
            }   
        }
    }
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CSimpleEdit::EnableFloat(BOOL bEnable, BOOL bUintOnly)
{
    m_bFloat = bEnable;
    m_bUintOnly = bUintOnly;
}

void CSimpleEdit::EnableNumberControl(BOOL bEnable)
{
    m_NumberControl = bEnable;
}

void CSimpleEdit::EnableButtonEdit(BOOL bEnable /*= TRUE*/)
{
	m_bEnbaleButtonEdit = bEnable;
}

void CSimpleEdit::GetHintText(CString& str)
{
	str = m_strWindowHint;
}

void CSimpleEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (13 == nChar)
	{
		NMHDR hdr;
		hdr.idFrom = GetDlgCtrlID();
		hdr.hwndFrom = GetSafeHwnd();
		hdr.code = NM_RETURN;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSimpleEdit::SetFont(HFONT hFont)
{
	m_hFont = hFont;
}




