#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include "AlertSpi.h"

using namespace std;

CAlertSpi::CAlertSpi()
{
}

/*SPI_OnData("{\"msgType\":\"91\",""\"msgDec\":\"链接成功\"}");
SPI_OnData("{\"msgType\":\"90\",""\"msgDec\":\"正在重连\"}");*/
void CAlertSpi::OnData(char* strJson, bool bSSE)
{
	DealQuit(SPtr_CData->bBeginRelease);
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
		OnConnectPartition(root,bSSE);
		break;
	case MsgType_Login:
		OnLogin(root, bSSE);
		break;
	case MsgType_Reset:
		OnReset(root,bSSE);
		break;
	case MsgType_SubScribe:
		OnSubScribe(root, bSSE);
		break;
	case MsgType_Publish:
		OnPublish(root,bSSE);//直连模式只同时发布一条
		break;
	case MsgType_Heart:
		break;
	case MsgType_reconnecting:
	{
		string s_tmp = getMarket(bSSE) + "MsgType_reconnecting";
		LOG(ERROR) << s_tmp;
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_DisConnect_alert, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		break;
	}
	case MsgType_reconned:
	{
		string s_tmp = getMarket(bSSE) + "MsgType_reconned";
		LOG(ERROR) << s_tmp;
		break;
	}
	default:
		ErrorLog("abnormal msgType");
		break;
	}
}

string CAlertSpi::getMarket(bool bSSE)
{
	string s_str;
	if (bSSE)
	{
		s_str = SSE;
	}
	else
		s_str = SZSE;
	return s_str;
}

bool CAlertSpi::OnConnectPartition(const Value &root, bool bSSE)
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
		if (true == bSSE)
		{
			SetEvent(SPtr_CData->Event_ConnectPartition_SSE);
		}
		else
		{
			SetEvent(SPtr_CData->Event_ConnectPartition_SZSE);
		}
	}
	return true;
}

bool CAlertSpi::OnLogin(const Value &root, bool bSSE)
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
		if (true == bSSE)
		{
			SetEvent(SPtr_CData->Event_loginSSEAlert);
		}
		else
		{
			SetEvent(SPtr_CData->Event_loginSZSEAlert);
		}
	}
	return true;
}

bool CAlertSpi::OnReset(const Value &root, bool bSSE)
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
		if (true == bSSE)
		{
			SetEvent(SPtr_CData->Event_ResetAlert_SSE);
		}
		else
		{
			SetEvent(SPtr_CData->Event_ResetAlert_SZSE);
		}
	}
	return true;
}

bool CAlertSpi::OnSubScribe(const Value &root, bool bSSE)
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

void CAlertSpi::OnPublish(const Value &root, bool bSSE)
{
	if (filterRsp(root) == true)
	{
		return;
	}
	switch (tool::find_warnningId(root["warnningId"].asString()))
	{
	case enum_Increase:
		DealIncrease(root,bSSE);
		break;
	case enum_N_Increase:
		Deal_N_Increase(root, bSSE);
		break;
	case enum_Low_Rise:
		Deal_Low_Rise(root, bSSE);
		break;
	default:
		ErrorLog("abnormal warnningId");
		break;
	}
	DealOnAlertEnd(SPtr_CData->bActivity_TradeData, SPtr_CData->hwnd_TradeDlg);
	return;
}

bool CAlertSpi::filterRsp(const Value &root)
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

bool CAlertSpi::DealIncrease(const Value &root, bool bSSE)
{
	CString marketCode;
	CString secCode;
	string warnningId;
	double price;
	double params;
	double returnVals;
	int index_Ratio = 0u;
	marketCode = Convert_MarketCode(root["marketId"].asString());
	secCode = root["secCode"].asString().c_str();
	warnningId = root["warnningId"].asString().c_str();
	price = tool::round(root["price"].asDouble() / Magnification_Alert, 2);
	params = root["params"][index_Ratio].asDouble() / (Magnification_Alert / 100);
	returnVals = root["returnVals"][index_Ratio].asDouble() / (Magnification_Alert / 100);
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(marketCode, secCode, obj_CSecurity) == false)
	{
		ErrorLog("GetSecurity fail");
		return false;
	}
	CAlertNeed obj_CAlertNeed;
	if (SPtr_CData->GetAlert(warnningId, MarketCode2BangSheng(marketCode),secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;
	obj_CAlertNeed.s_bBiaoDiQuan = obj_CSecurity.s_bBiaoDiQuan;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals;


	if (tool::round(returnVals, 2) < params)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, MarketCode2BangSheng(marketCode),secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, MarketCode2BangSheng(marketCode) ,obj_CAlertNeed) == false)
		{
			ErrorLog("SetAlert fail");
			return false;
		}
	}
	return true;
}

bool CAlertSpi::Deal_N_Increase(const Value &root, bool bSSE)
{
	CString marketCode;
	CString secCode;
	string warnningId;
	double price;
	double params;
	double returnVals;
	int index_Ratio = 1;
	marketCode = Convert_MarketCode(root["marketId"].asString());
	secCode = root["secCode"].asString().c_str();
	warnningId = root["warnningId"].asString().c_str();
	price = tool::round(root["price"].asDouble() / Magnification_Alert, 2);
	params = root["params"][index_Ratio].asDouble() / (Magnification_Alert / 100);
	returnVals = root["returnVals"][index_Ratio].asDouble() / (Magnification_Alert / 100);
	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(marketCode, secCode, obj_CSecurity) == false)
	{
		ErrorLog("GetSecurity fail");
		return false;
	}
	CAlertNeed obj_CAlertNeed;
	if (SPtr_CData->GetAlert(warnningId, MarketCode2BangSheng(marketCode), secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;
	obj_CAlertNeed.s_bBiaoDiQuan = obj_CSecurity.s_bBiaoDiQuan;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals;

	if (tool::round(returnVals, 2) < params)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, MarketCode2BangSheng(marketCode),secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, MarketCode2BangSheng(marketCode), obj_CAlertNeed) == false)
		{
			ErrorLog("SetAlert fail");
			return false;
		}
	}
	return true;
}

bool CAlertSpi::Deal_Low_Rise(const Value &root, bool bSSE)
{
	CString marketCode;
	CString secCode;
	string warnningId;
	double price;
	double params_x;
	double params_y;
	double returnVals_x;
	double returnVals_y;
	int index_Ratio_x = 0;
	int index_Ratio_y = 1;
	marketCode = Convert_MarketCode(root["marketId"].asString());
	secCode = root["secCode"].asString().c_str();
	warnningId = root["warnningId"].asString().c_str();
	price = tool::round(root["price"].asDouble() / Magnification_Alert, 2);

	params_x = root["params"][index_Ratio_x].asDouble() / (Magnification_Alert / 100);
	params_y = root["params"][index_Ratio_y].asDouble() / (Magnification_Alert / 100);

	returnVals_x = root["returnVals"][index_Ratio_x].asDouble() / (Magnification_Alert / 100);
	returnVals_y = root["returnVals"][index_Ratio_y].asDouble() / (Magnification_Alert / 100);

	CSecurity obj_CSecurity;
	if (SPtr_CData->GetSecurity(marketCode, secCode, obj_CSecurity) == false)
	{
		ErrorLog("GetSecurity fail");
		return false;
	}
	CAlertNeed obj_CAlertNeed;
	if (SPtr_CData->GetAlert(warnningId, MarketCode2BangSheng(marketCode), secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;
	obj_CAlertNeed.s_bBiaoDiQuan = obj_CSecurity.s_bBiaoDiQuan;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals_y;

	if (tool::round(returnVals_x, 2) > params_x || tool::round(returnVals_y, 2) < params_y)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, MarketCode2BangSheng(marketCode),secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, MarketCode2BangSheng(marketCode),obj_CAlertNeed) == false)
		{
			ErrorLog("SetAlert fail");
			return false;
		}
	}
	return true;
}

CString CAlertSpi::Convert_MarketCode(string marketCode)
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

CString CAlertSpi::MarketCode2BangSheng(CString marketCode)
{
	CString s_dst;
	if (marketCode == SSE)
	{
		s_dst = "sh";
	}
	else if (marketCode == SZSE)
	{
		s_dst = "sz";
	}
	else
	{
		ErrorLog("abnormal marketCode");
	}
	return s_dst;
}

void CAlertSpi::FillJumpTime(CString &s_str)
{
	char buf[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf(buf, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
	s_str = buf;
}

bool CAlertSpi::DealOnAlertEnd(const bool &bActivity_TradeData, const HWND &hwnd)
{
	if (true == bActivity_TradeData)
	{
		bPostMessage = true;
	}
	return true;
}

bool CAlertSpi::DealOnClose(bool bBeginRelease,const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo)
{
	if (bRun_MainPageDlg == true && false == bBeginRelease)
	{
		LOG(ERROR) << errorInfo;
		/*异常退出程序*/
		MessageBox(hwnd, errorInfo, "error", 0);
		/*退出程序*/
		SendMessage(hwnd, WM_AbnormalExit, 0, 0);
	}
	return true;
}

bool CAlertSpi::dll_InitCData(PtrData SPtr_CDataTmp)
{
	SPtr_CData = SPtr_CDataTmp;
	return true;
}