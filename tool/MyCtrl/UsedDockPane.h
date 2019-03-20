#pragma once
#include "SimpleDockPane.h"
class CUsedDockPane :
	public CSimpleDockPane
{
	DECLARE_DYNAMIC(CUsedDockPane)
public:
	CUsedDockPane(void);
	~CUsedDockPane(void);
private:
	UINT	m_nShowStyle;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

