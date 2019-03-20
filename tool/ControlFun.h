#pragma once
// CFastTradeDlg 对话框
#include "../share/share.h"
#include "afxdialogex.h"
#include "tool.h"

class CControlFun
{
	public:
		static void OnlyAllowDotOrNumber(HWND &hwnd_Dlg, const int CtrlId);

		static void OnlyAllowDouble(HWND &hwnd_Dlg, const int CtrlId);

	    static void OnlyAllowPositiveDouble(HWND &hwnd_Dlg, const int CtrlId);
		static void OnlyPositiveInteger(HWND &hwnd_Dlg, const int CtrlId);

		void SaveCComboBox(CComboBox &obj_CComboBox, CString &s_dst, ptree &pt, string Path_Node);
		void SaveCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node);
		void DealClickbRandom(HWND &hwnd_Dlg, const int ID_bRandom, const int ID_Order_Minimum, const int ID_Order_Maximum);
	    void SaveRadioGroup_StockTemplate(HWND &hwnd_Dlg,UINT ID_AllMarket, UINT ID_CustomizePool,
			string &s_Template_Stock, size_t &Template_Stock);
		void SaveRadioGroup_AlertTemplate(HWND &hwnd_Dlg, UINT ID_Alert1, UINT ID_Alert2, UINT ID_Alert3,
			string &s_Template_Alert, size_t &Template_Alert);
		void SavePeriod(size_t &Period_second,size_t &Period_Day, string &s_Period_Day, HWND &hwnd_Dlg, size_t ID_Period);
		bool VerifyFluctuateRatio_Low_Rise(HWND &hwnd_Dlg, const int ID_BeginRatio,const int ID_EndRatio);


public:
		template<class T>
		void SaveCEdit(HWND &hwnd_Dlg, const int CtrlId, T &t, ptree &pt, string Path_Node)
		{
			CString s_str;
			CEdit* pCEdit = (CEdit*)CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId));
			pCEdit->GetWindowText(s_str);
			if ("" == s_str)
			{
				t = 0;
				s_str = "0";
				//ErrorLog("输入的数字非法");
			}
			else
			{
				t = convert<T>(s_str);
			}

			pt.put(Path_Node, s_str);
		}

};
