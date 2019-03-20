
//*****************************************************
//********    PriceButton By YaoShuLong 2017.5
//*****************************************************

#include "stdafx.h"
#include "PriceButton.h"


// CPriceButton

IMPLEMENT_DYNAMIC(CPriceButton, CWnd)

CPriceButton::CPriceButton() :
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE),
m_strText(_T(""))
{
    RegisterWindowClass();
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
    m_hFont = (HFONT)m_font.GetSafeHandle();

    m_clrBk = RGB(18, 20, 22);

    m_clrText = RGB(215, 215, 215);
}

CPriceButton::~CPriceButton()
{
}


BEGIN_MESSAGE_MAP(CPriceButton, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CPriceButton 消息处理程序

void CPriceButton::OnPaint()
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

    dcMem.FillSolidRect(&rCl, RGB(18, 20, 22));

    CFont* pFontOld = dcMem.SelectObject(CFont::FromHandle(m_hFont));
    COLORREF clrBkOld = dcMem.SetBkMode(TRANSPARENT);
    COLORREF clrBkTextOld = dcMem.SetTextColor(m_clrText);
    UINT nStyple = DT_VCENTER | DT_SINGLELINE;
    if(GetStyle() & SS_LEFT)
    {
        nStyple = nStyple | DT_LEFT ;
    }
    else if (GetStyle() & SS_RIGHT)
    {
        nStyple = nStyple | DT_RIGHT;
    }
    else if (GetStyle() & SS_CENTER)
    {
        nStyple = nStyple | DT_CENTER;
    }
    dcMem.DrawText(m_strText, &rCl, nStyple);
    CPen* pOldPen = NULL;

    if (m_bMouseIn)
    {
        if (!m_bLMouseDown)
        {      
            CPen pen(PS_SOLID, 1, m_clrText);
            pOldPen = dcMem.SelectObject(&pen);
            CPoint ptsLine[2];
            ptsLine[0] = CPoint(rCl.left, rCl.top);
            ptsLine[1] = CPoint(rCl.left, rCl.bottom -1);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);

            ptsLine[0] = CPoint(rCl.left, rCl.bottom - 1);
            ptsLine[1] = CPoint(rCl.right - 1, rCl.bottom - 1);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);

            ptsLine[0] = CPoint(rCl.right - 1, rCl.bottom - 1);
            ptsLine[1] = CPoint(rCl.right - 1, rCl.top);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);

            ptsLine[0] = CPoint(rCl.right - 1, rCl.top);
            ptsLine[1] = CPoint(rCl.left, rCl.top);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);

        } 

    } 

    dc.BitBlt(rCl.left,rCl.top,rCl.Width(),rCl.Height(),&dcMem,rCl.left,rCl.top,SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.SelectObject(pFontOld);
    dcMem.SelectObject(pOldPen);
    dcMem.SetBkColor(clrBkOld);
    dcMem.SetTextColor(clrBkTextOld);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();
}

BOOL CPriceButton::Create(CString strText, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    m_strText = strText;

    return CWnd::Create(PriceButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CPriceButton::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, PriceButton_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = PriceButton_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CPriceButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
    {
        SetFocus();
    }
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);

    CWnd::OnLButtonDown(nFlags, point);
}

void CPriceButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    m_bLMouseDown = TRUE;
    NMHDR hdr;
    hdr.idFrom = GetDlgCtrlID();
    hdr.hwndFrom = GetSafeHwnd();
    hdr.code = NM_DBLCLK;
    GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
    Invalidate(FALSE);

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CPriceButton::OnLButtonUp(UINT nFlags, CPoint point)
{
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

void CPriceButton::OnMouseLeave()
{
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    if (!m_bLMouseDown)
    {
        Invalidate(FALSE);
    }
    CWnd::OnMouseLeave();
}


void CPriceButton::OnMouseMove(UINT nFlags, CPoint point)
{
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

void CPriceButton::SetFont(HFONT hFont)
{
    m_hFont = hFont;
    Invalidate(FALSE);
}

void CPriceButton::SetBkColor(COLORREF color)
{
    m_clrBk = color;
    Invalidate(FALSE);
}

void CPriceButton::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

BOOL CPriceButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CPriceButton::SetWindowText(CString str)
{
    m_strText = str;
    Invalidate(FALSE);
}

void CPriceButton::GetWindowText(CString& rString)
{
    rString = m_strText;
}

BOOL CPriceButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor(::LoadCursor(NULL  , IDC_HAND ) );
    return TRUE;
}






