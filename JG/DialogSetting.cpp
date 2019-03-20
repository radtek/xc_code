// DialogSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "JG.h"
#include "DialogSetting.h"
#include "afxdialogex.h"



// CDialogSetting 对话框

IMPLEMENT_DYNAMIC(CDialogSetting, CDialogEx)

CDialogSetting::CDialogSetting(MapStation* pMapStation, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent),
	m_pMapStation(pMapStation)
{

}

CDialogSetting::~CDialogSetting()
{
}

void CDialogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Trade_Address, T2_CComboBox);
	DDX_Control(pDX, IDC_COMBO_Market_SSE, SSEMarket_CComboBox);
	DDX_Control(pDX, IDC_COMBO_Market_SZSE, SZSEMarket_CComboBox);
	DDX_Control(pDX, IDC_COMBO_Alert, Alert_CComboBox);
	DDX_Control(pDX, IDC_COMBO_Update, m_comboUpdateServer);
	DDX_Control(pDX, IDC_COMBO_Alert_SSE, m_comboAlertServer_SSE);
	DDX_Control(pDX, IDC_COMBO_Alert_SZSE, m_comboAlertServer_SZSE);
}


BEGIN_MESSAGE_MAP(CDialogSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Confirm, &CDialogSetting::OnBnClickedButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, &CDialogSetting::OnBnClickedButtonCancel)
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Button_Confirm, IDC_Button_Cancel, &CDialogSetting::OnButton)
	ON_BN_CLICKED(IDC_BUTTON_TradeAddr_Add, &CDialogSetting::OnBnClickedButtonTradeaddrAdd)
	ON_BN_CLICKED(IDC_BUTTON_TradeAddr_Modify, &CDialogSetting::OnBnClickedButtonTradeaddrModify)
	ON_BN_CLICKED(IDC_BUTTON_TradeAddr_Delete, &CDialogSetting::OnBnClickedButtonTradeaddrDelete)
	ON_BN_CLICKED(IDC_BUTTON_MarketSSE_Add, &CDialogSetting::OnBnClickedButtonMarketsseAdd)
	ON_BN_CLICKED(IDC_BUTTON_MarketSSE_Modify, &CDialogSetting::OnBnClickedButtonMarketsseModify)
	ON_BN_CLICKED(IDC_BUTTON_MarketSSE_Delete, &CDialogSetting::OnBnClickedButtonMarketsseDelete)
	ON_BN_CLICKED(IDC_BUTTON_MarketSZSE_Add, &CDialogSetting::OnBnClickedButtonMarketszseAdd)
	ON_BN_CLICKED(IDC_BUTTON_MarketSZSE_Modify, &CDialogSetting::OnBnClickedButtonMarketszseModify)
	ON_BN_CLICKED(IDC_BUTTON_MarketSZSE_Delete, &CDialogSetting::OnBnClickedButtonMarketszseDelete)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Add, &CDialogSetting::OnBnClickedButtonAlertAdd)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Modify, &CDialogSetting::OnBnClickedButtonAlertModify)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Delete, &CDialogSetting::OnBnClickedButtonAlertDelete)
	ON_BN_CLICKED(IDC_BUTTON_Update_Add, &CDialogSetting::OnBnClickedButtonUpdateAdd)
	ON_BN_CLICKED(IDC_BUTTON_Update_Modify, &CDialogSetting::OnBnClickedButtonUpdateModify)
	ON_BN_CLICKED(IDC_BUTTON_Update_Delete, &CDialogSetting::OnBnClickedButtonUpdateDelete)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Add_SZSE, &CDialogSetting::OnBnClickedButtonAlertAddSzse)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Modify_SZSE, &CDialogSetting::OnBnClickedButtonAlertModifySzse)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Delete_SZSE, &CDialogSetting::OnBnClickedButtonAlertDeleteSzse)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Add_SSE, &CDialogSetting::OnBnClickedButtonAlertAddSse)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Modify_SSE, &CDialogSetting::OnBnClickedButtonAlertModifySse)
	ON_BN_CLICKED(IDC_BUTTON_Alert_Delete_SSE, &CDialogSetting::OnBnClickedButtonAlertDeleteSse)
END_MESSAGE_MAP()


// CDialogSetting 消息处理程序

bool CDialogSetting::InitInfo()
{
	SetComboData(StationTrade, T2_CComboBox, m_pMapStation);
	SetComboData(StationSSEMarket, SSEMarket_CComboBox, m_pMapStation);
	SetComboData(StationSZSEMarket, SZSEMarket_CComboBox, m_pMapStation);
//	SetComboData(StationAlert, Alert_CComboBox, m_pMapStation);
	SetComboData(StationAlertSSE, m_comboAlertServer_SSE, m_pMapStation);
	SetComboData(StationAlertSZSE, m_comboAlertServer_SZSE, m_pMapStation);
	SetUpdateServerComboData();
	return true;
}

void CDialogSetting::SetUpdateServerComboData()
{
	try
	{
		ptree pt;
		read_xml(ConfigPath, pt);
		BOOST_AUTO(child_Trade, pt.get_child("content.UpdateServer"));
		int nIndex = 0;
		for (BOOST_AUTO(pos, child_Trade.begin()); pos != child_Trade.end(); ++pos)
		{
			string servername = pos->second.get<string>("servername");
			string serverip = pos->second.get<string>("serverip");
			int serverport = pos->second.get<int>("port");

			CString strText;
			strText.Format("%s:%s:%d", servername.c_str(), serverip.c_str(), serverport);
			m_comboUpdateServer.InsertString(nIndex++, strText);
		}

		string serverLastUsed = pt.get<string>("content.UpdateServerUseFirst");
		CString strServerLastUsed = serverLastUsed.c_str();

		int nCount = m_comboUpdateServer.GetCount();
		if (nCount > 0)
		{
			m_comboUpdateServer.SetCurSel(0);
		}
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			CString strText;
			m_comboUpdateServer.GetLBText(nIndex, strText);
			if (strText.Find(strServerLastUsed) >= 0)
			{
				m_comboUpdateServer.SetCurSel(nIndex);
			}
		}
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CDialogSetting::SetComboData(StationName name, CComboBox& combo, const MapStation* pStation)
{
	if (pStation != nullptr)
	{
		combo.ResetContent();
		auto iterTrade = pStation->find(name);
		if (iterTrade != pStation->end())
		{
			int nIndex = 0;
			for (auto data : iterTrade->second.vecData)
			{
				combo.InsertString(nIndex++, data);
			}
			if (combo.GetCount() > iterTrade->second.nCurSel)
			{
				combo.SetCurSel(iterTrade->second.nCurSel);
			}
		}
	}
}

BOOL CDialogSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Layout();
	InitInfo();
	
	return TRUE;
}

void CDialogSetting::GetWndRect(CWnd* pWnd, CRect& rect)
{
	if (pWnd != nullptr && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

void CDialogSetting::GetServerData(const CString& strText, ServerData& serverData)
{
	int nIndex1 = strText.Find(":");
	if (nIndex1 >= 0)
	{
		serverData.strName = strText.Mid(0, nIndex1);

		int nIndex2 = strText.Find(":", nIndex1 + 1);
		if (nIndex2 > 0)
		{
			serverData.strIP = strText.Mid(nIndex1 + 1, nIndex2 - nIndex1 - 1);
			int nLength = strText.GetLength();
			serverData.strPort = strText.Mid(nIndex2 + 1, nLength - nIndex2 - 1);
		}
	}
}

void CDialogSetting::SaveUpdateServerData()
{
	try
	{
		ptree pt;
		string s_ConfigPath("..\\share\\config.xml");
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);

		CString strTextUseFirst;
		m_comboUpdateServer.GetWindowText(strTextUseFirst);
		ServerData data;
		GetServerData(strTextUseFirst, data);
		CString strUpdateServerUseFirst;
		strUpdateServerUseFirst.Format("%s:%s", data.strIP, data.strPort);
		pt.put("content.UpdateServerUseFirst", strUpdateServerUseFirst);

		ptree& pp = pt.get_child("content.UpdateServer");
		pp.clear();

		string subItem = "UpdateServer1";
		ptree treeSubItem;
		treeSubItem.put(subItem + ".servername", data.strName);
		treeSubItem.put(subItem + ".serverip", data.strIP);
		treeSubItem.put(subItem + ".port", data.strPort);

		string item = "content.UpdateServer." + subItem;
		pt.put_child(item, treeSubItem.get_child(subItem));

		int nCount = m_comboUpdateServer.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			CString strText;
			m_comboUpdateServer.GetLBText(nIndex, strText);

			if (strTextUseFirst.Trim() != strText.Trim())
			{
				string subItem = "UpdateServer" + convert<string>(nIndex + 2);

				ServerData data;
				GetServerData(strText, data);

				ptree treeSubItem;
				treeSubItem.put(subItem + ".servername", data.strName);
				treeSubItem.put(subItem + ".serverip", data.strIP);
				treeSubItem.put(subItem + ".port", data.strPort);

				string item = "content.UpdateServer." + subItem;
				pt.put_child(item, treeSubItem.get_child(subItem));
			}
		}

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CDialogSetting::OnBnClickedButtonConfirm()
{
	SaveUpdateServerData();
	OnOK();
}

void CDialogSetting::OnBnClickedButtonCancel()
{
	OnCancel();
}

LRESULT CDialogSetting::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

int CDialogSetting::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_btnConfirm.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_Confirm, "保  存", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	m_btnConfirm.SetRgnDefColor(RGB(132, 45, 43));
	m_btnConfirm.SetRgnHoverColor(RGB(170, 58, 55));
	m_btnConfirm.SetDisabledColor(RGB(100, 34, 32));

	m_btnCancel.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_Cancel, "取  消", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	m_btnCancel.SetRgnDefColor(RGB(132, 45, 43));
	m_btnCancel.SetRgnHoverColor(RGB(170, 58, 55));
	m_btnCancel.SetDisabledColor(RGB(100, 34, 32));

// 	m_btnTradeAddrAdd.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_TradeAddr_Add, "添加", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnTradeAddrAdd.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnTradeAddrAdd.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnTradeAddrModify.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_TradeAddr_Modify, "修改", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnTradeAddrModify.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnTradeAddrModify.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnTradeAddrDelete.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_TradeAddr_Delete, "删除", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnTradeAddrDelete.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnTradeAddrDelete.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSSEAdd.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSSE_Add, "添加", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSSEAdd.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSSEAdd.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSSEModify.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSSE_Modify, "修改", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSSEModify.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSSEModify.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSSEDelete.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSSE_Delete, "删除", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSSEDelete.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSSEDelete.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSZSEAdd.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSZSE_Add, "添加", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSZSEAdd.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSZSEAdd.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSZSEModify.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSZSE_Modify, "修改", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSZSEModify.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSZSEModify.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnMarketSZSEDelete.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_MarketSZSE_Delete, "删除", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnMarketSZSEDelete.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnMarketSZSEDelete.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnAlertAdd.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_Alert_Add, "添加", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnAlertAdd.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnAlertAdd.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnAlertModify.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_Alert_Modify, "修改", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnAlertModify.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnAlertModify.SetRgnHoverColor(RGB(170, 58, 55));
// 
// 	m_btnAlertDelete.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, ID_BUTTON_Alert_Delete, "删除", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnAlertDelete.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnAlertDelete.SetRgnHoverColor(RGB(170, 58, 55));



	return 0;
}

void CDialogSetting::Layout()
{
	CRect rectConfirm,  rectCancel;
	GetWndRect(GetDlgItem(IDC_BUTTON_Confirm), rectConfirm);
	GetWndRect(GetDlgItem(IDC_BUTTON_Cancel), rectCancel);

// 	CRect rectTradeAddrAdd, rectTradeAddrModify, rectTradeAddrDelete;
// 	GetWndRect(GetDlgItem(IDC_BUTTON_TradeAddr_Add), rectTradeAddrAdd);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_TradeAddr_Modify), rectTradeAddrModify);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_TradeAddr_Delete), rectTradeAddrDelete);
// 
// 	CRect rectMarketSSEAdd, rectMarketSSEModify, rectMarketSSEDelete;
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSSE_Add), rectMarketSSEAdd);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSSE_Modify), rectMarketSSEModify);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSSE_Delete), rectMarketSSEDelete);
// 
// 	CRect rectMarketSZSEAdd, rectMarketSZSEModify, rectMarketSZSEDelete;
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSZSE_Add), rectMarketSZSEAdd);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSZSE_Modify), rectMarketSZSEModify);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_MarketSZSE_Delete), rectMarketSZSEDelete);
// 
// 	CRect rectAlertAdd, rectAlertModify, rectAlertDelete;
// 	GetWndRect(GetDlgItem(IDC_BUTTON_Alert_Add), rectAlertAdd);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_Alert_Modify), rectAlertModify);
// 	GetWndRect(GetDlgItem(IDC_BUTTON_Alert_Delete), rectAlertDelete);



	if (::IsWindow(m_btnConfirm.GetSafeHwnd()))
	{
		m_btnConfirm.MoveWindow(&rectConfirm);
		m_btnCancel.MoveWindow(&rectCancel);

// 		m_btnTradeAddrAdd.MoveWindow(&rectTradeAddrAdd);
// 		m_btnTradeAddrModify.MoveWindow(&rectTradeAddrModify);
// 		m_btnTradeAddrDelete.MoveWindow(&rectTradeAddrDelete);
// 
// 	    m_btnMarketSSEAdd.MoveWindow(&rectMarketSSEAdd);
// 		m_btnMarketSSEModify.MoveWindow(&rectMarketSSEModify);
// 		m_btnMarketSSEDelete.MoveWindow(&rectMarketSSEDelete);
// 
// 		m_btnMarketSZSEAdd.MoveWindow(&rectMarketSZSEAdd);
// 		m_btnMarketSZSEModify.MoveWindow(&rectMarketSZSEModify);
// 		m_btnMarketSZSEDelete.MoveWindow(&rectMarketSZSEDelete);
// 
// 		m_btnAlertAdd.MoveWindow(&rectAlertAdd);
// 		m_btnAlertModify.MoveWindow(&rectAlertModify);
// 		m_btnAlertDelete.MoveWindow(&rectAlertDelete);

	}
}

void CDialogSetting::SetStationData(CStationInfo* pInfo, CComboBox* pComboBox)
{
	if (pInfo != nullptr && pComboBox != nullptr && ::IsWindow(pComboBox->GetSafeHwnd()))
	{
		pInfo->nCurSel = pComboBox->GetCurSel();
		int nCount = pComboBox->GetCount();
		for (int nIndex = 0; nIndex < nCount ; nIndex ++)
		{
			CString strText;
			pComboBox->GetLBText(nIndex, strText);
			pInfo->vecData.push_back(strText);
		}
	}
}

void CDialogSetting::InsertMapStation(StationName name, CComboBox* pComboBox)
{
	CStationInfo info;
	SetStationData(&info, pComboBox);
	m_pMapStation->insert(make_pair(name, info));
}

void CDialogSetting::OnButton(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Button_Confirm && nID <= IDC_Button_Cancel)
	{
		switch (nID)
		{
		case IDC_Button_Confirm:
			if (m_pMapStation != nullptr)
			{
				m_pMapStation->clear();
				InsertMapStation(StationTrade, &T2_CComboBox);
				InsertMapStation(StationSSEMarket, &SSEMarket_CComboBox);
				InsertMapStation(StationSZSEMarket, &SZSEMarket_CComboBox);
			//	InsertMapStation(StationAlert, &Alert_CComboBox);
				InsertMapStation(StationAlertSSE, &m_comboAlertServer_SSE);
				InsertMapStation(StationAlertSZSE, &m_comboAlertServer_SZSE);
			}
			SaveUpdateServerData();
			OnOK();
			break;
		case IDC_Button_Cancel:
			OnCancel();
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}

void CDialogSetting::GetData(const CComboBox* pComboBox, CEditInfo* pEditInfo)
{
	if (pComboBox != nullptr && ::IsWindow(pComboBox->GetSafeHwnd()) 
		&& pEditInfo != nullptr)
	{
		CString strText;
		pComboBox->GetWindowText(strText);
		int nIndex1 = strText.Find(":");
		if (nIndex1 >= 0)
		{
			pEditInfo->strName = strText.Mid(0, nIndex1);

			int nIndex2 = strText.Find(":", nIndex1 + 1);
			if (nIndex2 > 0)
			{
				pEditInfo->strAddr = strText.Mid(nIndex1 + 1, nIndex2 - nIndex1 - 1);
				int nLength = strText.GetLength();
				pEditInfo->strPort = strText.Mid(nIndex2 + 1, nLength - nIndex2 - 1);
			}

		}
	}
}

void CDialogSetting::EditData(CString strName, CComboBox* pComboBox, EditStyle style)
{
	if (EditDelete == style)
	{
		CString strTip;
		strTip.Format("您确定删除此%s吗？", strName);
		int nResult = MessageBox(strTip, "删除提示", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (IDOK == nResult)
		{
			int nCurSel = pComboBox->GetCurSel();
			pComboBox->DeleteString(nCurSel);
			int nCount = pComboBox->GetCount();
			if (nCount >= (nCurSel + 1))
			{
				pComboBox->SetCurSel(nCurSel);
			}
			else
			{
				pComboBox->SetCurSel(nCurSel - 1);
			}
		}
	}
	else
	{
		CEditInfo editInfo;
		InitialEditInfo(&editInfo, strName, style);

		if (EditModify == style)
		{
			GetData(pComboBox, &editInfo);
		}

		CEditDlg editDlg(&editInfo);
		int nResult = editDlg.DoModal();
		if (IDOK == nResult)
		{
			CString strText;
			strText.Format("%s:%s:%s", editInfo.strName, editInfo.strAddr, editInfo.strPort);

			if (EditAdd == style)
			{
				int nCount = pComboBox->GetCount();
				pComboBox->InsertString(nCount, strText);
				pComboBox->SetCurSel(nCount);
			}
			else
			{
				int nCurSel = pComboBox->GetCurSel();
				pComboBox->DeleteString(nCurSel);
				pComboBox->InsertString(nCurSel, strText);
				pComboBox->SetCurSel(nCurSel);
			}
		}
	}
}

void CDialogSetting::InitialEditInfo(CEditInfo* pEditInfo, CString strName, EditStyle style)
{
	if (pEditInfo != nullptr)
	{
		if (EditAdd == style)
		{
			pEditInfo->strCaption = "增加" + strName;
		}
		else if (EditModify == style)
		{
			pEditInfo->strCaption = "修改" + strName;
		}
		pEditInfo->strNameTip = strName + "名称：";
		pEditInfo->strAddrTip = strName + "地址：";
	}
}

void CDialogSetting::OnBnClickedButtonTradeaddrAdd()
{
	EditData("交易", &T2_CComboBox, EditAdd);
}

void CDialogSetting::OnBnClickedButtonTradeaddrModify()
{
	EditData("交易", &T2_CComboBox, EditModify);
}


void CDialogSetting::OnBnClickedButtonTradeaddrDelete()
{
	EditData("交易", &T2_CComboBox, EditDelete);
}


void CDialogSetting::OnBnClickedButtonMarketsseAdd()
{
	EditData("上海行情", &SSEMarket_CComboBox, EditAdd);
}


void CDialogSetting::OnBnClickedButtonMarketsseModify()
{
	EditData("上海行情", &SSEMarket_CComboBox, EditModify);
}


void CDialogSetting::OnBnClickedButtonMarketsseDelete()
{
	EditData("上海行情", &SSEMarket_CComboBox, EditDelete);
}


void CDialogSetting::OnBnClickedButtonMarketszseAdd()
{
	EditData("深圳行情", &SZSEMarket_CComboBox, EditAdd);
}


void CDialogSetting::OnBnClickedButtonMarketszseModify()
{
	EditData("深圳行情", &SZSEMarket_CComboBox, EditModify);
}


void CDialogSetting::OnBnClickedButtonMarketszseDelete()
{
	EditData("深圳行情", &SZSEMarket_CComboBox, EditDelete);
}


void CDialogSetting::OnBnClickedButtonAlertAdd()
{
//	EditData("预警", &Alert_CComboBox, EditAdd);
}


void CDialogSetting::OnBnClickedButtonAlertModify()
{
//	EditData("预警", &Alert_CComboBox, EditModify);
}

void CDialogSetting::OnBnClickedButtonAlertDelete()
{
//	EditData("预警", &Alert_CComboBox, EditDelete);
}

void CDialogSetting::OnBnClickedButtonUpdateAdd()
{
	EditData("升级服务器", &m_comboUpdateServer, EditAdd);
}

void CDialogSetting::OnBnClickedButtonUpdateModify()
{
	EditData("升级服务器", &m_comboUpdateServer, EditModify);
}

void CDialogSetting::OnBnClickedButtonUpdateDelete()
{
	EditData("升级服务器", &m_comboUpdateServer, EditDelete);
}

void CDialogSetting::OnBnClickedButtonAlertAddSse()
{
	EditData("上海预警", &m_comboAlertServer_SSE, EditAdd);
}

void CDialogSetting::OnBnClickedButtonAlertModifySse()
{
	EditData("上海预警", &m_comboAlertServer_SSE, EditModify);
}

void CDialogSetting::OnBnClickedButtonAlertDeleteSse()
{
	EditData("上海预警", &m_comboAlertServer_SSE, EditDelete);
}

void CDialogSetting::OnBnClickedButtonAlertAddSzse()
{
	EditData("深圳预警", &m_comboAlertServer_SZSE, EditAdd);
}

void CDialogSetting::OnBnClickedButtonAlertModifySzse()
{
	EditData("深圳预警", &m_comboAlertServer_SZSE, EditModify);
}

void CDialogSetting::OnBnClickedButtonAlertDeleteSzse()
{
	EditData("深圳预警", &m_comboAlertServer_SZSE, EditDelete);
}
