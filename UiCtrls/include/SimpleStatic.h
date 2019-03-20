
//*****************************************************
//********    SimpleStatic By YaoShuLong 2017.2
//*****************************************************

#pragma once
// CSimpleStatic

#define SimpleStatic_CLASSNAME    _T("CSimpleStatic")

class CSimpleStatic : public CWnd
{
	DECLARE_DYNAMIC(CSimpleStatic)

public:
	CSimpleStatic();
	virtual ~CSimpleStatic();

	BOOL Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);


private:
	static HFONT m_hFont;
    COLORREF m_clrText;
	COLORREF m_clrTextDisabled;

    COLORREF m_clrBk;
	BOOL m_bEnable;
	CFont m_font;
	BOOL m_bLMouseDown;
	BOOL m_bEnableClick;

	BOOL RegisterWindowClass();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
    
public:
    void SetTextColor(COLORREF color);
    void SetBkColor(COLORREF color);   
	void SetWindowText(CString str);
	static void SetFont(HFONT hFont);
	void EnableWindow(BOOL bEnable = TRUE);
	void SetTextDisabledColor(COLORREF color);
	void EnableClick(BOOL bEnable = TRUE);
};


