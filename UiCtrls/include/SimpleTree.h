
//*****************************************************
//********    SimpleTree By YaoShuLong 2018.12
//*****************************************************

#pragma once
#include<vector>
#include <map>
#include <memory>
using namespace std;

#include "SimpleButton.h"

// CSimpleTree

#define SimpleTree_CLASSNAME    _T("CSimpleTree")
#define  WM_TREEINFO       WM_USER + 1000
using SharedPtrBtn = shared_ptr<CSimpleButton>;

class CTreeData
{
public:
	CTreeData() :
		bClose(true),
		bTab(false),
		pBtnBase(nullptr)
	{}
	~CTreeData() {}

	bool bClose;
	bool bTab;
	CRect rectSub;
	CString strCaption;
	SharedPtrBtn pBtnBase;
	map<UINT, SharedPtrBtn> mapBtnSub;
};

struct TreeData
{
	int nIndexBase;
	int nIndexSub;
	CString strTextSub;
};

class CSimpleTree : public CWnd
{
	DECLARE_DYNAMIC(CSimpleTree)

public:
	CSimpleTree();
	virtual ~CSimpleTree();
	enum
	{
		IDC_START_BASE = 0x1001,
		ID_END_BASE = 0x1301,

		IDC_START_SUB = 0x1401,
		ID_END_SUB = 0x1801,
	};

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

public:
	void SetTreeData(CString strBase, vector<CString>& vecStrSub, CString strSelected, BOOL bEnableSubTab, BOOL bClose = TRUE);
	void SetTopGap(UINT nGap);
	void SetFontStyle(CString strFontStyle);

private:
    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnBtn(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);

private:
	map<UINT, CTreeData> m_mapTreeData;
	UINT m_nIDBase;
	UINT m_nIDSub;
	UINT m_nTopGap;
	UINT m_nBaseHeight;
	UINT m_nSubHeight;

private:
	CString GetCaption(BOOL bClose, CString strCaption);
	////parameter 1 树的主索引值. parameter 2是需要重刷的区域
	void ReLayout(int nIndexStart, CRect& rectInvalidate);
	void GetIndex(UINT nIDSub, int& nIndexBase, int& nIndexSub, UINT& nIDBase);
	UINT GetWidth(HFONT hFont, CString str);
	
protected:
	DECLARE_MESSAGE_MAP()
public:

};


