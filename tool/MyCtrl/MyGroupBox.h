#pragma once

#include "afxwin.h"
class CMyGroupBox :
	public CButton
{
public:
	CMyGroupBox();
	~CMyGroupBox();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

