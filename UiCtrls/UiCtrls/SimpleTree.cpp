
//*****************************************************
//********    SimpleTree By YaoShuLong 2018.12
//*****************************************************

#include "stdafx.h"
#include "SimpleTree.h"

// CSimpleTree

#define HeightBtn  25
#define BtnSubYGap  12
#define TopGapSub  15

#define SubBtnWidth  75

#define FontStandard  "标准"
#define FontMedium  "中等"
#define FontBig          "大号"


IMPLEMENT_DYNAMIC(CSimpleTree, CWnd)

CSimpleTree::CSimpleTree() :
	m_nIDBase(IDC_START_BASE),
	m_nIDSub(IDC_START_SUB),
	m_nTopGap(50)
{
	SetFontStyle(FontStandard);
}

CSimpleTree::~CSimpleTree()
{
}

BEGIN_MESSAGE_MAP(CSimpleTree, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_RANGE(NM_CLICK, IDC_START_BASE, ID_END_SUB, &CSimpleTree::OnBtn)
END_MESSAGE_MAP()

// CSimpleTree 消息处理程序

BOOL CSimpleTree::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!RegisterWindowClass())
	{
		return FALSE;
	}

    return CWnd::Create(SimpleTree_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleTree::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleTree_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleTree_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

int CSimpleTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


BOOL CSimpleTree::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSimpleTree::SetTreeData(CString strBase, vector<CString>& vecStrSub, CString strSelected, BOOL bEnableSubTab, BOOL bClose)
{
	SharedPtrBtn pBtnBase = make_shared<CSimpleButton>();
	pBtnBase->Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ++m_nIDBase, GetCaption(bClose, strBase), DT_LEFT, FALSE, FALSE, FALSE, FALSE);
	pBtnBase->SetRgnDefColor(RGB(54, 54, 54));
	pBtnBase->SetRgnHoverColor(RGB(70, 70, 70));
	pBtnBase->SetChangeCursor(TRUE);
	map<UINT, SharedPtrBtn> mapBtnSub;
	for (auto data : vecStrSub)
	{
		SharedPtrBtn pBtnSub = make_shared<CSimpleButton>();
		pBtnSub->Create(WS_CHILD, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ++m_nIDSub, data);
		pBtnSub->EnableTab(bEnableSubTab);
		if (!bEnableSubTab)
		{
			pBtnSub->SetRgnDefColor(RGB(18, 20, 22));
			pBtnSub->SetRgnHoverColor(RGB(61, 91, 153));
		}
		else if (strSelected.Trim() == data.Trim())
		{
			pBtnSub->SetSelected(TRUE);
		}
		mapBtnSub.insert(make_pair(m_nIDSub, pBtnSub));
	}

	CTreeData treeData;
	treeData.bClose = bClose;
	treeData.bTab = bEnableSubTab;
	treeData.pBtnBase = pBtnBase;
	treeData.strCaption = strBase;
	treeData.mapBtnSub = mapBtnSub;
	m_mapTreeData.insert(make_pair(m_nIDBase, treeData));
}

void CSimpleTree::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rect;
	ReLayout(0, rect);
}

void CSimpleTree::OnPaint()
{
	CPaintDC dc(this);

	CRect rCl;
	GetClientRect(&rCl);

	CDC dcMem;
	CBitmap bmpMem;
	CBitmap* pOldBmp = NULL;

	if (dcMem.CreateCompatibleDC(&dc))
	{
		if (bmpMem.CreateCompatibleBitmap(&dc, rCl.Width(), rCl.Height()))
		{
			pOldBmp = dcMem.SelectObject(&bmpMem);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	dcMem.FillSolidRect(&rCl, RGB(54, 54, 54));
	CRect rectTop;
	rectTop.left = 0;
	rectTop.top = m_nTopGap - 1;
	rectTop.right = rCl.right;
	rectTop.bottom = m_nTopGap;
	dcMem.FillSolidRect(&rectTop, RGB(18, 20, 22));

	for (auto iter = m_mapTreeData.begin(); iter != m_mapTreeData.end(); iter++)
	{
		dcMem.FillSolidRect(&(iter->second.rectSub), RGB(18, 20, 22));
	}

	dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();

}

void CSimpleTree::OnBtn(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID > IDC_START_BASE && nID < ID_END_BASE)
	{
		int nIndex = 0;
		for (auto iter = m_mapTreeData.begin(); iter != m_mapTreeData.end(); iter ++)
		{
			if (iter->first == nID)
			{
				iter->second.bClose = !iter->second.bClose;
				if (iter->second.pBtnBase != nullptr && ::IsWindow(iter->second.pBtnBase->GetSafeHwnd()))
				{
					iter->second.pBtnBase->SetWindowText(GetCaption(iter->second.bClose, iter->second.strCaption));
					CRect rect;
					ReLayout(nIndex, rect);
					//Invalidate(FALSE);
					InvalidateRect(&rect);
				}
				break;
			}
			nIndex++;
		}
	}
	else if (nID > IDC_START_SUB && nID < ID_END_SUB)
	{
		UINT nIDBase = 0;
		TreeData treeData;
		GetIndex(nID, treeData.nIndexBase, treeData.nIndexSub, nIDBase);
		CSimpleButton* pBtn = (CSimpleButton*)GetDlgItem(nID);
		pBtn->GetWindowText(treeData.strTextSub);
		*pResult = GetParent()->SendMessage(WM_TREEINFO, (WPARAM)&treeData, NULL);
		if (m_mapTreeData[nIDBase].bTab && *pResult != 0)
		{
			map<UINT, SharedPtrBtn>& mapTemp = m_mapTreeData[nIDBase].mapBtnSub;
			size_t nIndex = 0;
			for (auto iter = mapTemp.begin(); iter != mapTemp.end(); iter++)
			{
				if (nIndex != treeData.nIndexSub)
				{
					iter->second->SetSelected(FALSE);
				}
				nIndex++;
			}
		}
	}
}

void CSimpleTree::GetIndex(UINT nIDSub, int& nIndexBase, int& nIndexSub, UINT& nIDBase)
{
	nIndexBase = -1;
	for (auto iterBase = m_mapTreeData.begin(); iterBase != m_mapTreeData.end(); iterBase++)
	{
		nIndexBase++;
		BOOL bBreak = FALSE;
		nIndexSub = -1;
		for (auto iterSub = iterBase->second.mapBtnSub.begin(); iterSub != iterBase->second.mapBtnSub.end(); iterSub++)
		{
			nIndexSub++;
			if (iterSub->first == nIDSub)
			{
				nIDBase = iterBase->first;
				bBreak = TRUE;
				break;
			}
		}
		if (bBreak)
		{
			break;
		}
	}
}

void CSimpleTree::ReLayout(int nIndexStart, CRect& rectInvalidate)
{
	CRect rCl;
	GetClientRect(&rCl);
	int nY = m_nTopGap;
	int nIndex = 0;
	for (auto iterBase = m_mapTreeData.begin(); iterBase != m_mapTreeData.end(); iterBase++)
	{
		CRect rectBase;
		rectBase.left = 0;
		rectBase.top = nY;
		rectBase.right = rCl.right;
		rectBase.bottom = rectBase.top + m_nBaseHeight;
		
		if (nIndex == nIndexStart)
		{
			if (0 == nIndex)
			{
				rectInvalidate = rCl;
			}
			else
			{
				rectInvalidate.SetRect(0, rectBase.bottom, rCl.right, rCl.bottom);
			}
			
		}
		if (nIndex >= nIndexStart)
		{
			iterBase->second.pBtnBase->MoveWindow(&rectBase, TRUE);
		}
		
		nY = rectBase.bottom;

		iterBase->second.rectSub.left = 0;
		iterBase->second.rectSub.top = nY;
		iterBase->second.rectSub.right = rCl.right;

		if (iterBase->second.bClose)
		{
			nY++;
			for (auto iterSub = iterBase->second.mapBtnSub.begin(); iterSub != iterBase->second.mapBtnSub.end(); iterSub++)
			{
				iterSub->second->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			nY = nY + TopGapSub;
			int nIndexSub = 0;
			for (auto iterSub = iterBase->second.mapBtnSub.begin(); iterSub != iterBase->second.mapBtnSub.end(); iterSub++)
			{
				CString strCaption;
				iterSub->second->GetWindowText(strCaption);
				UINT nWidth = GetWidth(CSimpleButton::GetFont(), strCaption);
				CRect rectSub;
				rectSub.left = (rCl.right - nWidth) / 2;
				rectSub.top = nY;
				rectSub.right = rectSub.left + nWidth;
				rectSub.bottom = rectSub.top + m_nSubHeight;
				iterSub->second->ShowWindow(SW_SHOW);
				if (nIndex >= nIndexStart)
				{
					iterSub->second->MoveWindow(&rectSub, TRUE);
				}
				if (nIndexSub == iterBase->second.mapBtnSub.size() - 1)
				{
					nY = rectSub.bottom + TopGapSub;
				} 
				else
				{
					nY = rectSub.bottom + BtnSubYGap;
				}
				nIndexSub++;
			}
		}
		iterBase->second.rectSub.bottom = nY;
		nIndex++;
	}
}

CString CSimpleTree::GetCaption(BOOL bClose, CString strCaption)
{
	CString str;
	str = bClose ? ("   + " + strCaption) : (" - " + strCaption);
	return str;
}

void CSimpleTree::SetTopGap(UINT nGap)
{
	m_nTopGap = nGap;
}

void CSimpleTree::SetFontStyle(CString strFontStyle)
{
	if (FontStandard == strFontStyle)
	{
		m_nBaseHeight = 25;
		m_nSubHeight = 23;
	}
	else if (FontMedium == strFontStyle)
	{
		m_nBaseHeight = 27;
		m_nSubHeight = 25;
	}
	else if (FontBig == strFontStyle)
	{
		m_nBaseHeight = 29;
		m_nSubHeight = 27;
	}
}

UINT CSimpleTree::GetWidth(HFONT hFont, CString str)
{
	CRect rect;
	CDC *pDC = GetDC();
	CFont *pOld = pDC->SelectObject(CFont::FromHandle(hFont));
	pDC->DrawText(str, rect, DT_CALCRECT | DT_LEFT | DT_VCENTER);
	pDC->SelectObject(pOld);
	ReleaseDC(pDC);

	return (rect.Width() + 20);
}



