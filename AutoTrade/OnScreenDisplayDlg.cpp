// OnScreenDisplayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OnScreenDisplayDlg.h"

// COnScreenDisplayDlg 对话框

IMPLEMENT_DYNAMIC(COnScreenDisplayDlg, COnScreenDisplayBDlg)

COnScreenDisplayDlg::COnScreenDisplayDlg(CWnd* pParent /*=NULL*/)
	: COnScreenDisplayBDlg(COnScreenDisplayDlg::IDD, RGB(0, 255, 0), pParent)
{
	m_nElapseMs = 1000;
	m_tipColor = RGB(0,254,0);

	this->Create(COnScreenDisplayDlg::IDD, GetDesktopWindow());
	
	m_tipFont.CreateFont(
		50, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_NORMAL, // nWeight 
        FALSE, // bItalic 
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        _T("微软雅黑"));

	m_lbTip.SetFont(&m_tipFont);
}
COnScreenDisplayDlg::~COnScreenDisplayDlg()
{
}

void COnScreenDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	COnScreenDisplayBDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COnScreenDisplayDlg, COnScreenDisplayBDlg)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// COnScreenDisplayDlg 消息处理程序

void COnScreenDisplayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case IDC_MYTIMERID:
		this->ShowWindow(SW_HIDE);
		KillTimer(IDC_MYTIMERID);
		break;
	default:
	    break;
	}

	COnScreenDisplayBDlg::OnTimer(nIDEvent);
}

void COnScreenDisplayDlg::OnSize(UINT nType, int cx, int cy)
{
	COnScreenDisplayBDlg::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustWindow();
}

void COnScreenDisplayDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	COnScreenDisplayBDlg::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if(bShow)
	{
		SetTimer(IDC_MYTIMERID, m_nElapseMs, NULL);
	}
}

HBRUSH COnScreenDisplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COnScreenDisplayBDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if(CTLCOLOR_STATIC == nCtlColor) // IDC_STATIC_TIP
	{
		pDC->SetTextColor(m_tipColor);
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

BOOL COnScreenDisplayDlg::OnInitDialog()
{
	COnScreenDisplayBDlg::OnInitDialog();

	// 隐藏任务栏图标
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	m_lbTip.Create(m_strTip, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDC_LAB_TIPWORD);

	AdjustWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void COnScreenDisplayDlg::ResizeWindow(CString strTip)
{
	if(!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	CDC *pDC = GetDC();
	if(nullptr == pDC)
	{
		return;
	}

	HGDIOBJ pOld = pDC->SelectObject(m_lbTip.GetFont()->GetSafeHandle());
	pDC->SelectObject(m_lbTip.GetFont());
	CSize sz = pDC->GetTextExtent(strTip);
	pDC->SelectObject(pOld);

	CRect cr;
	GetClientRect(&cr);
	ClientToScreen(&cr);
	cr.bottom = cr.top + sz.cy;
	cr.right = cr.left + sz.cx;
	MoveWindow(cr);
    ReleaseDC(pDC);
}
void COnScreenDisplayDlg::AdjustWindow()
{
	CRect clientRect;
	GetClientRect(clientRect);
	clientRect.right = clientRect.Width();
	clientRect.bottom = clientRect.Height();

	if (IsWindow(m_lbTip.GetSafeHwnd()))
	{
		m_lbTip.MoveWindow(clientRect);
	}
}
void COnScreenDisplayDlg::TipWord(CString strTip, CWnd* pParent /*= GetDesktopWindow()*/, int nElapseMs /*= 1000*/, COLORREF tipColor /*= RGB(0,254,0)*/)
{
	m_nElapseMs = nElapseMs;
	if(1000 > m_nElapseMs)
	{
		m_nElapseMs = 1000;
	}
	if(m_tipColor != tipColor)
	{
		m_tipColor = tipColor;
	}

	ShowWindow(SW_HIDE);
	m_strTip = strTip;

	ResizeWindow(m_strTip);
	
	if(m_lbTip.GetSafeHwnd())
	{
		m_lbTip.SetWindowText(m_strTip);
	}

	this->CenterWindow(pParent); // 在指定窗口居中显示
	ShowWindow(SW_SHOWNOACTIVATE);
}