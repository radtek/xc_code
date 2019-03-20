
//*****************************************************
//********    By YaoShuLong 2017.7
//*****************************************************

#include "stdafx.h"
#include "SimplePopupMenu.h"
#include "..\resource.h"
#include "GridCtrlEx.h"

IMPLEMENT_DYNAMIC(CSimplePopupMenu, CWnd)

CSimplePopupMenu::CSimplePopupMenu() :
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE),
m_strCaption(_T(""))
{
    m_nSubMenuState = SUBMENU_HIDE_DEFAUT;
    m_pParentEx = nullptr;
    m_nX = 0;
    m_nY = 0;
    m_nX1 = 0;
    m_nY1 = 0;
    m_nWidth = 0;
    m_pParent = NULL;
    RegisterWindowClass();
    m_font.CreateFont(-12,0,0,0,
        FW_NORMAL,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("微软雅黑"));
    m_hFont = (HFONT)m_font.GetSafeHandle();
//     MenuList textInfo;
//     textInfo.string = _T("dsfsdfsdf");
//     textInfo.rect.SetRect(0, 0, 100, 25);
//     m_vecTextInfo.push_back(textInfo);
//     for (int i = 1; i < 20; i ++)
//     {
//         textInfo.rect.OffsetRect(0, 25);
//         m_vecTextInfo.push_back(textInfo);
//     }

    m_nCurPosition = -1;

    m_clrRegionDef = 0;

    m_clrText = RGB(255, 255, 255);
    m_clrLight = RGB(255, 255, 255);
    m_clrTextDisabled = RGB(189, 189, 189);
 //   m_clrRegionDef = RGB(37, 39, 53);
    m_clrRegionDef = RGB(78, 78, 78);
    m_clrRegionHover = RGB(51, 138, 249);
    m_clrOutline = RGB(50, 50, 50);
    m_bBase = TRUE;
    m_pParentWnd = NULL;
    m_bNew = FALSE;
    m_pMFCGridCtrl = NULL;
    m_bSetGridKillFocus = TRUE;
}

CSimplePopupMenu::~CSimplePopupMenu()
{
    for (size_t i = 0; i < m_vecTextInfo.size(); i++)
    {
        if (m_vecTextInfo[i].pPopupMenu != NULL && m_vecTextInfo[i].pPopupMenu->m_bNew)
        {
            delete m_vecTextInfo[i].pPopupMenu;
            m_vecTextInfo[i].pPopupMenu = NULL;
        }
    }
}


BEGIN_MESSAGE_MAP(CSimplePopupMenu, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEACTIVATE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ACTIVATE()
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()

END_MESSAGE_MAP()

// CSimpleComboBox 消息处理程序

void CSimplePopupMenu::OnPaint()
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

    CDC dcMemCheck;
    CBitmap bmpMemCheck;
    CBitmap* pOldBmpCheck = NULL;
    bmpMemCheck.LoadBitmap(IDB_BITMAP_CHECKED);

    if(dcMemCheck.CreateCompatibleDC(&dc))
    {
        pOldBmpCheck = dcMemCheck.SelectObject(&bmpMemCheck);

    }
    else
        return;

    CDC dcMemUncheck;
    CBitmap bmpMemUncheck;
    CBitmap* pOldBmpUncheck = NULL;
    bmpMemUncheck.LoadBitmap(IDB_BITMAP_UNCHECKED);

    if(dcMemUncheck.CreateCompatibleDC(&dc))
    {
        pOldBmpUncheck = dcMemUncheck.SelectObject(&bmpMemUncheck);

    }
    else
        return;

    Paint(dcMem, dcMemCheck, dcMemUncheck);

    CPen pen1(PS_SOLID, 1, m_clrOutline);
    CPen* pOldPen = dcMem.SelectObject(&pen1);
    CPoint ptsLine[4];
    ptsLine[0] = CPoint(rCl.left, rCl.top);
    ptsLine[1] = CPoint(rCl.left, rCl.bottom -1);
    ptsLine[2] = CPoint(rCl.right - 1, rCl.bottom - 1);
    ptsLine[3] = CPoint(rCl.right - 1, rCl.top);
    dcMem.MoveTo(ptsLine[0]);
    dcMem.LineTo(ptsLine[1]);
    dcMem.LineTo(ptsLine[2]);
    dcMem.LineTo(ptsLine[3]);
    dcMem.LineTo(ptsLine[0]);
    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(pOldPen);

    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();

    dcMemCheck.SelectObject(pOldBmpCheck);
    dcMemCheck.DeleteDC();
    bmpMemCheck.DeleteObject();

    dcMemUncheck.SelectObject(pOldBmpUncheck);
    dcMemUncheck.DeleteDC();
    bmpMemUncheck.DeleteObject();
}


BOOL CSimplePopupMenu::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{  
    m_pParentWnd = pParentWnd;
    return CWnd::CreateEx(NULL,SimplePopupMenu_CLASSNAME, _T(""), dwStyle, rect, pParentWnd ,nID);
}

BOOL CSimplePopupMenu::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimplePopupMenu_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimplePopupMenu_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimplePopupMenu::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bLMouseDown = TRUE;
 //   Invalidate(FALSE);

    CWnd::OnLButtonDown(nFlags, point);
}

void CSimplePopupMenu::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLMouseDown)
    {
        
        if (NULL != GetParent())
        {
            UINT nID = 1;
            if (m_nCurPosition >= 0)
            {
                if (!m_vecTextInfo[m_nCurPosition].bEnableSeparator && m_vecTextInfo[m_nCurPosition].bEnable)
                {
                    nID = m_vecTextInfo[m_nCurPosition].nID;                 
                    if (!m_vecTextInfo[m_nCurPosition].bEnableCheck && !m_vecTextInfo[m_nCurPosition].bEnablePopup)
                    {
                        ShowWindow(SW_HIDE);
                        if (m_pParent != nullptr)
                        {
                            if (m_pParent->IsWindowVisible())
                            {
                                m_pParent->ShowWindow(SW_HIDE);
                            }
                        }
                    }
                    else
                    {
                        m_vecTextInfo[m_nCurPosition].bCheck = !m_vecTextInfo[m_nCurPosition].bCheck;
                    }
                    GetParent()->SendMessage(WM_COMMAND, nID, 0);
//                     if (m_pParentEx != nullptr)
//                     {
//                         m_pParentEx->SendMessage(WM_COMMAND, nID, 0);
//                     }
//                     else if (m_pParent != nullptr)
//                     {
//                         if (m_pParent->m_pParentEx != nullptr)
//                         {
//                             m_pParent->m_pParentEx->SendMessage(WM_COMMAND, nID, 0);
//                         }
//                     }

                    
                    Invalidate(FALSE);
                }
            }
            
            
        }

        m_bLMouseDown = FALSE;
        
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimplePopupMenu::OnMouseLeave()
{ 
    if (m_nCurPosition >= 0 && m_nCurPosition < m_vecTextInfo.size())
    {
        if (!m_vecTextInfo[m_nCurPosition].bEnablePopup)
        {
            Invalidate(FALSE);
        }
    }
    
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    m_nCurPosition = -1;
    
    CWnd::OnMouseLeave();
}


void CSimplePopupMenu::OnMouseMove(UINT nFlags, CPoint point)
{
    CPoint pointEx = point;
    
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
    int nOldPosition = m_nCurPosition;
    m_nCurPosition = HitTest(point);
    if (nOldPosition != m_nCurPosition)
    {
        Invalidate(FALSE);
    }
    if (m_nCurPosition >= 0)
    {
        if (m_vecTextInfo[m_nCurPosition].bEnablePopup)
        {
            if (m_vecTextInfo[m_nCurPosition].pPopupMenu != nullptr)
            {
                if (!m_vecTextInfo[m_nCurPosition].pPopupMenu->IsWindowVisible())
                {
                    for (size_t i = 0; i < m_vecTextInfo.size(); i++)
                    {
                        if (m_vecTextInfo[i].bEnablePopup)
                        {
                            if (m_vecTextInfo[i].pPopupMenu != nullptr && i != m_nCurPosition)
                            {
                                if (m_vecTextInfo[i].pPopupMenu->IsWindowVisible())
                                {
                                    m_nSubMenuState = SUBMENU_HIDE_BY_MOVING;
                                    m_vecTextInfo[i].pPopupMenu->ShowWindow(SW_HIDE);
                                    this->SetFocus();
                                    break;
                                }
                            }
                        }
                    }
                    CPoint pt;
                    GetCursorPos(&pt);
                    CRect rect = m_vecTextInfo[m_nCurPosition].rect;
                    ClientToScreen(&rect);
                    m_vecTextInfo[m_nCurPosition].pPopupMenu->TrackPopupMenu(rect, this);
                    m_vecTextInfo[m_nCurPosition].pPopupMenu->m_pMFCGridCtrl = m_pMFCGridCtrl;
                    m_vecTextInfo[m_nCurPosition].pPopupMenu->m_bSetGridKillFocus = TRUE;
                 //   m_vecTextInfo[m_nCurPosition].pPopupMenu->ShowWindow(SW_SHOWNOACTIVATE);
                    m_nSubMenuState = SUBMENU_SHOW;
                    m_bSetGridKillFocus = FALSE;
                    m_vecTextInfo[m_nCurPosition].pPopupMenu->ShowWindow(SW_SHOW);
                    
                }
            }
        }
        else
        {
            for (size_t i = 0; i < m_vecTextInfo.size(); i++)
            {
                if (m_vecTextInfo[i].bEnablePopup)
                {
                    if (m_vecTextInfo[i].pPopupMenu != nullptr)
                    {
                        if (m_vecTextInfo[i].pPopupMenu->IsWindowVisible())
                        {
                            m_nSubMenuState = SUBMENU_HIDE_BY_MOVING;
                            m_vecTextInfo[i].pPopupMenu->m_bSetGridKillFocus = FALSE;
                            m_vecTextInfo[i].pPopupMenu->ShowWindow(SW_HIDE);
                            this->SetFocus();
                            m_bSetGridKillFocus = TRUE;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    
    CWnd::OnMouseMove(nFlags, point);
}

void CSimplePopupMenu::FillRegion(CRect& rect, CDC& dc, COLORREF clr)
{
    CRgn rgn;
    rgn.DeleteObject();
    CPoint pts[4];
     pts[0] = CPoint(rect.left, rect.top);
     pts[1] = CPoint(rect.left, rect.bottom);
     pts[2] = CPoint(rect.right, rect.bottom );
     pts[3] = CPoint(rect.right, rect.top);
     rgn.CreatePolygonRgn(pts, 4, WINDING);
     CBrush brush(clr);
     dc.FillRgn(&rgn, &brush);	  
 
}

void CSimplePopupMenu::DrawText(CRect rect, CDC& dc, CString& strCaption, HFONT hFont, COLORREF clrTextBk, COLORREF clrText)
{
    CFont* pFontOld = dc.SelectObject(CFont::FromHandle(hFont));
    COLORREF clrBkOld = dc.SetBkColor(clrTextBk);
    COLORREF clrBkTextOld = dc.SetTextColor(clrText);

    rect.SetRect(rect.left + 8, rect.top, rect.right, rect.bottom);
    dc.DrawText(strCaption, &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

    dc.SelectObject(pFontOld);
    dc.SetBkColor(clrBkOld);
    dc.SetTextColor(clrBkTextOld);

}

COLORREF CSimplePopupMenu::GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai)
{
    int rButton = GetRValue(clrButton);
    int gButton = GetGValue(clrButton);
    int bButton = GetBValue(clrButton);

    int rBk = GetRValue(clrBk);
    int gBk = GetGValue(clrBk);
    int bBk = GetBValue(clrBk);

    COLORREF clrFixed = RGB(rButton * ai + rBk * (1 - ai), gButton * ai + gBk * (1 - ai), bButton * ai + bBk * (1 - ai));
    return clrFixed;
}

void CSimplePopupMenu::DrawText(CDC& dcMem, MenuList* pMenuList)
{
    if (pMenuList == nullptr)
    {
        return;
    }
    if (pMenuList->bEnableSeparator)
    {
        CPen pen(PS_SOLID, 1, m_clrTextDisabled);
        CPen* pOldPen = dcMem.SelectObject(&pen);
        CPoint ptsLine[2];

        ptsLine[0] = CPoint(pMenuList->rect.left + 33, pMenuList->rect.top + 1);
        ptsLine[1] = CPoint(pMenuList->rect.right - 2, pMenuList->rect.top + 1);
        dcMem.MoveTo(ptsLine[0]);
        dcMem.LineTo(ptsLine[1]);
        dcMem.SelectObject(pOldPen);
    }
    else
    {
        COLORREF clrBk = RGB(51, 138, 249);

        CFont* pFontOld = dcMem.SelectObject(CFont::FromHandle(m_hFont));
        dcMem.SetBkMode(TRANSPARENT);
        COLORREF clrBkTextOld = dcMem.SetTextColor(pMenuList->bEnable ? m_clrText : m_clrTextDisabled);

        CRect rect = pMenuList->rect;

        rect.SetRect(rect.left + 35, rect.top, rect.right, rect.bottom);
        dcMem.DrawText(pMenuList->str, &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

        if (pMenuList->bEnablePopup && pMenuList->bEnable)
        {
            CRgn rgn;
            rgn.DeleteObject();
            CPoint pts[3];
            pts[0] = CPoint(rect.right - 18, rect.top + 7);
            pts[1] = CPoint(rect.right - 18, rect.top + 15);
            pts[2] = CPoint(rect.right - 14, rect.top + 11 );
            rgn.CreatePolygonRgn(pts, 3, WINDING);
            CBrush brush(pMenuList->bEnable ? m_clrText : m_clrTextDisabled);
            dcMem.FillRgn(&rgn, &brush);	  
        }
        if (pMenuList->bLight)
        {
            CRect rectLight;
            rectLight.SetRect(pMenuList->rect.left + 16, pMenuList->rect.top + 8,  pMenuList->rect.left + 24, pMenuList->rect.top + 16);
            COLORREF clrLight = pMenuList->bEnable ? m_clrLight : m_clrTextDisabled;
            COLORREF clrBk = (pMenuList->nPos == m_nCurPosition && !pMenuList->bEnableSeparator && pMenuList->bEnable ) ? m_clrRegionHover : m_clrRegionDef;
            dcMem.FillSolidRect(&rectLight, clrLight);
            CPen pen1(PS_SOLID, 1, GetFixedColor(clrLight, clrBk, 0.1));
            CPen* pOldPen = dcMem.SelectObject(&pen1);
            CPoint ptsLine[2];
            ptsLine[0] = CPoint(rectLight.left, rectLight.top);
            ptsLine[1] = CPoint(rectLight.left, rectLight.top + 1);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);
     
            ptsLine[0] = CPoint(rectLight.left, rectLight.bottom - 1);
            ptsLine[1] = CPoint(rectLight.left + 1, rectLight.bottom - 1 );
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);
     
            ptsLine[0] = CPoint(rectLight.right - 1, rectLight.bottom - 1);
            ptsLine[1] = CPoint(rectLight.right - 1, rectLight.bottom - 2);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);

     
            ptsLine[0] = CPoint(rectLight.right - 1, rectLight.top);
            ptsLine[1] = CPoint(rectLight.right - 1, rectLight.top + 1);
            dcMem.MoveTo(ptsLine[0]);
            dcMem.LineTo(ptsLine[1]);
        //****************************************************************************************************
             CPen pen2(PS_SOLID, 1, GetFixedColor(clrLight, clrBk, 0.5));
             dcMem.SelectObject(&pen2);

             ptsLine[0] = CPoint(rectLight.left, rectLight.top + 1);
             ptsLine[1] = CPoint(rectLight.left, rectLight.top + 2);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.left + 1, rectLight.top);
             ptsLine[1] = CPoint(rectLight.left + 2, rectLight.top);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);


             ptsLine[0] = CPoint(rectLight.left, rectLight.bottom - 1 - 1);
             ptsLine[1] = CPoint(rectLight.left, rectLight.bottom - 1 - 2);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.left + 1, rectLight.bottom - 1);
             ptsLine[1] = CPoint(rectLight.left + 2, rectLight.bottom - 1);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right - 1, rectLight.bottom - 1 - 1);
             ptsLine[1] = CPoint(rectLight.right - 1, rectLight.bottom - 1 - 2);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right - 1 - 1, rectLight.bottom - 1);
             ptsLine[1] = CPoint(rectLight.right -1 - 2, rectLight.bottom - 1);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);


             ptsLine[0] = CPoint(rectLight.right - 1, rectLight.top + 1);
             ptsLine[1] = CPoint(rectLight.right - 1, rectLight.top + 2);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right -1 - 1, rectLight.top);
             ptsLine[1] = CPoint(rectLight.right -1 - 2, rectLight.top);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);


        // //****************************************************************************************************
             CPen pen3(PS_SOLID, 1, GetFixedColor(clrLight, clrBk, 0.9));
             dcMem.SelectObject(&pen3);

             ptsLine[0] = CPoint(rectLight.left, rectLight.top + 2);
             ptsLine[1] = CPoint(rectLight.left, rectLight.top + 3);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.left + 2, rectLight.top);
             ptsLine[1] = CPoint(rectLight.left + 3, rectLight.top);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);


             ptsLine[0] = CPoint(rectLight.left, rectLight.bottom - 1 - 2);
             ptsLine[1] = CPoint(rectLight.left, rectLight.bottom - 1 - 3);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.left + 2, rectLight.bottom - 1);
             ptsLine[1] = CPoint(rectLight.left + 3, rectLight.bottom - 1);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);


             ptsLine[0] = CPoint(rectLight.right - 1, rectLight.bottom - 1 - 2);
             ptsLine[1] = CPoint(rectLight.right - 1, rectLight.bottom - 1 - 3);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right - 1 - 2, rectLight.bottom - 1);
             ptsLine[1] = CPoint(rectLight.right - 1 - 3, rectLight.bottom - 1);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right - 1, rectLight.top + 2);
             ptsLine[1] = CPoint(rectLight.right - 1, rectLight.top + 3);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rectLight.right - 1 - 2, rectLight.top);
             ptsLine[1] = CPoint(rectLight.right - 1 - 3, rectLight.top);
             dcMem.MoveTo(ptsLine[0]);
             dcMem.LineTo(ptsLine[1]);

             dcMem.SelectObject(pOldPen);

        }
        dcMem.SelectObject(pFontOld);
        dcMem.SetTextColor(clrBkTextOld);
    }
}

void CSimplePopupMenu::SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height)
{
    MenuList textInfo;
   // textInfo.nIndex = nIndex;
    textInfo.str = strCaption;
//    textInfo.x = x;
//    textInfo.y = y;
 //   textInfo.width = width;
 //   textInfo.height = height;

    m_vecTextInfo.push_back(textInfo);

}

void CSimplePopupMenu::RoundRect(CRect& rect, CDC& dc, COLORREF color1, COLORREF color2, COLORREF color3)
{
     CPen pen1(PS_SOLID, 1, color1);
     CPen* pOldPen = dc.SelectObject(&pen1);
     CPoint ptsLine[2];

     ptsLine[0] = CPoint(rect.left, rect.top);
     ptsLine[1] = CPoint(rect.left, rect.top + 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 1, rect.bottom - 1 );
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);
//****************************************************************************************************
     CPen pen2(PS_SOLID, 1, color2);
     dc.SelectObject(&pen2);

     ptsLine[0] = CPoint(rect.left, rect.top + 1);
     ptsLine[1] = CPoint(rect.left, rect.top + 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 1, rect.top);
     ptsLine[1] = CPoint(rect.left + 2, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 1);
     ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 2, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 1, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right -1 - 2, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top + 1);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 2);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right -1 - 1, rect.top);
     ptsLine[1] = CPoint(rect.right -1 - 2, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

// //****************************************************************************************************
     CPen pen3(PS_SOLID, 1, color3);
     dc.SelectObject(&pen3);

     ptsLine[0] = CPoint(rect.left, rect.top + 2);
     ptsLine[1] = CPoint(rect.left, rect.top + 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 2, rect.top);
     ptsLine[1] = CPoint(rect.left + 3, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 2);
     ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.left + 2, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.left + 3, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
     ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 2, rect.bottom - 1);
     ptsLine[1] = CPoint(rect.right - 1 - 3, rect.bottom - 1);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1, rect.top + 2);
     ptsLine[1] = CPoint(rect.right - 1, rect.top + 3);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     ptsLine[0] = CPoint(rect.right - 1 - 2, rect.top);
     ptsLine[1] = CPoint(rect.right - 1 - 3, rect.top);
     dc.MoveTo(ptsLine[0]);
     dc.LineTo(ptsLine[1]);

     dc.SelectObject(pOldPen);

}

void CSimplePopupMenu::UpdateText(int nIndex, CString strCaption)
{
    vector<MenuList>::iterator iter;  
    m_vecTextInfo[nIndex].str = strCaption;

//     for (iter = m_vecTextInfo.begin(); iter != m_vecTextInfo.end(); iter++)  
//     {  
//         if (iter->nIndex == nIndex)
//         {
//             iter->str = strCaption;
//         }
//     }  
    Invalidate(FALSE);
}

void CSimplePopupMenu::UpdateText(CString strCaption)
{
    m_strCaption = strCaption;
    Invalidate(FALSE);
}

void CSimplePopupMenu::SetFont(HFONT hFont)
{
    m_hFont = hFont;
    Invalidate(FALSE);
}

void CSimplePopupMenu::SetRgnDefColor(COLORREF color)
{
    m_clrRegionDef = color;
    Invalidate(FALSE);
}

void CSimplePopupMenu::SetRgnHoverColor(COLORREF color)
{
    Invalidate(FALSE);
}

void CSimplePopupMenu::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

int CSimplePopupMenu::HitTest(CPoint point)
{
    for (size_t i = 0; i < m_vecTextInfo.size(); i ++)
    {   
        if (m_vecTextInfo[i].rect.PtInRect(point))
        {
            return i;
        }
    }
         
    return -1;
}

void CSimplePopupMenu::Paint(CDC& dcMem, CDC& bmpMemCheck, CDC& bmpMemUncheck)
{
    for (size_t i = 0; i < m_vecTextInfo.size(); i++)
    {
        m_vecTextInfo[i].rect.SetRect(m_vecTextInfo[i].rect.left, m_vecTextInfo[i].rect.top, m_vecTextInfo[i].rect.left + m_nWidth, m_vecTextInfo[i].rect.bottom);
        dcMem.FillSolidRect(&m_vecTextInfo[i].rect, (m_vecTextInfo[i].nPos == m_nCurPosition && !m_vecTextInfo[i].bEnableSeparator && m_vecTextInfo[i].bEnable ) ? m_clrRegionHover : m_clrRegionDef);
        DrawText(dcMem, &(m_vecTextInfo[i]));
      //  dc.BitBlt(m_vecTextInfo[i].rect.left, m_vecTextInfo[i].rect.top, m_vecTextInfo[i].rect.Width(), m_vecTextInfo[i].rect.Height(), &dcMem, m_vecTextInfo[i].rect.left, m_vecTextInfo[i].rect.top,SRCCOPY);
        if (m_vecTextInfo[i].bEnable)
        {
            if (m_vecTextInfo[i].bEnableCheck && m_vecTextInfo[i].bCheck)
            {
                dcMem.BitBlt(m_vecTextInfo[i].rect.left + 3, m_vecTextInfo[i].rect.top + 2, 20, 19, &bmpMemCheck, 0, 0,SRCCOPY);
            }
            else if (m_vecTextInfo[i].bEnableCheck && !(m_vecTextInfo[i].bCheck))
            {
                dcMem.BitBlt(m_vecTextInfo[i].rect.left + 3, m_vecTextInfo[i].rect.top + 2, 20, 19, &bmpMemUncheck, 0, 0,SRCCOPY);
            }
        }      
    }
}

BOOL CSimplePopupMenu::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

int CSimplePopupMenu::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    return MA_NOACTIVATE;
}

void CSimplePopupMenu::AddMenu(BOOL bEnablePopup, CSimplePopupMenu* pPopupMenu, BOOL bEnableCheck, BOOL bCheck, UINT nID, CString str, BOOL bEnableSeparator/* = FALSE*/, BOOL bEnable/* = TRUE*/, BOOL bLight /*= FALSE*/, BOOL bPopupWnd /*= FALSE*/)
{
    MenuList menuList;
 //   memset(&menuList, 0, sizeof(menuList));
    menuList.bEnablePopup = bEnablePopup;
    menuList.pPopupMenu = pPopupMenu;
    if (pPopupMenu != nullptr)
    {
        pPopupMenu->m_pParent = this;
        pPopupMenu->m_bBase = FALSE;
    }
    menuList.bEnableCheck = bEnableCheck;
    menuList.bCheck = bCheck;
    menuList.nID = nID;
    menuList.str = str;
    menuList.bEnableSeparator = bEnableSeparator;
    
    menuList.bEnable = bEnable;
    menuList.nPos = m_vecTextInfo.size();
    menuList.bLight = bLight;
    menuList.bPopupWnd = bPopupWnd;
    
    int Height = 0;
    for (int i = 0; i < menuList.nPos; i++)
    {
        Height += m_vecTextInfo[i].rect.Height();
    }
    menuList.rect.SetRect(0, Height, 0, bEnableSeparator ? (Height + 3) : (Height + 23));

    m_vecTextInfo.push_back(menuList);
}

void CSimplePopupMenu::TrackPopupMenu(int x, int y, CWnd* pWnd)
{
    if (pWnd != nullptr)
    {
        m_pParentEx = pWnd;
        
     //   CFont *pFont = CFont::FromHandle(m_hFont);
        CFont *pFont = GetFont();
        
        CRect rectText;
        CDC *pDC = GetDC();
        CFont *pOld = pDC->SelectObject(pFont);
        int nWidth = 0;
        int nHeight = 0;

        for (size_t i = 0; i < m_vecTextInfo.size(); i++)
        {
            pDC->DrawText(m_vecTextInfo[i].str, rectText, DT_CALCRECT | DT_LEFT);
            nWidth = (nWidth >= rectText.Width() ? nWidth : rectText.Width());
            nHeight += m_vecTextInfo[i].rect.Height();
        }
        m_nWidth = nWidth + 75;
        pDC->SelectObject(pOld);
        ReleaseDC(pDC);
        

        CRect rect; 
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);  
        if ( nHeight <= rect.Height() - (y + 1))
        {
            if (m_nWidth <= rect.Width() - (x + 1))
            {
                MoveWindow(x + 1, y + 1, m_nWidth,  nHeight, FALSE);
            }
            else
            {
                MoveWindow(x - 1 - m_nWidth, y + 1, m_nWidth,  nHeight, FALSE);
            }
            
        }
        else
        {
            if (m_nWidth <= rect.Width() - (x + 1))
            {
                MoveWindow(x + 1, y - 1 - nHeight, m_nWidth,  nHeight, FALSE);
            }
            else
            {
                MoveWindow(x - 1 - m_nWidth, y - 1 - nHeight, m_nWidth,  nHeight, FALSE);
            }
            
        }  

        ShowWindow(SW_SHOW);
    }

}

void CSimplePopupMenu::TrackPopupMenu(CRect rect, CWnd* pWnd)
{
    if (pWnd != nullptr)
    {
     //   CFont *pFont = CFont::FromHandle(m_hFont);
        CFont *pFont = GetFont();
        CRect rectText;
        CDC *pDC = GetDC();
        CFont *pOld = pDC->SelectObject(pFont);
        int nWidth = 0;
        int nHeight = 0;

        for (size_t i = 0; i < m_vecTextInfo.size(); i++)
        {
            pDC->DrawText(m_vecTextInfo[i].str, rectText, DT_CALCRECT | DT_LEFT);
            nWidth = (nWidth >= rectText.Width() ? nWidth : rectText.Width());
            nHeight += m_vecTextInfo[i].rect.Height();
        }
        m_nWidth = nWidth + 75;
        pDC->SelectObject(pOld);
        ReleaseDC(pDC);


        CRect rectDesktop; 
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);  

        if ( nHeight <= rectDesktop.Height() - rect.top)
        {
            if (m_nWidth <= rectDesktop.Width() - (rect.right + 1))
            {
                MoveWindow(rect.right + 1, rect.top, m_nWidth,  nHeight, FALSE);
            }
            else
            {
                MoveWindow(rect.left - 1 - m_nWidth, rect.top, m_nWidth,  nHeight, FALSE);
            }

        }
        else
        {
            int nY = rect.bottom - nHeight;
            if (rect.bottom <= nHeight)
            {
                nY = 10;
            }

            if (m_nWidth <= rectDesktop.Width() - (rect.right + 1))
            {
                MoveWindow(rect.right + 1, nY, m_nWidth,  nHeight, FALSE);
            }
            else
            {
                MoveWindow(rect.left - 1 - m_nWidth, nY, m_nWidth,  nHeight, FALSE);
            }
        }          
    }
}

void CSimplePopupMenu::Clear()
{
    m_vecTextInfo.clear();
}

BOOL CSimplePopupMenu::IsGo(int x, int y)
{
    if (m_nX == x && m_nY == y)
    {
        return FALSE;
    }
    else
    {
        m_nX = x;
        m_nY = y;
        return TRUE;
    }
}

BOOL CSimplePopupMenu::IsGoEx(int x, int y)
{
    if (m_nX1 == x && m_nY1 == y)
    {
        return FALSE;
    }
    else
    {
        m_nX1 = x;
        m_nY1 = y;
        return TRUE;
    }
}

void CSimplePopupMenu::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (NULL != GetParent())
    {
        UINT nID = 1;
        if (m_nCurPosition >= 0)
        {
            if (!m_vecTextInfo[m_nCurPosition].bEnableSeparator && m_vecTextInfo[m_nCurPosition].bEnable)
            {
                nID = m_vecTextInfo[m_nCurPosition].nID;
                if (!m_vecTextInfo[m_nCurPosition].bEnableCheck && !m_vecTextInfo[m_nCurPosition].bEnablePopup)
                {
                    ShowWindow(SW_HIDE);
                }
                else
                {
                    m_vecTextInfo[m_nCurPosition].bCheck = !m_vecTextInfo[m_nCurPosition].bCheck;
                }

                GetParent()->SendMessage(WM_COMMAND, nID, 0);
                Invalidate(FALSE);
            }
        }
        

    }

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimplePopupMenu::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);

    if (nState == WA_INACTIVE && m_nSubMenuState != SUBMENU_SHOW)
    {
        for (size_t i = 0; i < m_vecTextInfo.size(); i++)
        {
            if (m_vecTextInfo[i].bEnablePopup)
            {
                if (m_vecTextInfo[i].pPopupMenu != nullptr)
                {
                    if (m_vecTextInfo[i].pPopupMenu->IsWindowVisible())
                    {
                        m_vecTextInfo[i].pPopupMenu->ShowWindow(SW_HIDE);
                    }
                }
            }
        }
        if (m_pParent != nullptr)
        {
            m_pParent->ShowWindowEx(SW_HIDE);
            m_pParent->SetSubMenuState(SUBMENU_HIDE_DEFAUT);
        }
        ShowWindow(SW_HIDE);
    }

}
void CSimplePopupMenu::ShowWindowEx(int nCmdShow)
{
    if (m_nSubMenuState != SUBMENU_HIDE_BY_MOVING)
    {
        ShowWindow(nCmdShow);
        if (m_pParent != nullptr)
        {
            m_pParent->ShowWindowEx(SW_HIDE);
            m_pParent->SetSubMenuState(SUBMENU_HIDE_DEFAUT);
        }
    }
}

void CSimplePopupMenu::SetSubMenuState(UINT nState)
{
    m_nSubMenuState = nState;
}

CSimplePopupMenu* CSimplePopupMenu::CreateSubMenu(CWnd * pParentWnd)
{
    CSimplePopupMenu* pCSimplePopupMenu = new CSimplePopupMenu();
    if (pCSimplePopupMenu != NULL)
    {
        pCSimplePopupMenu->Create(WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL);
        pCSimplePopupMenu->m_pParentWnd = pParentWnd;
     //   pCSimplePopupMenu->SetSub();
        pCSimplePopupMenu->m_bNew = TRUE;
    }
    return pCSimplePopupMenu;
}

BOOL CSimplePopupMenu::ShowWindow(int nCmdShow)
{
    if (SW_HIDE == nCmdShow && m_bBase)
    {
        for (size_t i = 0; i < m_vecTextInfo.size(); i++)
        {
            if (m_vecTextInfo[i].pPopupMenu != NULL && m_vecTextInfo[i].pPopupMenu->m_bNew)
            {
                delete m_vecTextInfo[i].pPopupMenu;
                m_vecTextInfo[i].pPopupMenu = NULL;
            }
        }
    }
    if (SW_SHOW == nCmdShow)
    {
        m_bSetGridKillFocus = TRUE;
    }
    return CWnd::ShowWindow(nCmdShow);
}

void CSimplePopupMenu::SetSub()
{
    m_bBase = FALSE;
}

CWnd* CSimplePopupMenu::GetParent()
{
    return m_pParentWnd;
}

void CSimplePopupMenu::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    TCHAR szBuff[256];
    GetClassName(pOldWnd->GetSafeHwnd(), szBuff, sizeof(szBuff));
    CString str = szBuff;
    if (str.Trim() == "MFCGridCtrl" && m_bBase && NULL == m_pMFCGridCtrl)
    {
        m_pMFCGridCtrl = pOldWnd;
    }
}

void CSimplePopupMenu::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    if (m_bSetGridKillFocus)
    {
        TCHAR szBuff[256];
        GetClassName(pNewWnd->GetSafeHwnd(), szBuff, sizeof(szBuff));
        CString str = szBuff;

        if (m_pMFCGridCtrl != NULL)
        {
            CGridCtrlEx* pGridCtrlEx = (CGridCtrlEx*)m_pMFCGridCtrl;
            if (pGridCtrlEx != NULL)
            {
                if (IsWindow(pGridCtrlEx->GetSafeHwnd()))
                {
                    if (pGridCtrlEx->GetActionOnFocus())
                    {  
                        CPoint point;
                        GetCursorPos(&point);
                        if (m_nCurPosition >= 0)
                        {
                            if (m_vecTextInfo[m_nCurPosition].bPopupWnd)
                            {
                                pGridCtrlEx->SetCurHighlightMode(HIGHLIGHT_KILLFOCUS);
                                return;
                            }
                        }

                        if (WindowFromPoint(point) != pGridCtrlEx)
                        {
                            pGridCtrlEx->SetCurHighlightMode(HIGHLIGHT_KILLFOCUS);
                        }
                    }                
                }
            }
        }
    }
}

