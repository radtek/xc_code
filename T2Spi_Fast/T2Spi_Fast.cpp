#include "stdafx.h"
#include "T2Spi_Fast.h"

VOID CALLBACK OneMilliSecondProc(PVOID lpParameter, BOOLEAN ret)
{
	CT2Spi_Fast *pCT2Spi_Fast = (CT2Spi_Fast *)lpParameter;
	map<string, atomic_bool>::iterator iter;
	for (iter = pCT2Spi_Fast->M_QueryData.begin();iter != pCT2Spi_Fast->M_QueryData.end(); iter++)
	{
		if (iter->second)
		{
			string SecCode = iter->first;
			/*在任何操作导致资金或持仓变化的时候更新一次资金和持仓*/
			if (pCT2Spi_Fast->QueryFund() == false)
			{
				LOG(ERROR) << "OnQueryFund fail";
				ExceptionInterrupt;
			}

			if (pCT2Spi_Fast->QuerySinglePosition(SecCode) == false)
			{
				LOG(ERROR) << "QuerySinglePosition fail";
				ExceptionInterrupt;
			}
			iter->second = false;
		}
	}
}

VOID CALLBACK OnProc_Order(PVOID lpParameter, BOOLEAN ret)
{
	CT2Spi_Fast *pCT2Spi_Fast = (CT2Spi_Fast *)lpParameter;
	if (pCT2Spi_Fast->bPostMessage)
	{
		tool::MyPostMessage(pCT2Spi_Fast->SPtr_CData->hwnd_TradeDlg, WM_RedrawOrder, pCT2Spi_Fast->bPostMessage);
	}
	pCT2Spi_Fast->bPostMessage = false;
}

CT2Spi_Fast::CT2Spi_Fast()
{
	if (!CreateTimerQueueTimer(&hTimer, NULL, WAITORTIMERCALLBACK(OneMilliSecondProc), (PVOID)this, DueTime, Timer_ms_Query, NULL))
	{
		LOG(ERROR) << "CreateTimerQueueTimer fail";
	}
	if (!CreateTimerQueueTimer(&hTimer_Order, NULL, WAITORTIMERCALLBACK(OnProc_Order), (PVOID)this, DueTime, Timer_ms_Order, NULL))
	{
		LOG(ERROR) << "CreateTimerQueueTimer fail";
	}
}

CT2Spi_Fast::~CT2Spi_Fast()
{
	if (!DeleteTimerQueueTimer(NULL, hTimer, NULL))
	{
		LOG(ERROR) << "DeleteTimerQueueTimer fail";
	}
	if (!DeleteTimerQueueTimer(NULL, hTimer_Order, NULL))
	{
		LOG(ERROR) << "DeleteTimerQueueTimer fail";
	}
}

void CT2Spi_Fast::OnClose()
{
	if (SPtr_CData->bRun_MainPageDlg == true && false == SPtr_CData->bBeginRelease)
	{
		/*异常退出程序*/
		LOG(ERROR) << "CT2Spi_Fast Close.........";
		MessageBox(SPtr_CData->hwnd_MainPageDlg, _T("连接中断，请重新登录"), "error", 0);
		/*退出程序*/
		if (PostMessage(SPtr_CData->hwnd_MainPageDlg, WM_AbnormalExit, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
		}
	}
}

void CT2Spi_Fast::OnDisConnect()
{
	static int count = 0;
	if (count %5 == 0)
	{
		PostMessage(SPtr_CData->hwnd_TradeDlg, WM_DisConnect_T2,NULL,NULL);
	}
	count++;
}

void CT2Spi_Fast::OnConnected()
{
	PostMessage(SPtr_CData->hwnd_TradeDlg, WM_Connected_T2, NULL, NULL);

	LOG(ERROR) << "OnConnected" << endl;
	if (tool::initSubscribe(SPtr_CData, pCApiFun, Api_Asy) != true)
	{
		return ;
	}
}

bool CT2Spi_Fast::OnLogin(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 登录找不到代表程序异常*/
		LOG(ERROR) << "OnLogin no json";
		ExceptionInterrupt;
		return false;
	}


	int count = root["json"].size();
	for (int i = 0; i< count; i++)
	{
		if (tool::VerifyElement(root, i,SPtr_CData))
		{
			SPtr_CData->branch_no = root["json"][i]["branch_no"].asString();

			string sysnode_id = root["json"][i]["sysnode_id"].asString();
			string uft_sysnode_id = root["json"][i]["uft_sysnode_id"].asString();
			if (uft_sysnode_id != "0" && uft_sysnode_id == "31") //测试和生产不一样，搞到配置文件里
			{
				SPtr_CData->sysnode_id = uft_sysnode_id;
			}
			else
			{
				SPtr_CData->sysnode_id = sysnode_id;
			}
			
			if (root["json"][i]["asset_prop"].asString()[0] != '0')
			{
				MessageBox(NULL, "请使用普通账户登录", "登录异常", MB_OK);
				return false;
			}
			string s_ClientRights = root["json"][i]["client_rights"].asString();
			if (s_ClientRights.find("E") == string::npos)
			{
				/*没找到E(订阅权限直接退出)*/
				MessageBox(NULL, "缺少权限E(委托、成交推送权限)，请重新登陆", "登录异常", MB_OK);
				return false;
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


bool CT2Spi_Fast::OnQueryStockAccount(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 资金账户下没有股东账户是可能的，因此是正确的、除此之外还有查持仓、成交、委托返回NULL也是正常的*/
		return true;
	}
	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CString market_code = "";
			CString main_flag = root["json"][i]["main_flag"].asString().c_str();
			if ('0' == main_flag[0])
			{
				ErrorLog("main_flag is 0");
				::MessageBox(SPtr_CData->hwnd_MainPageDlg,"此客户端不支持副股东账户，程序将退出","错误",NULL);
				if (PostMessage(SPtr_CData->hwnd_MainPageDlg, WM_AbnormalExit, 0, 0) == 0)
				{
					ErrorLog("PostMessage fail");
				}
				return false;
			}

			CString exchange_type = root["json"][i]["exchange_type"].asString().c_str();
			switch (exchange_type[0]) {
			case '1':
			{
				market_code = SSE;
				CAccount obj_CAccount;
				obj_CAccount.stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户
				obj_CAccount.market_code = market_code;
				if (SPtr_CData->SetAccount(SPtr_CData->s_user.c_str() , obj_CAccount) == false)
				{
					ErrorLog("SetAccount fail");
				}
				break;
			}
			case '2':
			{
				market_code = SZSE;
				CAccount obj_CAccount;
				obj_CAccount.stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户
				obj_CAccount.market_code = market_code;
				if (SPtr_CData->SetAccount(SPtr_CData->s_user.c_str(), obj_CAccount) == false)
				{
					ErrorLog("SetAccount fail");
				}
				break;
			}
			default:
				break;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			/*不用continue原因是只要有一个报错，就该认为查询股东账户成功，不发送对应信号，如果发送信号会隐藏问题，比如持仓，有一个错了就认定查询失败不刷新*/
			return false;
		}
	}
	
	return true;
}

bool CT2Spi_Fast::OnRevoke(const Value &root)
{
	if (root["json"].isNull())
	{
		ErrorLog("OnRevoke no json");
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData))
		{
			COrder obj_COrder;
			CString stock_account;
			stock_account = root["json"][i]["stock_account"].asString().c_str();
			/*委托推送、成交推送先到、应答先到*/
			if (GainObjOrder(root, i, obj_COrder) == false)
			{
				ErrorLog("GainObjOrder fail");
				return false;
			}
			//obj_COrder.ArrivalSequence_WithdrawOrder[index_Reply] = '1';
			/*如果原委托为未报，直接在撤单应答里更改为已撤*/
			string stock_code = root["json"][i]["stock_code"].asString();
			switch ((root["json"][i]["entrust_status_old"].asString())[0])
			{
			case '0'://未报(不会有推送)
			{
				obj_COrder.entrust_status = YiChe;
				break;
			}
			default:
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
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;//只有一个元素，而且要根据这个判断是否登录成功，因此一定要用return
		}
	}
	return true;
}

/*position*/
bool CT2Spi_Fast::QuerySinglePosition(const string &stock_code)
{
	string s_Fun333104;
	GainQuerySinglePosition(s_Fun333104, stock_code);
	if (tool::SendOrder(pCApiFun, Api_Asy,s_Fun333104, SPtr_CData)< 0)
	{
		return false;
	}
	return true;
}

bool CT2Spi_Fast::GainQuerySinglePosition(string &s_str, const string &stock_code)
{
	/*持仓查询*/
	Json::Value obj_Value;
	obj_Value["function_id"] = "333104";
	obj_Value["password"] = SPtr_CData->s_pass;
	obj_Value["fund_account"] = SPtr_CData->s_user;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["op_branch_no"] = SPtr_CData->branch_no;

	obj_Value["branch_no"] = SPtr_CData->branch_no;
	obj_Value["sysnode_id"] = SPtr_CData->sysnode_id;
	obj_Value["stock_code"] = stock_code;
	obj_Value["position_str"] = "";
	obj_Value["request_num"] = "1";

	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	
	s_str = obj_Value.toStyledString();
	return true;
}

bool CT2Spi_Fast::OnQuerySinglePosition(const Value &root)
{
	bool bBeginRedrawPosition = false;
	if (root["json"].isNull())
	{
		return true;
	}

	int count = root["json"].size();

	if (count >= SingleQuerySize)
	{
	}
	else
	{
		bBeginRedrawPosition = true;
	}
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CPosition obj_CPosition;

			CString fund_account = root["json"][i]["fund_account"].asString().c_str();//资金账户
			CString stock_account = root["json"][i]["stock_account"].asString().c_str();//股东账户

			obj_CPosition.stock_name = root["json"][i]["stock_name"].asString().c_str();//股票名称
			obj_CPosition.stock_code = root["json"][i]["stock_code"].asString().c_str();//股票代码
			obj_CPosition.current_amount = convert<UInt_64>(root["json"][i]["current_amount"].asString());//股票余额
			obj_CPosition.enable_amount = convert<UInt_64>(root["json"][i]["enable_amount"].asString());//可用数量
			obj_CPosition.market_value = convert<double>(root["json"][i]["market_value"].asString());//市值

			obj_CPosition.income_balance = convert<double>(root["json"][i]["income_balance"].asString());//盈亏
			obj_CPosition.cost_price = convert<double>(root["json"][i]["cost_price"].asString());//成本价
			obj_CPosition.last_price = convert<double>(root["json"][i]["last_price"].asString());//最新价
			obj_CPosition.profit_ratio = convert<double>(root["json"][i]["profit_ratio"].asString());

			if (SPtr_CData->SetPosition(fund_account, stock_account, obj_CPosition) == false)
			{
				LOG(ERROR) << "SetPosition fail";
				ExceptionInterrupt;
				return false;
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;
		}
	}
	if (true == bBeginRedrawPosition)
	{
		if (SPtr_CData->hwnd_TradeDlg == NULL)
		{
			SPtr_CData->Queue_WmMessage.push(WM_ReDrawPosition);
		}
		else if (PostMessage(SPtr_CData->hwnd_TradeDlg, WM_ReDrawPosition, 0, 0) == 0)
		{
			LOG(ERROR) << "PostMessage fail";
			ExceptionInterrupt;
			return false;
		}
	}
	return true;
}

/*fund*/
bool CT2Spi_Fast::OnQueryFund(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 查询资金为空程序异常*/
		ErrorLog("OnQueryFund no json");
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CFund obj_CFund;
			obj_CFund.fund_account = SPtr_CData->fund_account;
			//币种类型 资产值 证券市值 当前余额 可用资金 可取金额  回报买入金额 回报卖出金额
			CString money_type; 
			switch (root["json"][i]["money_type"].asString()[0])
			{
			case '0':
			{
				money_type = "人民币";
				break;
			}
			case '1':
			{
				money_type = "美元";
				break;
			}
			case '2':
			{
				money_type = "港币";
				break;
			}
			default:
				break;
			}
			obj_CFund.money_type = money_type;

			obj_CFund.asset_balance = convert<double>(root["json"][i]["asset_balance"].asString().c_str());//资产值
			obj_CFund.market_value = convert<double>(root["json"][i]["market_value"].asString().c_str());//证券市值
			obj_CFund.current_balance = convert<double>(root["json"][i]["current_balance"].asString().c_str());//当前余额
			obj_CFund.enable_balance = convert<double>(root["json"][i]["enable_balance"].asString().c_str());//可用资金
			
			obj_CFund.fetch_balance = convert<double>(root["json"][i]["fetch_balance"].asString().c_str());//可取金额
			obj_CFund.real_buy_balance = convert<double>(root["json"][i]["real_buy_balance"].asString().c_str());//回报买入金额
			obj_CFund.real_sell_balance = convert<double>(root["json"][i]["real_sell_balance"].asString().c_str());//回报卖出金额																		 
																										
			if (SPtr_CData->SetFund(obj_CFund) == false)
			{
				ErrorLog("SetFund fail");
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;
		}
	}
	return true;
}

bool CT2Spi_Fast::QueryFund()
{
	string s_Fun332255;
	tool::GainQueryFund332255(s_Fun332255,SPtr_CData);
	int RefId_HS = tool::SendOrder(pCApiFun,Api_Asy,s_Fun332255, SPtr_CData);
	if (RefId_HS < 0)
	{
		ErrorLog("abnormal RefId_HS");
		return false;
	}
	SPtr_CData->SetRefId_T2(RefId_HS, RefType_332255_NoFirstQueryFund);
	return true;
}

void CT2Spi_Fast::updateEntrust_status(COrder &obj_COrder,const string &entrust_status, const string &cancel_info)
{
	switch (entrust_status[0])
	{
	case '0'://未报
	{
		obj_COrder.entrust_status = WeiBao;
		break;
	}
	case '1'://待报
	{
		obj_COrder.entrust_status = DaiBao;
		break;
	}
	case '2'://已报
	{
		obj_COrder.entrust_status = YiBao;
		break;
	}
	case '3'://已报待撤
	{
		obj_COrder.entrust_status = YiBaoDaiChe;
		break;
	}
	case '4'://部成待撤
	{
		obj_COrder.entrust_status = BuChengDaiChe;
		break;
	}
	case '5'://部撤
	{
		obj_COrder.entrust_status = BuChe;
		break;
	}
	case '6'://已撤
	{
		obj_COrder.entrust_status = YiChe;
		break;
	}
	case '7'://部成
	{
		obj_COrder.entrust_status = BuCheng;
		break;
	}
	case '8'://已成
	{
		obj_COrder.entrust_status = YiCheng;
		break;
	}
	case '9'://废单
	{
		obj_COrder.entrust_status = FeiDan;
		LOG(ERROR) << "entrust_no:" << obj_COrder.entrust_no << " 废单原因:"<< cancel_info;
		break;
	}
	default:
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal entrust_status";
		break;
	}
}

void CT2Spi_Fast::updateEntrust_bs(COrder &obj_COrder, const string &entrust_bs)
{
	switch (entrust_bs[0])
	{
	case '1':
	{
		obj_COrder.entrust_bs = _Buy;
		break;
	}
	case '2':
	{
		obj_COrder.entrust_bs = _Sell;
		break;
	}
	default:
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal entrust_bs";
		break;
	}
}


void CT2Spi_Fast::updateEntrust_bs(CDeal &obj_CDeal, const string &entrust_bs)
{
	switch (entrust_bs[0])
	{
	case '1':
	{
		obj_CDeal.entrust_bs = _Buy;
		break;
	}
	case '2':
	{
		obj_CDeal.entrust_bs = _Sell;
		break;
	}
	default:
		break;
	}
}

bool CT2Spi_Fast::OnSendOrder(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 false退出即可*/
		ErrorLog("OnSendOrder no json");
		return false;
	}
	
	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			/*委托发送返还*/
			CString stock_account;
			COrder obj_COrder;

			Type_OrderList::iterator iter_RawOrder = SPtr_CData->M_OrderList.find(convert<int>(root["ref_id"].asString()) );
			if (SPtr_CData->M_OrderList.end() == iter_RawOrder)
			{
				ErrorLog("abanormal ref_id");
			}
			obj_COrder.stock_name = iter_RawOrder->second.stock_name;
			obj_COrder.stock_code = iter_RawOrder->second.stock_code;
			obj_COrder.entrust_bs = iter_RawOrder->second.entrust_bs;
			obj_COrder.entrust_price = iter_RawOrder->second.entrust_price;
			obj_COrder.entrust_amount = iter_RawOrder->second.entrust_amount;
			
			obj_COrder.entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());//委托编号
			if (VerifyEntrust_no(obj_COrder.entrust_no) == false)
			{
				return false;
			}
			if (SPtr_CData->SecCode2StockAccount(obj_COrder.stock_code, stock_account) == false)
			{
				ErrorLog("SecCode2StockAccount fail");
				return false;
			}

			tool::WriteCheckList(SPtr_CData,root, i, stock_account);

			/*先判断单子是否存在，存在就什么都不用做，在委托推送里已经用申报时间更新过了*/
			if (VerifybExist(stock_account, obj_COrder.entrust_no) == true)
			{
				return true;
			}

			if (SPtr_CData->SetOrder(SPtr_CData->fund_account, stock_account, obj_COrder) == false)
			{
				ErrorLog("SetOrder fail");
				return false;
			}
			UpdateFund_Position(string(obj_COrder.stock_code));
		}
		else
		{
			LOG(ERROR) << "function id:" << root["json"][i]["function_id"].asString() << " a element illegal";
			return false;
		}
	}
	bPostMessage = true;
	return true;
}

void CT2Spi_Fast::OnRecvJsonMsg(char* pRespdata)
{
	/*报文的合法性校验分为三步：首先判断是否成功转发(msg_id为0),其次判断是否有数据，没有数据没有json字段*/
	/*再者判断数据是否正确，通过error_no，先找 找不到合法，找得到为0合法* /
	/*首先判断T2是否成功转发了*/
	pRsp = pRespdata;
	DealQuit(SPtr_CData->bBeginRelease);
	Json::Reader reader;
	Json::Value root;

	/*SYSTEMTIME s_time;
	GetSystemTime(&s_time);*/
	//cerr << "begin parse:" << s_time.wMinute << ":" << s_time.wSecond << ":" << s_time.wMilliseconds << endl;
	if (!reader.parse(string(pRespdata), root, false))
	{
		LOG(ERROR) << "parse error";
		return;
	}
	
	//GetSystemTime(&s_time);
	//cerr << "end parse:" << s_time.wMinute << ":" << s_time.wSecond << ":" << s_time.wMilliseconds << endl;
	
	int function_id = convert<int>(root["function_id"].asString());
	switch (function_id)
	{
	case 331100://T2登陆
	{
		if (OnLogin(root) == true)
		{
			SetEvent(SPtr_CData->LoginT2_Event);
		}
		else
		{
			LOG(ERROR) << "OnLogin fail";
		}
		break;
	}
	case 331300://证券股东信息查询
	{
		if (OnQueryStockAccount(root) == true)
		{
			SetEvent(SPtr_CData->QueryStockAccount_Event);
		}
		else
		{
			ErrorLog("OnQueryStockAccount fail");
		}
		break;
	}
	case 332255://资金账户查询
	{
		/*考虑到如果返回就报错，没必要重新启动核心线程，刷新一遍结果*/
		if (OnQueryFund(root) == true)
		{
			int ref_id = convert<int>(root["ref_id"].asString());
			int MyRefId;
			if (SPtr_CData->GetEraseRefId_T2(ref_id, MyRefId) == false)
			{
				return;
			}
			switch (MyRefId)
			{
			case RefType_332255_InitFund:
			{
				SetEvent(SPtr_CData->InitFund_Event);
				break;
			}
			default:
				break;
			}

			if (SPtr_CData->hwnd_TradeDlg == NULL)
			{
				SPtr_CData->Queue_WmMessage.push(WM_RedrawFund);
			}
			else if (PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawFund, 0, 0) == 0)
			{
				LOG(ERROR) << "PostMessage fail";
				ExceptionInterrupt;
			}
		}
		else
		{
			LOG(ERROR) << "OnQueryFund fail";
			ExceptionInterrupt;
		}

		break;
	}
	case 333002://委托下单返还
	{
		if (SPtr_CData->TestSpeed == 1)
		{
			GetSystemTime(&(SPtr_CData->systime_OnSendOrder));
			LOG(ERROR) << "Recv Rsp of SendOrder consume time:" << SPtr_CData->systime_OnSendOrder.wSecond * 1000 + SPtr_CData->systime_OnSendOrder.wMilliseconds
				- (SPtr_CData->systime_SendOrder.wSecond * 1000 + SPtr_CData->systime_SendOrder.wMilliseconds) << " 毫秒";
		}
		OnSendOrder(root);
		break;
	}
	case 333017://撤单返还
	{
		OnRevoke(root);
		break;
	}
	case 333104://单个持仓查询
	{
		OnQuerySinglePosition(root);
		break;
	}
	case 620001://订阅应答
	{
		On620001(root);
		break;
	}
	case 620003://订阅(委托、成交)推送
	{
		OnSubscribe(root);
		break;
	}
	default:
		break;
	}
	return;
}

