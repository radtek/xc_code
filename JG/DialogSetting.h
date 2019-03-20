#pragma once
#include "afxwin.h"
#include "SimpleButton.h"
#include "EditDlg.h"
#include "LoginDlg.h"

// CDialogSetting 对话框

struct ServerData
{
	CString strName;
	CString strIP;
	CString strPort;
};

class CIndexInfo
{
public:
	CIndexInfo() :
		nTradeAddress(0),
		nMarketSSE(0),
		nMarketSZSE(0),
		nAlert(0)
	{}
	int nTradeAddress;
	int nMarketSSE;
	int nMarketSZSE;
	int nAlert;
};

class CDialogSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetting)
	enum
	{
		IDC_START = 0x1001,
		IDC_Button_Confirm,
		IDC_Button_Cancel,

		ID_BUTTON_TradeAddr_Add,
		ID_BUTTON_TradeAddr_Modify,
		ID_BUTTON_TradeAddr_Delete,

		ID_BUTTON_MarketSSE_Add,
		ID_BUTTON_MarketSSE_Modify,
		ID_BUTTON_MarketSSE_Delete,

		ID_BUTTON_MarketSZSE_Add,
		ID_BUTTON_MarketSZSE_Modify,
		ID_BUTTON_MarketSZSE_Delete,

		ID_BUTTON_Alert_Add,
		ID_BUTTON_Alert_Modify,
		ID_BUTTON_Alert_Delete


	};

public:
	CDialogSetting(MapStation* pMapStation,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogSetting();

	virtual BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

private:
	MapStation* m_pMapStation;
	bool InitInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox T2_CComboBox;
	CComboBox SSEMarket_CComboBox;
	CComboBox SZSEMarket_CComboBox;
	CComboBox Alert_CComboBox;
	afx_msg void OnBnClickedButtonConfirm();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnButton(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);

private:
	CSimpleButton m_btnConfirm;
	CSimpleButton m_btnCancel;

// 	CSimpleButton m_btnTradeAddrAdd;
// 	CSimpleButton m_btnTradeAddrModify;
// 	CSimpleButton m_btnTradeAddrDelete;
// 
// 	CSimpleButton m_btnMarketSSEAdd;
// 	CSimpleButton m_btnMarketSSEModify;
// 	CSimpleButton m_btnMarketSSEDelete;
// 
// 	CSimpleButton m_btnMarketSZSEAdd;
// 	CSimpleButton m_btnMarketSZSEModify;
// 	CSimpleButton m_btnMarketSZSEDelete;
// 
// 	CSimpleButton m_btnAlertAdd;
// 	CSimpleButton m_btnAlertModify;
// 	CSimpleButton m_btnAlertDelete;

private:
	void Layout();
	void GetWndRect(CWnd* pWnd, CRect& rect);
	void GetData(const CComboBox* pComboBox, CEditInfo* pEditInfo);
	void InitialEditInfo(CEditInfo* pEditInfo, CString strName, EditStyle style);
	void EditData(CString strName, CComboBox* pComboBox, EditStyle style);
	void SetStationData(CStationInfo* pInfo, CComboBox* pComboBox);
	void InsertMapStation(StationName name, CComboBox* pComboBox);
	void SetComboData(StationName name, CComboBox& combo, const MapStation* pStation);
	void SetUpdateServerComboData();
	void SaveUpdateServerData();
	void GetServerData(const CString& strText, ServerData& serverData);

public:
	afx_msg void OnBnClickedButtonTradeaddrAdd();
	afx_msg void OnBnClickedButtonTradeaddrModify();
	afx_msg void OnBnClickedButtonTradeaddrDelete();
	afx_msg void OnBnClickedButtonMarketsseAdd();
	afx_msg void OnBnClickedButtonMarketsseModify();
	afx_msg void OnBnClickedButtonMarketsseDelete();
	afx_msg void OnBnClickedButtonMarketszseAdd();
	afx_msg void OnBnClickedButtonMarketszseModify();
	afx_msg void OnBnClickedButtonMarketszseDelete();
	afx_msg void OnBnClickedButtonAlertAdd();
	afx_msg void OnBnClickedButtonAlertModify();
	afx_msg void OnBnClickedButtonAlertDelete();
	afx_msg void OnBnClickedButtonUpdateAdd();
	afx_msg void OnBnClickedButtonUpdateModify();
	afx_msg void OnBnClickedButtonUpdateDelete();
	CComboBox m_comboUpdateServer;
	CComboBox m_comboAlertServer_SSE;
	CComboBox m_comboAlertServer_SZSE;
	afx_msg void OnBnClickedButtonAlertAddSzse();
	afx_msg void OnBnClickedButtonAlertModifySzse();
	afx_msg void OnBnClickedButtonAlertDeleteSzse();
	afx_msg void OnBnClickedButtonAlertAddSse();
	afx_msg void OnBnClickedButtonAlertModifySse();
	afx_msg void OnBnClickedButtonAlertDeleteSse();
};
