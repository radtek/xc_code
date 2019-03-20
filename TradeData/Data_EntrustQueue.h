#pragma once
#include "Data_Gobal.h"

class CEntrustQueue
{
public:
	CEntrustQueue();
	CEntrustQueue(CSecurity *lpSec);
	CEntrustQueue(CSecurity *lpSec, CEntrustQueue &obj_CEntrustQueue);
	~CEntrustQueue();
	CEntrustQueue&	operator =(CEntrustQueue &obj_CEntrustQueue)
	{
		pSec = obj_CEntrustQueue.pSec;
		Quantity = obj_CEntrustQueue.Quantity;
		Grade = obj_CEntrustQueue.Grade;
		Price = obj_CEntrustQueue.Price;
		OrderID = obj_CEntrustQueue.OrderID;
		return *this;
	}
public:
	UInt_64 Quantity;
	int Grade = 0;
	double Price = 0;
	int OrderID = 0;
public:
	boost::shared_mutex DataMutex_CEntrustQueue;
	CSecurity*      pSec;
};

