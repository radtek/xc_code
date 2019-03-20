#pragma once

#include "SimpleCalendar.h"

#define SIMPLEDATETIMEPITCKER_CLASSNAME    _T("CSimpleDateTimePicker")
// CSimpleDateTimePicker

class CSimpleDateTimePicker : public CWnd
{
	DECLARE_DYNAMIC(CSimpleDateTimePicker)

public:
	CSimpleDateTimePicker();
	virtual ~CSimpleDateTimePicker();
	BOOL Create(UINT dwStyle, const CRect &rect, CWnd *pParentWnd, UINT nID);
	void EnableWindow(BOOL bEnable);
	void SetBkColor(COLORREF clr);
	void SetTextColor(COLORREF clrNormal, COLORREF clrDisable);
	void SetMonthCalColor(int nRegion, COLORREF ref);
	void SetBeExpand(BOOL bExpand);
	bool GetTime(CTime &tm);
	void SetDate(CTime tm);
	void SetYear(int year);
	void SetMonth(int month);
	void SetDay(int day);
    CString GetTime();//add by yaoshulong 10.13.2017
private:
	BOOL RegisterWindowClass();
private:
	COLORREF m_clrBk;  //控件背景色
	COLORREF m_clrTextNormal; //文本normal颜色      
	COLORREF m_clrTextDisabled;  //文本disable颜色
	CFont m_font;
	CBrush m_brBk;  //用于绘制背景的刷子

	BOOL m_bExpand;
	BOOL m_bEnable;

	CSimpleCalendar *m_pCalendar;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


