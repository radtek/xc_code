#include "ControlFun.h"

/*fixing 精读程序员的自我修养，精读整个编译、链接过程，解决新框架dll问题，解决编译、链接缓慢问题*/
void CControlFun::OnlyAllowDotOrNumber(HWND &hwnd_Dlg, const int CtrlId)
{
	CString csAreaS;
	CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->GetWindowText(csAreaS);
	// 只允许输数据
	int nStringLength = csAreaS.GetLength();
	int nDotCount = 0;
	// 点字符不能多于1个
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (csAreaS[nIndex] == '.')
		{
			nDotCount++;
			if (nDotCount > 1)
			{
				CString csTmp;
				csTmp = csAreaS.Left(nIndex);
				csTmp += csAreaS.Right(csAreaS.GetLength() - nIndex - 1);
				//csRadius = csRadius.Left( nIndex + 1 ) + csRadius.Right( nStringLength - ( nIndex + 1 ) - 1 );
				CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(csTmp);
				return;
			}
		}
	}

	// 不允许输入数字和点以外的字符
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (csAreaS[nIndex] != '.' && (csAreaS[nIndex] > '9' || csAreaS[nIndex] < '0'))
		{
			csAreaS = csAreaS.Left(nIndex) + csAreaS.Right(csAreaS.GetLength() - nIndex - 1);
			CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(csAreaS);
			return;
		}
	}
}

void CControlFun::OnlyAllowDouble(HWND &hwnd_Dlg, const int CtrlId)
{
	CString s_str;
	CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->GetWindowText(s_str);
	// 只允许输数据
	int nStringLength = s_str.GetLength();
	int nDotCount = 0;
	int count_hyphen = 0;
	// 点字符不能多于1个
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (s_str[nIndex] == '.')
		{
			nDotCount++;
			if (nDotCount > 1)
			{
				CString csTmp;
				csTmp = s_str.Left(nIndex);
				csTmp += s_str.Right(s_str.GetLength() - nIndex - 1);
				CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(csTmp);
				return;
			}
		}

		if (s_str[nIndex] == '-')
		{
			count_hyphen++;
			if (count_hyphen > 1)
			{
				CString csTmp;
				csTmp = s_str.Left(nIndex);
				csTmp += s_str.Right(s_str.GetLength() - nIndex - 1);
				CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(csTmp);
				return;
			}
		}
	}

	// 不允许输入数字和点以外的字符
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (s_str[nIndex] != '-' && s_str[nIndex] != '.' && (s_str[nIndex] > '9' || s_str[nIndex] < '0'))
		{
			s_str = s_str.Left(nIndex) + s_str.Right(s_str.GetLength() - nIndex - 1);
			CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(s_str);
			return;
		}
	}
}

void CControlFun::OnlyAllowPositiveDouble(HWND &hwnd_Dlg, const int CtrlId)
{
	CString s_str;
	CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->GetWindowText(s_str);
	// ??????
	int nStringLength = s_str.GetLength();
	int nDotCount = 0;
	// ???????1?
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (s_str[nIndex] == '.')
		{
			nDotCount++;
			if (nDotCount > 1)
			{
				CString csTmp;
				csTmp = s_str.Left(nIndex);
				csTmp += s_str.Right(s_str.GetLength() - nIndex - 1);
				CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(csTmp);
				return;
			}
		}
	}

	// ??????????????
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (s_str[nIndex] != '.' && (s_str[nIndex] > '9' || s_str[nIndex] < '0'))
		{
			s_str = s_str.Left(nIndex) + s_str.Right(s_str.GetLength() - nIndex - 1);
			CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(s_str);
			return;
		}
	}
}
void CControlFun::OnlyPositiveInteger(HWND &hwnd_Dlg, const int CtrlId)
{
	CString s_str;
	CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->GetWindowText(s_str);
	// 只允许输数据
	int nStringLength = s_str.GetLength();
	int nDotCount = 0;
	int count_hyphen = 0;

	if (s_str[0] == '0')
	{
		CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText("");
		return;
	}
	
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (s_str[nIndex] > '9' || s_str[nIndex] < '0')
		{
			s_str = s_str.Left(nIndex) + s_str.Right(s_str.GetLength() - nIndex - 1);
			CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId))->SetWindowText(s_str);
			return;
		}
	}
}

void CControlFun::SaveCComboBox(CComboBox &obj_CComboBox, CString &s_dst, ptree &pt, string Path_Node)
{
	obj_CComboBox.GetWindowText(s_dst);
	pt.put(Path_Node, s_dst);
}

void CControlFun::SaveCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node)
{
	string s_str;
	CButton* pCButton = (CButton*)CWnd::FromHandle(GetDlgItem(hwnd_Dlg, CtrlId));
	if (pCButton->GetCheck() == BST_CHECKED)
	{
		bFlag = 1;
		s_str = "1";
	}
	else
	{
		bFlag = 0;
		s_str = "0";
	}
	pt.put(Path_Node, s_str);
}


void CControlFun::DealClickbRandom(HWND &hwnd_Dlg, const int ID_bRandom, const int ID_Order_Minimum, const int ID_Order_Maximum)
{
	CButton* pCButton = (CButton*)CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_bRandom));
	if (pCButton->GetCheck() == BST_CHECKED)
	{
		CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_Order_Minimum))->EnableWindow(TRUE);
		CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_Order_Maximum))->EnableWindow(TRUE);
	}
	else
	{
		CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_Order_Minimum))->EnableWindow(FALSE);
		CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_Order_Maximum))->EnableWindow(FALSE);
	}
}
void CControlFun::SaveRadioGroup_StockTemplate(HWND &hwnd_Dlg, UINT ID_AllMarket, UINT ID_CustomizePool,
	string &s_Template_Stock, size_t &Template_Stock)
{
	UINT nSex;
	nSex = CWnd::FromHandle(hwnd_Dlg)->GetCheckedRadioButton(ID_AllMarket, ID_CustomizePool);
	if (ID_AllMarket == nSex)
	{
		s_Template_Stock = convert<string>(AllMarket);
		Template_Stock = AllMarket;
	}
	else if (ID_CustomizePool == nSex)
	{
		s_Template_Stock = convert<string>(CustomizePool);
		Template_Stock = CustomizePool;
	}
	else
	{
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal nSex";
	}
}

void CControlFun::SaveRadioGroup_AlertTemplate(HWND &hwnd_Dlg, UINT ID_Alert1, UINT ID_Alert2, UINT ID_Alert3,
	string &s_Template_Alert, size_t &Template_Alert)
{
	UINT nSex;
	nSex = CWnd::FromHandle(hwnd_Dlg)->GetCheckedRadioButton(ID_Alert1, ID_Alert3);
	if (ID_Alert1 == nSex)
	{
		Template_Alert = enum_Increase;
		s_Template_Alert = convert<string>(Template_Alert);
	}
	else if (ID_Alert2 == nSex)
	{
		Template_Alert = enum_N_Increase;
		s_Template_Alert = convert<string>(Template_Alert);
	}
	else if (ID_Alert3 == nSex)
	{
		Template_Alert = enum_Low_Rise;
		s_Template_Alert = convert<string>(Template_Alert);
	}
	else
	{
		ErrorLog("abnormal nSex");
	}
}

void CControlFun::SavePeriod(size_t &Period_second, size_t &Period_Day, string &s_Period_Day, HWND &hwnd_Dlg, size_t ID_Period)
{
	CString s_Period;
	CEdit *pCEdit = (CEdit *)CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_Period));
	pCEdit->GetWindowText(s_Period);
	if ("" == s_Period)
	{
		/*????""*/
		ErrorLog("abnormal s_Period_Increase");
	}
	else
	{
		Period_second = convert<unsigned int>(s_Period)*(60 * 60 * 24);
		Period_Day = convert<size_t>(string(s_Period));
	}
	s_Period_Day = s_Period;
}

bool CControlFun::VerifyFluctuateRatio_Low_Rise(HWND &hwnd_Dlg, const int ID_BeginRatio, const int ID_EndRatio)
{
	double BeginRatio = 0;
	double EndRatio = 0;
	CEdit *pCEdit = (CEdit *)CWnd::FromHandle(GetDlgItem(hwnd_Dlg, ID_BeginRatio));
	tool::GainLegalValue(BeginRatio, hwnd_Dlg, ID_BeginRatio);
	tool::GainLegalValue(EndRatio, hwnd_Dlg, ID_EndRatio);
	if (BeginRatio > EndRatio)
	{
		MessageBox(hwnd_Dlg,"低开预警参数设置错误,请重新设置","错误",MB_OK);
		ErrorLog("低开预警参数设置错误,请重新设置");/*fixed*/
		return false;
	}
	return true;
}