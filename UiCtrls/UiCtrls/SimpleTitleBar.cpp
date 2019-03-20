
//*****************************************************
//********     By YaoShuLong 3.24.2017
//*****************************************************

#include "stdafx.h"
#include "SimpleTitleBar.h"
#include "..\resource.h"

// CSimpleTitleBar

IMPLEMENT_DYNAMIC(CSimpleTitleBar, CWnd)

CSimpleTitleBar::CSimpleTitleBar()
{
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;

    m_nRegion = MENU_LEAVE;
    m_nPopupPosition = MENU_LEAVE;
    m_nLanguage = NULL;      
    m_nHeight = -1;
}

CSimpleTitleBar::~CSimpleTitleBar()
{
    for (auto pPopupMenuEx : m_vecpPopupMenuEx)
    {
        if (pPopupMenuEx != NULL)
        {
            delete pPopupMenuEx;
            pPopupMenuEx = NULL;
        }
    }
}


BEGIN_MESSAGE_MAP(CSimpleTitleBar, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_KILLFOCUS()
    ON_WM_SYSKEYDOWN()
    ON_WM_SYSKEYUP()
//    ON_MESSAGE(COMMAND_SPECIAL, &CSimpleTitleBar::OnCommandFrom)
END_MESSAGE_MAP()

// CSimpleTitleBar 消息处理程序

void CSimpleTitleBar::OnPaint()
{
    if (m_mainPopMenu.IsWindowVisible() && m_nRegion != m_nPopupPosition)
    {
        return;
    }

    CPaintDC dc(this);
    CRect rCl;
    GetClientRect(&rCl);
    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
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
        
    dcMem.FillSolidRect(&rCl, RGB(18, 20, 22));
    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();

    int nX = 0;
    for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i++)
    {
        DrawMenu(dc, nX, &m_rectTitleBarMenuInfo[i]);
    }
}

BOOL CSimpleTitleBar::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{  
	if (!RegisterWindowClass())
	{
		return FALSE;
	}
    
    BOOL bResult = CWnd::Create(SimpleTitleBar_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
    m_mainPopMenu.Create(WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL);
//     for (int i = 0; i < 2; i++)
//     {
//         CSimplePopupMenuEx * pPopupMenu = new CSimplePopupMenuEx();
//         if (pPopupMenu != NULL)
//         {
//             pPopupMenu->Create(WS_POPUP, CRect(0, 0, 0, 0), this, NULL);
//             m_vecpSimplePopupMenu.push_back(pPopupMenu);
//         }
//     }
       
    return bResult;
}

BOOL CSimpleTitleBar::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleTitleBar_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleTitleBar_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_nRegion != MENU_NULL)
    {
        for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
        {
            if (m_nPopupPosition == m_recPlaceListMenu[i].nMenuID)
            {
                m_mainPopMenu.Clear();
                
                for (int m = 0; m < m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); m++)
                {
                    m_mainPopMenu.AddMenu(m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnablePopup, 
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].pPopupMenu, 
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableCheck,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bCheck,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].nID,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].str,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableSeparator,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnable,
                                                           m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bLight
                                                           );
                }
                CRect rect = GetRect(m_nRegion);
                ClientToScreen(&rect);
                m_mainPopMenu.Invalidate(FALSE);
                m_mainPopMenu.TrackPopupMenu(rect.left - 1, rect.bottom - 1, m_nHeight, this);
                break;
            }
        }
    }
    else
    {
        if (m_mainPopMenu.IsWindowVisible())
        {       
            m_mainPopMenu.ShowWindow(SW_HIDE);
        }
    }

//     if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
//     {
//         SetFocus();
//     }
    m_bLMouseDown = TRUE;
  //  Invalidate(FALSE);

    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleTitleBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_nRegion != MENU_NULL)
    {
        for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
        {
            if (m_nPopupPosition == m_recPlaceListMenu[i].nMenuID)
            {
                m_mainPopMenu.Clear();
                for (int m = 0; m < m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); m++)
                {
                    m_mainPopMenu.AddMenu(m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnablePopup, 
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].pPopupMenu, 
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableCheck,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bCheck,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].nID,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].str,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableSeparator,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnable,
                        m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bLight
                        );
                }
                CRect rect = GetRect(m_nRegion);
                ClientToScreen(&rect);
                m_mainPopMenu.Invalidate(FALSE);
                m_mainPopMenu.TrackPopupMenu(rect.left - 1, rect.bottom - 1, m_nHeight, this);
                break;
            }
        }
    }
    else
    {
        if (m_mainPopMenu.IsWindowVisible())
        {       
            m_mainPopMenu.ShowWindow(SW_HIDE);
        }
    }

    //     if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
    //     {
    //         SetFocus();
    //     }
    m_bLMouseDown = TRUE;
    //  Invalidate(FALSE);
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleTitleBar::OnLButtonUp(UINT nFlags, CPoint point)
{

    if (m_bLMouseDown)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_CLICK;
    //    GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        m_bLMouseDown = FALSE;
        //Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleTitleBar::OnMouseLeave()
{
    for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i ++)
    {
        m_rectTitleBarMenuInfo[i].bHover = FALSE;
    }
    if (m_nRegion != MENU_LEAVE && m_nRegion != MENU_NULL)
    {
        InvalidateRect(GetRect(m_nRegion));
    }
    m_nRegion = MENU_LEAVE;

    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    
   // Invalidate(FALSE);
    
    CWnd::OnMouseLeave();
}

void CSimpleTitleBar::OnMouseMove(UINT nFlags, CPoint point)
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

    int nOldRegion = m_nRegion;

    m_nRegion = HitTest(point);
    if (nOldRegion != m_nRegion)
    {
        if (m_nRegion != MENU_NULL && m_nRegion != MENU_LEAVE)
        {
            InvalidateRect(GetRect(m_nRegion));
        }
        if (nOldRegion != MENU_NULL && nOldRegion != MENU_LEAVE)
        {
            InvalidateRect(GetRect(nOldRegion));
        }
        if (m_nRegion != MENU_NULL)
        {
            m_nPopupPosition = m_nRegion;
         //   m_vecpSimplePopupMenu[0]->m_nPopupPositionParent = m_nPopupPosition;
            if (m_mainPopMenu.IsWindowVisible())
            {
                for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
                {
                    if (m_nPopupPosition == m_recPlaceListMenu[i].nMenuID)
                    {
                        m_mainPopMenu.Clear();
                        for (int m = 0; m < m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); m++)
                        {
                            m_mainPopMenu.AddMenu(m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnablePopup, 
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].pPopupMenu, 
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableCheck,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bCheck,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].nID,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].str,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableSeparator,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnable,
                                m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bLight
                                );
                        }
                        CRect rect = GetRect(m_nRegion);
                        ClientToScreen(&rect);
                        m_mainPopMenu.Invalidate(FALSE);
                        m_mainPopMenu.TrackPopupMenu(rect.left - 1, rect.bottom - 1, m_nHeight, this);
                        break;
                    }
                }
            }
        }
        
    }
    

    CWnd::OnMouseMove(nFlags, point);
}

int CSimpleTitleBar::HitTest(CPoint point)
{
    for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i ++)
    {
        m_rectTitleBarMenuInfo[i].bHover = FALSE;
    }
    for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i ++)
    {
        if (m_rectTitleBarMenuInfo[i].rect.PtInRect(point))
        {
            m_rectTitleBarMenuInfo[i].bHover = TRUE;
            m_nRegion = m_rectTitleBarMenuInfo[i].nMenuID;     
            return m_nRegion;
        } 
     }
    m_nRegion = MENU_NULL;
    return m_nRegion;
}

BOOL CSimpleTitleBar::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_MOUSEMOVE)
    {
        
        
    }
    return FALSE;
}

BOOL CSimpleTitleBar::SendMessageToParent()
{
//     if (-1 == m_vecpSimplePopupMenu[0]->m_nCurPosition)
//     {
//         return FALSE;
//     }
//     UINT nID;
//     for (size_t i = 0; i < m_vecpSimplePopupMenu[0]->m_recPlaceListMenu.size(); i++)
//     {
//         if (m_nPopupPosition == m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].nMenuID)
//         {
//             for (size_t j = 0; j < m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); j ++)
//             {
//                 if (m_vecpSimplePopupMenu[0]->m_nCurPosition == j)
//                 {
//                     nID = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[j].nID;
//                     break;
//                 }
//             }
//             break;
//         }
//     }
// 
//     GetParent()->PostMessage(WM_COMMAND, nID, 0);
    return TRUE;
}

// void CSimpleTitleBar::SetPlcaListRect()
// {
//       int nHeight = 0;
//       int nWidth = 0;
//      CFont* pFont = CFont::FromHandle(m_vecpSimplePopupMenu[0]->GetFont());
//      for (size_t i = 0; i < m_vecpSimplePopupMenu[0]->m_recPlaceListMenu.size(); i++)
//      {
//          if (m_nPopupPosition == m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].nMenuID)
//          {
//          //    nHeight = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList.size() * 25;
//              
//              CRect rectText;
//              rectText.SetRectEmpty();
//              CDC *pDC = GetDC();
//              CFont *pOld = pDC->SelectObject(pFont);
// 
//              for (size_t j = 0; j < m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); j++)
//              {
//                  pDC->DrawText(m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[j].string, rectText, DT_CALCRECT | DT_LEFT);
//                  nWidth = (nWidth >= rectText.Width() ? nWidth : rectText.Width());
//                  int nHeightEx = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[j].rect.Height();
//                  nHeight += nHeightEx;
//              }
//              nWidth += 16;
//              pDC->SelectObject(pOld);
//              ReleaseDC(pDC);
// 
//              int nMenuWidth = 0;
//              for (size_t k = 0; i < m_rectTitleBarMenuInfo.size(); k++)
//              {
//                  if (m_nPopupPosition == m_rectTitleBarMenuInfo[k].nMenuID)
//                  {
//                      nMenuWidth = m_rectTitleBarMenuInfo[k].nWidth;
//                      break;
//                  }
//              }
//              int nWidthPlacelist = 0;
//              if (nWidth < nMenuWidth)
//              {
//                  nWidthPlacelist = nMenuWidth;
//              }
//              else
//              {
//                  nWidthPlacelist = nWidth;
//              }
// 
//              for (size_t m = 0; m < m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList.size(); m++)
//              {     
//                  BOOL bEnableSeparator = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableSeparator;
//                  if (0 == m)
//                  {
//                      m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect.SetRect(0, 0, nWidthPlacelist, bEnableSeparator ? 3 : 25);                
//                  }
//                  else
//                  {
//                      CRect rect = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m - 1].rect;
//                      m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect.SetRect(0, rect.bottom, rect.right, rect.bottom + (bEnableSeparator ? 3 : 25));
//                  }
// //                  m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect.SetRect(0, 0, nWidthPlacelist, 25);
// //                  m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect.OffsetRect(0, m*25);
// //                  if (m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].bEnableSeparator)
// //                  {
// //                      CRect rect = m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect;
// //                      m_vecpSimplePopupMenu[0]->m_recPlaceListMenu[i].vecTextInfoPlaceList[m].rect.SetRect(rect.left, rect.top, rect.right, rect.top + 3);
// //                  }
//              }
//              break;
//          }
//      }
// 
//     CRect rect = GetRect(m_nRegion);
//     if (nWidth < rect.Width())
//     {
//         nWidth = rect.Width();
//     }
//     ClientToScreen(&rect);
//     m_rectPlaceList.SetRect(rect.left, rect.bottom, rect.left + nWidth, rect.bottom +  nHeight );       
// 
//     m_vecpSimplePopupMenu[0]->Invalidate(FALSE);
// 
// }

BOOL CSimpleTitleBar::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleTitleBar::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
//     if (GetFocus() != &m_InPlaceList)
//     {
//         HidePlacelist();
//     }   
}

void CSimpleTitleBar::HidePlacelist()
{
    if (IsWindow(m_mainPopMenu.GetSafeHwnd()))
    {
        m_mainPopMenu.ShowWindow(SW_HIDE);
    }
}

BITMAP CSimpleTitleBar::GetBitmap(UINT nBmpID)
{  
    int nWidth = 0;
    CBitmap bmp;
    bmp.LoadBitmap(nBmpID);
    BITMAP bitMap;
    bmp.GetBitmap(&bitMap);
    bmp.DeleteObject();
    return bitMap;
}

BITMAP CSimpleTitleBar::GetBitmap(CString strBmpPathName)
{  
    int nWidth = 0;
    CBitmap bmp;
  //  bmp.LoadBitmap(strBmpPathName);


    HBITMAP hBitmap;  
    hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),strBmpPathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
    bmp.Attach(hBitmap);

    BITMAP bitMap;
    bmp.GetBitmap(&bitMap);
    bmp.DeleteObject();
    return bitMap;
}

void CSimpleTitleBar::DrawMenu(CPaintDC& dc, int& nX, TitleBarMenuInfo* pTitleBarMenuInfo)
{
    if (pTitleBarMenuInfo != NULL)
    {
        if (pTitleBarMenuInfo->bEnable)
        {
            CRect rCl;
            GetClientRect(&rCl);
            CDC dcMem;
            CBitmap bmp;
            CBitmap* pOldBmp = NULL;

            if (pTitleBarMenuInfo->bBmpIDMode)
            {
                bmp.LoadBitmap(pTitleBarMenuInfo->bHover ? pTitleBarMenuInfo->nBmpIDHover : pTitleBarMenuInfo->nBmpIDDef);
            }
            else
            {
                HBITMAP hBitmap;  
                hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),pTitleBarMenuInfo->bHover ? pTitleBarMenuInfo->strBmpHoverPathName : pTitleBarMenuInfo->strBmpDefPathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
                bmp.Attach(hBitmap);
            }
            
            if(dcMem.CreateCompatibleDC(&dc))
            {
                pOldBmp = dcMem.SelectObject(&bmp);
            }
            else
            {
                return;
            }

            dc.BitBlt(nX, 0, pTitleBarMenuInfo->nWidth, rCl.Height(), &dcMem, 0, 0, SRCCOPY);
            dcMem.SelectObject(pOldBmp);
            dcMem.DeleteDC();
            bmp.DeleteObject();
            nX += pTitleBarMenuInfo->nWidth + 1;
        }       
    }
}

void CSimpleTitleBar::SetVecMenuInfo(int& nX, int nBmpWidth, UINT nMenuID)
{
    MenuInfo menuInfo;
    menuInfo.rect.SetRect(nX, 0, nX + nBmpWidth, m_nHeight);
    nX += (nBmpWidth + 1);
    menuInfo.nMenuID = nMenuID;
    m_vecMenuInfo.push_back(menuInfo);
}

CRect CSimpleTitleBar::GetRect(UINT nMenuID)
{
    CRect rect;
    for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i++)
    {
        if (nMenuID == m_rectTitleBarMenuInfo[i].nMenuID)
        {
            return m_rectTitleBarMenuInfo[i].rect;
        }
    }
    return rect;
}

void CSimpleTitleBar::AddMenu(UINT nMenuID, UINT nBmpIDDef, UINT nBmpIDHover, BOOL bEnable /*= TRUE*/)
{
    if (bEnable)
    {
        TitleBarMenuInfo titleBarMenuInfo;
        titleBarMenuInfo.nMenuID = nMenuID;
        int nX = 0;
        for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i++)
        {
            nX += m_rectTitleBarMenuInfo[i].nWidth + 1;
        }   
        titleBarMenuInfo.nBmpIDDef = nBmpIDDef;
        titleBarMenuInfo.nBmpIDHover = nBmpIDHover;
        BITMAP bitMap;
        bitMap = GetBitmap(nBmpIDDef);
        //bitMap = GetBitmap(strBmpDefPathName);
        titleBarMenuInfo.nWidth = bitMap.bmWidth;
        m_nHeight = bitMap.bmHeight;
        titleBarMenuInfo.rect.SetRect(nX, 0, nX + titleBarMenuInfo.nWidth, m_nHeight);
        titleBarMenuInfo.bHover = FALSE;
        titleBarMenuInfo.bEnable = bEnable;
        titleBarMenuInfo.bBmpIDMode = TRUE;
        m_rectTitleBarMenuInfo.push_back(titleBarMenuInfo);
        PlaceListMenu placeListMenu;
        placeListMenu.nMenuID = nMenuID;
        m_recPlaceListMenu.push_back(placeListMenu);
    }
}

void CSimpleTitleBar::AddMenu(UINT nMenuID, CString strBmpDefPathName, CString strBmpHoverPathName, BOOL bEnable /*= TRUE*/)
{
    if (bEnable)
    {
        TitleBarMenuInfo titleBarMenuInfo;
        titleBarMenuInfo.nMenuID = nMenuID;
        int nX = 0;
        for (size_t i = 0; i < m_rectTitleBarMenuInfo.size(); i++)
        {
            nX += m_rectTitleBarMenuInfo[i].nWidth + 1;
        }   
        titleBarMenuInfo.nBmpIDDef = 0;
        titleBarMenuInfo.nBmpIDHover = 0;
        titleBarMenuInfo.strBmpDefPathName = strBmpDefPathName;
        titleBarMenuInfo.strBmpHoverPathName = strBmpHoverPathName;
        BITMAP bitMap;
        bitMap = GetBitmap(strBmpDefPathName);
        titleBarMenuInfo.nWidth = bitMap.bmWidth;
        m_nHeight = bitMap.bmHeight;
        titleBarMenuInfo.rect.SetRect(nX, 0, nX + titleBarMenuInfo.nWidth, m_nHeight);
        titleBarMenuInfo.bHover = FALSE;
        titleBarMenuInfo.bEnable = bEnable;
        titleBarMenuInfo.bBmpIDMode = FALSE;
        m_rectTitleBarMenuInfo.push_back(titleBarMenuInfo);
        PlaceListMenu placeListMenu;
        placeListMenu.nMenuID = nMenuID;
        m_recPlaceListMenu.push_back(placeListMenu);
    }
}

void CSimpleTitleBar::SetMenu(UINT nMenuID, CString str, UINT nID, BOOL bEnableSeparator /*= FALSE*/, BOOL bEnableCheck /*= FALSE*/, BOOL bCheck /*= FALSE*/, BOOL bEnable /*= TRUE*/, BOOL bLight /*= FALSE*/)
{
    for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
    {
        if (nMenuID == m_recPlaceListMenu[i].nMenuID)
        {
            TextInfoPlaceList textInfoPlaceList;
            textInfoPlaceList.bEnablePopup = FALSE;
            textInfoPlaceList.pPopupMenu = NULL;
            textInfoPlaceList.bEnableCheck = bEnableCheck;
            textInfoPlaceList.bCheck = bCheck;
            textInfoPlaceList.nID = nID;
            textInfoPlaceList.str = str;
            textInfoPlaceList.bEnableSeparator = bEnableSeparator;
            textInfoPlaceList.bEnable = bEnable;
            textInfoPlaceList.bLight = bLight;

            m_recPlaceListMenu[i].vecTextInfoPlaceList.push_back(textInfoPlaceList);
            break;
        }
    }
}

void CSimpleTitleBar::SetMenu(UINT nMenuID, CString str, UINT nID, CSimplePopupMenuEx* pPopupMenu, BOOL bEnableSeparator /*= FALSE*/, BOOL bEnableCheck /*= FALSE*/, BOOL bCheck /*= FALSE*/, BOOL bEnable /*= TRUE*/, BOOL bLight /*= FALSE*/)
{
    for (size_t i = 0; i < m_recPlaceListMenu.size(); i++)
    {
        if (nMenuID == m_recPlaceListMenu[i].nMenuID)
        {
            TextInfoPlaceList textInfoPlaceList;
            textInfoPlaceList.bEnablePopup = TRUE;
            textInfoPlaceList.pPopupMenu = pPopupMenu;       
            textInfoPlaceList.bEnableCheck = bEnableCheck;
            textInfoPlaceList.bCheck = bCheck;
            textInfoPlaceList.nID = nID;
            textInfoPlaceList.str = str;
            textInfoPlaceList.bEnableSeparator = bEnableSeparator;
            textInfoPlaceList.bEnable = bEnable;
            textInfoPlaceList.bLight = bLight;

            m_recPlaceListMenu[i].vecTextInfoPlaceList.push_back(textInfoPlaceList);
            break;
        }
    }
}

int CSimpleTitleBar::GetHeight()
{
    return m_nHeight;
}

void CSimpleTitleBar::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void CSimpleTitleBar::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void CSimpleTitleBar::Clear()
{
    m_rectTitleBarMenuInfo.clear();
    m_recPlaceListMenu.clear();
}

LRESULT CSimpleTitleBar::OnCommandFrom(WPARAM wParam, LPARAM lParam)
{
    GetParent()->PostMessage(WM_COMMAND, wParam, 0);
    return 0;
}

CSimplePopupMenuEx* CSimpleTitleBar::CreateSubMenu(CWnd * pParentWnd)
{
    CSimplePopupMenuEx* pCSimplePopupMenuEx = new CSimplePopupMenuEx();
    if (pCSimplePopupMenuEx != NULL)
    {
        pCSimplePopupMenuEx->Create(WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL);
        pCSimplePopupMenuEx->m_pParentWnd = pParentWnd;
        pCSimplePopupMenuEx->SetSub();
        m_vecpPopupMenuEx.push_back(pCSimplePopupMenuEx);
    }
    return pCSimplePopupMenuEx;
}

void CSimpleTitleBar::SetFont(HFONT hFont)
{
	m_mainPopMenu.SetFont(hFont);
}




