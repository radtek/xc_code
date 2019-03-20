/////////////////////////////////////////////////////////////////////////////
// GridCell.h : header file
//
// MFC Grid Control - Grid cell class header file
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
// For use with CGridCtrl v2.20+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGridCtrl;
#include "GridCellBase.h"

// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCell : public CGridCellBase
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCell)

// Construction/Destruction
public:
    CGridCell();
    virtual ~CGridCell();

// Attributes
public:
    void operator=(const CGridCell& cell);

    virtual void  SetText(LPCTSTR szText)        { m_strText = szText;  }         
    virtual void SetEditable(BOOL bEditable)       {m_bEditable = bEditable;}//add by yaoshulong 11.29.2017
	//add by yaoshulong 2018.6.21
	virtual void SetTick(double fTick)       { m_fTick = fTick; }
	virtual double GetTick() const  			{ return m_fTick; }
	virtual void SetPrecision(int nPrecision)       { m_nPrecision = nPrecision; }
	virtual int GetPrecision() const  			{ return m_nPrecision; }

	virtual void SetDateType(int nDateType)       { m_nDateType = nDateType; }
	virtual int GetDateType() const  			{ return m_nDateType; }

    virtual void  SetImage(int nImage)           { m_nImage = nImage;   }                        
    virtual void  SetData(LPARAM lParam)         { m_lParam = lParam;   }      
    virtual void  SetGrid(CGridCtrl* pGrid)      { m_pGrid = pGrid;     }                          
    // virtual void SetState(const DWORD nState);  -  use base class version   
    virtual void  SetFormat(DWORD nFormat)       { m_nFormat = nFormat; }                      
    virtual void  SetTextClr(COLORREF clr)       { m_crFgClr = clr;     }                          
    virtual void  SetBackClr(COLORREF clr)       { m_crBkClr = clr;     }                          
    virtual void  SetFont(const LOGFONT* plf);
    virtual void  SetMargin(UINT nMargin)        { m_nMargin = nMargin; }
    virtual CWnd* GetEditWnd() const             { return m_pEditWnd;   }
    virtual void  SetCoords(int /*nRow*/, int /*nCol*/) {}  // don't need to know the row and
                                                            // column for base implementation

    virtual LPCTSTR     GetText() const             { return (m_strText.IsEmpty())? _T("") : LPCTSTR(m_strText); }
    virtual BOOL         GetEditable() const            { return m_bEditable;  }//add by yaoshulong 11.29.2017
    virtual int         GetImage() const            { return m_nImage;  }
    virtual LPARAM      GetData() const             { return m_lParam;  }
    virtual CGridCtrl*  GetGrid() const             { return m_pGrid;   }
    // virtual DWORD    GetState() const - use base class
    virtual DWORD       GetFormat() const;
    virtual COLORREF    GetTextClr() const          { return m_crFgClr; } // TODO: change to use default cell
    virtual COLORREF    GetBackClr() const          { return m_crBkClr; }
    virtual LOGFONT*    GetFont() const;
    virtual CFont*      GetFontObject() const;
    virtual UINT        GetMargin() const;

    virtual BOOL        IsEditing() const           { return m_bEditing; }
    virtual BOOL        IsDefaultFont() const       { return (m_plfFont == NULL); }
    virtual void        Reset();
	
	// add by yangyong 20170328
	virtual COLORREF    GetBackClr2() const          { return m_crBkClr2; }
	virtual void  SetBackClr(COLORREF clr1, COLORREF clr2)       { m_crBkClr = clr1; m_crBkClr2 = clr2;     }

// editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
    virtual void OnEndEdit();

protected:
    CString    m_strText;      // Cell text (or binary data if you wish...)
    BOOL m_bEditable;
	double m_fTick;
	int m_nPrecision;
	int m_nDateType;

    LPARAM     m_lParam;       // 32-bit value to associate with item
    int        m_nImage;       // Index of the list view item’s icon
    DWORD      m_nFormat;
    COLORREF   m_crFgClr;
    COLORREF   m_crBkClr;
    LOGFONT*   m_plfFont;
    UINT       m_nMargin;

    BOOL       m_bEditing;     // Cell being edited?

    CGridCtrl* m_pGrid;        // Parent grid control
    CWnd*      m_pEditWnd;	

	// add by yangyong 20170328
	COLORREF   m_crBkClr2;
};

// This class is for storing grid default values. It's a little heavy weight, so
// don't use it in bulk 
class CGridDefaultCell : public CGridCell
{
    DECLARE_DYNCREATE(CGridDefaultCell)

// Construction/Destruction
public:
    CGridDefaultCell();
    virtual ~CGridDefaultCell();

public:
    virtual DWORD GetStyle() const                      { return m_dwStyle;      }
    virtual void  SetStyle(DWORD dwStyle)               { m_dwStyle = dwStyle;   }
    virtual int   GetWidth() const                      { return m_Size.cx;      }
    virtual int   GetHeight() const                     { return m_Size.cy;      }
    virtual void  SetWidth(int nWidth)                  { m_Size.cx = nWidth;    }
    virtual void  SetHeight(int nHeight)                { m_Size.cy = nHeight;   }

    // Disable these properties
    virtual void     SetData(LPARAM /*lParam*/)             { ASSERT(FALSE);         }      
    virtual void     SetState(DWORD /*nState*/)             { ASSERT(FALSE);         }
    virtual DWORD    GetState() const                       { return CGridCell::GetState()|GVIS_READONLY; }
    virtual void     SetCoords( int /*row*/, int /*col*/)   { ASSERT(FALSE);         }
    virtual void     SetFont(const LOGFONT* /*plf*/);
    virtual LOGFONT* GetFont() const;   
    virtual CFont*   GetFontObject() const;

	// add by yangyong 20170421
	virtual HBRUSH GetBackBrush1() const          { return m_hBkBrush1; }
	virtual HBRUSH GetBackBrush2() const          { return m_hBkBrush2; }
	virtual void  SetBackBrush1(HBRUSH brush)       
	{ 
		if (m_hBkBrush1 != NULL && m_hBkBrush1 != brush)
		{
			DeleteObject(m_hBkBrush1);
		}

		m_hBkBrush1 = brush;
	}

	virtual void  SetBackBrush2(HBRUSH brush)       
	{ 
		if (m_hBkBrush2 != NULL && m_hBkBrush2 != brush)
		{
			DeleteObject(m_hBkBrush2);
		}

		m_hBkBrush2 = brush;
	}

	virtual void  SetBackClr(COLORREF clr)       
	{ 
		CGridCell::SetBackClr(clr);
		if (m_hBkBrush1 != NULL)
		{
			DeleteObject(m_hBkBrush1);
		}

		m_hBkBrush1 = CreateSolidBrush(clr);
	} 

	virtual void  SetBackClr(COLORREF clr1, COLORREF clr2)
	{
		CGridCell::SetBackClr(clr1, clr2);
		if (m_hBkBrush1 != NULL)
		{
			DeleteObject(m_hBkBrush1);
		}

		m_hBkBrush1 = CreateSolidBrush(clr1);

		if (m_hBkBrush2 != NULL)
		{
			DeleteObject(m_hBkBrush2);
		}

		m_hBkBrush2 = CreateSolidBrush(clr2);
	}

protected:
    CSize m_Size;       // Default Size
    CFont m_Font;       // Cached font
    DWORD m_dwStyle;    // Cell Style - unused

	// add by yangyong 20170421
	HBRUSH   m_hBkBrush1;
	HBRUSH   m_hBkBrush2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
