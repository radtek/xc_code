#include "stdafx.h"
#include "DockLayoutWnd.h"
#include "stb_image.h"
#include "..\resource.h"
#pragma comment(lib,"Msimg32.lib")
extern "C"
{
	extern unsigned char *stbi_load_from_memory(unsigned char const *buffer, int len, int *x, int *y,int *comp, int req_comp);
	extern void     stbi_image_free(void *retval_from_stbi_load);
};

CDockLayoutWnd::CDockLayoutWnd(void):m_hWnd(NULL),m_nEmSize(ROOT_CENTER),m_hBitmapDockCenter(NULL),m_hBitmapDockLeft(NULL),
	m_hBitmapDockRight(NULL),m_hBitmapDockUp(NULL),m_hBitmapDockDown(NULL)
{
	m_rcLeft.SetRectEmpty();
	m_rcRight.SetRectEmpty();
	m_rcTop.SetRectEmpty();
	m_rcBottom.SetRectEmpty();
	m_rcCenter.SetRectEmpty();
	m_rcDrawDock.SetRectEmpty();
}


CDockLayoutWnd::~CDockLayoutWnd(void)
{
	if (m_hBitmapDockCenter)
	{
		DeleteObject(m_hBitmapDockCenter);
	}
	if (m_hBitmapDockCenter)
	{
		DeleteObject(m_hBitmapDockLeft);
	}
	if (m_hBitmapDockCenter)
	{
		DeleteObject(m_hBitmapDockRight);
	}
	if (m_hBitmapDockCenter)
	{
		DeleteObject(m_hBitmapDockUp);
	}
	if (m_hBitmapDockCenter)
	{
		DeleteObject(m_hBitmapDockDown);
	}
}

HWND CDockLayoutWnd::GetHWND() const
{
	return m_hWnd;
}

CDockLayoutWnd::operator HWND() const
{
	return m_hWnd;
}

bool CDockLayoutWnd::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CDockLayoutWnd::__WndProc;
	wc.hInstance = AfxGetResourceHandle();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClass(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LPCTSTR CDockLayoutWnd::GetWindowClassName() const
{
	return "DockLayoutWnd";
}

HWND CDockLayoutWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x /*= CW_USEDEFAULT*/, int y /*= CW_USEDEFAULT*/, int cx /*= CW_USEDEFAULT*/, int cy /*= CW_USEDEFAULT*/, HMENU hMenu /*= NULL*/)
{
	if( GetWindowClassName() != NULL && !RegisterWindowClass() ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, AfxGetResourceHandle(), this);
	ASSERT(m_hWnd!=NULL);
	m_hBitmapDockCenter = LoadImageFromResource(IDB_PNG_DOCKCENTER);
	m_hBitmapDockLeft = LoadImageFromResource(IDB_PNG_DOCKLEFT);
	m_hBitmapDockRight = LoadImageFromResource(IDB_PNG_DOCKRIGHT);
	m_hBitmapDockUp = LoadImageFromResource(IDB_PNG_DOCKUP);
	m_hBitmapDockDown = LoadImageFromResource(IDB_PNG_DOCKDOWN);
	return m_hWnd;
}

void CDockLayoutWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= true*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

LRESULT CDockLayoutWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0L*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CDockLayoutWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0L*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CDockLayoutWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDockLayoutWnd* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CDockLayoutWnd*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else {
		pThis = reinterpret_cast<CDockLayoutWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if( pThis != NULL ) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CDockLayoutWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps = {0};
			::BeginPaint(m_hWnd, &ps);
			Draw(ps.hdc);
			::EndPaint(m_hWnd, &ps);
		}	
		return 0;
	default:
		break;
	}
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void CDockLayoutWnd::Draw(HDC hdc)
{
	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if((dwExStyle&WS_EX_LAYERED) != WS_EX_LAYERED)
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle|WS_EX_LAYERED);
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	if (hMemDC == NULL)
	{
		return;
	}
	CRect rcClient;
	::GetWindowRect(m_hWnd,rcClient);
	void *pBits;
	BITMAPINFO info;
	memset(&info.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biWidth = rcClient.Width();
	info.bmiHeader.biHeight = rcClient.Height();
	HBITMAP hMemBitMap = CreateDIBSection(hdc, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (hMemBitMap == NULL)
	{
		DeleteObject(hMemDC);
		return;
	}
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC,hMemBitMap);
	if (!m_rcLeft.IsRectEmpty())
	{
		DrawImage(hMemDC,m_rcLeft,m_hBitmapDockLeft);
	}
	if (!m_rcRight.IsRectEmpty())
	{
		DrawImage(hMemDC,m_rcRight,m_hBitmapDockRight);
	}
	if (!m_rcTop.IsRectEmpty())
	{
		DrawImage(hMemDC,m_rcTop,m_hBitmapDockUp);
	}
	if (!m_rcBottom.IsRectEmpty())
	{
		DrawImage(hMemDC,m_rcBottom,m_hBitmapDockDown);
	}
	if (!m_rcCenter.IsRectEmpty())
	{
		DrawImage(hMemDC,m_rcCenter,m_hBitmapDockCenter);
	}
	if (!m_rcDrawDock.IsRectEmpty())
	{
		DrawAlpha(hMemDC,m_rcDrawDock,RGB(23, 88, 179),255 * 0.5);
	}
	RECT rcWnd = {0};
	::GetWindowRect(m_hWnd, &rcWnd);
	POINT pt = {rcWnd.left, rcWnd.top};
	SIZE szWindow = {rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top};
	POINT ptSrc = {0, 0};
	BLENDFUNCTION blendPixelFunction = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
	::UpdateLayeredWindow(m_hWnd, NULL, &pt, &szWindow, hMemDC, 
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

	::SelectObject(hMemDC, oldBitmap);
	DeleteObject(hMemBitMap);
	DeleteObject(hMemDC);
}

void CDockLayoutWnd::ResizeDockButton(LPPOINT pt,LPRECT rcDockPage)
{
	m_nEmSize = ROOT_CENTER;
	m_rcLeft.SetRectEmpty();
	m_rcRight.SetRectEmpty();
	m_rcTop.SetRectEmpty();
	m_rcBottom.SetRectEmpty();
	m_rcCenter.SetRectEmpty();
	m_rcDrawDock.SetRectEmpty();
	CRect rcClient;
	GetClientRect(m_hWnd,rcClient);
	if (rcClient.Width() < 114 || rcClient.Height() < 114)
	{
		InvalidateRect(m_hWnd,NULL,FALSE);
		return;
	}
	CPoint centerPoint = rcClient.CenterPoint(); 
	m_rcLeft.left = rcClient.left;
	m_rcLeft.right = m_rcLeft.left + 24;
	m_rcLeft.top = centerPoint.y - 20;
	m_rcLeft.bottom = centerPoint.y + 20;
	m_rcRight = m_rcLeft;
	m_rcRight.right = rcClient.right;
	m_rcRight.left = m_rcRight.right - 24;
	m_rcTop.top = rcClient.top;
	m_rcTop.bottom = m_rcTop.top + 24;
	m_rcTop.left = centerPoint.x - 20;
	m_rcTop.right = centerPoint.x + 20;
	m_rcBottom = m_rcTop;
	m_rcBottom.bottom = rcClient.bottom;
	m_rcBottom.top = m_rcBottom.bottom - 24;
	if (IsRectEmpty(rcDockPage))
	{
		m_rcCenter.left = centerPoint.x - 57;
		m_rcCenter.right = centerPoint.x + 57;
		m_rcCenter.top = centerPoint.y - 57;
		m_rcCenter.bottom = centerPoint.y + 57;
	}
	else
	{
		CRect rcTmep = rcDockPage;
		//::ScreenToClient(m_hWnd,(LPPOINT)&rcTmep);
		CPoint centerTempPoint = rcTmep.CenterPoint();
		m_rcCenter.left = centerTempPoint.x - 57;
		m_rcCenter.right = centerTempPoint.x + 57;
		m_rcCenter.top = centerTempPoint.y - 57;
		m_rcCenter.bottom = centerTempPoint.y + 57;
		if (m_rcCenter.left < rcClient.left)
		{
			m_rcCenter.left = rcClient.left;
			m_rcCenter.right = m_rcCenter.left + 114;
		}
		if (m_rcCenter.right > rcClient.right)
		{
			m_rcCenter.right = rcClient.right;
			m_rcCenter.left = m_rcCenter.right - 114;
		}
		if (m_rcCenter.top < rcClient.top)
		{
			m_rcCenter.top = rcClient.top;
			m_rcCenter.bottom = m_rcCenter.top + 114;
		}
		if (m_rcCenter.bottom > rcClient.bottom)
		{
			m_rcCenter.bottom = rcClient.bottom;
			m_rcCenter.top = m_rcCenter.bottom - 114;
		}
	}
	if (m_rcLeft.PtInRect(*pt))
	{
		 m_rcDrawDock = rcClient;
		 m_rcDrawDock.right = m_rcDrawDock.left + rcClient.Width() / 2;
		 m_nEmSize = ROOT_LEFT;
	}
	else if (m_rcRight.PtInRect(*pt))
	{
		m_rcDrawDock = rcClient;
		m_rcDrawDock.left = m_rcDrawDock.right - rcClient.Width() / 2;
		m_nEmSize = ROOT_RIGHT;
	}
	else if (m_rcTop.PtInRect(*pt))
	{
		m_rcDrawDock = rcClient;
		m_rcDrawDock.bottom = m_rcDrawDock.top + rcClient.Height() / 2;
		m_nEmSize = ROOT_TOP;
	}
	else if (m_rcBottom.PtInRect(*pt))
	{
		m_rcDrawDock = rcClient;
		m_rcDrawDock.top = m_rcDrawDock.bottom - rcClient.Height() / 2;
		m_nEmSize = ROOT_BOTTOM;
	}
	else if (m_rcCenter.PtInRect(*pt))
	{
		CRect rcLeft, rcRight, rcTop, rcBottom, rcCenter;
		CPoint centerTempPoint = m_rcCenter.CenterPoint();
		rcCenter.left = centerTempPoint.x - 44; 
		rcCenter.right = centerTempPoint.x + 44;
		rcCenter.top = centerTempPoint.y - 44; 
		rcCenter.bottom = centerTempPoint.y + 44;
		rcLeft = rcCenter;
		rcLeft.left = m_rcCenter.left;
		rcLeft.right = rcLeft.left + 35;
		rcRight = rcClient;
		rcRight.right = m_rcCenter.right;
		rcRight.left = rcRight.right - 35;
		rcTop = rcCenter;
		rcTop.top = m_rcCenter.top;
		rcTop.bottom = rcTop.top + 35;
		rcBottom = rcCenter;
		rcBottom.bottom = m_rcCenter.bottom;
		rcBottom.top = rcBottom.bottom - 35;
		CRect rcTemp = rcClient;
		if (!IsRectEmpty(rcDockPage))
		{
			rcTemp = rcDockPage;
		}
		if (rcLeft.PtInRect(*pt))
		{
			m_rcDrawDock = rcTemp;
			m_rcDrawDock.right = m_rcDrawDock.left + rcTemp.Width() / 2;
			if (rcTemp.Width() <= 200)
			{
				m_rcDrawDock.SetRectEmpty();
			}
			m_nEmSize = LEFT;
		}
		else if (rcRight.PtInRect(*pt))
		{
			m_rcDrawDock = rcTemp;
			m_rcDrawDock.left = m_rcDrawDock.right - rcTemp.Width() / 2;
			if (rcTemp.Width() <= 200)
			{
				m_rcDrawDock.SetRectEmpty();
			}
			m_nEmSize = RIGHT;
		}
		else if (rcTop.PtInRect(*pt))
		{
			m_rcDrawDock = rcTemp;
			m_rcDrawDock.bottom = m_rcDrawDock.top + rcTemp.Height() / 2;
			if (rcTemp.Height() <= 200)
			{
				m_rcDrawDock.SetRectEmpty();
			}
			m_nEmSize =TOP;
		}
		else if (rcBottom.PtInRect(*pt))
		{
			m_rcDrawDock = rcTemp;
			m_rcDrawDock.top = m_rcDrawDock.bottom - rcTemp.Height() / 2;
			if (rcTemp.Height() <= 200)
			{
				m_rcDrawDock.SetRectEmpty();
			}
			m_nEmSize = BOTTOM;
		}
		else if (rcCenter.PtInRect(*pt))
		{
			 m_rcDrawDock = rcTemp;
			 m_nEmSize = CENTER;
		}
	}
	InvalidateRect(m_hWnd,NULL,FALSE);
}

LPRECT CDockLayoutWnd::GetDockRect()
{
	return m_rcDrawDock;
}

CDockLayoutWnd::LAYOUT_DIR CDockLayoutWnd::GetLayoutDir()
{
	return m_nEmSize;
}

HBITMAP CDockLayoutWnd::LoadImageFromResource(UINT nId)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;   
 	//HINSTANCE hInst = GetCurrentModule();
	//HINSTANCE hInst = CTradeFunc::s_hInstMain;
	//HINSTANCE hInst = AfxGetInstanceHandle();
	HINSTANCE hInst = NULL;
	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(nId), _T("PNG"));
	if( hResource == NULL )
		return NULL;
	HGLOBAL hGlobal = ::LoadResource(hInst, hResource);
	if( hGlobal == NULL ) {
		FreeResource(hResource);
		return NULL;;
	}

	dwSize = ::SizeofResource(hInst, hResource);
	if( dwSize == 0 )
		return NULL;
	pData = new BYTE[ dwSize ];
	::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
	::FreeResource(hResource);
	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	pData = NULL;
	if( !pImage ) 
	{
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if( !hBitmap ) {
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
		}
	}

	for( int i = 0; i < x * y; i++ ) 
	{
		pDest[i*4 + 3] = pImage[i*4 + 3];
		if( pDest[i*4 + 3] < 255 )
		{
			pDest[i*4] = (BYTE)(DWORD(pImage[i*4 + 2])*pImage[i*4 + 3]/255);
			pDest[i*4 + 1] = (BYTE)(DWORD(pImage[i*4 + 1])*pImage[i*4 + 3]/255);
			pDest[i*4 + 2] = (BYTE)(DWORD(pImage[i*4])*pImage[i*4 + 3]/255); 
			bAlphaChannel = true;
		}
		else
		{
			pDest[i*4] = pImage[i*4 + 2];
			pDest[i*4 + 1] = pImage[i*4 + 1];
			pDest[i*4 + 2] = pImage[i*4]; 
		}

		if( *(DWORD*)(&pDest[i*4]) == 0 ) {
			pDest[i*4] = (BYTE)0;
			pDest[i*4 + 1] = (BYTE)0;
			pDest[i*4 + 2] = (BYTE)0; 
			pDest[i*4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	return hBitmap;
}

void CDockLayoutWnd::DrawAlpha(HDC hDC, const CRect& rect, const COLORREF clrMask, const int alp)
{
	CRect rc(0, 0, 0, 0);
	rc.right = rect.right-rect.left;
	rc.bottom = rect.bottom-rect.top;

	HDC hMaskDC;
	HBITMAP hMaskBmp;
	hMaskDC = ::CreateCompatibleDC(hDC);
	hMaskBmp = CreateCompatibleBitmap(hDC, rc.Width(), rc.Height());
	HBITMAP hOldBmp = (HBITMAP)(SelectObject(hMaskDC, hMaskBmp));
	HBRUSH hBrush = ::CreateSolidBrush(clrMask);
	::FillRect(hMaskDC,rc,hBrush);
	DeleteObject(hBrush);
	BLENDFUNCTION bm;
	bm.BlendOp = AC_SRC_OVER;
	bm.BlendFlags = 0;
	bm.SourceConstantAlpha = alp;
	bm.AlphaFormat = 0; 

	AlphaBlend(hDC, rect.left, rect.top, rc.Width(), rc.Height(), hMaskDC, 0, 0, rc.Width(), rc.Height(), bm);

	SelectObject(hMaskDC, hOldBmp);
	::DeleteObject(hMaskBmp);
	DeleteDC(hMaskDC);

}

void CDockLayoutWnd::DrawImage(HDC hDC, const CRect& rect,HBITMAP hBitMap)
{
	HDC hMaskDC;
	hMaskDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)(SelectObject(hMaskDC, hBitMap));
	::StretchBlt(hDC, rect.left, rect.top, rect.Width(), rect.Height(), hMaskDC,
		0, 0, rect.Width(), rect.Height(), SRCCOPY);
	SelectObject(hMaskDC, hOldBmp);
	DeleteDC(hMaskDC);
}

HMODULE CDockLayoutWnd::GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)
	// Here's a trick that will get you the handle of the module
	// you're running in without any a-priori knowledge:
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof(mbi) );

	return reinterpret_cast<HMODULE>(mbi.AllocationBase);
#else  
	// from ATL 7.0 sources
	return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}  
