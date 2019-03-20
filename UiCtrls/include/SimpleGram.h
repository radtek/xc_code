
//*****************************************************
//********    SimpleGram By YaoShuLong 2017.7
//*****************************************************

#pragma once


#define SimpleGram_CLASSNAME    _T("CSimpleGram")
// CSimpleGram


class CPointData
{
public:
    CPointData(void);
    ~CPointData(void);

    CString x;
    double y;
    BOOL bShowX;
    BOOL bEnable;
};



class CSimpleGram : public CWnd
{
	DECLARE_DYNAMIC(CSimpleGram)

public:
	CSimpleGram();
	virtual ~CSimpleGram();

    BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

public:
    void DrawLine(CPointData* pPointData, int nPointDataCount);
    void SetBkColor(COLORREF clr);
    void SetGridsColor(COLORREF clr);
    void SetTextColor(COLORREF clr);
    void SetXZeroColor(COLORREF clr);
    void SetFont(HFONT hFont);
    void SetYGridHeight(int nHeight);
    void SetLimit(double fYMix, double fYMax);

private:
    BOOL RegisterWindowClass();

    BOOL m_bHorizontalLines;//水平线
    BOOL m_bVerticalLines;//垂直线
    COLORREF m_clrBk;
    COLORREF m_clrBkGrid;
    COLORREF m_clrGrids;
    COLORREF m_clrLine;
    COLORREF m_clrText;
    COLORREF m_clrZeroX;
    //Y轴每个最小间隔的高度
    int m_nYGridHeight;

    HFONT m_hFont;
    CFont m_font;
    double m_fYMix;
    double m_fYMax;

    CPointData* m_pPointData;
    int m_nPointDataCount;

  //  vector<CPointData> m_vecDataInfo;
    double TransX(double nMix, double nMax, double x, double nXLength);
    double TransY(double nMix, double nMax, double y, double nYLength);

    void DrawXTick(CDC& dcMemGrid, CString str, int nX, int nY, int nWidth, int nHeight);
    void DrawYTick(CDC& dcMemGrid, CRect rect, double max, double mix, int nTickCount);
    

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

};


