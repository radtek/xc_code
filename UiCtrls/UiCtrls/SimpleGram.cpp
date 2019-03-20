
//*****************************************************
//********    SimpleGram By YaoShuLong 2017.7
//*****************************************************

#include "stdafx.h"
#include "SimpleGram.h"

CPointData::CPointData() 
{
    y = 0;
    bShowX = FALSE;
    bEnable = FALSE;
}

CPointData::~CPointData()
{        
}


// CSimpleGram

IMPLEMENT_DYNAMIC(CSimpleGram, CWnd)

CSimpleGram::CSimpleGram() 
{
    m_fYMix = 0;
    m_fYMax = 0;
    m_nYGridHeight = 20;
    m_bHorizontalLines = TRUE;
    m_bVerticalLines = TRUE;
    m_clrBk = RGB(0, 0, 0);
    m_clrBkGrid = RGB(0, 0, 0);
    m_clrGrids = RGB(0, 130, 66);
    m_clrLine = RGB(0, 255, 0);
    m_clrText = RGB(0, 255, 0);
    m_clrZeroX = RGB(255, 242, 0);
    m_pPointData = nullptr;
    m_nPointDataCount = 0;
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
}

CSimpleGram::~CSimpleGram()
{      
}


BEGIN_MESSAGE_MAP(CSimpleGram, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CSimpleGram 消息处理程序

void CSimpleGram::OnPaint()
{
    CPaintDC dc(this);
    if (m_pPointData == nullptr)
    {
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


        dcMem.FillSolidRect(&rCl, m_clrBk);
        dc.BitBlt(0 ,0 ,rCl.Width() ,rCl.Height() ,&dcMem ,0 ,0 ,SRCCOPY);
        dcMem.SelectObject(pOldBmp);
        dcMem.DeleteDC();
        bmpMem.DeleteObject();
        return;
    }

    CRect rCl;
    GetClientRect(&rCl);

    double yMix = m_fYMix;
    double yMax = m_fYMax;
    //确定Y轴的上限和下限
    for (int i = 0; i < m_nPointDataCount; i++)
    {
        yMix = m_pPointData[i].y < yMix ? m_pPointData[i].y : yMix;
        yMax = m_pPointData[i].y > yMax ? m_pPointData[i].y : yMax;
    }

    //计算x,y轴字符串最大宽度和最大高度
    CString str;
    CRect rectText;
    CDC *pDC = GetDC();
    pDC->SelectObject(CFont::FromHandle(m_hFont));
    int nYTextWidthMax = 0;
    int nYTextHeightMax = 0;

    int nXTextWidthMax = 0;
    int nXTextHeightMax = 0;
    for (int i = 0; i < m_nPointDataCount; i++)
    {
        str.Format("%.2f", m_pPointData[i].y);      
        pDC->DrawText(str, rectText, DT_CALCRECT | DT_LEFT | DT_VCENTER);
        nYTextWidthMax = rectText.Width() > nYTextWidthMax ? rectText.Width() : nYTextWidthMax;
        nYTextHeightMax = rectText.Height() > nYTextHeightMax ? rectText.Height() : nYTextHeightMax;

        rectText.SetRectEmpty();
        
        char time[5] = {0};
        if (m_pPointData[i].x != "")
        {
            str = m_pPointData[i].x;
            pDC->DrawText(str, rectText, DT_CALCRECT | DT_LEFT | DT_VCENTER);
            nXTextWidthMax = rectText.Width() > nXTextWidthMax ? rectText.Width() : nXTextWidthMax;
            nXTextHeightMax = rectText.Height() > nXTextHeightMax ? rectText.Height() : nXTextHeightMax;
        }
        
    }
    nYTextWidthMax += 6;
    nYTextHeightMax = (nYTextHeightMax / 2) * 2 + 4;

    nXTextWidthMax = (nXTextWidthMax / 2) * 2 + 4;
    nXTextHeightMax += 6;
    int nYGridCount = (rCl.Height() - nYTextHeightMax / 2 - nXTextHeightMax - 1) / m_nYGridHeight;
    int nYRamin = (rCl.Height() - nYTextHeightMax / 2 - nXTextHeightMax - 1) % m_nYGridHeight;

    //确定曲线表格的大小
    CRect rectGrid;
    rectGrid.SetRect(0, 0, rCl.Width() - nYTextWidthMax - nXTextWidthMax/2, nYGridCount * m_nYGridHeight + 1);

    CDC dcMemGrid;
    CBitmap bmpMemGrid;
    CBitmap* pOldBmpGrid=NULL;

    if(dcMemGrid.CreateCompatibleDC(&dc))
    {
        if(bmpMemGrid.CreateCompatibleBitmap(&dc,rectGrid.Width(),rectGrid.Height()))
        {
            pOldBmpGrid = dcMemGrid.SelectObject(&bmpMemGrid);
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

    dcMemGrid.FillSolidRect(&rectGrid, m_clrBkGrid);

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

    
    dcMem.FillSolidRect(&rCl, m_clrBk);

    if (m_nPointDataCount > 0)
    {
        //画Y轴的刻度
        CRect rect(0, nYRamin / 2, nYTextWidthMax - 3, nYRamin / 2 + nYTextHeightMax);
        DrawYTick(dcMem, rect, yMax, yMix, nYGridCount);
    }
    

    CPen penGrids;
    if(!penGrids.CreatePen(PS_SOLID, 1, m_clrGrids))
    {
        return;
    }
    CPen* pOldPen = dcMemGrid.SelectObject(&penGrids);

      //画水平线
      if (m_bHorizontalLines && m_nPointDataCount > 0)
      {
          for(int i = rectGrid.bottom - 1; i >=  rectGrid.top; i -= m_nYGridHeight)
          {
              dcMemGrid.MoveTo(rectGrid.left, i);
              dcMemGrid.LineTo(rectGrid.right, i);
          }
      }
  
      int nRemains = 0;
      int nTickWidth = 0;
      if (m_nPointDataCount > 1)
      {
          int nXGridCount = m_nPointDataCount - 1;
          if ((rectGrid.Width() - 1) > nXGridCount)
          {
              nTickWidth = (rectGrid.Width() - 1) / nXGridCount;
              nRemains = (rectGrid.Width() - 1) % nXGridCount;

              
              int nCount = 0;
              for(int i = rectGrid.left; i < rectGrid.right; )
              {
                  if (nCount < m_nPointDataCount)
                  {
                      if (m_pPointData[nCount].bShowX && m_pPointData[nCount].x != "")
                      {
                          //画X轴的刻度
                          DrawXTick(dcMem, m_pPointData[nCount].x, i + nYTextWidthMax, nYTextHeightMax / 2 + nYRamin / 2 + rectGrid.Height(), nXTextWidthMax, nXTextHeightMax );
                      }
                  }
                  if (m_bVerticalLines)
                  {
                      //画垂直线
                      dcMemGrid.MoveTo(i, rectGrid.top);
                      dcMemGrid.LineTo(i, rectGrid.bottom);
                  }
                  i += ((nCount < nRemains) ? (nTickWidth + 1) : nTickWidth);
                  nCount ++;
              }
                  
          }
          
      }
     

    dcMemGrid.SelectObject(pOldPen);
    //画数据曲线
    CPen penLine(PS_SOLID, 1, m_clrLine);
    pOldPen = dcMemGrid.SelectObject(&penLine);
    if (m_nPointDataCount > 1)
    {
        int nCount = 0;
        int i = rectGrid.left;
        for (size_t j = 0; j < m_nPointDataCount - 1 && i < rectGrid.right; j++)
        {
            if (m_pPointData[j].bEnable && m_pPointData[j + 1].bEnable)
            {
                dcMemGrid.MoveTo(i, TransY(yMix, yMax, m_pPointData[j].y, rectGrid.Height() - 1));          
                i += ((nCount < nRemains) ? (nTickWidth + 1) : nTickWidth);
                nCount ++;
                dcMemGrid.LineTo(i, TransY(yMix, yMax, m_pPointData[j + 1].y, rectGrid.Height() - 1));
            }
            else
            {
                i += ((nCount < nRemains) ? (nTickWidth + 1) : nTickWidth);
                nCount ++;
            }

        }

    }

    dcMemGrid.SelectObject(pOldPen);    

 
    dc.BitBlt(0 ,0 ,rCl.Width() ,rCl.Height() ,&dcMem ,0 ,0 ,SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();

    dc.BitBlt(nYTextWidthMax, nYTextHeightMax / 2 + nYRamin / 2 ,rectGrid.Width(), rectGrid.Height(),&dcMemGrid ,0 ,0 ,SRCCOPY);
    dcMemGrid.SelectObject(pOldBmpGrid);
    dcMemGrid.DeleteDC();
    bmpMemGrid.DeleteObject();
    ReleaseDC(pDC);
}

BOOL CSimpleGram::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    return CWnd::Create(SimpleGram_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleGram::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleGram_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleGram_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSimpleGram::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleGram::DrawLine(CPointData* pPointData, int nPointDataCount)
{
    if (pPointData != nullptr)
    {
        m_pPointData = pPointData;
        m_nPointDataCount = nPointDataCount;
    }

    Invalidate(FALSE);
}

double CSimpleGram::TransX(double nMix, double nMax, double x, double nXLength)
{
    return nXLength * ((x - nMix) / (nMax - nMix));
}

double CSimpleGram::TransY(double nMix, double nMax, double y, double nYLength)
{
    return (nYLength - nYLength * ((y - nMix) / (nMax - nMix)));
}

void CSimpleGram::SetBkColor(COLORREF clr)
{
    m_clrBk = clr;
    Invalidate(FALSE);
}

void CSimpleGram::SetGridsColor(COLORREF clr)
{
    m_clrGrids = clr;
    Invalidate(FALSE);
}

void CSimpleGram::SetTextColor(COLORREF clr)
{
    m_clrText = clr;
    Invalidate(FALSE);
}

void CSimpleGram::SetFont(HFONT hFont)
{
    m_hFont = hFont;
    Invalidate(FALSE);
}

// void EnableGrid(BOOL bHorizontalLines = TRUE, BOOL bVerticalLines = TRUE);
// void CSimpleGram::EnableGrid(BOOL bHorizontalLines/* = TRUE*/, BOOL bVerticalLines/* = TRUE*/)
// {
//     m_bHorizontalLines = bHorizontalLines;
//     m_bVerticalLines = bVerticalLines;
//     Invalidate(FALSE);
// }

void CSimpleGram::SetYGridHeight(int nHeight)
{
    m_nYGridHeight = nHeight;
    Invalidate(FALSE);
}

void CSimpleGram::SetXZeroColor(COLORREF clr)
{
    m_clrZeroX = clr;
    Invalidate(FALSE);
}

void CSimpleGram::DrawXTick(CDC& dcMemGrid, CString str, int nX, int nY, int nWidth, int nHeight)
{
    CFont* pFontOld = dcMemGrid.SelectObject(CFont::FromHandle(m_hFont));
    dcMemGrid.SetBkMode(TRANSPARENT);
    COLORREF clrBkTextOld = dcMemGrid.SetTextColor(m_clrText);
    CRect rect(nX - nWidth/2 + 1, nY, nX + nWidth/2 + 1, nY + nHeight);   
    dcMemGrid.DrawText(str, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    dcMemGrid.SelectObject(pFontOld);

    CPen penGrids;
    if(!penGrids.CreatePen(PS_SOLID, 1, m_clrGrids))
    {
        return;
    }
    CPen* pOldPen = dcMemGrid.SelectObject(&penGrids);
    dcMemGrid.MoveTo(nX, nY);
    dcMemGrid.LineTo(nX, nY + 4);
    dcMemGrid.SelectObject(pOldPen);
}

void CSimpleGram::DrawYTick(CDC& dcMemGrid, CRect rect, double max, double mix, int nTickCount)
{
    CFont* pFontOld = dcMemGrid.SelectObject(CFont::FromHandle(m_hFont));
    dcMemGrid.SetBkMode(TRANSPARENT);
    COLORREF clrBkTextOld = dcMemGrid.SetTextColor(m_clrText);
    double tick = (max - mix) / nTickCount;

    for (int i = 0; i < nTickCount; i++)
    {
        double draw = max - i * tick;
        CString str;
        str.Format("%.2f", draw);
        dcMemGrid.DrawText(str, &rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
        rect.OffsetRect(0, m_nYGridHeight);
    }

    CString str;
    str.Format("%.2f", mix);
    dcMemGrid.DrawText(str, &rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);


}

void CSimpleGram::SetLimit(double fYMix, double fYMax)
{
    m_fYMix = fYMix;
    m_fYMax = fYMax;
}