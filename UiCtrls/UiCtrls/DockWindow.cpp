#include "stdafx.h"

#include "dockwindow.h"

#include "Tracker.h"
//#pragma comment(lib, "user32.lib")  

#define DOCKWINDOW_CLASSNAME	"CDockWindow"

static bool			s_bDocking = false;

void DrawGhostRect(HDC hDC, RECT* pRect)
{
	//probably better use GetSystemMetrics
	enum{GhostRectSideSize=3};
	CBrush *pBrush = CDC::GetHalftoneBrush();
	if(pBrush->GetSafeHandle() != NULL)
	{
		HGDIOBJ hBrushOld = ::SelectObject(hDC, pBrush->GetSafeHandle());

		::PatBlt(hDC, pRect->left, pRect->top,
			pRect->right-pRect->left,GhostRectSideSize, PATINVERT);
		::PatBlt(hDC, pRect->left, pRect->bottom-GhostRectSideSize,
			pRect->right-pRect->left,GhostRectSideSize, PATINVERT);

		::PatBlt(hDC, pRect->left, pRect->top+GhostRectSideSize,
			GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);
		::PatBlt(hDC, pRect->right-GhostRectSideSize, pRect->top+GhostRectSideSize,
			GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);


		::SelectObject(hDC, hBrushOld);
	}

}

void CleanGhostRect(HDC hDC, RECT* pRect)
{
	DrawGhostRect(hDC, pRect);
}

IMPLEMENT_DYNAMIC(CDockWindow, CWnd)

CDockWindow::CDockWindow(CDockListener &listener)
	: m_listener(&listener)
	, m_bDocked(false)
	, m_bEnableDock(true)
	, m_bHideTabItem(false)
{
	m_rcUndock.SetRectEmpty();
}

CDockWindow::~CDockWindow(void)
{
	
}

BEGIN_MESSAGE_MAP(CDockWindow, baseClass)
	//{{AFX_MSG_MAP(CCustomTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	//ON_WM_CONTEXTMENU()
	ON_MESSAGE(UM_DOCKSTATECHANGE, OnDockStateChange)
	ON_MESSAGE(UM_GETDOCKWINDOWRECT, OnGetDockWindowRect)
	ON_MESSAGE(UM_FLOATWINDOW, OnFloatWindow)
	ON_WM_GETMINMAXINFO()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_SIZE() 
	//ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()


// Register the window class if it has not already been registered.

BOOL CDockWindow::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, DOCKWINDOW_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = DOCKWINDOW_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CDockWindow::Create(CWnd* pParent, LPCTSTR lpszCaption, const RECT& rect
		, DWORD dwStyle/* = WS_OVERLAPPEDWINDOW| WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/
		, DWORD dwExStyle/* = WS_EX_TOOLWINDOW*/)
{
	if (!RegisterWindowClass())
		return FALSE;

	if (!CWnd::CreateEx(dwExStyle, DOCKWINDOW_CLASSNAME, lpszCaption, dwStyle, rect, pParent, 0))
		return FALSE;

	m_rcUndock = rect;
	
	return TRUE;
}

BOOL CDockWindow::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDockWindow::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	s_bDocking = false;
	if (m_bEnableDock && !m_bDocked)
	{
		m_listener->SetDragFlag(TRUE);
	}
	//设置窗口拖动不显示内容拖动中不触发WM_MOVE
	//SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,FALSE,NULL,0);
	baseClass::OnNcLButtonDown(nHitTest, point);
	//SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,TRUE,NULL,0);
	if (IsWindow(GetSafeHwnd()))
	{
		m_listener->SetDragFlag(FALSE);
	}
	if (s_bDocking)
	{
		DockMe(m_dockRect);
	}

	/*if (HTCAPTION != nHitTest || !m_bEnableDock)
		baseClass::OnNcLButtonDown(nHitTest, point);
	else
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		
		if(::DragDetect(m_hWnd, point))
		{
			ScreenToClient(&point);
			BeginMoving(point);
		}	
	}*/
}

void CDockWindow::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	baseClass::OnWindowPosChanged(lpwndpos);
	if((lpwndpos->flags & SWP_SHOWWINDOW
		|| ((lpwndpos->flags & (SWP_NOSIZE | SWP_NOMOVE)) != (SWP_NOSIZE | SWP_NOMOVE)))
		&& !IsDocking()
		&& IsVisible()
		/*&& !(pWPos->flags&SWP_HIDEWINDOW)*/)
	{
		m_rcUndock.left		= lpwndpos->x;
		m_rcUndock.top		= lpwndpos->y;
		m_rcUndock.right	= m_rcUndock.left + lpwndpos->cx;
		m_rcUndock.bottom	= m_rcUndock.top + lpwndpos->cy;
	}
}

void CDockWindow::OnContextMenu(CWnd* pWnd, CPoint pt)
{
	if (!m_bDocked)
		return;
	
	//CMenu menu;
	//menu.CreatePopupMenu();
	//menu.AppendMenu(MF_STRING, 1000, "弹出");

	//UINT nID = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, pWnd);
	//if (1000 == nID)
	//{
	//	//m_listener->Undock(GetSafeHwnd());
	//	Float();
	//}
}

LRESULT CDockWindow::OnDockStateChange(WPARAM wParam, LPARAM lParam)
{
	m_bDocked = (bool)wParam;

	if (!m_bDocked)
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOP, m_rcUndock.left, m_rcUndock.top
				, m_rcUndock.right - m_rcUndock.left, m_rcUndock.bottom - m_rcUndock.top
				, SWP_FRAMECHANGED);
	}
	return 0;
}

LRESULT CDockWindow::OnFloatWindow(WPARAM wParam, LPARAM lParam)
{
	int x = (int)wParam;
	int y = (int)lParam;
	
	OffsetRect(&m_rcUndock, x - m_rcUndock.left - 20, y - m_rcUndock.top - 10);

	Float(&m_rcUndock);

	SendMessage(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, 0);

	//if (0x8000 & GetKeyState(VK_LBUTTON))
	//	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());

	/*tagINPUT input={0};
	// left down
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(tagINPUT));*/

	return 0;
}

void CDockWindow::OnClose()
{
	if (OnClosing())
		baseClass::OnClose();
}

bool CDockWindow::Float(LPCRECT pRc, UINT flags/* = SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_FRAMECHANGED*/
							, HWND hWndInsertAfter /*=HWND_TOP*/)
{
	bool bRes=IsDocking();
	if (bRes)
	{
		if(Undock())
			bRes=(::SetWindowPos(GetSafeHwnd(), hWndInsertAfter, pRc->left, pRc->top
			, pRc->right - pRc->left, pRc->bottom - pRc->top, flags) != FALSE);
	}
	return bRes;
}

bool CDockWindow::Float(void)
{
	bool bRes = !m_rcUndock.IsRectEmpty();
	if(bRes)
		bRes = Float(&m_rcUndock);
	return bRes;
}

bool CDockWindow::DockMe(SPDOCKRECT &dockRect)
{	
	dockRect.bHideTabItem = m_bHideTabItem;
	return m_listener->Dock(dockRect);
}

bool CDockWindow::Undock(void)
{
	ASSERT(IsDocking());

	return m_listener->Undock(GetSafeHwnd());
}

bool CDockWindow::OnClosing(void)
{
	bool bRes=true;
	if(IsDocking())
		bRes = Undock();

	return bRes;
}

bool CDockWindow::BeginMoving(const POINT& point)
{
	SPDOCKRECT dockRect;
	dockRect.hWnd = GetSafeHwnd();
	dockRect.dockBar = NULL;

	if(m_rcUndock.IsRectEmpty())
	{
		GetWindowRect(&dockRect.rect);
		//			dockHdr.hdr.code=DC_ADJUSTDRAGRECT;
		m_listener->AdjustDragRect(dockRect);
		m_rcUndock.CopyRect(&dockRect.rect);
	}
	GetWindowRect(&dockRect.rect);
	CPoint pt(point);
	ClientToScreen(&pt);
	
	// 		float ratio=float(pt.x-dockHdr.rect.left)/(dockHdr.rect.right-dockHdr.rect.left);
	// 		dockHdr.rect.left=pt.x-long(ratio*m_rcUndock.Width());
	dockRect.rect.left = pt.x - ::MulDiv(m_rcUndock.Width(), pt.x- dockRect.rect.left, dockRect.rect.right - dockRect.rect.left);
	// 		ratio=float(pt.y-dockHdr.rect.top)/(dockHdr.rect.bottom-dockHdr.rect.top);
	// 		dockHdr.rect.top=pt.y-long(ratio*m_rcUndock.Height());
	dockRect.rect.top = pt.y - ::MulDiv(m_rcUndock.Height(), pt.y- dockRect.rect.top, dockRect.rect.bottom- dockRect.rect.top);
		
	dockRect.rect.right		= dockRect.rect.left + m_rcUndock.Width();
	dockRect.rect.bottom	= dockRect.rect.top + m_rcUndock.Height();
	
	CSimpleMoveTracker tracker(GetSafeHwnd(), *m_listener, pt, dockRect);
	if(TrackDragAndDrop(tracker, m_hWnd))
	{
		CPoint ptCur;
		::GetCursorPos(&ptCur);
		if((ptCur.x != pt.x) || (ptCur.y != pt.y))
		{
			if(IsDocking())
				Float();
			if(dockRect.dockBar != NULL)
				DockMe(dockRect);
			else
			{
				::SetWindowPos(GetSafeHwnd(), HWND_TOP, dockRect.rect.left, dockRect.rect.top
					, dockRect.rect.right - dockRect.rect.left, dockRect.rect.bottom - dockRect.rect.top
					, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
		}
	}
	
	return true;
}

LRESULT CDockWindow::OnGetDockWindowRect(WPARAM wParam, LPARAM lParam)
{
	RECT *pRect = (RECT*)lParam;
	if (NULL == pRect)
		return 1;

	if (0 == wParam)
	{
		pRect->left		= m_rcUndock.left;
		pRect->top		= m_rcUndock.top;
		pRect->right	= m_rcUndock.right;
		pRect->bottom	= m_rcUndock.bottom;
	}
	
	return 0;
}

void CDockWindow::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	pMMI->ptMinTrackSize.x = 100;
	pMMI->ptMinTrackSize.y = 100;
}

void CDockWindow::OnMove(int cx, int cy)
{
	baseClass::OnMove(cx, cy);
}

void CDockWindow::OnMoving(UINT fwSide, LPRECT pRect)
{
	baseClass::OnMoving(fwSide, pRect);

	if (m_bEnableDock && !m_bDocked)
	{
		CPoint ptCur;
		::GetCursorPos(&ptCur);

		m_dockRect.hWnd = GetSafeHwnd();
		m_dockRect.dockBar = NULL;
		m_dockRect.rect.left = ptCur.x;
		m_dockRect.rect.top = ptCur.y;

		s_bDocking = false;
		

		if (m_listener->AdjustDragRect(m_dockRect))
		{
			if(m_dockRect.dockBar != NULL)
			{
				s_bDocking = true;

				/*HDC hWindowDC = ::GetWindowDC(NULL);
				CRect rcDock(m_dockRect.rect);
				rcDock.top -= 4;
				DrawGhostRect(hWindowDC, &rcDock);
				::ReleaseDC(NULL, hWindowDC);*/
			}
		}
	}
}

void CDockWindow::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd())
	{
		m_listener->SetDragFlag(FALSE);
	}
	baseClass::OnSize(nType, cx, cy);
}

void CDockWindow::SetWindowText(LPCTSTR lpszString)
{
	baseClass::SetWindowText(lpszString);
	if (m_bDocked)
	{
		m_listener->RefreshTabTitle(GetSafeHwnd());
	}
}

void CDockWindow::SetDockListener(CDockListener & listener)
{
	if (m_listener != &listener)
	{
		m_listener->SetDragFlag(FALSE);
		m_listener = &listener;
		m_listener->SetDragFlag(TRUE);
	}
}
