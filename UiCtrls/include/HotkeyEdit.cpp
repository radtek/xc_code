
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

// SimpleEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "HotkeyEdit.h"

// CHotkeyEdit



IMPLEMENT_DYNAMIC(CHotkeyEdit, CEdit)

CHotkeyEdit::CHotkeyEdit()
{
   // RegisterWindowClass();
    m_bUseDefBrush = TRUE;
    m_nTextHeight = 0;
    m_nTextOffset = 8;

//    m_clrTextBk = RGB(63, 65, 77);
    m_clrTextBk = RGB(18, 20, 22);
//    m_clrText = RGB(215, 215, 215);
    m_clrText = RGB(255, 255, 255);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBkDef.CreateSolidBrush(m_clrTextBk);
    m_bEnable = TRUE;
    m_bSetSel = FALSE;
    m_bFloat = FALSE;

    m_font.CreateFont(-12,0,0,0,
        FW_NORMAL,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("微软雅黑"));
}

CHotkeyEdit::~CHotkeyEdit()
{
}


BEGIN_MESSAGE_MAP(CHotkeyEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
//     ON_WM_NCCALCSIZE()
//     ON_WM_NCPAINT()
	ON_WM_MOUSEACTIVATE()
 //   ON_WM_LBUTTONDBLCLK()
 ON_WM_CHAR()
 ON_WM_KEYDOWN()
 ON_WM_KEYUP()
 ON_WM_SYSKEYDOWN()
 ON_WM_SYSKEYUP()
END_MESSAGE_MAP()



// CHotkeyEdit 消息处理程序

BOOL CHotkeyEdit::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, _T("CHotkeyEdit"), &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("CHotkeyEdit");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

HBRUSH CHotkeyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)(m_brushBkDef.GetSafeHandle());  
}

// void CHotkeyEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
// {
//     CRect rectWnd, rectClient;
// 
//     CFont *pFont = GetFont();
//     CRect rectText;
//     rectText.SetRectEmpty();
// 
//     CDC *pDC = GetDC();
//     CFont *pOld = pDC->SelectObject(pFont);
//     pDC->DrawText(_T("Ky"), rectText, DT_CALCRECT | DT_LEFT);
//     m_nTextHeight = rectText.Height();
//     pDC->SelectObject(pOld);
//     ReleaseDC(pDC);
// 
//     UINT uiCenterOffset = (m_rect.Height() - m_nTextHeight) / 2;
// 
//     lpncsp->rgrc[0].top +=uiCenterOffset;
//     lpncsp->rgrc[0].bottom -= uiCenterOffset;
// 
//     lpncsp->rgrc[0].left += m_nTextOffset;
//     lpncsp->rgrc[0].right -= m_nTextOffset;
// 
// 
// }

// void CHotkeyEdit::OnNcPaint()
// {
//     Default();
// 
//     CWindowDC dc(this);
//     CBrush Brush(m_clrTextBk);
// 
//     CRect rectLeft;
//     CRect rectRight;
//     rectLeft.SetRect(0, 0, m_nTextOffset, m_rect.Height());
//     rectRight.SetRect(m_rect.Width() - m_nTextOffset, 0, m_rect.Width(), m_rect.Height());
// 
//     UINT uiCenterOffset = (m_rect.Height() - m_nTextHeight) / 2;
//     CRect rectTop = m_rect;
//     rectTop.DeflateRect(0, 0, 0, uiCenterOffset + m_nTextHeight );
//     CRect rectBottom = m_rect;
//     rectBottom.DeflateRect(0, uiCenterOffset + m_nTextHeight, 0, 0);
// 
//     dc.FillRect(rectBottom, &Brush);
//     dc.FillRect(rectTop, &Brush);
// 
//     dc.FillRect(rectLeft, &Brush);
//     dc.FillRect(rectRight, &Brush);
//     
// }

void CHotkeyEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
   m_rect.SetRect(x, y, x + nWidth, y + nHeight );
   m_rect.OffsetRect(-m_rect.left, -m_rect.top);
   CEdit::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CHotkeyEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
    m_rect.SetRect(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
    m_rect.OffsetRect(-m_rect.left, -m_rect.top);
    CEdit::MoveWindow(lpRect, bRepaint);
}

void CHotkeyEdit::SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText)
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

void CHotkeyEdit::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

void CHotkeyEdit::SetTextOffset(int nTextOffset)
{
    m_nTextOffset = nTextOffset;
    Invalidate(FALSE);    
}

void CHotkeyEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
//     CRect rect;
//     GetClientRect(&rect);
//     SetWindowSize(rect);

    SetLimitText(nTextLimit);

    if (NULL == hFont)
    {
        SetFont(&m_font);
    }
    else
    {
        SetFont(CFont::FromHandle(hFont));
    }

 //   SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

}


void CHotkeyEdit::SetWindowSize(CRect rect)
{
    m_rect = rect;
}

void CHotkeyEdit::EnableWindow(BOOL bEnable/* = TRUE*/)
{
    if (IsWindowEnabled() == bEnable)
    {
        return;
    }
    
    m_bEnable = bEnable;
    Invalidate(FALSE);

}

BOOL CHotkeyEdit::IsWindowEnabled()
{
    return m_bEnable;
}

void CHotkeyEdit::EnableSetSelOnFocus(BOOL bSetSel)
{
    m_bSetSel = bSetSel;
}


// void CHotkeyEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
// {
//     if (m_bSetSel)
//     {
//         GetParent()->PostMessage(UM_SETCUL, GetDlgCtrlID(), NULL);
//     }
// 
//     CEdit::OnLButtonDblClk(nFlags, point);
// }m_strWindowHint

int CHotkeyEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (GetFocus() == this)  // 防止 DockTabPane 中 SetFocus
		return MA_ACTIVATE;

	return CEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);	
}

void CHotkeyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar!=VK_SHIFT)
        return;
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CHotkeyEdit::EnableFloat(BOOL bEnable)
{
    m_bFloat = bEnable;
}

void CHotkeyEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
//     if(m_HotKeyInput.Right(1)=='+'&&m_HotKeyInput.Right(5)!="Num +")
//     {
//         m_HotKeyInput=m_HotKeyInput.Left(m_HotKeyInput.GetLength()-1);
//         SetWindowText(m_HotKeyInput);
//     }
    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CHotkeyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//     if (GetKeyState(VK_SHIFT) < 0&&(m_HotKeyInput.Find("Shift+")<0))
//     {
//         if((m_HotKeyInput.Find("Ctrl+")==-1)&&(m_HotKeyInput.Find("Alt+")==-1))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Find('+')!=m_HotKeyInput.ReverseFind('+'))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Right(1)!='+')
//             m_HotKeyInput="";
//         m_HotKeyInput+="Shift+";
//     }
//     if (GetKeyState(VK_CONTROL) < 0&&(m_HotKeyInput.Find("Ctrl+")<0))
//     {
//         if((m_HotKeyInput.Find("Alt+")==-1)&&(m_HotKeyInput.Find("Shift+")==-1))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Find('+')!=m_HotKeyInput.ReverseFind('+'))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Right(1)!='+')
//             m_HotKeyInput="";
//         m_HotKeyInput+="Ctrl+";
//     }

//     if (GetKeyState(VK_MENU)<0&&(m_HotKeyInput.Find("Alt+")<0))
//     {
//         if((m_HotKeyInput.Find("Ctrl+")==-1)&&(m_HotKeyInput.Find("Shift+")==-1))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Find('+')!=m_HotKeyInput.ReverseFind('+'))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Right(1)!='+')
//             m_HotKeyInput="";
//         m_HotKeyInput+="Alt+";
//     }
//     if(GetKeyState(VK_CONTROL) < 0&&GetKeyState(VK_MENU) < 0&&GetKeyState(VK_SHIFT) < 0)
//         m_HotKeyInput="Shift+Ctrl+Alt+";
    if(nChar)
    {
//         if(m_HotKeyInput.Right(1)!='+')
//         {
//             if (GetKeyState(VK_CONTROL) < 0||GetKeyState(VK_MENU) < 0||GetKeyState(VK_SHIFT) < 0)
//             {
//                 int K=m_HotKeyInput.ReverseFind('+');
//                 m_HotKeyInput=m_HotKeyInput.Left(K+1);
//             }
//             else
//                 m_HotKeyInput="";
//         }
//         if(m_HotKeyInput.Right(5)=="Num +")
//         {
//             if(m_HotKeyInput.GetLength()>5&&(GetKeyState(VK_CONTROL) < 0||GetKeyState(VK_MENU) < 0||GetKeyState(VK_SHIFT) < 0))
//                 m_HotKeyInput=m_HotKeyInput.Left(m_HotKeyInput.GetLength()-5);
//             else
//                 m_HotKeyInput="";
//         }

        switch (nChar)
        {
        case VK_F1 : m_HotKeyInput = "F1"; 
            break;
        case VK_F2 : m_HotKeyInput = "F2"; 
            break;
        case VK_F3 : m_HotKeyInput = "F3"; 
            break;
        case VK_F4 : m_HotKeyInput = "F4"; 
            break;
        case VK_F5 : m_HotKeyInput = "F5"; 
            break;
        case VK_F6 : m_HotKeyInput = "F6"; 
            break;
        case VK_F7 : m_HotKeyInput = "F7"; 
            break;
        case VK_F8 : m_HotKeyInput = "F8"; 
            break;
        case VK_F9 : m_HotKeyInput = "F9"; 
            break;
        case VK_F10 : m_HotKeyInput = "F10"; 
            break;
        case VK_F11 : m_HotKeyInput = "F11"; 
            break;
        case VK_F12 : m_HotKeyInput = "F12"; 
            break;
        case 32 : m_HotKeyInput = "Space"; 
            break;
        case 33 : m_HotKeyInput = "PgUp"; 
            break;
        case 34 : m_HotKeyInput = "PgDn"; 
            break;
        case 35 : m_HotKeyInput = "End";
            break;
        case 36 : m_HotKeyInput = "Home"; 
            break;
        case 37 : m_HotKeyInput = "Left"; 
            break;
        case 38 : m_HotKeyInput = "Up"; 
            break;
        case 39 : m_HotKeyInput = "Right";
            break;
        case 40 : m_HotKeyInput="Down"; 
            break;
        case 45 : m_HotKeyInput="Insert"; 
            break;
        case 96 : m_HotKeyInput="Pad0"; 
            break;
        case 97 : m_HotKeyInput="Pad1"; 
            break;
        case 98 : m_HotKeyInput="Pad2"; 
            break;
        case 99 : m_HotKeyInput="Pad3"; 
            break;
        case 100 : m_HotKeyInput="Pad4"; 
            break;
        case 101 : m_HotKeyInput="Pad5"; 
            break;
        case 102 : m_HotKeyInput="Pad6"; 
            break;
        case 103 : m_HotKeyInput="Pad7"; 
            break;
        case 104 : m_HotKeyInput="Pad8"; 
            break;
        case 105 : m_HotKeyInput="Pad9";
            break;
        case 106 : m_HotKeyInput="Pad*"; 
            break;
        case 107 : m_HotKeyInput="Pad+"; 
            break;
        case 109 : m_HotKeyInput="Pad-"; 
            break;
        case 110 : m_HotKeyInput="Pad."; 
            break;
        case 111 : m_HotKeyInput="Pad/";
            break;
//         case 144 : m_HotKeyInput="PadNumLock"; 
//             break;
        case 186 : m_HotKeyInput=";"; 
            break;
        case 187 : m_HotKeyInput="="; 
            break;
        case 188 : m_HotKeyInput=","; 
            break;
        case 189 : m_HotKeyInput="-"; 
            break;
        case 190 : m_HotKeyInput="."; 
            break;
        case 191 : m_HotKeyInput="/"; 
            break;
        case 192 : m_HotKeyInput="~"; 
            break;
        case 219 : m_HotKeyInput="["; 
            break;
        case 220 : m_HotKeyInput="\\"; 
            break;
        case 221 : m_HotKeyInput="]"; 
            break;
        case 222 : m_HotKeyInput="'"; 
            break;
        case VK_DELETE : m_HotKeyInput="Delete";
            break;
        case VK_RETURN : m_HotKeyInput="Enter";
            break;
        case VK_SHIFT : m_HotKeyInput="Shift";
            break;
        default:
            {char key=(char)toascii(nChar);
            if((!(key<'A')&&!(key>'Z'))||(!(key<'0')&&!(key>'9')))
                m_HotKeyInput=key;
            if(key==8)
                m_HotKeyInput="";
            }
            break;
        }
    }
    if (!m_HotKeyInput.IsEmpty())
    {        
        SetWindowText(m_HotKeyInput);
    }
    else
    {
        if (VK_BACK == nChar)
        {
            SetWindowText(m_HotKeyInput);
        }
    }
    
    if (VK_DELETE != nChar)
    {
        CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
    }
    
}


void CHotkeyEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
//     if (GetKeyState(VK_MENU)<0&&(m_HotKeyInput.Find("Alt+")<0))
//     {
//         if((m_HotKeyInput.Find("Ctrl+")==-1)&&(m_HotKeyInput.Find("Shift+")==-1))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Find('+')!=m_HotKeyInput.ReverseFind('+'))
//             m_HotKeyInput="";
//         if(m_HotKeyInput.Right(1)!='+')
//             m_HotKeyInput="";
//         m_HotKeyInput+="Alt+";
//     }
//     if(GetKeyState(VK_CONTROL) < 0&&GetKeyState(VK_MENU) < 0&&GetKeyState(VK_SHIFT) < 0)
//         m_HotKeyInput="Shift+Ctrl+Alt+";
    
    OnKeyDown (nChar,nRepCnt,nFlags);
}

void CHotkeyEdit::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    OnKeyUp(nChar, nRepCnt, nFlags);
}
