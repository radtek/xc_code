#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "SimpleCheckBox.h"
#include "GridCtrlEx.h"
#include "SimpleButton.h"
#include "FastTradeMainView.h"

// COrderView

CString GetDirection(UINT nID);
CString GetStatus(UINT nID);


class COrderView : public CBaseView
{
	DECLARE_DYNAMIC(COrderView)

	enum
	{
		IDC_START = 0x1001,

		IDC_CHEC_NOTRADE,
		IDC_GRID_ORDERVIEW,
		IDC_Button_CancelCheck,
		IDC_Button_CancelAll,

	};

	//委托数据
	struct OrderData
	{
		LPARAM lParam;//委托指针
		int nType;	  //委托类型 order preorder
	};

public:
	COrderView(CDockListener &listener);
	virtual ~COrderView();

	static CBaseView *CreateView(CDockListener &listener) { return new COrderView(listener); }
	virtual void ReleaseView();
	virtual void PostNcDestroy();
	CGridCtrlEx* GetGrid_T();
	void CancelCheck();
	void CancelAll();
private:
	void InitialWindow();
	void CreateControls();
	void InitGrid();
	void CancelOrder(int nRow, BOOL bShowTip);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickedShowNoTrade(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnGridOrderLMouseDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGridOrderLDbMouseDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtnRange(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);

	bool RemoveOrder(const UInt_64 entrust_no, const string &exchange_type);
	bool GainRevoke333017(string &s_str, const UInt_64 entrust_no, const string &exchange_type);
	//虚表回调
	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
    //获取每一个格子的数据
	void GetVirtualCellInfo(GV_DISPINFO & dispinfo, const COrder &order);
	COLORREF GetClor(UINT nID);
	BOOL GetOrderNoCheck(UInt_64 entrust_no);

	DECLARE_MESSAGE_MAP()

private:
	map<CString, BOOL> m_mapCheckInfo;//CString为委托号


private:
	CSimpleCheckbox m_chcShowNoTrade;
    CListCtrl m_listOrder;
	CGridCtrlEx m_gridOrder;
	CSimpleButton m_btnCancelCheck;
	CSimpleButton m_btnCancelAll;
	vector<tagColumn> vec_ViewCol
	{
		//{ ColId_Check, "勾选", 39, COLTYPE_FIX, FALSE }
		{ ColId_stockName, "股票名称", 130, COLTYPE_FIX, TRUE }
		,{ COLID_stockCode, "股票代码", 90, COLTYPE_FIX, TRUE }
		,{ COLID_direction, "买卖方向", 90, COLTYPE_FIX, TRUE }
		,{ ColId_EntrustPrice, "委托价格", 90, COLTYPE_FIX, TRUE, TRUE }
		,{ ColId_EntrstAmount, "委托数量", 90, COLTYPE_FIX, TRUE }
		,{ ColId_BusinessAmount, "成交数量", 90, COLTYPE_FIX, TRUE }
		,{ COLID_entrustStatus, "委托状态", 90, COLTYPE_FIX, TRUE }
		,{ ColId_EntrustNo, "委托号", 90, COLTYPE_FIX, TRUE }
		,{ COLID_reportTime, "申报时间", 90, COLTYPE_FIX, TRUE }
	};
};


