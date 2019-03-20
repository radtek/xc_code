
#include "stdafx.h"
#include "QdComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT QDM_ONCBNKILLFOCUS = ::RegisterWindowMessage (_T("QDM_ONCBNKILLFOCUS"));

/////////////////////////////////////////////////////////////////////////////
// CQdComboBox

UINT CQdComboBox::CbnKillfocusMessage()
{
	return QDM_ONCBNKILLFOCUS;
}

IMPLEMENT_DYNAMIC(CQdComboBox, CComboBox)

CQdComboBox::CQdComboBox()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
	m_bIsDroppedDown = FALSE;
	m_bIsButtonHighlighted = FALSE;
	m_rectBtn.SetRectEmpty ();
	m_bTracked = FALSE;
	m_clrPrompt = (COLORREF)-1;
	m_clrErrorText = (COLORREF)-1;
	m_bDefaultPrintClient = FALSE;
	m_nEditStyle = -1;
	m_hRcvMessageHwnd = NULL;

	m_attachPosition.RowIndex = -1;
	m_attachPosition.ColIndex = -1;
}

CQdComboBox::~CQdComboBox()
{
}

BEGIN_MESSAGE_MAP(CQdComboBox, CComboBox)
	//{{AFX_MSG_MAP(CQdComboBox)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_EDITUPDATE, OnEditupdate)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT_EX(CBN_KILLFOCUS, OnCbnKillFocus)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQdComboBox message handlers

//**************************************************************************
void CQdComboBox::SubclassEditBox()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (-1 == m_nEditStyle)
	{
		if (m_wndNormalEdit.GetSafeHwnd() != NULL)
			return;
	}
	else if (CBS_EDIT_POSITIVE_NUMBER == (m_nEditStyle & CBS_EDIT_MASK))
	{
		if (m_wndIntegerEdit.GetSafeHwnd() != NULL)
			return;
	}

	if ((GetStyle () & CBS_DROPDOWN))
	{
		CWnd* pWndChild = GetWindow (GW_CHILD);
		
		while (pWndChild != NULL)
		{
			ASSERT_VALID (pWndChild);
			
			if (CWnd::FromHandlePermanent (pWndChild->GetSafeHwnd ()) == NULL)
			{
				TCHAR lpszClassName [MAX_CLASS_NAME + 1];
				::GetClassName (pWndChild->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
				CString strClass = lpszClassName;
				
				if (strClass == EDIT_CLASS)
				{
					if (-1 == m_nEditStyle)
					{
						m_wndNormalEdit.SubclassWindow (pWndChild->GetSafeHwnd ());
					}
					else if (CBS_EDIT_POSITIVE_NUMBER == (m_nEditStyle & CBS_EDIT_MASK))
					{
						m_wndIntegerEdit.SubclassWindow (pWndChild->GetSafeHwnd ());
						m_wndIntegerEdit.AllowNegative(false);
					}

					break;
				}
			}
			
			pWndChild = pWndChild->GetNextWindow ();
		}
	}
}
//**************************************************************************
void CQdComboBox::OnNcPaint() 
{
	Default();
}
//**************************************************************************
void CQdComboBox::OnPaint() 
{
	if ((GetStyle () & 0x0003L) == CBS_SIMPLE)
	{
		Default ();
		return;
	}

	BOOL bDrawPrompt = FALSE;

	if (!m_strPrompt.IsEmpty() || !m_strErrorMessage.IsEmpty())
	{
		BOOL bTextIsEmpty = GetWindowTextLength() == 0;

		/*if (m_wndEdit.GetSafeHwnd () != NULL)
		{
			if (!m_strErrorMessage.IsEmpty())
			{
				//m_wndEdit.SetErrorMessage(m_strErrorMessage, m_clrErrorText);
			}
			else
			{
				//m_wndEdit.SetPrompt(bTextIsEmpty ? m_strPrompt : _T(""), m_clrPrompt);
			}
		}
		else
		{
			bDrawPrompt = bTextIsEmpty || !m_strErrorMessage.IsEmpty();
		}*/
	}

	if (!m_bVisualManagerStyle && !m_bOnGlass && !bDrawPrompt)
	{
		Default ();
		return;
	}

	CPaintDC dc(this); // device context for painting
	OnDraw(&dc, bDrawPrompt);
}
//**************************************************************************
void CQdComboBox::OnDraw(CDC* pDCIn, BOOL bDrawPrompt)
{
	
}
//**************************************************************************
BOOL CQdComboBox::OnCloseup() 
{
	m_bIsDroppedDown = FALSE;
	m_bIsButtonHighlighted = FALSE;

	RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return FALSE;
}
//**************************************************************************
BOOL CQdComboBox::OnDropdown() 
{
	if (m_bTracked)
	{
		ReleaseCapture ();
		m_bTracked = FALSE;
	}

	m_bIsDroppedDown = TRUE;
	m_bIsButtonHighlighted = FALSE;
	RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return FALSE;
}
//**************************************************************************
void CQdComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_LBUTTON) == 0)
	{
		BOOL bIsButtonHighlighted = m_bIsButtonHighlighted;
		m_bIsButtonHighlighted = m_rectBtn.PtInRect (point);

		if (bIsButtonHighlighted != m_bIsButtonHighlighted)
		{
			if (!m_bTracked)
			{
				if (m_bIsButtonHighlighted)
				{
					SetCapture ();
					m_bTracked = TRUE;
				}
			}
			else
			{
				if (!m_bIsButtonHighlighted)
				{
					ReleaseCapture ();
					m_bTracked = FALSE;
				}
			}
		
			RedrawWindow(m_rectBtn, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
	
	CComboBox::OnMouseMove(nFlags, point);
}
//*****************************************************************************************
void CQdComboBox::OnCancelMode() 
{
	CComboBox::OnCancelMode();
	
	if (m_bTracked)
	{
		ReleaseCapture ();
		m_bIsButtonHighlighted = FALSE;
		m_bTracked = FALSE;

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
}
//**************************************************************************
void CQdComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracked)
	{
		ReleaseCapture ();
		m_bIsButtonHighlighted = FALSE;
		m_bTracked = FALSE;

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
	
	CComboBox::OnLButtonDown(nFlags, point);
}
//**************************************************************************
void CQdComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	BOOL bDrawPrompt = (!m_strPrompt.IsEmpty() && GetWindowTextLength() == 0) || !m_strErrorMessage.IsEmpty();
	if (bDrawPrompt)
	{
		m_bIsButtonHighlighted = FALSE;
	}

	if (m_bVisualManagerStyle || bDrawPrompt)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}	
}
//**************************************************************************
void CQdComboBox::SetPrompt(LPCTSTR lpszPrompt, COLORREF clrText, BOOL bRedraw)
{
	ASSERT_VALID (this);

	CString strOldPrompt = m_strPrompt;
	BOOL bColorWasChanged = m_clrPrompt != clrText;

	m_strPrompt = (lpszPrompt == NULL) ? _T("") : lpszPrompt;
	m_clrPrompt = clrText;
	
	if (!m_strPrompt.IsEmpty())
	{
		SubclassEditBox();
	}

	//if (m_wndEdit.GetSafeHwnd () != NULL)
	{
		//m_wndEdit.SetPrompt(lpszPrompt, clrText, bRedraw);
	}

	if (bRedraw && GetSafeHwnd() != NULL && (bColorWasChanged || m_strPrompt != strOldPrompt))
	{
		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
	}
}
//**************************************************************************
void CQdComboBox::SetEditStyle(const int nStyle)
{
	ASSERT_VALID (this);
	m_nEditStyle = nStyle;
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (-1 == nStyle)
		return;
	
	if (!(GetStyle () & CBS_DROPDOWN))
		return;	

	SubclassEditBox();
}
//**************************************************************************
void CQdComboBox::SetErrorMessage(LPCTSTR lpszPrompt, COLORREF clrText, BOOL bRedraw)
{
	ASSERT_VALID (this);

	CString strOldPrompt = m_strErrorMessage;
	BOOL bColorWasChanged = m_clrErrorText != clrText;

	m_strErrorMessage = (lpszPrompt == NULL) ? _T("") : lpszPrompt;
	m_clrErrorText = clrText;
	
	if (!m_strErrorMessage.IsEmpty())
	{
		SubclassEditBox();
	}

	//if (m_wndEdit.GetSafeHwnd () != NULL)
	{
		//m_wndEdit.SetErrorMessage(lpszPrompt, clrText, bRedraw);
	}

	if (bRedraw && GetSafeHwnd() != NULL && (bColorWasChanged || m_strErrorMessage != strOldPrompt))
	{
		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
	}
}
//**************************************************************************
void CQdComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();

	if (!m_strPrompt.IsEmpty() || m_nEditStyle != -1)
	{
		SubclassEditBox();
		
		//if (m_wndEdit.GetSafeHwnd () != NULL)
		{
			//m_wndEdit.SetPrompt(m_strPrompt, m_clrPrompt, FALSE);
		}
	}
}
//**************************************************************************
int CQdComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_strPrompt.IsEmpty() || m_nEditStyle != -1)
	{
		SubclassEditBox();
	
		//if (m_wndEdit.GetSafeHwnd () != NULL)
		{
			//m_wndEdit.SetPrompt(m_strPrompt, m_clrPrompt, FALSE);
		}
	}
	
	return 0;
}
//**************************************************************************
void CQdComboBox::OnSetFocus(CWnd* pOldWnd) 
{
	CComboBox::OnSetFocus(pOldWnd);
	
	BOOL bDrawPrompt = (!m_strPrompt.IsEmpty() && GetWindowTextLength() == 0) || !m_strErrorMessage.IsEmpty();
	if (bDrawPrompt)
	{
		m_bIsButtonHighlighted = TRUE;
	}
	
	if (bDrawPrompt)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
}
//**************************************************************************
BOOL CQdComboBox::OnEditupdate() 
{
	if (!m_strErrorMessage.IsEmpty())
	{
		SetErrorMessage(NULL, m_clrErrorText);
	}

	return FALSE;
}
//**************************************************************************
BOOL CQdComboBox::OnSelchange() 
{
	if (!m_strErrorMessage.IsEmpty())
	{
		SetErrorMessage(NULL, m_clrErrorText);
	}

	if (m_bVisualManagerStyle)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return FALSE;
}
//**************************************************************************
LRESULT CQdComboBox::OnSetText (WPARAM, LPARAM)
{
	LRESULT lRes = Default();

	if (m_bVisualManagerStyle)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return lRes;
}
//**************************************************************************
LRESULT CQdComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lRes = CComboBox::WindowProc(message, wParam, lParam);

	if (message == CB_SETCURSEL && m_bVisualManagerStyle)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return lRes;
}
//*******************************************************************************
LRESULT CQdComboBox::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		if (m_bDefaultPrintClient)
		{
			return Default();
		}

		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);

		if ((GetStyle () & 0x0003L) == CBS_SIMPLE)
		{
			return Default ();
		}
		
		BOOL bDrawPrompt = FALSE;
		
		if (!m_strPrompt.IsEmpty() || !m_strErrorMessage.IsEmpty())
		{
			BOOL bTextIsEmpty = GetWindowTextLength() == 0;
			
			//if (m_wndEdit.GetSafeHwnd () == NULL)
			{
				bDrawPrompt = bTextIsEmpty || !m_strErrorMessage.IsEmpty();
			}
		}

		if (!m_bVisualManagerStyle && !bDrawPrompt)
		{
			return Default();
		}
		
		OnDraw(pDC, bDrawPrompt);
	}

	return 0;
}

BOOL CQdComboBox::OnCbnKillFocus()
{
	if (m_hRcvMessageHwnd != NULL && IsWindow(m_hRcvMessageHwnd))
	{
		::SendMessage(m_hRcvMessageHwnd, QDM_ONCBNKILLFOCUS,(WPARAM)this, (LPARAM)(&m_attachPosition));
	}

	return FALSE;
}
void CQdComboBox::SetAttachPosition(const ATTACH_POSITION &attachPosition)
{
	memcpy(&m_attachPosition, &attachPosition, sizeof(attachPosition));
}