#include "Data_EntrustEach.h"

CEntrustEach::CEntrustEach()
{
	Price = 0;
	Volume = 0;
	Amount = 0;
	OrderNo = 0; 
	Side = ' '; 
	Time = 0;
}
CEntrustEach::CEntrustEach(CSecurity *lpSec)
{
	pSec = lpSec;
	Price = 0;
	Volume = 0;
	Amount = 0;
	OrderNo = 0;
	Side = ' ';
	Time = 0;
}
CEntrustEach::CEntrustEach(CSecurity *lpSec, CEntrustEach &obj_CEntrustEach)
{
	pSec = lpSec;
	Price = obj_CEntrustEach.Price;
	Volume = obj_CEntrustEach.Volume;
	Amount = obj_CEntrustEach.Amount;
	OrderNo = obj_CEntrustEach.OrderNo;
	Side = obj_CEntrustEach.Side;
	Time = obj_CEntrustEach.Time;
}
CEntrustEach::~CEntrustEach()
{
	pSec = NULL;
}