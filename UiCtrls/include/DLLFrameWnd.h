#pragma once

// CDLLFrameWnd ¶Ô»°¿ò


#define DLLFrameWnd_CLASSNAME    _T("CDLLFrameWnd")

class CDLLFrameWnd : public CWnd
{
	DECLARE_DYNAMIC(CDLLFrameWnd)

public:
	CDLLFrameWnd();  
	virtual ~CDLLFrameWnd();
	BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);


protected:
	BOOL RegisterWindowClass();

	COLORREF m_color;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:

	void SetBkClr(COLORREF color);
	

};
