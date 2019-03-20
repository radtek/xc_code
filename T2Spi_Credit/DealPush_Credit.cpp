#include "stdafx.h"
#include "T2Spi_Credit.h"


bool CT2Spi_Credit::On620001(const Value &root)
{
	if (root["json"].isNull())
	{
		ErrorLog("On620001 no json");
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData))
		{
			string s_str;
			int subscibe_result = convert<int>(root["json"][i]["subscibe_result"].asString());
			string result_info = root["json"][i]["result_info"].asString();
			int issue_type = convert<int>(root["json"][i]["issue_type"].asString());
			if (subscibe_result != 0)
			{
				switch (issue_type)
				{
				case 12:
					s_str = "成交推送:";
					s_str += result_info;
					break;
				case 23:
					s_str = "委托推送:";
					s_str += result_info;
					break;
				default:
					ErrorLog("abnormal issue_type");
					break;
				}
				ErrorLog(s_str);
				SPtr_CData->Queue_T2ErrorInfo.push(s_str);
			}
			else
			{
				switch (issue_type)
				{
				case 12:
					s_str = "成交推送:";
					break;
				case 23:
					s_str = "委托推送:";
					break;
				default:
					ErrorLog("abnormal issue_type");
					break;
				}
				s_str += result_info;
				LOG(INFO) << s_str;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;//只有一个元素，而且要根据这个判断是否登录成功，因此一定要用return
		}
	}
	return true;
}

/*成交、委托推送*/
bool CT2Spi_Credit::OnSubscribe(const Value &root)
{
	/*委托回报推送和成交回报推送绝对不可能用一个报文过来*/
	switch (convert<int>(root["issue_type"].asString()))
	{
	case 23:
	{
		//处理委托推送
		Deal_Entrust(root);
		break;
	}
	case 12:
	{
		/*处理成交推送*/
		Deal_Deal(root);
		break;
	}
	default:
		LOG(ERROR) << "OnSubscribe issue_type have a abnormal value";
		ExceptionInterrupt;
		break;
	}

	return true;
}

/*处理委托主推*/
bool CT2Spi_Credit::Deal_Entrust(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 false退出即可*/
		LOG(ERROR) << "DealEntrustPush no json";
		ExceptionInterrupt;
		return false;
	}
	/*有可能多个委托单在同一个推送里，因此只能在这一级别遍历。首先校验元素是否正确，如果正确再取委托类型，如果元素错误直接报错，并进入下一个元素*/
	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
	    if (Inspect_bStock(root,i) == false)
		{
			continue;
		}
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			switch (root["json"][i]["entrust_type"].asString()[0])
			{
			case '2':
			{
				/*处理委托类型为撤单的委托推送*/
				Deal_Entrust_Revoke(root, i, count);
				break;
			}
			case '6'://信用融资
			{
				Deal_Entrust_Entrust(root, i, count);
				break;
			}
			case '9'://信用交易
			{
				Deal_Entrust_Entrust(root, i, count);
				break;
			}
			default:
				LOG(ERROR) << "abnormal entrust_type";
				ExceptionInterrupt;
				break;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << "a element illegal";
			ExceptionInterrupt;
			return false;
		}
	}

	return true;
}

/*处理委托类型为撤单的委托推送*/
bool CT2Spi_Credit::Deal_Entrust_Revoke(const Value &root, const int i, const int count)
{
	string stock_code = root["json"][i]["stock_code"].asString();
	COrder obj_COrder;/*fixing 已报的单子退出重进撤单，出现这个错误*/
	if (GainObjOrder(root, i, obj_COrder) == false)
	{
		LOG(ERROR) << "GainObjOrder fail";
		ExceptionInterrupt;
		AfxMessageBox("GainObjOrder in Deal_Entrust_Revoke");//fixing 
		return false;
	}

	if ('1' == obj_COrder.ArrivalSequence_WithdrawOrder[index_DealPush])
	{
		/*成交推送先到，清空循环，扔掉推送包*/
		obj_COrder.ArrivalSequence_WithdrawOrder[index_EntrustPush] = '0';
		obj_COrder.ArrivalSequence_WithdrawOrder[index_DealPush] = '0';
		return true;
	}
	else
	{
		/*正常到达，交给成交推送清空*/
		obj_COrder.ArrivalSequence_WithdrawOrder[index_EntrustPush] = '1';
	}

	switch (obj_COrder.entrust_status)
	{
	case YiBao:
		obj_COrder.entrust_status = YiBaoDaiChe;
		break;
	case BuCheng:
		obj_COrder.entrust_status = BuChengDaiChe;
		break;
	default:
		LOG(ERROR) << "abnormal entrust_status";
		ExceptionInterrupt;/*fixing 在这报了一次错撤单的时候委托状态为已撤*/
		AfxMessageBox("abnormal entrust_status");//fixing
		break;
	}

	CString stock_account;
	stock_account = root["json"][i]["stock_account"].asString().c_str();

	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}
	bPostMessage = true;
	UpdateFund_Position(stock_code);
	return true;
}

/*处理委托类型为信用融资、信用交易的委托推送*/
bool CT2Spi_Credit::Deal_Entrust_Entrust(const Value &root, const int i, const int count)
{
	//股票名称 股票代码 方向 委托类别 委托价格 委托数量 成交数量 委托状态 委托号
	/*后续需要更新的成交数量、委托状态*/
	CString stock_account;
	CString MarketCode;
	COrder obj_COrder;

	if (GainObjOrder(root, i, obj_COrder) == true && obj_COrder.ArrivalSequence_PlaceOrder[index_DealPush] == '1')
	{
		/*成交推送先到*/
		ExceptionInterrupt;
	}
	else
	{
		/*委托推送先到，应答先到*/
		updateEntrust_status(obj_COrder, root["json"][i]["entrust_status"].asString(), root["json"][i]["cancel_info"].asString()); /*委托状态*/
	}

	stock_account = root["json"][i]["stock_account"].asString().c_str();
	//股票名称 股票代码 方向 委托类别 委托价格 委托数量 成交数量 委托状态 委托号
	obj_COrder.stock_code = root["json"][i]["stock_code"].asString().c_str();
	if (SPtr_CData->SecCode2SecName(obj_COrder.stock_code,obj_COrder.stock_name) == false)
	{
		return false;
	}
	updateEntrust_bs(obj_COrder, root["json"][i]["entrust_bs"].asString());
	updateEntrust_type(obj_COrder, root["json"][i]["entrust_type"].asString());
	obj_COrder.entrust_price = convert<double>(root["json"][i]["entrust_price"].asString());

	obj_COrder.entrust_amount = convert<UInt_64>(root["json"][i]["entrust_amount"].asString());
	obj_COrder.entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());

	tool::WriteCheckList(SPtr_CData,root, i, stock_account);

	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}

	/*有可能多个委托单的委托推送放在同一个返回中，但PostMessage只是重绘因此可以最后一次执行*/
	int pos = i;
	if (pos == count - 1)
	{
		bPostMessage = true;
	}
	UpdateFund_Position(string(obj_COrder.stock_code));
	return true;
}

/*处理成交推送*/
bool CT2Spi_Credit::Deal_Deal(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 false退出即可*/
		LOG(ERROR) << "Deal_Deal no json";
		ExceptionInterrupt;
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
        if (Inspect_bStock(root, i) == false)
		{
			continue;
		}
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			if (root["json"][i]["entrust_type"].isNull())
			{
				/*未报撤单时，原委托会收到一笔成交类型为撤单的成交推送，要核实只有在这种情况下才这样回答*/
				Deal_Deal_WeiBaoRevoke(root, i, count);
			}
			else
			{
				switch (root["json"][i]["entrust_type"].asString()[0])
				{
				case '6'://信用融资
				{
					/*处理委托类型为信用融资的成交推送 下单和撤单都会产生*/
					Deal_Deal_Entrust(root, i, count);
					break;
				}
				case '9'://信用交易
				{
					Deal_Deal_Entrust(root, i, count);
					break;
				}
				default:
					LOG(ERROR) << "abnormal enturst_type";
					ExceptionInterrupt;
					break;
				}
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

bool CT2Spi_Credit::Deal_Deal_WeiBaoRevoke(const Value &root, const int i, const int count)
{
	CString stock_account;
	stock_account = root["json"][i]["stock_account"].asString().c_str();
	string stock_code = root["json"][i]["stock_code"].asString().c_str();
	COrder obj_COrder;
	if (GainObjOrder(root, i, obj_COrder) == false)/*fix 实验下能否正常处理*/
	{
		LOG(ERROR) << "GainObjOrder fail";/*未报没有委托主推查不到委托，因此未报撤单无法处理，采用直接return掉*/
		ExceptionInterrupt;
		return false;
	}

	switch (root["json"][i]["entrust_status"].asString()[0])
	{
	case '6':
		obj_COrder.entrust_status = YiChe;
		break;
	default:
		LOG(ERROR) << "abnormal entrust_status";
		ExceptionInterrupt;
		break;
	}

	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}
	UpdateFund_Position(stock_code);
	bPostMessage = true;
	return true;
}

/*处理委托类型为信用融资的成交推送*/
bool CT2Spi_Credit::Deal_Deal_Entrust(const Value &root, const int i, const int count)
{
	switch (convert<int>(root["json"][i]["real_type"].asString()))
	{
	case 0://成交类型为买卖
	{
		Deal_Deal_Entrust_BS(root, i, count);
		break;
	}
	case 2://成交类型为撤单
	{
		Deal_Deal_Entrust_Revoke(root, i, count);
		break;
	}
	default:
		break;
	}

	return true;
}

bool CT2Spi_Credit::Deal_Deal_Entrust_BS(const Value &root, const int i, const int count)
{
	CString stock_account;
	stock_account = root["json"][i]["stock_account"].asString().c_str();
	string stock_code = root["json"][i]["stock_code"].asString().c_str();
	COrder obj_COrder;
	if (GainObjOrder(root, i,obj_COrder) == false)
	{
		ExceptionInterrupt;
	}
	obj_COrder.ArrivalSequence_PlaceOrder[index_DealPush] = '1';
	/*更新成交数量*/
	obj_COrder.business_amount += convert<UInt_64>(root["json"][i]["business_amount"].asString());
	/*更新委托状态*/
	updateEntrust_status(obj_COrder, root["json"][i]["entrust_status"].asString(), root["json"][i]["cancel_info"].asString());

	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}
	UpdateFund_Position(stock_code);
	/*写成交*/
	/*股票名称 股票代码 方向 成交数量 成交价格 成交时间 成交编号 委托号 */
	CString MarketCode;
	string s_entrust_no;
	CDeal obj_CDeal;//每次都是第一次，成交set之后不不能改了

	obj_CDeal.stock_code = root["json"][i]["stock_code"].asString().c_str();//证券代码
	obj_CDeal.stock_name = obj_COrder.stock_name;

	updateEntrust_bs(obj_CDeal, root["json"][i]["entrust_bs"].asString());

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
	if (obj_CDeal.entrust_no == 0)
	{
		LOG(ERROR) << "entrust_no is 0 in OnQueryDeal";
		ExceptionInterrupt;
	}

	obj_CDeal.business_balance = obj_CDeal.business_amount*obj_CDeal.business_price;
	if (SPtr_CData->SetDeal(SPtr_CData->fund_account, stock_account, obj_CDeal.entrust_no, obj_CDeal) == false)
	{
		LOG(ERROR) << "SetDeal fali";
		ExceptionInterrupt;
	}

	int pos = i;
	if (pos == count - 1)
	{
		/*重绘Deal、Order和其他查询可以放在最后一次执行*/
		bPostMessage = true;
		UpdateFund_Position(string(obj_CDeal.stock_code));
	}
	return true;
}

bool CT2Spi_Credit::Deal_Deal_Entrust_Revoke(const Value &root, const int i, const int count)
{
	CString stock_account;
	stock_account = root["json"][i]["stock_account"].asString().c_str();
	string stock_code = root["json"][i]["stock_code"].asString().c_str();
	COrder obj_COrder;
	if (GainObjOrder(root, i,obj_COrder) == false)
	{
		/*不支持多个客户端同时登陆*/
		LOG(ERROR) << "GainObjOrder fail";
		ExceptionInterrupt;
		return false;
	}
	if ('1' == obj_COrder.ArrivalSequence_WithdrawOrder[index_EntrustPush])
	{
		/*委托推送先到，清掉整个循环*/
		obj_COrder.ArrivalSequence_WithdrawOrder[index_EntrustPush] = '0';
		obj_COrder.ArrivalSequence_WithdrawOrder[index_DealPush] = '0';
	}
	else
	{
		/*成交异常的时候，提示委托推送，并交给委托推送清理*/
		obj_COrder.ArrivalSequence_WithdrawOrder[index_DealPush] = '1';
	}

	/*更新委托状态*/
	updateEntrust_status(obj_COrder, root["json"][i]["entrust_status"].asString(), root["json"][i]["cancel_info"].asString());

	if (0 == obj_COrder.entrust_no)
	{
		LOG(ERROR) << "entrust_no is 0";
		ExceptionInterrupt;
		return false;
	}

	if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
	{
		LOG(ERROR) << "SetOrder fail";
		ExceptionInterrupt;
		return false;
	}
	int pos = i;
	if (pos == count - 1)
	{
		bPostMessage = true;
	}
	UpdateFund_Position(stock_code);
	return true;
}

bool CT2Spi_Credit::UpdateFund_Position(const string SecCode)
{
	M_QueryData[SecCode] = true;
	return true;
}




