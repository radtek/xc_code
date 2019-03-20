#include "StdAfx.h"
#include "SelectButtonListBox.h"
#include "SimpleSelectButton.h"

CSelectButtonListBox::CSelectButtonListBox(void)
{
    m_bMouseIn = FALSE;
    m_nIndex = -1;
	m_nWidth				= 0;
	m_nHeight				= 0;
	m_szCheck				= "";
    m_font.CreateFont(-12,0,0,0,
        FW_BOLD,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("微软雅黑"));
}

CSelectButtonListBox::~CSelectButtonListBox(void)
{
}

void CSelectButtonListBox::Init()
{


}


BEGIN_MESSAGE_MAP(CSelectButtonListBox, CListBox)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CSelectButtonListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nCount = GetCount();
	if(nCount>0)
	{
		GetParent()->SendMessage(MESSAGE_DLG, MESSAGE_DLG_LISTBOX_CONFIRM, NULL);   
	}
    
	CListBox::OnLButtonUp(nFlags, point);
}


void CSelectButtonListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);
}


void CSelectButtonListBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CSelectButtonListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

}

void CSelectButtonListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
    pDC->SelectObject(&m_font);
	pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(215, 215, 215));
	pDC->TextOut(rectFull.left + 8,rectFull.top+2,text);

}


void CSelectButtonListBox::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseIn)
    {
        TRACKMOUSEEVENT csTME;
        csTME.cbSize = sizeof (csTME);    
        csTME.dwFlags = TME_LEAVE|TME_HOVER;    
        csTME.hwndTrack = GetSafeHwnd(); // 指定要 追踪 的窗口     
        csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER   
        ::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE 
        m_bMouseIn = TRUE;       
    }

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

BOOL CSelectButtonListBox::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        int nCount = GetCount();
        if(nCount>0)
        {
            GetParent()->SendMessage(MESSAGE_DLG, MESSAGE_DLG_LISTBOX_CONFIRM, NULL);   
        }
        
    }

    return CListBox::PreTranslateMessage(pMsg);
}


BOOL CSelectButtonListBox::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSelectButtonListBox::OnMouseLeave()
{
    m_nIndex = -1;
    m_bMouseIn = FALSE;
    SetCurSel(-1);
    Invalidate(FALSE);
    CWnd::OnMouseLeave();
}
