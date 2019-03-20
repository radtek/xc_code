#pragma once
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "indexSpi.h"

using namespace std;

CAutoTraeSpi obj_CAutoTraeSpi;

CAutoTraeSpi::CAutoTraeSpi()
{
}

CAutoTraeSpi::~CAutoTraeSpi()
{

}

void CAutoTraeSpi::OnData(char* strJson)
{
	DealQuit(SPtr_CData->bBeginRelease);
	recvTime = tool::GetTimestamp_ms();
	if (SPtr_CData->bFullLog)
	{
		LOG(INFO) << strJson;
	}
	pRsp = strJson;
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(string(strJson), root, false))
	{
		ExceptionInterrupt;
		LOG(ERROR) << "parse error";
		return;
	}
	int msgType = convert<int>(root["msgType"].asString());
	switch (msgType)
	{
	case MsgType_ConnectPartition:
		OnConnectPartition(root);
		break;
	case MsgType_Login:
		OnLogin(root);
		break;
	case MsgType_Reset:
		OnReset(root);
		break;
	case MsgType_SubScribe:
		OnSubScribe(root);
		break;
	case MsgType_Publish:
		OnPublish(root);//直连模式只同时发布一条
		break;
	case MsgType_Heart:
		break;
	case MsgType_reconnecting:
	{
		LOG(ERROR) << "MsgType_reconnecting";
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_DisConnect_alert, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		break;
	}
	case MsgType_reconned:
	{
		LOG(ERROR) << "MsgType_reconned";
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_Connected_alert, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		break;
	}
	default:
		ErrorLog("abnormal msgType");
		break;
	}
}

bool CAutoTraeSpi::OnConnectPartition(const Value &root)
{
	string RetCode = root["retCode"].asString();
	if (RetCode != "0")
	{
		ErrorLog(root["retMsg"].asString());
		MessageBox(SPtr_CData->hwnd_MainPageDlg, root["retMsg"].asString().c_str(), "提示", MB_OK);
		return false;
	}
	else
	{
		SetEvent(SPtr_CData->Event_ConnectPartition_SZSE);
	}
	return true;
}

bool CAutoTraeSpi::OnLogin(const Value &root)
{
	string RetCode = root["retCode"].asString();
	if (RetCode != "0")
	{
		ErrorLog(root["retMsg"].asString());
		MessageBox(SPtr_CData->hwnd_MainPageDlg, root["retMsg"].asString().c_str(), "提示", MB_OK);
		return false;
	}
	else
	{
		SetEvent(SPtr_CData->Event_loginSZSEAlert);
	}
	return true;
}

bool CAutoTraeSpi::OnReset(const Value &root)
{
	/*studying 鲁总觉得拖出来的窗口可以忽略主界面的最小化会方便很多*/
	string RetCode = root["retCode"].asString();
	if (RetCode != "0")
	{
		ErrorLog(root["retMsg"].asString());
		MessageBox(SPtr_CData->hwnd_MainPageDlg, root["retMsg"].asString().c_str(), "提示", MB_OK);
		return false;
	}
	else
	{
		SetEvent(SPtr_CData->Event_ResetAlert_SZSE);
	}
	return true;
}

bool CAutoTraeSpi::OnSubScribe(const Value &root)
{
	string RetCode = root["retCode"].asString();
	if (RetCode != "0")
	{
		ErrorLog(root["retMsg"].asString());
		MessageBox(SPtr_CData->hwnd_MainPageDlg, root["retMsg"].asString().c_str(), "提示", MB_OK);
		return false;
	}
	return true;
}

void CAutoTraeSpi::OnPublish(const Value &root)
{
	CAertRecord obj_CAertRecord;
	if (warnningId_EntrustEach == root["warnningId"].asString())
	{
		if (filterRsp(root) == true)
		{
			return;
		}
		initAletRecord(root, obj_CAertRecord);
		string s_Fun333002;
		if (verifyBCanTrade(s_Fun333002, root, obj_CAertRecord) == true)
		{
			if (SPtr_CData->obj_CAutoTradeConfig.bManualConfirm)
			{
				CString s_str;
				tool::GetOrderInfo(s_str, s_Fun333002, SPtr_CData);
				if (MessageBox(SPtr_CData->hwnd_TradeDlg, s_str, "提示,可能引起秒级延迟", MB_YESNO) == IDYES)
				{
					SendOrder(SPtr_CData, pCApiFun, s_Fun333002, obj_CAertRecord);
				}
			}
			else
			{
				SendOrder(SPtr_CData, pCApiFun, s_Fun333002, obj_CAertRecord);
			}
		}
	}
	else
	{
		ErrorLog("abnormal warnningId");
		return;
	}
	if (SPtr_CData->obj_CEntrustEachConfig.bSound == true)
	{
		tool::MyPlaySound(s_path_AlertSound);
	}
	SPtr_CData->SetAlertRecord(obj_CAertRecord);
	DealOnAlertEnd(SPtr_CData->hwnd_TradeDlg);
	if (bStopTrade)
	{
		/*sendmessage*/
		tool::MyPlaySound(s_path_abnormalSoun);
		SendMessage(SPtr_CData->hwnd_TradeDlg, WM_stopTrade_alert, NULL, NULL);
		bStopTrade = false;
	}
	return;
}

bool CAutoTraeSpi::verifyBCanTrade(string &s_fun, const Value &root, CAertRecord &obj_CAertRecord)
{
	if (SPtr_CData->obj_CAutoTradeConfig.bTimingTrading == false)
	{
		return false;
	}
	if (VerifybTradingHour() == false)
	{
		return false;
	}
	if (verifyBLegalSecCode(root, obj_CAertRecord) == false)
	{
		return false;
	}
	GainBuy333002(s_fun, root);
	if (verifyEntrustBalance(s_fun, obj_CAertRecord) == false)
	{
		return false;
	}
	return true;
}

bool CAutoTraeSpi::filterRsp(const Value &root)
{
	string key;
	key = root["warnningId"].asString();
	key += root["marketId"].asString();
	UInt_64 updateTime = (UInt_64)root["updateTime"].asDouble();
	auto iter = SPtr_CData->M_alertId.find(key);
	if (iter == SPtr_CData->M_alertId.end())
	{
		ErrorLog("abnormal key");
		return false;
	}
	if (iter->second == updateTime)
	{
		/*是正确的应答，不过滤*/
		return false;
	}
	return true;
}

void CAutoTraeSpi::initAletRecord(const Value &root, CAertRecord &obj_CAertRecord)
{
	getAlertTime(obj_CAertRecord.s_alertTime, root["entrustTime"].asDouble());
	getCause(obj_CAertRecord.cause, root);
	obj_CAertRecord.stock_code = root["secCode"].asCString();
	CString stock_name;
	if (SPtr_CData->SecCode2SecName(obj_CAertRecord.stock_code, stock_name) == false)
	{
		ErrorLog("SecCode2SecName fail");
	}
	obj_CAertRecord.stock_name = stock_name;
}

bool CAutoTraeSpi::VerifybTradingHour()
{
	CString s_now = tool::GainLocalTime();
	if ( SPtr_CData->obj_CAutoTradeConfig.beginTime < s_now && s_now < SPtr_CData->obj_CAutoTradeConfig.endTime)
	{
		return true;
	}
	return false;
}

bool CAutoTraeSpi::verifyBLegalSecCode(const Value &root, CAertRecord &obj_CAertRecord)
{
	CString stock_code = root["secCode"].asCString();
	auto iter = SPtr_CData->M_blackList.find(stock_code);
	if (iter == SPtr_CData->M_blackList.end())
	{
		return true;
	}
	obj_CAertRecord.note = "黑名单";
	return false;
}

bool CAutoTraeSpi::SendOrder(PtrData SPtr_CData, CApiFun *pCApiFun,string s_Fun333002, CAertRecord &obj_CAertRecord)
{
	LOG(INFO) << s_Fun333002 << endl;
	UInt_64 sendTime = tool::GetTimestamp_ms();
	if (tool::SendOrder(pCApiFun, Api_Syn, s_Fun333002, SPtr_CData) < 0)
	{
		return false;
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

		/*sendedTime = tool::GetTimestamp_ms();
		UInt_64 interval = sendedTime - recvTime;
		UInt_64 interval2 = sendedTime - sendTime;
		if (interval > 1000)
		{
			LOG(ERROR) << "sendedTime - recvTime:" << interval << " sendedTime - sendTime" << interval2;
		}
		else
		{
			LOG(WARNING) << "sendedTime - recvTime:" << interval << " sendedTime - sendTime" << interval2;
		}*/

		Json::Reader reader;
		Json::Value root;

		if (!reader.parse(string(pRespData), root, false))
		{
			ExceptionInterrupt;
			LOG(ERROR) << "parse fail";
			return false;
		}
		if (tool::VerifyElement(root, 0, SPtr_CData, obj_CAertRecord.note,bStopTrade) == true)
		{
			string entrust_no = root["json"][0u]["entrust_no"].asString();
			FillItem_AlertRecord(s_Fun333002, entrust_no, obj_CAertRecord);
		}
		else
		{
			LOG(ERROR) << pRespData;
		}
		delete[] pRespData;
	}
	return true;
}

bool CAutoTraeSpi::FillItem_AlertRecord(string s_Fun333002, string entrust_no, CAertRecord &obj_CAertRecord)
{
	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(s_Fun333002, root, false))
	{
		ExceptionInterrupt;
		LOG(ERROR) << "parse fail";
		return false;
	}
	obj_CAertRecord.entrust_no = entrust_no.c_str();
	obj_CAertRecord.entrust_price = root["entrust_price"].asCString();
	obj_CAertRecord.entrust_amount = root["entrust_amount"].asCString();
	return true;
}

void CAutoTraeSpi::getAlertTime(CString &alert_time, Int_64 entrust_time)
{
	CString s_str;
	int ms = entrust_time % 1000;
	s_str = stamp2Standard((time_t)entrust_time/1000);
	s_str += ":";
	s_str += convert<string>(ms).c_str();
	alert_time = s_str;
}

CString CAutoTraeSpi::stamp2Standard(time_t tick)
{
	struct tm tm;
	char s[100];
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%H:%M:%S", &tm);
	return s;
}

void CAutoTraeSpi::getCause(CString &s_cause, const Value &root)
{
	s_cause += "以";
	s_cause += convert<string>(root["price"].asDouble()/10000).c_str();
	s_cause += "(偏离现价";
	s_cause += convert<string>(root["latestPrice"].asDouble() / 10000).c_str();
	s_cause += ",";
	s_cause += convert<string>(root["returnVals"][paramsIndex_101_DeviateRatio].asDouble() / 100).c_str();
	s_cause += ")";
	map<int, CString> map_cause{ {paramsIndex_101_bBuy,"买入"},{paramsIndex_101_bSell,"卖出"},
	{paramsIndex_101_bWithdarwBuy,"撤买"},{paramsIndex_101_bWithdrawSell,"撤卖"}};
	for (auto iter : map_cause)
	{
		if (1 == root["returnVals"][iter.first].asInt())
		{
			s_cause += iter.second;
			break;
		}
	}
	s_cause += convert<string>(root["returnVals"][paramsIndex_101_EntrustAmount].asInt()/100).c_str();
	s_cause += "手";
}

void CAutoTraeSpi::GainBuy333002(string &s_str,const Value &root)
{
	string stock_code = root["secCode"].asString();

	double entrust_price;
	getEntrustPrice(entrust_price,root);
	string entrust_amount;
	GetEntrustAmount(entrust_amount, entrust_price);

	string exchange_type;
	if (SPtr_CData->SecCode2ExchangeType(stock_code.c_str(), exchange_type) == false)
	{
		ErrorLog("SecCode2ExchangeType fail");
		return;
	}

	Json::Value obj_Value;
	obj_Value["function_id"] = "333002";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["exchange_type"] = exchange_type;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["stock_code"] = string(stock_code);
	obj_Value["entrust_amount"] = entrust_amount;

	obj_Value["entrust_price"] = convert<string>(entrust_price);
	obj_Value["entrust_bs"] = entrust_bs_买入;
	obj_Value["entrust_prop"] = entrust_prop_买卖;
	s_str = obj_Value.toStyledString();
}

bool CAutoTraeSpi::verifyEntrustBalance(string s_fun, CAertRecord &obj_CAertRecord)
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(s_fun, root, false))
	{
		ErrorLog("parse fail");
		return false;
	}
	CString stock_code;
	double entrust_price;
	size_t entrust_amount;
	double entrust_balance;
	stock_code = root["stock_code"].asCString();
	entrust_price = convert<double>(root["entrust_price"].asCString());
	entrust_amount = convert<size_t>(root["entrust_amount"].asCString());
	entrust_balance = entrust_price * entrust_amount;
	/*校验委托额*/
	type_M_entrustBalance::iterator iter = map_entrustBalance.find(stock_code);
	if (iter == map_entrustBalance.end())
	{
		map_entrustBalance.insert(make_pair(stock_code, 0));
		iter = map_entrustBalance.find(stock_code);
	}
	if (entrust_balance > SPtr_CData->obj_CAutoTradeConfig.limit_entrustBalance - iter->second)
	{
		CString s_str = stock_code;
		s_str += "委托额超限";
		obj_CAertRecord.note = s_str;
		return false;
	}
	else
	{
		map_entrustBalance[stock_code] = iter->second + entrust_balance;
	}

	/*校验可用资金*/
	double enableBalance;
	getEnableBalance(enableBalance);
	if (entrust_balance > enableBalance)
	{
		bStopTrade = true;
		MessageBox(SPtr_CData->hwnd_TradeDlg,"委托非法：委托金额大于可用资金", "提示,可能引起秒级延迟", MB_OK);
		LOG(ERROR)<< "委托非法：委托金额大于可用资金";
		return false;
	}
	return true;
}

bool CAutoTraeSpi::getEnableBalance(double &enableBalance)
{
	CFund obj_CFund;
	if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
	{
		ErrorLog("GetFund fail");
		enableBalance = 0;
	}
	else
		enableBalance = obj_CFund.enable_balance;
	return true;
}

void CAutoTraeSpi::getEntrustPrice(double &entrust_price,const Value &root)
{
	double UpLimitPrice;
	double price_ret;
	if (AlertPrice == SPtr_CData->obj_CAutoTradeConfig.priceType)
	{
		price_ret = root["price"].asDouble() / 10000;
	}
	else
	{
		price_ret = root["latestPrice"].asDouble() / 10000;
	}
	double price_tmp = 0;
	getUpLimitPrice(UpLimitPrice,root);
	if (PercentageIncrease == SPtr_CData->obj_CAutoTradeConfig.s_bPercentageIncrease)
	{
		/*按百分比加价*/
		double ratio = SPtr_CData->obj_CAutoTradeConfig.IncreaseValue / 100 + 1;
		price_tmp = price_ret* ratio;
	}
	else
	{
		/*按元加价*/
		price_tmp = SPtr_CData->obj_CAutoTradeConfig.IncreaseValue + price_ret;
	}
	if (price_tmp >= UpLimitPrice)
	{
		price_tmp = UpLimitPrice;
	}
	entrust_price = tool::round(price_tmp, 2);
}

bool CAutoTraeSpi::getUpLimitPrice(double &UpLimitPrice,const Value &root)
{
	CString stock_code = root["secCode"].asString().c_str();
	CString market_code;
	if (SPtr_CData->SecCode2MarketCode(stock_code, market_code) == false)
	{
		ErrorLog("SecCode2MarketCode fail");
		return false;
	}
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(market_code, stock_code, obj_CSecurity) == false)
	{
		ErrorLog("GetSecurity fail");
		return false;
	}
	UpLimitPrice = obj_CSecurity.UpLimitPrice;
	return true;
}

void CAutoTraeSpi::GetEntrustAmount(string &s_entrust_amount,double entrust_price)
{
	size_t entrust_amount;
	if (true == SPtr_CData->obj_CAutoTradeConfig.bDefaultAmount)
	{
		entrust_amount = SPtr_CData->obj_CAutoTradeConfig.DefaultAmount;
	}
	else
	{
		entrust_amount = SPtr_CData->obj_CAutoTradeConfig.DefaultBalance / entrust_price;
	}
	s_entrust_amount = convert<string>(tool::modifyNum((size_t)entrust_amount));
}

CString CAutoTraeSpi::Convert_MarketCode(string marketCode)
{
	CString s_dst;
	if (marketCode == "sh")
	{
		s_dst = SSE;
	}
	else if (marketCode == "sz")
	{
		s_dst = SZSE;
	}
	else
	{
		ErrorLog("abnormal marketCode");
	}
	return s_dst;
}


void CAutoTraeSpi::FillJumpTime(CString &s_str)
{
	char buf[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf(buf, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
	s_str = buf;
}

bool CAutoTraeSpi::DealOnAlertEnd(const HWND &hwnd)
{
	if (PostMessage(hwnd, WM_RedrawAlertRecord, 0, 0) == 0)
	{
		UInt_32 ret = GetLastError();
		ErrorLog("PostMessage fail");
	}
	return true;
}

bool CAutoTraeSpi::DealOnClose(const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo)
{
	if (bRun_MainPageDlg == true && false == SPtr_CData->bBeginRelease)
	{
		LOG(ERROR) << errorInfo;
		/*异常退出程序*/
		MessageBox(hwnd, errorInfo, "error", 0);
		/*退出程序*/
		SendMessage(hwnd, WM_AbnormalExit, 0, 0);
	}
	return true;
}

bool CAutoTraeSpi::dll_InitCData(PtrData SPtr_CDataTmp)
{
	SPtr_CData = SPtr_CDataTmp;
	return true;
}

bool CAutoTraeSpi::dll_InitFun(CApiFun* pCApiFunTmp)
{
	pCApiFun = pCApiFunTmp;
	return true;
}

void CAutoTraeSpi::OnClose()
{
	DealOnClose(SPtr_CData->bRun_MainPageDlg, SPtr_CData->hwnd_MainPageDlg, "深圳预警主站连接中断,请重新登陆");
}

extern "C" bool dll_InitCData(PtrData SPtr_CDataTmp)
{
	return obj_CAutoTraeSpi.dll_InitCData(SPtr_CDataTmp);
}

extern "C" bool dll_InitFun(CApiFun* pCApiFunTmp)
{
	return obj_CAutoTraeSpi.dll_InitFun(pCApiFunTmp);
}

extern "C" void OnData(char* strJson)
{
	return obj_CAutoTraeSpi.OnData(strJson);
}

extern "C" void OnClose()
{
	return obj_CAutoTraeSpi.OnClose();
}