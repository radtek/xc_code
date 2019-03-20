#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include "AlertSpi.h"

using namespace std;

CAlertSpi XczbClientSpi;


/*SPI_OnData("{\"msgType\":\"91\",""\"msgDec\":\"链接成功\"}");
SPI_OnData("{\"msgType\":\"90\",""\"msgDec\":\"正在重连\"}");*/
void CAlertSpi::OnData(char* strJson, bool bSSE)
{
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
	case MsgType_abnormal:
		break;
	default:
		ErrorLog("abnormal msgType");
		break;
	}
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
			SetEvent(SPtr_CData->ConnectPartition_SSE_Event);
		}
		else
		{
			SetEvent(SPtr_CData->ConnectPartition_SZSE_Event);
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
			SetEvent(SPtr_CData->LoginSSEAlert_Event);
		}
		else
		{
			SetEvent(SPtr_CData->LoginSZSEAlert_Event);
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
	switch (tool::find_warnningId(root["warnningId"].asString()))
	{
	case enum_Increase:
		if (FilterRsp_Increase(root) == false)
		{
			return;
		}
		DealIncrease(root,bSSE);
		break;
	case enum_N_Increase:
		if (FilterRsp_N_Increase(root) == false)
		{
			return;
		}
		Deal_N_Increase(root, bSSE);
		break;
	case enum_Low_Rise:
		if (FilterRsp_Low_Rise(root) == false)
		{
			return;
		}
		Deal_Low_Rise(root, bSSE);
		break;
	default:
		ErrorLog("abnormal warnningId");/*fixed*/
		break;
	}
	DealOnAlertEnd(SPtr_CData->bActivity_TradeData, SPtr_CData->hwnd_TradeDlg);
	return;
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
	if (SPtr_CData->GetAlert(warnningId, secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals;

	if (tool::round(returnVals, 2) < params)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, obj_CAlertNeed) == false)
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
	if (SPtr_CData->GetAlert(warnningId, secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals;

	if (tool::round(returnVals, 2) < params)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, obj_CAlertNeed) == false)
		{
			ErrorLog("SetAlert fail");
			return false;
		}
	}
	return true;
}

bool CAlertSpi::FilterRsp_N_Increase(const Value &root) 
{
	UInt_32 params = root["params"][1].asInt();
	if (SPtr_CData->obj_CAlertConfig.IncreaseRatio_N_Rise * Magnification_Alert / 100 != params)
	{
		return false;
	}
	return true;
}


bool CAlertSpi::FilterRsp_Increase(const Value &root)
{
	UInt_32 params = root["params"][0u].asInt();
	if (SPtr_CData->obj_CAlertConfig.IncreaseRatio * Magnification_Alert/100 != params)
	{
		return false;
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
	if (SPtr_CData->GetAlert(warnningId, secCode, obj_CAlertNeed) == false)
	{
		obj_CAlertNeed.Price_Jump = price;
		FillJumpTime(obj_CAlertNeed.Time_Jump);
	}

	obj_CAlertNeed.SecName = obj_CSecurity.stock_name;
	obj_CAlertNeed.SecCode = obj_CSecurity.stock_code;
	obj_CAlertNeed.PrevClosePx = obj_CSecurity.PrevClosePx;
	obj_CAlertNeed.MarketCode = marketCode;

	obj_CAlertNeed.NewPrice = price;
	obj_CAlertNeed.FluctuateRatio = returnVals_y;

	if (tool::round(returnVals_x, 2) > params_x || tool::round(returnVals_y, 2) < params_y)//跌破
	{
		/*之前超过预警线之后又跌了回来,删除现有数据，并重新预警*/
		if (SPtr_CData->deleteElement_MAlert(warnningId, secCode) == false)
		{
			LOG(ERROR) << "deleteElement_MAlert fail" << warnningId << " " << secCode;
			ExceptionInterrupt;
			return false;
		}
	}
	else
	{
		if (SPtr_CData->SetAlert(warnningId, obj_CAlertNeed) == false)
		{
			ErrorLog("SetAlert fail");
			return false;
		}
	}
	return true;
}

bool CAlertSpi::FilterRsp_Low_Rise(const Value &root)
{
	UInt_32 params = root["params"][1].asInt();
	if (SPtr_CData->obj_CAlertConfig.EndRatio_Low_Rise * Magnification_Alert / 100 != params)
	{
		return false;
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
		if (PostMessage(hwnd, WM_RedrawAlert, 0, 0) == 0)
		{
			UInt_32 ret = GetLastError();
			ErrorLog("PostMessage fail");
		}
	}
	return true;
}

bool CAlertSpi::DealOnClose(const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo)
{
	/*fixing*/
	if (bRun_MainPageDlg == true)
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