
//*****************************************************
//********    By YaoShuLong 2017.7
//*****************************************************

#include "stdafx.h"
#include "SelectButtonPopupWindow.h"
#include "SimpleSelectButton.h"
#include "..\resource.h"

IMPLEMENT_DYNAMIC(CSelectButtonPopupWindow, CWnd)

CSelectButtonPopupWindow::CSelectButtonPopupWindow() 
{
    RegisterWindowClass();
    m_nHeight		= 0;
    m_nWidth		= 110;
    m_nItemCount = 0;
    m_pWndParent = NULL;
    m_bSeletctInstOK = FALSE;

}

CSelectButtonPopupWindow::~CSelectButtonPopupWindow()
{
}


BEGIN_MESSAGE_MAP(CSelectButtonPopupWindow, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_MESSAGE(MESSAGE_DLG, &CSelectButtonPopupWindow::OnUmCompareDlg)
    ON_WM_ACTIVATE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CSelectButtonPopupWindow::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SelectButtonPopupWindow_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SelectButtonPopupWindow_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSelectButtonPopupWindow::Create(CWnd * pParentWnd)
{  
    return CWnd::CreateEx(NULL,SelectButtonPopupWindow_CLASSNAME, _T(""), WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL);
}

int CSelectButtonPopupWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;


    m_listbox.Create(WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_VSCROLL, CRect(0,0,0,0), this, IDC_MYLISTBOX);

    m_edit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0,0,0,0), this, IDC_INPACEEDIT);
    m_edit.InitSettings(m_strHint);


    return 0;
}


void CSelectButtonPopupWindow::OnPaint()
{

    CPaintDC dc(this);
    CRect rCl;
    GetClientRect(&rCl);

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
            pOldBmp = dcMem.SelectObject(&bmpMem);
        else
            return;
    }
    else
        return;

    dcMem.FillSolidRect(rCl, RGB(63, 65, 77));  

    CRect rect;
    rect.SetRect(0, 25, rCl.right, 26);

    dcMem.FillSolidRect(rect, RGB(15, 15, 15));  

    dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
}

void CSelectButtonPopupWindow::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (::IsWindow(m_edit.GetSafeHwnd()))
    {
        m_edit.MoveWindow(0, 0, m_nWidth, 25, FALSE);
    }


    if (::IsWindow(m_listbox.GetSafeHwnd()))
    {
        m_listbox.MoveWindow(0, 26, m_nWidth, m_nItemCount * 20, TRUE);
    }

}

void CSelectButtonPopupWindow::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);

    if (WA_INACTIVE == nState)
    {
        if (!m_bSeletctInstOK)
        {
            CPoint pt;
            GetCursorPos(&pt);
            CRect rectButton;
            m_pWndParent->GetWindowRect(&rectButton);
            CRect rectTriangle;
            rectTriangle.SetRect(rectButton.right - 20, 0, rectButton.right, rectButton.bottom);
            if (!rectTriangle.PtInRect(pt))
            {
                this->ShowWindow(SW_HIDE);
            }
            
        }
        m_listbox.m_nIndex = -1;       
        m_listbox.SetFocus();
    }
    else if (WA_ACTIVE == nState)
    {
        m_edit.SetHintText(m_strHint);
        m_bSeletctInstOK = FALSE;
    }
}
LRESULT CSelectButtonPopupWindow::OnUmCompareDlg(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case MESSAGE_DLG_VK_UPANDDOWN:
        {
            UINT nChar = (UINT)lParam;
            m_listbox.PostMessage(WM_KEYDOWN, nChar, 0);
            break;
        }

    case MESSAGE_DLG_EDIT_CONFIRM:
        {
            ShowWindow(SW_HIDE);
            break;
        }

    case MESSAGE_DLG_LISTBOX_CONFIRM:
        {
            m_bSeletctInstOK = TRUE;
            int nCount = m_listbox.GetCount();
            if(nCount>0)
            {
                m_listbox.GetText(m_listbox.GetCurSel(), m_listbox.m_szCheck);
            }           
            ShowWindow(SW_HIDE);
            if (NULL != m_pWndParent)
            {
                m_pWndParent->SendMessage(MESSAGE_BUTTON, MESSAGE_BUTTON_CONFIRM, (LPARAM)(m_listbox.GetItemDataPtr(m_listbox.GetCurSel()))); 
            }
            break;
        }

    case MESSAGE_DLG_MOUSEWHEELUP:
        {
            CPoint* pPt = (CPoint*)lParam; 
            m_listbox.PostMessage(WM_MOUSEWHEEL, MAKELPARAM(0, WHEEL_DELTA), MAKELPARAM(pPt->x, pPt->y));  
            break;
        }

    case MESSAGE_DLG_MOUSEWHEELDOWN:
        {
            CPoint* pPt = (CPoint*)lParam; 
            m_listbox.PostMessage(WM_MOUSEWHEEL, MAKELPARAM(0, -WHEEL_DELTA), MAKELPARAM(pPt->x, pPt->y));  
            break;
        }

    case MESSAGE_DLG_EDIT_ENCHANGE:
        {
            if (NULL != m_pWndParent)
            {
                m_pWndParent->SendMessage(MESSAGE_BUTTON, MESSAGE_BUTTON_EN_CHANGE, lParam); 
            }
            break;
        }


    default:
        break;
    }

    return 0;
}

BOOL CSelectButtonPopupWindow::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

