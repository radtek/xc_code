#include "stdafx.h"
#include "BaseView.h"
#include "ColorSet.h"
#include "FontSet.h"
#include "DrawFun.h"
#include "../../tool/TradeDlg.h"

IMPLEMENT_DYNAMIC(CBaseView, CDockWindow)
CBaseView::CBaseView(CDockListener &listener)
	: CDockWindow(listener)
	, m_bMultiWindow(FALSE)
	, m_bStaticWindow(FALSE)
{
	m_nViewID = 0;
	m_clBackClr = SingleColorSet.GetColor(DLG_BACK);
	m_clBorderClr = RGB(104, 104, 104);
	m_clFocusBoderClr = RGB(0, 122, 204);
	m_clTilteBackClr = RGB(104, 104, 104);
	m_clFocusTilteBackClr = RGB(0, 122, 204);
	m_clTilteTextClr = RGB(255, 255, 255);
	m_clButtonClr = RGB(154, 164, 208);
	m_clHoverButtonClr = RGB(255, 255, 255);
	m_clHoverButtonBackClr = RGB(252, 74, 45);
	m_nBorder = 2;
	m_nTitleHeight = 30;
	//m_hTitleFont = SingleFontSet.GetDefaultFont(DEFAULT_FONT_12N);
	m_hTitleFont = CTradeDlg::s_hFontOrder;
	m_nHover = HOVER_NULL;
	m_bShowMaxMinButton = TRUE;
	m_rcClose.SetRectEmpty();
	m_rcMax.SetRectEmpty();
	m_rcMin.SetRectEmpty();
	m_bActivate = FALSE;
}

CBaseView::~CBaseView(void) 
{
	
}

void CBaseView::SetBackClr(COLORREF backClr)
{
	m_clBackClr = backClr;
}

void CBaseView::SetBorderClr(COLORREF borderClr)
{
	m_clBorderClr = borderClr;
}

void CBaseView::SetFocusBorderClr(COLORREF focusBorderClr)
{
	m_clFocusBoderClr = focusBorderClr;
}

void CBaseView::SetTilteBackClr(COLORREF titleBackClr)
{
	m_clTilteBackClr = titleBackClr;
}

void CBaseView::SetTilteFocusBackClr(COLORREF focusTitleBackClr)
{
	m_clFocusTilteBackClr = focusTitleBackClr;
}

void CBaseView::SetTilteTextClr(COLORREF titleTextClr)
{
	m_clTilteTextClr = titleTextClr;
}

void CBaseView::SetButtonClr(COLORREF buttonClr)
{
	m_clButtonClr = buttonClr;
}

void CBaseView::SetHoverButtonClr(COLORREF hoverButtonClr)
{
	m_clHoverButtonClr = hoverButtonClr;
}

void CBaseView::SetHoverButtonBackClr(COLORREF hoverButtonBackCrl)
{
	m_clHoverButtonBackClr = hoverButtonBackCrl;
}

void CBaseView::SetBorderSize(int nBorder)
{
	m_nBorder = nBorder;
}

void CBaseView::SetTitleHeight(int nTilteHeight)
{
	m_nTitleHeight = nTilteHeight;
}

void CBaseView::InitCtrls()
{

}

void CBaseView::Relayout()
{

}

void CBaseView::DrawBackGround(HDC hDC, const RECT rc)
{
	CDrawFun::FillRect(hDC, SingleColorSet.GetColor(DLG_BACK), rc);
}

void CBaseView::DrawBorder(CDC* pDC)
{
	if (m_hWnd && !m_bDocked)
	{
		COLORREF clBorderClr;
		COLORREF clTilteBackClr;
		if (m_bActivate)
		{
			clBorderClr = m_clFocusBoderClr;
			clTilteBackClr = m_clFocusTilteBackClr;
		}
		else
		{
			clBorderClr = m_clBorderClr;
			clTilteBackClr = m_clTilteBackClr;
		}

		CBrush Brush(clBorderClr);
		CBrush* pOldBrush = pDC->SelectObject(&Brush);

		CRect rtWnd;
		GetWindowRect(&rtWnd);

		CPoint point;
		//填充顶部框架
		point.x = rtWnd.Width();
		point.y = m_nBorder;
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充左侧框架
		point.x = m_nBorder;
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充底部框架
		point.x = rtWnd.Width();
		point.y = m_nBorder;
		pDC->PatBlt(0, rtWnd.Height() - point.y, point.x, point.y, PATCOPY);
		//填充右侧框架
		point.x = m_nBorder;
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width() - point.x, 0, point.x, point.y, PATCOPY);
		pDC->SelectObject(pOldBrush);
		pDC->FillSolidRect(0, 0, rtWnd.Width(), m_nTitleHeight, clTilteBackClr);
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clOldText = pDC->SetTextColor(m_clTilteTextClr);
		HGDIOBJ original = pDC->SelectObject(m_hTitleFont);

		int nTitleHeight = m_nTitleHeight;
		if (IsZoomed())
		{
			nTitleHeight = 40;
		}
		CRect rcTitle(8,
			0,
			rtWnd.Width() - 60,
			nTitleHeight - 1);
		CString strTitle;
		GetWindowText(strTitle);
		pDC->DrawText(strTitle, rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		pDC->SelectObject(original);
		pDC->SetBkMode(nOldMode);
		pDC->SetTextColor(clOldText);
		if (m_nHover == HOVER_CLOSE)
		{
			CRect rcTemp = m_rcClose;
			rcTemp.InflateRect(2, 2, 2, 2);
			CDrawFun::RoundRect(pDC->m_hDC, m_clHoverButtonBackClr, rcTemp, 2, 2);
			CDrawFun::DrawCloseButton(pDC->m_hDC, m_rcClose, m_clHoverButtonClr);
		}
		else
		{
			CDrawFun::DrawCloseButton(pDC->m_hDC, m_rcClose, m_clButtonClr);
		}

		if (!m_rcMax.IsRectEmpty())
		{
			if (m_nHover == HOVER_MAX)
			{
				if (IsZoomed())
				{
					CDrawFun::DrawMaxButton(pDC->m_hDC, m_rcMax, m_clHoverButtonClr, 2, true, clTilteBackClr);
				}
				else
				{
					CDrawFun::DrawMaxButton(pDC->m_hDC, m_rcMax, m_clHoverButtonClr);
				}
			}
			else
			{
				if (IsZoomed())
				{
					CDrawFun::DrawMaxButton(pDC->m_hDC, m_rcMax, m_clButtonClr, 2, true, clTilteBackClr);
				}
				else
				{
					CDrawFun::DrawMaxButton(pDC->m_hDC, m_rcMax, m_clButtonClr);
				}
			}
		}
		if (!m_rcMin.IsRectEmpty())
		{
			if (m_nHover == HOVER_MIN)
			{
				CDrawFun::DrawMinButton(pDC->m_hDC, m_rcMin, m_clHoverButtonClr);
			}
			else
			{
				CDrawFun::DrawMinButton(pDC->m_hDC, m_rcMin, m_clButtonClr);
			}
		}
	}
}

void CBaseView::OnShowMin()
{
	ShowWindow(SW_MINIMIZE);
}

void CBaseView::OnShowMax()
{
	ShowWindow(SW_MAXIMIZE);
}

void CBaseView::OnShowRestore()
{
	ShowWindow(SW_RESTORE);
}

BEGIN_MESSAGE_MAP(CBaseView, CDockWindow)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_MOVING()
	ON_WM_NCCALCSIZE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_NCACTIVATE()
	ON_WM_NCMOUSELEAVE()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CBaseView::OnEraseBkgnd(CDC* pDC)
{
	if (m_bDocked)
	{
		CBrush backBrush(SingleColorSet.GetColor(DLG_BACK));

		//保存旧的brush
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);
		CRect rect;
		pDC->GetClipBox(&rect);

		//画需要的区域
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
		pDC->SelectObject(pOldBrush);
	}
	return TRUE;
}

void CBaseView::OnClose()
{
	int nEndViewID = GetViewID();
    int m_nEndTick = GetTickCount();
	if (m_bStaticWindow)
	{
		ShowWindow(SW_HIDE);
		if (m_bDocked == true)
		{
			Undock();
		}
		return;
	}

	CDockWindow::OnClose();
}

void CBaseView::SaveSetInfo(CString &strSetInfo)
{

}

void CBaseView::LoadSetInfo(const CString &strSetInfo)
{

}

void CBaseView::SetDock(bool bDock)
{
	m_bDocked = bDock;
}

bool CBaseView::GetIsDock()
{
	return m_bDocked;
}

void CBaseView::OnMoving(UINT fwSide, LPRECT pRect)
{
	//if (m_bEnableDock && !m_bDocked)
	//{
	//	CPoint ptCur;
	//	::GetCursorPos(&ptCur);
	//	CSimpleDockPane &dockPane = ((CMainFrame*)AfxGetMainWnd())->m_wndView.GetDockPane(ptCur);
	//	SetDockListener(dockPane);
	//	//SetForegroundWindow();
	//}
	CDockWindow::OnMoving(fwSide, pRect);
}

void CBaseView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (!m_bDocked)
	{
		int nHeight = 0;
		int nHeightBorder = 0;
		int nWidthBorder = 0;
		if (m_nBorder == 0)
		{
			m_nBorder = 2;
		}
		lpncsp->rgrc[0].top += (m_nTitleHeight - nHeight - nHeightBorder);
		lpncsp->rgrc[0].bottom += (nHeightBorder - m_nBorder);
		lpncsp->rgrc[0].left -= (nWidthBorder - m_nBorder);
		lpncsp->rgrc[0].right += (nWidthBorder - m_nBorder);
	}
}

void CBaseView::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if (!m_bDocked)
	{
		CPoint _point = point;
		ScreenToClient(&_point);
		_point.y += m_nTitleHeight;
		if (m_rcClose.PtInRect(_point))
		{
			SendMessage(WM_CLOSE, 0, 0);
			return;
		}
		else if (m_rcMin.PtInRect(_point))
		{
			OnShowMin();
			m_bActivate = FALSE;
			m_nHover = HOVER_NULL;
			OnNcPaint();
			return;
		}
		else if (m_rcMax.PtInRect(_point))
		{
			m_nHover = HOVER_NULL;
			if (IsIconic())
			{
				OnShowRestore();
			}
			else if (IsZoomed())
			{
				OnShowRestore();
			}
			else
			{
				OnShowMax();
			}
			OnNcPaint();
			return;
		}
	}
	CDockWindow::OnNcLButtonUp(nHitTest, point);
}

void CBaseView::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (!m_bDocked)
	{
		CPoint _point = point;
		ScreenToClient(&_point);
		_point.y += m_nTitleHeight;
		if (m_rcClose.PtInRect(_point))
		{
			return;
		}
		else if (m_rcMin.PtInRect(_point))
		{
			return;
		}
		else if (m_rcMax.PtInRect(_point))
		{
			return;
		}
	}
	CDockWindow::OnNcLButtonDown(nHitTest, point);
}

void CBaseView::OnNcPaint()
{
	if (!m_bDocked)
	{
		CDC* pDC = GetWindowDC();
		CRect ncClient, rcClient;
		GetClientRect(rcClient);
		GetWindowRect(&ncClient);
		ncClient = CRect(0, 0, ncClient.Width(), ncClient.Height());
		CRgn rgn, rgn1;
		rgn.CreateRectRgn(0, 0, ncClient.right, ncClient.bottom);
		rgn1.CreateRectRgn(m_nBorder, m_nTitleHeight, ncClient.right - m_nBorder, ncClient.bottom - m_nBorder);
		rgn.CombineRgn(&rgn, &rgn1, RGN_DIFF);
		pDC->SelectClipRgn(&rgn);
		CDC dcMem;
		CBitmap bmp;
		dcMem.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC, ncClient.Width(), ncClient.Height());
		CBitmap * oldBitmap = dcMem.SelectObject(&bmp);
		DrawBorder(&dcMem);			// include caption and border 
		pDC->BitBlt(0, 0, ncClient.Width(), ncClient.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(oldBitmap);
		bmp.DeleteObject();
		dcMem.DeleteDC();
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
		rgn1.DeleteObject();
		ReleaseDC(pDC);
	}
}

void CBaseView::OnSize(UINT nType, int cx, int cy)
{
	CDockWindow::OnSize(nType, cx, cy);
	if (!m_bDocked)
	{
		CRect rc, rcWnd;
		GetWindowRect(&rc);
		rcWnd.left = 0;
		rcWnd.right = rc.Width();
		rcWnd.top = 0;
		rcWnd.bottom = rc.Height();

		m_rcClose.right = rcWnd.right - 8;
		m_rcClose.left = m_rcClose.right - 13;
		m_rcClose.top = (m_nTitleHeight - 13) / 2;
		m_rcClose.bottom = m_rcClose.top + 13;
		if (IsZoomed())
		{
			m_rcClose.top += 5;
			m_rcClose.bottom += 5;
			m_rcClose.left -= 5;
			m_rcClose.right -= 5;
		}
		if (m_bShowMaxMinButton)
		{
			m_rcMax = m_rcClose;
			m_rcMax.right = m_rcMax.left - 10;
			m_rcMax.left = m_rcMax.right - 13;

			m_rcMin = m_rcMax;
			m_rcMin.right = m_rcMin.left - 10;
			m_rcMin.left = m_rcMin.right - 13;
		}
	}	
	
	if (nType == SIZE_MAXIMIZED ||
		nType == SIZE_RESTORED)
	{
		OnNcPaint();
	}
	if (!m_bDocked)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
}

BOOL CBaseView::OnNcActivate(BOOL bActive)
{
	if (m_bActivate != bActive)
	{
		m_bActivate = bActive;
		m_nHover = HOVER_NULL;
		OnNcPaint();
	}
	return  TRUE;//CBaseView::OnNcActivate(bActive);不Return true 在移动的时候窗体有问题
}

void CBaseView::OnNcMouseLeave()
{
	if (m_nHover != HOVER_NULL)
	{
		m_nHover = HOVER_NULL;
		OnNcPaint();
	}
	CDockWindow::OnNcMouseLeave();
}

LRESULT CBaseView::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CDockWindow::OnNcHitTest(point);
	if (!m_bDocked)
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);
		CRect rc(0, 0, rcWnd.Width(), rcWnd.Height());
		rc.bottom = rc.top + m_nTitleHeight;
		CPoint _point = point;
		ScreenToClient(&_point);
		_point.y += m_nTitleHeight;
		if (rc.PtInRect(_point))
		{
			nHitTest = HTCAPTION;
		}
		CRect rect;
		GetWindowRect(&rect);
		if (point.x <= rect.left + 3)
			nHitTest = HTLEFT;
		else if (point.x >= rect.right - 3)
			nHitTest = HTRIGHT;
		else if (point.y <= rect.top + 3)
			nHitTest = HTTOP;
		else if (point.y >= rect.bottom - 3)
			nHitTest = HTBOTTOM;
		else if (point.x <= rect.left + 10 && point.y <= rect.top + 10)
			nHitTest = HTTOPLEFT;
		else if (point.x >= rect.right - 10 && point.y <= rect.top + 10)
			nHitTest = HTTOPRIGHT;
		else if (point.x <= rect.left + 10 && point.y >= rect.bottom - 10)
			nHitTest = HTBOTTOMLEFT;
		else if (point.x >= rect.right - 10 && point.y >= rect.bottom - 10)
			nHitTest = HTBOTTOMRIGHT;
	}
	if (nHitTest == HTCAPTION || nHitTest == HTTOPRIGHT || nHitTest == HTTOP)
	{
		BOOL bDraw = FALSE;

		CPoint _point = point;
		ScreenToClient(&_point);
		_point.y += m_nTitleHeight;
		HOVER_BUTTON nHover = HOVER_NULL;
		if (m_rcClose.PtInRect(_point))
		{
			nHover = HOVER_CLOSE;
		}
		else if (m_rcMin.PtInRect(_point))
		{
			nHover = HOVER_MIN;
		}
		if (m_rcMax.PtInRect(_point))
		{
			nHover = HOVER_MAX;
		}
		if (m_nHover != nHover)
		{
			bDraw = TRUE;
			m_nHover = nHover;
		}
		if (bDraw)
		{
			OnNcPaint();
		}
	}
	return nHitTest;
}

void CBaseView::OnPaint()
{
	if (m_bDocked)
	{
		CDockWindow::OnPaint();
		return;
	}
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);//获取客户区大小
	CRect rcTemp(rcClient);

	CDC dcMem;
	CBitmap bmpMem;
	CBitmap *pOldBmp = nullptr;

	if (dcMem.CreateCompatibleDC(&dc))
	{
		if (bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height()))
			pOldBmp = dcMem.SelectObject(&bmpMem);
		else
			return;
	}
	else
		return;
	DrawBackGround(dcMem.m_hDC, rcClient);
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}
