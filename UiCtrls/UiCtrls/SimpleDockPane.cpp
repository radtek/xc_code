#include "stdafx.h"
#include "simpledockpane.h"
#include "TabDockManager.h"
#include "PubDefine.h"

#define UM_CLOSECLIENTVIEW		(WM_USER + 2000)

IMPLEMENT_DYNAMIC(CSimpleDockPane, CWnd)

CSimpleDockPane::CSimpleDockPane(void)
	: m_dwtabbedStyle(TS_TOP)
	, m_crBackClr(RGB(100, 100, 100))
	, m_pDragContext(NULL)
{
	m_pManager = new CTabDockManager(this, this);
}

CSimpleDockPane::~CSimpleDockPane(void)
{
	delete m_pManager;
}

// Register the window class if it has not already been registered.

BOOL CSimpleDockPane::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SIMPLEDOCKPANE_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SIMPLEDOCKPANE_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSimpleDockPane::Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle/* = TS_TOP*/)
{
	if (!RegisterWindowClass())
		return FALSE;

	if (!CWnd::CreateEx(0, SIMPLEDOCKPANE_CLASSNAME, lpszCaption, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	m_dwtabbedStyle = dwTabbedStyle;
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSimpleDockPane, CWnd)
	//{{AFX_MSG_MAP(CCustomTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_MESSAGE(UM_CLOSECLIENTVIEW, OnCloseClientView)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CSimpleDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

	return 0;
}

BOOL CSimpleDockPane::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CSimpleDockPane::OnPaint()
{
	CPaintDC dc(this);
	
	dc.FillSolidRect(m_rcClient, m_crBackClr);
}

void CSimpleDockPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GetClientRect(m_rcClient);
	m_pManager->UpdateRect(m_rcClient);
}

void CSimpleDockPane::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT trackmouseevent;
	trackmouseevent.cbSize = sizeof(trackmouseevent);
	trackmouseevent.dwFlags = TME_LEAVE;
	trackmouseevent.hwndTrack = GetSafeHwnd();
	trackmouseevent.dwHoverTime = 0;
	_TrackMouseEvent(&trackmouseevent);
	
}

LRESULT CSimpleDockPane::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

void CSimpleDockPane::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	
	m_pManager->OnLButtonDown(GetSafeHwnd(), point);
}

BOOL CSimpleDockPane::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd != this || nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	DWORD dwPos = ::GetMessagePos();
    CPoint pt(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	ScreenToClient(&pt);

	HCURSOR hCursor = m_pManager->GetCursor(pt);
	if (hCursor != NULL)
	{
		SetCursor(hCursor);

		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSimpleDockPane::DockWindow(HWND hDockWnd, const EM_SIDE side, HWND hDestWnd, CSize &size, BOOL bHideTabItem)
{
	SPDOCKPOS dockpos;
	memset(&dockpos, 0, sizeof(dockpos));
	
	dockpos.hParent			= GetSafeHwnd();
	dockpos.hWnd			= hDockWnd;
	dockpos.nHeight			= size.cy;
	dockpos.nWidth			= size.cx;
	dockpos.dwDockSide		= side;
	dockpos.hDestWnd		= hDestWnd;

	dockpos.bHideTabItem	= bHideTabItem;
	m_pManager->DockWindow(dockpos);
	return TRUE;
}

bool CSimpleDockPane::UndockWindow(HWND hDockWnd)
{
	return m_pManager->UndockWindow(hDockWnd);
}

void CSimpleDockPane::ClientCloseClick(HWND childWnd, HWND clientViewWnd)
{	
	PostMessage( UM_CLOSECLIENTVIEW, 0, reinterpret_cast<LPARAM>(clientViewWnd) );
}

LRESULT CSimpleDockPane::OnCloseClientView(WPARAM wParam, LPARAM lParam)
{
	HWND clientViewWnd = reinterpret_cast<HWND>(lParam);
	::SendMessage(clientViewWnd, WM_CLOSE, 0, 0);

	return 0;
}

void CSimpleDockPane::DragStart(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed)
{	
	m_pDragContext = new DragContext;
	m_pDragContext->startX = x;
	m_pDragContext->startY = y;
	
	//::SendMessage(clientViewWnd, UM_GETDOCKWINDOWRECT, 0, (LPARAM)&m_pDragContext->dockRect.rect);
	
	//::OffsetRect(&m_pDragContext->dockRect.rect, x, y);
	
	//DrawGhostRect(m_pDragContext->hDC, &m_pDragContext->dockRect.rect);
}

void CSimpleDockPane::DragOver(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed, bool &cancelDrag)
{
	//RECT &rect = m_pDragContext->dockRect.rect;
	//CleanGhostRect(m_pDragContext->hDC, &rect);
	//::OffsetRect(&rect, x - rect.left, y - rect.top);
	//AdjustDragRect(m_pDragContext->dockRect);
	//DrawGhostRect(m_pDragContext->hDC, &m_pDragContext->dockRect.rect);
	if (y - m_pDragContext->startY > 10)
	{
		cancelDrag = true;
		::PostMessage(clientViewWnd, UM_FLOATWINDOW, (WPARAM)x, (LPARAM)y);
	}
}

void CSimpleDockPane::DragDrop(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed)
{
	//RECT &rect = m_pDragContext->dockRect.rect;
	//CleanGhostRect(m_pDragContext->hDC, &rect);
	
	delete m_pDragContext;
	m_pDragContext = NULL;
}

void CSimpleDockPane::DragCancel(HWND childWnd, HWND clientViewWnd)
{
	delete m_pDragContext;
	m_pDragContext = NULL;
}

bool CSimpleDockPane::AdjustDragRect(SPDOCKRECT &dockRect)
{	
	return m_pManager->AdjustDragRect(dockRect);
}

bool CSimpleDockPane::Dock(SPDOCKRECT &dockRect)
{
	return m_pManager->Dock(dockRect);
}

bool CSimpleDockPane::SetDragFlag(BOOL bDragFlag)
{
	return m_pManager->SetDragFlag(bDragFlag);
}

void CSimpleDockPane::GetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
	m_pManager->GetDockInfo(vctDockInfo);
}

void CSimpleDockPane::SetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
	m_pManager->SetDockInfo(vctDockInfo);
	GetClientRect(m_rcClient);
	m_pManager->UpdateRect(m_rcClient);
}

void CSimpleDockPane::UpdateRect(CRect rc)
{
	m_pManager->UpdateRect(rc);
}

bool CSimpleDockPane::RefreshTabTitle(HWND hWnd)
{
	return m_pManager->RefreshTabTitle(hWnd);
}

void CSimpleDockPane::ClientActivate(HWND childWnd, HWND clientViewWnd)
{
	m_pManager->RefreshAllTab();
}
