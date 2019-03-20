#pragma once
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
/*在此编写公共可用,严格按照字母顺序排列*/

class tool
{
public:
	static string addSuffix(int type, string s_str);

	static int bool2Int(bool bTrue);

	static CWnd* CtrlId2CWnd(HWND Hwnd_Dlg, const int CtrID);
	static string DealReportTime(string s_str);
	static bool DealRet_AlertSendOrder(int ret, HWND &hwnd_Dlg);
	static string deleteBackSlash(string s_str);

	static void ExceptionHandle_RecvMsg();

	static void File2String(string s_Path, string &s_dst);
	static enumPriceType find_enumPriceType(const std::string & str);
	static enumEntrustStatus find_enumEntrustStatus(const std::string & str);
	static enumMarketCode find_enumMarketCode(const std::string & str);
	static enumEntrustType find_enumEntrustType(const std::string & str);
	static enumSide_EntrustEach find_enumSide_EntrustEach(const std::string & str);
	static enumWarnningId find_warnningId(const std::string & str);
	static _EnumDirection find_EnumDirection(const std::string & str);
	static void FillElement(CString &s_dst, CString s_elementName, CString s_element);


	static int GainRuleValue(bool bIncrease, double PrevClosePx, double FluctuateRatio);
	static bool GainSpecificField(int &function_id, string s_src, string filedName);
	static bool GainFileContent(string s_path, string &s_dst);
	static void GainQueryFund335101(string &s_str, PtrData &SPtr_CData);
	static void GainQueryFund332255(string &s_str, PtrData &SPtr_CData);
	static void GainQueryCreditAsset335504(string &s_str, PtrData &SPtr_CData);
	static void GainResetAlert(string &s_dst, PtrData &SPtr_CData, int alertType);
	static bool GainSubscribeDeal620001(string &s_str, PtrData &SPtr_CData);
	static bool GainSubscribeOrder620001(string &s_str, PtrData &SPtr_CData);
	static void GetOrderInfo(CString &s_dst, string s_Order, PtrData SPtr_CData);
	static CString GainLocalTime();
	static UInt_64 GetTimestamp_ms();
	static void getAlertRecordPath(string &s_path, string s_str);

	static void initExePath(string &s_ExePath);
	static bool initSubscribe(PtrData &SPtr_CData, CApiFun *pCApiFun, const int ApiType);

	static void MyFreeLibrary(HINSTANCE hInst);
	static void MyReleae(p_Release Release);
	static void MyPlaySound(string path);
	static bool myCreateDirectory(string Path,bool bFilePath);
	static bool MyPostMessage(HWND hwdn, const int MessageId, bool bPostMessage = true);

	static double round(double number, unsigned int bits);

	static string& replace_all(string &str, const string &old_value, const string &new_value);

	static void SetEvent_EntrustQueue(int Grade, bool &bTrigger_EntrustQueue_B1, bool &bTrigger_EntrustQueue_S1);
	static void string2File(string s_path, string s_str);
	static int SendOrder(CApiFun *pCApiFun, const int ApiType, const string &s_Fun, PtrData &SPtr_CData);
	static void SetM_OrderList(int ref_id, const string &s_Fun, PtrData &SPtr_CData);
	static void SetCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId);
	static void SetEdit_Int(HWND Hwnd_Dlg, int Value, const int CtrlId);
	static void SetEdit_double(HWND Hwnd_Dlg, const double &Value, const int CtrlId);

	static void TrimeErrorInfo(string &s_str);
	
	static bool VerifyElement(const Value &root, const int element, PtrData &SPtr_CData);
	static bool VerifyElement(const Value &root, const int element, PtrData &SPtr_CData, CString &s_note, bool &bStopTrade);
	static bool Verify_bNewStock(PtrSecurity pSecurity, int Second_Period);

	/*下单应答添加(未报无推送)、委托推送里添加(多开)*/
	static void WriteCheckList(PtrData SPtr_CData,const Value &root, const int i, CString stock_account);

public://模板
	template<class T>
	static int GetColoumnSum(T &t)
	{
		int n;
		CHeaderCtrl* pHeaderCtrl = t.GetHeaderCtrl();
		if (pHeaderCtrl)
		{
			n = pHeaderCtrl->GetItemCount();
		}

		return n;
	}

	template<class in_type>
	static bool GainLegalValue(in_type &Value, HWND Hwnd_Dlg, const int CtrID)
	{
		CString s_str;
		CtrlId2CWnd(Hwnd_Dlg, CtrID)->GetWindowText(s_str);
		if ("" == s_str || "0" == s_str)
		{
			Value = 0;//目前就是UInt_64和double
			return false;
		}
		Value = convert<in_type>(s_str);
		return true;
	}
	template<class in_type>
	static UInt_64 modifyNum(in_type t)
	{
		UInt_64 number;
		UInt_64 dst;
		if (t <= 0)
		{
			number = 0;
			LOG(INFO) << "number is 0 t:" << t;
		}
		else
		{
			number = (UInt_64)t;
		}
		dst = number - number % 100;
		return dst;
	}
};

