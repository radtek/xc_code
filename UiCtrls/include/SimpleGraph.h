
//*****************************************************
//********    SimpleGraph By YaoShuLong 2017.9
//*****************************************************

#pragma once

#include<vector>
using namespace std;

// CSimpleGraph

#define SimpleGraph_CLASSNAME    _T("CSimpleGraph")

class CSimpleGraph : public CWnd
{
	DECLARE_DYNAMIC(CSimpleGraph)

public:
	CSimpleGraph();
	virtual ~CSimpleGraph();

    BOOL Create(UINT dwStyle, CWnd * pParentWnd, UINT nID);

private:
    BOOL RegisterWindowClass();
    double TransY(double dMix, double dMax, double dY, double dYLength);
    void GetLimit(double& dLowerLimit, double& dUpperLimit);

private:
   
    UINT m_nIDLine1;
    UINT m_nIDLine2;

    COLORREF m_clrBk;
    COLORREF m_clrLine1;
    COLORREF m_clrLine2;

    int m_nTick;
    int m_nPos;
    int m_nSingleLinePointsMaxCounts;

    int m_nUpperOffset;
    int m_nLowerOffset;
    vector<double> m_vecY;
    vector<double> m_vecY1;
    vector<double> m_vecY2;
    CRect m_rectClient;

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
    //设置2种线的颜色
    void SetLineColor(COLORREF clrLine1, COLORREF clrLine2);
    //设置背景颜色
    void SetBkColor(COLORREF clr);
    //2种线的Y值，注意第1个值对应第1条线，第2个值类同
    void DrawLine(double dY1, double dY2);
    //X轴每更新一个点的距离
    void SetXTick(int nTick);
    //划线的距离最右边的位置，
    void SetDrawPosToRight(int nPos);
    //清除线
    void Clear();
    //设置上下间距
    void SetOffset(int nUpperOffset, int nLowerOffset);
};


