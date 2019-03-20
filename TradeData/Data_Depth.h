#pragma once
#include "Data_Gobal.h"


class CDepth
{
public:
	CDepth();
	CDepth(CDynaQuote *lpDyna, CDepth &Depth);
	~CDepth();
	CDepth&	operator = (CDepth &Depth)
	{
		Grade = Depth.Grade;
		Price = Depth.Price;
		Quantity = Depth.Quantity;
		NumOrders = Depth.NumOrders;
		return *this;
	}
public:
	int             Grade;      // 档位
	double			Price;		// 按品种小数位数处理
	UInt_64			Quantity;	// 委托量，期货为手，股票为股
	UInt_64         NumOrders = 0;//总委托必输
public:
	boost::shared_mutex DataMutex_CDepth;
	CDynaQuote*     pDyna;
};

