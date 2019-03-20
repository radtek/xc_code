#include "stdafx.h"
#include "SimpleScrollBar.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HFSCROLLBAR_CLASSNAME _T("HFScrollBar")

#define HFSCROLLBARTIME		1001
#define HFSCROLLBARTIMEEX	1002
#define HFSCROLLBARHIDETIME	1003

#define d4_5_ 0.500001

#define HFSCROLLBARMINHEIGHT 18

#define	ZERO_CTRL 1e-10

// Triangle Style
enum
{
	HFSCROLLBAR_TRIANGLE_UP,
	HFSCROLLBAR_TRIANGLE_DOWN,
	HFSCROLLBAR_TRIANGLE_LEFT,
	HFSCROLLBAR_TRIANGLE_RIGHT,
};


IMPLEMENT_DYNAMIC(CSimpleScrollBar, CWnd)

BEGIN_MESSAGE_MAP(CSimpleScrollBar, CWnd)
	//{{AFX_MSG_MAP(CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_PBQ_SCROLLBAR_GETINFO, OnGetScrollInfo)
//	ON_MESSAGE(WM_PBQ_SCROLLBAR_SETINFO, OnSetScrollInfo)
	ON_MESSAGE(WM_MOUSEWHEEL, OnMouseWheel)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleScrollBar::CSimpleScrollBar()
{
	//m_hMutex = ::CreateMutex(NULL, FALSE, "PBQSCROLLBAR");
	m_dwStyle = SCROLLBAR_VERT;
	m_hBuddyWnd = NULL;
	m_nCurPos = 0;
	
	//m_crBk = RGB(37,39,53);            // 整个背景色 46 48 61
	m_crBk = RGB(46, 48, 61);
	m_crBorder = RGB(0xaa, 0xaa, 0xaa);
	m_crBar = RGB(61,91,153);          // 滚动条颜色
	m_crMouseInBar = RGB(98,174,229);   // 
	m_crBarBk = RGB(111, 110, 110);           // ?
	m_crBarBoder = RGB(0xb7, 0xb7, 0xb7);
	m_crTriangle = RGB(0xa9, 0xaa, 0xab);
	
	m_rcTriUp.SetRectEmpty();
	m_rcTriDown.SetRectEmpty();
	m_rcBar.SetRectEmpty();
	m_rcBlank1.SetRectEmpty();
	m_rcBlank2.SetRectEmpty();

	m_bCapture = false;
	m_bLButton = false;
	m_bTopTriangle = false;
	m_bBottomTriangle = false;
	m_bBlank1 = false;
	m_bBlank2 = false;
	m_bCompress = false;
	m_bBar = false;
	m_bMouseIn = false;
	m_bSetBarClr = true;                       //是否自定义滚动条颜色

	m_fPerHeight = 0.0;
	m_nPtDepart = 0;
	m_nStartPixel = 0;

	m_hBKParent = NULL;

	memset(&m_siSrcollBar, 0, sizeof(SCROLLINFO));
	m_bAutoShow = false;

	ChangeSchemes();
}

CSimpleScrollBar::~CSimpleScrollBar()
{
	if (m_BkBmp.GetSafeHandle())
		m_BkBmp.DeleteObject();

	if (m_BarBmp.GetSafeHandle())
		m_BarBmp.DeleteObject();

	if (m_memBkDC.GetSafeHdc())
		m_memBkDC.DeleteDC();

	if (m_memBarDC.GetSafeHdc())
		m_memBarDC.DeleteDC();

}

BOOL CSimpleScrollBar::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, HFSCROLLBAR_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;

		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);//LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = HFSCROLLBAR_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSimpleScrollBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!pParentWnd)
		return FALSE;
	m_dwStyle = dwStyle;

	BOOL bRet = FALSE;
	if (RegisterWindowClass())
	{
		bRet = CWnd::Create(HFSCROLLBAR_CLASSNAME, _T(""), CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW , rect, pParentWnd, nID);
	}
	
	return bRet;
}

int CSimpleScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_dwStyle == SCROLLBAR_VERT)
	{
		m_nStartPixel = rcClient.top + rcClient.Width();
	}
	else if (m_dwStyle == SCROLLBAR_HORZ)
	{
		m_nStartPixel = rcClient.left + rcClient.Height();
	}

	return 0;
}

UINT CSimpleScrollBar::ModifyHFStyle(UINT dwQlStyle)
{
	UINT ret = m_dwStyle;

	if(m_dwStyle != dwQlStyle)
	{
		m_dwStyle = dwQlStyle;
	}

	return ret;
}

// 通知窗体
BOOL CSimpleScrollBar::NotifyWnd(int nPos, WORD value)
{
	if((int)m_siSrcollBar.nPage > m_siSrcollBar.nMax - m_siSrcollBar.nMin)
	{
		return FALSE;
	}

	if (IsWindow(m_hBuddyWnd))
	{	
		if (m_dwStyle == SCROLLBAR_VERT)
		{
			return (BOOL)(::SendMessage(m_hBuddyWnd, WM_VSCROLL,(WPARAM)(MAKELONG(value, (WORD)nPos)),(LPARAM)m_hWnd));
		}
		else
		{
			return (BOOL)(::SendMessage(m_hBuddyWnd, WM_HSCROLL,(WPARAM)(MAKELONG(value, (WORD)nPos)),(LPARAM)m_hWnd));
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

int CSimpleScrollBar::GetScrollPos() const
{
	return m_nCurPos;
}

void CSimpleScrollBar::OnDestroy()
{
	
}

int CSimpleScrollBar::SetScrollPos(int nPos, BOOL bRedraw)
{
	m_nCurPos = nPos;

	if (bRedraw && m_hWnd)
	{
		ReDraw();
	}

	return 0;
}

void CSimpleScrollBar::GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
{
	*lpMinPos = m_siSrcollBar.nMin;
	*lpMaxPos = m_siSrcollBar.nMax;
}

void CSimpleScrollBar::SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw)
{
	m_siSrcollBar.nMin = nMinPos;
	m_siSrcollBar.nMax = nMaxPos;

	if (bRedraw && m_hWnd)
	{
		ReDraw();
	}
}

void CSimpleScrollBar::ShowScrollBar(BOOL bShow)
{

}

BOOL CSimpleScrollBar::EnableScrollBar(UINT nArrowFlags)
{
	BOOL ret = TRUE;
	return ret;
}

BOOL CSimpleScrollBar::SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	if (lpScrollInfo)
	{
		//memset(&m_siSrcollBar, 0, sizeof(SCROLLINFO));
		//memcpy(&m_siSrcollBar, lpScrollInfo, sizeof(SCROLLINFO));
		/*if (lpScrollInfo->fMask & SIF_ALL)
		{
			memcpy(&m_siSrcollBar, lpScrollInfo, sizeof(SCROLLINFO));
		}*/
		if (lpScrollInfo->fMask & SIF_PAGE)
		{
			m_siSrcollBar.nPage = lpScrollInfo->nPage;
		}
		if (lpScrollInfo->fMask & SIF_RANGE)
		{
			m_siSrcollBar.nMax = lpScrollInfo->nMax;
			m_siSrcollBar.nMin = lpScrollInfo->nMin;
		}
		if (lpScrollInfo->fMask & SIF_POS)
		{
			m_siSrcollBar.nPos = lpScrollInfo->nPos;
		}
		if (lpScrollInfo->fMask & SIF_TRACKPOS)
		{
			m_siSrcollBar.nTrackPos = lpScrollInfo->nTrackPos;
		}
		if (m_siSrcollBar.nPos + m_siSrcollBar.nPage >= m_siSrcollBar.nMax + 1 /*&& m_dwStyle == SCROLLBAR_VERT*/)
		{
			m_siSrcollBar.nPos = abs(m_siSrcollBar.nMax-m_siSrcollBar.nMin) >= m_siSrcollBar.nPage ? max((int)(m_siSrcollBar.nMax + 1 - m_siSrcollBar.nPage),m_siSrcollBar.nMin) : m_siSrcollBar.nMin;
		}
		//ASSERT(m_siSrcollBar.nPos >=m_siSrcollBar.nMin);
		if(m_siSrcollBar.nPos < m_siSrcollBar.nMin)
		{
			m_siSrcollBar.nPos = m_siSrcollBar.nMin;
		}
		m_nCurPos = m_siSrcollBar.nPos;
		bool bRet = CalcBar(0);

		if (bRet && bRedraw && m_hWnd && IsWindow(m_hWnd) && IsWindowVisible())
		{
			ReDraw();
		}
		else if (bRedraw && m_hWnd && IsWindow(m_hWnd) && IsWindowVisible())
		{
			memset(&m_siSrcollBar, 0, sizeof(SCROLLINFO));
			ReDraw();
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CSimpleScrollBar::GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	if (lpScrollInfo)
	{
		m_siSrcollBar.nPos = m_nCurPos;
		memcpy(lpScrollInfo, &m_siSrcollBar, sizeof(SCROLLINFO));
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

int CSimpleScrollBar::GetScrollLimit()
{
	return (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1);
}

CWnd* CSimpleScrollBar::SetBuddy(CWnd* pWndBuddy)
{
	if (pWndBuddy)
	{
		m_hBuddyWnd = pWndBuddy->m_hWnd;

		if (m_dwStyle == SCROLLBAR_VERT)
		{
			//::SendMessage(m_hBuddyWnd, WM_PBQ_SCROLLBAR_BUDDYSUCCESS, SCROLLBAR_HORZ&m_dwStyle, (LPARAM)m_hWnd);
		}
		else
		{
			//::SendMessage(m_hBuddyWnd, WM_PBQ_SCROLLBAR_BUDDYSUCCESS, SCROLLBAR_HORZ&m_dwStyle, (LPARAM)m_hWnd);
		}
	}

	return pWndBuddy;
}

LRESULT CSimpleScrollBar::OnGetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	return GetScrollInfo((LPSCROLLINFO)lParam);
}

LRESULT CSimpleScrollBar::OnSetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	return SetScrollInfo((LPSCROLLINFO)lParam);
}

CWnd* CSimpleScrollBar::GetBuddy()
{
	if (IsWindow(m_hBuddyWnd))
		return (CWnd*)(FromHandle(m_hBuddyWnd));

	return NULL;
}

void CSimpleScrollBar::PreSubclassWindow()
{
	InitMemBmp();
	ModifyStyle(0,WS_CLIPCHILDREN|SS_NOTIFY|SS_SIMPLE|WS_VISIBLE);
	CWnd::PreSubclassWindow();
}

void CSimpleScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bBar)
		return;

	SetCapture();
	CRect rcBar = m_rcBar;
	if (m_dwStyle == SCROLLBAR_VERT)
	{
		rcBar.InflateRect(5,0,5,0);
	}
	else
	{
		rcBar.InflateRect(0,5,0,5);
	}
	if (m_rcTriUp.PtInRect(point))
	{
		m_bTopTriangle = true;
		m_bBottomTriangle = false;
		m_bBlank1 = false;
		m_bBlank2 = false;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);
		m_nCurPos --;
		if (m_nCurPos < 0)
		{
			m_nCurPos = 0;
		}
		else
		{
			ReDraw();
			NotifyWnd(m_nCurPos, SB_LINEUP);
		}
	}
	else if (m_rcTriDown.PtInRect(point))
	{
		m_bTopTriangle = false;
		m_bBottomTriangle = true;
		m_bBlank1 = false;
		m_bBlank2 = false;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);
		m_nCurPos ++;
		if (m_nCurPos > (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
		{
			m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
		}
		else
		{
			ReDraw();

			NotifyWnd(m_nCurPos, SB_LINEDOWN);
		}
	}
	else if (m_rcBlank1.PtInRect(point))
	{
		m_bTopTriangle = false;
		m_bBottomTriangle = false;
		m_bBlank1 = true;
		m_bBlank2 = false;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);

		if (m_nCurPos == 0)
		{
		}
		else
		{
			m_nCurPos -= m_siSrcollBar.nPage;
			if (m_nCurPos < 0)
			{
				m_nCurPos = 0;
			}

			ReDraw();
			NotifyWnd(m_nCurPos, SB_PAGEUP);
		}
	}
	else if (m_rcBlank2.PtInRect(point))
	{
		m_bTopTriangle = false;
		m_bBottomTriangle = false;
		m_bBlank1 = false;
		m_bBlank2 = true;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);

		if (m_nCurPos == m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
		{
		}
		else
		{
			m_nCurPos += m_siSrcollBar.nPage;
			if (m_nCurPos > m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
			{
				m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
			}
			ReDraw();

			NotifyWnd(m_nCurPos, SB_PAGEDOWN);
		}
	}
	else if (rcBar.PtInRect(point))
	{
		m_ptOrg = point;
		m_bCapture = true;

		if (m_dwStyle == SCROLLBAR_VERT)
		{
			m_nPtDepart = point.y - m_nStartPixel;
		}
		else if (m_dwStyle == SCROLLBAR_HORZ)
		{
			m_nPtDepart = point.x - m_nStartPixel;
		}
		ReDraw();
	}
}

void CSimpleScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bBar)
		return;

	ReleaseCapture();
	KillTimer(HFSCROLLBARTIME);
	KillTimer(HFSCROLLBARTIMEEX);
	m_bTopTriangle = false;
	m_bBottomTriangle = false;
	m_bBlank1 = false;
	m_bBlank2 = false;

	if (m_bCapture || m_bLButton)
	{
		m_bCapture = false;
		m_bLButton = false;
	}
}

void CSimpleScrollBar::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	if (!m_bBar)
		return;

	if (m_rcTriUp.PtInRect(point))
	{
		m_bTopTriangle = true;
		m_bBottomTriangle = false;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);
		m_nCurPos --;
		if (m_nCurPos < 0)
		{
			m_nCurPos = 0;
		}
		else
		{
			ReDraw();
			NotifyWnd(m_nCurPos, SB_LINEUP);
		}
	}
	else if (m_rcTriDown.PtInRect(point))
	{
		m_bTopTriangle = false;
		m_bBottomTriangle = true;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);
		m_nCurPos ++;
		if (m_nCurPos > (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
		{
			m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
		}
		else
		{
			ReDraw();

			NotifyWnd(m_nCurPos, SB_LINEDOWN);
		}
	}
	else if (m_rcBlank1.PtInRect(point))
	{
		m_bBlank1 = true;
		m_bBlank2 = false;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);
		
		if (m_nCurPos == 0)
		{
		}
		else
		{
			m_nCurPos -= m_siSrcollBar.nPage;
			if (m_nCurPos < 0)
			{
				m_nCurPos = 0;
			}

			ReDraw();
			NotifyWnd(m_nCurPos, SB_PAGEUP);
		}
	}
	else if (m_rcBlank2.PtInRect(point))
	{
		m_bBlank1 = false;
		m_bBlank2 = true;
		KillTimer(HFSCROLLBARTIME);
		SetTimer(HFSCROLLBARTIME, 1000, NULL);

		if (m_nCurPos == m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
		{
		}
		else
		{
			m_nCurPos += m_siSrcollBar.nPage;
			if (m_nCurPos > m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
			{
				m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
			}
			ReDraw();

			NotifyWnd(m_nCurPos, SB_PAGEDOWN);
		}
	}
}

void CSimpleScrollBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseIn)
	{
		m_bMouseIn = true;
		ReDraw();
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = GetSafeHwnd();
	tme.dwFlags = TME_LEAVE;
	TrackMouseEvent(&tme);

	if (!m_bBar)
		return;

	if (m_bTopTriangle || m_bBottomTriangle || m_bBlank1 || m_bBlank2)
		return;

	if (m_bCapture || m_bLButton)
	{
		if (!m_rcTriUp.PtInRect(point) && m_bTopTriangle && m_bLButton)
		{
			m_bLButton = false;
		}
		else if (!m_rcTriDown.PtInRect(point) && m_bBottomTriangle && m_bLButton)
		{
			m_bLButton = false;
		}
		else if (!m_rcBlank1.PtInRect(point) && m_bBlank1 && m_bLButton)
		{
			m_bBlank1 = false;
		}
		else if (!m_rcBlank2.PtInRect(point) && m_bBlank2 && m_bLButton)
		{
			m_bBlank2 = false;
		}

		int nOldPos = 0;

		if (m_dwStyle == SCROLLBAR_VERT)
		{
			m_nStartPixel = point.y - m_nPtDepart;
		}
		else
		{
			m_nStartPixel = point.x - m_nPtDepart;
		}

		nOldPos = m_nCurPos;
		CalcBar(1);
		ReDraw();
		m_siSrcollBar.nTrackPos = m_nCurPos;
		if(nOldPos != m_nCurPos)
		{
			NotifyWnd(m_nCurPos, SB_THUMBTRACK);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CSimpleScrollBar::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(rcClient);

	CBitmap * pOldBkBmp = (CBitmap*)m_memBkDC.SelectObject(&m_BkBmp);
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_memBkDC, 0, 0, SRCCOPY);
	m_memBkDC.SelectObject(pOldBkBmp);
}

void CSimpleScrollBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if(cx>0 && cy>0)
	{
		ReDraw();
	}
}

LRESULT CSimpleScrollBar::OnMouseWheel(WPARAM wParam, LPARAM lParam) 
{
	if (m_dwStyle == SCROLLBAR_VERT || SCROLLBAR_HORZ)
	{
		//short zDelta = (short)GET_WHEEL_DELTA_WPARAM(wParam);
		short zDelta = (short)wParam;
		UINT nWheel = 0;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nWheel, 0); 

		if (m_siSrcollBar.nPage > m_siSrcollBar.nMax - m_siSrcollBar.nMin)
		{
			return TRUE;
		}
		if (zDelta == -120)
		{
			m_nCurPos += nWheel;
			if (m_nCurPos >= (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
			{
				m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
				NotifyWnd(m_nCurPos, SB_THUMBPOSITION);
				NotifyWnd(m_nCurPos, SB_ENDSCROLL);
			}
			else
			{
				NotifyWnd(m_nCurPos, SB_THUMBPOSITION);
			}

			CalcBar(0);
			ReDraw();
		}
		else if (zDelta == 120)
		{
			m_nCurPos -= nWheel;
			if (m_nCurPos <= 0)
			{
				m_nCurPos = 0;
				NotifyWnd(m_nCurPos, SB_THUMBPOSITION);
				NotifyWnd(m_nCurPos, SB_ENDSCROLL);
			}
			else
			{
				NotifyWnd(m_nCurPos, SB_THUMBPOSITION);
			}

			CalcBar(0);
			ReDraw();
		}
	}

	return TRUE;
}

void CSimpleScrollBar::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == HFSCROLLBARTIME)
	{
		KillTimer(HFSCROLLBARTIME);

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		if(m_bTopTriangle)
		{
			if(m_rcTriUp.PtInRect(pt))
			{
				m_bLButton = true;
				SetTimer(HFSCROLLBARTIMEEX, 50, NULL);
			}
		}
		else if (m_bBottomTriangle)
		{
			if(m_rcTriDown.PtInRect(pt))
			{
				m_bLButton = true;
				SetTimer(HFSCROLLBARTIMEEX, 50, NULL);
			}
		}
		else if (m_bBlank1)
		{
			if(m_rcBlank1.PtInRect(pt))
			{
				m_bLButton = true;

				SetTimer(HFSCROLLBARTIMEEX, 50, NULL);
			}
		}
		else if (m_bBlank2)
		{
			if(m_rcBlank2.PtInRect(pt))
			{
				m_bLButton = true;
				SetTimer(HFSCROLLBARTIMEEX, 50, NULL);
			}
		}
	}
	else if (nIDEvent == HFSCROLLBARTIMEEX)
	{
		if (m_bLButton)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);

			if(m_bTopTriangle)
			{
				if (!m_rcTriUp.PtInRect(pt))
				{
					m_bLButton = false;
					KillTimer(HFSCROLLBARTIMEEX);
					return;
				}
			}
			else if (m_bBottomTriangle)
			{
				if (!m_rcTriDown.PtInRect(pt))
				{
					m_bLButton = false;
					KillTimer(HFSCROLLBARTIMEEX);
					return;
				}
			}
			else if (m_bBlank1)
			{
				if (!m_rcBlank1.PtInRect(pt))
				{
					m_bLButton = false;
					KillTimer(HFSCROLLBARTIMEEX);
					return;
				}
			}
			else if (m_bBlank2)
			{
				if (!m_rcBlank2.PtInRect(pt))
				{
					m_bLButton = false;
					KillTimer(HFSCROLLBARTIMEEX);
					return;
				}
			}

			if (m_dwStyle == SCROLLBAR_VERT)
			{
				if (m_bTopTriangle)
				{
					m_nCurPos --;

					if (m_nCurPos < 0)
					{
						m_nCurPos = 0;
					}
					else
					{
						NotifyWnd(m_nCurPos, SB_LINEUP);
					}
				}
				else if (m_bBottomTriangle)
				{
					m_nCurPos ++;

					if (m_nCurPos > (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
					{
						m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
					}
					else
					{
						NotifyWnd(m_nCurPos, SB_LINEDOWN);
					}
				}
				else if (m_bBlank1)
				{
					if (m_nCurPos == 0)
					{

					}
					else
					{
						NotifyWnd(m_nCurPos, SB_PAGEUP);
					}

					m_nCurPos -= m_siSrcollBar.nPage;

					if (m_nCurPos < 0)
					{
						m_nCurPos = 0;
					}
				}
				else if (m_bBlank2)
				{
					if (m_nCurPos == m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
					{

					}
					else
					{
						NotifyWnd(m_nCurPos, SB_PAGEDOWN);
					}

					m_nCurPos += m_siSrcollBar.nPage;

					if (m_nCurPos > (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
					{
						m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
					}
				}

				CalcBar(0);
				ReDraw();
			}
			else
			{
				if (m_bTopTriangle)
				{
					m_nCurPos --;

					if (m_nCurPos < 0)
					{
						m_nCurPos = 0;
					}
					else
					{
						NotifyWnd(m_nCurPos, SB_LINEUP);
					}
				}
				else if (m_bBottomTriangle)
				{
					m_nCurPos ++;

					if (m_nCurPos > (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
					{
						m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
					}
					else
					{
						NotifyWnd(m_nCurPos, SB_LINEDOWN);
					}
				}
				else if (m_bBlank1)
				{
					if (m_nCurPos == 0)
					{

					}
					else
					{
						NotifyWnd(m_nCurPos, SB_PAGEUP);
					}

					m_nCurPos -= m_siSrcollBar.nPage;

					if (m_nCurPos <= 0)
					{
						m_nCurPos = 0;
					}
				}
				else if (m_bBlank2)
				{
					if (m_nCurPos == m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1)
					{

					}
					else
					{
						NotifyWnd(m_nCurPos, SB_PAGEDOWN);
					}

					m_nCurPos += m_siSrcollBar.nPage;

					if (m_nCurPos >= (m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1))
					{
						m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin - (int)m_siSrcollBar.nPage + 1;
					}
				}

				CalcBar(0);
				ReDraw();
			}
		}
		else
		{
			KillTimer(HFSCROLLBARTIMEEX);
		}
	}
	else if (nIDEvent == HFSCROLLBARHIDETIME)
	{
		KillTimer(HFSCROLLBARHIDETIME);
		ShowWindow(SW_HIDE);
	}

	CWnd::OnTimer(nIDEvent);
}

void CSimpleScrollBar::InitMemBmp(CDC* pDC)
{
	if(IsWindow(m_hWnd))
	{
		CRect rcClient;
		GetClientRect(rcClient);

		bool bNeedRelease = false;
		if (!pDC)
		{
			pDC = GetDC();
			if (!pDC || pDC->GetSafeHdc() == NULL)
				return;
			bNeedRelease = true;
		}

		if (!m_memBkDC.GetSafeHdc())
		{
			m_memBkDC.CreateCompatibleDC(pDC);
		}
		if (!m_memBarDC.GetSafeHdc())
		{
			m_memBarDC.CreateCompatibleDC(pDC);
		}

		if (m_BkBmp.GetSafeHandle())
		{
			BITMAP bm;
			m_BkBmp.GetBitmap(&bm);

			if (rcClient.Width() != bm.bmWidth && m_dwStyle == SCROLLBAR_HORZ)
			{
				m_BkBmp.DeleteObject();
				m_BkBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			}
			else if (rcClient.Height() != bm.bmHeight && m_dwStyle == SCROLLBAR_VERT)
			{
				m_BkBmp.DeleteObject();
				m_BkBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			}
		}
		else
		{
			m_BkBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
		}

		if ((m_siSrcollBar.nMax - m_siSrcollBar.nMin > 0) && (m_siSrcollBar.nMax - m_siSrcollBar.nPage >= 0))
		{
			if (m_BarBmp.GetSafeHandle())
			{
				BITMAP bm;
				m_BarBmp.GetBitmap(&bm);

				if (m_rcBar.Width() != bm.bmWidth && m_dwStyle == SCROLLBAR_HORZ)
				{
					m_BarBmp.DeleteObject();
					m_BarBmp.CreateCompatibleBitmap(pDC, m_rcBar.Width(), m_rcBar.Height());
				}
				else if (m_rcBar.Height() != bm.bmHeight && m_dwStyle == SCROLLBAR_VERT)
				{
					m_BarBmp.DeleteObject();
					m_BarBmp.CreateCompatibleBitmap(pDC, m_rcBar.Width(), m_rcBar.Height());
				}
			}
			else
			{
				m_BarBmp.CreateCompatibleBitmap(pDC, m_rcBar.Width(), m_rcBar.Height());
			}
		}

		if (bNeedRelease)
		{
			ReleaseDC(pDC);
		}
	}
}

void CSimpleScrollBar::ReDraw(CDC* pDC, bool bForce)
{
	//MutexLock mlock(m_hMutex);

	CalcAllRect();
	InitMemBmp(pDC);

	bool bNeedRelease = false;
	if (!pDC)
	{
		pDC = GetDC();
		if (!pDC || pDC->GetSafeHdc() == NULL)
			return;
		bNeedRelease = true;
	}

	CBitmap * pOldBkBmp = (CBitmap*)m_memBkDC.SelectObject(&m_BkBmp);
	CBitmap * pOldBarBmp = NULL;

	DrawBk();

	if ((m_siSrcollBar.nMax - m_siSrcollBar.nMin <= 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPage < 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPos < 0)
		|| (m_siSrcollBar.nPos + m_siSrcollBar.nPage > m_siSrcollBar.nMax+1) || m_siSrcollBar.nPos < 0)
	{
		if (bForce)
		{
			CRect rcClient;
			GetClientRect(rcClient);
			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_memBkDC, 0, 0, SRCCOPY);
		}

		m_memBkDC.SelectObject(pOldBkBmp);

		if (bNeedRelease)
		{
			ReleaseDC(pDC);
		}

		return;
	}
	else
	{
		pOldBarBmp = (CBitmap*)m_memBarDC.SelectObject(&m_BarBmp);
		DrawBar();

		if (m_dwStyle == SCROLLBAR_HORZ)
		{
			m_memBkDC.BitBlt(m_nStartPixel + 1, m_rcBar.top, m_rcBar.Width(), m_rcBar.Height(), &m_memBarDC, 0, 0, SRCCOPY);
		}
		else
		{
			m_memBkDC.BitBlt(m_rcBar.left, m_nStartPixel + 1, m_rcBar.Width(), m_rcBar.Height(), &m_memBarDC, 0, 0, SRCCOPY);
		}
	}

	if (bForce)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_memBkDC, 0, 0, SRCCOPY);
	}

	m_memBarDC.SelectObject(pOldBarBmp);
	m_memBkDC.SelectObject(pOldBkBmp);

	if (bNeedRelease)
	{
		ReleaseDC(pDC);
	}
}

bool CSimpleScrollBar::CalcBar(int nStyle)
{
	if ((m_siSrcollBar.nMax - m_siSrcollBar.nMin <= 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPage < 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPos < 0)
		|| (m_siSrcollBar.nPos + m_siSrcollBar.nPage > m_siSrcollBar.nMax+1))
	{
		return false;
	}

	if (IsSmallFloat(m_fPerHeight))
	{
		if(IsWindow(m_hWnd))
		{
			bool bRet = CalcAllRect();

			if (!bRet)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	int nPageHeight = 0;
	
	if (m_bCompress)
	{
		nPageHeight = HFSCROLLBARMINHEIGHT;
	}
	else
	{
		nPageHeight = (int)(m_fPerHeight*m_siSrcollBar.nPage+d4_5_);
	}

	float fPer = 0.0;

	if (m_siSrcollBar.nMax - m_siSrcollBar.nMin + 1 - m_siSrcollBar.nPage <= 0)
	{
		m_bBar = false;
		return false;
	}

	m_bBar = true;

	if (m_dwStyle == SCROLLBAR_VERT)
	{
		fPer = (float)(rcClient.Height() - 2*rcClient.Width() - nPageHeight)/(m_siSrcollBar.nMax - m_siSrcollBar.nMin + 1 - m_siSrcollBar.nPage);
	}
	else if (m_dwStyle == SCROLLBAR_HORZ)
	{
		fPer = (float)(rcClient.Width() - 2*rcClient.Height() - nPageHeight)/(m_siSrcollBar.nMax - m_siSrcollBar.nMin + 1 - m_siSrcollBar.nPage);
	}

	if (nStyle == 0)
	{
		if (m_dwStyle == SCROLLBAR_VERT)
		{
			m_nStartPixel = (int)(fPer*m_nCurPos+d4_5_) + m_rcBlank1.top;
		}
		else if (m_dwStyle == SCROLLBAR_HORZ)
		{
			m_nStartPixel = (int)(fPer*m_nCurPos+d4_5_) + m_rcBlank1.left;
		}
	}
	else if (nStyle == 1)
	{
		if (m_dwStyle == SCROLLBAR_VERT)
		{
			if (m_nStartPixel <= m_rcBlank1.top)
			{
				m_nStartPixel = m_rcBlank1.top;
			}
			else 
			{
				if (m_nStartPixel + nPageHeight >= m_rcBlank2.bottom)
				{	
					m_nStartPixel = m_rcBlank2.bottom - nPageHeight;
				}
			}

			if (m_rcBlank1.Height() <= m_rcBlank2.Height())
			{
				int nDepart = m_nStartPixel - (rcClient.top + rcClient.Width());

				m_nCurPos = (int)nDepart/fPer;
			}
			else
			{
				int nDepart = (rcClient.bottom - rcClient.Width()) - m_nStartPixel - nPageHeight;

				int nPos = (int)nDepart/fPer;
				if (fPer < 1 && nPos <= 1)
				{
					nDepart = 0;
				}
				m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin + 1 - m_siSrcollBar.nPage - (int)nDepart/fPer;
			}
		}
		else if (m_dwStyle == SCROLLBAR_HORZ)
		{
			if (m_nStartPixel < m_rcBlank1.left)
			{
				m_nStartPixel = m_rcBlank1.left;
			}
			else 
			{
				if (m_nStartPixel + nPageHeight >= m_rcBlank2.right)
				{
					m_nStartPixel = m_rcBlank2.right - nPageHeight;
				}
			}

			if (m_rcBlank1.Width() <= m_rcBlank2.Width())
			{
				int nDepart = m_nStartPixel - (rcClient.left + rcClient.Height());

				m_nCurPos = (int)nDepart/fPer;
				if (m_nCurPos > m_siSrcollBar.nMax)
					m_nCurPos = m_siSrcollBar.nMax;
			}
			else
			{
				int nDepart = (rcClient.right - rcClient.Height()) - m_nStartPixel - nPageHeight;

				int nPos = (int)nDepart/fPer;
				if (fPer < 1 && nPos <= 1)
				{
					nDepart = 0;
				}
				m_nCurPos = m_siSrcollBar.nMax - m_siSrcollBar.nMin + 1 - m_siSrcollBar.nPage - (int)nDepart/fPer;
				if (m_nCurPos > m_siSrcollBar.nMax)
					m_nCurPos = m_siSrcollBar.nMax;
			}
		}
	}

	return true;
}

bool CSimpleScrollBar::CalcAllRect()
{
	if ((m_siSrcollBar.nMax - m_siSrcollBar.nMin <= 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPage < 0) || (m_siSrcollBar.nMax - m_siSrcollBar.nPos < 0)
		|| (m_siSrcollBar.nPos + m_siSrcollBar.nPage > m_siSrcollBar.nMax+1))
	{
		return false;
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_dwStyle == SCROLLBAR_VERT)
	{
		m_rcTriUp = rcClient;
		m_rcTriUp.bottom = m_rcTriUp.top + rcClient.Width();
		m_rcTriDown = rcClient;
		m_rcTriDown.top = m_rcTriDown.bottom - rcClient.Width();
	}
	else if (m_dwStyle == SCROLLBAR_HORZ)
	{
		m_rcTriUp = rcClient;
		m_rcTriUp.right = m_rcTriUp.left + rcClient.Height();
		m_rcTriDown = rcClient;
		m_rcTriDown.left = m_rcTriDown.right - rcClient.Height();
	}

	if (m_siSrcollBar.nMax <= m_siSrcollBar.nMin || m_siSrcollBar.nPage == 0 || m_siSrcollBar.nPos < 0)
	{
		m_bCompress = false;
		return false;
	}

	if (m_dwStyle == SCROLLBAR_VERT)
	{
		m_fPerHeight = (float)(rcClient.Height() - 2*rcClient.Width())/(m_siSrcollBar.nMax - m_siSrcollBar.nMin+1);

		m_nStartPixel = rcClient.top + rcClient.Width() + m_nCurPos * m_fPerHeight + d4_5_;

		int nPageHeight = (int)(m_fPerHeight*m_siSrcollBar.nPage+d4_5_);
		if (nPageHeight < HFSCROLLBARMINHEIGHT)
		{
			m_bCompress = true;
			nPageHeight = HFSCROLLBARMINHEIGHT;

			m_fPerHeight = (float)(rcClient.Height() - 2*rcClient.Width() - HFSCROLLBARMINHEIGHT)/(m_siSrcollBar.nMax - m_siSrcollBar.nMin+1-m_siSrcollBar.nPage);
			m_nStartPixel = rcClient.top + rcClient.Width() + m_nCurPos * m_fPerHeight + d4_5_;
		}
		else
		{
			m_bCompress = false;
		}

		m_rcBar.top = m_nStartPixel;
		m_rcBar.bottom = m_rcBar.top + nPageHeight - 2;
		m_rcBar.left = rcClient.left + 4;
		m_rcBar.right = rcClient.right - 4;
		m_rcBar.InflateRect(-1, 0, -1, 0);

		m_rcBlank1 = rcClient;
		m_rcBlank1.top = m_rcTriUp.bottom;
		m_rcBlank1.bottom = m_rcBar.top;

		m_rcBlank2 = rcClient;
		m_rcBlank2.top = m_rcBar.bottom;
		m_rcBlank2.bottom = m_rcTriDown.top;
	}
	else if (m_dwStyle == SCROLLBAR_HORZ)
	{
		m_fPerHeight = (float)(rcClient.Width() - 2*rcClient.Height())/(m_siSrcollBar.nMax - m_siSrcollBar.nMin+1);
		m_nStartPixel = rcClient.left + rcClient.Height() + m_nCurPos * m_fPerHeight + d4_5_;

		int nPageHeight = (int)(m_fPerHeight*m_siSrcollBar.nPage+d4_5_);
		if (nPageHeight < HFSCROLLBARMINHEIGHT)
		{
			m_bCompress = true;
			nPageHeight = HFSCROLLBARMINHEIGHT;

			m_fPerHeight = (float)(rcClient.Width() - 2*rcClient.Height() - HFSCROLLBARMINHEIGHT)/(m_siSrcollBar.nMax - m_siSrcollBar.nMin+1-m_siSrcollBar.nPage);
			m_nStartPixel = rcClient.left + rcClient.Height() + m_nCurPos * m_fPerHeight + d4_5_;
		}
		else
		{
			m_bCompress = false;
		}

		m_rcBar.left = m_nStartPixel;
		m_rcBar.right = m_rcBar.left + nPageHeight - 2;
		m_rcBar.top = rcClient.top + 4;
		m_rcBar.bottom = rcClient.bottom - 4;
		m_rcBar.InflateRect(0, -1, 0, -1);

		m_rcBlank1 = rcClient;
		m_rcBlank1.left = m_rcTriUp.right;
		m_rcBlank1.right = m_rcBar.left;

		m_rcBlank2 = rcClient;
		m_rcBlank2.left = m_rcBar.right;
		m_rcBlank2.right = m_rcTriDown.left;
	}
	return true;
}

void CSimpleScrollBar::DrawBk()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	m_memBkDC.SetBkMode(TRANSPARENT);

	RECT rcWnd;
	GetWindowRect(&rcWnd);	
	m_memBkDC.FillSolidRect(&rcClient, m_crBk);
	DrawBorder();
	DrawTriangle();
}

void CSimpleScrollBar::DrawBorder()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	return;
	if (m_bCapture)
	{
		if (m_dwStyle == SCROLLBAR_VERT)
		{
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.top, rcClient.right-1, rcClient.top, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.top, rcClient.left, rcClient.top+rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.right-1, rcClient.top, rcClient.right-1, rcClient.top+rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.top+rcClient.Width(), rcClient.right, rcClient.top+rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);

			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.bottom-1, rcClient.right-1, rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.bottom-1, rcClient.left, rcClient.bottom-rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.right-1, rcClient.bottom-1, rcClient.right-1, rcClient.bottom-rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.bottom-rcClient.Width(), rcClient.right, rcClient.bottom-rcClient.Width(), PS_SOLID, 1, m_crBorder, R2_COPYPEN);
		}
		else if (m_dwStyle == SCROLLBAR_HORZ)
		{
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.top, rcClient.left + rcClient.Height(), rcClient.top, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.top, rcClient.left, rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left+rcClient.Height(), rcClient.top, rcClient.left+rcClient.Height(), rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.left, rcClient.bottom-1, rcClient.left+rcClient.Height(), rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);

			DrawLine(m_memBkDC.m_hDC, rcClient.right-1, rcClient.top, rcClient.right-rcClient.Height(), rcClient.top, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.right-1, rcClient.top, rcClient.right-1, rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.right-rcClient.Height(), rcClient.top, rcClient.right-rcClient.Height(), rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
			DrawLine(m_memBkDC.m_hDC, rcClient.right-rcClient.Height(), rcClient.bottom-1, rcClient.right, rcClient.bottom-1, PS_SOLID, 1, m_crBorder, R2_COPYPEN);
		}
	}
}

void CSimpleScrollBar::DrawTriangle()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_dwStyle == SCROLLBAR_VERT)
	{
		DrawLine(m_memBkDC.m_hDC, rcClient.left+8, rcClient.top+9, rcClient.left+10, rcClient.top+9, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+7, rcClient.top+10, rcClient.left+11, rcClient.top+10, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+6, rcClient.top+11, rcClient.left+12, rcClient.top+11, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);

		DrawLine(m_memBkDC.m_hDC, rcClient.left+8, rcClient.bottom-10, rcClient.left+10, rcClient.bottom-10, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+7, rcClient.bottom-11, rcClient.left+11, rcClient.bottom-11, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+6, rcClient.bottom-12, rcClient.left+12, rcClient.bottom-12, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
	}
	else if (m_dwStyle == SCROLLBAR_HORZ)
	{
		DrawLine(m_memBkDC.m_hDC, rcClient.left+9, rcClient.top+8, rcClient.left+9, rcClient.top+10, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+10, rcClient.top+7, rcClient.left+10, rcClient.top+11, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.left+11, rcClient.top+6, rcClient.left+11, rcClient.top+12, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);

		DrawLine(m_memBkDC.m_hDC, rcClient.right-10, rcClient.top+8, rcClient.right-10, rcClient.top+10, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.right-11, rcClient.top+7, rcClient.right-11, rcClient.top+11, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
		DrawLine(m_memBkDC.m_hDC, rcClient.right-12, rcClient.top+6, rcClient.right-12, rcClient.top+12, PS_SOLID, 1, m_crTriangle, R2_COPYPEN);
	}
}

void CSimpleScrollBar::DrawBar()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcBar;
	rcBar.SetRectEmpty();
	rcBar.right = rcBar.left + m_rcBar.Width();
	rcBar.bottom = rcBar.top + m_rcBar.Height();
	
	// RECT rcWnd(m_rcBar);
	// rcWnd(m_rcBar);
	// GetWindowRect(&rcWnd);	
	// m_memBarDC.FillSolidRect(&rcBar, m_crBk);

	if(m_bSetBarClr)
	{
		int nWidth = m_rcBar.Width();
		int nCenter = m_rcBar.Height() / 2;

		if (m_bMouseIn)
		{
			m_memBarDC.FillSolidRect(rcBar, m_crBk);
			RoundRect(m_memBarDC.m_hDC,m_crMouseInBar,rcBar,6,6);
		}
		else
		{
			m_memBarDC.FillSolidRect(rcBar, m_crBk);
			RoundRect(m_memBarDC.m_hDC,m_crBar,rcBar,6,6);
		}
	}
	else
	{
		if (m_dwStyle == SCROLLBAR_VERT)
		{
			int nWidth = m_rcBar.Width();
			int nCenter = m_rcBar.Height() / 2;

			if (m_bMouseIn)
			{
				DrawGradientRect(m_memBarDC.m_hDC, rcBar, m_crMouseInBar, m_crMouseInBar, FALSE);
			}
			else
			{
				DrawGradientRect(m_memBarDC.m_hDC, rcBar, m_crBar, m_crBar, FALSE);
			}
			DrawLine(m_memBarDC.m_hDC, rcBar.left, rcBar.top, rcBar.left, rcBar.bottom, PS_SOLID, 1, m_crBk, R2_COPYPEN);
			DrawLine(m_memBarDC.m_hDC, rcBar.right-1, rcBar.top, rcBar.right-1, rcBar.bottom, PS_SOLID, 1, m_crBk, R2_COPYPEN);

		}
		else if (m_dwStyle == SCROLLBAR_HORZ)
		{
			int nHeight = m_rcBar.Height();
			int nCenter = m_rcBar.Width() / 2;

			if (m_bMouseIn)
			{
				DrawGradientRect(m_memBarDC.m_hDC, rcBar, m_crMouseInBar, m_crMouseInBar);
			}
			else
			{
				DrawGradientRect(m_memBarDC.m_hDC, rcBar, m_crBar, m_crBar);
			}
			DrawLine(m_memBarDC.m_hDC, rcBar.left, rcBar.top, rcBar.right, rcBar.top, PS_SOLID, 1, m_crBk, R2_COPYPEN);
			DrawLine(m_memBarDC.m_hDC, rcBar.left, rcBar.bottom-1, rcBar.right, rcBar.bottom-1, PS_SOLID, 1, m_crBk, R2_COPYPEN);

		}
	}
}

void CSimpleScrollBar::SetScorllBarBkColor(COLORREF crbk)
{
	m_crBk = crbk;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}
}

void CSimpleScrollBar::SetBarBkColor(COLORREF crbk)
{
	m_crBarBk = crbk;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}	
}

void CSimpleScrollBar::SetScorllBarColor(COLORREF  crBar,bool m_bSetClr)
{
	m_crBar = crBar ;
	m_bSetBarClr = m_bSetClr ;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}
}

void CSimpleScrollBar::SetScorllBarBorderColor(COLORREF crBorder)
{
	m_crBorder = crBorder;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}	
}

void CSimpleScrollBar::SetBarBorderColor(COLORREF crBorder)
{
	m_crBarBoder = crBorder;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}
}

void CSimpleScrollBar::SetTriangleBorderColor(COLORREF crBorder)
{
	m_crTriangle = crBorder;

	if (IsWindow(m_hWnd))
	{
		ReDraw();
	}
}

BOOL CSimpleScrollBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

void CSimpleScrollBar::OnMouseLeave()
{
	if (m_bAutoShow)
	{
		SetTimer(HFSCROLLBARHIDETIME, 200, NULL);
	}

	if (m_bMouseIn)
	{
		m_bMouseIn = false;
		ReDraw();
	}

	CWnd::OnMouseLeave();
}

void CSimpleScrollBar::ChangeSchemes()
{
	//m_crBk = RGB(0xd9, 0xd9, 0xd9);           // 整个背景色 
	//m_crBorder = RGB(0xaa, 0xaa, 0xaa);
	//m_crBar = RGB(0x32, 0x85, 0xbb);          // 滚动条颜色
	//m_crMouseInBar = RGB(0xb9, 0xbd, 0xc0);   // 
	//m_crBarBk = RGB(111, 110, 110);           // ?
	//m_crBarBoder = RGB(0xb7, 0xb7, 0xb7);
	//m_crTriangle = RGB(0xa9, 0xaa, 0xab);
}

void CSimpleScrollBar::DrawLine(HDC hDC,int x1,int y1,int x2,int y2, int penStyle, int Width, COLORREF color, int nDrawMode)
{
	if(NULL != hDC)
	{
		int nOldDrawMode = SetROP2(hDC, nDrawMode);
		CPen pen;
		pen.CreatePen(penStyle, Width, color);

		HPEN hOldPen = (HPEN)::SelectObject(hDC, pen.m_hObject);

		MoveToEx(hDC, x1, y1, NULL);
		LineTo(hDC, x2, y2);

		::SelectObject(hDC, hOldPen);

		pen.DeleteObject();
		SetROP2(hDC, nOldDrawMode);
	}
}

void CSimpleScrollBar::DrawGradientRect(HDC hDC, const CRect& rect, COLORREF startColor, COLORREF endColor, BOOL bVertical/* = TRUE*/)
{
	/*if (sm_GradientFill != NULL)
	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    gRect;

		vert[0].x      = rect.left;
		vert[0].y      = rect.top;
		vert[0].Red    = GetRValue(startColor)<<8;
		vert[0].Green  = GetGValue(startColor)<<8;
		vert[0].Blue   = GetBValue(startColor)<<8;
		vert[0].Alpha  = 0x0000;

		vert[1].x      = rect.right;
		vert[1].y      = rect.bottom; 
		vert[1].Red    = GetRValue(endColor)<<8;
		vert[1].Green  = GetGValue(endColor)<<8;
		vert[1].Blue   = GetBValue(endColor)<<8;
		vert[1].Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;
		if (bVertical)
			sm_GradientFill(hDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
		else
			sm_GradientFill(hDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
	}
	else*/
	{
		const WORD wTopColorR = GetRValue(startColor);
		const WORD wTopColorG = GetGValue(startColor);
		const WORD wTopColorB = GetBValue(startColor);
		const WORD wBottomColorR = GetRValue(endColor);
		const WORD wBottomColorG = GetGValue(endColor);
		const WORD wBottomColorB = GetBValue(endColor);
		int nHeight;
		if (bVertical)
			nHeight = rect.Height();
		else
			nHeight = rect.Width();
		if (nHeight == 0)
			return;
		const double fDisR = (wBottomColorR-wTopColorR)/(float)nHeight;
		const double fDisG = (wBottomColorG-wTopColorG)/(float)nHeight;
		const double fDisB = (wBottomColorB-wTopColorB)/(float)nHeight;
		for (int i = 0; i < nHeight; i++)
		{
			WORD wR, wG, wB;
			wR = wTopColorR + (int)(fDisR*i);
			wG = wTopColorG + (int)(fDisG*i);
			wB = wTopColorB + (int)(fDisB*i);
			HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(wR, wG, wB));
			HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
			if (bVertical)
			{
				::MoveToEx(hDC, rect.left, rect.top+i, NULL);
				::LineTo(hDC, rect.right, rect.top+i);
			}
			else
			{
				::MoveToEx(hDC, rect.left+i, rect.top, NULL);
				::LineTo(hDC, rect.left+i, rect.bottom);
			}
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen);
		}
	}
}

bool CSimpleScrollBar::IsSmallFloat(double v)
{
	return v>(-ZERO_CTRL) && v<ZERO_CTRL;
}

void CSimpleScrollBar::RoundRect(HDC hDC, COLORREF color, const RECT *lpRect, 
						   int nRoundWidth/* = 10*/ 
						   , int nRoundHeight/* = 10*/)
{
	if(hDC == NULL)
		return;
	HGDIOBJ original = NULL;
	HGDIOBJ original1 = NULL;
	original = SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, color);
	original1 = SelectObject(hDC, GetStockObject(DC_PEN));
	SetDCPenColor(hDC,color);
	::RoundRect(hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, nRoundWidth, nRoundHeight);
	SelectObject(hDC, original1);
	SelectObject(hDC, original);
}