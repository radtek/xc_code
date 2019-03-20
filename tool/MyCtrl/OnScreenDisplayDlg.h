#pragma once

#include "OnScreenDisplayBDlg.h"
#include "../../FastTrade/Resource.h"

// COnScreenDisplayDlg 对话框

class COnScreenDisplayDlg : public COnScreenDisplayBDlg
{
	DECLARE_DYNAMIC(COnScreenDisplayDlg)

	enum
	{
		IDC_MYTIMERID = 1000, // 信息显示的 timer id
		IDC_LAB_TIPWORD,
	};

public:
	COnScreenDisplayDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COnScreenDisplayDlg(); 

// 对话框数据
	enum { IDD = IDD_ONSCREENDISPLAYDLG }; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_strTip;	// 提示信息
	CStatic m_lbTip;	// 显示提示的控件
	CFont m_tipFont;
	int m_nElapseMs;
	COLORREF m_tipColor;

private:
	void ResizeWindow(CString strTip);
	void AdjustWindow();

public:
	/*
	OSD提示语
	@strTip: 提示信息
	@pParent: 提示语显示的居中窗体
	@nElapseMs: 提示语的延迟时间(毫秒)
	@tipColor: 提示语显示颜色(不能设置颜色为 RGB(0,255,0))
	*/
	void TipWord(CString strTip, CWnd* pParent = GetDesktopWindow(), int nElapseMs = 1000, COLORREF tipColor = RGB(255, 0, 0));

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
