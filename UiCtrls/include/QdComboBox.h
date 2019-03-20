
#if !defined(__QDCOMBOBOX__)
#define __QDCOMBOBOX__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CQdComboBox window

#define	CBS_EDIT_POSITIVE_NUMBER	0x0001
#define	CBS_EDIT_NUMBER				0x0002
#define CBS_EDIT_MASK				0x000f

#define MAX_CLASS_NAME	255
#define EDIT_CLASS		_T("Edit")

class CQdComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CQdComboBox)
public:
	typedef struct 
	{
		int RowIndex;
		int ColIndex;
	} ATTACH_POSITION;
// Construction
public:
	CQdComboBox();

// Attributes
public:
	BOOL		m_bOnGlass;
	BOOL		m_bVisualManagerStyle;
	HWND		m_hRcvMessageHwnd;

protected:
	CAMSIntegerEdit	m_wndIntegerEdit;
	CEdit			m_wndNormalEdit;
	BOOL			m_bIsDroppedDown;
	CRect			m_rectBtn;
	BOOL			m_bIsButtonHighlighted;
	BOOL			m_bTracked;
	CString			m_strPrompt;
	COLORREF		m_clrPrompt;
	CString			m_strErrorMessage;
	COLORREF		m_clrErrorText;
	BOOL			m_bDefaultPrintClient;

	int				m_nEditStyle;

// Operations
public:
	void SetPrompt(LPCTSTR lpszPrompt, COLORREF clrText = (COLORREF)-1, BOOL bRedraw = TRUE);
	CString GetPrompt() const
	{
		return m_strPrompt;
	};
	
	void SetEditStyle(const int nStyle);

	void SetErrorMessage(LPCTSTR lpszMessage, COLORREF clrText = (COLORREF)-1, BOOL bRedraw = TRUE);
	CString GetErrorMessage() const
	{
		return m_strErrorMessage;
	}
	void SetAttachPosition(const ATTACH_POSITION &attachPosition);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQdComboBox)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void OnDraw(CDC* pDC, BOOL bDrawPrompt);

// Implementation
public:
	virtual ~CQdComboBox();

	static UINT CbnKillfocusMessage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CQdComboBox)
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg BOOL OnEditupdate();
	afx_msg BOOL OnSelchange();
	afx_msg BOOL OnCloseup();
	afx_msg BOOL OnDropdown();
	afx_msg BOOL OnCbnKillFocus();
	
	afx_msg LRESULT OnSetText (WPARAM, LPARAM);
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	void SubclassEditBox();

	ATTACH_POSITION m_attachPosition;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__QDCOMBOBOX__)
