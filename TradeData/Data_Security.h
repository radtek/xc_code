#pragma once
#include "Data_Gobal.h"

class CSecurity
{
public:
	CSecurity();
	CSecurity(CAccount *lpAccount, CString strSecCode);
	CSecurity(CAccount *lpAccount, CSecurity &Security);
	CSecurity(CMarket *lpMarket, CString strSecCode);
	CSecurity(CMarket *lpMarket, CSecurity &Security);
	~CSecurity();
	CSecurity&	operator = (CSecurity &Security)
	{
		VEntrustQueue_B1.resize(EntrustQueueSize);
		VEntrustQueue_S1.resize(EntrustQueueSize);
		stock_code = Security.stock_code;
		stock_type = Security.stock_type;
		stock_name = Security.stock_name;
		PinYin = Security.PinYin;
		StartDate = Security.StartDate;
		UpLimitPrice = Security.UpLimitPrice;
		DownLimitPrice = Security.DownLimitPrice;
		PrevClosePx = Security.PrevClosePx;
		fin_ratio = Security.fin_ratio;
		assure_ratio = Security.assure_ratio;
		fin_status = Security.fin_status;
		assure_status = Security.assure_status;

		buy_unit = Security.buy_unit;
		sell_unit = Security.sell_unit;
		up_price = Security.up_price;
		down_price = Security.down_price;
		enable_amount = Security.enable_amount;
		s_bBiaoDiQuan = Security.s_bBiaoDiQuan;
		return *this;
	}

public:
	bool		SetDepth(CDepth& Depth);
	bool		SetDyna(CDynaQuote& Dyna);
	bool		SetEntrustQueue(CEntrustQueue& obj_CEntrustQueue);
	bool        SetEntrustQueue(CEntrustQueue& obj_CEntrustQueue, VecEntrustQueue &VEntrustQueue, boost::shared_mutex &DataMutex_MEntrustQueue);
	bool		SetEntrustEach(CEntrustEach& obj_CEntrustEach);
	bool        ClearEntrustEach();

	bool		GetDepth(int iGrade, CDepth& Depth);
	bool		GetMEntrustQueue(int Grade, VecEntrustQueue &VEntrustQueue);
	bool		GetVEntrustEach(VecEntrustEach &VCEntrustEach, int RedrawSum);

	bool		GetDyna(CDynaQuote& Dyna);
public:
	CString	    stock_code;			// 证券代码
	CString	    stock_type;			// 类型  0 代表所以支持的股票 上海的ES，深圳的1(主板A股) 2(中小板股票002开头并在恒生里的0股票里) 3(创业板股票)
	CString  	stock_name;		    // 股票名称
	CString	    PinYin;				// 拼音（备用）
	CString     StartDate;          // 上市日期
	CString     buy_unit;			//买入单位
	CString		sell_unit;			//卖出单位
	double	    UpLimitPrice;		// 当日涨停价
	double	    DownLimitPrice;		// 当日跌停价
	double      PrevClosePx;        // 昨收盘价 
	double      fin_ratio;          //融资保证金比例
	double      assure_ratio;       //担保折算率

	double		up_price;			//上限价
	double		down_price;			//下限价
	UInt_64		enable_amount;		//可用数量

	char        fin_status;         //融资状态
	char        assure_status;      //担保状态
	CString     s_bBiaoDiQuan;        //是否是标的券，是的话标记为标的券


protected:
	bool        SetDyna();
	PtrDynaQuote	pDyna;          // 快照行情
	VecEntrustQueue VEntrustQueue_B1;
	VecEntrustQueue VEntrustQueue_S1;
	_EntrustEach MEntrustEach;
public:
	boost::shared_mutex DataMutex_CDyna;
	boost::shared_mutex DataMutex_MEntrustQueue_B1;
	boost::shared_mutex DataMutex_MEntrustQueue_S1;
	boost::shared_mutex DataMutex_MEntrustEach;
	boost::shared_mutex DataMutex_CSecurity;
	CMarket*        pMarket;
	CAccount*       pAccount;
};

