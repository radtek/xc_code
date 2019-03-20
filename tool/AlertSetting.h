#pragma once
// CFastTradeDlg ¶Ô»°¿ò
#include "../share/share.h"
#include "afxdialogex.h"
#include "ControlFun.h"
#include "tool.h"

class CAlertSetting :public CControlFun
{
	public:
		string s_ConfigPath;
		PtrData SPtr_CData;
		CApiFun *pCApiFun;
		CAlertSetting(PtrData SPtr_CDataTmp, CApiFun *pCApiFunTmp);

		bool initAlertSettingNeed(string Node_Father);
		bool ResetMonitor();
		bool SendResetMonitor(pAlertSendOrder SendOrder_Alert, int alertType);
		bool RequireMonitor(string path_CustomPool);
		bool Monitor_All(Json::Value &params, string warnningId, bool bFilterNewStock);
		bool Monitor_Config(Json::Value &params, string s_path, string warnningId);

		void FillParams_IncreaseRatio(Json::Value &params);
		void FillParams_N_Increase(Json::Value &params);
		void FillParams_Low_Rise(Json::Value &params);

		void FillParams_EntrustEeach(Json::Value &params);

		void InitAlertList(string &s_dst, Json::Value &root,bool bSSE);
		void FillItem(Json::Value &root, string marketId, Json::Value &params, string secCode, string warnningId);
		bool SendOrder(pAlertSendOrder SendOrder_Alert, string s_str);

		bool FillReqList(string s_Path, Json::Value &root_SSE, Json::Value &root_SZSE, string warnningId, Json::Value &params);
};
