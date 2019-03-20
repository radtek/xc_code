// ComboboxListboxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CompareDlg.h"
#include "CompareEdit.h"

#include<algorithm>
using namespace std;


BOOL SortStockCode(const CString str1, const CString str2)
{
	return str1 < str2;
}

// CCompareDlg 对话框

IMPLEMENT_DYNAMIC(CCompareDlg, CWnd)

    CCompareDlg::CCompareDlg()
{
    m_nHeight		= 0;
    m_nWidth		= 0;
    m_nItemCountMax = 7;
    m_pEdit = NULL;
    m_bSeletctInstOK = FALSE;
}

CCompareDlg::~CCompareDlg()
{

}


BEGIN_MESSAGE_MAP(CCompareDlg, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_MESSAGE(UM_COMPAREDLG, &CCompareDlg::OnUmCompareDlg)
    ON_WM_ACTIVATE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCompareDlg 消息处理程序

void CCompareDlg::AddVStroll()
{
    //     CBitmap bmp;
    //     bmp.LoadBitmap(IDB_BITMAP2);
    //     BITMAP bm;
    //     bmp.GetBitmap(&bm);
    //     m_hBmpScrollBar=(HBITMAP)bmp.Detach();
    //     SkinWndScroll(&m_listbox,m_hBmpScrollBar);

}

void CCompareDlg::AlterListBoxPos()
{
    m_listbox.MoveWindow(0, 26, m_nWidth, m_nHeight, FALSE);
    Invalidate(FALSE);

}

BOOL CCompareDlg::Create(CWnd * pParentWnd)
{
	RegisterWindowClass();
	return CWnd::CreateEx(NULL, CompareDlg_CLASSNAME, _T(""), WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL);
}

BOOL CCompareDlg::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, CompareDlg_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = CompareDlg_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

int CCompareDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_compareInplaceEdit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0,0,0,0), this, IDC_INPACEEDIT);
    m_compareInplaceEdit.InitSettings();

    m_listbox.Create(WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_VSCROLL, CRect(0,0,0,0), this, IDC_MYLISTBOX);
    m_listbox.m_pCompareDlg = this;


    return 0;
}

void CCompareDlg::OnPaint()
{

    CPaintDC dc(this);
    CRect rCl;
    GetClientRect(&rCl);

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
            pOldBmp = dcMem.SelectObject(&bmpMem);
        else
            return;
    }
    else
        return;

    dcMem.FillSolidRect(rCl, RGB(63, 65, 77));  

    CRect rect;
    rect.SetRect(0, 25, rCl.right, 26);

    dcMem.FillSolidRect(rect, RGB(15, 15, 15));  

    CRect rectEx;
    rectEx.SetRect(0, rCl.bottom - 1, rCl.right, rCl.bottom);

    dcMem.FillSolidRect(rectEx, RGB(15, 15, 15));  

    dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
}

void CCompareDlg::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (::IsWindow(m_compareInplaceEdit.GetSafeHwnd()))
    {
        m_compareInplaceEdit.MoveWindow(0, 0, 128, 25, FALSE);
    }
    

    if (::IsWindow(m_listbox.GetSafeHwnd()))
    {
        m_listbox.MoveWindow(0, 26, 128, 0, FALSE);
    }

}

void CCompareDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);

    if (WA_INACTIVE == nState && !m_bSeletctInstOK)
    {
        if (this->IsWindowVisible())
        {
            this->ShowWindow(SW_HIDE);
            if (NULL != m_pEdit)
            {
                m_pEdit->SendMessage(UM_COMPAREEDIT, UM_COMPAREEDIT_HIDE_DLG_ONT_BY_CONFIRM_INST, 0);
            }
        }
    }
    else if (WA_ACTIVE == nState)
    {
        m_bSeletctInstOK = FALSE;
    }
}
LRESULT CCompareDlg::OnUmCompareDlg(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case UM_COMPAREDLG_ONENCHANGE:
        {          
            if (::IsWindow(m_listbox.GetSafeHwnd()))
            {
                m_listbox.ShowWindow(SW_HIDE);
                m_listbox.ResetContent();
                SetWindowSize(0);
            }
            CString str;
            m_compareInplaceEdit.GetWindowText(str);
            str.MakeUpper();

            if (str.IsEmpty())
            {      
                CRect rect;
                GetWindowRect(&rect);
                if (rect.Height() != 25)
                {
                    MoveWindow(rect.left, rect.top, rect.Width(), 25, FALSE);
                }
                return 0;
            }
            CRect rect;
            GetWindowRect(&rect);
            if (rect.Height() != m_nItemCountMax*20 + 25 + 2)
            {
                MoveWindow(rect.left, rect.top, rect.Width(), m_nItemCountMax*20 + 25 + 2, FALSE);
            }
            
			vector<CString> vecStockCodeMatch;
            CString stringEx;
			for (auto& data : m_vecStockCode)
			{
				if (data.Find(str) >= 0)
				{
					vecStockCodeMatch.push_back(data);
				}
			}

            sort(vecStockCodeMatch.begin(), vecStockCodeMatch.end(), SortStockCode);
            int nOKCounts = 0;

            m_listbox.m_nHistorty = nOKCounts;
			for (auto& data : vecStockCodeMatch)
            {
                m_listbox.AddString(data);
                m_listbox.m_nIndex = -1;
                int nItemCount = m_listbox.GetCount();
                m_listbox.SetItemHeight(nItemCount - 1, 20);
                if (m_nItemCountMax >= nItemCount)
                {
                    SetWindowSize(nItemCount);
                }             
            }
            if (nOKCounts > 0)
            {              
                BOOL bFind = FALSE;
                int nIndex = 0;
                for (int i = 0; i < nOKCounts; i++)
                {
                    CString strGet;
                    m_listbox.GetText(i, strGet);
                    strGet.Trim().MakeUpper();
                    if (strGet.Find(str) == 0)
                    {
                        nIndex = i;
                        bFind = TRUE;
                        break;
                    }
                }
                if (bFind)
                {
                    m_listbox.SetCurSel(nIndex);     
                }
                else
                {
                    m_listbox.SetCurSel(nOKCounts);     
                }              
            }
            else
            {
                m_listbox.SetCurSel(nOKCounts);     
            }
                      
            
            if (!m_listbox.IsWindowVisible())
            {
                m_listbox.ShowWindow(SW_SHOW);
            }
			m_compareInplaceEdit.SetSel(str.GetLength(), str.GetLength());
            break;
        }

    case UM_COMPAREDLG_VK_UPANDDOWN:
        {
            UINT nChar = (UINT)lParam;
            m_listbox.PostMessage(WM_KEYDOWN, nChar, 0);
            break;
        }

    case UM_COMPAREDLG_LISTBOX_OK:
        {
            m_bSeletctInstOK = TRUE;
            int nCount = m_listbox.GetCount();
            if(nCount>0)
            {
                m_listbox.GetText(m_listbox.GetCurSel(), m_listbox.m_szCheck);
            }   
            
            ShowWindow(SW_HIDE);
            if (NULL != m_pEdit)
            {
                m_pEdit->SendMessage(UM_COMPAREEDIT, UM_COMPAREEDIT_OK, NULL); 
            }
              
            break;
        }

    case UM_COMPAREDLG_MOUSEWHEELUP:
        {
            CPoint* pPt = (CPoint*)lParam; 
            m_listbox.PostMessage(WM_MOUSEWHEEL, MAKELPARAM(0, WHEEL_DELTA), MAKELPARAM(pPt->x, pPt->y));  
            break;
        }

    case UM_COMPAREDLG_MOUSEWHEELDOWN:
        {
            CPoint* pPt = (CPoint*)lParam; 
            m_listbox.PostMessage(WM_MOUSEWHEEL, MAKELPARAM(0, -WHEEL_DELTA), MAKELPARAM(pPt->x, pPt->y));  
            break;
        }

    default:
        break;
    }



    return 0;
}

void CCompareDlg::SetWindowSize(int nItemCount)
{
    int nHeight = nItemCount*20;
    m_nHeight = nHeight;
    m_listbox.m_nHeight = nHeight;
    AlterListBoxPos();

}

BOOL CCompareDlg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


