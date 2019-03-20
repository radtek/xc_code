
//*****************************************************
//********    SimpleImageControl By YaoShuLong 2017.4
//*****************************************************

#pragma once

// CSimpleImageControl



#define SimpleImageControl_CLASSNAME    _T("CSimpleImageControl")

class CSimpleImageControl : public CWnd
{
	DECLARE_DYNAMIC(CSimpleImageControl)

public:
    CSimpleImageControl();
    virtual ~CSimpleImageControl();

    BOOL Create(UINT nBmpID, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    UINT m_nID;


 
protected:
	DECLARE_MESSAGE_MAP()
public:
    void SetImage(UINT nBmpID);
         
};


