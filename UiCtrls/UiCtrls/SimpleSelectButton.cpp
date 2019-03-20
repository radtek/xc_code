
//*****************************************************
//********    SimpleSelectButton By YaoShuLong 2017.10
//*****************************************************

#include "stdafx.h"
#include "SimpleSelectButton.h"

#define POS_BUTTON     0
#define POS_TRANGLE   1
#define POS_OUT             2


// CSimpleSelectButton

IMPLEMENT_DYNAMIC(CSimpleSelectButton, CWnd)

CSimpleSelectButton::CSimpleSelectButton() :
m_bMouseEnter(FALSE),
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE),
m_strCaption(_T(""))
{
    m_bGetPixel = TRUE;
    RegisterWindowClass();
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
    m_hFont = (HFONT)m_font.GetSafeHandle();
    m_hFont0 = (HFONT)m_font.GetSafeHandle();
    m_hFont1 = (HFONT)m_font.GetSafeHandle();
    m_bLeftTop = TRUE;
    m_bLeftBottom = TRUE;
    m_bRightBottom = TRUE;
    m_bRightTop = TRUE;
    m_nTextPosition = 0;
    m_bEnableHoverLeaveNotify = FALSE;
    m_bEnableLDBClick = TRUE;

    m_clrRegionBlueDef = RGB(61, 91, 153);
    m_clrRegionBlueHover = RGB(102, 182, 240);
    m_clrRegionBlueDisabled = RGB(30, 45, 76);

    m_clrRegionGrayDef = RGB(66, 66, 66);
    m_clrRegionGrayHover = RGB(105, 105, 105);

    m_clrRegionRedDef = RGB(153, 61, 61);
    m_clrRegionRedHover = RGB(228, 106, 94);
    m_clrRegionRedDisabled = RGB(76, 30, 30);

    m_clrRegionGreenDef = RGB(87, 153, 61);
    m_clrRegionGreenHover = RGB(113, 200, 85);
    m_clrRegionGreenDisabled = RGB(43, 76, 30);

    m_clrRegionDef = 0;

    m_clrRegionHover = 0;

    m_clrRegionDisabled = 0;

    m_clrText = RGB(255, 255, 255);

    m_nBtnColor = 0;

    m_bEnable = TRUE;
    m_bFocus = FALSE;
    m_bEnableGetFocusToDrawPoints = FALSE;
    m_bChangeCursor = FALSE;

    m_clrLeftTop1 = 0;
    m_clrLeftTop21 = 0;
    m_clrLeftTop22 = 0;
    m_clrLeftTop31 = 0;
    m_clrLeftTop32 = 0;

    m_clrLeftBottom1 = 0;
    m_clrLeftBottom21 = 0;
    m_clrLeftBottom22 = 0;
    m_clrLeftBottom31 = 0;
    m_clrLeftBottom32 = 0;

    m_clrRightBottom1 = 0;
    m_clrRightBottom21 = 0;
    m_clrRightBottom22 = 0;
    m_clrRightBottom31 = 0;
    m_clrRightBottom32 = 0;

    m_clrRightTop1 = 0;
    m_clrRightTop21 = 0;
    m_clrRightTop22 = 0;
    m_clrRightTop31 = 0;
    m_clrRightTop32 = 0;
    m_bEnableMultiBkColor = FALSE;
    m_nTrangleRegionWidth = 19;

    m_nPos = POS_OUT;
    m_CurSelectBtnStatus.bIsLastUseEdit = FALSE;
    m_CurSelectBtnStatus.bIsLastUseList = FALSE;
    m_CurSelectBtnStatus.listinfo.nID = 0;
}

CSimpleSelectButton::~CSimpleSelectButton()
{
}


BEGIN_MESSAGE_MAP(CSimpleSelectButton, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEHOVER()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_MESSAGE(MESSAGE_BUTTON, &CSimpleSelectButton::OnMessageFromDlg)
END_MESSAGE_MAP()

// CSimpleSelectButton 消息处理程序
int CSimpleSelectButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_selectButtonPopupWindow.Create(this);  
    m_selectButtonPopupWindow.m_pWndParent = this;

    return 0;
}

void CSimpleSelectButton::OnPaint()
{
    CPaintDC dc(this);

    CRect rCl;
    GetClientRect(&rCl);

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    CPoint point(m_rectTrangleRegion.left + 6, m_rectTrangleRegion.Height() / 2 - 2);

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

    if (m_bEnable)
    {
        if (m_bMouseIn)
        {
            if (m_bLMouseDown)
            {      
                FillRegion(rCl, dcMem, m_clrRegionDef);
                RoundRectEx(rCl, dcMem, m_clrRegionDef, RGB(18, 20, 22), m_bLeftTop, m_bLeftBottom, m_bRightBottom, m_bRightTop);
                DrawText(m_rectButton, dcMem, m_strCaption, m_hFont, m_hFont0, m_hFont1, m_clrRegionDef, m_clrText);
                DrawTrangle(point, dcMem, m_clrText);
            } 
            else
            {
                if (POS_TRANGLE == m_nPos)
                {
                    FillRegion(m_rectTrangleRegion, dcMem, m_clrRegionHover);
                    FillRegion(m_rectButton, dcMem, m_clrRegionDef);
                    DrawText(m_rectButton, dcMem, m_strCaption, m_hFont, m_hFont0, m_hFont1, m_clrRegionDef, m_clrText);
                    DrawTrangle(point, dcMem, m_clrText);
                    RoundRectEx(rCl, dcMem, m_clrRegionHover, RGB(18, 20, 22), FALSE, FALSE, m_bRightBottom, m_bRightTop);
                    RoundRectEx(rCl, dcMem, m_clrRegionDef, RGB(18, 20, 22), m_bLeftTop, m_bLeftBottom, FALSE, FALSE);

                }
                else
                {
                    FillRegion(m_rectTrangleRegion, dcMem, m_clrRegionDef);
                    FillRegion(m_rectButton, dcMem, m_clrRegionHover);
                    DrawText(m_rectButton, dcMem, m_strCaption, m_hFont, m_hFont0, m_hFont1, m_clrRegionHover, m_clrText);
                    DrawTrangle(point, dcMem, m_clrText);
                    RoundRectEx(rCl, dcMem, m_clrRegionHover, RGB(18, 20, 22), m_bLeftTop, m_bLeftBottom, FALSE, FALSE);
                    RoundRectEx(rCl, dcMem, m_clrRegionDef, RGB(18, 20, 22), FALSE, FALSE, m_bRightBottom, m_bRightTop);
                }                             
            }
        } 
        else
        {    
            FillRegion(rCl, dcMem, m_clrRegionDef);
            RoundRectEx(rCl, dcMem, m_clrRegionDef, RGB(18, 20, 22), m_bLeftTop, m_bLeftBottom, m_bRightBottom, m_bRightTop);
            DrawText(m_rectButton, dcMem, m_strCaption, m_hFont, m_hFont0, m_hFont1, m_clrRegionDef, m_clrText);
            DrawTrangle(point, dcMem, m_clrText);
        }
    } 
    else
    {
        FillRegion(rCl, dcMem, m_clrRegionDisabled);
        RoundRectEx(rCl, dcMem, m_clrRegionDisabled, RGB(18, 20, 22), m_bLeftTop, m_bLeftBottom, m_bRightBottom, m_bRightTop);
        DrawText(m_rectButton, dcMem, m_strCaption, m_hFont, m_hFont0, m_hFont1, m_clrRegionDisabled, RGB(189, 189, 189));
        DrawTrangle(point, dcMem, RGB(189, 189, 189));
    }

    dc.BitBlt(rCl.left,rCl.top,rCl.Width(),rCl.Height(),&dcMem,rCl.left,rCl.top,SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();
}

//BOOL CSimpleSelectButton::Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
BOOL CSimpleSelectButton::Create(UINT dwStyle, UINT dwBtnColor, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption, CString strHint, UINT nTextPosition, BOOL bLeftTop, BOOL bLeftBottom, BOOL bRightBottom, BOOL bRightTop)
{
    m_selectButtonPopupWindow.m_strHint = strHint;
    
    m_strCaption = strCaption;
    m_bLeftTop = bLeftTop;
    m_bLeftBottom = bLeftBottom;
    m_bRightBottom = bRightBottom;
    m_bRightTop = bRightTop;
    m_nTextPosition = nTextPosition;

    switch (dwBtnColor)
    {
    case BUTTON_BLUE:
        {
            m_clrRegionDef = m_clrRegionBlueDef;
            m_clrRegionHover = m_clrRegionBlueHover;
            m_clrRegionDisabled = m_clrRegionBlueDisabled;
            break;
        }
    case BUTTON_GRAY:
        {
            m_clrRegionDef = m_clrRegionGrayDef;
            m_clrRegionHover = m_clrRegionGrayHover;
            m_clrRegionDisabled = m_clrRegionGrayDef;
            break;
        }
    case BUTTON_RED:
        {
            m_clrRegionDef = m_clrRegionRedDef;
            m_clrRegionHover = m_clrRegionRedHover;
            m_clrRegionDisabled = m_clrRegionRedDisabled;
            break;
        }
    case BUTTON_GREEN:
        {
            m_clrRegionDef = m_clrRegionGreenDef;
            m_clrRegionHover = m_clrRegionGreenHover;
            m_clrRegionDisabled = m_clrRegionGreenDisabled;
            break;
        }


    default:
        break;
    }

    return CWnd::Create(SimpleSelectButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleSelectButton::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleSelectButton_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleSelectButton_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleSelectButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd() && m_bEnableGetFocusToDrawPoints)
    {
        SetFocus();
    }
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);

    if (POS_TRANGLE == m_nPos)
    {
        if (m_selectButtonPopupWindow.IsWindowVisible())
        {
            m_selectButtonPopupWindow.ShowWindow(SW_HIDE);
        }
        else
        {
            //显示下拉框
            CRect rect;
            GetClientRect(&rect);
            ClientToScreen(&rect);
            m_selectButtonPopupWindow.m_listbox.ResetContent();
            m_selectButtonPopupWindow.m_nItemCount = m_vecListInfo.size();
            for (size_t i = 0; i < m_vecListInfo.size(); i++)
            {
                m_selectButtonPopupWindow.m_listbox.AddString(m_vecListInfo[i].strText);
                m_selectButtonPopupWindow.m_listbox.SetItemDataPtr(i, &m_vecListInfo[i]);
                int nItemCount = m_selectButtonPopupWindow.m_listbox.GetCount();
                m_selectButtonPopupWindow.m_listbox.SetItemHeight(nItemCount - 1, 20);
            }

            //   m_selectButtonPopupWindow.SetWindowPos(&wndTop, rect.right - 110, rect.bottom + 1, 110/*rect.Width()*/, m_selectButtonPopupWindow.m_nItemCount*20 + 26, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
            m_selectButtonPopupWindow.MoveWindow(rect.right - 110, rect.bottom + 1, 110/*rect.Width()*/, m_selectButtonPopupWindow.m_nItemCount*20 + 26, FALSE);
            m_selectButtonPopupWindow.ShowWindow(SW_SHOW);
        }
    }


    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleSelectButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLMouseDown)
    {
        if (POS_BUTTON == m_nPos)
        {
            NMHDR hdr;
            hdr.idFrom = GetDlgCtrlID();
            hdr.hwndFrom = GetSafeHwnd();
            hdr.code = NM_CLICK;
            GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        }
        
        m_bLMouseDown = FALSE;
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleSelectButton::OnMouseLeave()
{
    m_nPos = POS_OUT;
    m_bMouseEnter = FALSE;
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    if (!m_bLMouseDown)
    {
        Invalidate(FALSE);
    }

    if (m_bEnableHoverLeaveNotify)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_RCLICK;//用NM_RCLICK代替leave消息
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);        
    }   
    CWnd::OnMouseLeave();
}


void CSimpleSelectButton::OnMouseMove(UINT nFlags, CPoint point)
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
//         if (m_bEnableHoverLeaveNotify)
//         {
//             NMHDR hdr;
//             hdr.idFrom = GetDlgCtrlID();
//             hdr.hwndFrom = GetSafeHwnd();
//             hdr.code = NM_HOVER;
//             GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
//         }       
    }
    int nPosOld = m_nPos;
    m_nPos = GetCurPos(point);

    if (m_nPos != nPosOld)
    {
        Invalidate(FALSE);
    }
 
    CWnd::OnMouseMove(nFlags, point);
}

void CSimpleSelectButton::FillRegion(CRect& rect, CDC& dc, COLORREF clr)
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

void CSimpleSelectButton::DrawTrangle(CPoint leftTopPoint, CDC& dc, COLORREF clrTrangle)
{
    CPen penLeftTop1(PS_SOLID, 1, RGB(189, 189, 189));
    dc.SelectObject(&penLeftTop1);
    CPoint ptsLine[2];
    ptsLine[0] = CPoint(m_rectTrangleRegion.left, m_rectTrangleRegion.top + 2);
    ptsLine[1] = CPoint(m_rectTrangleRegion.left, m_rectTrangleRegion.bottom - 2);
    dc.MoveTo(ptsLine[0]);
    dc.LineTo(ptsLine[1]);

    CRgn rgn;
    rgn.DeleteObject();
    CPoint pts[3];

    pts[0] = CPoint(leftTopPoint.x, leftTopPoint.y);
    pts[1] = CPoint(leftTopPoint.x + 9, leftTopPoint.y);
    pts[2] = CPoint(leftTopPoint.x + 4, leftTopPoint.y + 5);

    rgn.CreatePolygonRgn(pts, 3, WINDING);
    CBrush brush(clrTrangle);
    dc.FillRgn(&rgn, &brush);	  
}

void CSimpleSelectButton::DrawText(CRect& rect, CDC& dc, CString& strCaption, HFONT hFont, HFONT hFont0, HFONT hFont1, COLORREF clrTextBk, COLORREF clrText)
{

    CFont* pFontOld = dc.SelectObject(CFont::FromHandle(hFont));
    COLORREF clrBkOld = dc.SetBkColor(clrTextBk);
    COLORREF clrBkTextOld = dc.SetTextColor(clrText);
    
    
    if (strCaption.IsEmpty() && m_vecTextInfo.size() > 1)
    {
        pFontOld = dc.SelectObject(CFont::FromHandle(hFont0));
        CRect rectW;
        rectW.SetRect(m_vecTextInfo[0].x, m_vecTextInfo[0].y, m_vecTextInfo[0].width + m_vecTextInfo[0].x , m_vecTextInfo[0].height + m_vecTextInfo[0].y);
        dc.DrawText(m_vecTextInfo[0].str, &rectW, m_nTextPosition|DT_VCENTER|DT_SINGLELINE);

        pFontOld = dc.SelectObject(CFont::FromHandle(hFont1));
        rectW.SetRect(m_vecTextInfo[1].x, m_vecTextInfo[1].y, m_vecTextInfo[1].width + m_vecTextInfo[1].x , m_vecTextInfo[1].height + m_vecTextInfo[1].y);
        dc.DrawText(m_vecTextInfo[1].str, &rectW, m_nTextPosition|DT_VCENTER|DT_SINGLELINE);
    }
    else
    {
        dc.DrawText(strCaption, &rect, m_nTextPosition|DT_VCENTER|DT_SINGLELINE);
    }

    if (m_bFocus && m_bEnableGetFocusToDrawPoints)
    {
        CPen pen(PS_SOLID, 1, clrText);
        dc.SelectObject(&pen);
        CPoint ptsLine[2];

        int nY = rect.top + 2;
        while (nY < rect.bottom - 3)
        {
            ptsLine[0] = CPoint(rect.left + 2, nY);
            ptsLine[1] = CPoint(rect.left + 2, nY + 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);
            nY += 2;
        }

        nY = rect.top + 2;
        while (nY < rect.bottom - 3)
        {
            ptsLine[0] = CPoint(rect.right - 3, nY);
            ptsLine[1] = CPoint(rect.right - 3, nY + 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);
            nY += 2;
        }

        int nX = rect.left + 2;
        while (nX < rect.right - 3)
        {
            ptsLine[0] = CPoint(nX, rect.top + 2);
            ptsLine[1] = CPoint(nX + 1, rect.top + 2);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);
            nX += 2;
        }

        nX = rect.left + 2;
        while (nX < rect.right - 3)
        {
            ptsLine[0] = CPoint(nX, rect.bottom - 3);
            ptsLine[1] = CPoint(nX + 1, rect.bottom - 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);
            nX += 2;
        }
    }
   
    dc.SelectObject(pFontOld);
    dc.SetBkColor(clrBkOld);
    dc.SetTextColor(clrBkTextOld);
}

// void CSimpleSelectButton::InitialSettings(CString strCaption, COLORREF clrRegionDef, COLORREF clrRegionHover, COLORREF clrText, HFONT hFont)
// {
//     m_strCaption = strCaption;
//     if (NULL != hFont)
//     {
//         m_hFont = hFont;
//     }
//     
//     m_clrRegionDef = clrRegionDef;
//     m_clrRegionHover = clrRegionHover;
//     m_clrText = clrText;
// }

void CSimpleSelectButton::SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height)
{
    TextInfoo textInfo;
    textInfo.nIndex = nIndex;
    textInfo.str = strCaption;
    textInfo.x = x;
    textInfo.y = y;
    textInfo.width = width;
    textInfo.height = height;

    m_vecTextInfo.push_back(textInfo);

}

COLORREF CSimpleSelectButton::GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai)
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

void CSimpleSelectButton::RoundRectEx(CRect& rect, CDC& dc, COLORREF clrButton, COLORREF clrBk, BOOL bLeftTop, BOOL bLeftBottom, BOOL bRightBottom, BOOL bRightTop)
{
    if (!bLeftTop && !bLeftBottom && !bRightBottom && !bRightTop)
    {
        return;
    }
    if (m_bEnableMultiBkColor)
    {
        CRect rectEx;
        GetWindowRect(&rectEx);
        GetParent()->ScreenToClient(&rectEx);

        if (m_bGetPixel)
        {
            CDC* pDCParent = GetParent()->GetDC();
            if (pDCParent != nullptr)
            {
                m_clrLeftTop1 = pDCParent->GetPixel(rectEx.left, rectEx.top);
                m_clrLeftTop21 = pDCParent->GetPixel(rectEx.left + 1, rectEx.top);
                m_clrLeftTop22 = pDCParent->GetPixel(rectEx.left, rectEx.top + 1);
                m_clrLeftTop31 = pDCParent->GetPixel(rectEx.left + 2, rectEx.top);
                m_clrLeftTop32 = pDCParent->GetPixel(rectEx.left, rectEx.top + 2);

                m_clrLeftBottom1 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 1);
                m_clrLeftBottom21 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 2);
                m_clrLeftBottom22 = pDCParent->GetPixel(rectEx.left + 1, rectEx.bottom - 1);
                m_clrLeftBottom31 = pDCParent->GetPixel(rectEx.left, rectEx.bottom - 3);
                m_clrLeftBottom32 = pDCParent->GetPixel(rectEx.left + 2, rectEx.bottom - 1);

                m_clrRightBottom1 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 1);
                m_clrRightBottom21 = pDCParent->GetPixel(rectEx.right - 2, rectEx.bottom - 1);
                m_clrRightBottom22 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 2);
                m_clrRightBottom31 = pDCParent->GetPixel(rectEx.right - 3, rectEx.bottom - 1);
                m_clrRightBottom32 = pDCParent->GetPixel(rectEx.right - 1, rectEx.bottom - 3);

                m_clrRightTop1 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top);
                m_clrRightTop21 = pDCParent->GetPixel(rectEx.right - 2, rectEx.top);
                m_clrRightTop22 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top + 1);
                m_clrRightTop31 = pDCParent->GetPixel(rectEx.right - 3, rectEx.top);
                m_clrRightTop32 = pDCParent->GetPixel(rectEx.right - 1, rectEx.top + 2);
            }
            GetParent()->ReleaseDC(pDCParent);

            m_bGetPixel = FALSE;
        }

        CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
        CPen* pOldPen = dc.SelectObject(&pen);
        CPoint ptsLine[2];

        if (bLeftTop)
        {
            CPen penLeftTop1(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftTop1, 0.1));
            dc.SelectObject(&penLeftTop1);
            ptsLine[0] = CPoint(rect.left, rect.top);
            ptsLine[1] = CPoint(rect.left, rect.top + 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftTop21(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftTop21, 0.5));
            dc.SelectObject(&penLeftTop21);
            ptsLine[0] = CPoint(rect.left + 1, rect.top);
            ptsLine[1] = CPoint(rect.left + 2, rect.top);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftTop22(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftTop22, 0.5));
            dc.SelectObject(&penLeftTop22);
            ptsLine[0] = CPoint(rect.left, rect.top + 1);
            ptsLine[1] = CPoint(rect.left, rect.top + 2);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftTop31(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftTop31, 0.9));
            dc.SelectObject(&penLeftTop31);
            ptsLine[0] = CPoint(rect.left + 2, rect.top);
            ptsLine[1] = CPoint(rect.left + 3, rect.top);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftTop32(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftTop32, 0.9));
            dc.SelectObject(&penLeftTop32);
            ptsLine[0] = CPoint(rect.left, rect.top + 2);
            ptsLine[1] = CPoint(rect.left, rect.top + 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);       
        }

        if (bLeftBottom)
        {
            CPen penLeftBottom1(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftBottom1, 0.1));
            dc.SelectObject(&penLeftBottom1);
            ptsLine[0] = CPoint(rect.left, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.left + 1, rect.bottom - 1 );
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);  

            CPen penLeftBottom21(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftBottom21, 0.5));
            dc.SelectObject(&penLeftBottom21);
            ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 1);
            ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 2);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftBottom22(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftBottom22, 0.5));
            dc.SelectObject(&penLeftBottom22);
            ptsLine[0] = CPoint(rect.left + 1, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.left + 2, rect.bottom - 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftBottom31(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftBottom31, 0.9));
            dc.SelectObject(&penLeftBottom31);
            ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 2);
            ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penLeftBottom32(PS_SOLID, 1, GetFixedColor(clrButton, m_clrLeftBottom32, 0.9));
            dc.SelectObject(&penLeftBottom32);
            ptsLine[0] = CPoint(rect.left + 2, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.left + 3, rect.bottom - 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);              
        }

        if (bRightBottom)
        {
            CPen penRightBottom1(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightBottom1, 0.1));
            dc.SelectObject(&penRightBottom1);
            ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 2);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightBottom21(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightBottom21, 0.5));
            dc.SelectObject(&penRightBottom21);
            ptsLine[0] = CPoint(rect.right - 1 - 1, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.right -1 - 2, rect.bottom - 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightBottom22(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightBottom22, 0.5));
            dc.SelectObject(&penRightBottom22);
            ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 2);
            ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightBottom31(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightBottom31, 0.9));
            dc.SelectObject(&penRightBottom31);
            ptsLine[0] = CPoint(rect.right - 1 - 2, rect.bottom - 1);
            ptsLine[1] = CPoint(rect.right - 1 - 3, rect.bottom - 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightBottom32(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightBottom32, 0.9));
            dc.SelectObject(&penRightBottom32);
            ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
            ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);
        }

        if (bRightTop)
        {
            CPen penRightTop1(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightTop1, 0.1));
            dc.SelectObject(&penRightTop1);
            ptsLine[0] = CPoint(rect.right - 1, rect.top);
            ptsLine[1] = CPoint(rect.right - 1, rect.top + 1);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightTop21(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightTop21, 0.5));
            dc.SelectObject(&penRightTop21);
            ptsLine[0] = CPoint(rect.right -1 - 1, rect.top);
            ptsLine[1] = CPoint(rect.right -1 - 2, rect.top);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightTop22(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightTop22, 0.5));
            dc.SelectObject(&penRightTop22);
            ptsLine[0] = CPoint(rect.right - 1, rect.top + 1);
            ptsLine[1] = CPoint(rect.right - 1, rect.top + 2);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightTop31(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightTop31, 0.9));
            dc.SelectObject(&penRightTop31);
            ptsLine[0] = CPoint(rect.right - 1 - 2, rect.top);
            ptsLine[1] = CPoint(rect.right - 1 - 3, rect.top);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);

            CPen penRightTop32(PS_SOLID, 1, GetFixedColor(clrButton, m_clrRightTop32, 0.9));
            dc.SelectObject(&penRightTop32);
            ptsLine[0] = CPoint(rect.right - 1, rect.top + 2);
            ptsLine[1] = CPoint(rect.right - 1, rect.top + 3);
            dc.MoveTo(ptsLine[0]);
            dc.LineTo(ptsLine[1]);      
        }
        dc.SelectObject(pOldPen);

    }
    else
    {
        CPen pen1(PS_SOLID, 1, GetFixedColor(clrButton, clrBk, 0.1));
        CPen* pOldPen = dc.SelectObject(&pen1);
        CPoint ptsLine[2];
        if (bLeftTop)
         {
             ptsLine[0] = CPoint(rect.left, rect.top);
             ptsLine[1] = CPoint(rect.left, rect.top + 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }
     
         if (bLeftBottom)
         {
             ptsLine[0] = CPoint(rect.left, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.left + 1, rect.bottom - 1 );
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }
     
         if (bRightBottom)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 2);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }
     
         if (bRightTop)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.top);
             ptsLine[1] = CPoint(rect.right - 1, rect.top + 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }
    //****************************************************************************************************
         CPen pen2(PS_SOLID, 1, GetFixedColor(clrButton, clrBk, 0.5));
         dc.SelectObject(&pen2);

         if (bLeftTop)
         {
             ptsLine[0] = CPoint(rect.left, rect.top + 1);
             ptsLine[1] = CPoint(rect.left, rect.top + 2);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.left + 1, rect.top);
             ptsLine[1] = CPoint(rect.left + 2, rect.top);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

         if (bLeftBottom)
         {
             ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 1);
             ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 2);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.left + 1, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.left + 2, rect.bottom - 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }   

         if (bRightBottom)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 1);
             ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.right - 1 - 1, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.right -1 - 2, rect.bottom - 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

         if (bRightTop)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.top + 1);
             ptsLine[1] = CPoint(rect.right - 1, rect.top + 2);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.right -1 - 1, rect.top);
             ptsLine[1] = CPoint(rect.right -1 - 2, rect.top);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

    // //****************************************************************************************************
         CPen pen3(PS_SOLID, 1, GetFixedColor(clrButton, clrBk, 0.9));
         dc.SelectObject(&pen3);

         if (bLeftTop)
         {
             ptsLine[0] = CPoint(rect.left, rect.top + 2);
             ptsLine[1] = CPoint(rect.left, rect.top + 3);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.left + 2, rect.top);
             ptsLine[1] = CPoint(rect.left + 3, rect.top);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

         if (bLeftBottom)
         {
             ptsLine[0] = CPoint(rect.left, rect.bottom - 1 - 2);
             ptsLine[1] = CPoint(rect.left, rect.bottom - 1 - 3);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.left + 2, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.left + 3, rect.bottom - 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

         if (bRightBottom)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.bottom - 1 - 2);
             ptsLine[1] = CPoint(rect.right - 1, rect.bottom - 1 - 3);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.right - 1 - 2, rect.bottom - 1);
             ptsLine[1] = CPoint(rect.right - 1 - 3, rect.bottom - 1);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }

         if (bRightTop)
         {
             ptsLine[0] = CPoint(rect.right - 1, rect.top + 2);
             ptsLine[1] = CPoint(rect.right - 1, rect.top + 3);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);

             ptsLine[0] = CPoint(rect.right - 1 - 2, rect.top);
             ptsLine[1] = CPoint(rect.right - 1 - 3, rect.top);
             dc.MoveTo(ptsLine[0]);
             dc.LineTo(ptsLine[1]);
         }
         dc.SelectObject(pOldPen);

     
    }
    ////////////////////////////////////////////////
     
     
}

void CSimpleSelectButton::UpdateText(int nIndex, CString strCaption)
{
    vector<TextInfoo>::iterator iter;  
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

void CSimpleSelectButton::GetWindowText(int nIndex, CString& string)
{
    string = m_vecTextInfo[nIndex].str;
}

void CSimpleSelectButton::UpdateText(CString strCaption)
{
    m_strCaption = strCaption;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetFont(HFONT hFont)
{
    m_hFont = hFont;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetFont(HFONT hFont0, HFONT hFont1)
{
    m_hFont0 = hFont0;
    m_hFont1 = hFont1;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetRgnDefColor(COLORREF color)
{
    m_clrRegionDef = color;
    Invalidate(TRUE);
}

void CSimpleSelectButton::SetRgnHoverColor(COLORREF color)
{
    m_clrRegionHover= color;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetDisabledColor(COLORREF color)
{
    m_clrRegionDisabled = color;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate(FALSE);
}

BOOL CSimpleSelectButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleSelectButton::GetWindowText(CString& rString)
{
    rString = m_strCaption;
}

void CSimpleSelectButton::SetColorStyle(UINT nColor)
{
    switch (nColor)
    {
    case BUTTON_BLUE:
        {
            m_clrRegionDef = m_clrRegionBlueDef;
            m_clrRegionHover = m_clrRegionBlueHover;
            break;
        }
    case BUTTON_GRAY:
        {
            m_clrRegionDef = m_clrRegionGrayDef;
            m_clrRegionHover = m_clrRegionGrayHover;
            break;
        }
    case BUTTON_RED:
        {
            m_clrRegionDef = m_clrRegionRedDef;
            m_clrRegionHover = m_clrRegionRedHover;
            break;
        }
    case BUTTON_GREEN:
        {
            m_clrRegionDef = m_clrRegionGreenDef;
            m_clrRegionHover = m_clrRegionGreenHover;
            break;
        }


    default:
        break;
    }
    Invalidate(FALSE);
}

BOOL CSimpleSelectButton::EnableWindow(BOOL bEnable /* = TRUE */)
{
    m_bEnable = bEnable;
    Invalidate(FALSE);
    return CWnd::EnableWindow(bEnable);
}

void CSimpleSelectButton::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
    m_bFocus = TRUE;
    Invalidate(FALSE);
}

void CSimpleSelectButton::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    m_bFocus = FALSE;
    Invalidate(FALSE);
}

void CSimpleSelectButton::SetWindowText(CString str)
{
    m_strCaption = str;
    Invalidate(FALSE);
}

void CSimpleSelectButton::EnableGetFocusToDrawPoints(BOOL bEnable)
{
    m_bEnableGetFocusToDrawPoints = bEnable;
}

BOOL CSimpleSelectButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bChangeCursor)
    {
        ::SetCursor(::LoadCursor(NULL  , IDC_HAND ) );
        return TRUE;
    }
    else
    {
        return CWnd::OnSetCursor(pWnd, nHitTest, message);
    }
}

void CSimpleSelectButton::SetChangeCursor(BOOL bChangeCursor)
{
    m_bChangeCursor = bChangeCursor;
}

void CSimpleSelectButton::EnableHoverLeaveNotify(BOOL bEnable)
{
    m_bEnableHoverLeaveNotify = bEnable;
}

void CSimpleSelectButton::EnableLDBClick(BOOL bEnable)
{
    m_bEnableLDBClick = bEnable;
}

void CSimpleSelectButton::SetNextPosition(UINT posi)
{
	m_nTextPosition = posi;
	Invalidate(FALSE);
}

void CSimpleSelectButton::OnMouseHover(UINT nFlags, CPoint point)
{
    if (m_bEnableHoverLeaveNotify)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_HOVER;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
    }   

    CWnd::OnMouseHover(nFlags, point);
}

void CSimpleSelectButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_bEnableLDBClick)
    {    
        if (POS_BUTTON == m_nPos)
        {
            NMHDR hdr;
            hdr.idFrom = GetDlgCtrlID();
            hdr.hwndFrom = GetSafeHwnd();
            hdr.code = NM_CLICK;
            GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
            Invalidate(FALSE);
        }     
        if (POS_TRANGLE == m_nPos)
        {
            if (m_selectButtonPopupWindow.IsWindowVisible())
            {
                m_selectButtonPopupWindow.ShowWindow(SW_HIDE);
            }
            else
            {
                //显示下拉框
                CRect rect;
                GetClientRect(&rect);
                ClientToScreen(&rect);
                m_selectButtonPopupWindow.m_listbox.ResetContent();
                m_selectButtonPopupWindow.m_nItemCount = m_vecListInfo.size();
                for (size_t i = 0; i < m_vecListInfo.size(); i++)
                {
                    m_selectButtonPopupWindow.m_listbox.AddString(m_vecListInfo[i].strText);
                    m_selectButtonPopupWindow.m_listbox.SetItemDataPtr(i, &m_vecListInfo[i]);
                    int nItemCount = m_selectButtonPopupWindow.m_listbox.GetCount();
                    m_selectButtonPopupWindow.m_listbox.SetItemHeight(nItemCount - 1, 20);
                }

                //   m_selectButtonPopupWindow.SetWindowPos(&wndTop, rect.right - 110, rect.bottom + 1, 110/*rect.Width()*/, m_selectButtonPopupWindow.m_nItemCount*20 + 26, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
                m_selectButtonPopupWindow.MoveWindow(rect.right - 110, rect.bottom + 1, 110/*rect.Width()*/, m_selectButtonPopupWindow.m_nItemCount*20 + 26, FALSE);
                m_selectButtonPopupWindow.ShowWindow(SW_SHOW);
            }
        }

    }
    
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleSelectButton::EnableMultiBkColor(BOOL bEnable/* = TRUE*/)
{
    m_bEnableMultiBkColor = bEnable;
}

void CSimpleSelectButton::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CRect rCl;
    GetClientRect(&rCl);
    m_rectTrangleRegion.SetRect(rCl.right - 20, 0, rCl.right, rCl.bottom);
    m_rectButton.SetRect(0, 0, rCl.right - 20, rCl.bottom);
}

int CSimpleSelectButton::GetCurPos(CPoint point)
{
    int nPos = POS_OUT;
    if (m_rectButton.PtInRect(point))
    {
        nPos = POS_BUTTON;
    }
    else if (m_rectTrangleRegion.PtInRect(point))
    {
        nPos = POS_TRANGLE;
    }
    return nPos;
}

void CSimpleSelectButton::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
  //  m_selectButtonPopupWindow.m_nWidth = nWidth;
    CWnd::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CSimpleSelectButton::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
//     if (lpRect != nullptr)
//     {
//         m_selectButtonPopupWindow.m_nWidth = lpRect->right - lpRect->left;
//     }    
    CWnd::MoveWindow(lpRect, bRepaint);
}

void CSimpleSelectButton::AddString(UINT nID, CString strText)
{
    ListInfo listInfo;
    listInfo.nID = nID;
    listInfo.strText = strText;
    m_vecListInfo.push_back(listInfo);
}

LRESULT CSimpleSelectButton::OnMessageFromDlg(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case MESSAGE_BUTTON_CONFIRM:
        {
            if (GetParent() != NULL)
            {
                ListInfo* pListInfo = (ListInfo*)lParam;
                if (pListInfo != NULL)
                {
                    m_CurSelectBtnStatus.bIsLastUseEdit = FALSE;
                    m_CurSelectBtnStatus.bIsLastUseList = TRUE;
                    m_CurSelectBtnStatus.listinfo.nID = pListInfo->nID;
                    m_CurSelectBtnStatus.listinfo.strText = pListInfo->strText;
                }
                
                GetParent()->SendMessage(MESSAGE_BUTTON, MESSAGE_FROM_LIST, lParam);
            }
            break;
        }

    case MESSAGE_BUTTON_EN_CHANGE:
        {
            if (GetParent() != NULL)
            {
                CString* pStr = (CString*)lParam;
                if (pStr != NULL)
                {
                    m_CurSelectBtnStatus.bIsLastUseEdit = TRUE;
                    m_CurSelectBtnStatus.bIsLastUseList = FALSE;
                    m_CurSelectBtnStatus.strEditText = *pStr;
                }
                
                GetParent()->SendMessage(MESSAGE_BUTTON, MESSAGE_EN_CHANGE, lParam);
            }
            break;
        }

    default:
        break;
    }

    return 0;
}

CurSelectBtnStatus* CSimpleSelectButton::GetCurStatus()
{
    return &m_CurSelectBtnStatus;
}

void CSimpleSelectButton::SetInputStyle(UINT nStyleID)
{
    m_selectButtonPopupWindow.m_edit.SetInputStyle(nStyleID);
}

