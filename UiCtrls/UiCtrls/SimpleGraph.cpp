
//*****************************************************
//********    SimpleGraph By YaoShuLong 2017.9
//*****************************************************

#include "stdafx.h"
#include "SimpleGraph.h"

// CSimpleGraph

IMPLEMENT_DYNAMIC(CSimpleGraph, CWnd)

CSimpleGraph::CSimpleGraph() 
{
    m_clrBk = RGB(46, 48, 61);
    RegisterWindowClass();  

//    CRect rect; 
 //   SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);  
 //   m_nScreenWidth = rect.Width();
    m_nTick = 3;
    m_nPos = 11;
//    m_nSingleLinePointsMaxCounts = (m_nScreenWidth - m_nPos) / m_nTick;  
    m_nUpperOffset = 10;
    m_nLowerOffset = 10;
}

CSimpleGraph::~CSimpleGraph()
{      
}


BEGIN_MESSAGE_MAP(CSimpleGraph, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CSimpleGraph 消息处理程序

void CSimpleGraph::OnPaint()
{
    CPaintDC dc(this);

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height()))
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

    dcMem.FillSolidRect(&m_rectClient, m_clrBk);
    if (m_vecY1.size() > 1)
    {
        int nHeight = m_rectClient.Height() - m_nUpperOffset - m_nLowerOffset;
        double dLowerLimit = 0;
        double dUpperLimit = 0;
        GetLimit(dLowerLimit, dUpperLimit);

        CPen pen1(PS_SOLID, 1, m_clrLine1);
        dcMem.SelectObject(pen1);
        int nStartX = m_rectClient.Width() - m_nPos;
        for (size_t i = 0; i < m_vecY1.size() - 1; i++)
        {
            if (nStartX - m_nTick < 0 || 0 == nStartX - m_nTick)
            {
                break;
            }
            dcMem.MoveTo(nStartX, TransY(dLowerLimit, dUpperLimit, m_vecY1[i], nHeight) + m_nUpperOffset);
            dcMem.LineTo(nStartX - m_nTick, TransY(dLowerLimit, dUpperLimit, m_vecY1[i + 1], nHeight) + m_nUpperOffset);
            nStartX -= m_nTick;
        }

        CPen pen2(PS_SOLID, 1, m_clrLine2);
        dcMem.SelectObject(pen2);
        nStartX = m_rectClient.Width() - m_nPos;
        for (size_t i = 0; i < m_vecY2.size() - 1; i++)
        {
            if (nStartX - m_nTick < 0 || 0 == nStartX - m_nTick)
            {
                break;
            }
            dcMem.MoveTo(nStartX, TransY(dLowerLimit, dUpperLimit, m_vecY2[i], nHeight) + m_nUpperOffset);
            dcMem.LineTo(nStartX - m_nTick, TransY(dLowerLimit, dUpperLimit, m_vecY2[i + 1], nHeight) + m_nUpperOffset);
            nStartX -= m_nTick;
        }
    }

    dc.BitBlt(0, 0, m_rectClient.Width(), m_rectClient.Height(), &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
    bmpMem.DeleteObject();
}

void CSimpleGraph::GetLimit(double& dLowerLimit, double& dUpperLimit)
{
    if (m_vecY.size() > 0)
    {
        dLowerLimit = m_vecY[0];
        dUpperLimit = m_vecY[0];
        for (size_t i = 0; i < m_vecY.size(); i++)
        {
            if (m_vecY[i] < dLowerLimit)
            {
                dLowerLimit = m_vecY[i];
            }
            if (m_vecY[i] > dUpperLimit)
            {
                dUpperLimit = m_vecY[i];
            }
        }
    }
}

void CSimpleGraph::DrawLine(double dY1, double dY2)
{
    m_vecY.insert(m_vecY.begin(), dY1);
    m_vecY.insert(m_vecY.begin(), dY2);

    if (m_vecY1.empty())
    {
        m_vecY1.push_back(dY1);
        m_vecY2.push_back(dY2);
        return;
    }
    m_vecY1.insert(m_vecY1.begin(), dY1);
    m_vecY2.insert(m_vecY2.begin(), dY2);
    if (m_vecY1.size() > m_nSingleLinePointsMaxCounts)
    {
        m_vecY1.pop_back();
        m_vecY2.pop_back();
        m_vecY.pop_back();
        m_vecY.pop_back();
    }
    Invalidate(FALSE);
}

BOOL CSimpleGraph::Create(UINT dwStyle, CWnd * pParentWnd, UINT nID)
{
    return CWnd::Create(SimpleGraph_CLASSNAME, _T(""), dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
}

BOOL CSimpleGraph::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleGraph_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleGraph_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }        
    return TRUE;
}

BOOL CSimpleGraph::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleGraph::SetLineColor(COLORREF clrLine1, COLORREF clrLine2)
{
    m_clrLine1 = clrLine1;
    m_clrLine2 = clrLine2;
}

double CSimpleGraph::TransY(double dMix, double dMax, double dY, double dYLength)
{
    if (dMix == dMax)
    {
        return dYLength/2;
    }
    double dValue = dYLength - dYLength * ((dY - dMix) / (dMax - dMix));
    if (dValue == dYLength)
    {
        dValue --;
    }
    return dValue;
}

void CSimpleGraph::SetBkColor(COLORREF clr)
{
    m_clrBk = clr;
}

void CSimpleGraph::SetXTick(int nTick)
{
    if (nTick > 0)
    {
        m_nTick = nTick;
        m_nSingleLinePointsMaxCounts = (m_rectClient.Width() - m_nPos) / m_nTick; 
    }
}

void CSimpleGraph::SetDrawPosToRight(int nPos)
{
    m_nPos = nPos;
    m_nSingleLinePointsMaxCounts = (m_rectClient.Width() - m_nPos) / m_nTick; 
}

void CSimpleGraph::Clear()
{
    m_vecY.clear();
    m_vecY1.clear();
    m_vecY2.clear();
    Invalidate(FALSE);
}

void CSimpleGraph::SetOffset(int nUpperOffset, int nLowerOffset)
{
    m_nUpperOffset = nUpperOffset;
    m_nLowerOffset = nLowerOffset;
}

void CSimpleGraph::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    GetClientRect(&m_rectClient);
    m_nSingleLinePointsMaxCounts = (m_rectClient.Width() - m_nPos) / m_nTick; 

    while (m_vecY1.size() > m_nSingleLinePointsMaxCounts)
    {
        m_vecY1.pop_back();
        m_vecY2.pop_back();
        m_vecY.pop_back();
        m_vecY.pop_back();
    }   

}



