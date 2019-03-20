//*********************************************************
//*********** Created by YaoShulong 3.24.2017 *****************
//*********************************************************

#if !defined(AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CInPlaceFastkeyEdit : public CEdit
{
// Construction
public:
    CInPlaceFastkeyEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
 
// Operations
public:
     void EndEdit();
 
// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CInPlaceFastkeyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
 
// Implementation
public:
     virtual ~CInPlaceFastkeyEdit();
 
// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceFastkeyEdit)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;
    CRect   m_Rect;
   
public:
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
