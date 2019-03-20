#pragma once
#include "Data_Gobal.h"


class CMarket
{
public:
	CMarket();
	CMarket(CData *lpData, CString strMarketCode);
	CMarket(CData *lpData, CMarket &Market);
	~CMarket();
	CMarket&	operator = (CMarket &Market)
	{
		MarketCode = Market.MarketCode;
		Name = Market.Name;
		TimeZone = Market.TimeZone;
		OpenTime = Market.OpenTime;
		CloseTime = Market.CloseTime;
		BuyExchangeRate = Market.BuyExchangeRate;
		SellExchangeRate = Market.SellExchangeRate;
		return *this;
	}

public:
	CString     MarketCode;		// 市场代码
	CString	    Name;			// 市场全称
	UInt_32	    TimeZone;		// 所属时区
	UInt_32       OpenTime;       // 开市时间
	UInt_32       CloseTime;      // 闭市时间
	CString     BuyExchangeRate; // 买入汇率
	CString     SellExchangeRate;// 卖出汇率

public:
	bool        SetSecurity(CSecurity &Security);
	bool  		SetDyna(CString strSecCode, CDynaQuote &Dyna);
	bool		SetDepth(CString strSecCode, CDepth &Depth);
	bool		SetEntrustQueue(CString strSecCode, CEntrustQueue &obj_CEntrustQueue);
	bool		SetEntrustEach(CString strSecCode, CEntrustEach &obj_CEntrustEach);
	bool        ClearEntrustEach(CString strSecCode);

	bool        GetSecurity(CString strSecCode, CSecurity &Security);
	bool  		GetDyna(CString strSecCode, CDynaQuote &Dyna);
	bool		GetDepth(CString strSecCode, int iGrade, CDepth &Depth);
	bool		GetMEntrustQueue(CString strSecCode,int Grade, VecEntrustQueue &VEntrustQueuee);
	bool		GetVEntrustEach(CString strSecCode, VecEntrustEach &VCEntrustEach, int RedrawSum);

	bool        GetMSecurity(_Security& Security);
	bool        inspectbStock(const CString &stock_code);
protected:
	bool        SetSecurity(CString strSecCode);
public:
	boost::shared_mutex DataMutex_CMarket;
	boost::shared_mutex DataMutex_MSecurity;
	_Security   MSecurity;
	CData*      pData;
};

