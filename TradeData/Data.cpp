#include "Data.h"
#include "Data_Market.h"
#include "Data_Fund.h"
#include "Data_Account.h"

CData::CData()
{
	bRun_MainPageDlg = false;
	bEndQueryDanBaoQuan_SSE = false;
	bEndQueryDanBaoQuan_SZSE = false;
	TestSpeed = 0;
	/*后期有资金实时查询*/
	QueryStockAccount_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	QuerySSEStockMap_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	QuerySZSEStockMap_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	LoginT2_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	LoginSSEMarket_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	LoginSZSEMarket_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitFund_Event = CreateEvent(NULL, FALSE, FALSE, NULL);

	UnSubscribe_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Subscribe_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	hwnd_TradeDlg = NULL;
	hwnd_MainPageDlg = NULL;
	ref_id = 0;
	bActivity_TradeData = false;

	QueryDeal_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	QueryDanBaoQuanMap_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	TradingSecCode = "";
}

CData::~CData()
{
	if (QueryStockAccount_Event != NULL)
	{
		CloseHandle(QueryStockAccount_Event);
	}
	if (QuerySSEStockMap_Event != NULL)
	{
		CloseHandle(QuerySSEStockMap_Event);
	}
	if (QuerySZSEStockMap_Event != NULL)
	{
		CloseHandle(QuerySZSEStockMap_Event);
	}
	if (QueryDeal_Event != NULL)
	{
		CloseHandle(QueryDeal_Event);
	}
	if (QueryDanBaoQuanMap_Event != NULL)
	{
		CloseHandle(QueryDanBaoQuanMap_Event);
	}
	if (LoginT2_Event != NULL)
	{
		CloseHandle(LoginT2_Event);
	}
	if (LoginSSEMarket_Event != NULL)
	{
		CloseHandle(LoginSSEMarket_Event);
	}
	if (LoginSZSEMarket_Event != NULL)
	{
		CloseHandle(LoginSZSEMarket_Event);
	}
	if (Event_loginSSEAlert != NULL)
	{
		CloseHandle(Event_loginSSEAlert);
	}
	if (Event_loginSZSEAlert != NULL)
	{
		CloseHandle(Event_loginSZSEAlert);
	}
    if (Event_ConnectPartition_SSE != NULL)
	{
		CloseHandle(Event_ConnectPartition_SSE);
	}
	if (Event_ConnectPartition_SZSE != NULL)
	{
		CloseHandle(Event_ConnectPartition_SZSE);
	}
	
	
	if (InitFund_Event != NULL)
	{
		CloseHandle(InitFund_Event);
	}
	
	if (UnSubscribe_Event != NULL)
	{
		CloseHandle(UnSubscribe_Event);
	}
	if (Subscribe_Event != NULL)
	{
		CloseHandle(Subscribe_Event);
	}
	MMarket.clear();
	MALLAlert.clear();

}

bool CData::SetMarket(CMarket &Market)
{	
	MMarket_Lock.lock_shared();
	_Market::iterator ite = MMarket.find(Market.MarketCode);
	if (ite != MMarket.end())
	{
		ite->second->DataMutex_CMarket.lock();
		*ite->second = Market;      // 更新CMarket类
		ite->second->DataMutex_CMarket.unlock();
	}
	else
	{
		MMarket_Lock.lock_upgrade();
		PtrMarket NewMarket(new CMarket(this, Market)); // 新建CMarket类
		MMarket[NewMarket->MarketCode] = NewMarket;     // 更新CMarket的MAP容器
		MMarket_Lock.unlock_upgrade();
	}
	MMarket_Lock.unlock_shared();
	return true;
}

bool CData::SetMarket(CString strMarketCode)
{
	MMarket_Lock.lock_shared();
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return true;
	}
	MMarket_Lock.lock_upgrade();
	PtrMarket NewMarket(new CMarket(this, strMarketCode));  // 新建CMarket类
	MMarket[NewMarket->MarketCode] = NewMarket;             // 更新CMarket的MAP容器
	MMarket_Lock.unlock_upgrade();
	MMarket_Lock.unlock_shared();
	return true;
}

bool CData::SetSecurity(CString strMarketCode, CSecurity &Security)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->SetSecurity(Security);
		MMarket_Lock.unlock_shared();
		return ret;
	}
	
	if (!SetMarket(strMarketCode))
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	
	ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetSecurity(Security);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::SetDepth(CString strMarketCode, CString SecCode, CDepth &Depth)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->SetDepth(SecCode, Depth);
		MMarket_Lock.unlock_shared();
		return ret;
	}

	if (!SetMarket(strMarketCode))
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	
	ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetDepth(SecCode, Depth);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::SetEntrustQueue(CString strMarketCode, CString strSecCode, CEntrustQueue &obj_CEntrustQueue)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->SetEntrustQueue(strSecCode, obj_CEntrustQueue);
		MMarket_Lock.unlock_shared();
		return ret;
	}

	if (!SetMarket(strMarketCode))
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetEntrustQueue(strSecCode, obj_CEntrustQueue);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::SetEntrustEach(CString strMarketCode, CString strSecCode, CEntrustEach &obj_CEntrustEach)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->SetEntrustEach(strSecCode, obj_CEntrustEach);
		MMarket_Lock.unlock_shared();
		return ret;
	}

	if (!SetMarket(strMarketCode))
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetEntrustEach(strSecCode, obj_CEntrustEach);
	MMarket_Lock.unlock_shared();
	return ret;
}


bool CData::ClearEntrustEach(CString strMarketCode, CString strSecCode)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->ClearEntrustEach(strSecCode);
		MMarket_Lock.unlock_shared();
		return ret;
	}
	else
	{
		ErrorLog("abnormal strMarketCode");
		return false;
	}
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::ClearMPosition(CString strFundID)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->ClearMPosition();
	}
	else
	{
		ErrorLog("abnormal strFundID");
		MFund_Lock.unlock_shared();
		return false;
	}
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::ClearMOrder(CString strFundID)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->ClearMOrder();
	}
	else
	{
		ErrorLog("abnormal strFundID");
		MFund_Lock.unlock_shared();
		return false;
	}
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetDyna(CString strMarketCode, CString SecCode, CDynaQuote &Dyna)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite != MMarket.end())
	{
		ret = ite->second->SetDyna(SecCode, Dyna);
		MMarket_Lock.unlock_shared();
		return ret;
	}

	if (!SetMarket(strMarketCode))
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
		
	
	ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetDyna(SecCode, Dyna);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::SetFund(CFund& Fund)
{
	MFund_Lock.lock_shared();
	_Fund::iterator ite = MFund.find(Fund.fund_account);
	if (ite != MFund.end())
	{
		ite->second->DataMutex_CFund.lock();
		*ite->second = Fund;      // 更新CFund类
		ite->second->DataMutex_CFund.unlock();
	}
	else
	{
		MFund_Lock.lock_upgrade();
		PtrFund NewFund(new CFund(this, Fund)); // 新建CFund类
		MFund[NewFund->fund_account] = NewFund;       // 更新CFund的MAP容器 原有的
		MFund_Lock.unlock_upgrade();
	}
	MFund_Lock.unlock_shared();
	return true;
}

bool CData::SetFund(CString strFundID)
{	
	MFund_Lock.lock_shared();
	
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		MFund_Lock.unlock_shared();
		return true;
	}
	MFund_Lock.lock_upgrade();
	PtrFund NewFund(new CFund(this, strFundID));      // 新建CFund类
	MFund[NewFund->fund_account] = NewFund;             // 更新CFund的MAP容器
	MFund_Lock.unlock_upgrade();
	MFund_Lock.unlock_shared();
	return true;
}

bool CData::SetAccount(CString strFundID, CAccount& Account)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetAccount(Account);
		MFund_Lock.unlock_shared();
		return ret;
	}
	
	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	
	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	
	ret = ite->second->SetAccount(Account);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetPosition(CString strFundID, CString strAccountID, CPosition& Position)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetPosition(strAccountID, Position);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetPosition(strAccountID, Position);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetOrder(CString strFundID, CString strAccountID, COrder& Order)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetOrder(strAccountID, Order);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
    ret = ite->second->SetOrder(strAccountID, Order);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, CDeal& Deal)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetDeal(strAccountID, entrust_no, Deal);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetDeal(strAccountID, entrust_no, Deal);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetSecurity(CString strFundID, CString strAccountID, CSecurity &Security)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetSecurity(strAccountID, Security);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->SetSecurity(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetSecurity_BiaoDi(CString strFundID, CString strAccountID, CSecurity &Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		bool ret = ite->second->SetSecurity_BiaoDi(strAccountID, Security);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->SetSecurity_BiaoDi(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::SetSecurity_DanBao(CString strFundID, CString strAccountID, CSecurity &Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		bool ret = ite->second->SetSecurity_DanBao(strAccountID, Security);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->SetSecurity_DanBao(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}


bool CData::SetVCreditAsset(CString strFundID,VecCreditAsset &VCreditAsset)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite != MFund.end())
	{
		ret = ite->second->SetVCreditAsset(VCreditAsset);
		MFund_Lock.unlock_shared();
		return ret;
	}

	if (!SetFund(strFundID))
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->SetVCreditAsset(VCreditAsset);
	MFund_Lock.unlock_shared();
	return ret;

}

bool CData::GetMarket(CString strMarketCode, CMarket &Market)
{
	MMarket_Lock.lock_shared();
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	
	ite->second->DataMutex_CMarket.lock_shared();
	Market = *ite->second;
	ite->second->DataMutex_CMarket.unlock_shared();
	MMarket_Lock.unlock_shared();
	return true;
}

bool CData::GetSecurity(CString strMarketCode, CString strSecCode, CSecurity &Security)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetSecurity(strSecCode, Security);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetDyna(CString strMarketCode, CString strSecCode, CDynaQuote &Dyna)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetDyna(strSecCode, Dyna);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetDepth(CString strMarketCode, CString strSecCode, int iGrade, CDepth &Depth)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}
		
	ret = ite->second->GetDepth(strSecCode, iGrade, Depth);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetMEntrustQueue(CString strMarketCode, CString strSecCode, int Grade, VecEntrustQueue &VEntrustQueue)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->GetMEntrustQueue(strSecCode, Grade, VEntrustQueue);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetVEntrustEach(CString strMarketCode, CString strSecCode,VecEntrustEach &VCEntrustEach, int RedrawSu)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->GetVEntrustEach(strSecCode, VCEntrustEach, RedrawSu);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetFund(CString strFundID, CFund &Fund)
{
	MFund_Lock.lock_shared();
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
		
	ite->second->DataMutex_CFund.lock_shared();
	Fund = *ite->second;
	ite->second->DataMutex_CFund.unlock_shared();
	MFund_Lock.unlock_shared();
	return true;
}

bool CData::GetAccount(CString strFundID, CString strAccountID, CAccount& Account)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetAccount(strAccountID, Account);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetPosition(CString strFundID, CString strAccountID, CString strSecCode, CPosition& Position)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->GetPosition(strAccountID, strSecCode, Position);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetOrder(CString strFundID, CString strAccountID, UInt_64 entrust_no, COrder& Order)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->GetOrder(strAccountID, entrust_no, Order);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, CString business_id, CDeal& Deal)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
		
	ret = ite->second->GetDeal(strAccountID, entrust_no, business_id, Deal);;
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMAccount(CString strFundID, _Account& Account)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
		
	ret = ite->second->GetMAccount(Account);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMPosition(CString strFundID, CString strAccountID, _Position& Position)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetMPosition(strAccountID, Position);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMOrder(CString strFundID, CString strAccountID, _Order& Order)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}	
	ret = ite->second->GetMOrder(strAccountID, Order);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMDeal(CString strFundID, CString strAccountID, UInt_64 entrust_no, _Deal& Deal)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal strFundID"<< strFundID;
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetMDeal(strAccountID, entrust_no, Deal);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMSecurity(CString strMarketCode, _Security& Security)
{
	MMarket_Lock.lock_shared();
	bool ret;
	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	ret = ite->second->GetMSecurity(Security);
	MMarket_Lock.unlock_shared();
	return ret;
}

bool CData::GetMSecurity(CString strFundID, CString strAccountID, _Security& Security)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
		
	ret = ite->second->GetMSecurity(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMSecurity_BiaoDi(CString strFundID, CString strAccountID, _Security& Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetMSecurity_BiaoDi(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetMSecurity_DanBao(CString strFundID, CString strAccountID, _Security& Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetMSecurity_DanBao(strAccountID, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetSecurity(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetSecurity(strAccountID, strSecCode, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetVCreditAsset(CString strFundID, VecCreditAsset &VCreditAsset)
{
	MFund_Lock.lock_shared();
	bool ret;
	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}
	ret = ite->second->GetVCreditAsset(VCreditAsset);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetSecurity_BiaoDi(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetSecurity_BiaoDi(strAccountID, strSecCode, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

bool CData::GetSecurity_DanBao(CString strFundID, CString strAccountID, CString strSecCode, CSecurity &Security)
{
	MFund_Lock.lock_shared();

	_Fund::iterator ite = MFund.find(strFundID);
	if (ite == MFund.end())
	{
		MFund_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetSecurity_DanBao(strAccountID, strSecCode, Security);
	MFund_Lock.unlock_shared();
	return ret;
}

CAlertNeed::CAlertNeed()
{
	SecName = "";
	SecCode = "";
	FluctuateRatio = 0;
	PrevClosePx = 0;
	NewPrice = 0;
	Price_Jump = 0;
	Time_Jump = "";
}

CAlertNeed::CAlertNeed(CAlertNeed &obj_CAlertNeed)
{
	SecName = obj_CAlertNeed.SecName;
	SecCode = obj_CAlertNeed.SecCode;
	FluctuateRatio = obj_CAlertNeed.FluctuateRatio;
	PrevClosePx = obj_CAlertNeed.PrevClosePx;
	NewPrice = obj_CAlertNeed.NewPrice;
	MarketCode = obj_CAlertNeed.MarketCode;
	Price_Jump = obj_CAlertNeed.Price_Jump;
	Time_Jump = obj_CAlertNeed.Time_Jump;
	s_bBiaoDiQuan = obj_CAlertNeed.s_bBiaoDiQuan;
}

string CData::GetKey(string warnningId, CString marketId)
{
	string key = warnningId;
	key += marketId;
	return key;
}

bool CData::GetAlert(string warnningId, CString marketId, const CString &SecCode,CAlertNeed &obj_Alert)
{
	/*fixing 预警显示不全不是因为key的问题，而是因为定时刷新和先订阅再清除的问题，融资融券*/
	string key = GetKey(warnningId, marketId);
	MALLAlert_Lock.lock_shared();
	auto iter = MALLAlert.find(key);
	if (iter == MALLAlert.end())
	{
		MALLAlert_Lock.unlock_shared();
		return false;
	}
	auto iter2 = iter->second.find(SecCode);
	if (iter2 == iter->second.end())
	{
		MALLAlert_Lock.unlock_shared();
		return false;
	}
	obj_Alert = *iter2->second;
	MALLAlert_Lock.unlock_shared();
	return true;
}


bool CData::SetAlert(string warnningId, CString marketId, CAlertNeed &obj_Alert)
{
	string key = GetKey(warnningId, marketId);
	MALLAlert_Lock.lock();
	_MALLAlert::iterator ite1 = MALLAlert.find(key);
	if (ite1 == MALLAlert.end())
	{
		_MAlert MAlert;
		PtrAlert NewSec(new CAlertNeed(obj_Alert));
		MAlert[NewSec->SecCode] = NewSec;
		MALLAlert[key] = MAlert;
		ite1 = MALLAlert.find(key);
	}

	_MAlert::iterator ite2 = ite1->second.find(obj_Alert.SecCode);
	if (ite2 == ite1->second.end())
	{
		PtrAlert NewSec(new CAlertNeed(obj_Alert));
		ite1->second[NewSec->SecCode] = NewSec;
		MALLAlert_Lock.unlock();
		return true;
	}

	*ite2->second = obj_Alert;
	MALLAlert_Lock.unlock();
	return true;
}


bool CData::GetMAlert_All(_MALLAlert& Alert)
{
	MALLAlert_Lock.lock_shared();
	Alert.clear();
	_MALLAlert::iterator ite1 = MALLAlert.begin();
	while (ite1 != MALLAlert.end())
	{
		_MAlert MAlert;
		_MAlert::iterator ite2 = ite1->second.begin();
		while (ite2 != ite1->second.end())
		{
			PtrAlert NewSec(new CAlertNeed(*ite2->second));
			MAlert[ite2->first] = NewSec;
			ite2++;
		}
		Alert[ite1->first] = MAlert;
		ite1++;
	}

	MALLAlert_Lock.unlock_shared();
	return true;
}

bool CData::GetMAlert_Special(string warnningId,  _MSpecialAlert &MSpecialAlert)
{
	vector<string> vec_market = {"sh","sz"};/*fixing 加入手动刷新按钮*/
	MALLAlert_Lock.lock_shared();
	MSpecialAlert.clear();
	for (auto iter_market: vec_market)
	{
		string key = GetKey(warnningId, iter_market.c_str());
		auto iter_MALLAlert = MALLAlert.find(key);
		if (iter_MALLAlert == MALLAlert.end())
		{
			MALLAlert_Lock.unlock_shared();
			return false;
		}
		else
		{
			auto iter = iter_MALLAlert->second.begin();
			while (iter != iter_MALLAlert->second.end())
			{
				PtrAlert NewSec(new CAlertNeed(*iter->second));
				MSpecialAlert.insert(make_pair(iter->second->FluctuateRatio, NewSec));
				iter++;
			}
		}
	}
	MALLAlert_Lock.unlock_shared();
	return true;
}


bool CData::ClearMAlert_ALL()
{
	MALLAlert_Lock.lock();
	MALLAlert.clear();
	MALLAlert_Lock.unlock();
	return true;
}

bool CData::deleteElement_MAlert(string warnningId, CString marketId, const CString &stock_code)
{
	string key = GetKey(warnningId, marketId);
	MALLAlert_Lock.lock();
	auto iter_MALLAlert = MALLAlert.find(key);
	if (iter_MALLAlert != MALLAlert.end())
	{
		iter_MALLAlert->second.erase(stock_code);
	}
	else
	{
		LOG(ERROR) << "erase fail, UpOrLess:" << key << ",stock_code:" << stock_code;
		ExceptionInterrupt;
		MALLAlert_Lock.unlock();
		return false;
	}

	MALLAlert_Lock.unlock();
	return true;
}


bool CData::ClearPreviousRedrawList()
{
	M_PreviousRedrawList_Lock.lock();
	M_PreviousRedrawList.clear();
	M_PreviousRedrawList_Lock.unlock();
	return true;
}

bool CData::insertPreviousRedrawList(const CString &SecCode)
{
	M_PreviousRedrawList_Lock.lock();
	M_PreviousRedrawList.insert(make_pair(SecCode, 0));
	M_PreviousRedrawList_Lock.unlock();
	return true;
}

bool CData::VerifybJump(const CString &SecCode)
{
	bool bJump = false;
	M_PreviousRedrawList_Lock.lock_shared();
	if (M_PreviousRedrawList.find(SecCode) == M_PreviousRedrawList.end())
	{
		bJump = true;
	}
	else
	{
		bJump = false;
	}
	M_PreviousRedrawList_Lock.unlock_shared();
	return bJump;
}

bool CData::GetVAlertRecord(Type_VCAertRecord &vec_CAertRecordTmp)
{
	lock_VAlertRecord.lock_shared();
	vec_CAertRecordTmp = vec_CAertRecord;
	lock_VAlertRecord.unlock_shared();
	return true;
}

bool CData::SetAlertRecord(CAertRecord &obj_CAertRecord)
{
	lock_VAlertRecord.lock();
	vec_CAertRecord.push_back(obj_CAertRecord);
	lock_VAlertRecord.unlock();
	return true;
}
bool CData::inspectbStock(const CString strMarketCode, const CString &stock_code)
{
	MMarket_Lock.lock_shared();

	_Market::iterator ite = MMarket.find(strMarketCode);
	if (ite == MMarket.end())
	{
		MMarket_Lock.unlock_shared();
		return false;
	}

	bool ret = ite->second->inspectbStock(stock_code);
	MMarket_Lock.unlock_shared();
	return ret;
}
