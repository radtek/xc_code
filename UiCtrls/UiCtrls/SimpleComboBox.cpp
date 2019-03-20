//*****************************************************
//********    SimpleComboBox By YaoShuLong 2017.4
//*****************************************************

#include "stdafx.h"

#include "SimpleComboBox.h"
#include "..\resource.h"

HFONT CSimpleComboBox::m_hFont = NULL;

// CSimpleComboBox

IMPLEMENT_DYNAMIC(CSimpleComboBox, CComboBox)

CSimpleComboBox::CSimpleComboBox()
{
    m_nDropCount = 0;
    m_bDropDownReapaint = TRUE;
    m_bReapaint = TRUE;
    m_bEnable = TRUE;
    m_clrBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrDisabledText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrBk);
    //m_font.CreateFont(-12,0,0,0,
    //    FW_BOLD,
    //    FALSE,FALSE,
    //    FALSE, 
    //    DEFAULT_CHARSET,
    //    OUT_DEFAULT_PRECIS,
    //    CLIP_DEFAULT_PRECIS,
    //    PROOF_QUALITY,
    //    DEFAULT_PITCH, 
    //    _T("微软雅黑"));

    //m_hFont = (HFONT)m_font.GetSafeHandle();

}

CSimpleComboBox::~CSimpleComboBox()
{
}


BEGIN_MESSAGE_MAP(CSimpleComboBox, CComboBox)
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSimpleComboBox 消息处理程序


void CSimpleComboBox::OnPaint()
{
    CPaintDC dc(this); 
    CRect rCl;
    GetClientRect(&rCl);

    CDC dcMem1;
    CBitmap bmpMem1;
    CBitmap* pOldBmp1=NULL;

    if(dcMem1.CreateCompatibleDC(&dc))
    {
        if(bmpMem1.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
            pOldBmp1 = dcMem1.SelectObject(&bmpMem1);
        else
            return;
    }
    else
        return;

    dcMem1.FillSolidRect(&rCl, m_clrBk);

    CFont* pFontOld = dcMem1.SelectObject(GetFont());
    COLORREF clrBkOld = dcMem1.SetBkColor(m_clrBk);
    COLORREF clrBkTextOld = dcMem1.SetTextColor(m_bEnable ? m_clrText : m_clrDisabledText);

    CRect rectEx;
    rectEx.SetRect(4, 0, rCl.right - 20, rCl.bottom);
    CString string;
    GetWindowText(string);
    dcMem1.DrawText(string, &rectEx, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

    dcMem1.SelectObject(pFontOld);
    dcMem1.SetBkColor(clrBkOld);
    dcMem1.SetTextColor(clrBkTextOld);

    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem1, 0, 0, SRCCOPY);
    dcMem1.SelectObject(pOldBmp1);
    dcMem1.DeleteDC();
    bmpMem1.DeleteObject();
    ///***********************************************************************************************
	//按钮 
	CRect rcBitMap(rCl);
	COLORREF clrBorder;
	COLORREF clrInside;
	COLORREF clrPoint;

	if (m_bEnable)
	{
		clrBorder = RGB(169, 170, 175);
		clrInside = RGB(245, 245, 245);
		clrPoint = RGB(158, 159, 165);
	}
	else
	{
		clrBorder = RGB(94, 94, 98);
		clrInside = RGB(143, 143, 146);
		clrPoint = RGB(94, 94, 98);
	}
	//1
	rcBitMap.left = rCl.right - 18;
	rcBitMap.right = rcBitMap.left + 10;
	rcBitMap.top = 11;//11
	rcBitMap.bottom = rcBitMap.top + 4;//15

	CPen pen1(PS_SOLID, 1, clrBorder);
	dc.SelectObject(pen1);
	dc.MoveTo(rcBitMap.left + 2, rcBitMap.top + 1);
	dc.LineTo(rcBitMap.left + 5, rcBitMap.bottom);
	dc.LineTo(rcBitMap.left + 6, rcBitMap.bottom);
	dc.LineTo(rcBitMap.right, rcBitMap.top);

	//2
	CPen pen2(PS_SOLID, 1,  clrPoint);
	dc.SelectObject(pen2);
	dc.MoveTo(rcBitMap.left + 1, rcBitMap.top);
	dc.LineTo(rcBitMap.right + 1, rcBitMap.top);

	//2
	CPen pen3(PS_SOLID, 1,  clrInside);
	dc.SelectObject(pen3);
	dc.MoveTo(rcBitMap.left + 2, rcBitMap.top);
	dc.LineTo(rcBitMap.right, rcBitMap.top);

	dc.MoveTo(rcBitMap.left + 3, rcBitMap.top + 1);
	dc.LineTo(rcBitMap.right - 1, rcBitMap.top + 1);

	dc.MoveTo(rcBitMap.left + 4, rcBitMap.top + 2);
	dc.LineTo(rcBitMap.right - 2, rcBitMap.top + 2);

	dc.MoveTo(rcBitMap.left + 5, rcBitMap.top + 3);
	dc.LineTo(rcBitMap.right - 3, rcBitMap.top + 3);
    //****************************************************************************************
    CDC dcMem2;
    CBitmap bmpMem2;
    CBitmap* pOldBmp2=NULL;
    CRect rect;
    rect.SetRect(0, 0, rCl.Width(), 1);
    if(dcMem2.CreateCompatibleDC(&dc))
    {
        if(bmpMem2.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height()))
            pOldBmp2 = dcMem2.SelectObject(&bmpMem2);
        else
            return;
    }
    else
        return;

    dcMem2.FillSolidRect(&rect, RGB(18, 20, 22));


    if (GetDroppedState())
    {

        if (m_bDropDownReapaint)
        {
            m_nDropCount ++;
            if (2 == m_nDropCount)
            {
                Invalidate(FALSE);

            }
            if (3 == m_nDropCount)
            {
                m_bReapaint = TRUE;
                m_bDropDownReapaint = FALSE;
                dc.BitBlt(rCl.left,  rCl.bottom - 1, rCl.Width(), 1, &dcMem2, 0, 0, SRCCOPY);  
            }


        }

    }
    else
    {
        if (m_bReapaint)
        {
         //   Invalidate(FALSE);
            CRect rect;
            rect.SetRect(rCl.left,  rCl.bottom - 1, rCl.right, rCl.bottom);
            InvalidateRect(&rect, FALSE);
            m_bDropDownReapaint = TRUE;
            m_nDropCount = 0;
            m_bReapaint = FALSE;
        }

    }
    dcMem2.SelectObject(pOldBmp2);
    dcMem2.DeleteDC();
    bmpMem2.DeleteObject();
}

HBRUSH CSimpleComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);   
    pDC->SetBkColor(m_clrBk );  
    return (HBRUSH)m_brushBk.GetSafeHandle();  
}

HBRUSH CSimpleComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);   
    pDC->SetBkColor(m_clrBk );  
    return (HBRUSH)m_brushBk.GetSafeHandle();  
}

BOOL CSimpleComboBox::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleComboBox::EnableWindow(BOOL bEnable)
{
    m_bEnable = bEnable;
    Invalidate(FALSE);
    CComboBox::EnableWindow(bEnable);
}

void CSimpleComboBox::SetBkColor(COLORREF clr)
{
    m_clrBk = clr;
    if (m_brushBk.GetSafeHandle())
    {
        m_brushBk.DeleteObject();
    }
    m_brushBk.CreateSolidBrush(m_clrBk);
    Invalidate(FALSE);
}

void CSimpleComboBox::SetTextColor(COLORREF clr)
{
    m_clrText = clr;
    Invalidate(FALSE);
}

void CSimpleComboBox::SetDisabledTextColor(COLORREF clr)
{
    m_clrDisabledText = clr;
    Invalidate(FALSE);
}

void CSimpleComboBox::SetFont(CFont* pFont, BOOL bRedraw)
{
    if (NULL == pFont)
    {
        CComboBox::SetFont(CFont::FromHandle(m_hFont), bRedraw);
    }
    else
    {
        CComboBox::SetFont(pFont, bRedraw);
    }
}

void CSimpleComboBox::SetFontEx(CFont* pFont)
{
	SetFont(pFont, FALSE);
    Invalidate(FALSE);
}

int CSimpleComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_hFont != NULL)
	{
		SetFont(CFont::FromHandle(m_hFont), FALSE);
	}

	return 0;
}

void CSimpleComboBox::SetFont(HFONT hFont)
{
	m_hFont = hFont;
}