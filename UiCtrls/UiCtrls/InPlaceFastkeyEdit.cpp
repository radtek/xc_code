//*********************************************************
//*********** Created by YaoShulong 3.24.2017 *****************
//*********************************************************

#include "stdafx.h"
#include "TCHAR.h"
#include "InPlaceFastkeyEdit.h"

#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include   <imm.h> 
#pragma   comment(lib, "imm32.lib ")

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFastkeyEdit

CInPlaceFastkeyEdit::CInPlaceFastkeyEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar)
{
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

CInPlaceFastkeyEdit::~CInPlaceFastkeyEdit()
{
}

BEGIN_MESSAGE_MAP(CInPlaceFastkeyEdit, CEdit)
    //{{AFX_MSG_MAP(CInPlaceFastkeyEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_WM_SETFOCUS()
    ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CInPlaceFastkeyEdit message handlers

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceFastkeyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
void CInPlaceFastkeyEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    EndEdit();
}

void CInPlaceFastkeyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_RETURN)
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

UINT CInPlaceFastkeyEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceFastkeyEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CInPlaceFastkeyEdit::PreTranslateMessage(MSG* pMsg) 
{
    // Catch the Alt key so we don't choke if focus is going to an owner drawn button
    if (pMsg->message == WM_SYSCHAR)
        return TRUE;
    
    return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceFastkeyEdit::PostNcDestroy() 
{
    CEdit::PostNcDestroy();
    
    delete this;	
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceFastkeyEdit implementation

void CInPlaceFastkeyEdit::EndEdit()
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


void CInPlaceFastkeyEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    ::ImmAssociateContext(this->m_hWnd, NULL);
}


void CInPlaceFastkeyEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (::GetKeyState(VK_MENU))
    {

    }

    CEdit::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
