

// CompareEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SelectButtonInplaceEdit.h"
#include "SimpleSelectButton.h"

// CSelectButtonInplaceEdit



IMPLEMENT_DYNAMIC(CSelectButtonInplaceEdit, CEdit)

CSelectButtonInplaceEdit::CSelectButtonInplaceEdit()
{
    m_bOnEnChange = FALSE;
    m_bHintMode = TRUE;
    m_nTextHeight = 0;
    m_nTextOffset = 8;

    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_bSetSel = FALSE;
    m_bOnConfirmInstToEmptyEdit = FALSE;
    m_nInputStyleID = INPUT_STYLE_NOLIMIT;

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

CSelectButtonInplaceEdit::~CSelectButtonInplaceEdit()
{
}


BEGIN_MESSAGE_MAP(CSelectButtonInplaceEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_CONTROL_REFLECT(EN_CHANGE, &CSelectButtonInplaceEdit::OnEnChange)
    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_CHAR()
END_MESSAGE_MAP()



// CSelectButtonInplaceEdit 消息处理程序

HBRUSH CSelectButtonInplaceEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor( m_bHintMode ? m_clrHintText : m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)m_brushBk.GetSafeHandle();  
}

void CSelectButtonInplaceEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
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

void CSelectButtonInplaceEdit::OnNcPaint()
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

void CSelectButtonInplaceEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);
    if (m_bHintMode)
    {
        SetWindowText("");
        m_bHintMode = FALSE;
    }
}

void CSelectButtonInplaceEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    CString str;
    CEdit::GetWindowText(str);
    str.Trim();
    if (str.IsEmpty())
    {
        SetWindowText(m_strWindowHint);
        m_bHintMode = TRUE;
    }
    else
    {
        m_bHintMode = FALSE;
    }
}

void CSelectButtonInplaceEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rectEx.SetRect(0, 0, nWidth, nHeight);
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CSelectButtonInplaceEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
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

void CSelectButtonInplaceEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
}

void CSelectButtonInplaceEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
    CRect rect;
    GetClientRect(&rect);
    SetWindowSize(rect);

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
    //SetWindowText(m_strWindowHint);
    SetHintText(strHint);
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

}



void CSelectButtonInplaceEdit::SetWindowSize(CRect rect)
{
    m_rect = rect;
}

void CSelectButtonInplaceEdit::OnEnChange()
{
    if (!m_bOnEnChange)
    {
        return;
    }
    CString str;
    GetWindowText(str);
    GetParent()->SendMessage(MESSAGE_DLG, MESSAGE_DLG_EDIT_ENCHANGE, (LPARAM)&str);   
}

void CSelectButtonInplaceEdit::SetWindowText(CString str)
{   
    if (m_bOnEnChange)
    {
        m_bOnEnChange = FALSE;
    }
    str.Trim();
    if (str.IsEmpty() && GetFocus() != this)
    {
        str = m_strWindowHint;
        m_bHintMode = TRUE;
    }
    else
    {
        m_bHintMode = FALSE;
    }
    
    CEdit::SetWindowText(str);
}

void CSelectButtonInplaceEdit::SetHintText(CString str)
{   
    if (m_bOnEnChange)
    {
        m_bOnEnChange = FALSE;
    }
    str = m_strWindowHint;
    m_bHintMode = TRUE;

    CEdit::SetWindowText(str);
}

void CSelectButtonInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!m_bOnEnChange)
    {
        m_bOnEnChange = TRUE;
    }
    GetWindowText(m_strOriginal);
    if (VK_RETURN == nChar)
    {
        GetParent()->SendMessage(MESSAGE_DLG, MESSAGE_DLG_EDIT_CONFIRM, NULL);   
        return;
    }
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSelectButtonInplaceEdit::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSelectButtonInplaceEdit::EnableOnConfirmInstToEmptyEdit(BOOL bEnable)
{
    m_bOnConfirmInstToEmptyEdit = bEnable;
}

void CSelectButtonInplaceEdit::GetWindowText(CString& str)
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

void CSelectButtonInplaceEdit::EmptyWindowText()
{
    if (m_bOnEnChange)
    {
        m_bOnEnChange = FALSE;
    }

    CEdit::SetWindowText("");
}

void CSelectButtonInplaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (INPUT_STYLE_DOUBLE == m_nInputStyleID)
    {        
        if ((nChar < 48 || nChar > 57)  //0~9数字
            && nChar != 46 //小数点.
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
        else if (true)
        {

        }
    }
    else if(INPUT_STYLE_INT == m_nInputStyleID)
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

    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CSelectButtonInplaceEdit::SetInputStyle(UINT nStyleID)
{
    m_nInputStyleID = nStyleID;
}



