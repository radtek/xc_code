#pragma once
#include "Data_Gobal.h"

class CEntrustEach
{
public:
	CEntrustEach();
	CEntrustEach(CSecurity *lpSec);
	CEntrustEach(CSecurity *lpSec, CEntrustEach &obj_CEntrustEach);
	~CEntrustEach();
	CEntrustEach& operator =(CEntrustEach &obj_CEntrustEach)
	{
		pSec = obj_CEntrustEach.pSec;
		Price = obj_CEntrustEach.Price;
		Volume = obj_CEntrustEach.Volume;
		Amount = obj_CEntrustEach.Amount;
		OrderNo = obj_CEntrustEach.OrderNo;
		Side = obj_CEntrustEach.Side;
		Time = obj_CEntrustEach.Time;
		return *this;
	}
public:
	double Price  = 0;          // 价格,3位小数
	UInt_64 Volume = 0;         // 数量
	double Amount = 0;         // 金额，3位小数
	UInt_64 OrderNo = 0;        // 委托号
	char Side = ' ';           // 买卖方向 1：买  2：卖
	UInt_32 Time = 0;			  // 时间
public:
	boost::shared_mutex DataMutex_CEntrustEach;
	CSecurity*      pSec;
};


