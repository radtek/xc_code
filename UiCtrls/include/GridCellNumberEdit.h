#if !defined(__GRIDCELLNUMBEREDIT_H__)
#define __GRIDCELLNUMBEREDIT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GridCellCombo.h : header file
//
// MFC Grid Control - Grid combo cell class header file
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
// For use with CGridCtrl v2.10
//
//////////////////////////////////////////////////////////////////////


#include "GridCell.h"


class CGridCellNumberEdit : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellNumberEdit)

public:
    CGridCellNumberEdit();

    // editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__GRIDCELLNUMBEREDIT_H__)
