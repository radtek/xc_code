#include "stdafx.h"
#include "Login_Fast.h"

bool CLogin_Fast::initPosition_Fast()
{
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun333104;
			GainQuerySingleBatchPosition333104_Fast(s_Fun333104, position_str);
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun333104, 
				SPtr_CData) < 0)
			{
				LOG(ERROR) << "T2ApiSendBiz_Syn fail";
				ExceptionInterrupt;
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
				OnQueryPosition_Fast(root, position_str, bContinuousQuery);
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

bool CLogin_Fast::GainQuerySingleBatchPosition333104_Fast(string &s_str, const string &position_str)
{
	/*持仓查询*/
	Json::Value obj_Value;
	obj_Value["function_id"] = "333104";
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	obj_Value["position_str"] = position_str;
	s_str = obj_Value.toStyledString();
	return true;
}

bool CLogin_Fast::OnQueryPosition_Fast(const Value &root, string &position_str, bool &bContinuousQuery)
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
			obj_CPosition.current_amount = convert<UInt_64>(root["json"][i]["current_amount"].asString());//股票余额
			obj_CPosition.enable_amount = convert<UInt_64>(root["json"][i]["enable_amount"].asString());//可用数量
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
			return false;
		}
	}
	if (true == bBeginRedrawPosition)
	{
		if (SPtr_CData->hwnd_TradeDlg == NULL)
		{
			SPtr_CData->Queue_WmMessage.push(WM_ReDrawPosition);
		}
		else if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_ReDrawPosition, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
		}
	}
	return true;
}



bool CLogin_Fast::initOrderDemo_Fast()
{
	int ret;
	bool bRun = true;
	string position_str = "";
	bool bContinuousQuery = true;
	while (bRun)
	{
		if (true == bContinuousQuery)
		{
			string s_Fun333101;
			GainQuerySingleBatchOrder333101_Fast(s_Fun333101, position_str);
			if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun333101, 
				SPtr_CData) < 0)
			{
				bRun = false;
				continue;
			}
			else
			{
				ret = pCApiFun->Api_T2_Syn->RecvMsg();
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
				
				OnQueryOrder_Fast(root, position_str, bContinuousQuery);
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

bool CLogin_Fast::GainQuerySingleBatchOrder333101_Fast(string &s_str, const string &position_str)
{
	//委托查询
	Json::Value obj_Value;
	obj_Value["function_id"] = "333101";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["report_no"] = "0";
	obj_Value["etf_flag"] = "0";
	obj_Value["action_in"] = "0";
	obj_Value["locate_entrust_no"] = "0";

	obj_Value["query_type"] = "1";
	obj_Value["query_mode"] = "0";
	obj_Value["position_str"] = position_str;
	obj_Value["request_num"] = convert<string>(SingleQuerySize);
	obj_Value["password"] = SPtr_CData->s_pass;

	obj_Value["password_type"] = password_type_交易密码;
	s_str = obj_Value.toStyledString();
	return true;
}

bool CLogin_Fast::OnQueryOrder_Fast(const Value &root, string &position_str, bool &bContinuousQuery)
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
			InsertOrder_Fast(root, i, count);
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

bool CLogin_Fast::InsertOrder_Fast(const Value &root, const int i, const int count)
{
	CString stock_account;
	COrder obj_COrder;
	stock_account = root["json"][i]["stock_account"].asString().c_str();

	obj_COrder.stock_code = root["json"][i]["stock_code"].asString().c_str();//证券代码
	obj_COrder.stock_name = root["json"][i]["stock_name"].asString().c_str();//证券名称
																			 /*买卖方向*/
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
	obj_COrder.entrust_amount = convert<UInt_64>(root["json"][i]["entrust_amount"].asString());//委托数量
	obj_COrder.entrust_price = convert<double>(root["json"][i]["entrust_price"].asString());//委托价格

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
	obj_COrder.report_time = tool::DealReportTime(root["json"][i]["report_time"].asString()).c_str();//申报时间

	tool::WriteCheckList(SPtr_CData,root,i,stock_account);

	if (0 == obj_COrder.entrust_no)
	{
		LOG(ERROR) << "entrust_no 0";
		ExceptionInterrupt;
		return false;
	}
	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}
	return true;
}
