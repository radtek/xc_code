
//*****************************************************
//********    By YaoShuLong 2017.3
//*****************************************************

#include "stdafx.h"
#include "PlaceList.h"
#include "SimpleTitleBar.h"

IMPLEMENT_DYNAMIC(CPlaceList, CWnd)

CPlaceList::CPlaceList() :
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE),
m_strCaption(_T(""))
{
    m_nPopupPositionParent = MENU_NULL;
    m_pParent = NULL;
    RegisterWindowClass();
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
    m_hFont = (HFONT)m_font.GetSafeHandle();

    m_nCurPosition = -1;

    m_clrRegionDef = 0;

    m_clrText = RGB(255, 255, 255);

    m_clrRegionDef = RGB(78, 78, 78);
    
    m_clrOutline = RGB(50, 50, 50);
    m_clrRegionHover = RGB(51, 138, 249);
}

CPlaceList::~CPlaceList()
{
}


BEGIN_MESSAGE_MAP(CPlaceList, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ACTIVATE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CSimpleComboBox 消息处理程序

void CPlaceList::OnPaint()
{
    CPaintDC dc(this);

        // TODO: 在此处添加消息处理程序代码
        // 不为绘图消息调用 CWnd::OnPaint()
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

    Paint(dcMem);

    CPen pen1(PS_SOLID, 1, m_clrOutline);
    CPen* pOldPen = dcMem.SelectObject(&pen1);
    CPoint ptsLine[4];
    ptsLine[0] = CPoint(rCl.left, rCl.top);
    ptsLine[1] = CPoint(rCl.left, rCl.bottom -1);
    ptsLine[2] = CPoint(rCl.right - 1, rCl.bottom - 1);
    ptsLine[3] = CPoint(rCl.right - 1, rCl.top);
    dcMem.MoveTo(ptsLine[0]);
    dcMem.LineTo(ptsLine[1]);
    dcMem.LineTo(ptsLine[2]);
    dcMem.LineTo(ptsLine[3]);
    dcMem.LineTo(ptsLine[0]);
    
    dc.BitBlt(rCl.left,rCl.top,rCl.Width(),rCl.Height(),&dcMem,rCl.left,rCl.top,SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.SelectObject(pOldPen);
}


BOOL CPlaceList::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{  
    m_pParent = pParentWnd;
    return CWnd::CreateEx(NULL,PlaceList_CLASSNAME, _T(""), dwStyle, rect, pParentWnd ,NULL);
}

BOOL CPlaceList::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, PlaceList_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = PlaceList_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CPlaceList::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);

    CWnd::OnLButtonDown(nFlags, point);
}

void CPlaceList::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLMouseDown)
    {     
        if (NULL != m_pParent)
        {
            if (NULL != m_pParent->GetParent())
            {
                BOOL bResult = ((CSimpleTitleBar*)m_pParent)->SendMessageToParent();
                if (bResult)
                {
                    ShowWindow(SW_HIDE);
                }
            }
        }

        m_bLMouseDown = FALSE;
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CPlaceList::OnMouseLeave()
{ 
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    m_nCurPosition = -1;
    Invalidate(FALSE);
    CWnd::OnMouseLeave();
}


void CPlaceList::OnMouseMove(UINT nFlags, CPoint point)
{
    CPoint pointEx = point;
    
    if (!m_bMouseIn)
    {        
        TRACKMOUSEEVENT csTME;
        csTME.cbSize = sizeof (csTME);    
        csTME.dwFlags = TME_LEAVE|TME_HOVER;    
        csTME.hwndTrack = GetSafeHwnd(); // 指定要 追踪 的窗口     
        csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER   
        ::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE 
        m_bMouseIn = TRUE;
        
    }
    int nOldPosition = m_nCurPosition;
    m_nCurPosition = HitTest(point);
    if (nOldPosition != m_nCurPosition)
    {
        Invalidate(FALSE);
    }
    
    CWnd::OnMouseMove(nFlags, point);
}

void CPlaceList::FillRegion(CRect& rect, CDC& dc, COLORREF clr)
{
    CRgn rgn;
    rgn.DeleteObject();
    CPoint pts[4];
     pts[0] = CPoint(rect.left, rect.top);
     pts[1] = CPoint(rect.left, rect.bottom);
     pts[2] = CPoint(rect.right, rect.bottom );
     pts[3] = CPoint(rect.right, rect.top);
     rgn.CreatePolygonRgn(pts, 4, WINDING);
     CBrush brush(clr);
     dc.FillRgn(&rgn, &brush);	  
 
}

void CPlaceList::DrawText(CRect rect, CDC& dc, CString& strCaption, HFONT hFont, COLORREF clrTextBk, COLORREF clrText, BOOL bEnableSeparator)
{
    if (bEnableSeparator)
    {
        CPen pen(PS_SOLID, 1, RGB(189, 189, 189));
        CPen* pOldPen = dc.SelectObject(&pen);
        CPoint ptsLine[2];

        ptsLine[0] = CPoint(rect.left + 8, rect.top + 1);
        ptsLine[1] = CPoint(rect.right - 8, rect.top + 1);
        dc.MoveTo(ptsLine[0]);
        dc.LineTo(ptsLine[1]);
        dc.SelectObject(pOldPen);
    }
    else
    {
        CFont* pFontOld = dc.SelectObject(CFont::FromHandle(hFont));
        COLORREF clrBkOld = dc.SetBkColor(clrTextBk);
        COLORREF clrBkTextOld = dc.SetTextColor(clrText);

        rect.SetRect(rect.left + 8, rect.top, rect.right, rect.bottom);
        dc.DrawText(strCaption, &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

        dc.SelectObject(pFontOld);
        dc.SetBkColor(clrBkOld);
        dc.SetTextColor(clrBkTextOld);
    }
}

void CPlaceList::SetFont(HFONT hFont)
{
    m_hFont = hFont;
    Invalidate(FALSE);
}

void CPlaceList::SetRgnDefColor(COLORREF color)
{
    m_clrRegionDef = color;
    Invalidate(FALSE);
}

void CPlaceList::SetRgnHoverColor(COLORREF color)
{
    Invalidate(FALSE);
}

void CPlaceList::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

void CPlaceList::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);

    if (WA_INACTIVE == nState) 
    {
        ShowWindow(SW_HIDE);
    }
}


void CPlaceList::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CWnd::OnNcLButtonDown(nHitTest, point);
}

int CPlaceList::HitTest(CPoint point)
{
    for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
    {
        if (m_nPopupPositionParent == m_recPlaceListMenu[i].nMenuID)
        {
            for (size_t j = 0; j < m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); j ++)
            {           
                if (m_recPlaceListMenu[i].vecTextInfoPlaceList[j].rect.PtInRect(point))
                {
                    if (m_recPlaceListMenu[i].vecTextInfoPlaceList[j].bEnableSeparator)
                    {
                        return -1;
                    }
                    return j;
                }
            }
            break;
        }
    }

    return -1;
}

void CPlaceList::Paint(CDC& dc)
{
    for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
    {
        if (m_nPopupPositionParent == m_recPlaceListMenu[i].nMenuID)
        {
            for (size_t j = 0; j < m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); j++)
            {
                dc.FillSolidRect(&m_recPlaceListMenu[i].vecTextInfoPlaceList[j].rect, m_recPlaceListMenu[i].vecTextInfoPlaceList[j].nPosSub == m_nCurPosition ? m_clrRegionHover : m_clrRegionDef);
                DrawText(m_recPlaceListMenu[i].vecTextInfoPlaceList[j].rect, dc, m_recPlaceListMenu[i].vecTextInfoPlaceList[j].string, m_hFont, m_recPlaceListMenu[i].vecTextInfoPlaceList[j].nPosSub == m_nCurPosition ? m_clrRegionHover : m_clrRegionDef, m_clrText, m_recPlaceListMenu[i].vecTextInfoPlaceList[j].bEnableSeparator);
            }
            break;
        }
    }
}

BOOL CPlaceList::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

int CPlaceList::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    return MA_NOACTIVATE;
}

HFONT CPlaceList::GetFont()
{
    return m_hFont;
}