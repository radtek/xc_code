#pragma once

#define SIMPLETIME_CLASSNAME    _T("CSimpleTime")

#define EDITABLE_WHILE_CHECKED      0
#define EDITABLE_WHILE_UNCHECKED    1

typedef struct tagTimeCheckBox {
	NMHDR hdr;
	BOOL   bCheck;
} TimeCheckBox;

enum ST_TIME 
{
	HOUR = 0X0001,
	MINUTE,
	SECOND
};

// CSimpleTime

class CSimpleTime : public CWnd
{
	DECLARE_DYNAMIC(CSimpleTime)

public:
	CSimpleTime();
	virtual ~CSimpleTime();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetBkColor(COLORREF clr);
	void SetTextColor(COLORREF clrNormal, COLORREF clrDisable);
	void SetFont(HFONT font);
	void EnableWindow(BOOL bEnable = TRUE);

	void SetChecked(BOOL bChecked = FALSE);
	BOOL GetChecked();

	void SetBoxShow(BOOL bShow = TRUE);
	BOOL GetBoxShow();

	void SetMode(UINT mode = EDITABLE_WHILE_CHECKED);
	UINT GetMode();

	void GetTime(CTime &tm);
	void SetTime(CTime tm);

	CString GetTime();//add by yaoshulong 10.13.2017
	void SetTime(CString strTime);//add by yaoshulong 10.13.2017

private:
	BOOL RegisterWindowClass();

	void DrawUnchecked(CDC &dc, CRect rt, COLORREF clrA, COLORREF clrB, COLORREF clrC);
	void DrawChecked(CDC &dc, CRect rt, COLORREF clrA, COLORREF clrB);
private:
	COLORREF m_clrBk;  //控件背景色
	COLORREF m_clrTextNormal; //文本normal颜色      
	COLORREF m_clrTextDisabled;  //文本disable颜色

    CFont m_font;
	HFONT m_Font;

	int m_nHour;
	int m_nMin;
	int m_nSec;

	BOOL m_bEnable;
	BOOL m_bChecked;
	BOOL m_bBoxShow;
	UINT m_mode;

	ST_TIME m_time;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};