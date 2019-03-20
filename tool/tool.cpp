#include "tool.h"

string tool::addSuffix(int type, string s_str)
{
	switch (type)
	{
	case AlertType_SSE:
		s_str += SSE;
		break;
	case AlertType_SZSE:
		s_str += SZSE;
		break;
	case AlertType_Auto:
		s_str += Auto;
		break;
	default:
		ErrorLog("abnormal type");
		break;
	}
	return s_str;
}

int tool::bool2Int(bool bTrue)
{
	if (true == bTrue)
	{
		return 1;
	}
	return 0;
}

double tool::round(double number, unsigned int bits)
{
	ASSERT(bits != 0);
	int dividend = pow(10, bits);
	number = floor(number*dividend + 0.5);
	number /= dividend;
	return number;
}

string& tool::replace_all(string &str, const string &old_value, const string &new_value)
{
	while (true) {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
	}
	return str;
}

int tool::GainRuleValue(bool bIncrease, double PrevClosePx, double FluctuateRatio)
{
	double RuleValue;
	switch (bIncrease)
	{
	case true:
		RuleValue = PrevClosePx *(1 + FluctuateRatio / 100);
		break;
	case false:
		RuleValue = PrevClosePx *(1 - FluctuateRatio / 100);
		break;
	}
	return int(RuleValue * 1000);
}

void tool::ExceptionHandle_RecvMsg()
{
	//MessageBox(NULL, "pRecvMsg fail", ERROR, MB_OK);
	LOG(ERROR) << "pRecvMsg fail";/*studying*/
}

void tool::initExePath(string &s_ExePath)
{
	char szPathTemp[512];
	GetModuleFileName(NULL, szPathTemp, 512);
	for (int i = strlen(szPathTemp); i >= 0; i--)
	{
		if (szPathTemp[i] == '\\')
		{
			szPathTemp[i] = '\0';
			break;
		}
	}
	s_ExePath = szPathTemp;
}

void tool::MyFreeLibrary(HINSTANCE hInst)
{
	if (hInst != NULL)
	{
		if (FreeLibrary(hInst) == 0)
		{
			LOG(ERROR) << "GetLastError:" << GetLastError;
			ExceptionInterrupt;
		}
	}
}

void tool::SetEvent_EntrustQueue(int Grade, bool &bTrigger_EntrustQueue_B1, bool &bTrigger_EntrustQueue_S1)
{
	switch (Grade)
	{
	case -1:
		bTrigger_EntrustQueue_B1 = true;
		break;
	case 1:
		bTrigger_EntrustQueue_S1 = true;
		break;
	default:
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal Grade";
		break;
	}
}

bool tool::GainFileContent(string s_path, string &s_dst)
{
	int length;
	FILE *fp;
	fopen_s(&fp, s_path.c_str(), "r");
	if (!fp)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "s_Path no exsit";
		return false;
	}
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *buf = new char[length + 1];
	fread(buf, 1, length, fp);
	s_dst = buf;
	delete[]buf;
	fclose(fp);
	return true;
}

void tool::File2String(string s_Path, string &s_dst)
{
	std::ifstream t(s_Path);
	std::string s_FileContent((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	t.close();
	s_dst = s_FileContent;
}

void tool::string2File(string s_path, string s_str)
{
	myCreateDirectory(s_path,true);
	ofstream fout;
	fout.open(s_path);
	fout << s_str << endl;;
	fout.close();
}

string tool::DealReportTime(string s_str)
{
	int Length = s_str.length();
	char buf[64] = { 0 };
	string s_dst;
	string s_hour;
	string s_minute;
	string s_second;
	string s_millisecond;
	switch (Length)
	{
	case 5://92223
		s_hour = s_str.substr(0, 1);
		s_minute = s_str.substr(1, 2);
		s_second = s_str.substr(3, 2);
		sprintf_s(buf, 64, "%s:%s:%s", s_hour.c_str(), s_minute.c_str(), s_second.c_str());
		break;
	case 6://102223
		s_hour = s_str.substr(0, 2);
		s_minute = s_str.substr(2, 2);
		s_second = s_str.substr(4, 2);
		sprintf_s(buf, 64, "%s:%s:%s", s_hour.c_str(), s_minute.c_str(), s_second.c_str());
		break;
	case 8://92233123 ms
		s_hour = s_str.substr(0, 1);
		s_minute = s_str.substr(1, 2);
		s_second = s_str.substr(3, 2);
		s_millisecond = s_str.substr(5, 3);
		sprintf_s(buf, 64, "%s:%s:%s.%s", s_hour.c_str(), s_minute.c_str(), s_second.c_str(), s_millisecond.c_str());
		break;
	case 9://102233123 ms
		s_hour = s_str.substr(0, 2);
		s_minute = s_str.substr(2, 2);
		s_second = s_str.substr(4, 2);
		s_millisecond = s_str.substr(6, 3);
		sprintf_s(buf, 64, "%s:%s:%s.%s", s_hour.c_str(), s_minute.c_str(), s_second.c_str(), s_millisecond.c_str());
		break;
	default:
		s_dst = s_str;
		break;
	}
	s_dst = buf;
	return s_dst;
}

bool tool::DealRet_AlertSendOrder(int ret, HWND &hwnd_Dlg)
{
	if (ret < 0)
	{
		string s_Error = "预警主站连接失败，error_info:";
		switch (ret)
		{
		case -1:
			s_Error += "未链接";
			break;
		case -2:
			s_Error += "发送订阅数据失败";
			break;
		default:
			ErrorLog("abnormal ret)");
			break;
		}
		LOG(ERROR) << s_Error;
		MessageBox(hwnd_Dlg, s_Error.c_str(), "错误", NULL);
		return false;
	}
	return true;
}

string tool::deleteBackSlash(string s_str)
{
	s_str.erase(std::remove(s_str.begin(), s_str.end(), '\\'), s_str.end());
	return s_str;
}

int tool::SendOrder(CApiFun *pCApiFun, const int ApiType, const string &s_Fun, PtrData &SPtr_CData)
{
	if (SPtr_CData->bBeginRelease)
	{
		return false;
	}
	DWORD ret_Event = WaitForSingleObject(SPtr_CData->Event_SendOrder, INFINITE);
	switch (ret_Event)
	{
	case WAIT_ABANDONED:
		ExceptionInterrupt;
		LOG(ERROR) << "WAIT_ABANDONED";
		return -1;
		break;
	case WAIT_TIMEOUT:
		ExceptionInterrupt;
		LOG(ERROR) << "WAIT_TIMEOUT";
		return -1;
		break;
	case WAIT_FAILED:
		ExceptionInterrupt;
		LOG(ERROR) << "WAIT_FAILED";
		return -1;
		break;
	default:
		break;
	}
	CXcTradeApi *pApi_send = NULL;
	switch (ApiType)
	{
	case Api_Asy:
		pApi_send = pCApiFun->Api_T2_Asy;
		break;
	case Api_Syn:
		pApi_send = pCApiFun->Api_T2_Syn;
		break;
	case Api_Syn_SSE:
		pApi_send = pCApiFun->Api_T2_SSE_Syn;
		break;
	case Api_Syn_SZSE:
		pApi_send = pCApiFun->Api_T2_SZSE_Syn;
		break;
	default:
		ErrorLog("abnormal ApiType");
		break;
	}
	int function_id;
	if (GainSpecificField(function_id, s_Fun, "function_id") == false)
	{
		return -1;
	}
	int ret = pApi_send->SetJsonValue(s_Fun.c_str());
	if (ret != 0)
	{
		ErrorLog(pApi_send->GetLastErrorMsg());
		return -1;
	}
	ret = pApi_send->SendMsg(function_id, convert<int>(SPtr_CData->sysnode_id), convert<int>(SPtr_CData->branch_no));
	if (ret < 0)
	{
		ErrorLog(pApi_send->GetLastErrorMsg());/*studying 莫名的数据接收错误*/
		return false;
	}
	if (333002 == function_id || 335002 == function_id)
	{
		SetM_OrderList(ret, s_Fun, SPtr_CData);
	}
	//发送成功设为有信号，允许别的线程访问
	SetEvent(SPtr_CData->Event_SendOrder);
	return ret;
}

void tool::SetM_OrderList(int ref_id, const string &s_Fun, PtrData &SPtr_CData)
{
	Json::Reader reader;
	Json::Value root;
	COrder obj_COrder;

	if (!reader.parse(s_Fun, root, false))
	{
		LOG(ERROR) << "parse error";
		return;
	}

	obj_COrder.stock_code = root["stock_code"].asCString();
	if (SPtr_CData->SecCode2SecName(obj_COrder.stock_code, obj_COrder.stock_name) == false)
	{
		ErrorLog("SecCode2SecName fail");
	}
	obj_COrder.entrust_bs = find_EnumDirection(root["entrust_bs"].asCString());

	double entrust_price = convert<double>(root["entrust_price"].asString());
	obj_COrder.entrust_price = round(entrust_price, 2);

	UInt_64 entrust_amount = convert<UInt_64>(root["entrust_amount"].asString());
	obj_COrder.entrust_amount = entrust_amount;

	SPtr_CData->M_OrderList[ref_id] = obj_COrder;
}

CWnd* tool::CtrlId2CWnd(HWND Hwnd_Dlg, const int CtrID)
{
	return CWnd::FromHandle(GetDlgItem(Hwnd_Dlg, CtrID));
}

void tool::SetCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId)
{
	CButton *pCButton = (CButton*)CtrlId2CWnd(Hwnd_Dlg, CtrlId);
	if (bFlag == true)
	{
		pCButton->SetCheck(BST_CHECKED);
	}
	else
	{
		pCButton->SetCheck(BST_UNCHECKED);
	}
}

void tool::SetEdit_Int(HWND Hwnd_Dlg, int Value, const int CtrlId)
{
	CEdit *pCEdit = (CEdit*)CtrlId2CWnd(Hwnd_Dlg, CtrlId);
	CString s_tmp;
	s_tmp.Format(_T("%d"), Value);
	pCEdit->SetWindowText(s_tmp);
}

void tool::SetEdit_double(HWND Hwnd_Dlg, const double &Value, const int CtrlId)
{
	CEdit *pCEdit = (CEdit*)CtrlId2CWnd(Hwnd_Dlg, CtrlId);
	CString s_tmp;
	s_tmp.Format(_T("%0.2f"), Value);
	pCEdit->SetWindowText(s_tmp);
}

bool tool::GainSpecificField(int &function_id, string s_src, string filedName)
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(s_src, root, false))
	{
		ExceptionInterrupt;
		cerr << "Error: " << s_src;
	}
	function_id = convert<int>(root[filedName].asString());
	return true;
}

bool tool::VerifyElement(const Value &root, const int element, PtrData &SPtr_CData)
{
	bool bLegal = false;
	/*先查找error_no是否存在，如果存在则其为0时合法，其余非法，如果不存在则合法*/
	if (root["json"][element]["error_risk_id"].isNull() == false)
	{
		//存在
		ExceptionInterrupt;
		string s_ErroInfo = root["function_id"].asString();
		s_ErroInfo += " ";
		s_ErroInfo += root["json"][element]["error_risk_msg"].asString();
		s_ErroInfo += "\n";
		MessageBox(SPtr_CData->hwnd_TradeDlg, s_ErroInfo.c_str(), "风控", MB_OK);
		LOG(ERROR) << s_ErroInfo;
		return false;
	}
	if (root["json"][element]["error_no"].isNull())
	{
		/*未找到*/
		bLegal = true;
	}
	else
	{
		if ("0" != (root["json"][element])["error_no"].asString())
		{
			string s_ErroInfo;
			s_ErroInfo = root["function_id"].asString();
			s_ErroInfo += ":";
			s_ErroInfo += root["json"][element]["error_info"].asString();
			TrimeErrorInfo(s_ErroInfo);
			SPtr_CData->Queue_T2ErrorInfo.push(s_ErroInfo);
			ErrorLog(s_ErroInfo);
			return false;
		}
		else
		{
			bLegal = true;
		}
	}

	return bLegal;
}

bool tool::VerifyElement(const Value &root, const int element, PtrData &SPtr_CData, CString &s_note,bool &bStopTrade)
{
	bool bLegal = false;
	/*先查找error_no是否存在，如果存在则其为0时合法，其余非法，如果不存在则合法*/
	if (root["json"][element]["error_risk_id"].isNull() == false)
	{
		string s_ErroInfo = root["function_id"].asString();
		s_ErroInfo += " ";
		s_ErroInfo += root["json"][element]["error_risk_msg"].asString();
		s_ErroInfo += "\n";
		LOG(ERROR) << s_ErroInfo;
		s_note = s_ErroInfo.c_str();
		return false;
	}
	if (root["json"][element]["error_no"].isNull())
	{
		/*未找到*/
		bLegal = true;
	}
	else
	{
		if ("0" != (root["json"][element])["error_no"].asString())
		{
			string s_ErroInfo;
			s_ErroInfo = root["function_id"].asString();
			s_ErroInfo += ":";
			s_ErroInfo += root["json"][element]["error_info"].asString();
			TrimeErrorInfo(s_ErroInfo);
			ErrorLog(s_ErroInfo);
			bStopTrade = true;
			MessageBox(SPtr_CData->hwnd_TradeDlg, s_ErroInfo.c_str(), "提示,可能引起秒级延迟", MB_OK);
			return false;
		}
		else
		{
			bLegal = true;
		}
	}

	return bLegal;
}

void tool::TrimeErrorInfo(string &s_str)
{
	if (s_str.length() <= 0)
	{
		return;
	}
	int pos = s_str.find("\n");
	s_str = s_str.substr(0, pos);
}

bool tool::Verify_bNewStock(PtrSecurity pSecurity, int Second_Period)
{
	string s_str = pSecurity->StartDate;
	int year = convert<int>(s_str.substr(0, 4));
	int month = convert<int>(s_str.substr(4, 2));
	int day = convert<int>(s_str.substr(6, 2));

	struct tm stm;
	memset(&stm, 0, sizeof(stm));
	stm.tm_year = year - 1900;
	stm.tm_mon = month - 1;
	stm.tm_mday = day;
	int Time_tStandard = (int)mktime(&stm) + Second_Period;

	time_t now;
	int unixTime = (int)time(&now);

	if (Time_tStandard >= unixTime)
	{
		return false;
	}
	return true;
}

void tool::WriteCheckList(PtrData SPtr_CData, const Value &root, const int i, CString stock_account)
{
	string s_str;
	s_str = root["json"][i]["report_no"].asString();
	s_str += "_";
	s_str += stock_account;

	UInt_64 entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());
	cerr << s_str << " " << entrust_no <<" entrust_status:"<< root["json"][i]["entrust_status"].asString() <<endl;//fixing
	SPtr_CData->M_CheckList[s_str] = entrust_no;
}

enumPriceType tool::find_enumPriceType(const std::string & str)
{
	static std::map<std::string, enumPriceType> M_enumPriceType =
	{
		{ "涨停价", enum涨停价 },
		{ "跌停价", enum跌停价 },
		{ "买一价", enum买一价 },
		{ "买二价", enum买二价 },
		{ "买三价", enum买三价 },

		{ "买四价", enum买四价 },
		{ "买五价", enum买五价 },
		{ "卖一价", enum卖一价 },
		{ "卖二价", enum卖二价 },
		{ "卖三价", enum卖三价 },

		{ "卖四价", enum卖四价 },
		{ "卖五价", enum卖五价 }
	};

	return M_enumPriceType[str];
}

enumEntrustStatus tool::find_enumEntrustStatus(const std::string & str)
{
	static std::map<std::string, enumEntrustStatus> M_enumEntrustStatus =
	{
		{ "未报", WeiBao },
		{ "待报", DaiBao },
		{ "已报", YiBao },
		{ "已报待撤", YiBaoDaiChe },
		{ "部成待撤", BuChengDaiChe },

		{ "部撤", BuChe },
		{ "已撤", YiChe },
		{ "部成", BuCheng },
		{ "已成", YiCheng },
		{ "废单", FeiDan }
	};

	return M_enumEntrustStatus[str];
}

enumMarketCode tool::find_enumMarketCode(const std::string & str)
{
	static std::map<std::string, enumMarketCode> M_enumMarketCode =
	{
		{ SSE, enumSSE },
		{ SZSE, enumSZSE },
		{ HGT, enumHGT },
		{ SGT, enumSGT }
	};
	return M_enumMarketCode[str];
}

enumEntrustType tool::find_enumEntrustType(const std::string & str)
{
	static std::map<std::string, enumEntrustType> M_enumEntrustType =
	{
		{ CollateralBuy, enumXinYongJiaoYi },
		{ CollateralSell, enumXinYongJiaoYi },
		{ FinancingBuy, enumXinYongRongZi },
		{ FinancingSell, enumXinYongRongZi },
	};
	return M_enumEntrustType[str];
}

enumSide_EntrustEach tool::find_enumSide_EntrustEach(const std::string & str)
{
	static std::map<std::string, enumSide_EntrustEach> M_enumSide_EntrustEach =
	{
		{ "B", enumBuy },
		{ "S", enumSell },
		{ "BC", enumBC },
		{ "SC", enumSC },
	};
	return M_enumSide_EntrustEach[str];
}

enumWarnningId tool::find_warnningId(const std::string & str)
{
	static std::map<std::string, enumWarnningId> M_WarnningId =
	{
		{ warnningId_Deal, enum_Deal },
		{ warnningId_Revoke, enum_Revoke },
		{ warnningId_Entrust, enum_Entrust },
		{ warnningId_N_Increase, enum_N_Increase },

		{ warnningId_Low_Rise, enum_Low_Rise },
		{ warnningId_Increase, enum_Increase },
		{ warnningId_N_Decline, enum_N_Decline },
		{ warnningId_Decline, enum_Decline }
	};
	return M_WarnningId[str];
}

_EnumDirection tool::find_EnumDirection(const std::string & str)
{
	static std::map<std::string, _EnumDirection> M_enumDirection =
	{
		{ entrust_bs_买入,_Buy },
		{ entrust_bs_卖出, _Sell },
	};
	return M_enumDirection[str];
}

void tool::FillElement(CString &s_dst, CString s_elementName, CString s_element)
{
	s_dst += s_elementName;
	s_dst += s_element;
	s_dst += "\n";
}

void tool::MyReleae(p_Release Release)
{
	if (Release != NULL)
	{
		Release();
	}
}

void tool::MyPlaySound(string path)
{
	PlaySound(TEXT(path.c_str()), NULL, SND_FILENAME | SND_ASYNC);//s_path_AlertSound
}

bool tool::myCreateDirectory(string filePath, bool bFilePath)
{
	string path_directory;
	if (bFilePath)
	{
		char key = ' ';
		if (string::npos == filePath.find_last_of('\\'))
		{
			key = '/';
		}
		else
		{
			key = '\\';
		}
		int n = filePath.find_last_of(key);
		path_directory = filePath.substr(0, n+1);
	}
	else
	{
		path_directory = filePath;
	}
	replace_all(path_directory, "/", "\\");
	int ret = PathFileExistsA((char *)path_directory.c_str());
	if (ret != 1)
	{
		string commond = "md " + path_directory;
		system(commond.c_str());
	}
	return true;
}

bool tool::MyPostMessage(HWND hwdn, const int MessageId, bool bPostMessage)
{
	if (!bPostMessage)
	{
		return true;
	}
	if (PostMessage(hwdn, MessageId, 0, 0) == 0)
	{
		LOG(ERROR) << "PostMessage fail," << MessageId;
		ExceptionInterrupt;
		return false;
	}
	return true;
}

void tool::GainQueryFund335101(string &s_str, PtrData &SPtr_CData)
{
	//客户资金精确查询
	Json::Value obj_Value;
	obj_Value["function_id"] = "335101";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["asset_prop"] = AssetProp_信用账户;

	obj_Value["money_type"] = money_type_人民币;
	s_str = obj_Value.toStyledString();
	return;
}

void tool::GainQueryFund332255(string &s_str, PtrData &SPtr_CData)
{
	//客户资金精确查询
	Json::Value obj_Value;
	obj_Value["function_id"] = "332255";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;
	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;

	obj_Value["op_station"] = op_station_My;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["money_type"] = money_type_人民币;
	s_str = obj_Value.toStyledString();
}

void tool::GainQueryCreditAsset335504(string &s_str, PtrData &SPtr_CData)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "335504";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;

	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["money_type"] = money_type_人民币;

	s_str = obj_Value.toStyledString();
}

void tool::GainResetAlert(string &s_dst, PtrData &SPtr_CData, int alertType)
{
	string partitionId;
	string userId;
	partitionId = tool::addSuffix(alertType, SPtr_CData->partitionId);
	userId = tool::addSuffix(alertType, SPtr_CData->user_alert);
	Json::Value obj_Value;
	obj_Value["msgType"] = MsgType_Reset;
	obj_Value["refferId"] = "3";
	obj_Value["partitionId"] = partitionId;
	obj_Value["userId"] = userId;
	s_dst = obj_Value.toStyledString();
}

bool tool::GainSubscribeDeal620001(string &s_str, PtrData &SPtr_CData)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "620001";
	obj_Value["op_station"] = op_station_My;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["issue_type"] = issue_type_成交推送;

	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	s_str = obj_Value.toStyledString();
	return true;
}

bool tool::GainSubscribeOrder620001(string &s_str, PtrData &SPtr_CData)
{
	Json::Value obj_Value;
	obj_Value["function_id"] = "620001";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["op_station"] = op_station_My;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password"] = SPtr_CData->s_pass;

	obj_Value["issue_type"] = issue_type_委托推送;
	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	s_str = obj_Value.toStyledString();
	return true;
}

void tool::GetOrderInfo(CString &s_dst, string s_Order, PtrData SPtr_CData)
{
	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(s_Order, root, false))
	{
		LOG(ERROR) << "parse error";
		return;
	}
	CString stock_code = root["stock_code"].asCString();
	CString entrust_price = root["entrust_price"].asCString();
	CString entrust_amount = root["entrust_amount"].asCString();
	CString entrust_bs;
	if (entrust_bs_买入 == root["entrust_bs"].asString())
	{
		entrust_bs = "买入";
	}
	else
	{
		entrust_bs = "卖出";
	}
	CString stock_name;
	if (SPtr_CData->SecCode2SecName(stock_code, stock_name) == false)
	{
		ErrorLog("SecCode2SecName fail");
		return;
	}
	FillElement(s_dst, "               买卖方向:", entrust_bs);
	FillElement(s_dst, "               股票代码:", stock_code);
	FillElement(s_dst, "               股票名称:", stock_name);
	FillElement(s_dst, "               委托价格:", entrust_price);
	FillElement(s_dst, "               委托数量:", entrust_amount);
	s_dst += "               您确认发出委托吗?";
}

CString tool::GainLocalTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	return strTime;
}

UInt_64 tool::GetTimestamp_ms()
{
	auto timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
	UInt_64 time = timeNow.count();
	return time;
}

void tool::getAlertRecordPath(string &s_path,string s_str)
{
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	s_path = "..\\share\\存档数据\\";
	s_path += s_str;
	s_path += "\\";
	s_path += convert<string>(systime.wYear);
	s_path += convert<string>(systime.wMonth);
	s_path += "x";
	s_path += convert<string>(systime.wDay);
	s_path += "_预警列表.xml";
	return;
}

bool tool::initSubscribe(PtrData &SPtr_CData, CApiFun *pCApiFun, const int ApiType)
{
	string s_SubscribeDeal;
	GainSubscribeDeal620001(s_SubscribeDeal, SPtr_CData);
	if (SendOrder(pCApiFun, ApiType, s_SubscribeDeal, SPtr_CData) < 0)
	{
		return false;
	}

	//订阅（委托）
	string s_SubscribeOrder;
	GainSubscribeOrder620001(s_SubscribeOrder, SPtr_CData);
	if (SendOrder(pCApiFun, ApiType, s_SubscribeOrder, SPtr_CData) < 0)
	{
		return false;
	}
	return true;
}
