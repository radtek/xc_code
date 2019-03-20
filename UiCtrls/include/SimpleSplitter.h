
#ifndef __SIMPLESPLITER__
#define __SIMPLESPLITER__

#define DEFAULT_THICKNESS 3

class CSimpleSplitter :  public CRect
{
public:
	CSimpleSplitter(bool bHorizontal = true, long nThickness = DEFAULT_THICKNESS)
		: m_bHorizontal(bHorizontal)
		, m_nThickness(nThickness)
	{
	}
	CSimpleSplitter(const CSimpleSplitter& ref)
		: CRect(ref)
		, m_bHorizontal(ref.IsHorizontal())
		, m_nThickness(ref.m_nThickness)
	{

	}
	long GetThickness()
	{
		return m_nThickness;
	}
	void SetOrientation(bool bHorizontal)
	{
		m_bHorizontal=bHorizontal;
	}
	bool IsHorizontal() const
	{
		return m_bHorizontal;
	}

	bool IsPtIn(const CPoint& pt) const
	{
		return (PtInRect(pt)!=FALSE);
	}

	HCURSOR GetCursor(const CPoint& pt) const
	{
		HCURSOR hCursor=NULL;
		if(IsPtIn(pt))
		{
			hCursor=((IsHorizontal())
								? AfxGetApp()->LoadStandardCursor(IDC_SIZEWE)
								: AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		}
		return hCursor;
	}

    void Draw(CDC& dc) const
    {
        //dc.FillRect(this, (HBRUSH)LongToPtr(COLOR_3DFACE + 1));
    }
	void DrawGhostBar(CDC& dc) const
	{
        /*CBrush brush = CDCHandle::GetHalftoneBrush();
        if(brush.m_hBrush != NULL)
        {
            HBRUSH hBrushOld = dc.SelectBrush(brush);
            dc.PatBlt(this->left, this->top,this->Width(),this->Height(), PATINVERT);
            dc.SelectBrush(hBrushOld);

        }*/

	}
	void CleanGhostBar(CDC& dc) const
	{
		DrawGhostBar(dc);
	}
protected:
	bool m_bHorizontal;
	long m_nThickness;
};

#endif // __SIMPLESPLITER__
