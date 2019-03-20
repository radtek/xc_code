// InPlaceEdit.cpp : implementation file
//
// Adapted by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// The code contained in this file is based on the original
// CInPlaceIntEdit from http://www.codeguru.com/listview/edit_subitems.shtml
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10+
//
// History:
//         10 May 1998  Uses GVN_ notifications instead of LVN_,
//                      Sends notification messages to the parent, 
//                      instead of the parent's parent.
//         15 May 1998  There was a problem when editing with the in-place editor, 
//                      there arises a general protection fault in user.exe, with a 
//                      few qualifications:
//                         (1) This only happens with owner-drawn buttons;
//                         (2) This only happens in Win95
//                         (3) This only happens if the handler for the button does not 
//                             create a new window (even an AfxMessageBox will avoid the 
//                             crash)
//                         (4) This will not happen if Spy++ is running.
//                      PreTranslateMessage was added to route messages correctly.
//                      (Matt Weagle found and fixed this problem)
//         26 Jul 1998  Removed the ES_MULTILINE style - that fixed a few probs!
//          6 Aug 1998  Added nID to the constructor param list
//          6 Sep 1998  Space no longer clears selection when starting edit (Franco Bez)
//         10 Apr 1999  Enter, Tab and Esc key prob fixed (Koay Kah Hoe)
//                      Workaround for bizzare "shrinking window" problem in CE
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCHAR.h"
#include "InPlaceIntEdit.h"

#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CInPlaceIntEdit

CInPlaceIntEdit::CInPlaceIntEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar, int nStyle)
{
    m_nStyle = nStyle;
    m_clrTextBk = RGB(18, 20, 22);
    m_clrText = RGB(255, 255, 255);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_sInitText     = sInitText;
    m_nRow          = nRow;
    m_nColumn       = nColumn;
    m_nLastChar     = 0; 
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
                                                     // then no exit on arrows

    m_Rect = rect;  // For bizarre CE bug.
    
    DWORD dwEditStyle = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL //|ES_MULTILINE
        | dwStyle;
    if (!Create(dwEditStyle, rect, pParent, nID)) return;
    
    SetFont(pParent->GetFont());
    
    SetWindowText(sInitText);
    SetFocus();
    
    switch (nFirstChar){
        case VK_LBUTTON: 
        case VK_RETURN:   SetSel((int)_tcslen(m_sInitText), -1); return;
        case VK_BACK:     SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_TAB:
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:
        case VK_SPACE:
        case VK_END:      SetSel(0,-1); return;
        default:          SetSel(0,-1);
    }

    // Added by KiteFly. When entering DBCS chars into cells the first char was being lost
    // SenMessage changed to PostMessage (John Lagerquist)
    if( nFirstChar < 0x80)
        PostMessage(WM_CHAR, nFirstChar);   
    else
        PostMessage(WM_IME_CHAR, nFirstChar);
}

CInPlaceIntEdit::~CInPlaceIntEdit()
{
}

BEGIN_MESSAGE_MAP(CInPlaceIntEdit, CEdit)
    //{{AFX_MSG_MAP(CInPlaceIntEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
    ON_WM_CREATE()
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CInPlaceIntEdit message handlers

HBRUSH CInPlaceIntEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)(m_brushBk.GetSafeHandle());  
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceIntEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
        nChar == VK_DOWN  || nChar == VK_UP   ||
        nChar == VK_RIGHT || nChar == VK_LEFT) &&
        (m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

// As soon as this edit loses focus, kill it.
void CInPlaceIntEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    EndEdit();
}

void CInPlaceIntEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_TAB || nChar == VK_RETURN)
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();    // This will destroy this window
        return;
    }
    if (nChar == VK_ESCAPE) 
    {
        SetWindowText(m_sInitText);    // restore previous text
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }

    if(INT_INPUT == m_nStyle)
    {
        if ((nChar < 48 || nChar > 57)  //0~9数字
            && nChar != 8 //Backspace
            && nChar != 20//大小写
            && nChar != 26 //Ctrl + Z
            && nChar != 24 //Ctrl + X
            && nChar != 3 //Ctrl + C
            && nChar != 22 //Ctrl + V
            && nChar != 25 //Ctrl + Y
            && nChar != 45
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
        if (45 == nChar)
        {
            if (nStartChar != 0)
            {
                return;
            }
            else
            {
                if (str.Find("-") >= 0 && 0 == nEndChar)
                {
                    return;
                }
                if ("0" == str && 0 != nEndChar)
                {
                    return;
                }
            }
        }
        if ("-" == str)
        {
            if (0 == nStartChar && 0 == nEndChar)
            {
                return;
            }
            if (1 == nStartChar && "0" == strAdd)
            {
                return;
            }
        }
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
    else
    {
        if ((nChar < 48 || nChar > 57)  //0~9数字
            && nChar != 46 //小数点.
            && nChar != 8 //Backspace
            && nChar != 20//大小写
            && nChar != 26 //Ctrl + Z
            && nChar != 24 //Ctrl + X
            && nChar != 3 //Ctrl + C
            && nChar != 22 //Ctrl + V
            && nChar != 25 //Ctrl + Y
            && nChar != 45
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
        if (45 == nChar)
        {
            if (nStartChar != 0)
            {
                return;
            }
            else
            {
                if (str.Find("-") >= 0 && 0 == nEndChar)
                {
                    return;
                }

            }
        }
        if ("-" == str)
        {
            if (0 == nStartChar && 0 == nEndChar)
            {
                return;
            }
            if ("." == strAdd && 1 == nStartChar)
            {
                return;
            }
        }
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
    }
    /////////////////////////////////////////////////////////123
    CEdit::OnChar(nChar, nRepCnt, nFlags);
    
    // Resize edit control if needed
    
    // Get text extent
    CString str;
    GetWindowText( str );

    // add some extra buffer
    str += _T("  ");
    
    CWindowDC dc(this);
    CFont *pFontDC = dc.SelectObject(GetFont());
    CSize size = dc.GetTextExtent( str );
    dc.SelectObject( pFontDC );
       
    // Get client rect
    CRect ParentRect;
    GetParent()->GetClientRect( &ParentRect );
    
    // Check whether control needs to be resized
    // and whether there is space to grow
    if (size.cx > m_Rect.Width())
    {
        if( size.cx + m_Rect.left < ParentRect.right )
            m_Rect.right = m_Rect.left + size.cx;
        else
            m_Rect.right = ParentRect.right;
        MoveWindow( &m_Rect );
    }
}

UINT CInPlaceIntEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceIntEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CInPlaceIntEdit::PreTranslateMessage(MSG* pMsg) 
{
    // Catch the Alt key so we don't choke if focus is going to an owner drawn button
    if (pMsg->message == WM_SYSCHAR)
        return TRUE;
    
    return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceIntEdit::PostNcDestroy() 
{
    CEdit::PostNcDestroy();
    
    delete this;	
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceIntEdit implementation

void CInPlaceIntEdit::EndEdit()
{
    CString str;

    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    static BOOL bAlreadyEnding = FALSE;

    if(bAlreadyEnding)
        return;

    bAlreadyEnding = TRUE;
    GetWindowText(str);

    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.strText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar;

    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
        SendMessage(WM_CLOSE, 0, 0);
    bAlreadyEnding = FALSE;
}
