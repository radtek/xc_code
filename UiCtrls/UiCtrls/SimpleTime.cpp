// SimpleTime.cpp : 实现文件
//

#include "stdafx.h"
#include "SimpleTime.h"

// CSimpleTime

IMPLEMENT_DYNAMIC(CSimpleTime, CWnd)

	CSimpleTime::CSimpleTime()
{
	RegisterWindowClass();
	m_clrBk = RGB(18, 20, 22);
	m_clrTextNormal = RGB(215, 215, 215);
	m_clrTextDisabled = RGB(189, 189, 189);
	m_font.CreateFont(-12,0,0,0,FW_BOLD,FALSE,FALSE,FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,_T("微软雅黑"));
    m_Font = (HFONT)m_font.GetSafeHandle();
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_nHour = st.wHour;
	m_nMin = st.wMinute;
	m_nSec = st.wSecond;

	m_bEnable = TRUE;
	m_bChecked = FALSE;
	m_bBoxShow = TRUE;
	m_mode = EDITABLE_WHILE_CHECKED;

	m_time = HOUR;
}

CSimpleTime::~CSimpleTime()
{
}

BOOL CSimpleTime::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{

	return CWnd::Create(SIMPLETIME_CLASSNAME, "", dwStyle, rect, pParentWnd, nID);
}

void CSimpleTime::SetBkColor(COLORREF clr)
{
	m_clrBk = clr;
	Invalidate(FALSE);
}

void CSimpleTime::SetTextColor(COLORREF clrNormal, COLORREF clrDisable)
{
	m_clrTextNormal = clrNormal;
	m_clrTextDisabled = clrDisable;
	Invalidate(FALSE);
}

void CSimpleTime::SetFont(HFONT font)
{
	m_Font = font;
	Invalidate(FALSE);
}

void CSimpleTime::EnableWindow(BOOL bEnable /*= TRUE*/)
{
	if (m_bEnable != bEnable)
	{
		m_bEnable = bEnable;
		Invalidate(FALSE);
	}
}

void CSimpleTime::SetChecked(BOOL bChecked /*= FALSE*/)
{
	if (!m_bEnable)
	{
		return;
	}
	if (m_bChecked != bChecked)
	{
		m_bChecked = bChecked;
		Invalidate(FALSE);
	}
}

BOOL CSimpleTime::GetChecked()
{
	return m_bChecked;
}

void CSimpleTime::SetBoxShow(BOOL bShow /*= TRUE*/)
{
	if (!m_bEnable)
	{
		return;
	}
	if (m_bBoxShow != bShow)
	{
		m_bBoxShow = bShow;
		Invalidate(FALSE);
	}
}

BOOL CSimpleTime::GetBoxShow()
{
	return m_bBoxShow;
}

void CSimpleTime::SetMode(UINT mode /*= EDITABLE_WHILE_CHECKED*/)
{
	if (!m_bEnable)
	{
		return;
	}
	if (m_mode != mode)
	{
		m_mode = mode;
		if(m_mode < 0 || m_mode > 1) m_mode = EDITABLE_WHILE_CHECKED;
	}
}

UINT CSimpleTime::GetMode()
{
	return m_mode;
}

void CSimpleTime::GetTime(CTime &tm)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CTime time(st.wYear, st.wMonth, st.wDay, m_nHour, m_nMin, m_nSec);
	tm = time;
}

CString CSimpleTime::GetTime()
{
	CString strTime;
	strTime.Format("%02d:%02d:%02d", m_nHour, m_nMin, m_nSec);
	return strTime;
}

void CSimpleTime::SetTime(CTime tm)
{
	m_nHour = tm.GetHour();
	m_nMin = tm.GetMinute();
	m_nSec = tm.GetSecond();
	Invalidate(FALSE);
}

void CSimpleTime::SetTime(CString strTime)
{
	CString strHour = strTime.Mid(0, 2);
	CString strMin = strTime.Mid(3, 2);
	CString strSec = strTime.Mid(6, 2);
	m_nHour = atoi(strHour.GetBuffer());
	m_nMin = atoi(strMin.GetBuffer());
	m_nSec = atoi(strSec.GetBuffer());
	Invalidate(FALSE);
}

BOOL CSimpleTime::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, SIMPLETIME_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = SIMPLETIME_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void CSimpleTime::DrawUnchecked(CDC &dc, CRect rt, COLORREF clrA, COLORREF clrB, COLORREF clrC)
{
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, clrA);
	dc.SelectObject(pen1);
	dc.MoveTo(rt.TopLeft());
	dc.LineTo(rt.left + 2, rt.top + 2);
	dc.MoveTo(rt.right, rt.top);
	dc.LineTo(rt.right - 2, rt.top + 2);
	dc.MoveTo(rt.left, rt.bottom);
	dc.LineTo(rt.left + 2, rt.bottom - 2);
	dc.MoveTo(rt.BottomRight());
	dc.LineTo(rt.right - 2, rt.bottom - 2);

	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1, clrB);
	dc.SelectObject(pen2);
	dc.MoveTo(rt.left + 1, rt.top);
	dc.LineTo(rt.right, rt.top);
	dc.MoveTo(rt.right, rt.top + 1);
	dc.LineTo(rt.right, rt.bottom);
	dc.MoveTo(rt.right - 1, rt.bottom);
	dc.LineTo(rt.left, rt.bottom);
	dc.MoveTo(rt.left, rt.bottom - 1);
	dc.LineTo(rt.left, rt.top);


	CPen pen3;
	pen3.CreatePen(PS_SOLID, 1, clrC);
	dc.SelectObject(pen3);
	dc.MoveTo(rt.left + 2, rt.top + 1);
	dc.LineTo(rt.right - 1, rt.top + 1);
	dc.MoveTo(rt.right - 1, rt.top + 2);
	dc.LineTo(rt.right - 1, rt.bottom - 1);
	dc.MoveTo(rt.right - 2, rt.bottom - 1);
	dc.LineTo(rt.left + 1, rt.bottom - 1);
	dc.MoveTo(rt.left + 1, rt.bottom - 2);
	dc.LineTo(rt.left + 1, rt.top + 1);
}

void CSimpleTime::DrawChecked(CDC &dc, CRect rt, COLORREF clrA, COLORREF clrB)
{
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, clrA);
	dc.SelectObject(pen1);
	dc.MoveTo(rt.left, rt.top);
	dc.LineTo(rt.right, rt.top);
	dc.MoveTo(rt.right, rt.top);
	dc.LineTo(rt.right, rt.bottom);
	dc.MoveTo(rt.right, rt.bottom);
	dc.LineTo(rt.left, rt.bottom);
	dc.MoveTo(rt.left, rt.bottom);
	dc.LineTo(rt.left, rt.top);

	dc.MoveTo(rt.left + 2, rt.top + 5);
	dc.LineTo(rt.left + 5, rt.top + 8);
	dc.MoveTo(rt.left + 5, rt.top + 6);
	dc.LineTo(rt.left + 10, rt.top + 1);

	dc.MoveTo(rt.left + 1, rt.top + 6);
	dc.LineTo(rt.left + 5, rt.top + 10);
	dc.MoveTo(rt.left + 5, rt.top + 8);
	dc.LineTo(rt.left + 11, rt.top + 2);

	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1, clrB);
	dc.SelectObject(pen2);
	dc.MoveTo(rt.left + 1, rt.top);
	dc.LineTo(rt.right, rt.top);

	dc.MoveTo(rt.left, rt.top + 1);
	dc.LineTo(rt.right + 1, rt.top + 1);

	dc.MoveTo(rt.left, rt.top + 2);
	dc.LineTo(rt.right - 2, rt.top + 2);
	dc.MoveTo(rt.right - 1, rt.top + 2);
	dc.LineTo(rt.right + 1, rt.top + 2);

	dc.MoveTo(rt.left, rt.top + 3);
	dc.LineTo(rt.right - 3, rt.top + 3);
	dc.MoveTo(rt.right, rt.top + 3);
	dc.LineTo(rt.right + 1, rt.top + 3);

	dc.MoveTo(rt.left, rt.top + 4);
	dc.LineTo(rt.right - 4, rt.top + 4);
	dc.MoveTo(rt.right - 1, rt.top + 4);
	dc.LineTo(rt.right + 1, rt.top + 4);

	dc.MoveTo(rt.left, rt.top + 5);
	dc.LineTo(rt.left + 2, rt.top + 5);
	dc.MoveTo(rt.left + 3, rt.top + 5);
	dc.LineTo(rt.left + 6, rt.top + 5);
	dc.MoveTo(rt.right - 2, rt.top + 5);
	dc.LineTo(rt.right + 1, rt.top + 5);

	dc.MoveTo(rt.left, rt.top + 6);
	dc.LineTo(rt.left + 1, rt.top + 6);
	dc.MoveTo(rt.left + 4, rt.top + 6);
	dc.LineTo(rt.left + 5, rt.top + 6);
	dc.MoveTo(rt.right - 3, rt.top + 6);
	dc.LineTo(rt.right + 1, rt.top + 6);

	dc.MoveTo(rt.left, rt.top + 7);
	dc.LineTo(rt.left + 2, rt.top + 7);
	dc.MoveTo(rt.right - 4, rt.top + 7);
	dc.LineTo(rt.right + 1, rt.top + 7);

	dc.MoveTo(rt.left, rt.top + 8);
	dc.LineTo(rt.left + 3, rt.top + 8);
	dc.MoveTo(rt.right - 5, rt.top + 8);
	dc.LineTo(rt.right + 1, rt.top + 8);

	dc.MoveTo(rt.left, rt.top + 9);
	dc.LineTo(rt.left + 4, rt.top + 9);
	dc.MoveTo(rt.right - 6, rt.top + 9);
	dc.LineTo(rt.right + 1, rt.top + 9);

	dc.MoveTo(rt.left, rt.top + 10);
	dc.LineTo(rt.right + 1, rt.top + 10);

	dc.MoveTo(rt.left + 1, rt.top + 11);
	dc.LineTo(rt.right, rt.top + 11);
}

BEGIN_MESSAGE_MAP(CSimpleTime, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CSimpleTime 消息处理程序

void CSimpleTime::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, m_clrBk);

	CRect rcBox(rcClient);
	if (m_bBoxShow)
	{
		//打钩框
		rcBox.right = rcBox.left + 11;
		rcBox.top += 6;
		rcBox.bottom = rcBox.top + 11;
		if (m_bEnable)
		{
			if (m_bChecked)
			{
				DrawChecked(dc, rcBox, RGB(33, 48, 81), RGB(61, 91, 153));
			}
			else
			{
				DrawUnchecked(dc, rcBox, RGB(120, 120, 120), RGB(215, 215, 215), RGB(72, 72, 72));
			}
		}
		else
		{
			if (m_bChecked)
			{
				DrawChecked(dc, rcBox, RGB(30, 41, 66), RGB(30, 45, 76));
			}
			else
			{
				DrawUnchecked(dc, rcBox, RGB(88, 88, 88), RGB(107, 107, 107), RGB(70, 70, 70));
			}
		}
	}

	//文字
	COLORREF clrText = m_clrTextNormal;
	if (m_bBoxShow)
	{
		clrText = m_bEnable && m_bChecked ? m_clrTextNormal : m_clrTextDisabled;
	} 
	else
	{
		clrText = m_bEnable ? m_clrTextNormal : m_clrTextDisabled;
	}
	dc.SelectObject(m_Font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrText);

	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, clrText);
	dc.SelectObject(pen1);

	CString str = "";
	CRect rcText(rcClient);
	rcText.left = m_bBoxShow ? rcBox.left + rcBox.Width() + 8 : 4;
	rcText.top += 2;
	rcText.bottom = rcText.top + 20;

	rcText.right = rcText.left + 16;
	str.Format("%02d", m_nHour);
	dc.DrawText(str, rcText, DT_VCENTER | DT_SINGLELINE);
	if (m_time == HOUR)
	{
		dc.MoveTo(rcText.left, rcText.bottom);
		dc.LineTo(rcText.right, rcText.bottom);
	}

	rcText.left += rcText.Width() + 2;
	rcText.right = rcText.left + 5;
	str = ":";
	dc.DrawText(str, rcText, DT_VCENTER | DT_SINGLELINE);

	rcText.left += rcText.Width() + 2;
	rcText.right = rcText.left + 16;
	str.Format("%02d", m_nMin);
	dc.DrawText(str, rcText, DT_VCENTER | DT_SINGLELINE);
	if (m_time == MINUTE)
	{
		dc.MoveTo(rcText.left, rcText.bottom);
		dc.LineTo(rcText.right, rcText.bottom);
	}

	rcText.left += rcText.Width() + 2;
	rcText.right = rcText.left + 5;
	str = ":";
	dc.DrawText(str, rcText, DT_VCENTER | DT_SINGLELINE);

	rcText.left += rcText.Width() + 2;
	rcText.right = rcText.left + 16;
	str.Format("%02d", m_nSec);
	dc.DrawText(str, rcText, DT_VCENTER | DT_SINGLELINE);
	if (m_time == SECOND)
	{
		dc.MoveTo(rcText.left, rcText.bottom);
		dc.LineTo(rcText.right, rcText.bottom);
	}
}

void CSimpleTime::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bEnable)
	{
		return;
	}


	//box
	CRect rtBox;
	//hour
	CRect rtHour;
	//minute
	CRect rtMinute;
	//second
	CRect rtSecond;
	if (m_bBoxShow)
	{
		rtBox.SetRect(0, 6, 11, 17);
		rtHour.SetRect(19, 2, 35, 22);
		rtMinute.SetRect(44, 2, 60, 22);
		rtSecond.SetRect(69, 2, 85, 22);
	}
	else
	{
		rtBox.SetRect(0, 0, 0, 0);
		rtHour.SetRect(4, 2, 20, 22);
		rtMinute.SetRect(29, 2, 45, 22);
		rtSecond.SetRect(54, 2, 70, 22);
	}

	if (rtBox.PtInRect(point))
	{
		m_bChecked = !m_bChecked;
		Invalidate(FALSE);
	}
	else if (rtHour.PtInRect(point)
		&& ((m_bChecked && m_mode == EDITABLE_WHILE_CHECKED) || (!m_bChecked && m_mode == EDITABLE_WHILE_UNCHECKED)))
	{
		if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
		{
			SetFocus();
		}
		m_time = HOUR;
		Invalidate(FALSE);
	}
	else if (rtMinute.PtInRect(point)
		&& ((m_bChecked && m_mode == EDITABLE_WHILE_CHECKED) || (!m_bChecked && m_mode == EDITABLE_WHILE_UNCHECKED)))
	{
		if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
		{
			SetFocus();
		}
		m_time = MINUTE;
		Invalidate(FALSE);
	}
	else if (rtSecond.PtInRect(point)
		&& ((m_bChecked && m_mode == EDITABLE_WHILE_CHECKED) || (!m_bChecked && m_mode == EDITABLE_WHILE_UNCHECKED)))
	{
		if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
		{
			SetFocus();
		}
		m_time = SECOND;
		Invalidate(FALSE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleTime::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!m_bEnable)
	{
		return;
	}
	OnLButtonDown(nFlags, point);
	CWnd::OnLButtonDblClk(nFlags, point);
}

int CSimpleTime::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CSimpleTime::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bEnable)
	{
		return;
	}
	if ((m_bChecked && m_mode != EDITABLE_WHILE_CHECKED) || (!m_bChecked && m_mode != EDITABLE_WHILE_UNCHECKED))
	{
		return;
	}

	//不处于focus状态 不作处理
	if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
	{
		return;
	}

	//box
	CRect rtBox;
	//hour
	CRect rtHour;
	//minute
	CRect rtMinute;
	//second
	CRect rtSecond;
	if (m_bBoxShow)
	{
		rtBox.SetRect(0, 6, 11, 17);
		rtHour.SetRect(19, 2, 35, 22);
		rtMinute.SetRect(44, 2, 60, 22);
		rtSecond.SetRect(69, 2, 85, 22);
	}
	else
	{
		rtBox.SetRect(0, 0, 0, 0);
		rtHour.SetRect(4, 2, 20, 22);
		rtMinute.SetRect(29, 2, 45, 22);
		rtSecond.SetRect(54, 2, 70, 22);
	}

	switch (nChar)
	{
	case VK_TAB:
	case VK_RIGHT:
		if (m_time == HOUR)
		{
			m_time = MINUTE;
			Invalidate(FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_time = SECOND;
			Invalidate(FALSE);
		}
		else if (m_time == SECOND)
		{
			m_time = HOUR;
			Invalidate(FALSE);
		}
		break;
	case VK_BACK:
	case VK_LEFT:
		if (m_time == HOUR)
		{
			m_time = SECOND;
			Invalidate(FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_time = HOUR;
			Invalidate(FALSE);
		}
		else if (m_time == SECOND)
		{
			m_time = MINUTE;
			Invalidate(FALSE);
		}
		break;
	case VK_DELETE:
		if (m_time == HOUR)
		{
			m_nHour = m_nHour / 10;
			InvalidateRect(rtHour, FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_nMin = m_nMin / 10;
			InvalidateRect(rtMinute, FALSE);
		}
		else if (m_time == SECOND)
		{
			m_nSec = m_nSec / 10;
			InvalidateRect(rtSecond, FALSE);
		}
		break;
	case VK_DOWN:
		if (m_time == HOUR)
		{
			m_nHour--;
			if (m_nHour < 0)
			{
				m_nHour = 23;
			}
			InvalidateRect(rtHour, FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_nMin--;
			if (m_nMin < 0)
			{
				m_nMin = 59;
			}
			InvalidateRect(rtMinute, FALSE);
		}
		else if (m_time == SECOND)
		{
			m_nSec--;
			if (m_nSec < 0)
			{
				m_nSec = 59;
			}
			InvalidateRect(rtSecond, FALSE);
		}
		break;
	case VK_UP:
		if (m_time == HOUR)
		{
			m_nHour++;
			if (m_nHour > 23)
			{
				m_nHour = 0;
			}
			InvalidateRect(rtHour, FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_nMin++;
			if (m_nMin > 59)
			{
				m_nMin = 0;
			}
			InvalidateRect(rtMinute, FALSE);
		}
		else if (m_time == SECOND)
		{
			m_nSec++;
			if (m_nSec > 59)
			{
				m_nSec = 0;
			}
			InvalidateRect(rtSecond, FALSE);
		}
		break;
	default:
		break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSimpleTime::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bEnable)
	{
		return;
	}
	if ((m_bChecked && m_mode != EDITABLE_WHILE_CHECKED) || (!m_bChecked && m_mode != EDITABLE_WHILE_UNCHECKED))
	{
		return;
	}

	//不处于focus状态 不作处理
	if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
	{
		return;
	}

	//box
	CRect rtBox;
	//hour
	CRect rtHour;
	//minute
	CRect rtMinute;
	//second
	CRect rtSecond;
	if (m_bBoxShow)
	{
		rtBox.SetRect(0, 6, 11, 17);
		rtHour.SetRect(19, 2, 35, 22);
		rtMinute.SetRect(44, 2, 60, 22);
		rtSecond.SetRect(69, 2, 85, 22);
	}
	else
	{
		rtBox.SetRect(0, 0, 0, 0);
		rtHour.SetRect(4, 2, 20, 22);
		rtMinute.SetRect(29, 2, 45, 22);
		rtSecond.SetRect(54, 2, 70, 22);
	}

	int num = 0;
	if (('0' <= nChar && nChar <= '9')
		||(VK_NUMPAD0 <= nChar && nChar <= VK_NUMPAD9))
	{
		if ('0' <= nChar && nChar <= '9')
		{
			num = nChar - '0';
		}
		else if (VK_NUMPAD0 <= nChar && nChar <= VK_NUMPAD9)
		{
			num = nChar - VK_NUMPAD0;
		}

		if (m_time == HOUR)
		{
			m_nHour = m_nHour * 10 + num;
			if (m_nHour > 23)
			{
				m_nHour = m_nHour % 100 ;
				if (m_nHour > 23)
				{
					m_nHour = num;
				}
			}
			InvalidateRect(rtHour, FALSE);
		} 
		else if (m_time == MINUTE)
		{
			m_nMin = m_nMin * 10 + num;
			if (m_nMin > 59)
			{
				m_nMin = m_nMin % 100 ;
				if (m_nMin > 59)
				{
					m_nMin = num;
				}
			}
			InvalidateRect(rtMinute, FALSE);
		}
		else if (m_time == SECOND)
		{
			m_nSec = m_nSec * 10 + num;
			if (m_nSec > 59)
			{
				m_nSec = m_nSec % 100;
				if (m_nSec > 59)
				{
					m_nSec = num;
				}
			}
			InvalidateRect(rtSecond, FALSE);
		}
	}
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CSimpleTime::PreTranslateMessage(MSG* pMsg)
{
	if (!m_bEnable)
	{
		return CWnd::PreTranslateMessage(pMsg);
	}
	//不处于focus状态 不作处理
	if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
	{
		return CWnd::PreTranslateMessage(pMsg);
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP
			|| pMsg->wParam == VK_DOWN
			|| pMsg->wParam == VK_LEFT
			|| pMsg->wParam == VK_RIGHT 
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_TAB
			|| pMsg->wParam == VK_BACK)
		{
			SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	else if (pMsg->message == WM_CHAR)
	{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CSimpleTime::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CSimpleTime::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bEnable)
	{
		return;
	}

	if (m_bBoxShow)
	{
		CRect rt;
		GetClientRect(rt);
		//box
		CRect rtBox(0, 6, 11, 17);
		if (rtBox.PtInRect(point))
		{
			TimeCheckBox timeCheckBox;
			timeCheckBox.bCheck = m_bChecked;
			timeCheckBox.hdr.idFrom = GetDlgCtrlID();
			timeCheckBox.hdr.hwndFrom = GetSafeHwnd();
			timeCheckBox.hdr.code = NM_CLICK;
			GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&timeCheckBox);
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}
