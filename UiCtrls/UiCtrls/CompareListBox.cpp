#include "StdAfx.h"
#include "CompareListBox.h"
#include "CompareEdit.h"

CCompareListBox::CCompareListBox(void)
{
    m_nIndex = -1;
	m_nWidth				= 0;
	m_nHeight				= 0;
	m_szCheck				= "";
	m_pCompareDlg				= NULL;
    m_font.CreateFont(-12,0,0,0,
        FW_BOLD,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("Î¢ÈíÑÅºÚ"));
    m_font2.CreateFont(-12,0,0,0,
        FW_BOLD,
        TRUE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("Î¢ÈíÑÅºÚ"));

    m_nHistorty = 0;
}

CCompareListBox::~CCompareListBox(void)
{
}

void CCompareListBox::Init()
{


}


BEGIN_MESSAGE_MAP(CCompareListBox, CListBox)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CCompareListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nCount = GetCount();
	if(nCount>0)
	{
		GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_LISTBOX_OK, NULL);   
	}
    
	CListBox::OnLButtonUp(nFlags, point);
}


void CCompareListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);
}


void CCompareListBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CCompareListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

}

void CCompareListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int nCount = GetCount();
	if (nCount == 0)
	{
		return;
	}
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	switch ( lpDrawItemStruct->itemAction ) 
	{ 
	case ODA_SELECT: 
	case ODA_DRAWENTIRE: 
	case ODA_FOCUS:
		if ( lpDrawItemStruct->itemState & ODS_SELECTED ) 
		{ 
            pDC->FillSolidRect(&lpDrawItemStruct->rcItem,RGB(51,138,249));
		} 
		else
		{ 
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem,RGB(63, 65, 77));
		} 
	}  
	CRect rectFull = lpDrawItemStruct->rcItem;
	CString Text;
	CString strItem((LPCTSTR)lpDrawItemStruct->itemData);
	CString text;  
	GetText(lpDrawItemStruct->itemID, text);  
	UINT nIndex = lpDrawItemStruct->itemID;
	GetText(nIndex,Text);
    pDC->SetBkMode(TRANSPARENT);    
    if (lpDrawItemStruct->itemID < m_nHistorty)
    {
        pDC->SelectObject(&m_font2);
        pDC->SetTextColor(RGB(170, 170, 170));
    }
    else
    {
        pDC->SelectObject(&m_font);
        pDC->SetTextColor(RGB(215, 215, 215));
    }

    
	pDC->TextOut(rectFull.left + 8,rectFull.top+2,text);

}


void CCompareListBox::OnMouseMove(UINT nFlags, CPoint point)
{
//     if (GetFocus() != this)
//     {
//         SetFocus();
//     }
	int nCount = GetCount();
	for (int i = 0; i <nCount; i++)
	{
		RECT rc;
		this->GetItemRect(i,&rc);
		if(rc.top<point.y&&rc.bottom>point.y&&rc.left<point.x&&rc.right>point.x)
		{
            if (m_nIndex != i)
            {
                SetCurSel(i);
                m_nIndex = i;
            }
			
			break;
		}

	}

	CListBox::OnMouseMove(nFlags, point);
}

BOOL CCompareListBox::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        int nCount = GetCount();
        if(nCount>0)
        {
            GetParent()->SendMessage(UM_COMPAREDLG, UM_COMPAREDLG_LISTBOX_OK, NULL);   
        }
        
    }

    return CListBox::PreTranslateMessage(pMsg);
}


BOOL CCompareListBox::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}
