// GridCellCombo.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
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
// For use with CGridCtrl v2.22+
//
// History:
// 6 Aug 1998 - Added CComboEdit to subclass the edit control - code 
//              provided by Roelf Werkman <rdw@inn.nl>. Added nID to 
//              the constructor param list.
// 29 Nov 1998 - bug fix in onkeydown (Markus Irtenkauf)
// 13 Mar 2004 - GetCellExtent fixed by Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "GridCellButtonEdit.h"
#include "InPlaceButtonEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGridCellButtonEdit 
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CGridCellButtonEdit, CGridCell)

CGridCellButtonEdit::CGridCellButtonEdit() : CGridCell()
{
}

BOOL CGridCellButtonEdit::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    if ( m_bEditing )
    {      
        if (m_pEditWnd)
            m_pEditWnd->SendMessage ( WM_CHAR, nChar );    
    }  
    else  
    {   
        DWORD dwStyle = ES_LEFT;

        m_bEditing = TRUE;

        // InPlaceEdit auto-deletes itself
        CGridCtrl* pGrid = GetGrid();
		m_pEditWnd = new CInPlaceButtonEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetText(), nChar, GetTick(), GetPrecision(), GetDateType());
    }
    return TRUE;
}
// Cancel the editing.
void CGridCellButtonEdit::EndEdit()
{
    if (m_pEditWnd)
		((CInPlaceButtonEdit*)m_pEditWnd)->EndEdit();
}

