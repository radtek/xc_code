
//*****************************************************
//********    SimpleButtonEdit By YaoShuLong 2018.6.13
//*****************************************************

#include "stdafx.h"
#include "SimpleButtonEdit.h"
#include "UiDefine.h"

// CSimpleButtonEdit

IMPLEMENT_DYNAMIC(CSimpleButtonEdit, CWnd)

CSimpleButtonEdit::CSimpleButtonEdit()
{
    RegisterWindowClass();
    m_clrBk = RGB(18, 20, 22);
    m_style = LeftRight;
    m_nHeight = Def_Height;
	m_nBtnLeftRightWidth = Def_BtnLeftRightWidth;
    m_nEditWidth = Def_EditWidth;
    m_nGap = Def_Gap;
	m_fTick = 0;
	m_nPrecision = 0;
	m_dwStyle = 0;
	m_bSetSel = TRUE;
	m_dateType = TypeDouble;
	m_IntValueControl = TRUE;
	m_bHotKey = FALSE;
	m_EnableMouseWheel = TRUE;
	m_bEnableMixZero = FALSE;
}

CSimpleButtonEdit::~CSimpleButtonEdit()
{
}

BEGIN_MESSAGE_MAP(CSimpleButtonEdit, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
	ON_EN_CHANGE(IDC_BUTTONEDIT_EDIT, &CSimpleButtonEdit::OnEnChangeEdit)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_BUTTONEDIT_UP, IDC_BUTTONEDIT_RIGHT, &CSimpleButtonEdit::OnButtonRange)

END_MESSAGE_MAP()

// CSimpleButtonEdit 消息处理程序

void CSimpleButtonEdit::OnPaint()
{
    CPaintDC dc(this);

    CRect rCl;
    GetClientRect(&rCl);
    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp = NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc, rCl.Width(), rCl.Height()))
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

    dcMem.FillSolidRect(&rCl, m_clrBk);
    dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();
}

BOOL CSimpleButtonEdit::Create(DWORD dwStyle, const CRect& rect, CWnd * pParentWnd, UINT nID)
{
	m_dwStyle = dwStyle;
	dwStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CWnd::Create(SimpleButtonEdit_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleButtonEdit::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleButtonEdit_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleButtonEdit_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSimpleButtonEdit::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

int CSimpleButtonEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

	m_btnUp.SetBtnStyle(UpStyle);
	m_btnUp.Create(WS_CHILD, 0, CRect(0, 0, 0, 0), this, IDC_BUTTONEDIT_UP, "", 0, FALSE, FALSE, FALSE, FALSE);
	m_btnUp.SetRgnDefColor(RGB(63, 65, 77));
	m_btnUp.SetRgnHoverColor(RGB(81, 83, 96));
	m_btnUp.SetDisabledColor(RGB(52, 54, 69));
	m_btnUp.EnableClickToGetFocus(FALSE);

	m_btnDown.SetBtnStyle(DownStyle);
	m_btnDown.Create(WS_CHILD, 0, CRect(0, 0, 0, 0), this, IDC_BUTTONEDIT_DOWN, "", 0, FALSE, FALSE, FALSE, FALSE);
	m_btnDown.SetRgnDefColor(RGB(63, 65, 77));
	m_btnDown.SetRgnHoverColor(RGB(81, 83, 96));
	m_btnDown.SetDisabledColor(RGB(52, 54, 69));
	m_btnDown.EnableClickToGetFocus(FALSE);

	m_btnLeft.SetBtnStyle(LeftStyle);
	m_btnLeft.Create(WS_CHILD, 0, CRect(0, 0, 0, 0), this, IDC_BUTTONEDIT_LEFT, "", DT_CENTER);
    m_btnLeft.SetChangeCursor(TRUE);
	m_btnLeft.SetRgnDefColor(RGB(63, 65, 77));
	m_btnLeft.SetRgnHoverColor(RGB(81, 83, 96));
	m_btnLeft.SetDisabledColor(RGB(52, 54, 69));
	m_btnLeft.EnableClickToGetFocus(FALSE);

	m_btnRight.SetBtnStyle(RightStyle);
	m_btnRight.Create(WS_CHILD, 0, CRect(0, 0, 0, 0), this, IDC_BUTTONEDIT_RIGHT, "", DT_CENTER);
	m_btnRight.SetChangeCursor(TRUE);
	m_btnRight.SetRgnDefColor(RGB(63, 65, 77));
	m_btnRight.SetRgnHoverColor(RGB(81, 83, 96));
	m_btnRight.SetDisabledColor(RGB(52, 54, 69));
	m_btnRight.EnableClickToGetFocus(FALSE);

	m_edit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_BUTTONEDIT_EDIT);			   
    //m_edit.InitSettings();
	m_edit.EnableButtonEdit();
    return 0;
}

void CSimpleButtonEdit::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

	if (LeftRight == m_style)
	{
		if (IsWindow(m_btnLeft.GetSafeHwnd()))
		{
			m_btnLeft.ShowWindow(SW_SHOW);
		}
		if (IsWindow(m_btnRight.GetSafeHwnd()))
		{
			m_btnRight.ShowWindow(SW_SHOW);
		}

		if (IsWindow(m_btnUp.GetSafeHwnd()))
		{
			m_btnUp.ShowWindow(SW_HIDE);
		}
		if (IsWindow(m_btnDown.GetSafeHwnd()))
		{
			m_btnDown.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (IsWindow(m_btnLeft.GetSafeHwnd()))
		{
			m_btnLeft.ShowWindow(SW_HIDE);
		}
		if (IsWindow(m_btnRight.GetSafeHwnd()))
		{
			m_btnRight.ShowWindow(SW_HIDE);
		}

		if (IsWindow(m_btnUp.GetSafeHwnd()))
		{
			m_btnUp.ShowWindow(SW_SHOW);
		}
		if (IsWindow(m_btnDown.GetSafeHwnd()))
		{
			m_btnDown.ShowWindow(SW_SHOW);
		}
	}


    if (IsWindow(m_edit.GetSafeHwnd()))
    {
        if (LeftRight == m_style)
        {
			CRect rectButton(0, 0, m_nBtnLeftRightWidth, m_nHeight);
            if (IsWindow(m_btnLeft.GetSafeHwnd()))
            {               
                m_btnLeft.MoveWindow(rectButton);
            }

            CRect rectEdit(rectButton.right + m_nGap, 0, rectButton.right + m_nGap + m_nEditWidth, m_nHeight);
            m_edit.MoveWindow(&rectEdit);

			rectButton.OffsetRect(m_nBtnLeftRightWidth + m_nGap * 2 + m_nEditWidth, 0);
            if (IsWindow(m_btnRight.GetSafeHwnd()))
            {   
                m_btnRight.MoveWindow(&rectButton);
            }
        }
        else
        {
            CRect rectEdit(0, 0, m_nEditWidth, m_nHeight);
            m_edit.MoveWindow(&rectEdit);

			CRect rectBtnUp(rectEdit.right, 0, rectEdit.right + Def_BtnUpDownWidth, Def_BtnUpHeight);
            if (IsWindow(m_btnUp.GetSafeHwnd()))
            {
				m_btnUp.MoveWindow(&rectBtnUp);
            }

			CRect rectBtnDown(rectBtnUp.left, rectBtnUp.bottom, rectBtnUp.right, rectBtnUp.bottom + Def_BtnDownHeight);
			if (IsWindow(m_btnDown.GetSafeHwnd()))
			{
				m_btnDown.MoveWindow(&rectBtnDown);
			}
        }
    }
}

void CSimpleButtonEdit::SetEditBkColor(COLORREF clr)
{
    m_edit.SetBkColor(clr);
}

void CSimpleButtonEdit::SetTextColor(COLORREF clr)
{
    m_edit.SetTextColor(clr);
}

void CSimpleButtonEdit::SetWindowText(CString str)
{
    m_edit.SetWindowText(str);
}

void CSimpleButtonEdit::SetTextOffset(int nTextOffset)
{
    m_edit.SetTextOffset(nTextOffset);
}

void CSimpleButtonEdit::GetWindowText(CString& str)
{
    m_edit.GetWindowText(str);
}

void CSimpleButtonEdit::InitSettings(CString strHint /*= _T("")*/, int nTextLimit /*= -1*/, HFONT hFont /*= NULL*/)
{
    m_edit.InitSettings(strHint, nTextLimit, hFont);
}

void CSimpleButtonEdit::SetTextPos(TextPos textPos /*= TextRight*/)
{
	if (TextLeft == textPos)
	{
		m_edit.ModifyStyle(0, ES_LEFT);
	}
	else if (TextCenter == textPos)
	{
		m_edit.ModifyStyle(0, ES_CENTER);
	}
	else
	{
		m_edit.ModifyStyle(0, ES_RIGHT);
	}
}


void CSimpleButtonEdit::InitSettings(ButtonStyle style, DataTypeBtnEdit dataType, COLORREF clrText, HFONT hFont /*= NULL*/, TextPos textPos /*= TextRight*/, int nTextOffset /*= 0*/, CString strHint /*= _T("")*/, int nTextLimit /*= -1*/)
{
	m_style = style;
	if (UpDown == style)
	{
		m_bSetSel = FALSE;
	}
	m_dateType = dataType;
	if (TypeDouble == m_dateType)
	{
		m_edit.EnableFloat();
	}
	else
	{
		m_edit.ModifyStyle(0, ES_NUMBER);
	}

	if (TextLeft == textPos)
	{
		m_edit.ModifyStyle(0, ES_LEFT);
	}
	else if (TextCenter == textPos)
	{
		m_edit.ModifyStyle(0, ES_CENTER);
	}
	else
	{
		m_edit.ModifyStyle(0, ES_RIGHT);
	}
	m_edit.SetTextColor(clrText);
	m_edit.SetTextOffset(nTextOffset);
	m_edit.InitSettings(strHint, nTextLimit, hFont);
}

void CSimpleButtonEdit::SetBkColor(COLORREF clr)
{
    m_clrBk = clr;
    Invalidate();
}

void CSimpleButtonEdit::SetButtonStyle(ButtonStyle style)
{
    m_style = style;
}

void CSimpleButtonEdit::GetWidthAndHeight(int& nWidth, int& nHeight)
{
    nHeight = m_nHeight;
    if (LeftRight == m_style)
    {
		nWidth = (m_nBtnLeftRightWidth * 2 + m_nGap * 2 + m_nEditWidth);
    }
    else
    {
		nWidth = m_nEditWidth + Def_BtnUpDownWidth;
    }
}

void CSimpleButtonEdit::OnButtonRange(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_BUTTONEDIT_UP && nID <= IDC_BUTTONEDIT_RIGHT)
	{
		switch (nID)
		{
		case IDC_BUTTONEDIT_DOWN:
		case IDC_BUTTONEDIT_LEFT:
		{
									DoPlusMinus(Minus, m_fTick, m_nPrecision);
									break;
		}
		case IDC_BUTTONEDIT_UP:
		case IDC_BUTTONEDIT_RIGHT:
		{
									 DoPlusMinus(Plus, m_fTick, m_nPrecision);
									break;
		}
		default:
			break;
		}
	}
}

void CSimpleButtonEdit::SetTick(double tick)
{
	m_fTick = tick;
}

void CSimpleButtonEdit::SetPrecision(int precision)
{
	m_nPrecision = precision;
}

void CSimpleButtonEdit::EnableIntValueControl(BOOL bEnable)
{
	m_IntValueControl = bEnable;
}

void CSimpleButtonEdit::DoPlusMinus(PlusMinus type, double fTick, int nPrecision)
{
	CString strValue;
	m_edit.GetWindowText(strValue);
	double fValue = atof(strValue.GetBuffer(0));
	if (Plus == type)
	{
		fValue += fTick;
	}
	else
	{
		fValue -= fTick;
		
		if (TypeInt == m_dateType && m_IntValueControl)
		{
			if (m_bEnableMixZero)
			{
				if (IS_DOUBLE_LESS(fValue, 0))
				{
					fValue = 0;
				}
			}
			else
			{
				if (IS_DOUBLE_ZERO(fValue))
				{
					fValue += fTick;
				}
				else if (IS_DOUBLE_LESS(fValue, 0))
				{
					fValue = 1;
				}
			}
		}
	}
	strValue = TransDoubleToString(fValue, nPrecision);
	m_edit.SetWindowText(strValue);

	if (m_bSetSel && (GetFocus() == &m_edit || !m_bHotKey))
	{
		if (TypeInt == m_dateType)
		{
			m_edit.SetSel(0, -1);
		}
		else
		{
			int nLenth = strValue.GetLength();
			m_edit.SetSel(nLenth - 1, nLenth);
		}
		m_edit.SetFocus();
	}
}

CString CSimpleButtonEdit::TransDoubleToString(const double dValue, const int nSpot)
{
	CString snDouble;
	if (IS_DOUBLE_ZERO(dValue) || DBL_MAX_EDIT_BUTTON == dValue)
	{
		snDouble = "0";
	}
	else
	{
		switch (nSpot)
		{
		case 0:
			snDouble.Format("%.0f", dValue);
			break;
		case 1:
			snDouble.Format("%.1f", dValue);
			break;
		case 2:
			snDouble.Format("%.2f", dValue);
			break;
		case 3:
			snDouble.Format("%.3f", dValue);
			break;
		case 4:
			snDouble.Format("%.4f", dValue);
			break;
		case 5:
			snDouble.Format("%.5f", dValue);
			break;
		case 6:
			snDouble.Format("%.6f", dValue);
			break;
		case 7:
			snDouble.Format("%.7f", dValue);
			break;
		case 8:
			snDouble.Format("%.8f", dValue);
			break;
		case 9:
			snDouble.Format("%.9f", dValue);
			break;
		default:
			snDouble.Format("%.2f", dValue);
			break;
		}
	}

	return snDouble;
}

void CSimpleButtonEdit::SetFocus()
{
	if (m_bSetSel)
	{
		CString str;
		m_edit.GetWindowText(str);
		if (TypeInt == m_dateType)
		{
			m_edit.SetSel(0, -1);
		}
		else
		{
			int nLenth = str.GetLength();
			m_edit.SetSel(nLenth - 1, nLenth);
		}
	}	
	m_bHotKey = FALSE;
	m_edit.SetFocus();
}

BOOL CSimpleButtonEdit::PreTranslateMessage(MSG* pMsg)
{
	if (WM_MOUSEWHEEL == pMsg->message)
	{
		PlusMinus type = 0;
		int zDelta = GET_WHEEL_DELTA_WPARAM(pMsg->wParam);
		if (zDelta > 0)
		{
			type = Plus;
		}
		else if (zDelta < 0)
		{
			type = Minus;
		}
		if (m_EnableMouseWheel)
		{
			DoPlusMinus(type, m_fTick, m_nPrecision);
		}
		else
		{
			SendMouseWheelMsg(type);
		}
		return TRUE;
	}
	else if (WM_KEYDOWN == pMsg->message && VK_UP == pMsg->wParam)
	{
		DoPlusMinus(Plus, m_fTick, m_nPrecision);
		return TRUE;
	}
	else if (WM_KEYDOWN == pMsg->message && VK_DOWN == pMsg->wParam)
	{
		DoPlusMinus(Minus, m_fTick, m_nPrecision);
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CSimpleButtonEdit::SendMouseWheelMsg(PlusMinus type)
{
	NMHDR hdr;
	hdr.idFrom = GetDlgCtrlID();
	hdr.hwndFrom = GetSafeHwnd();
	if (Plus == type)
	{
		hdr.code = NM_RCLICK;	  //代表滑轮的上
	}
	else
	{
		hdr.code = NM_RDBLCLK;	  //代表滑轮的下
	}
	
	GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
}

void CSimpleButtonEdit::SetValue(PlusMinus type)
{
	DoPlusMinus(type, m_fTick, m_nPrecision);
}

void CSimpleButtonEdit::EnableWindow(BOOL bEnable /*= TRUE*/)
{
	m_edit.EnableWindow(bEnable);
	if (LeftRight == m_style)
	{
		m_btnLeft.EnableWindow(bEnable);
		m_btnRight.EnableWindow(bEnable);
	}
	else
	{
		m_btnDown.EnableWindow(bEnable);
		m_btnUp.EnableWindow(bEnable);
	}
}

BOOL CSimpleButtonEdit::IsWindowEnabled()
{
	return m_edit.IsWindowEnabled();
}

HWND CSimpleButtonEdit::GetFocusHwnd()
{
	return m_edit.GetSafeHwnd();
}

void CSimpleButtonEdit::OnEnChangeEdit()
{
	CString str;
	m_edit.GetWindowText(str);
	CString strHint;
	m_edit.GetHintText(strHint);
	if (str.Trim() != strHint.Trim() || 
		(str.Trim() == strHint.Trim() && strHint.IsEmpty())
		)
	{
		NMHDR hdr;
		hdr.idFrom = GetDlgCtrlID();
		hdr.hwndFrom = GetSafeHwnd();
		hdr.code = NM_CLICK;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
	}
}

void CSimpleButtonEdit::EnableSetSel(BOOL bEnable /*= TRUE*/)
{
	m_bSetSel = bEnable;
}

void CSimpleButtonEdit::SetDateType(DataTypeBtnEdit dataType)
{
	m_dateType = dataType;
	if (TypeDouble == m_dateType)
	{
		m_edit.EnableFloat();
	}
	else
	{
		m_edit.ModifyStyle(0, ES_NUMBER);
	}
}

void CSimpleButtonEdit::SetSize(int nEditWidth, int nBtnWidth, int nHeight, int nGap)
{
	m_nEditWidth = nEditWidth;
	m_nBtnLeftRightWidth = nBtnWidth;
	m_nHeight = nHeight;
	m_nGap = nGap;
}

void CSimpleButtonEdit::EnableHotkey(BOOL bEnable)
{
	m_bHotKey = bEnable;
}

void CSimpleButtonEdit::EnableMouseWheel(BOOL bEnable)
{
	m_EnableMouseWheel = bEnable;
}

void CSimpleButtonEdit::EnableMixZero(BOOL bEnable)
{
	m_bEnableMixZero = bEnable;
}


