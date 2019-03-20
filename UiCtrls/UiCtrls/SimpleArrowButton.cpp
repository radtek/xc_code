
//*****************************************************
//********    SimpleArrowButton By YaoShuLong 2017.2
//*****************************************************

#include "stdafx.h"
#include "SimpleArrowButton.h"
#include "..\resource.h"
// CSimpleArrowButton

IMPLEMENT_DYNAMIC(CSimpleArrowButton, CWnd)

CSimpleArrowButton::CSimpleArrowButton()
{
    RegisterWindowClass();
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    m_bEnable = TRUE;
    m_dwBtnStyle = 5;

}

CSimpleArrowButton::~CSimpleArrowButton()
{
}


BEGIN_MESSAGE_MAP(CSimpleArrowButton, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
END_MESSAGE_MAP()

// CSimpleArrowButton 消息处理程序

void CSimpleArrowButton::OnPaint()
{
    CPaintDC dc(this);

    CRect rCl;
    GetClientRect(&rCl);

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if (!m_bEnable)
    {
        if (ARROW_BUTTON_MOVEUP == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(IDB_BITMAP_ARROW_BUTTON_MOVEUP_DISABLE);
        }
        else if (ARROW_BUTTON_MOVEDOWN == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(IDB_BITMAP_ARROW_BUTTON_MOVEDOWN_DISABLE);
        }
        else if (ARROW_BUTTON_UP == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(IDB_BITMAP_ARROW_BUTTON_UP_DISABLED);
        }
        else if (ARROW_BUTTON_DOWN == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(IDB_BITMAP_ARROW_BUTTON_DOWN_DISABLED);
        }

    } 
    else
    {
        BOOL bBtnHover = FALSE;
        if (m_bMouseIn && !m_bLMouseDown)
        {
            bBtnHover = TRUE;
        }

        if (ARROW_BUTTON_UP == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_UP_HOVER : IDB_BITMAP_ARROW_BUTTON_UP);
        } 
        else if (ARROW_BUTTON_DOWN == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_DOWN_HOVER : IDB_BITMAP_ARROW_BUTTON_DOWN);
        }
        else if (ARROW_BUTTON_TORIGHT == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_TORIGHT_HOVER : IDB_BITMAP_ARROW_BUTTON_TORIGHT);
        }
        else if (ARROW_BUTTON_TOLEFT == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_TOLEFT_HOVER : IDB_BITMAP_ARROW_BUTTON_TOLEFT);
        }
        else if (ARROW_BUTTON_ADD == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_ADD_HOVER : IDB_BITMAP_ARROW_BUTTON_ADD);
        }
        else if (ARROW_BUTTON_ADDALL == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_ADDALL_HOVER : IDB_BITMAP_ARROW_BUTTON_ADDALL);
        }
        else if (ARROW_BUTTON_DELETE == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_DELETE_HOVER : IDB_BITMAP_ARROW_BUTTON_DELETE);
        }
        else if (ARROW_BUTTON_DELETEALL == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_DELETEALL_HOVER : IDB_BITMAP_ARROW_BUTTON_DELETEALL);
        }
        else if (ARROW_BUTTON_MOVEUP == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_MOVEUP_HOVER : IDB_BITMAP_ARROW_BUTTON_MOVEUP);
        }
        else if (ARROW_BUTTON_MOVEDOWN == m_dwBtnStyle)
        {
            bmpMem.LoadBitmap(bBtnHover ? IDB_BITMAP_ARROW_BUTTON_MOVEDOWN_HOVER : IDB_BITMAP_ARROW_BUTTON_MOVEDOWN);
        }

    }


    if(dcMem.CreateCompatibleDC(&dc))
    {
        pOldBmp = dcMem.SelectObject(&bmpMem);
    }
    else
    {
        return;
    }
        


    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();

}

BOOL CSimpleArrowButton::Create(UINT dwStyle, UINT dwBtnStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    m_dwBtnStyle = dwBtnStyle;
    return CWnd::Create(SimpleArrowButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleArrowButton::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleArrowButton_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleArrowButton_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleArrowButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bEnable)
    {
        return;
    }
//     if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
//     {
//         SetFocus();
//     }
    m_bLMouseDown = TRUE;
    SetTimer(1, 500, NULL);
    Invalidate(FALSE);
    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleArrowButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (!m_bEnable)
    {
        return;
    }
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleArrowButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    KillTimer(2);
    if (!m_bEnable)
    {
        return;
    }
    if (m_bLMouseDown)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_CLICK;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        m_bLMouseDown = FALSE;
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleArrowButton::OnMouseLeave()
{
    if (!m_bEnable)
    {
        return;
    }
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    Invalidate(FALSE);
    CWnd::OnMouseLeave();
}

void CSimpleArrowButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bEnable)
    {
        return;
    }
    if (!m_bMouseIn)
    {
        TRACKMOUSEEVENT csTME;
        csTME.cbSize = sizeof (csTME);    
        csTME.dwFlags = TME_LEAVE|TME_HOVER;    
        csTME.hwndTrack = GetSafeHwnd(); // 指定要 追踪 的窗口     
        csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER   
        ::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE 
        m_bMouseIn = TRUE;
        Invalidate(FALSE);
    }

    CWnd::OnMouseMove(nFlags, point);
}

void CSimpleArrowButton::EnableWindow(BOOL bEnable)
{  
    if (m_bEnable != bEnable)
    {
        m_bEnable = bEnable;
        Invalidate(FALSE);
    }
}

BOOL CSimpleArrowButton::IsWindowEnabled()
{  
    return m_bEnable;
}

BOOL CSimpleArrowButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleArrowButton::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 1:
        {
            KillTimer(1);
            if (m_bLMouseDown)
            {
                SetTimer(2, 50, NULL);
            }
            break;
        }
    case 2:
        {
            if (m_bLMouseDown)
            {
                NMHDR hdr;
                hdr.idFrom = GetDlgCtrlID();
                hdr.hwndFrom = GetSafeHwnd();
                hdr.code = NM_CLICK;
                GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);            
            }
            break;
        }
    default:
        break;
    }

    CWnd::OnTimer(nIDEvent);
}






