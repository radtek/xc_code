#include "Data_Depth.h"

CDepth::CDepth()
{
	pDyna = NULL;
	Grade = 0;      // 档位
	Price = 0;		// 按品种小数位数处理
	Quantity = 0;	// 委托量，期货为手，股票为股
	NumOrders = 0;
}

CDepth::CDepth(CDynaQuote *lpDyna, CDepth &Depth)
{
	pDyna = lpDyna;
	Grade = Depth.Grade;
	Price = Depth.Price;
	Quantity = Depth.Quantity;
	NumOrders = Depth.NumOrders;
}

CDepth::~CDepth()
{
	pDyna = NULL;
}