
//*****************************************************
//********    CompareEdit By YaoShuLong 2017.2
//*****************************************************

// CompareEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "CompareEdit.h"

// CCompareEdit



IMPLEMENT_DYNAMIC(CCompareEdit, CEdit)

CCompareEdit::CCompareEdit()
{
    m_bOnEnChange = FALSE;
    m_bHintMode = TRUE;
    m_nTextHeight = 0;
    m_nTextOffset = 8;

    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_bSetSel = TRUE;
    m_bOnConfirmInstToEmptyEdit = FALSE;
    m_bEnableSafeMode = TRUE;
    m_bInitalStatus = TRUE;

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

CCompareEdit::~CCompareEdit()
{
}


BEGIN_MESSAGE_MAP(CCompareEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
    ON_CONTROL_REFLECT(EN_CHANGE, &CCompareEdit::OnEnChange)
    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(UM_COMPAREEDIT, &CCompareEdit::OnCompareEdit)
END_MESSAGE_MAP()



// CCompareEdit 消息处理程序

HBRUSH CCompareEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor( m_bHintMode ? m_clrHintText : m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)m_brushBk.GetSafeHandle();  
}

void CCompareEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
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

void CCompareEdit::OnNcPaint()
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

void CCompareEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);
    GetParent()->PostMessage(UM_SETCUL, GetDlgCtrlID(), NULL);
    if (m_bHintMode)
    {
        SetWindowText("");
        m_bHintMode = FALSE;
    }
  //  SetSel(0, -1);
//     TRACE("CCompareEdit::OnSetFocus\r");
//     CString str;
//     GetWindowText(str);
//     m_compareDlg.m_compareInplaceEdit.SetWindowText(str);
//     m_compareDlg.m_compareInplaceEdit.SetSel(0, str.GetLength());
// 
//     CRect rect;
//     rect = m_rectEx;
//     ClientToScreen(&rect);    
//     m_compareDlg.SetWindowPos(&wndTop, rect.left - 8, rect.top - 4, rect.Width(), rect.Height(), SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
    

}

void CCompareEdit::OnKillFocus(CWnd* pNewWnd)
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

void CCompareEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
    m_compareDlg.m_nWidth = nWidth;
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rectEx.SetRect(0, 0, nWidth, nHeight);
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CCompareEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
    if (lpRect != NULL)
    {
        m_compareDlg.m_nWidth = lpRect->right - lpRect->left;
        m_rect.SetRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom );
        m_rectEx.SetRect(0, 0, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
        m_rect.OffsetRect(-m_rect.left, -m_rect.top);
    }
    
    CEdit::MoveWindow(lpRect, bRepaint);
}

void CCompareEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
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

void CCompareEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
}

void CCompareEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
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
//    SetWindowText(m_strWindowHint);
    if (m_bOnEnChange)
    {
        m_bOnEnChange = FALSE;
    }
    CEdit::SetWindowText(strHint);


    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

}



void CCompareEdit::SetWindowSize(CRect rect)
{
    m_rect = rect;
}

int CCompareEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;
 
    m_compareDlg.Create(this);  
    m_compareDlg.m_pEdit = this;
    return 0;
}

void CCompareEdit::OnEnChange()
{
    if (!m_bOnEnChange)
    {
        return;
    }

    CString str;
    GetWindowText(str);
    str.Trim();
    if (m_bEnableSafeMode)
    {
        SetWindowText(m_strOriginal);
    }
    else
    {
        SetWindowText("");
    }
   // SetWindowText(m_strOriginal);
    CRect rect;
    rect = m_rectEx;
    ClientToScreen(&rect);    
//     if (!str.IsEmpty())
//     {        
//         m_compareDlg.SetWindowPos(&wndTop, rect.left - 8, rect.top - 4, rect.Width(), 25 + m_compareDlg.m_nItemCountMax*20, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);       
//     }
//     else
//     {
//         m_compareDlg.SetWindowPos(&wndTop, rect.left - 8, rect.top - 4, rect.Width(), 25, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);        
//     }
    m_compareDlg.SetWindowPos(&wndTop, rect.left - 8, rect.top - 4, rect.Width(), 25 + m_compareDlg.m_nItemCountMax*20 + 2, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);      
    m_compareDlg.m_compareInplaceEdit.SetWindowText(str);
}

void CCompareEdit::SetWindowText(CString str)
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

void CCompareEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    m_bInitalStatus = FALSE;
    if (!m_bOnEnChange)
    {
        m_bOnEnChange = TRUE;
    }
    GetWindowText(m_strOriginal);
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CCompareEdit::OnCompareEdit(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case UM_COMPAREEDIT_SETWINDOWTEXTNULL:
        {
            SetWindowText("");
            break;
        }
    case UM_COMPAREEDIT_HIDE_DLG_ONT_BY_CONFIRM_INST:
        {
            if (m_bEnableSafeMode)
            {
                SetWindowText(m_strOriginal);
            }
            else
            {
                if (m_bInitalStatus)
                {
                    SetWindowText(m_strOriginal);
                }
                else
                {
                    CString str;
                    if (IsWindow(m_compareDlg.m_compareInplaceEdit.GetSafeHwnd()))
                    {
                        m_compareDlg.m_compareInplaceEdit.GetWindowText(str);
                    }                
                    SetWindowText(str);
                }

            }
            
            break;
        }

    case UM_COMPAREEDIT_OK:
        {
            if (!m_bOnConfirmInstToEmptyEdit)
            {
				CString str = m_compareDlg.m_listbox.m_szCheck;
				int nLength = str.GetLength();
				SetWindowText(str);
				
            }
             
            if (m_compareDlg.m_listbox.GetCurSel() >= 0)
            {
				CString str;
				m_compareDlg.m_listbox.GetText(m_compareDlg.m_listbox.GetCurSel(), str);
				GetParent()->SendMessage(UM_CHANGEINSTRUMENT, GetDlgCtrlID(), NULL);
            }
            
            break;
        }
    default:
        break;
    }
    return 0;
}

void CCompareEdit::EnableSetSelOnFocus(BOOL bSetSel)
{
    m_bSetSel = bSetSel;
}

BOOL CCompareEdit::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CCompareEdit::EnableOnConfirmInstToEmptyEdit(BOOL bEnable)
{
    m_bOnConfirmInstToEmptyEdit = bEnable;
}

void CCompareEdit::GetWindowText(CString& str)
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

void CCompareEdit::EmptyWindowText()
{
    if (m_bOnEnChange)
    {
        m_bOnEnChange = FALSE;
    }

    CEdit::SetWindowText("");
}

void CCompareEdit::EnableSafeMode(BOOL bEnable /*= TRUE*/)
{
    m_bEnableSafeMode = bEnable;
}

void CCompareEdit::SetStockCodeVec(vector<CString>& vecStockCode)
{
	m_compareDlg.m_vecStockCode = vecStockCode;
}




