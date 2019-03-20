#include "stdafx.h"
#include "UsedDockPane.h"

IMPLEMENT_DYNAMIC(CUsedDockPane, CSimpleDockPane)
CUsedDockPane::CUsedDockPane(void)
{
	m_nShowStyle = SW_SHOWNORMAL;
}

CUsedDockPane::~CUsedDockPane(void)
{
}

BEGIN_MESSAGE_MAP(CUsedDockPane, CSimpleDockPane)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/*
bShow
指定窗口是否显示。 如果窗口显示，它是 TRUE ;，如果窗口是隐藏的，它是 FALSE。

nStatus
指定显示的窗口的状态。 它是0，如果发送由于函数调用的 ShowWindow 成员;否则 nStatus 为下列之一：

SW_PARENTCLOSING 父窗口是结束(使图标样式)或弹出窗口隐藏。

SW_PARENTOPENING 父窗口打开(显示)或弹出窗口显示。
*/
void CUsedDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSimpleDockPane::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (!bShow && nStatus != 0)
	{
		WINDOWPLACEMENT wp;
		::memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(m_hWnd, &wp);
		m_nShowStyle = wp.showCmd;
	}
	else if(bShow && nStatus)
	{
		ShowWindow(m_nShowStyle);
	}
}
