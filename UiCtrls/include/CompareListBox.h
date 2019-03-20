#pragma once
#include "afxwin.h"
class CCompareListBox : public CListBox
{
public:
	CCompareListBox(void);
	virtual ~CCompareListBox(void);
public:
	void Init();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

public:
	unsigned int		m_nWidth;
	unsigned int		m_nHeight;
	CString				m_szCheck;
	CWnd*			m_pCompareDlg;
    CFont m_font;
    CFont m_font2;
    int m_nIndex;
    int m_nHistorty;

private:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

