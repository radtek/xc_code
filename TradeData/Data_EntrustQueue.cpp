#include "Data_EntrustQueue.h"

CEntrustQueue::CEntrustQueue()
{
	Quantity = 0;
	Grade = 0;
	Price = 0;
	OrderID = 0;
}

CEntrustQueue::CEntrustQueue(CSecurity *lpSec)
{
	pSec = lpSec;
	Quantity = 0;
	Grade = 0;
	Price = 0;
	OrderID = 0;

}

CEntrustQueue::CEntrustQueue(CSecurity *lpSec, CEntrustQueue &obj_CEntrustQueue)
{
	pSec = lpSec;
	Quantity = obj_CEntrustQueue.Quantity;
	Grade = obj_CEntrustQueue.Grade;
	Price = obj_CEntrustQueue.Price;
	OrderID = obj_CEntrustQueue.OrderID;
}

CEntrustQueue::~CEntrustQueue()
{
	pSec = NULL;
}

