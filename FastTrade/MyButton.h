#pragma once
#include "afxwin.h"
#include "resource.h"
class CMyButton :
	public CButton
{
public:
	CMyButton();
	~CMyButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

