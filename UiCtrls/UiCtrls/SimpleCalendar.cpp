// SimpleCalendar.cpp : 实现文件
//

#include "stdafx.h"
#include "..\resource.h"
#include "SimpleCalendar.h"
#include "SimpleDateTimePicker.h"

#define STARTYEAR 1099
#define ENDYEAR  2099

// CSimpleCalendar

IMPLEMENT_DYNAMIC(CSimpleCalendar, CWnd)

	CSimpleCalendar::CSimpleCalendar()
{
	RegisterWindowClass();
	m_pParent = nullptr;
	m_crlBackground = RGB(63, 65, 77);
	m_crlText = RGB(215, 215, 215);
	m_crlSelBk = RGB(51, 78, 188);
	m_crlSelBorder = RGB(82, 148, 215);
	m_crlTitleBk = RGB(63, 65, 77);
	m_crlTitleText = RGB(215, 215, 215);
	m_crlTrailingText = RGB(140, 140, 140);
	m_crlMonthBk = RGB(63, 65, 77);
	m_font.CreateFont(-12,0,0,0,FW_BOLD,FALSE,FALSE,FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,_T("微软雅黑"));
	m_Font = (HFONT)m_font.GetSafeHandle();
    m_brushBk.CreateSolidBrush(m_crlBackground);
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_nDay = st.wDay;
	m_nMonth = st.wMonth;
	m_nYear = st.wYear;

	m_nSelIndex = -1;

	InitCellRect();
	RefreshCellDate();
}

CSimpleCalendar::~CSimpleCalendar()
{
	m_pParent = nullptr;
}

void CSimpleCalendar::ShowCalendar(int nCmdShow)
{
	if (nCmdShow == SW_HIDE)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	CRect rcParent, rcCalendar, rcClient;
	GetWindowRect(&rcClient);
	m_pParent->GetWindowRect(&rcParent);
	rcCalendar.left = rcParent.left;
	rcCalendar.right = rcCalendar.left + rcClient.Width();
	rcCalendar.top = rcParent.bottom;
	rcCalendar.bottom = rcCalendar.top + rcClient.Height();

	//与屏幕的关系处理
	CSize ScreenSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
	if (rcCalendar.right > ScreenSize.cx)
	{
		rcCalendar.OffsetRect(ScreenSize.cx - rcCalendar.right, 0);
	}

	if (rcCalendar.left < 0)
	{
		rcCalendar.OffsetRect(-rcCalendar.left, 0);		
	}

	if (rcCalendar.bottom > ScreenSize.cy)
	{		
		rcCalendar.OffsetRect(0, -(rcParent.Height() + rcCalendar.Height()));
	}

	MoveWindow(rcCalendar, TRUE);
	ShowWindow(nCmdShow);
}


void CSimpleCalendar::SetFont(HFONT hFont)
{
	m_Font = hFont;
}

void CSimpleCalendar::SetColor(int nRegion, COLORREF ref)
{
	switch (nRegion)
	{
	case SCSC_BACKGROUND:  // the background color (between months)
		m_crlBackground = ref;
		break;
	case SCSC_TEXT:    // the dates
		m_crlText = ref;
		break;
	case SCSC_TITLEBK:  // background of the title
		m_crlTitleBk = ref;
		break;
	case SCSC_TITLETEXT:  
		m_crlTitleText = ref;
		break;
	case SCSC_MONTHBK:  // background within the month cal
		m_crlMonthBk = ref;
		break;
	case SCSC_TRAILINGTEXT:  // the text color of header & trailing days
		m_crlTrailingText = ref;
		break;
	case SCSC_SELBK:           // the text color of Sel cell bk 
		m_crlSelBk = ref;
		break;
	case SCSC_SELBORDER:       // the text color of Sel cell Border
		m_crlSelBorder = ref;
		break;
	default:
		break;
	}
}

BOOL CSimpleCalendar::Create(CWnd *pParent)
{
	m_pParent = (CSimpleDateTimePicker *)pParent;
	return CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST , SIMPLECALENDAR_CLASSNAME, "", WS_POPUP, CRect(0, 0, 210, 225), nullptr, NULL, nullptr);
}

int CSimpleCalendar::GetYear()
{
	return m_nYear;
}

int CSimpleCalendar::GetMonth()
{
	return m_nMonth;
}

int CSimpleCalendar::GetDay()
{
	return m_nDay;
}

void CSimpleCalendar::SetYear(int year)
{
	m_nYear = year;
}

void CSimpleCalendar::SetMonth(int month)
{
	m_nMonth = month;
}

void CSimpleCalendar::SetDay(int day)
{
	m_nDay = day;
}

bool CSimpleCalendar::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, SIMPLECALENDAR_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = nullptr;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = nullptr;
		wndcls.lpszClassName    = SIMPLECALENDAR_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return false;
		}
	}

	return true;
}

bool CSimpleCalendar::IsLeapYear(int year)
{
	bool ret = false;
	if (year % 100 == 0)//整百
	{
		ret = year % 400 == 0 ? true : false;
	}
	else
	{
		ret = year % 4 == 0 ? true : false;
	}

	return ret;
}

void CSimpleCalendar::InitCellRect()
{
	int nLeft = 0;
	int nTop= 45;
	memset(m_Dates, 0, sizeof(m_Dates));
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			m_Dates[row * 7 + col].mcRcCell.left = nLeft + 30 * col;
			m_Dates[row * 7 + col].mcRcCell.right = m_Dates[row * 7 + col].mcRcCell.left + 30;
			m_Dates[row * 7 + col].mcRcCell.top = nTop + 30 * row;
			m_Dates[row * 7 + col].mcRcCell.bottom = m_Dates[row * 7 + col].mcRcCell.top + 30;
		}
	}
}

void CSimpleCalendar::RefreshCellDate()
{
	SYSTEMTIME today;
	GetLocalTime(&today);
	int dayOfWeek = GetDayOfWeek( m_nYear, m_nMonth, 1);//日期所在月的1号是礼拜几
	int daysOfMonth = GetDaysOfMonth(m_nYear, m_nMonth);//该月天数
	int day = 0;
	for (int i = 0; i < 42; i++)
	{
		// Check starting day of the month
		if (i < dayOfWeek || day >= daysOfMonth)
		{
			m_Dates[i].mcDay = 0;
			m_Dates[i].mcState = EMPTY;
			m_Dates[i].mcToday = false;
			m_Dates[i].mcWeekendDay = false;
		}
		else
		{
			day++;
			m_Dates[i].mcDay = day;
			if (m_nDay == m_Dates[i].mcDay)
			{
				m_Dates[i].mcState = SELECTED;
				m_nSelIndex = i;
			}
			else
			{
				m_Dates[i].mcState = UNSELECTED;
			}
		}

		if (m_Dates[i].mcState != EMPTY)
		{
			// If current day is today
			if ((m_Dates[i].mcDay == today.wDay) && (m_nMonth == today.wMonth) && (m_nYear == today.wYear))
			{
				m_Dates[i].mcToday = true;
			}
			else
			{
				m_Dates[i].mcToday = false;
			}

			// If current day is weekendDay
			int weekendDay = GetDayOfWeek(m_nYear, m_nMonth, m_Dates[i].mcDay);
			m_Dates[i].mcWeekendDay = (weekendDay == 0 || weekendDay == 6) ? true : false;
		}
	}
}

void CSimpleCalendar::DrawTitleDate(CDC *pDC)
{
	pDC->FillSolidRect(CRect(0, 0, 210, 25), m_crlTitleBk);
	pDC->SetTextColor(m_crlTitleText);

	CString str;
	str.Format("%04d    %02d", m_nYear, m_nMonth);
	CRect rc(0, 0, 210, 25);
	pDC->DrawText(str, rc, DT_CALCRECT | DT_SINGLELINE);
	int nLeft = (210 - rc.Width()) / 2;
	int nTop = (25 - rc.Height()) / 2;
	m_rcTitleDate = CRect(nLeft, nTop, nLeft+rc.Width(), nTop+rc.Height());
	pDC->DrawText(str, m_rcTitleDate, DT_SINGLELINE|DT_VCENTER);	
}

void CSimpleCalendar::RefreshTitleDate()
{
	InvalidateRect(m_rcTitleDate, FALSE);
}

void CSimpleCalendar::RefreshMonthCell()
{
	InvalidateRect(CRect(0, 45, 210, 225), FALSE);
}

void CSimpleCalendar::DrawWeek(CDC *pDC)
{
	pDC->FillSolidRect(CRect(0, 25, 210, 45), m_crlTitleBk);
	pDC->SetTextColor(m_crlTrailingText);

	CString str = "";

	//英文 week
	// 	char chWeek[7][10] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
	// 	 	CRect rcZone(7, 25, 37, 45);
	// 	 	for (auto i : chWeek)
	// 	 	{
	// 	 		str = i;
	// 	 		pDC->DrawText(str, rcZone, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	// 	 		pDC->DrawText(str, rcZone, DT_SINGLELINE | DT_VCENTER | DT_CENTER);	
	// 	 		rcZone.OffsetRect(rcZone.Width() + 4, 0);
	// 	 	}

	//中文 week
	char chWeek[7][10] = {"日", "一", "二", "三", "四", "五", "六"};
	CRect rcZone(0, 25, 30, 45);
	for (auto i : chWeek)
	{
		str = i;
		pDC->DrawText(str, rcZone, DT_SINGLELINE | DT_VCENTER | DT_CENTER);	
		rcZone.OffsetRect(30, 0);
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_crlTrailingText);
	pDC->SelectObject(&pen);
	pDC->MoveTo(0, 44);
	pDC->LineTo(210, 44);
}

void CSimpleCalendar::DrawDate(CDC *pDC)
{
	RefreshCellDate();
	pDC->FillSolidRect(CRect(0, 45, 210, 200), m_crlMonthBk);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_crlSelBorder);
	pDC->SelectObject(pen);
	for(int i = 1; i < 43; i++)
	{
		if(m_Dates[i-1].mcState != EMPTY)//字
		{
			if (m_Dates[i - 1].mcState == SELECTED)
			{
				pDC->FillSolidRect(m_Dates[i - 1].mcRcCell, m_crlSelBk);
			}
			else
			{
				pDC->SetBkColor(m_crlMonthBk);
			}

			CString strDay;
			strDay.Format("%02d", m_Dates[i-1].mcDay);	
			if (m_Dates[i - 1].mcWeekendDay)
			{
				pDC->SetTextColor(m_crlTrailingText);
			} 
			else
			{
				pDC->SetTextColor(m_crlText);
			}
			pDC->DrawText(strDay, m_Dates[i-1].mcRcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		if (m_Dates[i - 1].mcToday)
		{	
			pDC->MoveTo(m_Dates[i-1].mcRcCell.TopLeft());
			pDC->LineTo(m_Dates[i-1].mcRcCell.left, m_Dates[i-1].mcRcCell.bottom);
			pDC->LineTo(m_Dates[i-1].mcRcCell.right, m_Dates[i-1].mcRcCell.bottom);
			pDC->LineTo(m_Dates[i-1].mcRcCell.right, m_Dates[i-1].mcRcCell.top);
			pDC->LineTo(m_Dates[i-1].mcRcCell.left, m_Dates[i-1].mcRcCell.top);
		}
	}

	// 	CPen penBottom;
	// 	penBottom.CreatePen(PS_SOLID, 1, m_crlTrailingText);
	// 	pDC->SelectObject(&penBottom);
	// 	pDC->MoveTo(0, 224);
	// 	pDC->LineTo(210, 224);
}

int CSimpleCalendar::GetDayOfWeek(int year, int month, int day)
{
	//蔡勒公式 并考虑负数
	//  w：星期（计算所得的数值对应的星期：0-星期日；1-星期一；2-星期二；3-星期三；4-星期四；5-星期五；6-星期六）[1]
	//  c：年份前两位数
	//	y：年份后两位数
	//	m：月（m的取值范围为3至14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，比如2003年1月1日要看作2002年的13月1日来计算）
	//	d：日
	//	[　]：称作高斯符号，代表向下取整，即，取不大于原数的最大整数。
	//	mod：同余（这里代表括号里的答案除以7后的余数）
	int c = 0,y = 0,m = 0,d = 0,w = -1;

	if (month == 1 || month == 2)
	{
		year--;
		month += 12;
	}
	c = year / 100;
	y = year % 100;
	m = month;
	d = day;

	int temp1, temp2, temp3;
	temp1 = y / 4;
	temp2 = c / 4;
	temp3 = 13 * (m + 1) / 5;

	w = y + temp1 + temp2 - 2 * c + temp3 + d - 1;
	w = (w % 7 + 7) % 7;
	return w;
}

int CSimpleCalendar::GetDaysOfMonth(int year, int month)
{
	int numbers = 0;
	if (month == 2)
	{
		numbers = IsLeapYear(year) ? 29 : 28;
	}
	else
	{
		if (month <= 7)
		{
			numbers = month % 2 == 0 ? 30 : 31;
		}
		else
		{
			numbers = month % 2 == 0 ? 31 : 30;
		}
	}

	return numbers;
}

int CSimpleCalendar::GetClickedCell(CPoint pt)
{
	for(int i = 0; i < 42; i++)
	{
		if(m_Dates[i].mcState != EMPTY && m_Dates[i].mcRcCell.PtInRect(pt))
		{
			return i;			
		}
	}
	return -1;
}

void CSimpleCalendar::SetCellState(int nIndex, bool bHighlight)
{
	if (m_Dates[nIndex].mcState != EMPTY)
	{
		CClientDC dc(this);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_crlSelBorder);
		dc.SelectObject(pen);
		dc.SelectObject(m_Font);
		if(bHighlight)
		{
			dc.FillSolidRect(m_Dates[nIndex].mcRcCell, m_crlSelBk);
		}
		else
		{
			dc.FillSolidRect(m_Dates[nIndex].mcRcCell, m_crlMonthBk);
		}

		CString strDay;
		strDay.Format("%02d", m_Dates[nIndex].mcDay);	
		if (m_Dates[nIndex].mcWeekendDay)
		{
			dc.SetTextColor(m_crlTrailingText);
		} 
		else
		{
			dc.SetTextColor(m_crlText);
		}
		dc.DrawText(strDay, m_Dates[nIndex].mcRcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		if (m_Dates[nIndex].mcToday)
		{	
			dc.MoveTo(m_Dates[nIndex].mcRcCell.TopLeft());
			dc.LineTo(m_Dates[nIndex].mcRcCell.left, m_Dates[nIndex].mcRcCell.bottom);
			dc.LineTo(m_Dates[nIndex].mcRcCell.right, m_Dates[nIndex].mcRcCell.bottom);
			dc.LineTo(m_Dates[nIndex].mcRcCell.right, m_Dates[nIndex].mcRcCell.top);
			dc.LineTo(m_Dates[nIndex].mcRcCell.left, m_Dates[nIndex].mcRcCell.top);
		}
	}
}

BEGIN_MESSAGE_MAP(CSimpleCalendar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_MONTHPRE, &CSimpleCalendar::OnClickedButtonMonthPre)
	ON_BN_CLICKED(IDC_BUTTON_MONTHNEXT, &CSimpleCalendar::OnClickedButtonMonthNext)
	ON_EN_CHANGE(IDC_EDIT_YEAR, &CSimpleCalendar::OnChangeEditYear)
	ON_EN_CHANGE(IDC_EDIT_MONTH, &CSimpleCalendar::OnChangeEditMonth)
	ON_EN_KILLFOCUS(IDC_EDIT_YEAR, &CSimpleCalendar::OnKillfocusEditYear)
	ON_EN_KILLFOCUS(IDC_EDIT_MONTH, &CSimpleCalendar::OnKillfocusEditMonth)
END_MESSAGE_MAP()



// CSimpleCalendar 消息处理程序

int CSimpleCalendar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_btnMonthPre.Create("", WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, CRect(15, 0, 40, 25), this, IDC_BUTTON_MONTHPRE);	
	m_btnMonthPre.LoadBitmaps(IDB_BITMAP_CALENDAR_LEFT, IDB_BITMAP_CALENDAR_LEFT_PRESS);

	m_btnMonthNext.Create("", WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, CRect(170, 0, 195, 25), this, IDC_BUTTON_MONTHNEXT);	
	m_btnMonthNext.LoadBitmaps(IDB_BITMAP_CALENDAR_RIGHT, IDB_BITMAP_CALENDAR_RIGHT_PRESS);

	m_tbYear.Create(WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, CRect(70, 2, 105, 23), this, IDC_EDIT_YEAR);
	m_tbYear.SetFont(CFont::FromHandle(m_Font));
	m_tbYear.SetLimitText(4);

	m_tbMonth.Create(WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, CRect(110, 2, 135, 23), this, IDC_EDIT_MONTH);
	m_tbMonth.SetFont(CFont::FromHandle(m_Font));
	m_tbMonth.SetLimitText(2);
	return 0;
}

void CSimpleCalendar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);

	dc.SelectObject(m_Font);//字体
	dc.FillSolidRect(rc, m_crlBackground); // 背景色

	//head + title
	DrawTitleDate(&dc);
	//week
	DrawWeek(&dc);
	//Date
	DrawDate(&dc);
}

HBRUSH CSimpleCalendar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crlTitleText);
		return (HBRUSH)m_brushBk.GetSafeHandle();
	}
	return hbr;
}

void CSimpleCalendar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(rc);

	CRect rcYear(70, 2, 105, 23);
	if(rcYear.PtInRect(point))
	{
		m_tbYear.ShowWindow(SW_SHOW);
		m_tbYear.SetWindowText("");
		m_tbYear.SetFocus();
	}
	else
	{
		if(m_tbYear.IsWindowVisible())
		{
			m_tbYear.ShowWindow(SW_HIDE);
		}
	}

	CRect rcMonth(110, 2, 135, 23);
	if(rcMonth.PtInRect(point))
	{
		m_tbMonth.ShowWindow(SW_SHOW);
		m_tbMonth.SetWindowText("");
		m_tbMonth.SetFocus();
	}
	else
	{
		if(m_tbMonth.IsWindowVisible())
		{
			m_tbMonth.ShowWindow(SW_HIDE);
		}
	}

	int nCellIndex = GetClickedCell(point);
	if(nCellIndex >= 0)
	{
		if(m_nSelIndex >= 0)
		{	
			m_Dates[m_nSelIndex].mcState = UNSELECTED;
			SetCellState(m_nSelIndex, false);
		}
		m_Dates[nCellIndex].mcState = SELECTED;
		SetCellState(nCellIndex, true);
		m_nSelIndex = nCellIndex;
	}

	TRACE("OnLButtonDown %d\n", m_nSelIndex);
	CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleCalendar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rc(0, 45, 210, 225);
	if(rc.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
		int nIndex = GetClickedCell(point);
		if(nIndex >= 0)
		{
			m_nDay = m_Dates[nIndex].mcDay;
		}
		else
		{
			RefreshCellDate();		
		}

		m_pParent->SetBeExpand(FALSE);
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleCalendar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if(WA_INACTIVE == nState && IsWindowVisible())
	{		
		ShowWindow(SW_HIDE);
		m_pParent->SetBeExpand(FALSE);		
	}
}

void CSimpleCalendar::OnClickedButtonMonthPre()
{
	if(m_nMonth == 1)
	{
		m_nMonth = 12;
		m_nYear--;
	}
	else 
	{
		m_nMonth--;
	}

	RefreshCellDate();
	RefreshMonthCell();
	RefreshTitleDate();
}

void CSimpleCalendar::OnClickedButtonMonthNext()
{
	if(m_nMonth == 12)
	{
		m_nMonth = 1;
		m_nYear++;
	}
	else 
	{
		m_nMonth++;
	}

	RefreshCellDate();
	RefreshMonthCell();
	RefreshTitleDate();
}

void CSimpleCalendar::OnChangeEditYear()
{
	CString str;
	m_tbYear.GetWindowText(str);
	if(str.GetLength() == 4)
	{
		m_tbYear.ShowWindow(SW_HIDE);
		int y = atoi(str);
		if ( y < STARTYEAR )
		{
			y = STARTYEAR;
		}
		else if ( y > ENDYEAR )
		{
			y = ENDYEAR;
		}
		m_nYear = y;
		RefreshCellDate();
		RefreshMonthCell();
	}
}

void CSimpleCalendar::OnChangeEditMonth()
{
	CString str;
	m_tbMonth.GetWindowText(str);
	if(str.GetLength() == 2)
	{
		m_tbMonth.ShowWindow(SW_HIDE);
		int m = atoi(str);
		if ( m < 1 )
		{
			m = 1;
		}
		else if ( m > 12 )
		{
			m = 12;
		}
		m_nMonth = m;
		RefreshCellDate();
		RefreshMonthCell();
	}
}

void CSimpleCalendar::OnKillfocusEditYear()
{
	CString str;
	m_tbYear.GetWindowText(str);

	int y = atoi(str);
	if(y == 0)
	{
		y = m_nYear;
	}
	else if (y < STARTYEAR)
	{
		y = STARTYEAR;
	}
	else if (y > ENDYEAR)
	{
		y = ENDYEAR;
	}
	m_nYear = y;
	RefreshCellDate();
	RefreshMonthCell();
	if(m_tbYear.IsWindowVisible())
	{
		m_tbYear.ShowWindow(SW_HIDE);
	}
}

void CSimpleCalendar::OnKillfocusEditMonth()
{
	CString str;
	m_tbMonth.GetWindowText(str);

	int m = atoi(str);
	if(m == 0)
	{
		m = m_nMonth;
	}
	else if ( m < 0 )
	{	
		m = 1;
	}
	else if ( m > 12 )
	{
		m = 12;
	}
	m_nMonth = m;
	RefreshCellDate();
	RefreshMonthCell();
	if(m_tbMonth.IsWindowVisible())
	{
		m_tbMonth.ShowWindow(SW_HIDE);
	}
}

