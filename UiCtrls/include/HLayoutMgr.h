#pragma once
#include <vector>
#include "QdUiLib.h"

using std::vector;

class CHLayoutMgr
{
	
public:
	enum EM_HPOS
	{
		HPOS_LEFT = 0,
		HPOS_RIGHT,
		HPOS_CENTER,
		HPOS_DEFAULT,
	};

	typedef struct
	{
		HWND hWnd;
		EM_HPOS pos;
		int width;
	} HLayoutWndInfo;

	CHLayoutMgr();
	~CHLayoutMgr();

	void SetRect(CRect *pRc);
	
	void AddWnd(HWND hWnd, int nWidth, EM_HPOS pos);

private:
	void Layout();
private:
	vector<HLayoutWndInfo> m_vctHwndInfo;
	CRect m_rect;
	int m_nItemHeigh;

};

