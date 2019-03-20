#include "TradeDlg.h"
#include "./MyCtrl/FontSet.h"

HFONT CTradeDlg::s_hFontOrder = NULL;
HFONT CTradeDlg::s_hFontGrid = NULL;
HFONT CTradeDlg::s_hFontTab = NULL;
HFONT CTradeDlg::s_hFontTabSel = NULL;

CTradeDlg::CTradeDlg()
{
	s_ConfigPath = ConfigPath;
	bCreate_TradeDlg = false;
	bShowAll = true;
	m_strStyle = GetFontStyle();
	m_hMainViewWnd = nullptr;
}

bool CTradeDlg::initCaption(const unsigned int CtrID, const CString &s_caption)
{
	unsigned int fontSize;
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		fontSize = pt.get<int>("content.FontSize");
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	unsigned int size = fontSize*1.5;
	size *= 10;
	font_Caption.CreatePointFont(size, "MS Shell Dlg");//控件1		
	GetDlgItem(CtrID)->SetWindowText(s_caption);
	GetDlgItem(CtrID)->SetFont(&font_Caption, true); //更改控件1的字体
	return true;
}

bool CTradeDlg::initBDirectlyOrder(const string &treePath,const unsigned int &CtrID)
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		bDirectlyOrder = pt.get<bool>(treePath);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}

	switch (bDirectlyOrder)
	{
	case false://未被选中
		((CButton*)GetDlgItem(CtrID))->SetCheck(BST_UNCHECKED);
		break;
	case true://复选框被选中
		((CButton*)GetDlgItem(CtrID))->SetCheck(BST_CHECKED);
		break;
	}
	return true;
}

bool CTradeDlg::init_bStockAssociated(const string &treePath)
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		bStockAssociated = pt.get<bool>(treePath);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	return true;
}

void CTradeDlg::InitialAutoCheck(CSimpleCheckbox* pCheck, string str)
{
	if (IsWindowValid(pCheck))
	{
		ptree pt;
		try {
			read_xml(s_ConfigPath, pt);
			bAutoRoll = pt.get<bool>(str);
		}
		catch (std::exception& e)
		{
			LOG(ERROR) << "Error:" << e.what();
			ExceptionInterrupt;
			return;
		}

		pCheck->SetCheck(bAutoRoll);
	}
}

void CTradeDlg::InitialRMouseOrderCheck(CSimpleCheckbox* pCheck, string str)
{
	if (IsWindowValid(pCheck))
	{
		ptree pt;
		try {
			read_xml(s_ConfigPath, pt);
			bDirectlyOrder = pt.get<bool>(str);
		}
		catch (std::exception& e)
		{
			LOG(ERROR) << "Error:" << e.what();
			ExceptionInterrupt;
			return;
		}

		pCheck->SetCheck(bDirectlyOrder);
	}
}


void CTradeDlg::initLayout_BEntrustQueue()
{
	obj_CListCtrlInfo.str_vector.clear();
	obj_CListCtrlInfo.str_vector.push_back("买一");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("手数");
	UInt_32 dwStyle = GainNormalExtendedStyle(CListCtrl_BEntrustQueue)& ~LVS_EX_GRIDLINES;
	uniformClistCtrl(obj_CListCtrlInfo, CListCtrl_BEntrustQueue, dwStyle);
}

void CTradeDlg::initLayout_SEntrustQueue()
{
	obj_CListCtrlInfo.str_vector.clear();
	obj_CListCtrlInfo.str_vector.push_back("卖一");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("");
	obj_CListCtrlInfo.str_vector.push_back("手数");
	UInt_32 dwStyle = GainNormalExtendedStyle(CListCtrl_SEntrustQueue)& ~LVS_EX_GRIDLINES;
	uniformClistCtrl(obj_CListCtrlInfo, CListCtrl_SEntrustQueue, dwStyle);
}

void CTradeDlg::initLayout_EntrustEach()
{
	obj_CListCtrlInfo.str_vector.clear();
	obj_CListCtrlInfo.str_vector.push_back("时间");
	obj_CListCtrlInfo.str_vector.push_back(Name_EntrustNo);
	obj_CListCtrlInfo.str_vector.push_back("价格");
	obj_CListCtrlInfo.str_vector.push_back("数量");
	obj_CListCtrlInfo.str_vector.push_back("金额");
	obj_CListCtrlInfo.str_vector.push_back(Name_EntrustBs);
	UInt_32 dwStyle = GainNormalExtendedStyle(CListCtrl_EntrustEach)& ~LVS_EX_GRIDLINES;
	uniformClistCtrl(obj_CListCtrlInfo, CListCtrl_EntrustEach, dwStyle);
}

void CTradeDlg::initLayout_CustomPool()
{
	obj_CListCtrlInfo.str_vector.clear();
	obj_CListCtrlInfo.str_vector.push_back("市场");
	obj_CListCtrlInfo.str_vector.push_back("股票名称");
	obj_CListCtrlInfo.str_vector.push_back(Name_SecCode);
	UInt_32 dwStyle = GainNormalExtendedStyle(CListCtrl_CustomPool) | LVS_EX_CHECKBOXES;
	uniformClistCtrl(obj_CListCtrlInfo, CListCtrl_CustomPool, dwStyle);
}

void CTradeDlg::initRefreshAlert(const unsigned int CtrID)
{
	( (CButton *)GetDlgItem(CtrID)) ->SetCheck(BST_CHECKED);
}

void CTradeDlg::RedrawCustomPool(string s_Path, CGridCtrlEx* pCustomPool)
{
	if (!IsWindowValid(pCustomPool))
	{
		return;
	}
	map<CString, CCustomInfo> M_CustomPool;//key stock_code value stock_name
	if (File2MCustomPool(s_Path, M_CustomPool) == false)
	{
		return;
	}
	pCustomPool->SetRowCount(M_CustomPool.size() + 1);
	int i = 0;
	for (auto iter = M_CustomPool.begin(); iter != M_CustomPool.end(); iter++)
	{
		CString s_str;
		// 市场
		switch (tool::find_enumMarketCode(string(iter->second.MarketCode)))
		{
		case enumSSE:
			s_str = "上海";
			break;
		case enumSZSE:
			s_str = "深圳";
			break;
		default:
			ExceptionInterrupt;
			LOG(ERROR) << "abnormal MarketCode";
			break;
		}
		pCustomPool->SetCellCheckBox(i + 1, 0);
		pCustomPool->SetItemEditable(i + 1, 0, TRUE);

		pCustomPool->SetItemTextEx(i + 1, 0, s_str);

		//股票名称
		pCustomPool->SetItemTextEx(i + 1, 1, iter->second.stock_name);
		//股票代码
		pCustomPool->SetItemTextEx(i + 1, 2, iter->second.stock_code);
		i++;
	}
	//pCustomPool->MySetSelectedRangeEx(-1, -1, -1, -1, TRUE);
	pCustomPool->Invalidate();
}

bool CTradeDlg::File2MCustomPool(string s_Path, map<CString, CCustomInfo> &M_CustomPool)
{
	std::string s_FileContent;
	tool::File2String(s_Path, s_FileContent);
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(s_FileContent, root, false))
	{
		int Sum = root["CustomInfo"].size();
		if (Sum >= Max_Monitor)
		{
			ExceptionInterrupt;
			LOG(ERROR) << "预警个数过多";
			return false;
		}
		for (int i = 0; i < Sum; ++i)
		{
			CCustomInfo obj_CCustomInfo;
			obj_CCustomInfo.stock_code = root["CustomInfo"][i]["stock_code"].asString().c_str();
			obj_CCustomInfo.stock_name = root["CustomInfo"][i]["stock_name"].asString().c_str();
			obj_CCustomInfo.MarketCode = root["CustomInfo"][i]["MarketCode"].asString().c_str();
			M_CustomPool.insert(make_pair(obj_CCustomInfo.stock_code, obj_CCustomInfo));
		}
	}
	else
	{
		if ("" == s_FileContent)
		{
			return false;
		}
		else
		{
			ExceptionInterrupt;
			LOG(ERROR) << "prase fail:" << s_FileContent;
			return false;
		}
	}
	return true;
}

void CTradeDlg::DealClickedRemovePick(string s_Path, CGridCtrlEx* pCustomPool)
{
	if (!IsWindowValid(pCustomPool))
	{
		return;
	}
	map<CString, CCustomInfo> Map_CustomPool;
	if (File2MCustomPool(s_Path, Map_CustomPool) == false)
	{
		return;
	}
	int nRowCount = pCustomPool->GetRowCount();
	for (int nRow = 1; nRow < nRowCount; nRow++)
	{
		if (pCustomPool->GetCellCheck(nRow, 0))
		{
			CString stock_code = pCustomPool->GetItemText(nRow, 2);
			Map_CustomPool.erase(stock_code);
		}
	}
	/*写配置文件*/
	Json::Reader reader;
	Json::Value root;
	string s_FileContent;
	tool::File2String(s_Path, s_FileContent);

	if (reader.parse(s_FileContent, root, false))
	{
		root.removeMember("CustomInfo");
		for (auto iter = Map_CustomPool.begin(); iter != Map_CustomPool.end(); iter++)
		{
			Json::Value val;
			val["stock_code"] = Json::Value(iter->second.stock_code);
			val["stock_name"] = Json::Value(iter->second.stock_name);
			val["MarketCode"] = Json::Value(iter->second.MarketCode);
			root["CustomInfo"].append(Json::Value(val));
		}

		Json::StyledWriter sw;
		std::string res_str = sw.write(root);
		tool::string2File(s_Path, res_str);
	}
	else
	{
		ExceptionInterrupt;
		LOG(ERROR) << "parse fail:" << s_FileContent;
		return;
	}
	/*重绘自选池*/	
	RedrawCustomPool(s_Path, pCustomPool);
}

bool CTradeDlg::DealInsertStock(CSimpleEdit* pEditCode, string s_Path, CGridCtrlEx* pCustomPool)
{
	if (!IsWindowValid(pEditCode) || !IsWindowValid(pCustomPool))
	{
		return false;
	}
	CCustomInfo obj_CCustomInfo;
	/*校验添加代码是否合法*/
	if (GainAlertInfo(pEditCode, obj_CCustomInfo) == false)
	{
		return false;
	}
	/*写入添加代码*/
	if (WriteAlertInfo(s_Path, obj_CCustomInfo) == false)
	{
		return false;
	}
	/*重绘自选池*/
	RedrawCustomPool(s_Path, pCustomPool);
	return true;
}

void CTradeDlg::DealClickedRefreshAlert(CSimpleCheckbox* pRefresh)
{
	if (IsWindowValid(pRefresh))
	{
		bRefreshAlert = pRefresh->GetCheck();		
	}
}

bool CTradeDlg::save_bStockAssociated(const string &treePath, const unsigned int &CtrID)
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		CString s_tmp;
		if (((CButton*)GetDlgItem(CtrID))->GetCheck() == BST_CHECKED)
		{
			s_tmp = "1";
		}
		else
		{
			s_tmp = "0";
		}
		pt.put(treePath, s_tmp);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	return true;
}


bool CTradeDlg::SubscribePosition()
{
	interface_struct_Subscribe sSubscribe1;
	sSubscribe1.Dyna_flag = true;
	socket_struct_SubscribeDetail SubList1[MaxSize];
	int Sum_Subscribe = 0;
	CString MarketCode;

	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Position obj_MPosition;
			if (SPtr_CData->GetMPosition(SPtr_CData->fund_account, iter_MAccount->first, obj_MPosition) == false)
			{
				ErrorLog("GetMPosition fail");
				continue;
			}
			if (SPtr_CData->Account2Market(SPtr_CData->fund_account, iter_MAccount->first,MarketCode) == false)
			{
				ErrorLog("Account2Market fail");
				continue;
			}

			for (auto iter = obj_MPosition.begin(); iter != obj_MPosition.end(); iter++)
			{
				strcpy(SubList1[Sum_Subscribe].MarketCode, MarketCode);
				strcpy(SubList1[Sum_Subscribe].SecCode, iter->second->stock_code);
				Sum_Subscribe++;
			}
			SendSubscribeMarket(false, MarketCode, IDMarket_SubscribePosition, &sSubscribe1, SubList1, Sum_Subscribe);/*fixing 融资融券存在问题*/
			memset(SubList1, 0,sizeof(socket_struct_SubscribeDetail)*Sum_Subscribe);
			Sum_Subscribe = 0;
		}
	}
	else
	{
		ErrorLog("GetAccountMap fail");
		return false;
	}

	return true;
}

bool CTradeDlg::Subscribed_SingleStock(bool bLock, const CString &MarketCode, const CString &SecCode, bool bEntrustQueue, bool bEntrustEach)
{
	interface_struct_Subscribe sSubscribe1;
	sSubscribe1.Dyna_flag = false;
	if (true == bEntrustQueue)
	{
		sSubscribe1.DepthOrder_flag = true;
	}
	else
	{
		sSubscribe1.DepthOrder_flag = false;
	}
	sSubscribe1.Depth_flag = true;
	sSubscribe1.EachDeal_flag = false;
	if (true == bEntrustEach)
	{
		sSubscribe1.EachOrder_flag = true;
	}
	else
	{
		sSubscribe1.EachOrder_flag = false;
	}

	socket_struct_SubscribeDetail SubList1[MaxSize];
	if (sSubscribe1.EachOrder_flag == true)
	{
		if (SPtr_CData->ClearEntrustEach(MarketCode, SecCode) == false)
		{
			ExceptionInterrupt;
			LOG(ERROR) << "ClearEntrustEach fail";
			Sleep(1000);
		}
	}

	strcpy(SubList1[0].MarketCode, MarketCode);
	strcpy(SubList1[0].SecCode, SecCode);
	SendSubscribeMarket(bLock,MarketCode, IDMarket_SubscribeSingle, &sSubscribe1, SubList1, 1);
	return true;
}

bool CTradeDlg::SendSubscribeMarket(bool bLock,const CString MarketCode, UInt_64 RefID, interface_struct_Subscribe* pSubscribe,
	socket_struct_SubscribeDetail SubList[MaxSize], int SubSize)
{
	if (SubSize < 1)
	{
		return true;
	}
	int ret;
	switch (tool::find_enumMarketCode(string(MarketCode)))
	{
	case enumSSE:
	{
		ret = pCApiFun->Api_Market_SSE->Subscribe(RefID, pSubscribe, SubList, SubSize);
		break;
	}
	case enumSZSE:
	{
		ret = pCApiFun->Api_Market_SZSE->Subscribe(RefID, pSubscribe, SubList, SubSize);
		break;
	}
	default:
		LOG(ERROR) << "abnormal MarketCode";
		ExceptionInterrupt;
		return false;
		break;
	}
	if (ret < 0)
	{
		LOG(ERROR) << "SendSubscribeMarket fail! ret:" << ret;
		ExceptionInterrupt;
		return false;
	}
	if (true == bLock)
	{
		/*正常清空下lock unlock应该是一对，但是由于行情模块的特殊性，有两种模块会引起unlock，却只有单个模块lock，使lock生效*/
		ResetEvent(SPtr_CData->Subscribe_Event);
		/*等待是为了保证快照行情等已达到*/
		UInt_32 ret_Wait = WaitForSingleObject(SPtr_CData->Subscribe_Event, TimeShortest);
		bool b_ret;
		DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	}
	return true;
}

bool CTradeDlg::SendAllOrder(vector<string> &Vec_Dismantling, CApiFun *pCApiFun, const int ApiType,
	const string &s_Fun, PtrData &SPtr_CData)
{
	int OrderInterval = GainSleepTime(SPtr_CData);
	int Size = Vec_Dismantling.size();
	if (Size == 1)//单个指令
	{
		string s_str;
		if (modifyEntrustAmount(s_str, s_Fun, Vec_Dismantling[0]) == true)
		{
			if (tool::SendOrder(pCApiFun, ApiType, s_str, SPtr_CData) < 0)
			{
				ExceptionInterrupt;
				LOG(ERROR) << "SendOrder fail";
				return false;
			}
		}
	}
	else
	{
		std::thread t_ErrorInfo(Thread_SendAllOrder, OrderInterval, Vec_Dismantling, pCApiFun,
			ApiType, s_Fun, SPtr_CData);
		t_ErrorInfo.detach();
	}
	return true;
}

void CTradeDlg::Thread_SendAllOrder(int OrderInterval, vector<string> Vec_Dismantling, CApiFun *pCApiFun,
	const int ApiType, const string s_Fun, PtrData SPtr_CData)
{
	bool b_ret;
	DWORD ret_Event = WaitForSingleObject(SPtr_CData->Event_SendAllOrder, INFINITE);
	DealReturnValue_WaitForSingleObject(ret_Event, b_ret);
	if (b_ret == false)
	{
		return;
	}
	int Size = Vec_Dismantling.size();
	Json::Reader reader;
	Json::Value root;
	for (int i = 0; i < Size; i++)
	{
		string s_str;
		//从字符串中读取数据
		if (reader.parse(s_Fun, root))
		{
			root["entrust_amount"] = Vec_Dismantling[i];
			s_str = root.toStyledString();
			//cerr << "now tid is " << GetCurrentThreadId() << endl;
			if (tool::SendOrder(pCApiFun, ApiType, s_str, SPtr_CData) < 0)
			{
				ExceptionInterrupt;
				LOG(ERROR) << "SendOrder fail";
				return;
			}
			if (0 != OrderInterval)
			{
				Sleep(OrderInterval);
			}
		}
		else
		{
			string s_error = "parse fail:";
			s_error += s_Fun;
			ErrorLog(s_error);
			return;
		}
	}
	SetEvent(SPtr_CData->Event_SendAllOrder);
}


int CTradeDlg::GainSleepTime(PtrData &SPtr_CData)
{
	/*下单间隔为0的时候，客户端不生效下单间隔和每s下单笔数，不为0的时候取两者最大值*/
	int OrderInterval = SPtr_CData->OrderInterval;
	if (0 == OrderInterval)
	{
		return 0;
	}
	int Frequency = SPtr_CData->Frequency;
	int Space = 1 * 1000 / Frequency;
	int max;
	if (Space > OrderInterval)
	{
		max = Space;
	}
	else
	{
		max = OrderInterval;
	}
	return (int)(1.5*max);
}

bool CTradeDlg::modifyEntrustAmount(string &s_dst, string s_src, string entrust_amount)
{
	Json::Reader reader;
	Json::Value root;

	//从字符串中读取数据
	if (reader.parse(s_src, root))
	{
		string s_str;
		root["entrust_amount"] = entrust_amount;
		s_dst = root.toStyledString();
	}
	else
	{
		string s_error = "parse fail:";
		s_error += s_src;
		ErrorLog(s_error);
		return false;
	}
	return true;
}

bool CTradeDlg::GainAlertInfo(const int ID_InsertStock, CCustomInfo &obj_CCustomInfo)
{
	GetDlgItem(ID_InsertStock)->GetWindowText(obj_CCustomInfo.stock_code);
	if (SPtr_CData->SecCode2MarketCode(obj_CCustomInfo.stock_code, obj_CCustomInfo.MarketCode) == false)
	{
		return false;
	}
	/*更新自选池、预警池、重绘自选池map、配置文件*/
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(obj_CCustomInfo.MarketCode, obj_CCustomInfo.stock_code, obj_CSecurity) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "GetSecurity fail";
		return false;
	}
	obj_CCustomInfo.stock_name = obj_CSecurity.stock_name;
	return true;
}

bool CTradeDlg::GainAlertInfo(CSimpleEdit* pEditCode, CCustomInfo &obj_CCustomInfo)
{
	if (!IsWindowValid(pEditCode))
	{
		return false;
	}
	pEditCode->GetWindowText(obj_CCustomInfo.stock_code);
	if (SPtr_CData->SecCode2MarketCode(obj_CCustomInfo.stock_code, obj_CCustomInfo.MarketCode) == false)
	{
		return false;
	}
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(obj_CCustomInfo.MarketCode, obj_CCustomInfo.stock_code, obj_CSecurity) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "GetSecurity fail";
		return false;
	}
	obj_CCustomInfo.stock_name = obj_CSecurity.stock_name;
	return true;
}

bool CTradeDlg::WriteAlertInfo(string s_Path, CCustomInfo &obj_CCustomInfo)
{
	Json::Reader reader;
	Json::Value root;
	string s_FileContent;
	tool::File2String(s_Path, s_FileContent);

	/*s_str为空的时候理论上也该是解析失败({}解析成功)，实际上也的确是解析失败*/
	if (reader.parse(s_FileContent, root, false))
	{
		Json::Value val;
		val["stock_code"] = Json::Value(obj_CCustomInfo.stock_code);
		val["stock_name"] = Json::Value(obj_CCustomInfo.stock_name);
		val["MarketCode"] = Json::Value(obj_CCustomInfo.MarketCode);
		root["CustomInfo"].append(Json::Value(val));
		Json::StyledWriter  sw;
		std::string res_str = sw.write(root);
		tool::string2File(s_Path, res_str);
	}
	else
	{
		if ("" == s_FileContent)
		{
			Json::Value val;
			val["stock_code"] = Json::Value(obj_CCustomInfo.stock_code);
			val["stock_name"] = Json::Value(obj_CCustomInfo.stock_name);
			val["MarketCode"] = Json::Value(obj_CCustomInfo.MarketCode);
			root["CustomInfo"].append(Json::Value(val));
			Json::StyledWriter  sw;
			std::string res_str = sw.write(root);
			tool::string2File(s_Path, res_str);
		}
		else
		{
			ExceptionInterrupt;
			LOG(ERROR) << "parse fail:" << s_FileContent;
			return false;
		}
	}
	return true;
}
bool CTradeDlg::IsSubscribed(CString stock_account, CString SecCode)
{
	_Security obj_MSecurity;
	if (SPtr_CData->GetMSecurity(SPtr_CData->fund_account, stock_account, obj_MSecurity) == false)
	{
		LOG(ERROR) << "GetMSecurity fail";
		ExceptionInterrupt;
		return true;
	}

	auto iter = obj_MSecurity.find(SecCode);
	if (iter == obj_MSecurity.end())
	{
		return false;
	}
	return true;
}

bool CTradeDlg::GuaranteedSubscription(bool bLock, CString MarketCode, CString SecCode, bool bEntrustQueue, bool bEntrustEach)
{
	UnSubscribe_All(MarketCode);
	Subscribed_SingleStock(bLock,MarketCode, SecCode, bEntrustQueue, bEntrustEach);
	return true;
}

void CTradeDlg::DealSubscribe(bool bLock, CString MarketCode, CString SecCode, bool bEntrustQueue, bool bEntrustEach)
{
	GuaranteedSubscription(bLock,MarketCode, SecCode, bEntrustQueue, bEntrustEach);
	CListCtrl_BEntrustQueue.SetItemCount(0);//清空原有数据
	CListCtrl_SEntrustQueue.SetItemCount(0);
	CListCtrl_EntrustEach.SetItemCount(0);
	SetClistColoumnText(CListCtrl_BEntrustQueue, 1, "");
	SetClistColoumnText(CListCtrl_SEntrustQueue, 1, "");
}

void CTradeDlg::DealSubscribe(bool bLock, CString MarketCode, CString SecCode, bool bEntrustQueue, bool bEntrustEach, CGridCtrlEx* pGridBuy1, CGridCtrlEx* pGridSell1, CGridCtrlEx* pGridOrderEach)
{
	if (IsWindowValid(pGridBuy1))
	{
		pGridBuy1->SetRowCount(1);
		m_strGearBuy1.Empty();
		m_strNumOrdersBuy1.Empty();
	}
	if (IsWindowValid(pGridSell1))
	{
		pGridSell1->SetRowCount(1);
		m_strGearSell1.Empty();
		m_strNumOrdersSell1.Empty();
	}
	if (IsWindowValid(pGridOrderEach))
	{
		pGridOrderEach->SetRowCount(1);
	}
	GuaranteedSubscription(bLock, MarketCode, SecCode, bEntrustQueue, bEntrustEach);
	
}

bool CTradeDlg::UnSubscribe_All(CString MarketCode)
{
	interface_struct_Subscribe sCancel1;
	sCancel1.Dyna_flag = false;//重置只重置除了快照以外的其他行情、系统中暂存两套机制快照行情，深度、委托队列、逐笔委托行情
	sCancel1.DepthOrder_flag = true;//true代表修改这个值，全置为true
	sCancel1.Depth_flag = true;
	sCancel1.EachDeal_flag = true;
	sCancel1.EachOrder_flag = true;
	socket_struct_SubscribeDetail CanList[MaxSize];
	strcpy(CanList[0].MarketCode, "*");
	CXcMarketApi * pAPI_Unsubscribe = NULL;
	switch (tool::find_enumMarketCode(string(MarketCode)))
	{
	case enumSSE:
		pAPI_Unsubscribe = pCApiFun->Api_Market_SSE;
		break;
	case enumSZSE:
		pAPI_Unsubscribe = pCApiFun->Api_Market_SZSE;;
		break;
	default:
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal MarketCode";
		break;
	}
	if (pAPI_Unsubscribe->Cancel(IDMarket_UnSubscribe, &sCancel1, CanList, 1) < 0)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "API_Cancel fail";
		return false;
	}
	return true;

}

bool CTradeDlg::DrawDepth(const CString &MarketCode, const CString &SecCode,
	const int grade, const int IDC_Price, const int IDC_Quantity)
{
	CString s_str;
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, grade, obj_CDepth) == false)
	{
	}
	s_str.Format(_T("%0.2f"), obj_CDepth.Price);
	GetDlgItem(IDC_Price)->SetWindowText(s_str);
	s_str.Format(_T("%u"), obj_CDepth.Quantity);
	GetDlgItem(IDC_Quantity)->SetWindowText(s_str);
	return true;
}

bool CTradeDlg::DrawDepth(const CString &MarketCode, const CString &SecCode,
	const int grade, CSimpleStatic* pPrice, CSimpleStatic* pQuantity)
{
	CString s_str;
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, grade, obj_CDepth) == false)
	{
	}
	s_str.Format(_T("%0.2f"), obj_CDepth.Price);
	if (IsWindowValid(pPrice))
	{
		pPrice->SetWindowText(s_str);
	}
	s_str.Format(_T("%u"), obj_CDepth.Quantity);
	if (IsWindowValid(pQuantity))
	{
		pQuantity->SetWindowText(s_str);
	}
	return true;
}

bool CTradeDlg::DrawDepth(const CString &MarketCode, const CString &SecCode,
	const int grade, CSimpleButton* pPrice, CSimpleStatic* pQuantity)
{
	CString s_str;
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, grade, obj_CDepth) == false)
	{
	}
	s_str.Format(_T("%0.2f"), obj_CDepth.Price);
	if (IsWindowValid(pPrice))
	{
		pPrice->SetWindowText(s_str);
	}
	s_str.Format(_T("%u"), obj_CDepth.Quantity);
	if (IsWindowValid(pQuantity))
	{
		pQuantity->SetWindowText(s_str);
	}
	return true;
}

void CTradeDlg::OnClickedBDirectlyOrder(const int ID_bDirectlyOrder, const string &NodePath)
{
	switch (bDirectlyOrder)
	{
	case true:
		bDirectlyOrder = false;
		((CButton*)GetDlgItem(ID_bDirectlyOrder))->SetCheck(BST_UNCHECKED);
		break;
	case false:
		((CButton*)GetDlgItem(ID_bDirectlyOrder))->SetCheck(BST_CHECKED);
		bDirectlyOrder = true;
		break;
	}

	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		CString s_tmp;
		if (true == bDirectlyOrder)
		{
			s_tmp = "1";
		}
		else
		{
			s_tmp = "0";
		}
		pt.put(NodePath, s_tmp);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return;
	}
}

void CTradeDlg::OnClickedBDirectlyOrder(CSimpleCheckbox* pRMouseOrder, const string &NodePath)
{
	if (!IsWindowValid(pRMouseOrder))
	{
		return;
	}
	bDirectlyOrder = pRMouseOrder->GetCheck();

	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		CString s_tmp;
		if (true == bDirectlyOrder)
		{
			s_tmp = "1";
		}
		else
		{
			s_tmp = "0";
		}
		pt.put(NodePath, s_tmp);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return;
	}
}

bool CTradeDlg::OnClickedbStockAssociated(const string &NodePath, const unsigned int &CtrID)
{
	switch (((CButton*)GetDlgItem(CtrID))->GetCheck())
	{
	case BST_CHECKED:
		bStockAssociated = true;
		break;
	case BST_UNCHECKED:
		bStockAssociated = false;
		break;
	default:
		break;
	}
	save_bStockAssociated(NodePath, CtrID);
	return true;
}

bool CTradeDlg::OnQRedrawFund(CListCtrl &CListCtrl_Funds,const int CtrlId)
{
	CFund obj_CFund;
	if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
	{
		LOG(ERROR) << "GetFund fail";
		ExceptionInterrupt;
	}
	int n = 1;//资金账户只有一列
	CListCtrl_Funds.Invalidate(false);
	CListCtrl_Funds.SetItemCount(n);
	/*同步更新可用资金 IDC_BEnableBalance*/
	CString s_str;
	s_str.Format("%0.2f", obj_CFund.enable_balance);
	GetDlgItem(CtrlId)->SetWindowText(s_str);
	return true;
}

void CTradeDlg::ClickedShowNoTrade(CSimpleCheckbox& chcShowNoTrade, CGridCtrlEx &gridCtrl)
{
	int state = chcShowNoTrade.GetCheck();
	switch (state)
	{
	case 0://未被选中
		bShowAll = true;
		OnRedrawOrder(&gridCtrl);
		break;
	case 1://复选框被选中
		bShowAll = false;
		OnRedrawOrder(&gridCtrl);
		break;
	default:
		break;
	}
}

void CTradeDlg::OnRedrawOrder(CGridCtrlEx* pGrid)
{
	if (!IsWindowValid(pGrid))
	{
		return;
	}
	/*每次重绘前，必须把上次保存的数据清空掉，再重新从tradedata中获取,由于mfc消息机制，同时只能执行一个因此不可能同时执行*/
	_Order MOrder_Redraw;
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Order obj_MOrder;
			_Order::iterator iter_MOrder;
			_Order::iterator iter_MOrder_Redraw;

			if (SPtr_CData->GetMOrder(SPtr_CData->fund_account, iter_MAccount->first, obj_MOrder) == false)
			{
				LOG(ERROR) << "GetMOrder fail";
				ExceptionInterrupt;
				continue;
			}
			if (obj_MOrder.find(0) != obj_MOrder.end())
			{
				LOG(ERROR) << "entrust_no is 0";
				ExceptionInterrupt;
			}
			if (obj_MOrder.size() > 0)
			{
				/*处理checkbox*/
				for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
				{
					/*查找order中的每一项的委托编号是不是在OrderCheckBox_map中，没有就insert进false的否则什么都不处理*/
					if (OrderCheckBox_map.find(iter_MOrder->first) == OrderCheckBox_map.end())
					{
						OrderCheckBox_map.insert(make_pair(iter_MOrder->first, false));
					}
				}

				/*为MOrder_Redraw赋值*/
				for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
				{
					if (bShowAll == false)
					{
						/*仅显示可撤*/
						if (iter_MOrder->second->entrust_status != WeiBao && iter_MOrder->second->entrust_status != YiBao
							&& iter_MOrder->second->entrust_status != BuCheng)
						{
							continue;
						}
					}

					/*如果MOrder_Redraw里没有则insert，否则采用赋值，这里map有个极大的操作误区，
					同时inset同一个key，但是不同value的键值，键值并不会替换*/
					iter_MOrder_Redraw = MOrder_Redraw.find(iter_MOrder->first);

					if (iter_MOrder_Redraw == MOrder_Redraw.end())
					{
						MOrder_Redraw.insert(make_pair(iter_MOrder->first, iter_MOrder->second));
					}
					else
					{
						iter_MOrder_Redraw->second = iter_MOrder->second;
					}

				}
			}
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
		return;
	}
	/*把map中的数据转移到Vector里*/
	VOrder_Redraw.clear();
	for (auto iter = MOrder_Redraw.rbegin(); iter != MOrder_Redraw.rend(); iter++)
	{
		VOrder_Redraw.push_back(iter->second);
	}

	pGrid->SetRowCount(VOrder_Redraw.size() + 1);
	if (pGrid->GetRowCount() <= 1)
	{
		return;
	}
	pGrid->MySetSelectedRangeEx(1, 0, 1, pGrid->GetColumnCount() - 1, TRUE);
	pGrid->Invalidate();
}

bool CTradeDlg::OnRedrawPosition(CGridCtrlEx* pGrid)
{
	if (!IsWindowValid(pGrid))
	{
		return false;
	}
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	VPosition_Redraw.clear();
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Position obj_MPosition;
			if (SPtr_CData->GetMPosition(SPtr_CData->fund_account, iter_MAccount->first, obj_MPosition) == false)
			{
				LOG(ERROR) << "GetMPosition fail";
				ExceptionInterrupt;
				continue;
			}

			for (auto iter = obj_MPosition.begin(); iter != obj_MPosition.end(); iter++)
			{
				VPosition_Redraw.push_back(iter->second);
			}
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
		return false;
	}
	pGrid->SetRowCount(VPosition_Redraw.size() + 1);
	pGrid->Invalidate();
	return true;
}

bool CTradeDlg::RedrawOrder(CListCtrl &CListCtrl_Order)
{
	/*每次重绘前，必须把上次保存的数据清空掉，再重新从tradedata中获取,由于mfc消息机制，同时只能执行一个因此不可能同时执行*/
	_Order MOrder_Redraw;
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Order obj_MOrder;
			_Order::iterator iter_MOrder;
			_Order::iterator iter_MOrder_Redraw;

			if (SPtr_CData->GetMOrder(SPtr_CData->fund_account, iter_MAccount->first, obj_MOrder) == false)
			{
				LOG(ERROR) << "GetMOrder fail";
				ExceptionInterrupt;
				continue;
			}
			if (obj_MOrder.find(0) != obj_MOrder.end())
			{
				LOG(ERROR) << "entrust_no is 0";
				ExceptionInterrupt;
			}
			if (obj_MOrder.size() > 0)
			{
				/*处理checkbox*/
				for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
				{
					/*查找order中的每一项的委托编号是不是在OrderCheckBox_map中，没有就insert进false的否则什么都不处理*/
					if (OrderCheckBox_map.find(iter_MOrder->first) == OrderCheckBox_map.end())
					{
						OrderCheckBox_map.insert(make_pair(iter_MOrder->first, false));
					}
				}

				/*为MOrder_Redraw赋值*/
				for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
				{
					if (bShowAll == false)
					{
						/*仅显示可撤*/
						if (iter_MOrder->second->entrust_status != WeiBao && iter_MOrder->second->entrust_status != YiBao
							&& iter_MOrder->second->entrust_status != BuCheng)
						{
							continue;
						}
					}

					/*如果MOrder_Redraw里没有则insert，否则采用赋值，这里map有个极大的操作误区，
					同时inset同一个key，但是不同value的键值，键值并不会替换*/
					iter_MOrder_Redraw = MOrder_Redraw.find(iter_MOrder->first);

					if (iter_MOrder_Redraw == MOrder_Redraw.end())
					{
						MOrder_Redraw.insert(make_pair(iter_MOrder->first, iter_MOrder->second));
					}
					else
					{
						iter_MOrder_Redraw->second = iter_MOrder->second;
					}

				}
			}
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
		return false;
	}
	/*把map中的数据转移到Vector里*/
	VOrder_Redraw.clear();
	for (auto iter = MOrder_Redraw.rbegin(); iter != MOrder_Redraw.rend(); iter++)
	{
		VOrder_Redraw.push_back(iter->second);
	}
	//CListCtrl_Order.Invalidate(false);
	CListCtrl_Order.SetItemCount(VOrder_Redraw.size());
	CListCtrl_Order.EnsureVisible(0,FALSE);
	return true;
}

void CTradeDlg::OnRedrawAlert(CGridCtrlEx* pAlertGrid)
{
	if (false == bRefreshAlert || !IsWindowValid(pAlertGrid))
	{
		return;
	}
	bool bJump = false;
	int JumpIndex = 0;
	int index_vec_Alert = 0;
	bool bFirstRedraw = false;
	if (SPtr_CData->M_PreviousRedrawList.size() == 0)
	{
		bFirstRedraw = true;
	}

	_MSpecialAlert MSpecialAlert;
	_MSpecialAlert::reverse_iterator iter;
	vec_Alert.clear();
	int nIndexHighlighted = -1;
	GetMAlert_Special(MSpecialAlert);
	for (iter = MSpecialAlert.rbegin(); iter != MSpecialAlert.rend(); iter++)
	{
		if (iter->second->SecCode == highlightedSecCode)
		{
			nIndexHighlighted = vec_Alert.size();
		}
		vec_Alert.push_back(iter->second);
		index_vec_Alert++;
		if (true == bAutoRoll)
		{
			if (false == bFirstRedraw)
			{
				/*出现了新跳出的股票*/
				if (SPtr_CData->VerifybJump(iter->second->SecCode) == true)
				{
					highlightedSecCode = iter->second->SecCode;
					bJump = true;
					JumpIndex = index_vec_Alert;
#ifdef _DEBUG
					static int count = 0;
					cerr << "jump count:" << count << " jump coude:" << highlightedSecCode << endl;
					count++;
#endif
					if (SPtr_CData->obj_CAlertConfig.Template_Stock == CustomizePool)
					{
						PlaySound(TEXT(s_path_AlertSound.c_str()), NULL, SND_FILENAME | SND_ASYNC);
					}
				}
			}
		}
	}
	
	if (true == bAutoRoll)
	{
		fillPreviousRedrawList(MSpecialAlert);
	}

	int nSelected = 0;
	if (true == bJump)
	{
		nSelected = JumpIndex;
	}
	else
	{
		if (-1 == nIndexHighlighted)
		{
			nSelected = -1;
		}
		else
		{
			nSelected = nIndexHighlighted + 1;
		}

	}
	pAlertGrid->SetRowCount(vec_Alert.size() + 1);
	int nColCount = pAlertGrid->GetColumnCount();
	if (-1 == nSelected && bAutoRoll)
	{
		pAlertGrid->MySetEndVScroll();
	}
	if (bAutoRoll || pAlertGrid->MyGetEnableSelectHighlight())
	{
		pAlertGrid->MySetSelectedRangeEx(nSelected, 0, nSelected, nColCount - 1, TRUE);
	}

	pAlertGrid->Invalidate(FALSE);
} 

////void CTradeDlg::OnRedrawAlert(CGridCtrlEx* pAlertGrid)
////{
////	if (false == bRefreshAlert || !IsWindowValid(pAlertGrid))
////	{
////		return;
////	}
////	bool bJump = false;
////	int JumpIndex = 0;
////	int index_vec_Alert = 0;
////	bool bFirstRedraw = false;
////	if (SPtr_CData->M_PreviousRedrawList.size() == 0)
////	{
////		bFirstRedraw = true;
////	}
////	_MSpecialAlert MSpecialAlert;
////	_MSpecialAlert::reverse_iterator iter;
////	vec_Alert.clear();
////	if (GetMAlert_Special(MSpecialAlert) == false)
////	{
////		return;
////	}
////	for (iter = MSpecialAlert.rbegin(); iter != MSpecialAlert.rend(); iter++)
////	{
////		vec_Alert.push_back(iter->second);
////		index_vec_Alert++;
////		if (true == bAutoRoll)
////		{
////			if (false == bFirstRedraw)
////			{
////				/*出现了新跳出的股票*/
////				if (SPtr_CData->VerifybJump(iter->second->SecCode) == true)
////				{
////					highlightedSecCode = iter->second->SecCode;
////					bJump = true;
////					JumpIndex = index_vec_Alert;
////#ifdef _DEBUG
////					static int count = 0;
////					cerr << "jump count:" << count << " jump coude:" << highlightedSecCode << endl;
////					count++;
////#endif
////					if (SPtr_CData->obj_CAlertConfig.Template_Stock == CustomizePool)
////					{
////						tool::MyPlaySound(s_path_AlertSound);
////					}
////				}
////			}
////		}
////	}
////	
////	if (true == bAutoRoll)
////	{
////		fillPreviousRedrawList(MSpecialAlert);
////	}
////
////	pAlertGrid->SetRowCount(vec_Alert.size() + 1);
////	if (true == bJump)
////	{
////		pAlertGrid->SetEndVScroll();
////	}
////	pAlertGrid->Invalidate(FALSE);
////}/*studying 完全可以改成这样，在setCell里设选择那行就好*/

bool CTradeDlg::GetMAlert_Special(_MSpecialAlert &MSpecialAlert)
{
	string warnningID;
	switch (SPtr_CData->obj_CAlertConfig.Template_Alert)
	{
	case enum_Increase:
		warnningID = warnningId_Increase;
		break;
	case enum_N_Increase:
		warnningID = warnningId_N_Increase;
		break;
	case enum_Low_Rise:
		warnningID = warnningId_Low_Rise;
		break;
	default:
		ErrorLog("abnormal Template_Alert");
		break;
	}
	if (SPtr_CData->GetMAlert_Special(warnningID, MSpecialAlert) == false)
	{
		//ErrorLog("GetMAlert_Special fail");/*存在没数据的时候要刷新预警池*/
		return false;
	}
	return true;
}

void CTradeDlg::fillPreviousRedrawList(_MSpecialAlert &MSpecialAlert)
{
	SPtr_CData->ClearPreviousRedrawList();
	for (auto iter = MSpecialAlert.rbegin(); iter != MSpecialAlert.rend(); iter++)
	{
		SPtr_CData->insertPreviousRedrawList(iter->second->SecCode);
	}
	return;
}

bool CTradeDlg::OnStnClickedBigCanBuyTotal(const int ID_BigCanBuyTotal, const int ID_BEntrustAmount)
{
	UInt_64 BigCanBuyTotal;
	GainLegalValue(BigCanBuyTotal, ID_BigCanBuyTotal);
	GetDlgItem(ID_BEntrustAmount)->SetWindowText(_T(convert<string>(BigCanBuyTotal).c_str()));
	return true;
}

void CTradeDlg::ClickedbStockAssociated(const string &NodePath, CSimpleCheckbox* pSimpleCheckbox)
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		CString s_tmp;
		if (IsWindowValid(pSimpleCheckbox))
		{
			if (pSimpleCheckbox->GetCheck())
			{
				s_tmp = "1";
			}
			else
			{
				s_tmp = "0";
			}
			pt.put(NodePath, s_tmp);
			xml_writer_settings<string> settings('\t', 1, "GB2312");
			write_xml(s_ConfigPath, pt, std::locale(), settings);
		}

	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return;
	}
}

void CTradeDlg::OnClickedShowNoTrade(const int ID_ShowNoTrade,CListCtrl &CListCtrl_Order)
{
	int state = ((CButton*)GetDlgItem(ID_ShowNoTrade))->GetCheck();
	switch (state)
	{
	case 0://未被选中
		bShowAll = true;
		RedrawOrder(CListCtrl_Order);
		break;
	case 1://复选框被选中
		bShowAll = false;
		RedrawOrder(CListCtrl_Order);
		break;
	default:
		break;
	}	
}

void CTradeDlg::OnRedrawEntrustQueue(VecEntrustQueue &VEntrustQueue, int Grade, CListCtrl &CListCtrl_EntrustQueue, unsigned int &Sum_Coloumn)
{
	int sum_entrust = 0;
	int item = 0;
	string Gear1;
	CString MarketCode;
	CString SecCode = SPtr_CData->TradingSecCode;
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		ErrorLog("SecCode2MarketCode fail");
		return;
	}
	/*更新委托总笔数*/
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, Grade, obj_CDepth) == false)
	{
		ErrorLog("GetDepth fail");
	}
	string NumOrders = convert<string>(obj_CDepth.NumOrders);
	NumOrders += " 笔";

	VEntrustQueue.clear();
	if (SPtr_CData->GetMEntrustQueue(MarketCode, SecCode, Grade, VEntrustQueue) == false)
	{
		ErrorLog("GetMEntrustQueue fail");
		return;
	}
	if (VEntrustQueue[0])
	{
		Gear1 = convert<string>(VEntrustQueue[0]->Price);
	}
	else
	{
		Gear1 = "";
	}
	
	for (auto iter : VEntrustQueue)
	{
		if (iter)
		{
			if (iter->Quantity != 0)
			{
				sum_entrust++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	Sum_Coloumn = tool::GetColoumnSum(CListCtrl_EntrustQueue);
	item = sum_entrust / Sum_Coloumn + 1;
	CListCtrl_EntrustQueue.SetItemCount(item);
	SetClistColoumnText(CListCtrl_EntrustQueue, 1, Gear1);
	SetClistColoumnText(CListCtrl_EntrustQueue, 2, NumOrders);
}

void CTradeDlg::OnRedrawEntrustQueue(VecEntrustQueue &VEntrustQueue, int Grade, CGridCtrlEx* pGridQueue, unsigned int &Sum_Coloumn)
{
	if (!IsWindowValid(pGridQueue))
	{
		return;
	}
	int sum_entrust = 0;
	int item = 0;
	string Gear1;
	CString MarketCode;
	CString SecCode = SPtr_CData->TradingSecCode;
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		ErrorLog("SecCode2MarketCode fail");
		return;
	}
	/*更新委托总笔数*/
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, Grade, obj_CDepth) == false)
	{
		ErrorLog("GetDepth fail");
	}
	string NumOrders = convert<string>(obj_CDepth.NumOrders);
	NumOrders += " 笔";

	VEntrustQueue.clear();
	if (SPtr_CData->GetMEntrustQueue(MarketCode, SecCode, Grade, VEntrustQueue) == false)
	{
		ErrorLog("GetMEntrustQueue fail");
		return;
	}
	if (VEntrustQueue[0])
	{
		Gear1 = convert<string>(VEntrustQueue[0]->Price);
	}
	else
	{
		Gear1 = "";
	}

	for (auto iter : VEntrustQueue)
	{
		if (iter)
		{
			if (iter->Quantity != 0)
			{
				sum_entrust++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	Sum_Coloumn = pGridQueue->GetColumnCount();
	item = sum_entrust / Sum_Coloumn + 1;
	
	CString strGear;
	strGear.Format("%s", Gear1.c_str());
	CString strNumOrders;
	strNumOrders.Format("%s", NumOrders.c_str());
	if (-1 == Grade)
	{
		m_strGearBuy1 = strGear;
		m_strNumOrdersBuy1 = strNumOrders;
	}
	else if (1 == Grade)
	{
		m_strGearSell1 = strGear;
		m_strNumOrdersSell1 = strNumOrders;
	}
	pGridQueue->SetRowCount(item + 1);
	pGridQueue->Invalidate();
}

void CTradeDlg::OnGetdispinfoEntrustQueue(LV_ITEM* pItem, unsigned int Sum_Coloumn, VecEntrustQueue &VEntrustQueue)
{
	int iItemIndx = pItem->iItem;
	CString s_str;
	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
		case 0:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 1:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 2:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 3:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 4:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 5:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 6:
		{
			s_str = GetQuantity(iItemIndx, pItem->iSubItem, Sum_Coloumn, VEntrustQueue);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		default:
			break;
		}
	}
}

void CTradeDlg::OnCustomDrawEntrustQueue(LPNMTVCUSTOMDRAW &pNMCD, string &s_str)
{
	UInt_64 Quantity = convert<UInt_64>(s_str);
	if (Quantity > Threshold_Discoloration)
	{
		pNMCD->clrText = RGB(255, 0, 0);//红色		
	}
	else
	{
		pNMCD->clrText = RGB(51, 153, 255);
	}
}

void CTradeDlg::OnRedrawEntrustEach(VecEntrustEach &VEntrustEach)
{
	CString MarketCode;
	CString SecCode = SPtr_CData->TradingSecCode;
	SPtr_CData->SecCode2MarketCode(SecCode, MarketCode);
	VEntrustEach.clear();
	if (SPtr_CData->GetVEntrustEach(MarketCode, SecCode, VEntrustEach, RedrawSum_EntrustEach) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "GetVEntrustEach fail";
		return;
	}
	CListCtrl_EntrustEach.SetItemCount(VEntrustEach.size());
}

void CTradeDlg::OnRedrawEntrustEach(CGridCtrlEx* pOrderEach)
{
	if (!IsWindowValid(pOrderEach))
	{
		return;
	}
	CString MarketCode;
	CString SecCode = SPtr_CData->TradingSecCode;
	SPtr_CData->SecCode2MarketCode(SecCode, MarketCode);
	VEntrustEach.clear();
	if (SPtr_CData->GetVEntrustEach(MarketCode, SecCode, VEntrustEach, RedrawSum_EntrustEach) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "GetVEntrustEach fail";
		return;
	}
// 	CListCtrl_EntrustEach.SetItemCount(VEntrustEach.size());
	pOrderEach->SetRowCount(VEntrustEach.size() + 1);
	pOrderEach->Invalidate();
}

void CTradeDlg::OnGetdispinfoEntrustEach(LV_ITEM* pItem, VecEntrustEach &VEntrustEach)
{
	int iItemIndx = pItem->iItem;
	CString s_str;
	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
		case 0://时间
		{
			s_str = convert<string>(VEntrustEach[iItemIndx]->Time).c_str();
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 1://委托号
		{
			s_str = convert<string>(VEntrustEach[iItemIndx]->OrderNo).c_str();
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 2://价格
		{
			s_str.Format("%0.2f", VEntrustEach[iItemIndx]->Price);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 3://数量
		{
			s_str = convert<string>(VEntrustEach[iItemIndx]->Volume).c_str();
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 4://金额
		{
			s_str.Format("%0.2f", VEntrustEach[iItemIndx]->Amount);
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		case 5://买卖方向
		{
			switch (VEntrustEach[iItemIndx]->Side)
			{
			case '1':
				s_str = "B";
				break;
			case '2':
				s_str = "S";
				break;
			default:
				ExceptionInterrupt;
				LOG(ERROR) << "abnoraml Side";
				break;
			}
			lstrcpyn(pItem->pszText, s_str, pItem->cchTextMax);
			break;
		}
		default:break;
		}
	}
}

void CTradeDlg::OnCustomDrawEntrustEach(LPNMTVCUSTOMDRAW &pNMCD, string &s_str)
{
	switch (tool::find_enumSide_EntrustEach(s_str))
	{
	case enumBuy:
		pNMCD->clrText = RGB(255, 0, 0);//红色
		break;
	case enumSell:
		pNMCD->clrText = RGB(51, 153, 255);//蓝色
		break;
	case enumBC:
		break;
	case enumSC:
		break;
	default:
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal Side";
		break;
	}
}


bool CTradeDlg::InspectbRedraw(const CString &stock_code, unsigned int CtrID)
{
	CString s_str;
	GetDlgItem(CtrID)->GetWindowText(s_str);
	if (s_str == stock_code)
	{
		return false;
	}
	return true;
}

bool CTradeDlg::VerifybUpdateMoudle(const int ID_SecCode)
{
	CString s_str;
	(GetDlgItem(ID_SecCode))->GetWindowText(s_str);
	if (s_str == SPtr_CData->TradingSecCode)
	{
		return true;
	}
	return false;
}

bool CTradeDlg::InitStockAssociated(const string &treePath)
{
	bool bResult = false;
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		bResult = pt.get<bool>(treePath);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	return bResult;
}

bool CTradeDlg::VerifybUpdateMoudle(CSimpleEdit* pEdit)
{
	CString s_str;
	if (IsWindowValid(pEdit))
	{
		pEdit->GetWindowText(s_str);
	}
	if (s_str == SPtr_CData->TradingSecCode)
	{
		return true;
	}
	return false;
}

bool CTradeDlg::VerifyMarketStatus()
{
	_Market::iterator iter_MMarket;
	bool bMarketServerStatus = false;
	for (iter_MMarket = SPtr_CData->MMarket.begin(); iter_MMarket != SPtr_CData->MMarket.end(); )
	{
		if (iter_MMarket->second->MSecurity.size() <= 0)
		{
			bMarketServerStatus = false;
			break;
		}
		iter_MMarket++;
		bMarketServerStatus = true;
	}
	return bMarketServerStatus;
}

bool CTradeDlg::getDepthPrice(const CString &MarketCode, const CString &SecCode, const int grade, double &EntrustPrice)
{
	CDepth obj_CDepth;
	if (SPtr_CData->GetDepth(MarketCode, SecCode, grade, obj_CDepth) == false)
	{
		EntrustPrice = 0;
		return false;
	}
	else
	{
		EntrustPrice = obj_CDepth.Price;
	}

	return true;
}

bool CTradeDlg::getDefalutPrice(const CString &DefalutGear, const CString &MarketCode, const CString &SecCode, double &EntrustPrice)
{
	switch (tool::find_enumPriceType(string(DefalutGear)))
	{
	case enum涨停价:
	{
		CSecurity obj_CSecurity;
		if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
		{
			ErrorLog("GetSecurity fail");
			EntrustPrice = 0;
			return false;
		}
		else
		{
			EntrustPrice = obj_CSecurity.UpLimitPrice;
		}

		break;
	}
	case enum跌停价:
	{
		CSecurity obj_CSecurity;
		if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
		{
			LOG(ERROR) << "GetSecurity fail";
			ExceptionInterrupt;
			EntrustPrice = 0;
			return false;
		}
		else
		{
			EntrustPrice = obj_CSecurity.DownLimitPrice;
		}
		break;
	}
	case enum买一价:
	{
		getDepthPrice(MarketCode, SecCode, -1, EntrustPrice);
		break;
	}
	case enum买二价:
	{
		getDepthPrice(MarketCode, SecCode, -2, EntrustPrice);
		break;
	}
	case enum买三价:
	{
		getDepthPrice(MarketCode, SecCode, -3, EntrustPrice);
		break;
	}
	case enum买四价:
	{
		getDepthPrice(MarketCode, SecCode, -4, EntrustPrice);
		break;
	}
	case enum买五价:
	{
		getDepthPrice(MarketCode, SecCode, -5, EntrustPrice);
		break;
	}
	case enum卖一价:
	{
		getDepthPrice(MarketCode, SecCode, 1, EntrustPrice);
		break;
	}
	case enum卖二价:
	{
		getDepthPrice(MarketCode, SecCode, 2, EntrustPrice);
		break;
	}
	case enum卖三价:
	{
		getDepthPrice(MarketCode, SecCode, 3, EntrustPrice);
		break;
	}
	case enum卖四价:
	{
		getDepthPrice(MarketCode, SecCode, 4, EntrustPrice);
		break;
	}
	case enum卖五价:
	{
		getDepthPrice(MarketCode, SecCode, 5, EntrustPrice);
		break;
	}
	default:
		ErrorLog("abnoraml DefaultGear");
		break;
	}
	return true;
}

bool CTradeDlg::GainCSecurity(CSecurity &obj_CSecurity, const CString stock_code)
{
	if ("" == stock_code)
	{
		MessageBox("股票代码为空", "提示", MB_OK);
		return false;
	}
	else
	{
		CString MarketCode;
		if (SPtr_CData->SecCode2MarketCode(stock_code, MarketCode) == false)
		{
			LOG(ERROR) << "SecCode2MarketCode fail";
			ExceptionInterrupt;
			return false;
		}
		if (SPtr_CData->GetSecurity(MarketCode, stock_code, obj_CSecurity) == false)
		{
			LOG(ERROR) << "GetSecurity fail";
			ExceptionInterrupt;
			return false;
		}
	}
	return true;
}

bool CTradeDlg::GaineEable_amount(const CString &SecCode, UInt_64 &enable_amount)
{
	/*获取持仓里的可用数量*/
	CString StockAccount;
	if (SPtr_CData->SecCode2StockAccount(SecCode, StockAccount) == false)
	{
		enable_amount = 0;
		return false;
	}

	CPosition obj_CPosition;
	if (SPtr_CData->GetPosition(SPtr_CData->fund_account, StockAccount, SecCode, obj_CPosition) == false)
	{
		enable_amount = 0;
		return true;
	}
	else
	{
		enable_amount = obj_CPosition.enable_amount;
	}
	return true;
}

bool CTradeDlg::GainEntrustInfoByEntrustNo(const UInt_64 entrust_no, enumEntrustStatus &entrust_status, string &exchange_type)
{
	bool bFind = false;
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Order obj_MOrder;
			if (SPtr_CData->GetMOrder(SPtr_CData->fund_account, iter_MAccount->first, obj_MOrder) == false)
			{
				LOG(ERROR) << "GetMOrder fail";
				ExceptionInterrupt;
				continue;
			}
			auto iter = obj_MOrder.find(entrust_no);
			if (iter != obj_MOrder.end())
			{
				entrust_status = iter->second->entrust_status;
				SPtr_CData->Account2ExchangeType(iter_MAccount->first, exchange_type);
				bFind = true;
			}
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
		return false;
	}
	return bFind;
}

double CTradeDlg::GainPoundage(double EntrustBalance)
{
	if (EntrustBalance <= 0)
		return 0;
	double Poundage;
	Poundage = EntrustBalance*(Ratio_poundage);
	if (Poundage < 5)
	{
		Poundage = 5;
	}
	return Poundage;
}

double CTradeDlg::GainBalance_MaxEnable(bool bFinancingBuy)
{
	double balance_MaxEnable;
	if (bFinancingBuy == true)//融资买入
	{
		VecCreditAsset VCreditAsset;
		if (SPtr_CData->GetVCreditAsset(SPtr_CData->fund_account, VCreditAsset) == false)
		{
			ErrorLog("GetVCreditAsset fail");
			balance_MaxEnable = 0;
		}
		else
		{
			VCreditAsset[0][1]; //per_assurescale_value 维持担保比率 0<=x<1.5可以购买 -1 代表没有负债
			VCreditAsset[0][3];//融资可用额度 fin_enable_quota
			VCreditAsset[1][1];//"可用保证金"; enable_bail_balance 
			double per_assurescale_value = convert<double>(VCreditAsset[0][1]);
			if (0 <= per_assurescale_value &&  per_assurescale_value < 1.5)
			{
				balance_MaxEnable = 0;
			}
			else
			{
				double fin_enable_quota = convert<double>(VCreditAsset[0][3]);
				double enable_bail_balance = convert<double>(VCreditAsset[1][1]);
				BOOST_AUTO(x, boost::minmax(fin_enable_quota, enable_bail_balance));
				balance_MaxEnable = x.get<0>();
			}
		}
	}
	else
	{
		CFund obj_CFund;
		if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
		{
			ErrorLog("GetFund fail");
			balance_MaxEnable = 0;
		}
		else
			balance_MaxEnable = obj_CFund.enable_balance;
	}

	if (balance_MaxEnable < 0)
	{
		balance_MaxEnable = 0;
	}
	return balance_MaxEnable;
}

UInt_64 CTradeDlg::GainBigCanBuy(bool bFinancingBuy, const double &entrust_price)
{
	double enable_balance = GainBalance_MaxEnable(bFinancingBuy);
	enable_balance = enable_balance - GainPoundage(enable_balance);
	return modifyNum(enable_balance / entrust_price);
}

UInt_64 CTradeDlg::GainCanBuy(bool bFinancingBuy, double DefaultBalance, const double &entrust_price)
{
	double balance_MaxEnable = GainBalance_MaxEnable(bFinancingBuy);
	BOOST_AUTO(x, boost::minmax(balance_MaxEnable, DefaultBalance));
	DefaultBalance = x.get<0>();
	DefaultBalance = DefaultBalance - GainPoundage(DefaultBalance);
	return modifyNum(DefaultBalance / entrust_price);
}

bool CTradeDlg::GainSetModuleInfo(const int CtrlId, CString &MarketCode, CString &SecCode)
{
	GetDlgItem(CtrlId)->GetWindowText(SecCode);
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return false;
	}
	return true;
}

bool CTradeDlg::GainSetModuleInfo(CSimpleEdit* pCode, CString &MarketCode, CString &SecCode)
{
	if (IsWindowValid(pCode))
	{
		pCode->GetWindowText(SecCode);
	}
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return false;
	}
	return true;
}

void CTradeDlg::GainOrderInfo(CString &s_dst, CString entrust_bs, int ID_SecCode, int ID_SecName, int ID_EntrustPrice, int ID_EntrustAmount)
{
	s_dst = "               买卖方向:";
	s_dst += entrust_bs;
	s_dst += "\n";
	FillElement(s_dst, "               股票代码:", ID_SecCode);
	FillElement(s_dst, "               股票名称:", ID_SecName);
	FillElement(s_dst, "               委托价格:", ID_EntrustPrice);
	FillElement(s_dst, "               委托数量:", ID_EntrustAmount);
	s_dst += "               您确认发出委托吗?";
}

void CTradeDlg::GainOrderInfo(CString &s_dst, CString entrust_bs, CSimpleEdit* pCode, CSimpleStatic* pName, CSimpleEdit* pEntrustPrice, CSimpleEdit* pEntrustAmount)
{
	s_dst = "               买卖方向:";
	s_dst += entrust_bs;
	s_dst += "\n";
	FillElement(s_dst, "               股票代码:", pCode);
	FillElement(s_dst, "               股票名称:", pName);
	FillElement(s_dst, "               委托价格:", pEntrustPrice);
	FillElement(s_dst, "               委托数量:", pEntrustAmount);
	s_dst += "               您确认发出委托吗?";
}

void CTradeDlg::FillElement(CString &s_dst, CString s_elementName, int CtrlID)
{
	CString s_str;
	GetDlgItem(CtrlID)->GetWindowText(s_str);
	s_dst += s_elementName;
	s_dst += s_str;
	s_dst += "\n";
}

void CTradeDlg::FillElement(CString &s_dst, CString s_elementName, CSimpleEdit* pEdit)
{
	if (IsWindowValid(pEdit))
	{
		CString s_str;
		pEdit->GetWindowText(s_str);
		s_dst += s_elementName;
		s_dst += s_str;
		s_dst += "\n";
	}
}

void CTradeDlg::FillElement(CString &s_dst, CString s_elementName, CSimpleStatic* pStatic)
{
	if (IsWindowValid(pStatic))
	{
		CString s_str;
		pStatic->GetWindowText(s_str);
		s_dst += s_elementName;
		s_dst += s_str;
		s_dst += "\n";
	}
}

void CTradeDlg::GainOrderInfo(CString &s_dst, CString entrust_bs, CString stock_code, const double &entrust_price, UInt_64 entrust_amount)
{
	s_dst = "               买卖方向:";
	s_dst += entrust_bs;
	s_dst += "\n";

	s_dst += "               股票代码:";
	s_dst += stock_code;
	s_dst += "\n";

	CString stock_name;
	if (SPtr_CData->SecCode2SecName(stock_code, stock_name) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "SecCode2SecName fail";
		return;
	}
	s_dst += "               股票名称:";
	s_dst += stock_name;
	s_dst += "\n";

	s_dst += "               委托价格:";
	s_dst += convert<string>(entrust_price).c_str();
	s_dst += "\n";

	s_dst += "               委托数量:";
	s_dst += convert<string>(entrust_amount).c_str();
	s_dst += "\n";
	s_dst += "               您确认发出委托吗?";
}


bool CTradeDlg::gainBEntrustAmount_Directly(bool bFinancingBuy, const double &entrust_price, UInt_64 &entrust_amount, bool bCalculateByBalance,
	double DefaultBalance, int DefaultAmount)
{
	if (true == bCalculateByBalance)
	{
		/*根据默认资金计算*/
		entrust_amount = GainCanBuy(bFinancingBuy,DefaultBalance,entrust_price);
	}
	else
	{
		/*首先获取最大可买，在比较最大可买和默认股数*/
		UInt_64 BigCanBuyTotal = GainBigCanBuy(bFinancingBuy, entrust_price);
		BOOST_AUTO(y, boost::minmax(BigCanBuyTotal, (UInt_64)DefaultAmount));
		entrust_amount = y.get<0>(); 
	}
	return true;
}

CString CTradeDlg::GetQuantity(const int &iItem, const int &iSubItem, const int &Sum_Coloumn, VecEntrustQueue &VEntrustQueue)
{
	UInt_64 Quantity;
	CString s_str;
	int index = iItem * Sum_Coloumn + iSubItem;
	if (VEntrustQueue[index])
	{
		Quantity = VEntrustQueue[index]->Quantity;
		if (Quantity != 0)
		{
			s_str = convert<string>(Quantity).c_str();
		}
		else
		{
			s_str = "0";
		}
	}
	else
	{
		s_str = "";
	}
	return s_str;
}

bool CTradeDlg::FillVec_Dismantling(vector<string> &Vec_Dismantling, UInt_64 entrust_amount,bool bDismantling, bool bRandom, int BaseQuantity
,UInt_32 Order_Minimum,UInt_32 Order_Maximum)
{
	if (true == bDismantling && entrust_amount > BaseQuantity*Unit_BaseQuantiry)
	{
		if (FillByDismantling(Vec_Dismantling, entrust_amount, bRandom,BaseQuantity
			,Order_Minimum, Order_Maximum) == false)
		{
			return false;
		}
	}
	else
	{
		Vec_Dismantling.push_back(convert<string>(entrust_amount));
	}
	return true;
}

bool CTradeDlg::FillByDismantling(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, bool bRandom, int BaseQuantity
	, UInt_32 Order_Minimum, UInt_32 Order_Maximum)
{
	if (true == bRandom)
	{
		return FillByRandom(Vec_Dismantling, entrust_amount, Order_Minimum, Order_Maximum);
	}
	else
	{
		return FillByAverage(Vec_Dismantling, entrust_amount, BaseQuantity);
	}
}

bool CTradeDlg::FillByRandom(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, UInt_32 Order_Minimum, UInt_32 Order_Maximum)
{
	int sum = 0;
	int i = 0;
	for (i = 0;i < Toplimit_Dismantling;i++)
	{
		if (Order_Minimum > Order_Maximum)
		{
			MessageBox("单笔上限小于单笔下限，拆单失败，请重设下单参数", "错误", NULL);
			return false;
		}
		UInt_64 Each_EntrustAmount = random(Order_Minimum, Order_Maximum)*100;
		sum += Each_EntrustAmount;
		if (sum >= entrust_amount)
		{
			Each_EntrustAmount -= (sum - entrust_amount);
			if (Each_EntrustAmount < Order_Minimum*100)
			{
				int tmp = convert<UInt_64>(Vec_Dismantling[i - 1]) + Each_EntrustAmount;
				Vec_Dismantling[i - 1] = convert<string>(tmp);
			}
			else
			{
				Vec_Dismantling.push_back(convert<string>(Each_EntrustAmount));
			}
			break;
		}
		Vec_Dismantling.push_back(convert<string>(Each_EntrustAmount));
	}
	if (sum < entrust_amount)
	{
		string  s_str = "您委托的分仓次数已超过系统允许的最大分仓次数(";
		s_str += convert<string>(Toplimit_Dismantling);
		s_str += ")";
		MessageBox(s_str.c_str(), "提示", MB_OK);
		return false;
	}
	return true;
}

bool CTradeDlg::FillByAverage(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, int BaseQuantity)
{
	BaseQuantity = BaseQuantity * Unit_BaseQuantiry;
	int Sum_Entrust = entrust_amount / BaseQuantity;
	for (int i = 0;i < Sum_Entrust; i++)
	{
		Vec_Dismantling.push_back(convert<string>(BaseQuantity));
	}
	int remainder = entrust_amount%BaseQuantity;
	if (remainder != 0)
	{
		Vec_Dismantling.push_back(convert<string>(remainder));
		Sum_Entrust++;
	}
	if (Sum_Entrust >= Toplimit_Dismantling)
	{
		string  s_str = "您委托的分仓次数已超过系统允许的最大分仓次数(";
		s_str += convert<string>(Toplimit_Dismantling);
		s_str += ")";
		MessageBox(s_str.c_str(), "提示", MB_OK);
		Vec_Dismantling.clear();
		return false;
	}
	return true;
}

bool CTradeDlg::VerifyBEntrustPrice(const int ID_BSecName, const int Id_BSecCode, const int Id_BEntrustPrice)
{
	/*CString SecName;
	GetDlgItem(ID_BSecName)->GetWindowText(SecName);
	if (SecName[0] == 'N')
	{
		return true;
	}*/

	double UpLimitPrice;
	CString stock_code;
	GetDlgItem(Id_BSecCode)->GetWindowText(stock_code);
	CSecurity obj_CSecurity;
	if (GainCSecurity(obj_CSecurity, stock_code) == false)
	{
		ErrorLog("GainCSecurity fail");
		return false;
	}
	UpLimitPrice = obj_CSecurity.UpLimitPrice;
	/*委托价格大于涨停价非法*/
	double EntrustPrice = 0;
	if (GainLegalValue(EntrustPrice, Id_BEntrustPrice) == false)
	{
		MessageBox("委托价格异常", "提示", MB_OK);
		return false;
	}
	if (tool::round(EntrustPrice, 2) > tool::round(UpLimitPrice, 2))
	{
		if (MessageBox("委托价格大于涨停价,下单可能失败", "提示", MB_YESNO) == IDYES)
		{
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
	return true;
}

bool CTradeDlg::VerifyBEntrustPrice(CSimpleStatic* pBSecName, CSimpleEdit* pBSecCode, CSimpleEdit* pBEntrustPrice)
{
	double UpLimitPrice;
	CString stock_code;
	if (pBSecCode != nullptr && IsWindow(pBSecCode->GetSafeHwnd()))
	{
		pBSecCode->GetWindowText(stock_code);
	}
	CSecurity obj_CSecurity;
	if (GainCSecurity(obj_CSecurity, stock_code) == false)
	{
		ErrorLog("GainCSecurity fail");
		return false;
	}
	UpLimitPrice = obj_CSecurity.UpLimitPrice;
	/*委托价格大于涨停价非法*/
	double EntrustPrice = 0;
	if (GainLegalValue(EntrustPrice, pBEntrustPrice) == false)
	{
		MessageBox("委托价格异常", "提示", MB_OK);
		return false;
	}
	if (tool::round(EntrustPrice, 2) > tool::round(UpLimitPrice, 2))
	{
		if (MessageBox("委托价格大于涨停价,下单可能失败", "提示", MB_YESNO) == IDYES)
		{
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
	return true;
}

bool CTradeDlg::VerifySEntrustPrice(const int ID_SSecName, const int Id_SSecCode, const int Id_SEntrustPrice)
{
	CString stock_code;
	GetDlgItem(Id_SSecCode)->GetWindowText(stock_code);
	
	CSecurity obj_CSecurity;
	if (GainCSecurity(obj_CSecurity, stock_code) == false)
	{
		ErrorLog("GainCSecurity fail");
		return false;
	}

	/*委托价格小于跌停板价非法*/
	double EntrustPrice = 0;
	if (GainLegalValue(EntrustPrice, Id_SEntrustPrice) == false)
	{
		MessageBox("委托价格非法", "提示", MB_OK);
		return false;
	}

	if (tool::round(EntrustPrice, 2) < tool::round(obj_CSecurity.DownLimitPrice, 2))
	{
		if (MessageBox("委托价格小于跌停价,下单可能失败", "提示", MB_YESNO) == IDYES)
		{
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
	return true;
}

bool CTradeDlg::VerifySEntrustPrice(CSimpleStatic* pSSecName, CSimpleEdit* pSSecCode, CSimpleEdit* pSEntrustPrice)
{
	CString stock_code;
	if (IsWindowValid(pSSecCode))
	{
		pSSecCode->GetWindowText(stock_code);
	}

	CSecurity obj_CSecurity;
	if (GainCSecurity(obj_CSecurity, stock_code) == false)
	{
		ErrorLog("GainCSecurity fail");
		return false;
	}

	/*委托价格小于跌停板价非法*/
	double EntrustPrice = 0;
	if (GainLegalValue(EntrustPrice, pSEntrustPrice) == false)
	{
		MessageBox("委托价格非法", "提示", MB_OK);
		return false;
	}

	if (tool::round(EntrustPrice, 2) < tool::round(obj_CSecurity.DownLimitPrice, 2))
	{
		if (MessageBox("委托价格小于跌停价,下单可能失败", "提示", MB_YESNO) == IDYES)
		{
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
	return true;
}

bool CTradeDlg::VerifyBEntrustAmount(bool bDismantling, CSimpleStatic* pBigCanBuyTotal, CSimpleEdit* pBEntrustAmount)
{
	/*委托数量大于100 0000或者可买数量非法*/
	UInt_64 EntrustAmount = 0;
	UInt_64 BigCanBuyTotal = 0;
	if (GainLegalValue(BigCanBuyTotal, pBigCanBuyTotal) == false)
	{
		MessageBox("可买股数异常", "提示", MB_OK);
		return false;
	}
	if (GainLegalValue(EntrustAmount, pBEntrustAmount) == false)
	{
		MessageBox("委托数量异常", "提示", MB_OK);
		return false;
	}

	if (EntrustAmount > BigCanBuyTotal)
	{
		MessageBox("委托数量非法", "提示", MB_OK);
		return false;
	}

	if (!VerifyEntrustAmount(true, pBEntrustAmount))
	{
		return false;
	}

	if (false == bDismantling)
	{
		if (EntrustAmount > Max_EntrustAmount)
		{
			MessageBox("委托数量非法", "提示", MB_OK);
			return false;
		}
	}
	return true;
}

bool CTradeDlg::VerifyEntrustAmount(bool bBuyModule, CSimpleEdit* pEntrustAmount)
{
	CString s_str;
	if (!IsWindowValid(pEntrustAmount))
	{
		return false;
	}
	pEntrustAmount->GetWindowText(s_str);
	if ("" == s_str || "0" == s_str)
	{
		MessageBox("输入的委托数量非法，请重新输入", "提示", MB_OK);
		return false;
	}
	else
	{
		if (true == bBuyModule)
		{
			if (convert<unsigned int>(s_str) % 100 != 0)
			{
				MessageBox("输入的委托数量非法，请重新输入", "提示", MB_OK);
				return false;
			}
		}
		/*卖模块直接不校验*/
	}
	return true;
}

bool CTradeDlg::VerifySEntrustAmount(bool bDismantling, CSimpleStatic* pSCanSellTotal, CSimpleEdit* pSEntrustAmount)
{
	/*获取可卖股数*/
	double CanSellTotal = 0;
	if (GainLegalValue(CanSellTotal, pSCanSellTotal) == false)
	{
		MessageBox("可卖股数非法", "提示", MB_OK);
		return false;
	}

	/*委托数量大于可用数量非法*/
	UInt_64 EntrustAmount = 0;
	if (GainLegalValue(EntrustAmount, pSEntrustAmount) == false)
	{
		MessageBox("委托数量异常", "提示", MB_OK);
		return false;
	}

	if (EntrustAmount > CanSellTotal)
	{
		MessageBox("委托数量非法", "提示", MB_OK);
		return false;
	}

	if (false == bDismantling)
	{
		if (EntrustAmount > Max_EntrustAmount)
		{
			MessageBox("委托数量非法", "提示", MB_OK);
			return false;
		}
	}

	return true;
}

bool CTradeDlg::VerifySEntrustAmount(bool bDismantling, const int Id_SCanSellTotal, const int Id_SEntrustAmount)
{
	/*获取可卖股数*/
	double CanSellTotal = 0;
	if (GainLegalValue(CanSellTotal, Id_SCanSellTotal) == false)
	{
		MessageBox("可卖股数非法", "提示", MB_OK);
		return false;
	}

	/*委托数量大于可用数量非法*/
	UInt_64 EntrustAmount = 0;
	if (GainLegalValue(EntrustAmount, Id_SEntrustAmount) == false)
	{
		MessageBox("委托数量异常", "提示", MB_OK);
		return false;
	}

	if (EntrustAmount > CanSellTotal)
	{
		MessageBox("委托数量非法", "提示", MB_OK);
		return false;
	}

	if (false == bDismantling)
	{
		if (EntrustAmount > Max_EntrustAmount)
		{
			MessageBox("委托数量非法", "提示", MB_OK);
			return false;
		}
	}

	return true;
}

bool CTradeDlg::VerifyBEntrustBalance(bool bFinancingBuy, const int Id_BEnableBalance, const int Id_BEntrustBalance)
{
	/*获取可用资金*/
	double EnableBalance = GainBalance_MaxEnable(bFinancingBuy);
	/*委托金额大于可用资金非法*/
	double EntrustBalance = 0;
	if (GainLegalValue(EntrustBalance, Id_BEntrustBalance) == false)
	{
		MessageBox("委托金额异常", "提示", MB_OK);
		return false;
	}

	if (EntrustBalance > EnableBalance)
	{
		MessageBox("委托非法：委托金额大于可用资金", "提示", MB_OK);
		return false;
	}
	return true;
}

bool CTradeDlg::VerifyBEntrustBalance(bool bFinancingBuy, CSimpleStatic* pBEnableBalance, CSimpleStatic* pBEntrustBalance)
{
	/*获取可用资金*/
	double EnableBalance = GainBalance_MaxEnable(bFinancingBuy);
	/*委托金额大于可用资金非法*/
	double EntrustBalance = 0;
	if (GainLegalValue(EntrustBalance, pBEntrustBalance) == false)
	{
		MessageBox("委托金额异常", "提示", MB_OK);
		return false;
	}

	if (EntrustBalance > EnableBalance)
	{
		MessageBox("委托非法：委托金额大于可用资金", "提示", MB_OK);
		return false;
	}
	return true;
}

bool CTradeDlg::VerifybEmpty_SecCode(const int ID_SecCode)
{
	CString stock_code;
	GetDlgItem(ID_SecCode)->GetWindowText(stock_code);
	if ("" == stock_code)
	{
		return true;
	}
	return false;
}

bool CTradeDlg::VerifybEmpty_SecCode(CSimpleEdit* pSecCode)
{
	CString stock_code;
	if (IsWindowValid(pSecCode))
	{
		pSecCode->GetWindowText(stock_code);
	}

	if ("" == stock_code)
	{
		return true;
	}
	return false;
}

bool CTradeDlg::VerifyBuyEntrustment_Directly(bool bDismantling, const double &entrust_price, const UInt_64 &entrust_amount)
{
	if (entrust_price <= 0)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "委托价格不合法";
		MessageBox("委托价格不合法", "委托错误", NULL);
		return false;
	}

	if (entrust_amount <= 0)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "委托数量不合法";
		MessageBox("委托数量不合法", "委托错误", NULL);
		return false;
	}

	if (false == bDismantling)
	{
		if (entrust_amount > Max_EntrustAmount)
		{
			ExceptionInterrupt;
			LOG(ERROR) << "委托数量不合法";
			MessageBox("委托数量不合法", "委托错误", NULL);
			return false;
		}
	}

	//CFund obj_CFund;
	//if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
	//{
	//	ExceptionInterrupt;
	//	LOG(ERROR) << "GetFund fail";
	//	return false;
	//}

	//double entrust_balance = entrust_price * entrust_amount;
	//if (entrust_balance <= 0 || entrust_balance > obj_CFund.enable_balance)
	//{
	//	ExceptionInterrupt;
	//	LOG(ERROR) << "委托金额不合法";
	//	MessageBox("委托金额不合法", "委托错误", NULL);
	//	return false;
	//}
	return true;
}


bool CTradeDlg::SetSecName(const CString &MarketCode, const CString SecCode, const int ID_SecName)
{
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
	{
		LOG(ERROR) << "GetSecurity fail";
		ExceptionInterrupt;
	}

	GetDlgItem(ID_SecName)->SetWindowText(obj_CSecurity.stock_name);
	return true;
}

bool CTradeDlg::SetSecName(const CString &MarketCode, const CString SecCode, CSimpleStatic* pSecName)
{
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
	{
		LOG(ERROR) << "GetSecurity fail";
		ExceptionInterrupt;
	}
	if (IsWindowValid(pSecName))
	{
		pSecName->SetWindowText(obj_CSecurity.stock_name);
	}

	return true;
}

bool CTradeDlg::SetEntrustPrice(const int Id_EntrustPrice, const CString &DefalutGear, const CString &MarketCode,
	const CString &SecCode)
{
	CString s_EntrustPrice;
	double EntrustPrice = 0;
	if (getDefalutPrice(DefalutGear, MarketCode, SecCode, EntrustPrice) == false)
	{
		LOG(ERROR) << "getDefalutPrice fail";
		ExceptionInterrupt;
	}
	s_EntrustPrice.Format(_T("%0.2f"), EntrustPrice);
	GetDlgItem(Id_EntrustPrice)->SetWindowText(s_EntrustPrice);
	return true;
}

bool CTradeDlg::SetEntrustPrice(CSimpleEdit* pEntrustPrice, const CString &DefalutGear, const CString &MarketCode,
	const CString &SecCode)
{
	CString s_EntrustPrice;
	double EntrustPrice = 0;
	if (getDefalutPrice(DefalutGear, MarketCode, SecCode, EntrustPrice) == false)
	{
		LOG(ERROR) << "getDefalutPrice fail";
		ExceptionInterrupt;
	}
	s_EntrustPrice.Format(_T("%0.2f"), EntrustPrice);
	if (IsWindowValid(pEntrustPrice))
	{
		pEntrustPrice->SetWindowText(s_EntrustPrice);
	}
	return true;
}

bool CTradeDlg::SetCanSellTotal(const int Id_CanSellTotal, const CString &SecCode)
{
	CString s_str;
	UInt_64 enable_amount;
	GaineEable_amount(SecCode, enable_amount);
	s_str.Format(_T("%u"), enable_amount);
	GetDlgItem(Id_CanSellTotal)->SetWindowText(s_str);
	return true;
}

bool CTradeDlg::SetCanSellTotal(CSimpleStatic* pCanSellTotal, const CString &SecCode)
{
	CString s_str;
	UInt_64 enable_amount;
	GaineEable_amount(SecCode, enable_amount);
	s_str.Format(_T("%u"), enable_amount);
	if (IsWindowValid(pCanSellTotal))
	{
		pCanSellTotal->SetWindowText(s_str);
	}

	return true;
}

bool CTradeDlg::SetEntrustBalance(const int ID_EntrustPrice, const int ID_EntrustAmount, const int ID_EntrustBalance)
{
	CString s_str;
	double EntrustPrice;
	GainLegalValue(EntrustPrice, ID_EntrustPrice);
	UInt_64 EntrustAmount;
	GainLegalValue(EntrustAmount, ID_EntrustAmount);
	double EntrustBalance = EntrustPrice*EntrustAmount;
	s_str.Format(_T("%0.2f"), EntrustBalance);
	GetDlgItem(ID_EntrustBalance)->SetWindowText(s_str);
	return true;
}
/*fixing 所有此类冗余代码全部砍掉 下单有声音，下单会变色*/
bool CTradeDlg::SetEntrustBalance(CSimpleEdit* pEntrustPrice, CSimpleEdit* pEntrustAmount, CSimpleStatic* pEntrustBalance)
{
	CString s_str;
	double EntrustPrice;
	GainLegalValue(EntrustPrice, pEntrustPrice);
	UInt_64 EntrustAmount;
	GainLegalValue(EntrustAmount, pEntrustAmount);
	double EntrustBalance = EntrustPrice*EntrustAmount;
	s_str.Format(_T("%0.2f"), EntrustBalance);
	if (IsWindowValid(pEntrustBalance))
	{
		pEntrustBalance->SetWindowText(s_str);
	}
	return true;
}

bool CTradeDlg::SetEnable_balance(const int ID_EnableBalance)
{
	/*CString s_str;

	CFund obj_CFund;
	if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
	{
		LOG(ERROR) << "GetFund fail";
		ExceptionInterrupt;
	}
	s_str.Format(_T("%0.2f"), obj_CFund.enable_balance);
	GetDlgItem(ID_EnableBalance)->SetWindowText(s_str);*/
	return true;
}

bool CTradeDlg::SetEnable_balance(CSimpleStatic* pEnableBalance, bool bFinancingBuy)
{
	CString s_str;
	double enable_balance = GainBalance_MaxEnable(bFinancingBuy);;
	s_str.Format(_T("%0.2f"), enable_balance);
	if (IsWindowValid(pEnableBalance))
	{
		pEnableBalance->SetWindowText(s_str);
	}
	return true;
}

bool CTradeDlg::SetDefaultBalance(bool &bCalculateByBalance, double &DefaultBalance, const int ID_DefaultBalance)
{
	CString s_DefaultBalance;
	if (true == bCalculateByBalance)
	{
		s_DefaultBalance.Format(_T("%0.0f"), DefaultBalance);
		(GetDlgItem(ID_DefaultBalance))->SetWindowText(s_DefaultBalance);
		GetDlgItem(ID_DefaultBalance)->EnableWindow(TRUE);
	}
	else
	{
		s_DefaultBalance = "0";
		(GetDlgItem(ID_DefaultBalance))->SetWindowText(s_DefaultBalance);
		GetDlgItem(ID_DefaultBalance)->EnableWindow(FALSE);
	}

	return true;
}

bool CTradeDlg::SetDefaultBalance(bool &bCalculateByBalance, double &DefaultBalance, CSimpleEdit* pDefaultBalance)
{
	CString s_DefaultBalance;
	if (IsWindowValid(pDefaultBalance))
	{
		if (true == bCalculateByBalance)
		{
			s_DefaultBalance.Format(_T("%0.0f"), DefaultBalance);
			pDefaultBalance->SetWindowText(s_DefaultBalance);
			pDefaultBalance->EnableWindow(TRUE);
		}
		else
		{
			s_DefaultBalance = "0";
			pDefaultBalance->SetWindowText(s_DefaultBalance);
			pDefaultBalance->EnableWindow(FALSE);
		}
	}

	return true;
}

bool CTradeDlg::SetBigCanBuyTotal(bool bFinancingBuy, CSimpleEdit* pEntrustPrice, CSimpleStatic* pBigCanBuyTotal)
{
	CString s_str;
	double EntrustPrice;
	GainLegalValue(EntrustPrice, pEntrustPrice);
	UInt_64 BigCanBuyTotal = 0;
	if (EntrustPrice == 0)
	{
		BigCanBuyTotal = 0;
	}
	else
	{
		BigCanBuyTotal = GainBigCanBuy(bFinancingBuy, EntrustPrice);
	}
	s_str.Format(_T("%u"), BigCanBuyTotal);
	if (IsWindowValid(pBigCanBuyTotal))
	{
		pBigCanBuyTotal->SetWindowText(s_str);
	}
	return true;
}

bool CTradeDlg::SetBottomTop(const CString &MarketCode, const CString &SecCode, const int ID_Bottom, const int ID_Top)
{
	CSecurity obj_CSecurity;
	CString s_str;
	if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
	{
		LOG(ERROR) << "GetSecurity fail";
		ExceptionInterrupt;
	}
	/*涨停价*/
	s_str.Format(_T("%0.2f"), obj_CSecurity.UpLimitPrice);
	GetDlgItem(ID_Top)->SetWindowText(s_str);
	/*跌停价*/
	s_str.Format(_T("%0.2f"), obj_CSecurity.DownLimitPrice);
	GetDlgItem(ID_Bottom)->SetWindowText(s_str);
	return true;
}

bool CTradeDlg::SetBottomTop(const CString &MarketCode, const CString &SecCode, CSimpleButton* pBottom, CSimpleButton* pTop)
{
	CSecurity obj_CSecurity;
	CString s_str;
	if (SPtr_CData->GetSecurity(MarketCode, SecCode, obj_CSecurity) == false)
	{
		LOG(ERROR) << "GetSecurity fail";
		ExceptionInterrupt;
	}
	/*涨停价*/
	s_str.Format(_T("%0.2f"), obj_CSecurity.UpLimitPrice);
	if (pTop != nullptr && IsWindow(pTop->GetSafeHwnd()))
	{
		pTop->SetWindowText(s_str);
	}

	/*跌停价*/
	s_str.Format(_T("%0.2f"), obj_CSecurity.DownLimitPrice);
	if (pBottom != nullptr && IsWindow(pBottom->GetSafeHwnd()))
	{
		pBottom->SetWindowText(s_str);
	}
	return true;
}

bool CTradeDlg::SetCanBuyTotal(bool bFinancingBuy, CSimpleEdit* pBEntrustPrice, CSimpleEdit* pBDefaultBalance, CSimpleStatic* p_BCanBuyTotal,
	CSimpleStatic* p_BigCanBuyTotal, bool bCalculateByBalance, const int DefaultAmount)
{
	CString s_str;
	double EntrustPrice;
	UInt_64 CanBuyTotal;
	GainLegalValue(EntrustPrice, pBEntrustPrice);
	if (EntrustPrice == 0)
	{
		CanBuyTotal = 0;
	}
	else
	{
		if (true == bCalculateByBalance)
		{
			/*根据默认资金计算*/
			double DefaultBalance;
			if (GainLegalValue(DefaultBalance, pBDefaultBalance) == false)
			{
				MessageBox("默认资金异常", "error", MB_OK);
			}
			CanBuyTotal = GainCanBuy(bFinancingBuy, DefaultBalance, EntrustPrice);
		}
		else
		{
			/*首先获取最大可买，在比较最大可买和可买股数*/
			UInt_64 BigCanBuyTotal;
			GainLegalValue(BigCanBuyTotal, p_BigCanBuyTotal);
			if (BigCanBuyTotal >= DefaultAmount)
			{
				CanBuyTotal = DefaultAmount;
			}
			else
			{
				CanBuyTotal = BigCanBuyTotal;
			}
		}
	}
	s_str.Format(_T("%u"), CanBuyTotal);
	if (IsWindowValid(p_BCanBuyTotal))
	{
		p_BCanBuyTotal->SetWindowText(s_str);
	}
	return true;
}

void CTradeDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case EventId_SetTimer_Invalidate:
		//CListCtrl_Alert.Invalidate();
		KillTimer(EventId_SetTimer_Invalidate);
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CTradeDlg::ClearPosition_Order_Deal(CListCtrl &CListCtrl_Position, CListCtrl &CListCtrl_Order, 
	CListCtrl &CListCtrl_Deal)
{
	/*持仓因为可能会少所以要清内存，其他都不会有类似情况*/
	CListCtrl_Position.SetItemCount(0);
	SPtr_CData->ClearMPosition(SPtr_CData->fund_account);
	CListCtrl_Order.SetItemCount(0);
	CListCtrl_Deal.DeleteAllItems();
}

void CTradeDlg::ClearData_DisConnect(CGridCtrlEx* pGridPosition, CGridCtrlEx* pGridOrder, CGridCtrlEx* pGridDeal)
{
	/*持仓因为可能会少所以要清内存，其他都不会有类似情况*/
	if (IsWindowValid(pGridPosition))
	{
		pGridPosition->SetRowCount(1);
	}
	
	SPtr_CData->ClearMPosition(SPtr_CData->fund_account);
	SPtr_CData->ClearMOrder(SPtr_CData->fund_account);
	SPtr_CData->M_OrderList.clear();
	if (IsWindowValid(pGridOrder))
	{
		pGridOrder->SetRowCount(1);
	}
	if (IsWindowValid(pGridDeal))
	{
		pGridDeal->SetRowCount(1);
	}
}

bool CTradeDlg::IsWindowValid(CWnd* pWnd)
{
	bool bValid = false;
	if (pWnd != nullptr && IsWindow(pWnd->GetSafeHwnd()))
	{
		bValid = true;
	}
	else
	{
		ExceptionInterrupt;
		bValid = false;
	}
	return bValid;
}

bool CTradeDlg::InspectbRedraw(const CString &stock_code, const CString &stock_codeBuy)
{
	bool bResult = true;
	if (stock_code == stock_codeBuy)
	{
		bResult = false;
	}
	return bResult;
}

/*subDeal如果采用实时刷新就不该采用这种算法*/
bool CTradeDlg::RedrawSubDeal(CString SecCode, CGridCtrlEx* pDealEachGrid)
{
	if (!IsWindowValid(pDealEachGrid))
	{
		return false;
	}
	VDealEach.clear();
	CString stock_account;
	if (SPtr_CData->SecCode2StockAccount(SecCode, stock_account) == false)
	{
		LOG(ERROR) << "SecCode2StockAccount fail";
		ExceptionInterrupt;
		return false;
	}

	if (SPtr_CData->GetMDeal(SPtr_CData->fund_account, stock_account, ShowingEntrust_no, m_mapDealEach) == false)
	{
		LOG(ERROR) << "GetMDeal fail";
		ExceptionInterrupt;
		return false;
	}
	for (auto iter = m_mapDealEach.begin(); iter != m_mapDealEach.end(); iter++)
	{
		if (iter->first == "0")
		{
			LOG(ERROR) << "business_id is 0";
			ExceptionInterrupt;
			continue;
		}
		VDealEach.push_back(iter->second);
	}
	pDealEachGrid->SetRowCount(VDealEach.size() + 1);
	pDealEachGrid->Invalidate();
	return true;
}

bool CTradeDlg::RedrawDealSummary(CGridCtrlEx* pDealSummaryGrid)
{
	if (!IsWindowValid(pDealSummaryGrid))
	{
		return false;
	}
	
	m_mapDealSummary.clear();
	VDealSummary.clear();

	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == SPtr_CData->GetMAccount(SPtr_CData->fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Order obj_MOrder;
			_Order::iterator iter_MOrder;

			if (SPtr_CData->GetMOrder(SPtr_CData->fund_account, iter_MAccount->first, obj_MOrder) == false)
			{
				LOG(ERROR) << "GetMOrder fail";
				ExceptionInterrupt;
				continue;
			}

			if (obj_MOrder.size() > 0)
			{
				/*处理每个委托单下的Deal*/
				for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
				{
					enumEntrustStatus entrust_status = iter_MOrder->second->entrust_status;
					if (BuChengDaiChe == entrust_status || BuChe == entrust_status || BuCheng == entrust_status ||
						YiCheng == entrust_status)
					{
						_Deal obj_MDeal;
						if (SPtr_CData->GetMDeal(SPtr_CData->fund_account, iter_MAccount->first, iter_MOrder->first, obj_MDeal) == false)
						{
							ErrorLog("GetMDeal fail");
							continue;
						}
						for (auto iter = obj_MDeal.begin(); iter != obj_MDeal.end(); iter++)
						{
							iter->second;/*成交结构体*/
							auto iter_DealSummary = m_mapDealSummary.find(iter->second->stock_code);
							if (iter_DealSummary == m_mapDealSummary.end())
							{
								/*没有找到此证券代码*/
								CDealSummary obj_CDealSummary;
								obj_CDealSummary.stock_code = iter->second->stock_code;
								obj_CDealSummary.stock_name = iter->second->stock_name;
								m_mapDealSummary.insert(make_pair(iter->second->stock_code, obj_CDealSummary));
								iter_DealSummary = m_mapDealSummary.find(iter->second->stock_code);
							}

							if (iter->second->entrust_bs == _Buy)
							{
								iter_DealSummary->second.Buy_BusinessAmount += iter->second->business_amount;
								iter_DealSummary->second.Buy_BusinessBalance += iter->second->business_balance;
								iter_DealSummary->second.AverageBuyPrice = iter_DealSummary->second.Buy_BusinessBalance /
									iter_DealSummary->second.Buy_BusinessAmount;
							}
							else
							{
								iter_DealSummary->second.Sell_BusinessAmount += iter->second->business_amount;
								iter_DealSummary->second.Sell_BusinessBalance += iter->second->business_balance;
								iter_DealSummary->second.AverageSellPrice = iter_DealSummary->second.Sell_BusinessBalance /
									iter_DealSummary->second.Sell_BusinessAmount;
							}
						}
					}				
				}

			}
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
	}

	/*重绘成交汇总表*/
	for (auto iter = m_mapDealSummary.begin(); iter != m_mapDealSummary.end(); iter++)
	{
		VDealSummary.push_back(iter->second);
	}

	pDealSummaryGrid->SetRowCount(VDealSummary.size() + 1);
	pDealSummaryGrid->Invalidate();

	return true;
}

CString CTradeDlg::GetFontStyle()
{
	CString s_FontSize;
	ptree pt;
	try 
	{
		read_xml(s_ConfigPath, pt);
		s_FontSize = pt.get<string>("content.FontSize").c_str();
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return "";
	}
	return s_FontSize;
}

void CTradeDlg::SetGlobalFont()
{
	CTradeDlg func;
	CString strStyle = func.GetFontStyle();
	if (FontStandard == strStyle.Trim())
	{
		s_hFontOrder = SingleFontSet.GetDefaultFont(DEFAULT_FONT_12B);
		s_hFontGrid = SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N);
		s_hFontTab = SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N);
		s_hFontTabSel = SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N);
	}
	else if (FontMedium == strStyle.Trim())
	{
		s_hFontOrder = SingleFontSet.GetDefaultFont(DEFAULT_FONT_14B);
		s_hFontGrid = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
		s_hFontTab = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
		s_hFontTabSel = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
	}
	else if (FontBig == strStyle.Trim())
	{
		s_hFontOrder = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16B);
		s_hFontGrid = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
		s_hFontTab = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
		s_hFontTabSel = SingleFontSet.GetDefaultFont(DEFAULT_FONT_16N);
	}
}

void CTradeDlg::CalculateTextRect(CString strText, HFONT hFont, CRect& rect)
{
	if (::IsWindow(m_hMainViewWnd))
	{
		CWnd* pWnd = FromHandle(m_hMainViewWnd);
		if (pWnd != nullptr)
		{
			CDC *pDC = pWnd->GetDC();
			CFont *pOld = pDC->SelectObject(CFont::FromHandle(hFont));
			pDC->DrawText(strText, rect, DT_CALCRECT | DT_LEFT | DT_VCENTER);
			pDC->SelectObject(pOld);
			pWnd->ReleaseDC(pDC);
		}
	}
	
}

void CTradeDlg::GetBtnRect(CString strText, HFONT hFont, CRect& rect)
{
	CRect rc;
	CalculateTextRect(strText, hFont, rc);
	
	int nHeight = 0;
	if (FontStandard == m_strStyle)
	{
		nHeight = 23;
	}
	else if (FontMedium == m_strStyle)
	{
		nHeight = 25;
	}
	else if (FontBig == m_strStyle)
	{
		nHeight = 27;
	}
	rect.SetRect(0, 0, rc.Width() + 20, nHeight);
}

void CTradeDlg::GetCheckRadioRect(CString strText, HFONT hFont, CRect& rect)
{
	CRect rc;
	CalculateTextRect(strText, hFont, rc);
	
	rect.SetRect(0, 0, rc.Width() + 18, rc.Height());
}

bool CTradeDlg::initBLog()
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		SPtr_CData->bLog_MarketAlert = pt.get<bool>("content.bLog_MarketAlert");
	}
	catch (std::exception& e)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "initBLog fail Error:" << e.what();
		return false;
	}

	return true;
}


