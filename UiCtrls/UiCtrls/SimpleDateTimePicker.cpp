// SimpleDateTimePicker.cpp : 实现文件
//

#include "stdafx.h"
#include "..\resource.h"
#include "SimpleDateTimePicker.h"


// CSimpleDateTimePicker

IMPLEMENT_DYNAMIC(CSimpleDateTimePicker, CWnd)

	CSimpleDateTimePicker::CSimpleDateTimePicker()
{
	RegisterWindowClass();

	m_clrBk = RGB(63, 65, 77);
	m_clrTextNormal = RGB(215, 215, 215);
	m_clrTextDisabled = RGB(189, 189, 189);
	m_brBk.CreateSolidBrush(m_clrBk);
	m_font.CreateFont(-12,0,0,0,FW_BOLD,FALSE,FALSE,FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,_T("微软雅黑"));
	m_bExpand = FALSE;
	m_bEnable = TRUE;
    m_pCalendar = nullptr;
}

CSimpleDateTimePicker::~CSimpleDateTimePicker()
{
	if (m_pCalendar != nullptr)
	{
		delete m_pCalendar;
		m_pCalendar = nullptr;
	}
}


BOOL CSimpleDateTimePicker::Create(UINT dwStyle, const CRect &rect, CWnd *pParentWnd, UINT nID)
{
	return CWnd::Create(SIMPLEDATETIMEPITCKER_CLASSNAME,"", dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleDateTimePicker::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, SIMPLEDATETIMEPITCKER_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = SIMPLEDATETIMEPITCKER_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSimpleDateTimePicker, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSimpleDateTimePicker 消息处理程序




int CSimpleDateTimePicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pCalendar = new CSimpleCalendar();
	m_pCalendar->Create(this);	

	return 0;
}


void CSimpleDateTimePicker::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(rcClient);

	//按钮图片
	CBitmap bitmapBtn;
	if (m_bEnable)
	{
		bitmapBtn.LoadBitmap(m_bExpand ? IDB_BITMAP_CALENDAR_GRAY : IDB_BITMAP_CALENDAR);
	} 
	else
	{
		bitmapBtn.LoadBitmap(IDB_BITMAP_CALENDAR_GRAY);
	}
	BITMAP bmp;
	GetObject(bitmapBtn.m_hObject, sizeof(BITMAP), (LPBYTE)&bmp);

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dc);
	CRect rcBmp(rcClient);
	rcBmp.left = rcClient.right - bmp.bmWidth;
	dcBmp.SelectObject(&bitmapBtn);
	dc.BitBlt(rcBmp.left, rcBmp.top, rcBmp.Width(), rcBmp.Height(), &dcBmp, 0, 0, SRCCOPY);
	dcBmp.DeleteDC();
	bitmapBtn.DeleteObject();

	//文字
	CRect rcText(rcClient);
	rcText.left += 4;
	rcText.top += 1;
	rcText.right = rcClient.right - bmp.bmWidth;

	CString str;
	dc.SelectObject(&m_font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrTextNormal);
	str.Format("%04d/%02d/%02d", m_pCalendar->GetYear(), m_pCalendar->GetMonth(), m_pCalendar->GetDay());
	dc.DrawText(str, &rcText, DT_VCENTER | DT_SINGLELINE);
}


void CSimpleDateTimePicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetFocus()->GetSafeHwnd() != this->GetSafeHwnd())
	{
		SetFocus();
	}
	if (m_pCalendar->GetSafeHwnd())
	{
		if(m_pCalendar->IsWindowVisible())
		{
			m_pCalendar->ShowCalendar(SW_HIDE);
			m_bExpand = FALSE;
		}
		else
		{
			m_pCalendar->ShowCalendar(SW_SHOW);
			m_bExpand = TRUE;
		}
		Invalidate(TRUE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleDateTimePicker::EnableWindow(BOOL bEnable)
{
	m_bEnable = bEnable;
	Invalidate(TRUE);
	CWnd::EnableWindow(bEnable);
}

void CSimpleDateTimePicker::SetBkColor(COLORREF clr)
{
	m_clrBk = clr;
	Invalidate(FALSE);
}

void CSimpleDateTimePicker::SetTextColor(COLORREF clrNormal, COLORREF clrDisable)
{
	m_clrTextNormal = clrNormal;
	m_clrTextDisabled = clrDisable;
	Invalidate(FALSE);
}


void CSimpleDateTimePicker::SetMonthCalColor(int nRegion, COLORREF ref)
{
	if (m_pCalendar != nullptr)
	{
		m_pCalendar->SetColor(nRegion, ref);
	}
}

void CSimpleDateTimePicker::SetBeExpand(BOOL bExpand)
{
	m_bExpand = bExpand;
	Invalidate(TRUE);
}

bool CSimpleDateTimePicker::GetTime(CTime &tm)
{
	if(m_pCalendar->GetYear() < 0 || m_pCalendar->GetMonth() < 0 || m_pCalendar->GetDay() < 0)
	{
		return false;
	}
	CTime time(m_pCalendar->GetYear(), m_pCalendar->GetMonth(), m_pCalendar->GetDay(), 0, 0, 0, -1);
	tm = time;
	return true;
}

CString CSimpleDateTimePicker::GetTime()
{
    if(m_pCalendar->GetYear() < 0 || m_pCalendar->GetMonth() < 0 || m_pCalendar->GetDay() < 0)
    {
        return NULL;
    }
    CString strDate;
    strDate.Format("%04d%02d%02d", m_pCalendar->GetYear(), m_pCalendar->GetMonth(), m_pCalendar->GetDay());
    return strDate;
}

void CSimpleDateTimePicker::SetDate(CTime tm)
{
	SetYear(tm.GetYear());
	SetMonth(tm.GetMonth());
	SetDay(tm.GetDay());
	Invalidate(TRUE);
}

void CSimpleDateTimePicker::SetYear(int year)
{
	if (m_pCalendar != nullptr && IsWindow(m_pCalendar->GetSafeHwnd()))
	{
		m_pCalendar->SetYear(year);
	}
}

void CSimpleDateTimePicker::SetMonth(int month)
{
	if (m_pCalendar != nullptr && IsWindow(m_pCalendar->GetSafeHwnd()))
	{
		m_pCalendar->SetMonth(month);
	}
}

void CSimpleDateTimePicker::SetDay(int day)
{
	if (m_pCalendar != nullptr && IsWindow(m_pCalendar->GetSafeHwnd()))
	{
		m_pCalendar->SetDay(day);
	}
}

BOOL CSimpleDateTimePicker::OnEraseBkgnd(CDC* pDC)
{
	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&m_brBk);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}
