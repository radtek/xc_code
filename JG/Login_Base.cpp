#include "stdafx.h"
#include "Login_Base.h"

CLogin_Base::CLogin_Base(HWND Hwnd_LoginDlgTmp, PtrData SPtr_CDataTmp, CApiFun *pCApiFunTmp)
{
	Hwnd_LoginDlg = Hwnd_LoginDlgTmp;
	SPtr_CData = SPtr_CDataTmp;
	pCApiFun = pCApiFunTmp;
}


bool CLogin_Base::loginT2(CApiFun *pCApiFun, const string &s_Fun33110, PtrData &SPtr_CData)
{
	string fund_account;
    if (ConnectT2(Api_Asy, SPtr_CData->fund_account.GetBuffer()) == false)
	{
		return false;
	}

	if (tool::SendOrder(pCApiFun, Api_Asy,s_Fun33110, SPtr_CData) < 0)
	{
		return false;
	}

	if (WAIT_TIMEOUT == WaitForSingleObject(SPtr_CData->LoginT2_Event, ShortTime))
	{
		AfxMessageBox("登录交易服务器失败");
		LOG(ERROR) << "登录交易服务器失败";
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool CLogin_Base::ConnectT2(const int ApiType, const string &fund_account)
{
	int ret;
	string s_Adress;
	if (GainConnectInfo(IDC_Trade, s_Adress) == false)
	{
		return false;
	}
	CXcTradeApi *pCXcTradeApi = NULL;
	switch (ApiType)
	{
	case Api_Asy:
		pCXcTradeApi = pCApiFun->Api_T2_Asy;
		break;
	case Api_Syn:
		pCXcTradeApi = pCApiFun->Api_T2_Syn;
		break;
	case Api_Syn_SSE:
		pCXcTradeApi = pCApiFun->Api_T2_SSE_Syn;
		break;
	case Api_Syn_SZSE:
		pCXcTradeApi = pCApiFun->Api_T2_SZSE_Syn;
		break;
	default:
		ErrorLog("abnormal ApiType");
		break;
	}
	ret = pCXcTradeApi->Connect((char *)s_Adress.c_str(), (char *)s_LicensePath.c_str(), System_UFX, (char *)fund_account.c_str());
	if (ret < 0)
	{
		string s_str = pCXcTradeApi->GetLastErrorMsg();
		LOG(ERROR) << s_str;
		AfxMessageBox(s_str.c_str());
		return false;
	}
	return true;
}


bool CLogin_Base::loginT2_Syn(CXcTradeApi **ppCXcTradeApi, const int ApiType)
{
	if (GainT2Api(ppCXcTradeApi) == false)
	{
		return false;
	}
	int ret = (*ppCXcTradeApi)->Register(Trans_Mode_SYN, Data_Proto_Json);
	if (ret < 0)
	{
		ErrorLog((*ppCXcTradeApi)->GetLastErrorMsg());
		return false;
	}
	if (ConnectT2(ApiType, SPtr_CData->fund_account.GetBuffer()) == false)
	{
		return false;
	}
	return true;
}

bool CLogin_Base::loginMarket(string s_uer, const int CtrID, HANDLE &LoginMarket_Event)
{
	int ret;
	string s_Ip;
	string s_Port;
	if (GainConnectInfo(CtrID, s_Ip, s_Port) == false)
	{
		return false;
	}
	CXcMarketApi *pApiConnect = NULL;
	switch (CtrID)
	{
	case IDC_Market_SSE:
		pApiConnect = pCApiFun->Api_Market_SSE;
		break;
	case IDC_Market_SZSE:
		pApiConnect = pCApiFun->Api_Market_SZSE;
		break;
	default:
		break;
	}
	ret = pApiConnect->Connect((char *)s_uer.c_str(),(char *)s_Ip.c_str(), (char *)s_Port.c_str(), (char *)License_Market.c_str());
	if (ret < 0)
	{
		/*-1：链接失败
		-2：发送登陆请求失败
		-3: 接收登陆通讯异常或者用户验证错误（错误原因见OnUserLogin）
		-10:密钥文件路径错误
		-11:密钥读取失败
		-12:密钥长度错误
		-13:用户账户长度错误
		-14:通讯初始化错误*/
		string s_str;
		switch (CtrID)
		{
		case IDC_Market_SSE:
			s_str += "上海主站连接失败";
			break;
		case IDC_Market_SZSE:
			s_str += "深圳主站连接失败";
			break;
		default:
			ErrorLog("abnormal CtrID");
			break;
		}
		s_str += " error_info:";
		switch (ret)
		{
		case -1:
			s_str += "链接失败";
			break;
		case -2:
			s_str += "发送登陆请求失败";
			break;
		case -3:
			s_str += "接收登陆通讯异常或者用户验证错误";
			break;
		case -10:
			s_str += "密钥文件路径错误";
			break;
		case -11:
			s_str += "密钥读取失败";
			break;
		case -12:
			s_str += "密钥长度错误";
			break;
		case -13:
			s_str += "用户账户长度错误";
			break;
		case -14:
			s_str += "通讯初始化错误";
			break;
		default:
			ErrorLog("abnormal ret");
			break;
		}
		LOG(ERROR) << s_str;
		MessageBox(Hwnd_LoginDlg,s_str.c_str(),"错误",NULL);
		return false;
	}

	if (WAIT_TIMEOUT == WaitForSingleObject(LoginMarket_Event, ShortTime))
	{
		AfxMessageBox("登录行情服务器失败");
		LOG(ERROR) << "登录行情服务器失败";
		return false;
	}
	return true;
}

bool CLogin_Base::loginAlert(int alertType, const int CtrID, pAlertConnect AlertApi_Connect, pAlertSendOrder AlertAPi_SendOrder, HANDLE &ConnectPartition_Event,
	HANDLE &LoginAlert_Event, HANDLE &ResetAlet_Event, PtrData &SPtr_CData)
{
	string s_Ip;
	string s_Port;
	if (GainConnectInfo(CtrID, s_Ip, s_Port) == false)
	{
		return false;
	}
	string s_ConnectPartition;
	GainConnectPartition(s_ConnectPartition, SPtr_CData, alertType);
	int ret = AlertApi_Connect((char *)s_Ip.c_str(), (char *)s_Port.c_str(), (char*)s_ConnectPartition.c_str());
	if (ret < 0)
	{
		string s_Error = "预警主站连接失败，error_info:";
		switch (ret)
		{
		case -1:
			s_Error += "未链接";
			break;
		case -2:
			s_Error += "发送登录信息失败";
			break;
		case -3:
			s_Error += "登录信息验证失败";
			break;
		case -4:
			s_Error += "数据接收创建失败";
			break;
		default:
			ErrorLog("abnormal ret)");
			break;
		}
		LOG(ERROR) << s_Error;
		MessageBox(Hwnd_LoginDlg, s_Error.c_str(), "错误", NULL);
		return false;
	}
	UInt_32 ret_Wait = WaitForSingleObject(ConnectPartition_Event, ShortTime);
	bool b_ret;
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	if (b_ret == false)
	{
		AfxMessageBox("链接预警服务器失败");
		LOG(ERROR) << "链接预警服务器失败";
		return false;
	}
	string s_ResetAlert;
	tool::GainResetAlert(s_ResetAlert, SPtr_CData, alertType);
	ret = AlertAPi_SendOrder((char *)s_ResetAlert.c_str());
	if (tool::DealRet_AlertSendOrder(ret, Hwnd_LoginDlg) == false)
	{
		return false;
	}
	ret_Wait = WaitForSingleObject(ResetAlet_Event, ShortTime);
	b_ret;
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	if (b_ret == false)
	{
		AfxMessageBox("链接预警服务器失败");
		LOG(ERROR) << "链接预警服务器失败";
		return false;
	}
	
	string s_LoginAlert;
	GainLoginAlert(s_LoginAlert,SPtr_CData, alertType);
	ret = AlertAPi_SendOrder((char *)s_LoginAlert.c_str());
	if (tool::DealRet_AlertSendOrder(ret,Hwnd_LoginDlg) == false)
	{
		return false;
	}

	ret_Wait = WaitForSingleObject(LoginAlert_Event, ShortTime);
	b_ret;
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	if (b_ret == false)
	{
		AfxMessageBox("链接预警服务器失败");
		LOG(ERROR) << "链接预警服务器失败";
		return false;
	}
	return true;
}

void CLogin_Base::GainConnectPartition(string &s_dst, PtrData &SPtr_CData,int alertType)
{
	string  partitionId;
	partitionId = tool::addSuffix(alertType, SPtr_CData->partitionId);
	Json::Value obj_Value;
	obj_Value["msgType"] = MsgType_ConnectPartition;
	obj_Value["refferId"] = "1";
	obj_Value["partitionId"] = partitionId;
	s_dst = obj_Value.toStyledString();
}

void CLogin_Base::GainLoginAlert(string &s_dst, PtrData &SPtr_CData, int alertType)
{
	string partitionId;
	string userId;
	partitionId = tool::addSuffix(alertType, SPtr_CData->partitionId);
	userId = tool::addSuffix(alertType, SPtr_CData->user_alert);
	Json::Value obj_Value;
	obj_Value["msgType"] = MsgType_Login;
	obj_Value["refferId"] = "2";
	obj_Value["partitionId"] = partitionId;
	obj_Value["userId"] = userId;
	s_dst = obj_Value.toStyledString();
}


bool CLogin_Base::GainT2Api(CXcTradeApi **ppCXcTradeApi)
{
	*ppCXcTradeApi = CXcTradeApi::CreateTradeApi();
	if (NULL == *ppCXcTradeApi)
	{
		ErrorLog("pCXcTradeApi is NULL");
		return false;
	}
	return true;
}

bool CLogin_Base::GainMarketApi(CXcMarketApi **ppCXcMarketApi)
{
	*ppCXcMarketApi = CXcMarketApi::CreateMarketApi();
	if (*ppCXcMarketApi == NULL)
	{
		ErrorLog("pCXcMarketApi is NULL");
		return false;
	}
	return true;
}

bool CLogin_Base::GainConnectInfo(const int CtrID, string &s_Adress)
{
	CString s_str;
	CtrlId2CWnd(CtrID)->GetWindowText(s_str);
	if ("" == s_str)
	{
		s_Adress = "";
		return false;
	}
	s_Adress = s_str;
	int pos_begin = s_Adress.find(":");
	pos_begin += strlen(":");
	s_Adress = s_Adress.substr(pos_begin, s_Adress.length() - pos_begin);
	return true;
}

bool CLogin_Base::GainConnectInfo(const int CtrID, string &s_Ip, string &s_Port)
{
	CString s_str;
	CtrlId2CWnd(CtrID) ->GetWindowText(s_str);
	s_str.Delete(0, s_str.Find(":") + 1);
	int pos = 0;
	pos = s_str.Find(":");
	if (pos < 0)
	{
		LOG(ERROR) << "GainConnectInfo fail";
		ExceptionInterrupt;
		return false;
	}
	s_Ip = s_str.Left(pos);
	s_Port = s_str.Right(s_str.GetLength() - pos - 1);
	return true;
}


bool CLogin_Base::initStockAccount(PtrData &SPtr_CData, CApiFun *pCApiFun)
{
	string s_Fun331300;
	GainStockAccount331300(s_Fun331300, SPtr_CData);
	if (tool::SendOrder(pCApiFun,Api_Asy,s_Fun331300, SPtr_CData) < 0)
	{
		return false;
	}
	if (WAIT_TIMEOUT == WaitForSingleObject(SPtr_CData->QueryStockAccount_Event, ShortTime))
	{
		AfxMessageBox("查询股东账户失败，请重新登录");
		return false;
	}
	return true;
}


bool CLogin_Base::initStockMap(PtrData &SPtr_CData, CApiFun *pCApiFun)
{
	//根据股东账户去初始化全市场代码，没有股东账户直接就不能输入股票代码
	_Account MAccount;
	if (SPtr_CData->GetMAccount(SPtr_CData->fund_account,MAccount) == false)
	{
		ErrorLog("GetMAccount fail");
		return false;
	}
	for (auto iter = MAccount.begin(); iter != MAccount.end(); iter++)
	{
		iter->second->market_code;//市场代码
		switch (tool::find_enumMarketCode(string(iter->second->market_code) ) )
		{
		case enumSSE:
			if (initStockMap_Single(SPtr_CData, pCApiFun, iter->second->market_code,
				SPtr_CData->QuerySSEStockMap_Event) == false)
			{
				return false;
			}
			break;
		case enumSZSE:
			if (initStockMap_Single(SPtr_CData, pCApiFun, iter->second->market_code,
				SPtr_CData->QuerySZSEStockMap_Event) == false)
			{
				return false;
			}
			break;
		case enumHGT:
			break;
		case enumSGT:
			break;
		default:
			ExceptionInterrupt;
			LOG(ERROR) << "abanormal market_code:" << iter->second->market_code;
			break;
		}	
	}
	return true;
}

bool CLogin_Base::initAlertApi_SSE(HINSTANCE &ApiDll_Alert_SSE, const char *ApiPath)
{
	if (MyLoadLibrary(ApiDll_Alert_SSE, ApiPath) == false)
	{
		return false;
	}
	pCApiFun->Connect_Alert_SSE = (pAlertConnect)GetProcAddress(ApiDll_Alert_SSE, "Connect");
	pCApiFun->RegisterSpi_Alert_SSE = (pAlertRegisterSpi)GetProcAddress(ApiDll_Alert_SSE, "RegisterSpi");
	pCApiFun->SendOrder_Alert_SSE = (pAlertSendOrder)GetProcAddress(ApiDll_Alert_SSE, "Subscribe");
	pCApiFun->Release_Alert_SSE = (pAlertRelease)GetProcAddress(ApiDll_Alert_SSE, "Release");

	if (pCApiFun->Connect_Alert_SSE == NULL || pCApiFun->RegisterSpi_Alert_SSE == NULL || pCApiFun->SendOrder_Alert_SSE == NULL ||
		pCApiFun->Release_Alert_SSE == NULL)
	{
		LOG(ERROR) << "TransferSSEAlertApiToFast error";
		return false;
	}
	return true;
}

bool CLogin_Base::initAlertApi_SZSE(HINSTANCE &ApiDll_Alert_SZSE, const char *ApiPath)
{
	if (MyLoadLibrary(ApiDll_Alert_SZSE, ApiPath) == false)
	{
		return false;
	}

	pCApiFun->Connect_Alert_SZSE = (pAlertConnect)GetProcAddress(ApiDll_Alert_SZSE, "Connect");
	pCApiFun->RegisterSpi_Alert_SZSE = (pAlertRegisterSpi)GetProcAddress(ApiDll_Alert_SZSE, "RegisterSpi");
	pCApiFun->SendOrder_Alert_SZSE = (pAlertSendOrder)GetProcAddress(ApiDll_Alert_SZSE, "Subscribe");
	pCApiFun->Release_Alert_SZSE = (pAlertRelease)GetProcAddress(ApiDll_Alert_SZSE, "Release");

	if (pCApiFun->Connect_Alert_SZSE == NULL || pCApiFun->RegisterSpi_Alert_SZSE == NULL || pCApiFun->SendOrder_Alert_SZSE == NULL ||
		pCApiFun->Release_Alert_SZSE == NULL)
	{
		LOG(ERROR) << "TransferSZSEAlertApiToFast error";
		return false;
	}

	return true;
}

bool CLogin_Base::TransferCDataToSpi(char* SpiPath)
{
	HINSTANCE hSpiTmp;
	if (MyLoadLibrary(hSpiTmp, SpiPath) == false)
	{
		return false;
	}
	InitCData* pInitCData;
	pInitCData = (InitCData*)GetProcAddress(hSpiTmp, "dll_InitCData");
	if (NULL == pInitCData)
	{
		ErrorLog("init pInitData fail");
		FreeLibrary(hSpiTmp);
		return false;
	}
	pInitCData(SPtr_CData);
	FreeLibrary(hSpiTmp);
	return true;
}

bool CLogin_Base::GainQuerySingleBatch330352(PtrData &SPtr_CData, string &s_fun, string position_str, string exchange_type)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "330352";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["exchange_type"] = exchange_type;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	obj_Value["position_str"] = position_str;
	s_fun = obj_Value.toStyledString();
	return true;
}


bool CLogin_Base::initStockMap_Single(PtrData &SPtr_CData, CApiFun *pCApiFun,CString MarketCode, 
	HANDLE  &QueryStockMap_Event)
{
	socket_struct_RequireDetail ReqList2[MaxSize];
	strcpy(ReqList2[0].MarketCode, MarketCode);
	strcpy(ReqList2[0].SecCode, "*");

	SendRequireSecurity(pCApiFun, MarketCode, IDMarket_QueryStockMap, ReqList2, 1);
	LOG(INFO) << "init stockMap:" << ReqList2;
 	Int_64 ret = WaitForSingleObject(QueryStockMap_Event, MediumTime);
	bool b_ret;
	DealReturnValue_WaitForSingleObject(ret, b_ret);
	if (b_ret == false)
	{
		CString s_str;
		s_str += "初始化";
		s_str += MarketCode;
		s_str += "全市场代码失败";
		AfxMessageBox(s_str);
		LOG(ERROR) << s_str;
		return false;
	}
	return true;
}

bool CLogin_Base::GainLogin331100(string &s_str, PtrData &SPtr_CData, string asset_prop)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "331100";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = "TEST";
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = "2";
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["input_content"] = "1";
	obj_Value["account_content"] = SPtr_CData->s_user;
	obj_Value["content_type"] = "0";
	obj_Value["asset_prop"] = asset_prop;
	s_str = obj_Value.toStyledString();
	return true;
}

bool CLogin_Base::GainStockAccount331300(string &s_str, PtrData &SPtr_CData)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "331300";
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password_type"] = password_type_交易密码;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	obj_Value["branch_no"] = SPtr_CData->branch_no;
	s_str = obj_Value.toStyledString();
	return true;
}


bool CLogin_Base::GainInfo_RiskControl(PtrData &SPtr_CData)
{
	SPtr_CData->OrderInterval = pCApiFun->Api_T2_Asy->GetSpace();
	SPtr_CData->Frequency = pCApiFun->Api_T2_Asy->GetFrequency();
	return true;
}


void CLogin_Base::ExceptionHandle(string s_str, const int ErrorLevel)
{
	ExceptionInterrupt;
	switch (ErrorLevel)
	{
	case MyINFO:
		LOG(INFO) << s_str;
		break;
	case MyWARNING:
		LOG(WARNING) << s_str;
		break;
	case MyERROR:
		LOG(ERROR) << s_str;
		break;
	case MyFATAL:
		LOG(FATAL) << s_str;
		break;
	default:
		break;
	}
}

bool CLogin_Base::SendRequireSecurity(CApiFun *pCApiFun, const CString MarketCode, UInt_64 RefID,
	socket_struct_RequireDetail ReqList[MaxSize], int ReqSize)
{
	int ret;
	switch (tool::find_enumMarketCode(string(MarketCode)))
	{
	case enumSSE:
	{
		ret = pCApiFun->Api_Market_SSE->Require(RefID, ReqList, ReqSize);
		break;
	}
	case enumSZSE:
	{
		ret = pCApiFun->Api_Market_SZSE->Require(RefID, ReqList, ReqSize);
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
		AfxMessageBox("SendRequireSecurity failed");
		LOG(ERROR) << "SendRequireSecurity failed";
		return false;
	}

	return true;
}


bool CLogin_Base::MyLoadLibrary(HINSTANCE &hDllTmp, const char *DllName)
{
	string s_ExePath;
	tool::initExePath(s_ExePath);
	string s_DllPath = s_ExePath + "\\" + DllName;
	hDllTmp = LoadLibrary(s_DllPath.c_str());
	if (hDllTmp == NULL)
	{
		LOG(ERROR) << "load Dll error";
		ExceptionInterrupt;
		FreeLibrary(hDllTmp);
		return false;
	}
	return true;
}

CWnd* CLogin_Base::CtrlId2CWnd(const int CtrID)
{
	return CWnd::FromHandle(GetDlgItem(Hwnd_LoginDlg, CtrID));
}

void CLogin_Base::initCProgressCtrl(CProgressCtrl &obj_CProgressCtrl)
{
	obj_CProgressCtrl.SetRange(1, 100);
	obj_CProgressCtrl.SetPos(0);
	obj_CProgressCtrl.SetPos(10);
	obj_CProgressCtrl.SetStep(10);
}

void CLogin_Base::InitUser_Pass(PtrData SPtr_CData)
{
	CString s_user;
	CString s_pass;
	CtrlId2CWnd(IDC_User)->GetWindowText(s_user);
	CtrlId2CWnd(IDC_Pass)->GetWindowText(s_pass);
	SPtr_CData->s_user = s_user;
	SPtr_CData->s_pass = s_pass;
	SPtr_CData->fund_account = s_user;
}

void CLogin_Base::FillAlertUser(int DllID,PtrData SPtr_CData)
{
	string s_UUID = GainUUId();
	SPtr_CData->partitionId = s_UUID;
	SPtr_CData->user_alert = s_UUID;
	/*string Partition_Node;
	string UserId_Node;
	Partition_Node = "Partition";
	UserId_Node = "UserId";*/
	/*char path[256];
	switch (DllID)
	{
	case DllId_Fast:
		strcpy(path, "../share/AlertUser_Fast.txt");
		break;
	case DllId_Credit:
		strcpy(path, "../share/AlertUser_Credit.txt");
		break;
	case DllId_Auto:
		strcpy(path, "../share/AlertUser_auto.txt");
		break;
	default:
		ErrorLog("abnormal DllID");
		return ;
		break;
	}*/
	
	//Json::Reader reader;
	//Json::Value root;
	//string s_FileContent;
	//if (PathFileExistsA(path) != TRUE)
	//{
	//	/*不存在，就创建*/
	//	string s_UUID = GainUUId();
	//	root[Partition_Node] = Json::Value(s_UUID);
	//	root[UserId_Node] = Json::Value(s_UUID);
	//	
	//	SPtr_CData->partitionId = s_UUID;
	//	SPtr_CData->user_alert = s_UUID;

	//	Json::StyledWriter sw;
	//	std::string res_str = sw.write(root);
	//	tool::string2File(path, res_str);
	//}
	//else
	//{
	//	tool::File2String(path, s_FileContent);
	//	if (reader.parse(s_FileContent, root, false))
	//	{
	//		SPtr_CData->partitionId = root[Partition_Node].asString();
	//		SPtr_CData->user_alert = root[UserId_Node].asString();
	//	}
	//	else
	//	{
	//		ExceptionInterrupt;
	//		LOG(ERROR) << "parse fail:" << reader.getFormatedErrorMessages();
	//		return;
	//	}	
	//}
}

string CLogin_Base::GainUUId()
{
	char buffer[64] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");
		ErrorLog("create guid error");
		return "";
	}
	_snprintf(buffer, sizeof(buffer),
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return buffer;
}

bool CLogin_Base::Inspect_bStock(const Value &root, int i)
{
	/*添加这段代码是为了防范非股票*/
	CString MarketCode;
	CString SecCode = root["json"][i]["stock_code"].asString().c_str();
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return false;
	}
	return true;
}