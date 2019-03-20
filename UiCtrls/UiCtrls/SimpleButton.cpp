
//*****************************************************
//********    SimpleButton By YaoShuLong 2017.1
//*****************************************************

#include "stdafx.h"
#include "SimpleButton.h"

#define TIMER_ID_HOLD  	   1
#define TIMER_ID_GAP 	  	2

#define TIME_CLAPSE_HOLD  500
#define TIME_CLAPSE_GAP     50

// CSimpleButton

HFONT CSimpleButton::m_hFont = NULL;


IMPLEMENT_DYNAMIC(CSimpleButton, CWnd)

CSimpleButton::CSimpleButton() :
m_bMouseEnter(FALSE),
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE),
m_bLMouseDBDown(FALSE),
m_strCaption(_T(""))
{
    m_bGetPixel = TRUE;
    //RegisterWindowClass();
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
    //m_hFont = (HFONT)m_font.GetSafeHandle();
    m_hFont0 = (HFONT)m_font.GetSafeHandle();
    m_hFont1 = (HFONT)m_font.GetSafeHandle();
    m_bEnableLeftTopRound = TRUE;
    m_bEnableLeftBottomRound = TRUE;
    m_bEnableRightBottomRound = TRUE;
    m_bEnableRightTopRound = TRUE;
    m_nTextPosition = 0;
    m_bEnableHoverLeaveNotify = FALSE;

    m_clrBkBlue = RGB(61, 91, 153);
    //m_clrBkBlueHover = RGB(102, 182, 240);
	m_clrBkBlueHover = RGB(91, 123, 189);
    m_clrBkBlueDisabled = RGB(30, 45, 76);

    m_clrBkGray = RGB(66, 66, 66);
    m_clrBkGrayHover = RGB(105, 105, 105);

    m_clrBkRed = RGB(153, 61, 61);
    m_clrBkRedHover = RGB(228, 106, 94);
    m_clrBkRedDisabled = RGB(76, 30, 30);

    m_clrBkGreen = RGB(87, 153, 61);
    m_clrBkGreenHover = RGB(113, 200, 85);
    m_clrBkGreenDisabled = RGB(43, 76, 30);

    m_clrBk = 0;

    m_clrBkHover = 0;

    m_clrBkDisabled = 0;

    m_clrText = RGB(255, 255, 255);
    m_clrTextDisabled = RGB(189, 189, 189);
	m_clrFocusOutline = RGB(215, 215, 215);
	m_clrToolTipBkClr = RGB(37, 39, 53);
	m_clrToolTipTextClr = RGB(215, 215, 215);

    m_nBtnColor = 0;   
    m_bEnable = TRUE;
    m_bEnableClickToGetFocus = FALSE;
    m_bChangeCursor = FALSE;

    m_clrLeftTop1 = 0;
    m_clrLeftTop21 = 0;
    m_clrLeftTop22 = 0;
    m_clrLeftTop31 = 0;
    m_clrLeftTop32 = 0;

    m_clrLeftBottom1 = 0;
    m_clrLeftBottom21 = 0;
    m_clrLeftBottom22 = 0;
    m_clrLeftBottom31 = 0;
    m_clrLeftBottom32 = 0;

    m_clrRightBottom1 = 0;
    m_clrRightBottom21 = 0;
    m_clrRightBottom22 = 0;
    m_clrRightBottom31 = 0;
    m_clrRightBottom32 = 0;

    m_clrRightTop1 = 0;
    m_clrRightTop21 = 0;
    m_clrRightTop22 = 0;
    m_clrRightTop31 = 0;
    m_clrRightTop32 = 0;
    m_bEnableMultiBkColor = FALSE;
	m_btnStyle = NormalStyle;
	m_bEnableTip = FALSE;
	m_bEnableTab = FALSE;
	m_bSelected = FALSE;
	m_clrBkUnselected = RGB(18, 20, 22);
}

CSimpleButton::~CSimpleButton()
{
}

BEGIN_MESSAGE_MAP(CSimpleButton, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEHOVER()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSimpleButton 消息处理程序

void CSimpleButton::OnPaint()
{
	Paint();
}

void CSimpleButton::Paint()
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
		if (m_bSelected)
		{
			DrawButton(rCl, dcMem, m_clrBk, RGB(18, 20, 22), GetTextColor());
		}
		else
		{
			if (m_bMouseIn)
			{
				if (m_bLMouseDown)
				{
					DrawButton(rCl, dcMem, m_clrBk, RGB(18, 20, 22), GetTextColor());
				}
				else
				{
					DrawButton(rCl, dcMem, m_clrBkHover, RGB(18, 20, 22), GetTextColor());
				}
			}
			else
			{
				DrawButton(rCl, dcMem, m_bEnableTab ? m_clrBkUnselected : m_clrBk, RGB(18, 20, 22), GetTextColor());
			}
		}		
	}
	else
	{
		DrawButton(rCl, dcMem, m_clrBkDisabled, RGB(18, 20, 22), m_clrTextDisabled);
	}

	dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}

void CSimpleButton::DrawButton(CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrParentBk, COLORREF clrText)
{
	dcMem.FillSolidRect(&rect, clrButton);
	RoundRect(rect, dcMem, clrButton, clrParentBk, m_bEnableLeftTopRound, m_bEnableLeftBottomRound, m_bEnableRightBottomRound, m_bEnableRightTopRound);
	Draw(rect, dcMem, GetCaption(), GetHfont(), m_hFont0, m_hFont1, clrButton, clrText);
}

BOOL CSimpleButton::Create(UINT dwStyle, UINT dwBtnColor, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption, UINT nTextPosition, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound)
{
	if (!RegisterWindowClass())
	{
		return FALSE;
	}
    m_strCaption = strCaption;
    m_bEnableLeftTopRound = bEnableLeftTopRound;
    m_bEnableLeftBottomRound = bEnableLeftBottomRound;
    m_bEnableRightBottomRound = bEnableRightBottomRound;
    m_bEnableRightTopRound = bEnableRightTopRound;
    m_nTextPosition = nTextPosition;

    switch (dwBtnColor)
    {
    case BUTTON_BLUE:
        {
            m_clrBk = m_clrBkBlue;
            m_clrBkHover = m_clrBkBlueHover;
            m_clrBkDisabled = m_clrBkBlueDisabled;
            break;
        }
    case BUTTON_GRAY:
        {
            m_clrBk = m_clrBkGray;
            m_clrBkHover = m_clrBkGrayHover;
            m_clrBkDisabled = m_clrBkGray;
            break;
        }
    case BUTTON_RED:
        {
            m_clrBk = m_clrBkRed;
            m_clrBkHover = m_clrBkRedHover;
            m_clrBkDisabled = m_clrBkRedDisabled;
            break;
        }
    case BUTTON_GREEN:
        {
            m_clrBk = m_clrBkGreen;
            m_clrBkHover = m_clrBkGreenHover;
            m_clrBkDisabled = m_clrBkGreenDisabled;
            break;
        }

    default:
        break;
    }

    return CWnd::Create(SimpleButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleButton::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleButton_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleButton_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!IsGetFocus() && m_bEnableClickToGetFocus)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_SETFOCUS;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        SetFocus();
    }

	if (!m_bSelected)
	{
		m_bLMouseDown = TRUE;
		if (IsNeedTimer())
		{
			SetTimer(TIMER_ID_HOLD, TIME_CLAPSE_HOLD, NULL);
		}
		Invalidate(FALSE);
	}
    
    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsNeedTimer())
	{
		KillTimer(TIMER_ID_GAP);
	}
	
	if (m_bLMouseDown && !m_bSelected)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
		hdr.code = NM_CLICK;
		LRESULT nResult = GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);	
        m_bLMouseDown = FALSE;
		m_bLMouseDBDown = FALSE;
		if (m_bEnableTab && 0 != nResult)
		{
			m_bSelected = TRUE;
		}
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleButton::OnMouseLeave()
{
    m_bMouseEnter = FALSE;
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    if (!m_bLMouseDown)
    {
        Invalidate(FALSE);
    }

    if (m_bEnableHoverLeaveNotify)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_RCLICK;//用NM_RCLICK代替leave消息
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);        
    }   
    CWnd::OnMouseLeave();
}


void CSimpleButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseIn)
    {
		if (!m_bSelected)
		{
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof (csTME);
			csTME.dwFlags = TME_LEAVE | TME_HOVER;
			csTME.hwndTrack = GetSafeHwnd(); // 指定要 追踪 的窗口     
			csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER   
			::_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE 
			m_bMouseIn = TRUE;
			Invalidate(FALSE);
		}         
    }
	if (IsWindow(m_toolTip.GetSafeHwnd()) && m_bEnableTip)
	{
		m_toolTip.AddTool(this, m_strTipText);
	}
    CWnd::OnMouseMove(nFlags, point);
}

void CSimpleButton::Draw(CRect& rect, CDC& dcMem, CString& strCaption, HFONT hFont, HFONT hFont0, HFONT hFont1, COLORREF clrTextBk, COLORREF clrText)
{
	////文字
	if (NormalStyle == m_btnStyle || LeftStyle == m_btnStyle || RightStyle == m_btnStyle)
	{
		CFont* pFontOld = dcMem.SelectObject(CFont::FromHandle(hFont));
		COLORREF clrBkOld = dcMem.SetBkColor(clrTextBk);
		COLORREF clrBkTextOld = dcMem.SetTextColor(clrText);

		if (strCaption.IsEmpty() && m_vecTextInfo.size() > 1)
		{
			pFontOld = dcMem.SelectObject(CFont::FromHandle(hFont0));
			CRect rectW;
			rectW.SetRect(m_vecTextInfo[0].x, m_vecTextInfo[0].y, m_vecTextInfo[0].width + m_vecTextInfo[0].x, m_vecTextInfo[0].height + m_vecTextInfo[0].y);
			dcMem.DrawText(m_vecTextInfo[0].str, &rectW, m_nTextPosition | DT_VCENTER | DT_SINGLELINE);

			pFontOld = dcMem.SelectObject(CFont::FromHandle(hFont1));
			rectW.SetRect(m_vecTextInfo[1].x, m_vecTextInfo[1].y, m_vecTextInfo[1].width + m_vecTextInfo[1].x, m_vecTextInfo[1].height + m_vecTextInfo[1].y);
			dcMem.DrawText(m_vecTextInfo[1].str, &rectW, m_nTextPosition | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			dcMem.DrawText(strCaption, &rect, m_nTextPosition | DT_VCENTER | DT_SINGLELINE);
		}
		dcMem.SelectObject(pFontOld);
		dcMem.SetBkColor(clrBkOld);
		dcMem.SetTextColor(clrBkTextOld);
	}
	else if (UpStyle == m_btnStyle)	   ////向上三角形
	{
		CPen pen(PS_SOLID, 1, RGB(106, 106, 106));
		CPen* pOldPen = dcMem.SelectObject(&pen);
		CPoint ptsLine[3];
		ptsLine[0] = CPoint(rect.left, rect.top);
		ptsLine[1] = CPoint(rect.left, rect.bottom - 1);
		ptsLine[2] = CPoint(rect.right, rect.bottom - 1);
		dcMem.MoveTo(ptsLine[0]);
		dcMem.LineTo(ptsLine[1]);
		dcMem.LineTo(ptsLine[2]);

		CRgn rgn;
		rgn.DeleteObject();
		CPoint pts[3];
		pts[0] = CPoint(4, 8);
		pts[1] = CPoint(8, 3);
		pts[2] = CPoint(12, 7);
		rgn.CreatePolygonRgn(pts, 3, WINDING);
		CBrush brush(GetTriangleColor());
		dcMem.FillRgn(&rgn, &brush);
		dcMem.SelectObject(pOldPen);
	}
	else if (DownStyle == m_btnStyle)	     ////向下三角形
	{
		CPen pen(PS_SOLID, 1, RGB(106, 106, 106));
		CPen* pOldPen = dcMem.SelectObject(&pen);
		CPoint ptsLine[2];
		ptsLine[0] = CPoint(rect.left, rect.top);
		ptsLine[1] = CPoint(rect.left, rect.bottom);
		dcMem.MoveTo(ptsLine[0]);
		dcMem.LineTo(ptsLine[1]);

		CRgn rgn;
		rgn.DeleteObject();
		CPoint pts[3];
		pts[0] = CPoint(5, 4);
		pts[1] = CPoint(8, 8);
		pts[2] = CPoint(12, 4);
		rgn.CreatePolygonRgn(pts, 3, WINDING);
		CBrush brush(GetTriangleColor());
		dcMem.FillRgn(&rgn, &brush);
		dcMem.SelectObject(pOldPen);
	}

	if (IsGetFocus())	  ////获取焦点的虚线
    {
		CPen pen(PS_SOLID, 1, m_clrFocusOutline);
		CPen* pOldPen = dcMem.SelectObject(&pen);
        CPoint ptsLine[2];

        int nY = rect.top + 2;
        while (nY < rect.bottom - 3)
        {
            ptsLine[0] = CPoint(rect.left + 2, nY);
            ptsLine[1] = CPoint(rect.left + 2, nY + 1);
			dcMem.MoveTo(ptsLine[0]);
			dcMem.LineTo(ptsLine[1]);
            nY += 2;
        }

        nY = rect.top + 2;
        while (nY < rect.bottom - 3)
        {
            ptsLine[0] = CPoint(rect.right - 3, nY);
            ptsLine[1] = CPoint(rect.right - 3, nY + 1);
			dcMem.MoveTo(ptsLine[0]);
			dcMem.LineTo(ptsLine[1]);
            nY += 2;
        }

        int nX = rect.left + 2;
        while (nX < rect.right - 3)
        {
            ptsLine[0] = CPoint(nX, rect.top + 2);
            ptsLine[1] = CPoint(nX + 1, rect.top + 2);
			dcMem.MoveTo(ptsLine[0]);
			dcMem.LineTo(ptsLine[1]);
            nX += 2;
        }

        nX = rect.left + 2;
        while (nX < rect.right - 3)
        {
            ptsLine[0] = CPoint(nX, rect.bottom - 3);
            ptsLine[1] = CPoint(nX + 1, rect.bottom - 3);
			dcMem.MoveTo(ptsLine[0]);
			dcMem.LineTo(ptsLine[1]);
            nX += 2;
        }

		dcMem.SelectObject(pOldPen);
    }   
}

void CSimpleButton::SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height)
{
    TextInfo textInfo;
    textInfo.nIndex = nIndex;
    textInfo.str = strCaption;
    textInfo.x = x;
    textInfo.y = y;
    textInfo.width = width;
    textInfo.height = height;

    m_vecTextInfo.push_back(textInfo);
}

COLORREF CSimpleButton::GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai)
{
    int rButton = GetRValue(clrButton);
    int gButton = GetGValue(clrButton);
    int bButton = GetBValue(clrButton);

    int rBk = GetRValue(clrBk);
    int gBk = GetGValue(clrBk);
    int bBk = GetBValue(clrBk);

    COLORREF clrFixed = RGB(rButton * ai + rBk * (1 - ai), gButton * ai + gBk * (1 - ai), bButton * ai + bBk * (1 - ai));
    return clrFixed;
}

void CSimpleButton::RoundSingleColorBk(const CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrBk, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound)
{
	COLORREF clr_1 = GetFixedColor(clrButton, clrBk, 0.1);
	COLORREF clr_2 = GetFixedColor(clrButton, clrBk, 0.5);
	COLORREF clr_3 = GetFixedColor(clrButton, clrBk, 0.9);
	
	if (bEnableLeftTopRound)
	{
		dcMem.SetPixel(rect.left, rect.top, clr_1);

		dcMem.SetPixel(rect.left, rect.top + 1, clr_2);
		dcMem.SetPixel(rect.left + 1, rect.top, clr_2);

		dcMem.SetPixel(rect.left, rect.top + 2, clr_3);
		dcMem.SetPixel(rect.left + 2, rect.top, clr_3);
	}	

	if (bEnableLeftBottomRound)
	{
		dcMem.SetPixel(rect.left, rect.bottom - 1, clr_1);

		dcMem.SetPixel(rect.left + 1, rect.bottom - 1, clr_2);
		dcMem.SetPixel(rect.left, rect.bottom - 2, clr_2);

		dcMem.SetPixel(rect.left + 2, rect.bottom - 1, clr_3);
		dcMem.SetPixel(rect.left, rect.bottom - 3, clr_3);
	}

	if (bEnableRightBottomRound)
	{
		dcMem.SetPixel(rect.right - 1, rect.bottom - 1, clr_1);

		dcMem.SetPixel(rect.right - 2, rect.bottom - 1, clr_2);
		dcMem.SetPixel(rect.right - 1, rect.bottom - 2, clr_2);

		dcMem.SetPixel(rect.right - 3, rect.bottom - 1, clr_3);
		dcMem.SetPixel(rect.right - 1, rect.bottom - 3, clr_3);
	}

	if (bEnableRightTopRound)
	{
		dcMem.SetPixel(rect.right - 1, rect.top, clr_1);

		dcMem.SetPixel(rect.right - 2, rect.top, clr_2);
		dcMem.SetPixel(rect.right - 1, rect.top + 1, clr_2);

		dcMem.SetPixel(rect.right - 3, rect.top, clr_3);
		dcMem.SetPixel(rect.right - 1, rect.top + 2, clr_3);
	}
}

void CSimpleButton::RoundMultiColorBk(const CRect& rect, CDC& dcMem, COLORREF clrButton, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound)
{
	if (m_bGetPixel)
	{
		CRect rectEx;
		GetWindowRect(&rectEx);
		GetParent()->ScreenToClient(&rectEx);

		CDC* pDCParent = GetParent()->GetDC();
		if (pDCParent != nullptr)
		{
			if (bEnableLeftTopRound)
			{
				m_clrLeftTop1 = pDCParent->GetPixel(rectEx.left, rectEx.top);
				m_clrLeftTop21 = pDCParent->GetPixel(rectEx.left + 1, rectEx.top);
				m_clrLeftTop22 = pDCParent->GetPixel(rectEx.left, rectEx.top + 1);
				m_clrLeftTop31 = pDCParent->GetPixel(rectEx.left + 2, rectEx.top);
				m_clrLeftTop32 = pDCParent->GetPixel(rectEx.left, rectEx.top + 2);
			}
			
			if (bEnableLeftBottomRound)
			{
				m_clrLeftBottom1 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 1);
				m_clrLeftBottom21 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 2);
				m_clrLeftBottom22 = pDCParent->GetPixel(rectEx.left + 1, rectEx.bottom - 1);
				m_clrLeftBottom31 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 3);
				m_clrLeftBottom32 = pDCParent->GetPixel(rectEx.left + 2, rectEx.bottom - 1);
			}
			
			if (bEnableRightBottomRound)
			{
				m_clrRightBottom1 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 1);
				m_clrRightBottom21 = pDCParent->GetPixel(rectEx.right - 2, rectEx.bottom - 1);
				m_clrRightBottom22 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 2);
				m_clrRightBottom31 = pDCParent->GetPixel(rectEx.right - 3, rectEx.bottom - 1);
				m_clrRightBottom32 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 3);
			}
			
			if (bEnableRightTopRound)
			{
				m_clrRightTop1 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top);
				m_clrRightTop21 = pDCParent->GetPixel(rectEx.right - 2, rectEx.top);
				m_clrRightTop22 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top + 1);
				m_clrRightTop31 = pDCParent->GetPixel(rectEx.right - 3, rectEx.top);
				m_clrRightTop32 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top + 2);
			}		
		}
		GetParent()->ReleaseDC(pDCParent);
		m_bGetPixel = FALSE;
	}

	if (bEnableLeftTopRound)
	{
		dcMem.SetPixel(rect.left, rect.top, GetFixedColor(clrButton, m_clrLeftTop1, 0.1));

		dcMem.SetPixel(rect.left + 1, rect.top, GetFixedColor(clrButton, m_clrLeftTop21, 0.5));
		dcMem.SetPixel(rect.left, rect.top + 1, GetFixedColor(clrButton, m_clrLeftTop22, 0.5));

		dcMem.SetPixel(rect.left + 2, rect.top, GetFixedColor(clrButton, m_clrLeftTop31, 0.9));
		dcMem.SetPixel(rect.left, rect.top + 2, GetFixedColor(clrButton, m_clrLeftTop32, 0.9));
	}

	if (bEnableLeftBottomRound)
	{
		dcMem.SetPixel(rect.left, rect.bottom - 1, GetFixedColor(clrButton, m_clrLeftBottom1, 0.1));

		dcMem.SetPixel(rect.left, rect.bottom - 2, GetFixedColor(clrButton, m_clrLeftBottom21, 0.5));
		dcMem.SetPixel(rect.left + 1, rect.bottom - 1, GetFixedColor(clrButton, m_clrLeftBottom22, 0.5));

		dcMem.SetPixel(rect.left, rect.bottom - 3, GetFixedColor(clrButton, m_clrLeftBottom31, 0.9));
		dcMem.SetPixel(rect.left + 2, rect.bottom - 1, GetFixedColor(clrButton, m_clrLeftBottom32, 0.9));
	}

	if (bEnableRightBottomRound)
	{
		dcMem.SetPixel(rect.right - 1, rect.bottom - 1, GetFixedColor(clrButton, m_clrRightBottom1, 0.1));

		dcMem.SetPixel(rect.right - 2, rect.bottom - 1, GetFixedColor(clrButton, m_clrRightBottom21, 0.5));
		dcMem.SetPixel(rect.right - 1, rect.bottom - 2, GetFixedColor(clrButton, m_clrRightBottom22, 0.5));

		dcMem.SetPixel(rect.right - 3, rect.bottom - 1, GetFixedColor(clrButton, m_clrRightBottom31, 0.9));
		dcMem.SetPixel(rect.right - 1, rect.bottom - 3, GetFixedColor(clrButton, m_clrRightBottom32, 0.9));
	}

	if (bEnableRightTopRound)
	{
		dcMem.SetPixel(rect.right - 1, rect.top, GetFixedColor(clrButton, m_clrRightTop1, 0.1));

		dcMem.SetPixel(rect.right - 2, rect.top, GetFixedColor(clrButton, m_clrRightTop21, 0.5));
		dcMem.SetPixel(rect.right - 1, rect.top + 1, GetFixedColor(clrButton, m_clrRightTop22, 0.5));

		dcMem.SetPixel(rect.right - 3, rect.top, GetFixedColor(clrButton, m_clrRightTop31, 0.9));
		dcMem.SetPixel(rect.right - 1, rect.top + 2, GetFixedColor(clrButton, m_clrRightTop32, 0.9));
	}
}

void CSimpleButton::RoundRect(const CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrBk, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound)
{
    if (!bEnableLeftTopRound && !bEnableLeftBottomRound && !bEnableRightBottomRound && !bEnableRightTopRound)
    {
        return;
    }
    if (m_bEnableMultiBkColor)
    {
		RoundMultiColorBk(rect, dcMem, clrButton, bEnableLeftTopRound, bEnableLeftBottomRound, bEnableRightBottomRound, bEnableRightTopRound);
    }
    else
    {
		RoundSingleColorBk(rect, dcMem, clrButton, clrBk, bEnableLeftTopRound, bEnableLeftBottomRound, bEnableRightBottomRound, bEnableRightTopRound);
    }        
}

void CSimpleButton::SetWindowText(int nIndex, CString str)
{
	if (m_vecTextInfo.size() > nIndex)
	{
		m_vecTextInfo[nIndex].str = str;
		Invalidate(FALSE);
	}   
}

void CSimpleButton::GetWindowText(int nIndex, CString& str)
{
	if (m_vecTextInfo.size() > nIndex)
	{
		str = m_vecTextInfo[nIndex].str;
	}
	else
	{
		str.Empty();
	}
}

void CSimpleButton::SetFont(HFONT hFont)
{
    m_hFont = hFont;
}

HFONT CSimpleButton::GetFont()
{
	return m_hFont;
}

void CSimpleButton::SetFont(HFONT hFont0, HFONT hFont1)
{
    m_hFont0 = hFont0;
    m_hFont1 = hFont1;
    Invalidate(FALSE);
}

void CSimpleButton::SetRgnDefColor(COLORREF color)
{
    m_clrBk = color;
    Invalidate(TRUE);
}

void CSimpleButton::SetRgnHoverColor(COLORREF color)
{
    m_clrBkHover= color;
    Invalidate(FALSE);
}

void CSimpleButton::SetDisabledColor(COLORREF color)
{
    m_clrBkDisabled = color;
    Invalidate(FALSE);
}

void CSimpleButton::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

void CSimpleButton::SetTextDisabledColor(COLORREF color)
{
	m_clrTextDisabled = color;
	Invalidate(FALSE);
}

BOOL CSimpleButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleButton::GetWindowText(CString& str)
{
	str = m_strCaption;
}

void CSimpleButton::SetColorStyle(UINT nColor)
{
    switch (nColor)
    {
    case BUTTON_BLUE:
        {
            m_clrBk = m_clrBkBlue;
            m_clrBkHover = m_clrBkBlueHover;
            break;
        }
    case BUTTON_GRAY:
        {
            m_clrBk = m_clrBkGray;
            m_clrBkHover = m_clrBkGrayHover;
            break;
        }
    case BUTTON_RED:
        {
            m_clrBk = m_clrBkRed;
            m_clrBkHover = m_clrBkRedHover;
            break;
        }
    case BUTTON_GREEN:
        {
            m_clrBk = m_clrBkGreen;
            m_clrBkHover = m_clrBkGreenHover;
            break;
        }


    default:
        break;
    }
    Invalidate(FALSE);
}

BOOL CSimpleButton::EnableWindow(BOOL bEnable /* = TRUE */)
{
    m_bEnable = bEnable;
    Invalidate(FALSE);
    return CWnd::EnableWindow(bEnable);
}

void CSimpleButton::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
    Invalidate(FALSE);
}

void CSimpleButton::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    Invalidate(FALSE);
}

void CSimpleButton::SetWindowText(CString str)
{
    m_strCaption = str;
    Invalidate(FALSE);
}

void CSimpleButton::EnableClickToGetFocus(BOOL bEnable)
{
    m_bEnableClickToGetFocus = bEnable;
}

BOOL CSimpleButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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

void CSimpleButton::SetChangeCursor(BOOL bChangeCursor)
{
    m_bChangeCursor = bChangeCursor;
}

void CSimpleButton::EnableHoverLeaveNotify(BOOL bEnable)
{
    m_bEnableHoverLeaveNotify = bEnable;
}

void CSimpleButton::SetNextPosition(UINT posi)
{
	m_nTextPosition = posi;
	Invalidate(FALSE);
}

void CSimpleButton::OnMouseHover(UINT nFlags, CPoint point)
{
    if (m_bEnableHoverLeaveNotify)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_HOVER;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
    }   

    CWnd::OnMouseHover(nFlags, point);
}

void CSimpleButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!m_bSelected)
	{
		m_bLMouseDown = TRUE;
		m_bLMouseDBDown = TRUE;

		Invalidate(FALSE);
	}
	
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleButton::EnableMultiBkColor(BOOL bEnable/* = TRUE*/)
{
    m_bEnableMultiBkColor = bEnable;
}

BOOL CSimpleButton::IsGetFocus()
{
	if (GetFocus() == this)
	{
		return TRUE;
	}
	return FALSE;
}

void CSimpleButton::SetBtnStyle(CBtnStyle btnStyle)
{
	m_btnStyle = btnStyle;
	if (LeftStyle == btnStyle || RightStyle == btnStyle)
	{
		m_fontLeftRight.CreateFont(-10, 0, 0, 0,
						  FW_BOLD,
						  FALSE, FALSE,
						  FALSE,
						  DEFAULT_CHARSET,
						  OUT_DEFAULT_PRECIS,
						  CLIP_DEFAULT_PRECIS,
						  PROOF_QUALITY,
						  DEFAULT_PITCH,
						  _T("西文正文"));
	}	
}

CString CSimpleButton::GetCaption()
{
	CString strCaption;
	if (NormalStyle == m_btnStyle)
	{
		strCaption = m_strCaption;
	}
	else if (LeftStyle == m_btnStyle)
	{
		strCaption = "<";
	}
	else if (RightStyle == m_btnStyle)
	{
		strCaption = ">";
	}
	return strCaption;
}

HFONT CSimpleButton::GetHfont()
{
	HFONT hFont = NULL;
	if (LeftStyle == m_btnStyle || RightStyle == m_btnStyle)
	{
		hFont = (HFONT)m_fontLeftRight.GetSafeHandle();
	}
	else
	{
		hFont = m_hFont;
	}
	return hFont;
}

COLORREF CSimpleButton::GetTextColor()
{
	COLORREF clr = 0;
	if (NormalStyle == m_btnStyle)
	{
		clr = m_clrText;
	}
	else
	{
		clr = m_clrFocusOutline;
	}
	return clr;
}

COLORREF CSimpleButton::GetTriangleColor()
{
	COLORREF clr = 0;
	if (m_bEnable)
	{
		clr = m_clrFocusOutline;
	}
	else
	{
		clr = m_clrTextDisabled;
	}
	return clr;
}

void CSimpleButton::OnTimer(UINT_PTR nIDEvent)
{
	if (IsNeedTimer())
	{
		switch (nIDEvent)
		{
		case TIMER_ID_HOLD:
		{
							  KillTimer(TIMER_ID_HOLD);
							  if (m_bLMouseDown)
							  {
								  SetTimer(TIMER_ID_GAP, TIME_CLAPSE_GAP, NULL);
							  }
							  break;
		}
		case TIMER_ID_GAP:
		{
							 if (m_bLMouseDown)
							 {
								 NMHDR hdr;
								 hdr.idFrom = GetDlgCtrlID();
								 hdr.hwndFrom = GetSafeHwnd();
								 hdr.code = NM_CLICK;
								 GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
							 }
							 break;
		}
		default:
		break;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CSimpleButton::IsNeedTimer()
{
	if (NormalStyle != m_btnStyle)
	{
		return TRUE;
	}
	return FALSE;
}

int CSimpleButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
	m_toolTip.SetFont(&m_font);
	m_toolTip.SetTipBkColor(m_clrToolTipBkClr);
	m_toolTip.SetTipTextColor(m_clrToolTipTextClr);
	m_toolTip.SetMaxTipWidth(360);

	return 0;
}

BOOL CSimpleButton::PreTranslateMessage(MSG* pMsg)
{
	if (IsWindow(m_toolTip.GetSafeHwnd()) && m_bEnableTip)
	{
		m_toolTip.RelayEvent(pMsg);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CSimpleButton::EnableTip(BOOL bEnable)
{
	m_bEnableTip = bEnable;
}

void CSimpleButton::SetTipText(CString str)
{
	m_strTipText = str;
}

void CSimpleButton::EnableTab(BOOL bEnable)
{
	m_bEnableTab = bEnable;
	if (bEnable)
	{
		EnableClickToGetFocus(FALSE);
	}	
}

void CSimpleButton::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
	Invalidate(FALSE);
}

BOOL CSimpleButton::IsSelected()
{
	return m_bSelected;
}

void CSimpleButton::SetBkClrUnselected(COLORREF clr)
{
	m_clrBkUnselected = clr;
}