#include "stdafx.h"
#include "Login_Credit.h"

static void thread_ErrorInfo(PtrData SPtr_CData)
{
	/*登录界面不存在且此dll没有在登录的时候关闭*/
	while (true)
	{
		if (true == SPtr_CData->bQuit_ErrorInfo)
		{
			break;
		}
		
		if (SPtr_CData->Queue_T2ErrorInfo.size() > 0)
		{
			CString s_ErrorInfo = SPtr_CData->Queue_T2ErrorInfo.front().c_str();
			SPtr_CData->Queue_T2ErrorInfo.pop();
			::MessageBox(NULL, s_ErrorInfo, _T("融资融券_交易"), MB_OK);
			continue;
		}

		if (SPtr_CData->Queue_MarketErrorInfo.size() > 0)
		{
			CString s_ErrorInfo = SPtr_CData->Queue_MarketErrorInfo.front().c_str();
			SPtr_CData->Queue_MarketErrorInfo.pop();
			::MessageBox(NULL, s_ErrorInfo, _T("融资融券_行情"), MB_OK);
			continue;
		}
		Sleep(20);
	}
	SPtr_CData->bQuit_ErrorInfo = false;
	SPtr_CData->bRuning_ErrorInfo = false;
}

CLogin_Credit::CLogin_Credit(PtrData SPtr_CDataTmp, HWND Hwnd_LoginDlgTmp, CApiFun *pCApiFunTmp)
 :CLogin_Base(Hwnd_LoginDlgTmp, SPtr_CDataTmp, pCApiFunTmp)
{

	Hwnd_LoginDlg = Hwnd_LoginDlgTmp;

}

void CLogin_Credit::release()
{
	SPtr_CData->bQuit_ErrorInfo = true;
	SPtr_CData->bBeginRelease = true;
	pCApiFun->release();
	tool::MyFreeLibrary(ApiDll_Alert_SSE);
	tool::MyFreeLibrary(ApiDll_Alert_SZSE);
}

bool CLogin_Credit::Login_Credit(CProgressCtrl &obj_CProgressCtrl)
{
	InitUser_Pass(SPtr_CData);

	initCProgressCtrl(obj_CProgressCtrl);

	/*初始化错误信息线程 要考虑到这个线程被启动了，但是接下来的其他步骤出错的时候要关闭此线程的情况*/
	std::thread t_ErrorInfo(thread_ErrorInfo, SPtr_CData);
	t_ErrorInfo.detach();
	Sleep(100);
	SPtr_CData->bRuning_ErrorInfo = true;
    FillAlertUser(DllId_Credit, SPtr_CData);

	/*一定要保证loginCredit_T2_Asy最先执行，因为loginT2_Credit_SSE 和loginT2_Credit_SZSE都没有登录过程*/
	if (false == loginCredit_T2_Asy())
	{
		return false;
	}
	if (loginT2_Syn(&(pCApiFun->Api_T2_Syn),Api_Syn) == false)
	{
		return false;
	}
	if (loginT2_Syn(&(pCApiFun->Api_T2_SSE_Syn), Api_Syn_SSE) == false)
	{
		return false;
	}
	if (loginT2_Syn(&(pCApiFun->Api_T2_SZSE_Syn), Api_Syn_SZSE) == false)
	{
		return false;
	}
	
	if (false == loginCredit_Market_SSE())
	{
		return false;
	}

	if (false == loginCredit_Market_SZSE())
	{
		return false;
	}
	
	if (false == loginCredit_Alert_SSE())
	{
		return false;
	}
	if (false == loginCredit_Alert_SZSE())
	{
		return false;
	}

	obj_CProgressCtrl.SetPos(30);
	obj_CProgressCtrl.SetStep(30);

	/*首次登陆查询资金账户*/
	if (initFund_Credit() == false)
	{
		LOG(ERROR) << "Credit initFund false";
		AfxMessageBox(_T("Credit initFund false，请重新登录"));
		return false;
	}

	if (initStockAccount(SPtr_CData, pCApiFun) == false)
	{
		return false;
	}
	initCreditAsset();

	if (false == initStockMap(SPtr_CData, pCApiFun))
	{
		return false;
	}

	if (false == initDanBaoQuanMap())
	{
		return false;
	}

	/*订阅资金账户下的成交和委托*/
	if (tool::initSubscribe(SPtr_CData, pCApiFun, Api_Asy) != true)
	{
		return false;
	}
	obj_CProgressCtrl.SetPos(60);
	obj_CProgressCtrl.SetStep(60);

	/*查询持仓*/
	if (initPosition_Credit() == false)
	{
		LOG(ERROR) << "Credit initPosition false";
		AfxMessageBox(_T("Credit initPosition false，请重新登录"));
		return false;
	}

	/*查询全部委托*/
	if (initOrderDemo_Credit() == false)
	{
		LOG(ERROR) << "initOrderDemo fail";
		AfxMessageBox(_T("initOrderDemo false，请重新登录"));
		return false;
	}

	/*查询全部成交*/
	if (initDealDemo_Credit() == false)
	{
		LOG(ERROR) << "init DealDemo fail，请重新登录";
		AfxMessageBox(_T("init DealDemo fail，请重新登录"));
		return false;
	}

	if (false == initBiaoDiQuanMap())
	{
		return false;
	}
	/*等待担保券查询完毕*/
	if (WAIT_TIMEOUT == WaitForSingleObject(SPtr_CData->QueryDanBaoQuanMap_Event, LongestTime))
	{
		AfxMessageBox("query DanBaoQuan fail");
		LOG(ERROR) << "query DanBaoQuan fail";
		return false;
	}
	SPtr_CData->initM_AlertSecCode();
	obj_CProgressCtrl.SetPos(100);
	obj_CProgressCtrl.SetStep(100);
	return true;
}

bool CLogin_Credit::loginCredit_T2_Asy()
{
	int ret = 0;
	/*把Api的接口传递到client*/
	if (false == GainT2Api(&pCApiFun->Api_T2_Asy))
	{
		return false;
	}

	/*把spi的接口传递到Api*/
	CT2Spi *pCT2Spi = new CT2Spi_Credit;
	pCT2Spi->pCApiFun = pCApiFun;
	pCT2Spi->SPtr_CData = SPtr_CData;
	ret = pCApiFun->Api_T2_Asy->Register(Trans_Mode_ASY, Data_Proto_Json, pCT2Spi);
	if (ret < 0)
	{
		LOG(ERROR) << "RegisterSpi error:" << ret;
		ExceptionInterrupt;
		return false;
	}
	
	string s_Fun33110;
	GainLogin331100(s_Fun33110, SPtr_CData, AssetProp_信用账户);
	if (loginT2(pCApiFun, s_Fun33110, SPtr_CData) == false)
	{
		return false;
	}
	GainInfo_RiskControl(SPtr_CData);
	return true;
}

/*登录行情*/
bool CLogin_Credit::loginCredit_Market_SSE()
{
	/*把api的接口传递给client*/
	if (false == GainMarketApi(&pCApiFun->Api_Market_SSE))
	{
		return false;
	}
	/*Register spi*/
	CMarketSpi *pSpi_Market_SSE_Credit = new CMarketSpi_Credit_SSE;
	pSpi_Market_SSE_Credit->pCApiFun = pCApiFun;
	pSpi_Market_SSE_Credit->SPtr_CData = SPtr_CData;
	int ret = pCApiFun->Api_Market_SSE->Register(pSpi_Market_SSE_Credit);
	if (ret < 0)
	{
		LOG(ERROR) << "RegisterSpi_Market_SSE error ret:"<< ret;
		return false;
	}
    if (loginMarket(SPtr_CData->s_user, IDC_Market_SSE, SPtr_CData->LoginSSEMarket_Event)
		== false)
	{
		return false;
	}

	return true;
}



bool CLogin_Credit::loginCredit_Market_SZSE()
{
	/*把api的接口传递给client*/
	if (false == GainMarketApi(&pCApiFun->Api_Market_SZSE))
	{
		return false;
	}
	/*Register spi*/
	CMarketSpi *pCMarketSpi = new CMarketSpi_Credit_SZSE;
	pCMarketSpi->pCApiFun = pCApiFun;
	pCMarketSpi->SPtr_CData = SPtr_CData;
	int ret = pCApiFun->Api_Market_SZSE->Register(pCMarketSpi);
	if (ret < 0)
	{
		LOG(ERROR) << "RegisterSpi_Market_SSE error ret:" << ret;
		return false;
	}

    if (loginMarket(SPtr_CData->s_user, IDC_Market_SZSE,SPtr_CData->LoginSZSEMarket_Event) == false)
	{
		return false;
	}

	return true;
}



/*登录预警池*/
bool CLogin_Credit::loginCredit_Alert_SSE()
{
	/*init WarningPoolApi_Client.dll*/
	if (false == initAlertApi_SSE(ApiDll_Alert_SSE,"AlertApi_Credit_SSE.dll"))
	{
		return false;
	}
	/*Register spi*/
	int ret = 0;
	ret = pCApiFun->RegisterSpi_Alert_SSE("AlertSpi_Credit_SSE.dll");
	if (ret < 0)
	{
		switch (ret)
		{
		case -1:
			ErrorLog("Spi文件加载失败");
			break;
		case -2:
			ErrorLog("Spi文件函数重载错误");
			break;
		default:
			ErrorLog("abnormal ret");
			break;
		}
		return false;
	}

	/*init CDataInSpi*/
	if (false == TransferCDataToSpi("AlertSpi_Credit_SSE.dll"))
	{
		return false;
	}

	if (loginAlert(AlertType_SSE, IDC_AlertServer_SSE, pCApiFun->Connect_Alert_SSE, pCApiFun->SendOrder_Alert_SSE, SPtr_CData->Event_ConnectPartition_SSE
		, SPtr_CData->Event_loginSSEAlert, SPtr_CData->Event_ResetAlert_SSE, SPtr_CData)
		== false)
	{
		return false;
	}

	return true;
}

/*登录预警池*/
bool CLogin_Credit::loginCredit_Alert_SZSE()
{
	/*init WarningPoolApi_Client.dll*/
	if (false == initAlertApi_SZSE(ApiDll_Alert_SZSE,"AlertApi_Credit_SZSE.dll"))
	{
		return false;
	}
	/*Register spi*/
	int ret = 0;
	ret = pCApiFun->RegisterSpi_Alert_SZSE("AlertSpi_Credit_SZSE.dll");
	if (ret < 0)
	{
		switch (ret)
		{
		case -1:
			ErrorLog("Spi文件加载失败");
			break;
		case -2:
			ErrorLog("Spi文件函数重载错误");
			break;
		default:
			ErrorLog("abnormal ret");
			break;
		}
		return false;
	}

	/*init CDataInSpi*/
	if (false == TransferCDataToSpi("AlertSpi_Credit_SZSE.dll"))
	{
		return false;
	}

	if (loginAlert(AlertType_SZSE, IDC_AlertServer_SZSE, pCApiFun->Connect_Alert_SZSE, pCApiFun->SendOrder_Alert_SZSE, SPtr_CData->Event_ConnectPartition_SZSE
		, SPtr_CData->Event_loginSZSEAlert, SPtr_CData->Event_ResetAlert_SZSE, SPtr_CData)
		== false)
	{
		return false;
	}
	return true;
}

bool CLogin_Credit::initFund_Credit()
{
	string s_Fun335101;
	tool::GainQueryFund335101(s_Fun335101,SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun, Api_Asy, s_Fun335101,
		SPtr_CData);
	if (RefId_HS < 0)
	{
		ErrorLog("abnormal RefId_HS");
		return false;
	}
	SPtr_CData-> SetRefId_T2(RefId_HS, RefType_335101_InitFund);
	if (WAIT_TIMEOUT == WaitForSingleObject(SPtr_CData->InitFund_Event, ShortTime))
	{
		ErrorLog("init fund fail");
		return false;
	}
	return true;
}

bool CLogin_Credit::initCreditAsset()
{
	string s_Fun335504;
	tool::GainQueryCreditAsset335504(s_Fun335504, SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun, Api_Asy, s_Fun335504,SPtr_CData);
	if (RefId_HS < 0)
	{
		ErrorLog("abnormal RefId_HS");
		return false;
	}
	return true;
}

bool CLogin_Credit::initPosition_Credit()
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335102;
			GainQuerySingleBatchPosition335102_Credit(s_Fun335102, position_str);
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun335102,SPtr_CData) < 0)
			{
				bRun = false;
				continue;
			}
			else
			{
				int ret = pCApiFun->Api_T2_Syn->RecvMsg();
				if (ret < 0)
				{
			     	ErrorLog(pCApiFun->Api_T2_Syn->GetLastErrorMsg());
					tool::ExceptionHandle_RecvMsg();
					return false;
				}
				char* pRespData = new char[ret + 1];
				ret = pCApiFun->Api_T2_Syn->GetJsonValue(pRespData);

				Json::Reader reader;
				Json::Value root;

				if (!reader.parse(string(pRespData), root, false))
				{
					ExceptionInterrupt;
					LOG(ERROR) << "parse fail";
					continue;
				}
				OnQueryPosition_Credit(root, position_str, bContinuousQuery);
				delete[] pRespData;
			}
		}
		else
		{
			bRun = false;
		}
	}

	return true;
}

bool CLogin_Credit::GainQuerySingleBatchPosition335102_Credit(string &s_str, const string &position_str)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335102";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["asset_prop"] = AssetProp_信用账户;
	obj_Value["position_str"] = position_str;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	s_str = obj_Value.toStyledString();
	return true;
}

bool CLogin_Credit::OnQueryPosition_Credit(const Value &root, string &position_str, bool &bContinuousQuery)
{
	bool bBeginRedrawPosition = false;
	if (root["json"].isNull())
	{
		bContinuousQuery = false;
		return true;
	}

	int count = root["json"].size();

	if (count >= SingleQuerySize)
	{
		position_str = root["json"][count - 1]["position_str"].asString();
		bContinuousQuery = true;
	}
	else
	{
		bContinuousQuery = false;
		bBeginRedrawPosition = true;
	}
	for (int i = 0; i < count; i++)
	{
		if (Inspect_bStock(root, i) == false)
		{
			continue;
		}
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CPosition obj_CPosition;

			CString fund_account = root["json"][i]["fund_account"].asString().c_str();//资金账户
			CString stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户

			obj_CPosition.stock_name = root["json"][i]["stock_name"].asString().c_str();//股票名称
			obj_CPosition.stock_code = root["json"][i]["stock_code"].asString().c_str();//股票代码
			obj_CPosition.current_amount = convert<UInt_64>(root["json"][i]["current_amount"].asString());//股票余额 UInt_32 为了应对e先转double
			obj_CPosition.enable_amount = convert<UInt_64>(root["json"][i]["enable_amount"].asString());//可用数量 UInt_32
			obj_CPosition.market_value = convert<double>(root["json"][i]["market_value"].asString());//市值

			obj_CPosition.income_balance = convert<double>(root["json"][i]["income_balance"].asString());//盈亏
			obj_CPosition.cost_price = convert<double>(root["json"][i]["cost_price"].asString());//成本价
			obj_CPosition.last_price = convert<double>(root["json"][i]["last_price"].asString());//最新价
			obj_CPosition.profit_ratio = convert<double>(root["json"][i]["profit_ratio"].asString());

			if (SPtr_CData->SetPosition(fund_account, stock_account, obj_CPosition) == false)
			{
				LOG(ERROR) << "SetPosition fail";
				ExceptionInterrupt;
				return false;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			ExceptionInterrupt;
			return false;
		}
	}
	if (true == bBeginRedrawPosition)
	{
		if (SPtr_CData->hwnd_TradeDlg == NULL)
		{
			SPtr_CData->Queue_WmMessage.push(WM_ReDrawPosition);
		}
		else
		{
			if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_ReDrawPosition, 0, 0) == 0)
			{
				LOG(ERROR) << "PostMessage fail";
				ExceptionInterrupt;
			}
		}
	}
	return true;
}
