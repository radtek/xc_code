#pragma once
#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "SimpleCheckbox.h"
#include "SimpleEdit.h"
#include "SimpleStatic.h"
#include "SimpleButton.h"
#include "SimpleComboBox.h"
#include "SimpleRadioButton.h"
#include "ViewManager_auto.h"
#include "SimpleButtonEdit.h"
#include "SimpleTime.h"
#include "GridCtrlEx.h"
#include "MainView_AutoTrade.h"

class CView_AlertRecord : public CBaseView
{
	DECLARE_DYNAMIC(CView_AlertRecord)
public:
	CView_AlertRecord(CDockListener &listener);
	~CView_AlertRecord();
	static CBaseView *CreateView(CDockListener &listener) 
	{ 
		return new CView_AlertRecord(listener);
	}
	virtual void ReleaseView();
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void InitialWindow();
	void CreateControls();
	bool initPreRecord();
	virtual BOOL GetDefaultRect(CRect &rect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitGrid();
	//虚表回调
	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
	void SetCell(GV_DISPINFO & dispinfo, CAertRecord &obj_CAertRecord);
public:
	void SetAlertRecord();
private:
	enum
	{
		IDC_Start = 0x1001,
		//逐笔委托过滤
		IDC_Grid_AlertRecord,

	};
	CGridCtrlEx CGridCtrlEx_alertRecord;
	CApiFun *pCApiFun;
	PtrData SPtr_CData;

	vector<tagColumn> vec_ViewCol
	{
		{ ColID_time, "触发时间", 90, COLTYPE_FIX, TRUE }
		,{ ColID_secCode, "股票代码", 90, COLTYPE_FIX, TRUE }
		,{ ColID_secName, "股票名称", 90, COLTYPE_FIX, TRUE }
		,{ ColID_triggerName, "触发原因", 300, COLTYPE_FIX, TRUE }
		,{ ColID_entrustNo, "委托号", 90, COLTYPE_FIX, TRUE }
		,{ ColID_entrustPrice, "委托价格", 90, COLTYPE_FIX, TRUE }
		,{ ColID_entrustAmount, "委托数量", 90, COLTYPE_FIX, TRUE }
		,{ ColID_note, "注释", 150, COLTYPE_FIX, TRUE }
	};
};