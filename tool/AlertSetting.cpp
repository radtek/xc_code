#include "stdafx.h"
#include "AlertSetting.h"

CAlertSetting::CAlertSetting(PtrData SPtr_CDataTmp, CApiFun *pCApiFunTmp)
{
	SPtr_CData = SPtr_CDataTmp;
	pCApiFun = pCApiFunTmp;
	s_ConfigPath = "..\\share\\config.xml";
}

bool CAlertSetting::initAlertSettingNeed(string Node_Father)
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		/*默认配置文件只能程序修改都是正确的，即使转换为int失败也没有严重影响，顶多全空白，用户自己手动设*/
		SPtr_CData->obj_CAlertConfig.Template_Stock = pt.get<size_t>(Node_Father+ "Template_Stock");

		SPtr_CData->obj_CAlertConfig.Template_Alert = pt.get<size_t>(Node_Father + "Template_Alert");

		/*涨幅*/
		SPtr_CData->obj_CAlertConfig.IncreaseRatio = tool::round(pt.get<double>(Node_Father + "IncreaseRatio"), 2);

		SPtr_CData->obj_CAlertConfig.bRemove_Increase = pt.get<bool>(Node_Father + "bRemove_Increase");

		SPtr_CData->obj_CAlertConfig.Period_Increase_day = pt.get<unsigned int>(Node_Father + "Period_Increase");

		SPtr_CData->obj_CAlertConfig.Period_Increase_second = SPtr_CData->obj_CAlertConfig.Period_Increase_day*(60 * 60 * 24);

		/*N分钟涨跌幅*/
		SPtr_CData->obj_CAlertConfig.Minute_N_Rise = pt.get<size_t>(Node_Father + "Minute_N_Rise");
		SPtr_CData->obj_CAlertConfig.IncreaseRatio_N_Rise = pt.get<double>(Node_Father + "IncreaseRatio_N_Rise");

		/*低开预警*/
		SPtr_CData->obj_CAlertConfig.Minute_Low_Rise = pt.get<size_t>(Node_Father + "Minute_Low_Rise");
		SPtr_CData->obj_CAlertConfig.BeginRatio_Low_Rise = pt.get<double>(Node_Father + "BeginRatio_Low_Rise");
		SPtr_CData->obj_CAlertConfig.EndRatio_Low_Rise = pt.get<double>(Node_Father + "EndRatio_Low_Rise");
	}
	catch (std::exception& e)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "initAlertSettingNeed fail Error:" << e.what();
		return false;
	}
	return true;
}

bool CAlertSetting::ResetMonitor()
{
	if (SendResetMonitor(pCApiFun->SendOrder_Alert_SSE,AlertType_SSE) == false)
	{
		return false;
	}
	if (SendResetMonitor(pCApiFun->SendOrder_Alert_SZSE,AlertType_SZSE) == false)
	{
		return false;
	}
	UInt_32 ret_Wait;
	bool b_ret;
    ret_Wait = WaitForSingleObject(SPtr_CData->Event_ResetAlert_SSE, ShortTime);
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	ret_Wait = WaitForSingleObject(SPtr_CData->Event_ResetAlert_SZSE, ShortTime);
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	return true;
}

bool CAlertSetting::SendResetMonitor(pAlertSendOrder SendOrder_Alert, int alertType)
{
	int ret;
	string s_ResetAlert;
	tool::GainResetAlert(s_ResetAlert, SPtr_CData , alertType);
	ret = SendOrder_Alert((char *)s_ResetAlert.c_str());
	if (tool::DealRet_AlertSendOrder(ret, SPtr_CData->hwnd_TradeDlg) == false)
	{
		return false;
	}
	return true;
}

bool CAlertSetting::RequireMonitor(string path_CustomPool)
{
	Json::Value params;
	switch (SPtr_CData->obj_CAlertConfig.Template_Alert)
	{
	case enum_Increase:
		FillParams_IncreaseRatio(params);
		switch (SPtr_CData->obj_CAlertConfig.Template_Stock)
		{
		case AllMarket:
			Monitor_All(params, warnningId_Increase, SPtr_CData->obj_CAlertConfig.bRemove_Increase);
			break;
		case CustomizePool:
			Monitor_Config(params, path_CustomPool, warnningId_Increase);
			break;
		default:
			ErrorLog("abnoraml StockTemplate");
			break;
		}
		break;
	case enum_N_Increase:
		FillParams_N_Increase(params);
		switch (SPtr_CData->obj_CAlertConfig.Template_Stock)
		{
		case AllMarket:
			Monitor_All(params, warnningId_N_Increase, false);
			break;
		case CustomizePool:
			Monitor_Config(params, path_CustomPool, warnningId_N_Increase);
			break;
		default:
			ErrorLog("abnoraml StockTemplate");
			break;
		}
		break;
	case enum_Low_Rise:
		FillParams_Low_Rise(params);
		switch (SPtr_CData->obj_CAlertConfig.Template_Stock)
		{
		case AllMarket:
			Monitor_All(params, warnningId_Low_Rise, false);
			break;
		case CustomizePool:
			Monitor_Config(params, path_CustomPool, warnningId_Low_Rise);
			break;
		default:
			ErrorLog("abnoraml StockTemplate");
			break;
		}
		break;
	default:
		ErrorLog("abnormal Template_Alert");
		break;
	}
	return true;
}

bool CAlertSetting::Monitor_All(Json::Value &params, string warnningId, bool bFilterNewStock)
{
	Json::Value root_SSE;
	Json::Value root_SZSE;
	string s_AlertList_SSE;
	string s_AlertList_SZSE;
	InitAlertList(s_AlertList_SSE, root_SSE,true);
	InitAlertList(s_AlertList_SZSE, root_SZSE,false);

	for (auto iter = SPtr_CData->MMarket.begin(); iter != SPtr_CData->MMarket.end(); iter++)
	{
		iter->first;//marketcode
		_Security MSecurity;
		if (SPtr_CData->GetMSecurity(iter->first, MSecurity) == false)
		{
			ErrorLog("GetMSecurity fail");
			continue;
		}
		for (auto iter_MSecurity = MSecurity.begin(); iter_MSecurity != MSecurity.end(); iter_MSecurity++)
		{
			/*先检查是不是需要预警*/
			if (SPtr_CData->inspect_bAlert(iter_MSecurity->first) == false)
			{
				continue;
			}
			/*开启过滤后，在这个日期内的统统返回false，直接continue*/
			if (bFilterNewStock == true)
			{
				if (tool::Verify_bNewStock(iter_MSecurity->second, SPtr_CData->obj_CAlertConfig.Period_Increase_second) == false)
				{
					continue;
				}
			}
			switch (tool::find_enumMarketCode(string(iter->first)))
			{
			case enumSSE:
			{
				FillItem(root_SSE, "sh", params, string(iter_MSecurity->second->stock_code), warnningId);
				break;
			}
			case enumSZSE:
			{
				FillItem(root_SZSE, "sz", params, string(iter_MSecurity->second->stock_code), warnningId);
				break;
			}
			default:
				ErrorLog("abnormal MarketCode");
				break;
			}
		}
	}
	s_AlertList_SSE = root_SSE.toStyledString();
	s_AlertList_SZSE = root_SZSE.toStyledString();
	SendOrder(pCApiFun->SendOrder_Alert_SSE, s_AlertList_SSE);
	SendOrder(pCApiFun->SendOrder_Alert_SZSE, s_AlertList_SZSE);
	return true;
}

void CAlertSetting::InitAlertList(string &s_dst, Json::Value &root, bool bSSE)
{
	string partitionId;
	string userId;
	if (true == bSSE)
	{
		partitionId = SPtr_CData->partitionId + SSE;
		userId = SPtr_CData->user_alert + SSE;
	}
	else
	{
		partitionId = SPtr_CData->partitionId + SZSE;
		userId = SPtr_CData->user_alert + SZSE;
	}
	s_dst = "";
	Json::Value obj_Value;
	obj_Value["msgType"] = MsgType_SubScribe;
	obj_Value["partitionId"] = partitionId;
	obj_Value["refferId"] = 11;
	obj_Value["userId"] = userId;
	s_dst = obj_Value.toStyledString();
	Json::Reader reader;
	if (!reader.parse(string(s_dst), root, false))
	{
		LOG(ERROR) << "parse error";
		return;
	}
	return;
}

void CAlertSetting::FillItem(Json::Value &root, string marketId, Json::Value &params,
	string secCode, string warnningId)
{
	Json::Value Value_item;
	Value_item["deleteMark"] = false;
	Value_item["marketId"] = marketId;
	Value_item["params"] = params;
	Value_item["secCode"] = secCode;
	Value_item["warnningId"] = warnningId;
	root["userJson"].append(Value_item);
}

bool CAlertSetting::SendOrder(pAlertSendOrder SendOrder_Alert, string s_str)
{
	/*保留预警配置*/
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(s_str, root, false))
	{
		if (root["userJson"].size() > 0)
		{
			string s_key;
			s_key = root["userJson"][0u]["warnningId"].asString();
			s_key += root["userJson"][0u]["marketId"].asString();
			auto iter = SPtr_CData->M_alertId.find(s_key);
			if (iter == SPtr_CData->M_alertId.end())
			{
				SPtr_CData->M_alertId[s_key] = 0;
				iter = SPtr_CData->M_alertId.find(s_key);
			}
			iter->second = tool::GetTimestamp_ms();
			root["updateTime"] = (double)iter->second;
			s_str = root.toStyledString();
		}
		else
		{
			return true;
		}
	}
	else
	{
		ExceptionInterrupt;
		LOG(ERROR) << "parse fail:" << s_str;
		return false;
	}
	int ret;
	ret = SendOrder_Alert((char *)s_str.c_str());
	if (tool::DealRet_AlertSendOrder(ret, SPtr_CData->hwnd_TradeDlg) == false)
	{
		return false;
	}
	return true;
}

void CAlertSetting::FillParams_IncreaseRatio(Json::Value &params)
{
	params[0u] = (UInt_32)(SPtr_CData->obj_CAlertConfig.IncreaseRatio * (Magnification_Alert / 100));
}

bool CAlertSetting::Monitor_Config(Json::Value &params, string s_path, string warnningId)
{
	Json::Value root_SSE;
	Json::Value root_SZSE;
	string s_AlertList_SSE;
	string s_AlertList_SZSE;

	InitAlertList(s_AlertList_SSE, root_SSE,true);
	InitAlertList(s_AlertList_SZSE, root_SZSE, false);
	if (FillReqList(s_path, root_SSE, root_SZSE, warnningId, params) == false)
	{
		return false;
	}
	s_AlertList_SSE = root_SSE.toStyledString();
	s_AlertList_SZSE = root_SZSE.toStyledString();
	if (root_SSE["userJson"].size() > 0)
	{
		SendOrder(pCApiFun->SendOrder_Alert_SSE, s_AlertList_SSE);
	}
	if (root_SZSE["userJson"].size() > 0)
	{
		SendOrder(pCApiFun->SendOrder_Alert_SZSE, s_AlertList_SZSE);
	}
	return true;
}

void CAlertSetting::FillParams_N_Increase(Json::Value &params)
{
	params[0u] = SPtr_CData->obj_CAlertConfig.Minute_N_Rise;
	params[1] = (size_t)(SPtr_CData->obj_CAlertConfig.IncreaseRatio_N_Rise * (Magnification_Alert / 100));
}

void CAlertSetting::FillParams_Low_Rise(Json::Value &params)
{
	params[0u] = (int)(SPtr_CData->obj_CAlertConfig.BeginRatio_Low_Rise * (Magnification_Alert / 100));
	params[1] = (int)(SPtr_CData->obj_CAlertConfig.EndRatio_Low_Rise * (Magnification_Alert / 100));
	params[2] = SPtr_CData->obj_CAlertConfig.Minute_Low_Rise;
}

void CAlertSetting::FillParams_EntrustEeach(Json::Value &params)
{
	params[0u] = 10;
	params[1] = 1000;
	params[2] = 1;
	params[3] = 0;
	params[4] = 0;
	params[5] = 0;
	params[6] = 0;
	params[7] = -2000;
}

bool CAlertSetting::FillReqList(string s_Path, Json::Value &root_SSE, Json::Value &root_SZSE,
	string warnningId, Json::Value &params)
{
	Json::Reader reader;
	Json::Value root;
	string s_FileContent;
	tool::File2String(s_Path, s_FileContent);

	if (reader.parse(s_FileContent, root, false))
	{
		int Sum = root["CustomInfo"].size();
		_Security M_Security_SSE;
		SPtr_CData->GetMSecurity(SSE, M_Security_SSE);
		_Security M_Security_SZSE;
		SPtr_CData->GetMSecurity(SZSE, M_Security_SZSE);
		for (int i = 0; i < Sum; ++i)
		{
			bool bIncrease = true;
			CString stock_code = root["CustomInfo"][i]["stock_code"].asString().c_str();
			_Security::iterator iter = M_Security_SSE.find(stock_code);
			if (iter != M_Security_SSE.end())
			{
				FillItem(root_SSE, "sh", params, string(stock_code), warnningId);
			}
			iter = M_Security_SZSE.find(stock_code);
			if (iter != M_Security_SZSE.end())
			{
				FillItem(root_SZSE, "sz", params, string(stock_code), warnningId);
			}
		}
	}
	else
	{
		LOG(INFO) << "parse fail:" << s_FileContent;
	}
	return true;
}

//void CAlertSetting::FillSingleStock(CString MarketCode, CString stock_code, socket_struct_MonitorDetail ReqList[MaxSize],
//	int &countReqList, UInt_64 &RuleValue, char MoreOrLess)
//{
//	strcpy(ReqList[countReqList].MarketCode, MarketCode);
//	strcpy(ReqList[countReqList].SecCode, stock_code);
//	ReqList[countReqList].MoreOrLess = MoreOrLess;
//	ReqList[countReqList].RuleValue = RuleValue;
//	countReqList++;
//}
