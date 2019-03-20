#include "Data_Market.h"
#include "Data_Security.h"

CMarket::CMarket()
{
	pData = NULL;
	MarketCode = "";		// 市场代码
	Name = "";	    		// 市场全称
	TimeZone = 0;	    	// 所属时区
	OpenTime = 0;           // 开市时间
	CloseTime = 0;          // 闭市时间
	BuyExchangeRate = "";
	SellExchangeRate = "";
}

CMarket::CMarket(CData *lpData, CString strMarketCode)
{
	pData = lpData;
	MarketCode = strMarketCode;		// 市场代码
	Name = "";	    	        	// 市场全称
	TimeZone = 0;	            	// 所属时区
	OpenTime = 0;                   // 开市时间
	CloseTime = 0;                  // 闭市时间
	BuyExchangeRate = "";
	SellExchangeRate = "";
}

CMarket::CMarket(CData *lpData, CMarket &Market)
{
	pData = lpData;
	MarketCode = Market.MarketCode;		// 市场代码
	Name = Market.Name;	    	    	// 市场全称
	TimeZone = Market.TimeZone;	    	// 所属时区
	OpenTime = Market.OpenTime;         // 开市时间
	CloseTime = Market.CloseTime;       // 闭市时间
	BuyExchangeRate = Market.BuyExchangeRate;
	SellExchangeRate = Market.SellExchangeRate;
}

CMarket::~CMarket()
{
	MSecurity.clear();
	pData = NULL;
}

bool CMarket::SetSecurity(CSecurity &Security)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(Security.stock_code);
	if (ite != MSecurity.end())
	{
		ite->second->DataMutex_CSecurity.lock();
		*ite->second = Security;      // 更新CSecurity类
		ite->second->DataMutex_CSecurity.unlock();
	}
	else
	{
		DataMutex_MSecurity.lock_upgrade();
		PtrSecurity NewSecurity(new CSecurity(this, Security)); // 新建CSecurity类
		MSecurity[NewSecurity->stock_code] = NewSecurity;          // 更新CSecurity的MAP容器
		DataMutex_MSecurity.unlock_upgrade();
	}
	DataMutex_MSecurity.unlock_shared();
	return true;
}

bool CMarket::SetSecurity(CString strSecCode)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return true;
	}
		
	
	DataMutex_MSecurity.lock_upgrade();
	PtrSecurity NewSecurity(new CSecurity(this, strSecCode));  // 新建CSecurity类
	MSecurity[NewSecurity->stock_code] = NewSecurity;             // 更新CSecurity的MAP容器
	DataMutex_MSecurity.unlock_upgrade();
	DataMutex_MSecurity.unlock_shared();
	return true;
}

bool CMarket::SetDyna(CString strSecCode, CDynaQuote &Dyna)
{
	DataMutex_MSecurity.lock_shared();
	bool ret;
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		ret = ite->second->SetDyna(Dyna);
		DataMutex_MSecurity.unlock_shared();
		return ret;
	}
	if (!SetSecurity(strSecCode))
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
		
	ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	ret = ite->second->SetDyna(Dyna);;
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::SetDepth(CString strSecCode, CDepth &Depth)
{
	DataMutex_MSecurity.lock_shared();
	bool ret;
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		ret = ite->second->SetDepth(Depth);
		DataMutex_MSecurity.unlock_shared();
		return  ret;
	}
	
	if (!SetSecurity(strSecCode))
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
		
	ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	ret = ite->second->SetDepth(Depth);
	DataMutex_MSecurity.unlock_shared();	
	return ret;
}

bool CMarket::SetEntrustQueue(CString strSecCode, CEntrustQueue &obj_CEntrustQueue)
{
	DataMutex_MSecurity.lock_shared();
	bool ret;
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		ret = ite->second->SetEntrustQueue(obj_CEntrustQueue);
		DataMutex_MSecurity.unlock_shared();
		return  ret;
	}

	if (!SetSecurity(strSecCode))
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}

	ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	ret = ite->second->SetEntrustQueue(obj_CEntrustQueue);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::SetEntrustEach(CString strSecCode, CEntrustEach &obj_CEntrustEach)
{
	DataMutex_MSecurity.lock_shared();
	bool ret;
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		ret = ite->second->SetEntrustEach(obj_CEntrustEach);
		DataMutex_MSecurity.unlock_shared();
		return  ret;
	}

	if (!SetSecurity(strSecCode))
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}

	ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	ret = ite->second->SetEntrustEach(obj_CEntrustEach);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::ClearEntrustEach(CString strSecCode)
{
	DataMutex_MSecurity.lock_shared();
	bool ret;
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite != MSecurity.end())
	{
		ret = ite->second->ClearEntrustEach();
		DataMutex_MSecurity.unlock_shared();
		return  ret;
	}
	else
	{
		ErrorLog("abnormal strSecCode");
		return false;
	}

	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::GetSecurity(CString strSecCode, CSecurity &Security)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	
	ite->second->DataMutex_CSecurity.lock_shared();
	Security = *ite->second;
	ite->second->DataMutex_CSecurity.unlock_shared();
	DataMutex_MSecurity.unlock_shared();
	return true;
}

bool CMarket::GetDyna(CString strSecCode, CDynaQuote &Dyna)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetDyna(Dyna);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::GetDepth(CString strSecCode, int iGrade, CDepth &Depth)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetDepth(iGrade, Depth);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::GetMEntrustQueue(CString strSecCode, int Grade, VecEntrustQueue &VEntrustQueue)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetMEntrustQueue(Grade, VEntrustQueue);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::GetVEntrustEach(CString strSecCode, VecEntrustEach &VCEntrustEach, int RedrawSum)
{
	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.find(strSecCode);
	if (ite == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetVEntrustEach(VCEntrustEach, RedrawSum);
	DataMutex_MSecurity.unlock_shared();
	return ret;
}

bool CMarket::GetMSecurity(_Security& Security)
{
	if (MSecurity.size() <= 0)
		return false;

	Security.clear();

	DataMutex_MSecurity.lock_shared();
	_Security::iterator ite = MSecurity.begin();
	while (ite != MSecurity.end())
	{
		ite->second->DataMutex_CSecurity.lock_shared();
		PtrSecurity NewSecurity(new CSecurity(this, *ite->second)); // 新建CSecurity类
		Security[NewSecurity->stock_code] = NewSecurity;       // 更新CSecurity的MAP容器
		ite->second->DataMutex_CSecurity.unlock_shared();
		ite++;
	}
	DataMutex_MSecurity.unlock_shared();
	return true;
}

bool CMarket::inspectbStock(const CString &stock_code)
{
	if (MSecurity.size() <= 0)
	{
		return false;
	}
	DataMutex_MSecurity.lock_shared();
	auto iter = MSecurity.find(stock_code);
	if (iter == MSecurity.end())
	{
		DataMutex_MSecurity.unlock_shared();
		return false;
	}
	DataMutex_MSecurity.unlock_shared();
	return true;
}

