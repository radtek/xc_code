#pragma once
#include "../share/define.h"

class CMyListCtrl : public CListCtrl
{
public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	HWND hwnd_WarningPool;
	unsigned int itemIndex;
public:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	void init_HwndWarningPool(HWND hwnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
