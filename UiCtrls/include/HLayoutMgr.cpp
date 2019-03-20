#include "stdafx.h"
#include "HLayoutMgr.h"


CHLayoutMgr::CHLayoutMgr()
: m_nItemHeigh(20)
{
}


CHLayoutMgr::~CHLayoutMgr()
{
}

void CHLayoutMgr::SetRect(CRect *pRc)
{
	m_rect = *pRc;

	Layout();
}

void CHLayoutMgr::AddWnd(HWND hWnd, int nWidth, EM_HPOS pos)
{
	m_vctHwndInfo.push_back({ hWnd, pos, nWidth });
	
	Layout();
}

void CHLayoutMgr::Layout()
{
	if (m_rect.Width() <= 0)
		return;

	int offset = (m_rect.Height() - m_nItemHeigh) / 2;
	int y = m_rect.top;
	if (offset > 0)
		y += offset;

	int x = m_rect.left;
	for (int i = 0; i < m_vctHwndInfo.size(); i++)
	{
		HLayoutWndInfo &wndInfo = m_vctHwndInfo[i];
		if (HPOS_LEFT == wndInfo.pos)
			::MoveWindow(wndInfo.hWnd, m_rect.left, y, wndInfo.width, m_nItemHeigh, FALSE);
		else if (HPOS_RIGHT == wndInfo.pos)
			::MoveWindow(wndInfo.hWnd, m_rect.right - wndInfo.width, y, wndInfo.width, m_nItemHeigh, FALSE);
		else if (HPOS_CENTER == wndInfo.pos)
			::MoveWindow(wndInfo.hWnd, m_rect.left + (m_rect.Width() - wndInfo.width) / 2, y, wndInfo.width, m_nItemHeigh, FALSE);
		else
			::MoveWindow(wndInfo.hWnd, x, y, wndInfo.width, m_nItemHeigh, FALSE);

		x += wndInfo.width;
	}
}