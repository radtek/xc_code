#pragma once
#include "afxwin.h"
class CSelectButtonListBox : public CListBox
{
public:
	CSelectButtonListBox(void);
	virtual ~CSelectButtonListBox(void);
public:
	void Init();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

public:
	unsigned int		m_nWidth;
	unsigned int		m_nHeight;
	CString				m_szCheck;
    CFont m_font;
    int m_nIndex;
    BOOL m_bMouseIn;

private:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

