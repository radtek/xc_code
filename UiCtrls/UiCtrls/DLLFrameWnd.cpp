//BStatus.cpp : 实现文件

#include "stdafx.h"
#include "DLLFrameWnd.h"
//#include "MainFrm.h"

//CDLLFrameWnd 对话框

IMPLEMENT_DYNAMIC(CDLLFrameWnd, CWnd)

CDLLFrameWnd::CDLLFrameWnd() 
	: m_color(RGB(18, 20, 22))
{
}

CDLLFrameWnd::~CDLLFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CDLLFrameWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//CDLLFrameWnd 消息处理程序


void CDLLFrameWnd::OnPaint()
{
	CPaintDC dc(this);

	CRect rCl;
	GetClientRect(&rCl);

	CDC dcMem;
	CBitmap bmpMem;
	CBitmap* pOldBmp = NULL;

	if (dcMem.CreateCompatibleDC(&dc))
	{
		if (bmpMem.CreateCompatibleBitmap(&dc, rCl.Width(), rCl.Height()))
			pOldBmp = dcMem.SelectObject(&bmpMem);
		else
			return;
	}
	else
		return;


	dcMem.FillSolidRect(&rCl, m_color);
	dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}

BOOL CDLLFrameWnd::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext )
{
	if (!RegisterWindowClass())
	{
		return FALSE;
	}

	return CWnd::Create(lpszClassName, lpszWindowName,  dwStyle,  rect, pParentWnd,  nID, pContext);
}

BOOL CDLLFrameWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, DLLFrameWnd_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = DLLFrameWnd_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void CDLLFrameWnd::SetBkClr(COLORREF color)
{
	m_color = color;
	Invalidate();
}

BOOL CDLLFrameWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}



