#include "stdafx.h"
#include "T2Spi_Credit.h"

VOID CALLBACK OneMilliSecondProc(PVOID lpParameter, BOOLEAN ret)
{
	CT2Spi_Credit *pCT2Spi_Credit = (CT2Spi_Credit *)lpParameter;
	map<string, atomic_bool>::iterator iter;
	for (iter = pCT2Spi_Credit->M_QueryData.begin(); iter != pCT2Spi_Credit->M_QueryData.end(); iter++)
	{
		if (iter->second)
		{
			string SecCode = iter->first;
			/*在任何操作导致资金或持仓变化的时候更新一次资金和持仓*/
			if (pCT2Spi_Credit->QueryFund() == false)
			{
				LOG(ERROR) << "QueryFund fail";
				ExceptionInterrupt;
			}

			if (pCT2Spi_Credit->QueryCreditAsset() == false)
			{
				LOG(ERROR) << "QueryCreditAsset fail";
				ExceptionInterrupt;
			}

			if (pCT2Spi_Credit->QuerySinglePosition(SecCode) == false)
			{
				LOG(ERROR) << "QuerySinglePosition fail";
				ExceptionInterrupt;
			}
			iter->second = false;
		}
	}
}

VOID CALLBACK OnProc_Order(PVOID lpParameter, BOOLEAN ret)
{
	CT2Spi_Credit *pCT2Spi_Credit = (CT2Spi_Credit *)lpParameter;
	if (pCT2Spi_Credit->bPostMessage)
	{
		tool::MyPostMessage(pCT2Spi_Credit->SPtr_CData->hwnd_TradeDlg, WM_RedrawOrder, pCT2Spi_Credit->bPostMessage);
	}
	pCT2Spi_Credit->bPostMessage = false;
}

CT2Spi_Credit::CT2Spi_Credit()
{
	if (!CreateTimerQueueTimer(&hTimer, NULL, WAITORTIMERCALLBACK(OneMilliSecondProc), (PVOID)this, DueTime, Timer_ms_Query, NULL))
	{
		LOG(ERROR) << "CreateTimerQueueTimer fail";
	}
	if (!CreateTimerQueueTimer(&hTimer_Order, NULL, WAITORTIMERCALLBACK(OnProc_Order), (PVOID)this, DueTime, Timer_ms_Order, NULL))
	{
		LOG(ERROR) << "CreateTimerQueueTimer fail";
	}
}

CT2Spi_Credit::~CT2Spi_Credit()
{
	if (!DeleteTimerQueueTimer(NULL, hTimer, NULL))
	{
		LOG(ERROR) << "DeleteTimerQueueTimer fail";
	}
	if (!DeleteTimerQueueTimer(NULL, hTimer_Order, NULL))
	{
		LOG(ERROR) << "DeleteTimerQueueTimer fail";
	}
}

void CT2Spi_Credit::OnClose()
{
	if (SPtr_CData->bRun_MainPageDlg == true && false == SPtr_CData->bBeginRelease)
	{
		/*异常退出程序*/
		LOG(ERROR) << "CT2Spi_Credit Close.........";
		MessageBox(SPtr_CData->hwnd_MainPageDlg, _T("连接中断，请重新登录"), "error", 0);
		/*退出程序*/
		if (PostMessage(SPtr_CData->hwnd_MainPageDlg, WM_AbnormalExit, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
		}
	}
}

void CT2Spi_Credit::OnDisConnect()
{
	static int count = 0;
	if (count % 5 == 0)
	{
		PostMessage(SPtr_CData->hwnd_TradeDlg, WM_DisConnect_T2, NULL, NULL);
	}
	count++;
}

void CT2Spi_Credit::OnConnected()
{
	PostMessage(SPtr_CData->hwnd_TradeDlg, WM_Connected_T2, NULL, NULL);

	LOG(ERROR) << "OnConnected" << endl;
	if (tool::initSubscribe(SPtr_CData, pCApiFun, Api_Asy) != true)
	{
		return;
	}
}

bool CT2Spi_Credit::OnLogin(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 登录找不到代表程序异常*/
		LOG(ERROR) << "OnLogin no json";
		ExceptionInterrupt;
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i< count; i++)
	{
		if (tool::VerifyElement(root, i,SPtr_CData))
		{
			SPtr_CData->branch_no = root["json"][i]["branch_no"].asString();
			SPtr_CData->sysnode_id = root["json"][i]["sysnode_id"].asString();

			string uft_sysnode_id = root["json"][i]["uft_sysnode_id"].asString();
			if (uft_sysnode_id != "0" && uft_sysnode_id == "51")
			{
				SPtr_CData->sysnode_id = uft_sysnode_id;
			}
			if (root["json"][i]["asset_prop"].asString()[0] != '7')
			{
				MessageBox(NULL, "请使用信用账户登录", "登录异常", MB_OK);
				return false;
			}
			string s_ClientRights = root["json"][i]["client_rights"].asString();
			if (s_ClientRights.find("E") == string::npos)
			{
				/*没找到E(订阅权限直接退出)*/
				MessageBox(NULL, "缺少权限E(委托、成交推送权限)，请重新登陆", "登录异常", MB_OK);
				return false;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			ExceptionInterrupt;
			return false;//只有一个元素，而且要根据这个判断是否登录成功，因此一定要用return
		}
	}
	
	return true;
}

bool CT2Spi_Credit::OnQueryStockAccount(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 资金账户下没有股东账户是可能的，因此是正确的、除此之外还有查持仓、成交、委托返回NULL也是正常的*/
		return true;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			/*当T2的msg_id为0，如果找不到error_no的功能号，默认正确，如果找得到要确定其为0才代表正确，有些恒生接口正确的时候不返还error_no，只有在出错的时候才会返还*/
			CString market_code = "";
			CString exchange_type = root["json"][i]["exchange_type"].asString().c_str();
			switch (exchange_type[0]) 
			{
			case '1':
			{
				market_code = SSE;
				CAccount obj_CAccount;
				CString fund_account = root["json"][i]["fund_account"].asString().c_str();//资金账户
				obj_CAccount.stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户
				obj_CAccount.market_code = market_code;
				if (SPtr_CData->SetAccount(fund_account, obj_CAccount) == false)
				{
					LOG(ERROR) << "SetAccount fail";
					ExceptionInterrupt;
				}
				break;
			}
			case '2':
			{
				market_code = SZSE;
				CAccount obj_CAccount;
				CString fund_account = root["json"][i]["fund_account"].asString().c_str();//资金账户
				obj_CAccount.stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户
				obj_CAccount.market_code = market_code;
				if (SPtr_CData->SetAccount(fund_account, obj_CAccount) == false)
				{
					LOG(ERROR) << "SetAccount fail";
					ExceptionInterrupt;
				}
				break;
			}
			default:
				ErrorLog("abnormal exchange_type");
				break;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			ExceptionInterrupt;
			/*不用continue原因是只要有一个报错，就该认为查询股东账户成功，不发送对应信号，如果发送信号会隐藏问题，比如持仓，有一个错了就认定查询失败不刷新*/
			return false;
		}
	
	}
	
	return true;
}

bool CT2Spi_Credit::OnRevoke(const Value &root)
{
	//uft下通过entrust_no找委托单较为复杂，因此在撤单应答中不更新资金和持仓
	//if (root["json"].isNull())
	//{
	//	LOG(ERROR) << "OnRevoke no json";
	//	ExceptionInterrupt;
	//	return false;
	//}

	//int count = root["json"].size();
	//for (int i = 0; i < count; i++)
	//{
	//	if (tool::VerifyElement(root, i, SPtr_CData))
	//	{
	//		string stock_code;
	//		stock_code = root["json"][i]["stock_code"].asString().c_str();
	//		UpdateFund_Position(stock_code);
	//	}
	//	else
	//	{
	//		LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
	//		return false;//只有一个元素，而且要根据这个判断是否登录成功，因此一定要用return
	//	}
	//}
	return true;
}

/*Position*/
bool CT2Spi_Credit::QuerySinglePosition(const string &stock_code)
{
	string s_Fun335102;
	GainQuerySinglePosition(s_Fun335102, stock_code);

	if (tool::SendOrder(pCApiFun, Api_Asy,s_Fun335102,SPtr_CData) == false)
	{
		return false;
	}
	return true;
}

bool CT2Spi_Credit::GainQuerySinglePosition(string &s_str, const string &stock_code)
{
	/*持仓查询*/
	Json::Value obj_Value;
	obj_Value["function_id"] = "335102";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["stock_code"] = string(stock_code);
	obj_Value["asset_prop"] = AssetProp_信用账户;

	obj_Value["position_str"] = "";
	obj_Value["request_num"] = "1";
	s_str = obj_Value.toStyledString();
	return true;
}

bool CT2Spi_Credit::OnQuerySinglePosition(const Value &root)
{
	bool bBeginRedrawPosition = false;
	if (root["json"].isNull())
	{
		return true;
	}

	int count = root["json"].size();

	if (count >= SingleQuerySize)
	{
	}
	else
	{
		bBeginRedrawPosition = true;
	}
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CPosition obj_CPosition;

			CString fund_account = root["json"][i]["fund_account"].asString().c_str();//资金账户
			CString stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户

			obj_CPosition.stock_name = root["json"][i]["stock_name"].asString().c_str();//股票名称
			obj_CPosition.stock_code = root["json"][i]["stock_code"].asString().c_str();//股票代码
			obj_CPosition.current_amount = convert<UInt_64>(root["json"][i]["current_amount"].asString());//股票余额
			obj_CPosition.enable_amount = convert<UInt_64>(root["json"][i]["enable_amount"].asString());//可用数量
			obj_CPosition.market_value = convert<double>(root["json"][i]["market_value"].asString());//市值

			obj_CPosition.income_balance = convert<double>(root["json"][i]["income_balance"].asString());//盈亏
			obj_CPosition.cost_price = convert<double>(root["json"][i]["cost_price"].asString());//成本价
			obj_CPosition.last_price = convert<double>(root["json"][i]["last_price"].asString());//最新价
			obj_CPosition.profit_ratio = convert<double>(root["json"][i]["profit_ratio"].asString());

			if (SPtr_CData->SetPosition(fund_account, stock_account, obj_CPosition) == false)
			{
				ErrorLog("SetPosition fail");
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
				ErrorLog("PostMessage fail");
			}
		}
	}
	return true;
}

bool CT2Spi_Credit::OnQueryCreditAsset(const Value &root)
{
	if (root["json"].isNull())
	{
		return true;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			VecCreditAsset VCreditAsset;
			VCreditAsset.resize(Size_Row);
			InitVCreditAsset(VCreditAsset);
			FillVCreditAsset(VCreditAsset,root,i);
			
			if (SPtr_CData->SetVCreditAsset(SPtr_CData->fund_account, VCreditAsset) == false)
			{
				ErrorLog("SetVCreditAsset fail");
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

	if (PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawCreditAsset, 0, 0) == 0)
	{
		ErrorLog("PostMessage fail");
	}
	
	return true;
}

void CT2Spi_Credit::InitVCreditAsset(VecCreditAsset &VCreditAsset)
{
	VCreditAsset.resize(Size_Row);
	for (unsigned int i = 0; i < VCreditAsset.size(); i++)
		VCreditAsset[i].resize(Siz_Column);

	VCreditAsset[0][0] = MaintenanceGuaranteeRatio;
	VCreditAsset[0][2] = "融资可用额度";
	VCreditAsset[0][4] = "融券可用额度";

	VCreditAsset[1][0] = "可用保证金";
	VCreditAsset[1][2] = "融资已用额度";
	VCreditAsset[1][4] = "融券已用额度";

	VCreditAsset[2][0] = "负债总浮盈";
	VCreditAsset[2][2] = "融资合约金额";
	VCreditAsset[2][4] = "融券合约金额";

	VCreditAsset[3][0] = "已用保证金";
	VCreditAsset[3][2] = "融资合约费用";
	VCreditAsset[3][4] = "融券合约费用";

	VCreditAsset[4][0] = "买担保品可用资金";
	VCreditAsset[4][2] = "融资合约利息";
	VCreditAsset[4][4] = "融券合约利息";

	VCreditAsset[5][0] = "现金还款可用资金";
	VCreditAsset[5][2] = "融资市值";
	VCreditAsset[5][4] = "融券市值";

	VCreditAsset[6][0] = "买融资标的可用资金";
	VCreditAsset[6][2] = "融资合约盈亏";
	VCreditAsset[6][4] = "融券合约盈亏";

	VCreditAsset[7][0] = "证券市值";
	VCreditAsset[7][2] = "融资已用保证金";
	VCreditAsset[7][4] = "融券已用保证金";

	VCreditAsset[8][0] = "可取金额";
	VCreditAsset[8][2] = "现金资产";
	VCreditAsset[8][4] = "融券卖出所得总额";

	VCreditAsset[9][0] = "融券额度上限";
	VCreditAsset[9][2] = "担保资产";
	VCreditAsset[9][4] = "负债总额";

	VCreditAsset[10][0] = "";
	VCreditAsset[10][2] = "其他费用";
	VCreditAsset[10][4] = "融资额度上限";

	VCreditAsset[11][0] = "";
	VCreditAsset[11][2] = "净资产";
	VCreditAsset[11][4] = "买券还券可用资金";
}

string CT2Spi_Credit::GainPer_assurescale_value(const Value &root, const int i)
{
	//fin_income + slo_income
	string per_assurescale_value;
	string s_str = root["json"][i]["fin_income"].asString().c_str();
	double fin_income = convert<double>(s_str);
	s_str = root["json"][i]["slo_income"].asString().c_str();
	double slo_income = convert<double>(s_str);
	per_assurescale_value = to_string(fin_income + slo_income);
	return per_assurescale_value;
}

bool CT2Spi_Credit::GainFetch_balance(string &fetch_balance)
{
    string s_Fun332255;
	tool::GainQueryFund332255(s_Fun332255, SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun, Api_Syn, s_Fun332255, SPtr_CData);
	if (RefId_HS < 0)
	{
		ErrorLog("abnormal RefId_HS");
		return false;
	}
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
		return false;
	}
	fetch_balance = root["json"][0u]["fetch_balance"].asString();
	delete[] pRespData;
	return true;
}

void CT2Spi_Credit::FillVCreditAsset(VecCreditAsset &VCreditAsset, const Value &root, const int i)
{
	VCreditAsset[0][1] = root["json"][i]["per_assurescale_value"].asString();//维持担保比例 N16.4
	VCreditAsset[0][3] = root["json"][i]["fin_enable_quota"].asString();
	VCreditAsset[0][5] = root["json"][i]["slo_enable_quota"].asString();

	VCreditAsset[1][1] = root["json"][i]["enable_bail_balance"].asString();
	VCreditAsset[1][3] = root["json"][i]["fin_used_quota"].asString();
	VCreditAsset[1][5] = root["json"][i]["slo_used_quota"].asString();
	
	VCreditAsset[2][1] = GainPer_assurescale_value(root, i); //fin_income + slo_income
	VCreditAsset[2][3] = root["json"][i]["fin_compact_balance"].asString();
	VCreditAsset[2][5] = root["json"][i]["slo_compact_balance"].asString();

	VCreditAsset[3][1] = root["json"][i]["used_bail_balance"].asString();
	VCreditAsset[3][3] = root["json"][i]["fin_compact_fare"].asString();
	VCreditAsset[3][5] = root["json"][i]["slo_compact_fare"].asString();

	VCreditAsset[4][1] = root["json"][i]["assure_enbuy_balance"].asString();
	VCreditAsset[4][3] = root["json"][i]["fin_compact_interest"].asString();
	VCreditAsset[4][5] = root["json"][i]["slo_compact_interest"].asString();

	VCreditAsset[5][1] = root["json"][i]["fin_enrepaid_balance"].asString();
	VCreditAsset[5][3] = root["json"][i]["fin_market_value"].asString();
	VCreditAsset[5][5] = root["json"][i]["slo_market_value"].asString();

	VCreditAsset[6][1] = root["json"][i]["fin_enbuy_balance"].asString();
	VCreditAsset[6][3] = root["json"][i]["fin_income"].asString();
	VCreditAsset[6][5] = root["json"][i]["slo_income"].asString();

	VCreditAsset[7][1] = root["json"][i]["market_value"].asString();
	VCreditAsset[7][3] = root["json"][i]["fin_used_bail"].asString();
	VCreditAsset[7][5] = root["json"][i]["slo_used_bail"].asString();

	string fetch_balance;
	GainFetch_balance(fetch_balance);
	VCreditAsset[8][1] = fetch_balance;//获取可取资金
	VCreditAsset[8][3] = root["json"][i]["cash_asset"].asString();
	VCreditAsset[8][5] = root["json"][i]["slo_sell_balance"].asString();

	VCreditAsset[9][1] = root["json"][i]["slo_max_quota"].asString();
	VCreditAsset[9][3] = root["json"][i]["assure_asset"].asString();
	VCreditAsset[9][5] = root["json"][i]["total_debit"].asString();

	VCreditAsset[10][1] = "";
	VCreditAsset[10][3] = root["json"][i]["other_fare"].asString();
	VCreditAsset[10][5] = root["json"][i]["fin_max_quota"].asString();

	VCreditAsset[11][1] = "";
	VCreditAsset[11][3] = root["json"][i]["net_asset"].asString();
	VCreditAsset[11][5] = root["json"][i]["slo_enrepaid_balance"].asString();
}

string CT2Spi_Credit::Modifystring(string s_str)
{
	string s_dst;
	double d_tmp;
	d_tmp = convert<double>(s_str);
	d_tmp = tool::round(d_tmp, 2);

	return s_dst;
}

/*fund*/
bool CT2Spi_Credit::OnQueryFund(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 查询资金为空程序异常*/
		LOG(ERROR) << "OnQueryFund no json";
		ExceptionInterrupt;
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CFund obj_CFund;
			obj_CFund.fund_account = SPtr_CData->fund_account;
			//当前余额 可用资金 冻结资金 解冻资金 回报买入金额 回报卖出金额
			obj_CFund.current_balance = convert<double>(root["json"][i]["current_balance"].asString().c_str());//当前余额
			obj_CFund.enable_balance = convert<double>(root["json"][i]["enable_balance"].asString().c_str());//可用资金
			obj_CFund.frozen_balance = convert<double>(root["json"][i]["frozen_balance"].asString().c_str());//冻结资金
			obj_CFund.unfrozen_balance = convert<double>(root["json"][i]["unfrozen_balance"].asString().c_str());//解冻资金
			obj_CFund.real_buy_balance = convert<double>(root["json"][i]["real_buy_balance"].asString().c_str());//回报买入金额
			obj_CFund.real_sell_balance = convert<double>(root["json"][i]["real_sell_balance"].asString().c_str());//回报卖出金额
			if (SPtr_CData->SetFund(obj_CFund) == false)
			{
				LOG(ERROR) << "SetFund fail";
				ExceptionInterrupt;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			ExceptionInterrupt;
			return false;
		}
	}
	return true;
}

bool CT2Spi_Credit::QueryFund()
{
	string s_Fun335101;
	tool::GainQueryFund335101(s_Fun335101,SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun,Api_Asy, s_Fun335101, SPtr_CData);
	if (RefId_HS < 0)
	{
		LOG(ERROR) << "abnormal RefId_HS";
		return false;
	}
	SPtr_CData->SetRefId_T2(RefId_HS, RefType_335101_NoFirstQueryFund);
	return true;
}

bool CT2Spi_Credit::QueryCreditAsset()
{
	string s_Fun335504;
	tool::GainQueryCreditAsset335504(s_Fun335504, SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun, Api_Asy, s_Fun335504, SPtr_CData);
	if (RefId_HS < 0)
	{
		ErrorLog("abnormal RefId_HS");
		return false;
	}
	return true;
}

void CT2Spi_Credit::updateEntrust_status(COrder &obj_COrder, const string &entrust_status,string cancel_info)
{
	switch (entrust_status[0])
	{
	case '0'://未报
	{
		obj_COrder.entrust_status = WeiBao;
		break;
	}
	case '1'://待报
	{
		obj_COrder.entrust_status = DaiBao;
		break;
	}
	case '2'://已报
	{
		obj_COrder.entrust_status = YiBao;
		break;
	}
	case '3'://已报待撤
	{
		obj_COrder.entrust_status = YiBaoDaiChe;
		break;
	}
	case '4'://部成待撤
	{
		obj_COrder.entrust_status = BuChengDaiChe;
		break;
	}
	case '5'://部撤
	{
		obj_COrder.entrust_status = BuChe;
		break;
	}
	case '6'://已撤
	{
		obj_COrder.entrust_status = YiChe;
		break;
	}
	case '7'://部成
	{
		obj_COrder.entrust_status = BuCheng;
		break;
	}
	case '8'://已成
	{
		obj_COrder.entrust_status = YiCheng;
		break;
	}
	case '9'://废单
	{
		obj_COrder.entrust_status = FeiDan;
		LOG(ERROR) << "废单原因:cancel_info:" << cancel_info<< " entrust_no"<<obj_COrder.entrust_no;
		break;
	}
	default:
		break;
	}
}

void CT2Spi_Credit::updateEntrust_bs(COrder &obj_COrder, const string &entrust_bs)
{
	switch (entrust_bs[0])
	{
	case '1':
	{
		obj_COrder.entrust_bs = _Buy;
		break;
	}
	case '2':
	{
		obj_COrder.entrust_bs = _Sell;
		break;
	}
	default:
		break;
	}
}


void CT2Spi_Credit::updateEntrust_bs(CDeal &obj_CDeal, const string &entrust_bs)
{
	switch (entrust_bs[0])
	{
	case '1':
	{
		obj_CDeal.entrust_bs = _Buy;
		break;
	}
	case '2':
	{
		obj_CDeal.entrust_bs = _Sell;
		break;
	}
	default:
		break;
	}
}

void CT2Spi_Credit::updateEntrust_type(COrder &obj_COrder, const string &entrust_type)
{
	switch (entrust_type[0])
	{
	case '6':
	{
		obj_COrder.entrust_type = enumXinYongRongZi;
		break;
	}
	case '9':
	{
		obj_COrder.entrust_type = enumXinYongJiaoYi;
		break;
	}
	default:
		ErrorLog("abnormal entrust_type");
		break;
	}
}

bool CT2Spi_Credit::OnSendOrder(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 false退出即可*/
		ErrorLog("OnSendOrder no json");
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			/*委托发送返还*/
			CString stock_account;
			COrder obj_COrder;
			
			Type_OrderList::iterator iter_RawOrder = SPtr_CData->M_OrderList.find(convert<int>(root["ref_id"].asString()));
			if (SPtr_CData->M_OrderList.end() == iter_RawOrder)
			{
				ErrorLog("abanormal ref_id");
			}
			obj_COrder.stock_name = iter_RawOrder->second.stock_name;
			obj_COrder.stock_code = iter_RawOrder->second.stock_code;
			obj_COrder.entrust_bs = iter_RawOrder->second.entrust_bs;
			obj_COrder.entrust_price = iter_RawOrder->second.entrust_price;
			obj_COrder.entrust_amount = iter_RawOrder->second.entrust_amount;

			obj_COrder.entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());//委托编号
			if (VerifyEntrust_no(obj_COrder.entrust_no) == false)
			{
				return false;
			}
			if (SPtr_CData->SecCode2StockAccount(obj_COrder.stock_code, stock_account) == false)
			{
				ErrorLog("SecCode2StockAccount fail");
				return false;
			}

			tool::WriteCheckList(SPtr_CData,root, i, stock_account);
			/*先判断单子是否存在，存在就什么都不用做，在委托推送里已经用申报时间更新过了*/
			if (VerifybExist(stock_account, obj_COrder.entrust_no) == true)
			{
				return true;
			}
			if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
			{
				ErrorLog("SetOrder return fail");
				return false;
			}
			UpdateFund_Position(string(obj_COrder.stock_code));
		}
		else
		{
			LOG(ERROR) << "function id:" << root["json"][i]["function_id"].asString() << " a element illegal";
			return false;
		}
	}

	bPostMessage = true;
	return true;
}


void CT2Spi_Credit::OnRecvJsonMsg(char* pRespdata)
{
	/*报文的合法性校验分为三步：首先判断是否成功转发(msg_id为0),其次判断是否有数据，没有数据没有json字段
	如果有数据则通过error_no判断是否合法，先找 找不到合法，找得到为0合法*/
	pRsp = pRespdata;
	DealQuit(SPtr_CData->bBeginRelease);
	Json::Reader reader;
	Json::Value root;
	
	if (!reader.parse(string(pRespdata), root, false))
	{
		return;
	}
	
	int function_id = convert<int>(root["function_id"].asString());
	switch (function_id)
	{
	case 331100://T2登陆
	{
		if (OnLogin(root) == true)
		{
			SetEvent(SPtr_CData->LoginT2_Event);
		}
		break;
	}
	case 331300://证券股东信息查询
	{
		if (OnQueryStockAccount(root) == true)
		{
			SetEvent(SPtr_CData->QueryStockAccount_Event);
		}
		else
		{
			LOG(ERROR) << "warning:OnQueryStockAccount return fail in OnReceivedBiz";
			ExceptionInterrupt;
		}
		break;
	}
	case 335002://委托下单返还
	{
		if (SPtr_CData->TestSpeed == 1)
		{
			GetSystemTime(&(SPtr_CData->systime_OnSendOrder));
			LOG(ERROR) << "Recv Rsp of SendOrder consume time:" << SPtr_CData->systime_OnSendOrder.wSecond * 1000 + SPtr_CData->systime_OnSendOrder.wMilliseconds
				- (SPtr_CData->systime_SendOrder.wSecond * 1000 + SPtr_CData->systime_SendOrder.wMilliseconds) << " 毫秒";
		}
		OnSendOrder(root);
		break;
	}
	case 335004://委托撤单返还
	{
		OnRevoke(root);
		break;
	}
	case 335101://资金账户查询
	{
		/*考虑到如果返回就报错，没必要重新启动核心线程，刷新一遍结果*/
		if (OnQueryFund(root) == true)
		{
			int ref_id = convert<int>(root["ref_id"].asString());
			int MyRefId;
			if (SPtr_CData->GetEraseRefId_T2(ref_id, MyRefId) == false)
			{
				return;
			}
			switch (MyRefId)
			{
			case RefType_335101_InitFund:
			{
				SetEvent(SPtr_CData->InitFund_Event);
				break;
			}
			default:
				break;
			}
			if (SPtr_CData->hwnd_TradeDlg == NULL)
			{
				SPtr_CData->Queue_WmMessage.push(WM_RedrawFund);
			}
			else if (PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawFund, 0, 0) == 0)
			{
				ErrorLog("PostMessage fail");
			}
		}
		break;
	}
	case 335102://单个持仓查询
	{
		OnQuerySinglePosition(root);
		break;
	}
	case 335504:
		OnQueryCreditAsset(root);
		break;
	case 620001:
		On620001(root);
		break;
	case 620003://订阅(委托、成交)推送
	{
		/*fixing 融资融券生产环境12:55推送消失（快速交易正常），更新为未报，重新登录更新为待报，如果不重新登录就会导致到1点了因为是待报无法撤单*/
		LOG(INFO) << pRsp;/*fixing*/
		OnSubscribe(root);
		break;
	}
	default:
		break;
	}
	return;
}
