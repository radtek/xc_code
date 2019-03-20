
//*****************************************************
//********    SimpleStatic By YaoShuLong 2017.2
//*****************************************************

// SimpleEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SimpleStatic.h"


// CSimpleStatic

HFONT CSimpleStatic::m_hFont = NULL;


IMPLEMENT_DYNAMIC(CSimpleStatic, CWnd)

CSimpleStatic::CSimpleStatic()
{  
    m_clrText = RGB(215, 215, 215);
	m_clrTextDisabled = RGB(100, 100, 100);
    m_clrBk = RGB(18, 20, 22);
	m_bEnable = TRUE;
	m_bLMouseDown = FALSE;
	m_bEnableClick = FALSE;
	//m_font.CreateFont(-12,0,0,0,
 //       FW_BOLD,
 //       FALSE,FALSE,
 //       FALSE, 
 //       DEFAULT_CHARSET,
 //       OUT_DEFAULT_PRECIS,
 //       CLIP_DEFAULT_PRECIS,
 //       PROOF_QUALITY,
 //       DEFAULT_PITCH, 
 //       _T("微软雅黑"));
	//m_hFont = (HFONT)m_font.GetSafeHandle();
}

CSimpleStatic::~CSimpleStatic()
{
}


BEGIN_MESSAGE_MAP(CSimpleStatic, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CSimpleStatic 消息处理程序

void CSimpleStatic::OnPaint()  
{  
    CPaintDC dc(this);
    UINT nStyple = DT_SINGLELINE|DT_NOPREFIX;
    if(GetStyle() & SS_LEFT)
    {
        nStyple = DT_LEFT | nStyple ;
    }
    else if (GetStyle() & SS_RIGHT)
    {
        nStyple = DT_RIGHT | nStyple ;
    }
    else if (GetStyle() & SS_CENTER)
    {
        nStyple = DT_CENTER | nStyple ;
    }

    /*if(GetStyle() & SS_CENTERIMAGE)
    {
        nStyple = DT_VCENTER | nStyple ;
    }*/
	nStyple = DT_VCENTER | nStyple;

    CRect rect;
    GetClientRect(&rect);
    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height()))
            pOldBmp = dcMem.SelectObject(&bmpMem);
        else
            return;
    }
    else
        return;


	CString strCaption;
	CWnd::GetWindowText(strCaption);
    dcMem.SetBkMode(TRANSPARENT);  
    dcMem.FillSolidRect(rect, m_clrBk);  
  
    CFont* pFontOld = dcMem.SelectObject(CFont::FromHandle(m_hFont));

    COLORREF clrBkTextOld = dcMem.SetTextColor(m_bEnable ? m_clrText : m_clrTextDisabled);
    dcMem.DrawText(strCaption, &rect, nStyple);

    dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, rect.left, rect.top, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.SelectObject(pFontOld);
    dcMem.DeleteDC();
	bmpMem.DeleteObject();
}  

void CSimpleStatic::SetBkColor(COLORREF color) 
{
    m_clrBk = color;
    Invalidate(FALSE);
}

void CSimpleStatic::SetTextColor(COLORREF color) 
{
    m_clrText = color;
    Invalidate(FALSE);
}


BOOL CSimpleStatic::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


int CSimpleStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    //CWnd::SetFont(&m_fond);
    return 0;
}

BOOL CSimpleStatic::Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	if (!RegisterWindowClass())
	{
		return FALSE;
	}

	return CWnd::Create(SimpleStatic_CLASSNAME, strCaption, dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleStatic::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, SimpleStatic_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = SimpleStatic_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void CSimpleStatic::SetWindowText(CString str)
{
	CWnd::SetWindowText(str);
	Invalidate(FALSE);
}

void CSimpleStatic::SetFont(HFONT hFont)
{
	m_hFont = hFont;
}

void CSimpleStatic::EnableWindow(BOOL bEnable/* = TRUE*/)
{
	m_bEnable = bEnable;
	Invalidate(FALSE);
}

void CSimpleStatic::SetTextDisabledColor(COLORREF color)
{
	m_clrTextDisabled = color;
	Invalidate(FALSE);
}

void CSimpleStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLMouseDown = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bLMouseDown && m_bEnableClick)
	{
		NMHDR hdr;
		hdr.idFrom = GetDlgCtrlID();
		hdr.hwndFrom = GetSafeHwnd();
		hdr.code = NM_CLICK;
		LRESULT nResult = GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
		m_bLMouseDown = FALSE;
		Invalidate(FALSE);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CSimpleStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bEnableClick)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		return TRUE;
	}
	else
	{
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}
}

void CSimpleStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bLMouseDown = TRUE;
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleStatic::EnableClick(BOOL bEnable/* = TRUE*/)
{
	m_bEnableClick = bEnable;
}
