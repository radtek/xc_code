#include "stdafx.h"
#include "Login_Credit.h"

bool CLogin_Credit::initOrderDemo_Credit()
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335103;
			GainQuerySingleBatchOrder335103_Credit(s_Fun335103, position_str);
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun335103, SPtr_CData) < 0)
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
				/*fixing*/
				LOG(INFO) << pRespData;
 				OnQueryOrder_Credit(root, position_str, bContinuousQuery);
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

bool CLogin_Credit::GainQuerySingleBatchOrder335103_Credit(string &s_str, const string &position_str)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335103";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["report_no"] = "0";
	obj_Value["action_in"] = "0";

	obj_Value["asset_prop"] = AssetProp_信用账户;
	obj_Value["query_type"] = "1";
	obj_Value["position_str"] = position_str;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	obj_Value["query_mode"] = "0";
	s_str = obj_Value.toStyledString();
	return true;
}

bool CLogin_Credit::OnQueryOrder_Credit(const Value &root, string &position_str, bool &bContinuousQuery)
{
	bool bBeginRedrawOrder = false;
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
		bBeginRedrawOrder = true;
	}

	for (int i = 0; i < count; i++)
	{
		if (Inspect_bStock(root, i) == false)
		{
			continue;
		}
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			InsertOrder_Credit(root, i, count);
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			ExceptionInterrupt;
			return false;
		}
	}

	if (true == bBeginRedrawOrder)
	{
		if (SPtr_CData->hwnd_TradeDlg == NULL)
		{
			SPtr_CData->Queue_WmMessage.push(WM_RedrawOrder);
		}
		else if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawOrder, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
		}
	}
	return true;
}

bool CLogin_Credit::InsertOrder_Credit(const Value &root, const int i, const int count)
{
	//股票名称 股票代码 方向 委托类别 委托价格 委托数量 成交数量 委托状态 委托号
	CString stock_account;
	COrder obj_COrder;
	stock_account = root["json"][i]["stock_account"].asString().c_str();

	obj_COrder.stock_code = root["json"][i]["stock_code"].asString().c_str();//证券代码
	obj_COrder.stock_name = root["json"][i]["stock_name"].asString().c_str();//证券名称
	switch (root["json"][i]["entrust_bs"].asString()[0])
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
	switch (root["json"][i]["entrust_type"].asString()[0])
	{
	case '6':
	{
		obj_COrder.entrust_type = enumXinYongRongZi;
		break;
	}
	case '7':
	{
		obj_COrder.entrust_type = enumXinYongRongQuan;
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
	obj_COrder.entrust_price = convert<double>(root["json"][i]["entrust_price"].asString());//委托价格
	obj_COrder.entrust_amount = convert<UInt_64>(root["json"][i]["entrust_amount"].asString());//委托数量
	obj_COrder.business_amount = convert<UInt_64>(root["json"][i]["business_amount"].asString());//成交数量

	switch (root["json"][i]["entrust_status"].asString()[0])
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
		break;
	}
	default:
		break;
	}
	obj_COrder.entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());//委托编号
	obj_COrder.entrust_time = convert<UInt_32>(root["json"][i]["entrust_time"].asString());//申报时间

	if (0 == obj_COrder.entrust_no)
	{
		LOG(ERROR) << "entrust_no is 0";
		ExceptionInterrupt;
	}
	tool::WriteCheckList(SPtr_CData,root, i, stock_account);
	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
	}
	return true;
}


bool CLogin_Credit::initDealDemo_Credit()
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335104;
			GainQuerySingleBatchDeal335104_Credit(s_Fun335104, position_str);
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun335104,SPtr_CData) < 0)
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
				
				OnQueryDeal_Credit(root, position_str, bContinuousQuery);
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

bool CLogin_Credit::GainQuerySingleBatchDeal335104_Credit(string &s_str, const string &position_str)
{
	//证券成交查询
	Json::Value obj_Value;
	obj_Value["function_id"] = "335104";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["report_no"] = "0";
	obj_Value["asset_prop"] = AssetProp_信用账户;

	obj_Value["query_type"] = "1";
	obj_Value["query_mode"] = "0";
	obj_Value["position_str"] = position_str;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	s_str = obj_Value.toStyledString();

	return true;
}

bool CLogin_Credit::OnQueryDeal_Credit(const Value &root, string &position_str, bool &bContinuousQuery)
{
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
	}
	for (int i = 0; i < count; i++)
	{
		if (Inspect_bStock(root, i) == false)
		{
			continue;
		}
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			//股票名称 股票代码 方向 成交数量 成交价格 成交时间 成交编号 委托号 
			CString stock_account;
			CString MarketCode;
			CDeal obj_CDeal;

			stock_account = root["json"][i]["stock_account"].asString().c_str();
			obj_CDeal.stock_code = root["json"][i]["stock_code"].asString().c_str();//证券代码
			obj_CDeal.stock_name = root["json"][i]["stock_name"].asString().c_str();//证券名称

																					/*买卖方向*/
			switch (root["json"][i]["entrust_bs"].asString()[0])
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
				LOG(ERROR) << "abnoraml entrust_bs in OnQueryDeal";
				ExceptionInterrupt;
				break;
			}
			/*成交数量*/
			obj_CDeal.business_amount = convert<UInt_64>(root["json"][i]["business_amount"].asString());
			/*成交价格*/
			obj_CDeal.business_price = convert<double>(root["json"][i]["business_price"].asString());
			/*成交时间*/
			obj_CDeal.business_time = convert<UInt_32>(root["json"][i]["business_time"].asString());
			/*成交编号*/
			obj_CDeal.business_id = root["json"][i]["business_id"].asString().c_str();
			/*委托编号*/
			obj_CDeal.entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());
			/*成交金额，为了成交汇总*/
			obj_CDeal.business_balance = convert<double>(root["json"][i]["business_balance"].asString());
			if (obj_CDeal.entrust_no == 0)
			{
				LOG(ERROR) << "entrust_no is 0 in OnQueryDeal";
				ExceptionInterrupt;
				return false;
			}
			obj_CDeal.business_balance = convert<double>(root["json"][i]["business_balance"].asString());
			if (SPtr_CData->SetDeal(SPtr_CData->fund_account, stock_account, obj_CDeal.entrust_no, obj_CDeal) == false)
			{
				LOG(ERROR) << "SetDeal return fail";
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
	return true;
}



bool CLogin_Credit::initBiaoDiQuanMap()
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335502;
			GainSingleBatchBiaoDiQuan335502(s_Fun335502, position_str);
			/*SYSTEMTIME sys_time;
			GetLocalTime(&sys_time);
			cerr << "Start query BiaoDiQuan " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;*/
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun335502,
				SPtr_CData) < 0)
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

			/*	SYSTEMTIME sys_time;
				GetLocalTime(&sys_time);
				cerr << "end query BiaoDiQuan " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;*/

				Json::Reader reader;
				Json::Value root;

				if (!reader.parse(string(pRespData), root, false))
				{
					ExceptionInterrupt;
					LOG(ERROR) << "parse fail";
					continue;
				}
				
				OnQueryBiaoDiQuan(root,  position_str, bContinuousQuery);
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

void CLogin_Credit::GainSingleBatchBiaoDiQuan335502(string &s_str, const string position_str)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335502";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["request_num"] = SingleQuerySize;
	obj_Value["position_str"] = position_str;
	s_str = obj_Value.toStyledString();
}

bool CLogin_Credit::OnQueryBiaoDiQuan(const Value &root, string &position_str, bool &bContinuousQuery)
{
	bool bBeginRedrawBiaoDiQuan = false;//确保只有最后一次才发生重绘
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
		bBeginRedrawBiaoDiQuan = true;
	}
	/*要过滤掉非股票*/
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			/*客户端依次是证券名称 证券代码 融资保证金比例 担保折算率 融资状态*/
			CSecurity obj_CSecurity;
			obj_CSecurity.stock_code = root["json"][i]["stock_code"].asString().c_str();
			char c_stock_type = root["json"][i]["stock_type"].asString()[0];
			if (c_stock_type != '0' && c_stock_type != 'c')
			{
				continue;
			}

			obj_CSecurity.stock_name = root["json"][i]["stock_name"].asString().c_str();
			obj_CSecurity.fin_ratio = convert<double>(root["json"][i]["fin_ratio"].asString());
			obj_CSecurity.assure_ratio = convert<double>(root["json"][i]["assure_ratio"].asString());
			if (root["json"][i]["fin_status"].asString()[0] != '0')
			{
				continue;
			}
			obj_CSecurity.fin_status = root["json"][i]["fin_status"].asString()[0];
			char exchange_type = root["json"][i]["exchange_type"].asString()[0];
			CString StockAccount;
			CString MarketCode;
			switch (exchange_type)
			{
			case '1':
				MarketCode = "SSE";
				break;
			case '2':
				MarketCode = "SZSE";
				break;
			default:
				break;
			}

			if (SPtr_CData->Market2Account(SPtr_CData->fund_account, MarketCode, StockAccount) == false)
			{
				LOG(ERROR) << "Market2Account fail";
				ExceptionInterrupt;
				return false;
			}
			if (SPtr_CData->SetSecurity_BiaoDi(SPtr_CData->fund_account, StockAccount, obj_CSecurity) == false)
			{
				LOG(ERROR) << "SetSecurity return fail";
				ExceptionInterrupt;
				return false;
			}
			
			CSecurity obj_CSecurity2;
			if (SPtr_CData->GetSecurity(MarketCode, obj_CSecurity.stock_code, obj_CSecurity2) == false)
			{
			/*	LOG(ERROR) << "GetSecurity fail";
				ExceptionInterrupt; fixing*/
				continue;;
			}
			obj_CSecurity2.s_bBiaoDiQuan = "标的券";
			if (SPtr_CData->SetSecurity(MarketCode, obj_CSecurity2) == false)
			{
				LOG(ERROR) << "SetSecurity fail";
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

	if (true == bBeginRedrawBiaoDiQuan)
	{
		if (SPtr_CData->hwnd_TradeDlg == NULL)
		{
			SPtr_CData->Queue_WmMessage.push(WM_RedrawBiaoDiQuan);
		}
		else if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawBiaoDiQuan, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
		}
	}
	return true;
}

bool CLogin_Credit::initDanBaoQuanMap()
{
	/*上海市场担保券*/
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	cerr << "Start query SSE danbaoquan " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;
	std::thread t_SSE_Syn(thread_SSE_Syn, pCApiFun, SPtr_CData);
	t_SSE_Syn.detach();
	/*深圳市场担保券*/
	std::thread t_SZSE_Syn(thread_SZSE_Syn, pCApiFun, SPtr_CData);
	t_SZSE_Syn.detach();
	return true;
}

void CLogin_Credit::thread_SSE_Syn(CApiFun *pCApiFun, PtrData SPtr_CData)
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335501;
			GainSingleBatchDanBaoQuan335501_SSE(s_Fun335501, SPtr_CData, position_str);

			if (tool::SendOrder(pCApiFun, Api_Syn_SSE, s_Fun335501,SPtr_CData) < 0)
			{
				bRun = false;
				SPtr_CData->bEndQueryDanBaoQuan_SSE = true;
				continue;
			}
			else
			{
				int ret = pCApiFun->Api_T2_SSE_Syn->RecvMsg();
				if (ret < 0)
				{
					CString s_ErrorInfo = pCApiFun->Api_T2_SSE_Syn->GetLastErrorMsg();
					ErrorLog(s_ErrorInfo);
					AfxMessageBox(s_ErrorInfo);
					return;
				}
				char* pRespData = new char[ret + 1];
				ret = pCApiFun->Api_T2_SSE_Syn->GetJsonValue(pRespData);

				Json::Reader reader;
				Json::Value root;

				if (!reader.parse(string(pRespData), root, false))
				{
					ExceptionInterrupt;
					LOG(ERROR) << "parse fail";
					continue;
				}
				
				OnQueryDanBaoQuan_SSE(root, SPtr_CData, position_str, bContinuousQuery);
				delete[] pRespData;
			}
		}
		else
		{
			bRun = false;
			SYSTEMTIME sys_time;
			GetLocalTime(&sys_time);
			cerr << "OnQueryDanBaoQuan_SSE " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;
		}
	}
}

void CLogin_Credit::GainSingleBatchDanBaoQuan335501_SSE(string &s_str, PtrData SPtr_CData, const string position_str)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335501";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["exchange_type"] = exchange_type_SSE;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);

	obj_Value["position_str"] = position_str;
	s_str = obj_Value.toStyledString();
}

bool CLogin_Credit::OnQueryDanBaoQuan_SSE(const Value &root, PtrData SPtr_CData, string &position_str, bool &bContinuousQuery)
{
	bool bEndQueryDanBaoQuan_SSE = false;
	/*SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	cerr << "OnQueryDanBaoQuan_SSE " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;*/
	/*static int kkk = 0;
	cerr << "kkk in OnQueryDanBaoQuan_SSE:" << kkk << endl;
	kkk++;*/
	if (root["json"].isNull())
	{
		bEndQueryDanBaoQuan_SSE = true;
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
		bEndQueryDanBaoQuan_SSE = true;
		bContinuousQuery = false;
	}
	/*要过滤掉非股票 不正常的要continue掉*/
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i,SPtr_CData) == true)
		{
			//股票名称 股票代码 担保折算率 担保状态 
			CSecurity obj_CSecurity;
			CString MarketCode;
			/*由于没有stock_type因此只能采用去查找全市场代码的方式去过滤，因此必须要让全市场代码先初始化完毕才可以执行这个操作*/
			obj_CSecurity.stock_name = root["json"][i]["stock_name"].asString().c_str();
			if ('0' != root["json"][i]["assure_status"].asString()[0])
			{
				continue;
			}
			obj_CSecurity.assure_status = root["json"][i]["assure_status"].asString()[0];
			obj_CSecurity.stock_code = root["json"][i]["stock_code"].asString().c_str();
			switch (root["json"][i]["exchange_type"].asString()[0])
			{
			case '1':
				MarketCode = "SSE";
				break;
			default:
				LOG(ERROR) << "abnoraml exchange_type";
				break;
			}
			if (false == SPtr_CData->inspectbStock(MarketCode, obj_CSecurity.stock_code))
			{
				continue;
			}

			obj_CSecurity.assure_ratio = convert<double>(root["json"][i]["assure_ratio"].asString());

			CString StockAccount;
			if (SPtr_CData->Market2Account(SPtr_CData->fund_account, MarketCode, StockAccount) == false)
			{
				LOG(ERROR) << "Market2Account fail";
				ExceptionInterrupt;
				return false;
			}
			if (SPtr_CData->SetSecurity_DanBao(SPtr_CData->fund_account, StockAccount, obj_CSecurity) == false)
			{
				LOG(ERROR) << "SetSecurity return fail";
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

	if (true == bEndQueryDanBaoQuan_SSE)
	{
		SPtr_CData->bEndQueryDanBaoQuan_SSE = true;
		if ((true == SPtr_CData->bEndQueryDanBaoQuan_SSE) && (true == SPtr_CData->bEndQueryDanBaoQuan_SZSE))
		{
			if (SPtr_CData->hwnd_TradeDlg == NULL)
			{
				SPtr_CData->Queue_WmMessage.push(WM_RedrawDanBaoQuan);
				SetEvent(SPtr_CData->QueryDanBaoQuanMap_Event);	
			}
			else if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawDanBaoQuan, 0, 0) == 0)
			{
				LOG(ERROR) << "PostMessage fail";
				ExceptionInterrupt;
			}
		}
	}
	return true;
}


void CLogin_Credit::thread_SZSE_Syn(CApiFun *pCApiFun, PtrData SPtr_CData)
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun335501;
			GainSingleBatchDanBaoQuan335501_SZSE(s_Fun335501, SPtr_CData, position_str);
			/*SYSTEMTIME sys_time;
			GetLocalTime(&sys_time);
			cerr << "Start query SZSE danbaoquan " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;*/
			if (tool::SendOrder(pCApiFun, Api_Syn_SZSE, s_Fun335501,SPtr_CData) < 0)
			{
				bRun = false;
				continue;
			}
			else
			{
				int ret = pCApiFun->Api_T2_SZSE_Syn->RecvMsg();
				if (ret < 0)
				{
					ErrorLog(pCApiFun->Api_T2_SZSE_Syn->GetLastErrorMsg());
					tool::ExceptionHandle_RecvMsg();
					return;
				}
				char* pRespData = new char[ret + 1];
				ret = pCApiFun->Api_T2_SZSE_Syn->GetJsonValue(pRespData);

				Json::Reader reader;
				Json::Value root;

				if (!reader.parse(string(pRespData), root, false))
				{
					LOG(ERROR) << "parse error";
					ExceptionInterrupt;
					continue;
				}
				
				OnQueryDanBaoQuan_SZSE(root, SPtr_CData, position_str, bContinuousQuery);
				delete[] pRespData;
			}
		}
		else
		{
			bRun = false;
		}
	}
}

void CLogin_Credit::GainSingleBatchDanBaoQuan335501_SZSE(string &s_str, PtrData SPtr_CData, const string position_str)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335501";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["exchange_type"] = exchange_type_SZSE;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);

	obj_Value["position_str"] = position_str;
	s_str = obj_Value.toStyledString();
}

bool CLogin_Credit::OnQueryDanBaoQuan_SZSE(const Value &root, PtrData SPtr_CData, string &position_str, bool &bContinuousQuery)
{
	bool bEndQueryDanBaoQuan_SZSE = false;
	/*SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	cerr << "OnQueryDanBaoQuan_SZSE " << sys_time.wMinute << ":" << sys_time.wSecond << ":" << sys_time.wMilliseconds << endl;
	static int kkk = 0;
	cerr << "kkk in OnQueryDanBaoQuan_SZSE:" << kkk << endl;
	kkk++;*/
	if (root["json"].isNull())
	{
		bEndQueryDanBaoQuan_SZSE = true;
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
		bEndQueryDanBaoQuan_SZSE = true;
		bContinuousQuery = false;
	}
	/*要过滤掉非股票 不正常的要continue掉*/
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			//股票名称 股票代码 担保折算率 担保状态 
			CSecurity obj_CSecurity;
			CString MarketCode;
			/*由于没有stock_type因此只能采用去查找全市场代码的方式去过滤，因此必须要让全市场代码先初始化完毕才可以执行这个操作*/
			obj_CSecurity.stock_name = root["json"][i]["stock_name"].asString().c_str();
			if ('0' != root["json"][i]["assure_status"].asString()[0])
			{
				continue;
			}
			obj_CSecurity.assure_status = root["json"][i]["assure_status"].asString()[0];
			obj_CSecurity.stock_code = root["json"][i]["stock_code"].asString().c_str();
	
			switch (root["json"][i]["exchange_type"].asString()[0])
			{
			case '2':
				MarketCode = "SZSE";
				break;
			default:
				LOG(ERROR) << "abnoraml exchange_type";
				break;
			}
			if (false == SPtr_CData->inspectbStock(MarketCode, obj_CSecurity.stock_code))
			{
				continue;
			}

			obj_CSecurity.assure_ratio = convert<double>(root["json"][i]["assure_ratio"].asString());

			CString StockAccount;
			if (SPtr_CData->Market2Account(SPtr_CData->fund_account, MarketCode, StockAccount) == false)
			{
				ExceptionInterrupt;
				LOG(ERROR) << "Market2Account fail";
				return false;
			}
			if (SPtr_CData->SetSecurity_DanBao(SPtr_CData->fund_account, StockAccount, obj_CSecurity) == false)
			{
				ExceptionInterrupt;
				LOG(ERROR) << "SetSecurity return fail";
				return false;
			}
		}
		else
		{
			ExceptionInterrupt;
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;
		}
	}

	if (true == bEndQueryDanBaoQuan_SZSE)
	{
		SPtr_CData->bEndQueryDanBaoQuan_SZSE = true;
		if ((true == SPtr_CData->bEndQueryDanBaoQuan_SSE) && (true == SPtr_CData->bEndQueryDanBaoQuan_SZSE))
		{
			if (SPtr_CData->hwnd_TradeDlg == NULL)
			{
				SPtr_CData->Queue_WmMessage.push(WM_RedrawDanBaoQuan);
				SetEvent(SPtr_CData->QueryDanBaoQuanMap_Event);
			}
			else if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawDanBaoQuan, 0, 0) == 0)
			{
				LOG(ERROR) << "PostMessage fail";
				ExceptionInterrupt;
			}
		}
	}
	return true;
}