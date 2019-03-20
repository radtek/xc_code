
// MainView.h : CMainView 类的接口
//


#pragma once

#include <map>
#include "../share/SharedPart_Dlls.h"
#include <string>
#include <iostream>
#include <io.h>
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;

#include "SimpleButton.h"
#include "Resource.h"
#include "SimpleCheckbox.h"
#include "SimpleImageButton.h"
#include "SimpleSeparator.h"
#include "SpecialImageButton.h"
#include "SimpleTree.h"
#include <vector>

using namespace std;

// CMainView 窗口

class CMainView : public CWnd
{
// 构造
public:
	CMainView();
	enum
	{
		IDC_START = 0x1001,
		IDC_SEPARATOR,
		IDC_TREE,
		IDC_BUTTON_OPEN,
		IDC_BUTTON_CLOSE,
		IDC_BUTTON_START,

		ID_END = 0x2001,
	};

public:

	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainView();


	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	afx_msg void OnBtnCloseOpen(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg LRESULT OnAbnormalExit(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnTree(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_clrBk;
	COLORREF m_clrSeparator;
	CSpecialImageButton m_btnOpen;
	CSpecialImageButton m_btnClose;
	CSimpleSeparator m_separator;
	CSimpleTree m_tree;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString GetApplactionPath();

public:
	int m_nGap;
	UINT m_nButton;
	CSharedByALLCData m_sharedByALLCData;

	string m_strConfigPath;
	string m_strPrivLoginedName;
	int m_nWriteLog;

	CString GetBtnTextByID(UINT nID);
	CSimpleButton* GetBtnByID(UINT nID);


	//加载的模块DLL的信息
	MapModuleInfo* m_pMapModuleDLLInfo;

	bool Inspect_bLoad(const string s_str);
	bool gainChild_rect(CRect &child_rect);

	bool hideOtherDlg(string DllName_Show);

	//virtual BOOL OnInitDialog();
	bool GainGlogConfig();
	string getProcessName();
	void initGlog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	
	bool VerifyValidClick(CString &strText);
	void showSpecificDlg(string DllName_Show);
	void CloseWnd();
	void GetFirstBtnRect(CRect& rect, int& nOffsetY);
	void FoldButtons(BOOL bFold);
	void SetModuleDLLInfo(MapModuleInfo* pMapModuleDLLInfo, string LoginedDllName);
};

