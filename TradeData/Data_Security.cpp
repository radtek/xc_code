#include "Data_Security.h"
#include "Data_DynaQuote.h"
#include "Data_Position.h"
#include "Data_Order.h"
#include "Data_EntrustQueue.h"
#include "Data_EntrustEach.h"

CSecurity::CSecurity()
{
	VEntrustQueue_B1.resize(EntrustQueueSize);
	VEntrustQueue_S1.resize(EntrustQueueSize);
	pDyna = PtrDynaQuote(new CDynaQuote());
	pMarket = NULL;
	pAccount = NULL;
	stock_code = "";			// 证券代码
	stock_type = "";			// 类型
	stock_name = "";				// 简称
	PinYin = "";		    // 拼音（备用）
	StartDate = "";
	PrevClosePx = 0;
	UpLimitPrice = 0;		// 当日涨停价
	DownLimitPrice = 0;		// 当日跌停价
	fin_ratio = 0;
	assure_ratio = 0;
	fin_status = '0';
	assure_status = '0';
	s_bBiaoDiQuan = "";
}

CSecurity::CSecurity(CAccount *lpAccount, CSecurity &Security)
{
	VEntrustQueue_B1.resize(EntrustQueueSize);
	VEntrustQueue_S1.resize(EntrustQueueSize);
	pDyna = PtrDynaQuote(new CDynaQuote());
	pAccount = lpAccount;
	stock_code = Security.stock_code;   // 证券代码
	stock_type = Security.stock_type;			// 类型
	stock_name = Security.stock_name;				// 简称
	PinYin = Security.PinYin;		    // 拼音（备用）
	StartDate = Security.StartDate;
	PrevClosePx = Security.PrevClosePx;
	UpLimitPrice = Security.UpLimitPrice;		    // 当日涨停价
	DownLimitPrice = Security.DownLimitPrice;		// 当日跌停价
	fin_ratio = Security.fin_ratio;
	assure_ratio = Security.assure_ratio;
	fin_status = Security.fin_status;
	assure_status = Security.assure_status;
	s_bBiaoDiQuan = Security.s_bBiaoDiQuan;
}

CSecurity::CSecurity(CAccount *lpAccount, CString strSecCode)
{
	VEntrustQueue_B1.resize(EntrustQueueSize);
	VEntrustQueue_S1.resize(EntrustQueueSize);
	pDyna = PtrDynaQuote(new CDynaQuote());
	pAccount = lpAccount;
	stock_code = strSecCode;   // 证券代码
	stock_type = "";			// 类型
	stock_name = "";				// 简称
	PinYin = "";		    // 拼音（备用）
	StartDate = "";
	PrevClosePx = 0;
	UpLimitPrice = 0;		// 当日涨停价
	DownLimitPrice = 0;		// 当日跌停价
	fin_ratio = 0;
	assure_ratio = 0;
	fin_status = '0';
	assure_status = '0';
	s_bBiaoDiQuan = "";
}

CSecurity::CSecurity(CMarket *lpMarket, CString strSecCode)
{
	VEntrustQueue_B1.resize(EntrustQueueSize);
	VEntrustQueue_S1.resize(EntrustQueueSize);
	pDyna = PtrDynaQuote(new CDynaQuote());
	pMarket = lpMarket;
	stock_code = strSecCode;   // 证券代码
	stock_type = "";			// 类型
	stock_name = "";				// 简称
	PinYin = "";		    // 拼音（备用）
	StartDate = "";
	PrevClosePx = 0;
	UpLimitPrice = 0;		// 当日涨停价
	DownLimitPrice = 0;		// 当日跌停价
	fin_ratio = 0;
	assure_ratio = 0;
	fin_status = '0';
	assure_status = '0';
	s_bBiaoDiQuan = "";
}

CSecurity::CSecurity(CMarket *lpMarket, CSecurity &Security)
{
	VEntrustQueue_B1.resize(EntrustQueueSize);
	VEntrustQueue_S1.resize(EntrustQueueSize);
	pDyna = PtrDynaQuote(new CDynaQuote());
	pMarket = lpMarket;
	stock_code = Security.stock_code;   // 证券代码
	stock_type = Security.stock_type;	  // 类型
	stock_name = Security.stock_name;		  //股票名称
	PinYin = Security.PinYin;	  // 拼音（备用）
	StartDate = Security.StartDate;
	PrevClosePx = Security.PrevClosePx;
	UpLimitPrice = Security.UpLimitPrice;	    	// 当日涨停价
	DownLimitPrice = Security.DownLimitPrice;		// 当日跌停价
	fin_ratio = Security.fin_ratio;
	assure_ratio = Security.assure_ratio;
	fin_status = Security.fin_status;
	assure_status = Security.assure_status;
	s_bBiaoDiQuan = Security.s_bBiaoDiQuan;
}

CSecurity::~CSecurity()
{
	//CloseHandle(DataMutex);
	pDyna.reset();
	pDyna = NULL;
	pMarket = NULL;
	pAccount = NULL;
}

bool CSecurity::SetDepth(CDepth& Depth)
{
	if (pDyna == NULL)
	{
		if (!SetDyna())
			return false;
	}

	return pDyna->SetDepth(Depth);
}

bool CSecurity::SetDyna(CDynaQuote& Dyna)
{
	if (pDyna != NULL)
	{
		pDyna->DataMutex_CDynaQuote.lock();
		*pDyna = Dyna;
		pDyna->DataMutex_CDynaQuote.unlock();
	}
	else
	{
		DataMutex_CDyna.lock();
		pDyna = PtrDynaQuote(new CDynaQuote(this, Dyna));
		DataMutex_CDyna.unlock();
	}
	
	return true;
}

bool CSecurity::SetDyna()
{	
	if (pDyna != NULL)
		return true;
	DataMutex_CDyna.lock();
	pDyna = PtrDynaQuote(new CDynaQuote(this));
	DataMutex_CDyna.unlock();
	return true;
}

bool CSecurity::SetEntrustQueue(CEntrustQueue& obj_CEntrustQueue)
{
	switch (obj_CEntrustQueue.Grade)
	{
	case 1://sell1
		SetEntrustQueue(obj_CEntrustQueue,VEntrustQueue_S1,DataMutex_MEntrustQueue_S1);
		break;
	case -1://buy1
		SetEntrustQueue(obj_CEntrustQueue, VEntrustQueue_B1, DataMutex_MEntrustQueue_B1);
		break;
	default:
		break;
	}
	return true;
}

bool CSecurity::SetEntrustQueue(CEntrustQueue& obj_CEntrustQueue, VecEntrustQueue &VEntrustQueue,
	boost::shared_mutex &DataMutex_MEntrustQueue)
{
	DataMutex_MEntrustQueue.lock_shared();
	PtrEntrustQueue ptr = VEntrustQueue[obj_CEntrustQueue.OrderID];
	if (ptr)
	{
		ptr->DataMutex_CEntrustQueue.lock();
		*ptr = obj_CEntrustQueue;
		ptr->DataMutex_CEntrustQueue.unlock();
	}
	else
	{
		DataMutex_MEntrustQueue.lock_upgrade();
		PtrEntrustQueue NewEntrustQueue(new CEntrustQueue(this, obj_CEntrustQueue));
		VEntrustQueue[NewEntrustQueue->OrderID] = NewEntrustQueue;
		DataMutex_MEntrustQueue.unlock_upgrade();
	}
	DataMutex_MEntrustQueue.unlock_shared();
	return true;
}

bool CSecurity::SetEntrustEach(CEntrustEach& obj_CEntrustEach)
{
	DataMutex_MEntrustEach.lock();
	PtrEntrustEach NewEntrustEach(new CEntrustEach(this, obj_CEntrustEach));
	MEntrustEach[NewEntrustEach->OrderNo] = NewEntrustEach;
	DataMutex_MEntrustEach.unlock();

	/*DataMutex_MEntrustEach.lock_shared();
	_EntrustEach::iterator ite = MEntrustEach.find(obj_CEntrustEach.OrderNo);
	if (ite != MEntrustEach.end())
	{
		ite->second->DataMutex_CEntrustEach.lock();
		*ite->second = obj_CEntrustEach;
		ite->second->DataMutex_CEntrustEach.unlock();
	}
	else
	{
		DataMutex_MEntrustEach.lock_upgrade();
		PtrEntrustEach NewEntrustEach(new CEntrustEach(this, obj_CEntrustEach));
		MEntrustEach[NewEntrustEach->OrderNo] = NewEntrustEach;
		DataMutex_MEntrustEach.unlock_upgrade();
	}
	DataMutex_MEntrustEach.unlock_shared();*/
	return true;
}

bool CSecurity::ClearEntrustEach()
{
	DataMutex_MEntrustEach.lock();
	MEntrustEach.clear();
	DataMutex_MEntrustEach.unlock();
	return true;
}


bool CSecurity::GetDepth(int iGrade, CDepth& Depth)
{
	if (pDyna == NULL)
		return false;
	
	return pDyna->GetDepth(iGrade, Depth);
}

bool CSecurity::GetMEntrustQueue(int Grade, VecEntrustQueue &VEntrustQueue)
{
	switch (Grade)
	{
	case 1://sell 1
		DataMutex_MEntrustQueue_S1.lock();
		VEntrustQueue = VEntrustQueue_S1;
		DataMutex_MEntrustQueue_S1.unlock();
		break;
	case -1://buy 1
		DataMutex_MEntrustQueue_B1.lock();
		VEntrustQueue = VEntrustQueue_B1;
		DataMutex_MEntrustQueue_B1.unlock();
		break;
	default:
		break;
	}
	return true;
}

bool CSecurity::GetVEntrustEach(VecEntrustEach &VCEntrustEach, int RedrawSum)
{
	DataMutex_MEntrustEach.lock_shared();
	int index = 0;
	_EntrustEach::reverse_iterator ite; ;
	for (ite = MEntrustEach.rbegin(); ite != MEntrustEach.rend() && index < RedrawSum; ite++, index++)
	{
		ite->second->DataMutex_CEntrustEach.lock_shared();
		VCEntrustEach.push_back(ite->second);
		//VCEntrustEach[index] = ite->second;
		ite->second->DataMutex_CEntrustEach.unlock_shared();
	}
	DataMutex_MEntrustEach.unlock_shared();
	return true;
}

bool CSecurity::GetDyna(CDynaQuote& Dyna)
{
	if (pDyna == NULL)
		return false;
	pDyna->DataMutex_CDynaQuote.lock_shared();
	Dyna = *pDyna;
	pDyna->DataMutex_CDynaQuote.unlock_shared();
	return true;
}