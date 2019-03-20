#pragma once
#include "Data_Gobal.h"
#include "../share/define.h"

class CAertRecord;
class CEntrustEachConfig;
class CAutoTradeConfig;
class CAertRecord
{
public:
	CAertRecord()
	{

	}
	~CAertRecord()
	{
		int i = 0;
	}
	CString s_alertTime;
	CString stock_code;
	CString stock_name;
	CString cause;
	CString entrust_no;
	CString entrust_price;
	CString entrust_amount;
	CString note;
public:
	void changeIntoJson(Json::Value &val)
	{
		val["s_alertTime"] = string(s_alertTime);
		val["stock_code"] = string(stock_code);
		val["stock_name"] = string(stock_name);
		val["cause"] = string(cause);
		val["entrust_no"] = string(entrust_no);

		val["entrust_price"] = string(entrust_price);
		val["entrust_amount"] = string(entrust_amount);
		val["note"] = string(note);
	}
};
class CAlertConfig
{
public:
	size_t Template_Stock = AllMarket;
	size_t Template_Alert = enum_Increase;
	double IncreaseRatio = 7;
	bool bRemove_Increase = false;
	unsigned int Period_Increase_day = 30;
	size_t Period_Increase_second = 0;

	size_t Minute_N_Rise = 5;
	double IncreaseRatio_N_Rise = 3;

	size_t Minute_Low_Rise = 5;
	double BeginRatio_Low_Rise = 3;
	double EndRatio_Low_Rise = 3;
};


class CAlertNeed
{
public:
	CAlertNeed();
	CAlertNeed(CAlertNeed &obj_CAlertNeed);
	~CAlertNeed() {};
	CAlertNeed& operator = (CAlertNeed &obj_CAlertNeed)
	{
		SecName = obj_CAlertNeed.SecName;
		SecCode = obj_CAlertNeed.SecCode;
		MarketCode = obj_CAlertNeed.MarketCode;
		FluctuateRatio = obj_CAlertNeed.FluctuateRatio;
		PrevClosePx = obj_CAlertNeed.PrevClosePx;
		NewPrice = obj_CAlertNeed.NewPrice;
		Price_Jump = obj_CAlertNeed.Price_Jump;
		Time_Jump = obj_CAlertNeed.Time_Jump;
		s_bBiaoDiQuan = obj_CAlertNeed.s_bBiaoDiQuan;
		return *this;
	}
public:
	CString SecName;//证券名称
	CString SecCode;//证券代码
	CString MarketCode = "";
	double FluctuateRatio;//涨跌幅
	double PrevClosePx;//昨收盘
	double NewPrice;//最新价
	double Price_Jump;//跳出价格
	CString Time_Jump;//跳出时间
	CString s_bBiaoDiQuan = "";
};

class CEntrustEachConfig
{
public:
	size_t entrustAmount = 100;
	double entrustBalance = 0;//万元
	bool bBuy = true;
	bool bSell = false;
	bool bWithdrawBuy = false;

	bool bWithdrawSell = false;
	double DeviateRatio = 2;
	bool bFilterRatio = false;
	double increaseRatio = 0;
	size_t Template_stock = AllMarket;
	bool bSound = false;
};

class CAutoTradeConfig
{
public:
	CString s_bPercentageIncrease = "";
	double IncreaseValue = 0;
	CString priceType = "";
	bool bDefaultAmount = true;
	size_t DefaultAmount = 100;
	double DefaultBalance = 100;

	bool bManualConfirm = true;
	double limit_entrustBalance = 100;
	bool bTimingTrading = false;
	CString beginTime = "";
	CString endTime = "";
};
typedef std::tr1::shared_ptr<CAlertNeed> PtrAlert;
typedef map<CString, PtrAlert> _MAlert; // map<SecCode,PtrAlert>
typedef map<string, _MAlert> _MALLAlert;    // map<warningId_market,map<SecCode,PtrAlert>>

typedef multimap<double, PtrAlert> _MSpecialAlert;//map<FluctuateRatio,PtrAlert>;用于显示用
typedef map<string, UInt_64> Type_CheckList;
typedef map<CString, int> Type_M_blackList;

#include "Data_Order.h"
typedef map<int, COrder> Type_OrderList; //map<ref_id,COrder> 

typedef vector<CAertRecord> Type_VCAertRecord;
typedef map<string, UInt_64> Type_MAlertId;//map<warningId,version>version 为版本号，每次发起预警更改一次version

class CData
{
public:
	CData();
	~CData();

public:
	/*登陆时仅查询一次，保存所有股东账户下的所有证券代码，用于合法性校验，以及进行行情订阅的时候获取对应的MarketCode*/
	bool         	SetMarket(CMarket &Market);   // CMarket市场代码必填     
	bool 		    SetSecurity(CString strMarketCode, CSecurity &Security);  //CSecurity证券代码必填                                       
	bool  		    SetDyna(CString strMarketCode, CString strSecCode, CDynaQuote &Dyna);
	bool		    SetDepth(CString strMarketCode, CString strSecCode, CDepth &Depth);//CDepth挡位必填
	bool		    SetEntrustQueue(CString strMarketCode, CString strSecCode, CEntrustQueue &obj_CEntrustQueue);
	bool		    SetEntrustEach(CString strMarketCode, CString strSecCode, CEntrustEach &obj_CEntrustEach);
	bool            ClearEntrustEach(CString strMarketCode, CString strSecCode);
	bool            ClearMPosition(CString strFundID);
	bool            ClearMOrder(CString strFundID);


	bool            SetFund(CFund &Fund);//CFund资金账户必须填
	bool            SetAccount(CString strFundID, CAccount& Account);//CAccount股东账户必填
	bool            SetPosition(CString strFundID, CString strAccountID, CPosition& Position);//CPosition 证券代码必填
	bool            SetOrder(CString strFundID, CString strAccountID, COrder& Order);//COrder 委托id必填
	bool            SetDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, CDeal& Deal); //CDeal 成交编号必填
																										/*保留所有的订阅的证券代码*/
	bool 		    SetSecurity(CString strFundID, CString strAccountID, CSecurity &Security);//CSecurity证券代码必填 
																							
	bool 		    SetSecurity_BiaoDi(CString strFundID, CString strAccountID, CSecurity &Security);
	/*set担保证券*/
	bool 		    SetSecurity_DanBao(CString strFundID, CString strAccountID, CSecurity &Security);
	
	bool            SetVCreditAsset(CString strFundID, VecCreditAsset &VCreditAsset);

	/*从全部代码中获取指定信息（合法性检查用）*/
	bool         	GetMarket(CString strMarketCode, CMarket &Market);
	bool 		    GetSecurity(CString strMarketCode, CString strSecCode, CSecurity &Security);
	bool  		    GetDyna(CString strMarketCode, CString strSecCode, CDynaQuote &Dyna);
	bool		    GetDepth(CString strMarketCode, CString strSecCode, int iGrade, CDepth &Depth);
	bool		    GetMEntrustQueue(CString strMarketCode, CString strSecCode, int Grade, VecEntrustQueue &VEntrustQueue);
	bool		    GetVEntrustEach(CString strMarketCode, CString strSecCode, VecEntrustEach &VCEntrustEach,int RedrawSum);
	/*获取交易数据*/
	bool            GetFund(CString strFundID, CFund &Fund);
	/*所有的get Account类的操作是获取一个Account下的所有东西,除了其下挂的map之类的，但是get AccountMap类的统统的只获取Account下除了map之类的
	大量数据的结构体除外的成员变量，这样可以最大限度的减少内存拷贝，提高程序速度，就是使用起来麻烦很多，所以get account map只是保证map里的value
	有account的成员变量，其下级类的map没处理，因此要使用position map就只能使用get position map不能使用get account map、或者get acoount（=被重载了）*/
	bool            GetAccount(CString strFundID, CString strAccountID, CAccount& Account);
	bool            GetPosition(CString strFundID, CString strAccountID, CString strSecCode, CPosition& Position);
	bool            GetOrder(CString strFundID, CString strAccountID, UInt_64 entrust_no, COrder& Order);
	bool            GetDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, CString business_id, CDeal& Deal);

	bool 		    GetSecurity_BiaoDi(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security);
	bool 		    GetSecurity_DanBao(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security);
	bool            GetMSecurity_BiaoDi(CString strFundID, CString strAccountID, _Security& Security);
	bool            GetMSecurity_DanBao(CString strFundID, CString strAccountID, _Security& Security);
	
	bool            GetMAccount(CString strFundID, _Account& Account);
	bool            GetMPosition(CString strFundID, CString strAccountID, _Position& Position);
	bool            GetMOrder(CString strFundID, CString strAccountID, _Order& Order);
	bool            GetMDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, _Deal& Deal);
	bool            GetMSecurity(CString strFundID, CString strAccountID, _Security& Security);
	bool            GetMSecurity(CString strMarketCode, _Security& Security);

	/*获取订阅信息*/
	bool 		    GetSecurity(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security);
	bool            GetVCreditAsset(CString strFundID, VecCreditAsset &VCreditAsset);
	


	/*预警池*/
	string           GetKey(string warnningId, CString marketId);
	bool            GetAlert(string warnningId, CString marketId,const CString &SecCode,CAlertNeed &obj_Alert);
	bool            SetAlert(string warnningId, CString marketId,CAlertNeed &obj_Alert);
	bool            GetMAlert_All(_MALLAlert& Alert);
	bool            GetMAlert_Special(string warnningId,_MSpecialAlert &MSpecialAlert);
	bool            ClearMAlert_ALL();
	bool            deleteElement_MAlert(string warnningId, CString marketId, const CString &stock_code);
	bool            ClearPreviousRedrawList();
	bool            insertPreviousRedrawList(const CString &SecCode);
	bool            VerifybJump(const CString &SecCode);
	/*AlertRecord*/
	bool            GetVAlertRecord(Type_VCAertRecord &vec_CAertRecordTmp);
	bool            SetAlertRecord(CAertRecord &obj_CAertRecord);
	
	/*工具方法*/
	bool            Market2Account(const CString &strFundID, const CString &strMarketCode, CString& strAccountID);
	bool            Account2Market(CString strFundID, CString strAccountID, CString& strMarketCode);
	bool            SecCode2StockAccount(const CString &SecCode, CString &StockAccount);
	bool            SecCode2MarketCode(const CString &SecCode, CString &MarketCode);
	bool            SecCode2ExchangeType(const CString &SecCode, string &ExchangeType);
	bool            SecCode2SecName(const CString &SecCdoe, CString &SecName);

	bool            Account2ExchangeType(const CString &stock_account, string &exchange_type);
	bool            initM_AlertSecCode();
	bool            inspect_bAlert(CString stock_code);
	bool            inspectbStock(const CString strMarketCode, const CString &stock_code);
	/*ref_id*/
	bool SetRefId_T2(int RefID_HS, int MyRefId);
	bool GetEraseRefId_T2(int RefID_HS,int &MyRefId);
protected:
	bool            SetMarket(CString strMarketCode);
	bool            SetFund(CString strFundID);
public:
	_MALLAlert              MALLAlert;
	boost::shared_mutex       MALLAlert_Lock;
	map<CString, int> M_PreviousRedrawList;
	boost::shared_mutex M_PreviousRedrawList_Lock;

	Type_CheckList M_CheckList;//key report_no+ "_" +stock_account ,entrust_no  在委托回报的时候insert进map，在撤单的时候使用
	Type_OrderList M_OrderList;//key ref_id 重连会重新计数，所以重连一定要清空

	_Longlong                 ref_id;
	boost::shared_mutex       ref_Lock;
	_Fund   MFund;//资金账户数据
	boost::shared_mutex      MFund_Lock;
	/*第一次登陆时更新，保存所有证券列表用于合法性校验，之后用来保存用T2查询的行情和market server推送的行情*/
	_Market MMarket;
	boost::shared_mutex      MMarket_Lock;

	CAlertConfig obj_CAlertConfig;
	volatile bool bBeginRelease = false;//在release资源的时候不要响应onclose
	bool bRun_MainPageDlg;
	bool bEndQueryDanBaoQuan_SSE;
	bool bEndQueryDanBaoQuan_SZSE;
	bool bT2SupportMarket;//用来标记用T2查询的证券账号的行情是否支持 true为支持
	bool bActivity_TradeData;
	bool bQuit_ErrorInfo = false;
	bool bRuning_ErrorInfo = false;
	bool bLog_MarketAlert = true;

	int OrderInterval = 0;//毫秒
	int Frequency = 0;//每秒下单次数

	CString fund_account;//资金账户
	CString TradingSecCode;

	HANDLE  LoginT2_Event;//登陆T2的锁
	HANDLE  LoginSSEMarket_Event;//登陆Market的锁
	HANDLE  LoginSZSEMarket_Event;//登陆Market的锁

	HANDLE  QueryStockAccount_Event; //  登陆的时候用来查询一个资金账户下的所有股东账户  
	HANDLE  QuerySSEStockMap_Event;     //  登陆的时候用来查询现支持行情订阅的股票map
	HANDLE  QuerySZSEStockMap_Event;
	HANDLE  InitFund_Event;

	HANDLE  QueryDeal_Event;//登陆的时候用来初始化deal demo
	HANDLE  QueryDanBaoQuanMap_Event; //登录的时候用来查询一个资金账户下的担保证券

	HANDLE  UnSubscribe_Event;
	HANDLE  Subscribe_Event;

	HANDLE  Event_ConnectPartition_SSE = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE  Event_ConnectPartition_SZSE = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE  Event_loginSSEAlert = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE  Event_loginSZSEAlert = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE  Event_ResetAlert_SSE = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE  Event_ResetAlert_SZSE = CreateEvent(NULL, FALSE, FALSE, NULL);


	HANDLE Event_SendOrder = CreateEvent(NULL, FALSE, TRUE, NULL);//2 自动置为无信号,3初始有信号
	HANDLE Event_SendAllOrder = CreateEvent(NULL, FALSE, TRUE, NULL);
	/*对象指针，为了自定义消息*/
	intptr_t intptr_CFastTradeDlg{};

	string branch_no;
	string sysnode_id;
	string s_user;
	string s_pass;
	HWND hwnd_MainPageDlg;
	HWND hwnd_TradeDlg;

	SYSTEMTIME systime_SendOrder;
	SYSTEMTIME systime_OnSendOrder;
	SYSTEMTIME systime_BeginQueryOrder;
	SYSTEMTIME systime_RecvQueryOrder;
	SYSTEMTIME systime_FinishQueryOrder;
	int TestSpeed;
	bool bFullLog = false;
	queue<string> Queue_T2ErrorInfo;
	queue<string> Queue_MarketErrorInfo;
	queue<unsigned int> Queue_WmMessage;//在交易窗口没有初始化之前暂存重绘消息	
	map<CString, int> M_AlertSecCode;
	map<int, int> MRefId_T2;//key 恒生Refid, value 自定义Id
	boost::shared_mutex Lock_MRefId_T2;
	string S_StockPath_Fast = "../share/StockList_Alert_Fast.json";
	string S_StockPath_Credit = "../share/StockList_Alert_Credit.json";
	string S_StockPath_Auto = "../share/StockList_Alert_Auto.json";
	
	string partitionId = "";
	string user_alert = "";
	CAutoTradeConfig obj_CAutoTradeConfig;

	Type_VCAertRecord vec_CAertRecord;
	boost::shared_mutex lock_VAlertRecord;

	CEntrustEachConfig obj_CEntrustEachConfig;
	Type_M_blackList M_blackList;
	Type_MAlertId M_alertId;
};