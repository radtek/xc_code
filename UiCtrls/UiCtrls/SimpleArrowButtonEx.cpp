
//*****************************************************
//********    SimpleArrowButton By YaoShuLong 2017.2
//*****************************************************

#include "stdafx.h"
#include "SimpleArrowButtonEx.h"
#include "..\resource.h"

// CSimpleArrowButtonEx

IMPLEMENT_DYNAMIC(CSimpleArrowButtonEx, CWnd)

CSimpleArrowButtonEx::CSimpleArrowButtonEx()
{
    RegisterWindowClass();
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    m_bEnable = TRUE;
    m_dwBtnStyle = 5;
    m_clrDef = RGB(63, 65, 77);
    m_clrHover = RGB(81, 83, 96);
    m_clrDisabled = RGB(52, 54, 69);
    m_clrText = RGB(215, 215, 215);
    m_clrTextDisabled = RGB(189, 189, 189);

    m_font.CreateFont(-10,0,0,0,
        FW_BOLD,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("西文正文"));
    m_bChangeCursor = FALSE;
}

CSimpleArrowButtonEx::~CSimpleArrowButtonEx()
{
}


BEGIN_MESSAGE_MAP(CSimpleArrowButtonEx, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CSimpleArrowButtonEx 消息处理程序

void CSimpleArrowButtonEx::OnPaint()
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
        {
            pOldBmp = dcMem.SelectObject(&bmpMem);
        }
        else
        {
            return;
        }

    }
    else
    {
        return;
    }

    if (m_bEnable)
    {
        if (m_bMouseIn)
        {
            if (m_bLMouseDown)
            {      
                dcMem.FillSolidRect(&rCl, m_clrDef);
                RoundRectEx(rCl, dcMem, RGB(18, 20, 22), m_clrDef);
                DrawText(rCl, dcMem, m_strCaption, &m_font, m_clrDef, m_clrText);

            } 
            else
            {
                dcMem.FillSolidRect(&rCl, m_clrHover);
                RoundRectEx(rCl, dcMem, RGB(18, 20, 22), m_clrHover);
                DrawText(rCl, dcMem, m_strCaption, &m_font, m_clrHover, m_clrText);
            }
        } 
        else
        {    
            dcMem.FillSolidRect(&rCl, m_clrDef);
            RoundRectEx(rCl, dcMem, RGB(18, 20, 22), m_clrDef);
            DrawText(rCl, dcMem, m_strCaption, &m_font, m_clrDef, m_clrText);
        }
    } 
    else
    {
        dcMem.FillSolidRect(&rCl, m_clrDisabled);
        RoundRectEx(rCl, dcMem, RGB(18, 20, 22), m_clrDisabled);
        DrawText(rCl, dcMem, m_strCaption, &m_font, m_clrDisabled, m_clrTextDisabled);
    }

    dc.BitBlt(rCl.left,rCl.top,rCl.Width(),rCl.Height(),&dcMem,rCl.left,rCl.top,SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();

}

BOOL CSimpleArrowButtonEx::Create(UINT dwStyle, BOOL bToRight, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    m_strCaption = bToRight ? ">" : "<";
    return CWnd::Create(SimpleArrowButtonEx_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleArrowButtonEx::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleArrowButtonEx_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleArrowButtonEx_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleArrowButtonEx::OnLButtonDown(UINT nFlags, CPoint point)
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

void CSimpleArrowButtonEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (!m_bEnable)
    {
        return;
    }
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleArrowButtonEx::OnLButtonUp(UINT nFlags, CPoint point)
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

void CSimpleArrowButtonEx::OnMouseLeave()
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

void CSimpleArrowButtonEx::OnMouseMove(UINT nFlags, CPoint point)
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

void CSimpleArrowButtonEx::EnableWindow(BOOL bEnable)
{  
    if (m_bEnable != bEnable)
    {
        m_bEnable = bEnable;
        Invalidate(FALSE);
        CWnd::EnableWindow(bEnable);
    }
    
}

BOOL CSimpleArrowButtonEx::IsWindowEnabled()
{  
    return m_bEnable;
}

BOOL CSimpleArrowButtonEx::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleArrowButtonEx::OnTimer(UINT_PTR nIDEvent)
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

void CSimpleArrowButtonEx::RoundRectEx(CRect& rect, CDC& dc, COLORREF clrBk, COLORREF clrButton)
{
//      double fClrButton = clrButton;
//      double fClrBk = clrBk;
//      double ai = 0.1;
//      double fColor1 = fClrButton * ai + fClrBk * (1 - ai);
//      COLORREF color1 = fColor1;
//      ai = 0.5;
//      double fColor2 = fClrButton * ai + fClrBk * (1 - ai);
//      COLORREF color2 = fColor2;
//      ai = 0.9;
//      COLORREF fColor3 = fClrButton * ai + fClrBk * (1 - ai);
//      double color3 = fColor3;


     int rButton = GetRValue(clrButton);
     int gButton = GetGValue(clrButton);
     int bButton = GetBValue(clrButton);

     int rBk = GetRValue(clrBk);
     int gBk = GetGValue(clrBk);
     int bBk = GetBValue(clrBk);

     double ai = 0.1;
     COLORREF r1 = rButton * ai + rBk * (1 - ai);
     COLORREF g1 = gButton * ai + gBk * (1 - ai);
     COLORREF b1 = bButton * ai + bBk * (1 - ai);

     ai = 0.5;
     COLORREF r2 = rButton * ai + rBk * (1 - ai);
     COLORREF g2 = gButton * ai + gBk * (1 - ai);
     COLORREF b2 = bButton * ai + bBk * (1 - ai);

     ai = 0.9;
     COLORREF r3 = rButton * ai + rBk * (1 - ai);
     COLORREF g3 = gButton * ai + gBk * (1 - ai);
     COLORREF b3 = bButton * ai + bBk * (1 - ai);



     CPen pen1(PS_SOLID, 1, RGB(r1, g1, b1));
     CPen* pOldPen = dc.SelectObject(&pen1);
     CPoint ptsLine[2];

     ptsLine[0] = CPoint(rect.left, rect.top);
     ptsLine[1] = CPoint(rect.left, rect.top + 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 1, rect.bottom - 1 );
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);
//****************************************************************************************************
     CPen pen2(PS_SOLID, 1, RGB(r2, g2, b2));
     dc.SelectObject(&pen2);

     ptsLine[0] = CPoint(rect.left, rect.top + 1);
     ptsLine[1] = CPoint(rect.left, rect.top + 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 1, rect.top);
     ptsLine[1] = CPoint(rect.left + 2, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 1);
     ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 2, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right -1 - 2, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top + 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right -1 - 1, rect.top);
     ptsLine[1] = CPoint(rect.right -1 - 2, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

// //****************************************************************************************************
     CPen pen3(PS_SOLID, 1, RGB(r3, g3, b3));
     dc.SelectObject(&pen3);

     ptsLine[0] = CPoint(rect.left, rect.top + 2);
     ptsLine[1] = CPoint(rect.left, rect.top + 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 2, rect.top);
     ptsLine[1] = CPoint(rect.left + 3, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 2);
     ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 2, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 3, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 2, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right - 1 - 3, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top + 2);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 2, rect.top);
     ptsLine[1] = CPoint(rect.right - 1 - 3, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     dc.SelectObject(pOldPen);

}

void CSimpleArrowButtonEx::DrawText(CRect& rect, CDC& dc, CString& strCaption, CFont* pFont, COLORREF clrTextBk, COLORREF clrText)
{

    CFont* pFontOld = dc.SelectObject(pFont);
    COLORREF clrBkOld = dc.SetBkColor(clrTextBk);
    COLORREF clrBkTextOld = dc.SetTextColor(clrText);

    dc.DrawText(strCaption, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

    dc.SelectObject(pFontOld);
    dc.SetBkColor(clrBkOld);
    dc.SetTextColor(clrBkTextOld);

}

void CSimpleArrowButtonEx::SetChangeCursor(BOOL bChangeCursor)
{
    m_bChangeCursor = bChangeCursor;
}

BOOL CSimpleArrowButtonEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bChangeCursor)
    {
        ::SetCursor(::LoadCursor(NULL  , IDC_HAND ) );
        return TRUE;
    }
    else
    {
        return CWnd::OnSetCursor(pWnd, nHitTest, message);
    }
}



