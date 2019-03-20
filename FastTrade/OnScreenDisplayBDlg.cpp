// OnScreenDisplayBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OnScreenDisplayBDlg.h"

// COnScreenDisplayBDlg 对话框

IMPLEMENT_DYNAMIC(COnScreenDisplayBDlg, CDialog)

COnScreenDisplayBDlg::COnScreenDisplayBDlg(UINT nTemplateIDD, COLORREF clrTransparent, CWnd* pParent)
	: CDialog(nTemplateIDD, pParent)
	, m_clrTransparent(clrTransparent)
{

}

COnScreenDisplayBDlg::~COnScreenDisplayBDlg()
{
}

void COnScreenDisplayBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COnScreenDisplayBDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// COnScreenDisplayBDlg 消息处理程序
BOOL COnScreenDisplayBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	VERIFY((dwStyle & WS_POPUP) != 0);
	VERIFY((dwStyle & WS_BORDER) == 0);
	VERIFY((dwStyle & WS_SIZEBOX) == 0);
	VERIFY((dwStyle & WS_DLGFRAME) == 0);

	m_brush.CreateSolidBrush(m_clrTransparent);
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	VERIFY((dwExStyle & WS_EX_APPWINDOW) == 0);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | 0x80000);

	HMODULE hInst = LoadLibrary(_T("User32.DLL")); 
	typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD); 
	MYFUNC SetLayeredWindowAttributes = NULL; 
	SetLayeredWindowAttributes = (MYFUNC)::GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
	SetLayeredWindowAttributes(GetSafeHwnd(), m_clrTransparent, 0, 1); 
	::FreeLibrary(hInst);

	return TRUE;
}

HBRUSH COnScreenDisplayBDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
			return m_brush;

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void COnScreenDisplayBDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* hParent = GetParent();
	if (hParent != NULL)
	{
		HWND hWnd = hParent->m_hWnd; 
		::SendMessage(hWnd, WM_CLOSE, 0, 0); 
	}

	CDialog::OnOK();
}

void COnScreenDisplayBDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* hParent = GetParent();
	if (hParent != NULL)
	{
		HWND hWnd = hParent->m_hWnd; 
		::SendMessage(hWnd, WM_CLOSE, 0, 0); 
	}

	CDialog::OnCancel();
}
