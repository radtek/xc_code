#pragma once
#include "dockwindow.h"
#include "FontSet.h"
#include "ColorSet.h"
#include "SimpleButton.h"
#include "../../share/SharedPart_Dlls.h"
#include "../../tool/TradeDlg.h"
#include "SimpleDockPane.h"
#include "GridCtrlEx.h"
#include "../baseView/GridRes.h"

#define dwStyle_ComboBox WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST |WS_VSCROLL
#define dwStyle_Static WS_CHILD | WS_VISIBLE
#define dwStyle_Edit WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL
#define dwStyle_Checkbox WS_CHILD | BS_AUTOCHECKBOX | WS_VISIBLE
#define dwStyle_Radio BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP 
#define dwStyle_button WS_CHILD | WS_VISIBLE
#define length_Time 80

/*studying 新框架用mfc dll各种问题，改用静态库试试，理论上应该是一样的,mfc dll 可以自己搞成.lib使用吗？理论上可行的啊*/
class CBaseView : public CDockWindow
{
	DECLARE_DYNAMIC(CBaseView)
public:
	CBaseView(CDockListener &listener);
	virtual ~CBaseView(void);

	virtual void SaveSetInfo(CString &strSetInfo);
	virtual void LoadSetInfo(const CString &strSetInfo);
	void SetDock(bool bDock);
	bool GetIsDock();
	enum HOVER_BUTTON
	{
		HOVER_NULL,
		HOVER_MIN,
		HOVER_MAX,
		HOVER_CLOSE,
	};
public:
	void SetBackClr(COLORREF backClr);
	void SetBorderClr(COLORREF borderClr);
	void SetFocusBorderClr(COLORREF focusBorderClr);
	void SetTilteBackClr(COLORREF titleBackClr);
	void SetTilteFocusBackClr(COLORREF focusTitleBackClr);
	void SetTilteTextClr(COLORREF titleTextClr);
	void SetButtonClr(COLORREF buttonClr);
	void SetHoverButtonClr(COLORREF hoverButtonClr);
	void SetHoverButtonBackClr(COLORREF hoverButtonBackCrl);
	void SetTilteText(CString strTitleText);
	void SetBorderSize(int nBorder);
	void SetTitleHeight(int nTilteHeight);
private:
	COLORREF m_clBackClr;	//背景色
	COLORREF m_clBorderClr;	//边框色
	COLORREF m_clFocusBoderClr;	//焦点边框色
	COLORREF m_clTilteBackClr;//标题背景色
	COLORREF m_clFocusTilteBackClr;//焦点标题背景色
	COLORREF m_clTilteTextClr;//标题字体颜色
	COLORREF m_clButtonClr;//按钮颜色
	COLORREF m_clHoverButtonClr;//按钮Hover颜色
	COLORREF m_clHoverButtonBackClr;//按钮Hover背景色
	int		 m_nBorder;		  //边框宽度
	int		 m_nTitleHeight;  //标题高度
	HFONT	 m_hTitleFont;	  //标题字体
	CRect    m_rcClose;		  //关闭 
	CRect    m_rcMin;		  //最小化
	CRect    m_rcMax;		  //最大化
	HOVER_BUTTON m_nHover;	  //悬浮
	BOOL	 m_bShowMaxMinButton;//是否显示最大化最小化
	BOOL     m_bActivate;//焦点判断
protected:
	virtual void InitCtrls(); //初始化控件
	virtual void Relayout(); //改变控件位置
	virtual void DrawBackGround(HDC hDC, const RECT rc);
private:
	void DrawBorder(CDC* pDC);
	virtual void OnShowMin();
	virtual void OnShowMax();
	virtual void OnShowRestore();
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcMouseLeave();
	afx_msg LRESULT OnNcHitTest(CPoint point);


public:
	virtual void ReleaseView() = 0;

	void SetViewID(const UINT nID) { m_nViewID = nID; }
	UINT GetViewID() const { return m_nViewID; }

	virtual BOOL GetDefaultRect(CRect &rect) { return FALSE; }
	virtual void OnChangeLanguage() {}

public:
	BOOL m_bMultiWindow; // 是否可以同时打开多个
	BOOL m_bStaticWindow; // 关闭时隐藏

protected:
	UINT m_nViewID;

public:
	afx_msg void OnPaint();
};

