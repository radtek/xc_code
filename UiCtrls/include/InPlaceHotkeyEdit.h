//////////////////////////////////////////////////////////////////////
// InPlaceEdit.h : header file
//
// MFC Grid Control - inplace editing class
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPLACEHOTKEYEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACEHOTKEYEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HotkeyEdit.h"

class CInPlaceHotkeyEdit : public CHotkeyEdit
{
// Construction
public:
    CInPlaceHotkeyEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
 
// Operations
public:
     void EndEdit();
 
// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CInPlaceHotkeyEdit)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
 
// Implementation
public:
     virtual ~CInPlaceHotkeyEdit();
 
// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceHotkeyEdit)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;
    CRect   m_Rect;
};
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEHOTKEYEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
