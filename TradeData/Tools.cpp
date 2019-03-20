#include "Tools.h"

bool CData::Market2Account(const CString &strFundID, const CString &strMarketCode, CString& strAccountID)
{
	_Account Account;
	if (!GetMAccount(strFundID, Account))
		return false;

	_Account::iterator ite = Account.begin();
	while (ite != Account.end())
	{
		if (ite->second->market_code == strMarketCode)
		{
			strAccountID = ite->second->stock_account;
			return true;
		}
		ite++;
	}
	
	return false;
}

bool CData::Account2Market(CString strFundID, CString strAccountID, CString& strMarketCode)
{
	_Account Account;
	if (!GetMAccount(strFundID, Account))
		return false;

	_Account::iterator ite = Account.find(strAccountID);
	if (ite == Account.end())
		return false;

	strMarketCode = ite->second->market_code;
	return true;
}

bool CData::Account2ExchangeType(const CString &stock_account, string &exchange_type)
{
	CString MarketCode;
	if (Account2Market(fund_account, stock_account, MarketCode) == true)
	{
		if (MarketCode == "SSE")
		{
			exchange_type = "1";
		}
		else if (MarketCode == "SZSE")
		{
			exchange_type = "2";
		}
		else
		{
			ErrorLog("abanormal MarketCode");
		}
	}
	else
	{
		ErrorLog("Account2Market fail");
		return false;
	}
	return true;
}

bool CData::SecCode2StockAccount(const CString &SecCode, CString &StockAccount)
{
	bool bFind = false;
	CString MarketCode;
	_Market::iterator iter_MMarket;
	MMarket_Lock.lock_shared();
	/*股票全市场唯一*/
	for (iter_MMarket = MMarket.begin(); iter_MMarket != MMarket.end(); iter_MMarket++)
	{
		iter_MMarket->second->MSecurity;//保存所有全列表的证券代码map
		_Security::iterator iter_MSecurity;
		iter_MSecurity = iter_MMarket->second->MSecurity.find(SecCode);
		if (iter_MSecurity == iter_MMarket->second->MSecurity.end())
		{
			continue;
		}
		else
		{

			if (Market2Account(fund_account, iter_MMarket->first, StockAccount) == false)
			{
				bFind = false;
			}
			else
			{
				bFind = true;
			}
			break;
		}
	}
	MMarket_Lock.unlock_shared();
	return bFind;
}

bool CData::SecCode2MarketCode(const CString &SecCode, CString &MarketCode)
{
	bool bFind = false;
	MMarket_Lock.lock_shared();
	_Market::iterator iter_MMarket;
	for (iter_MMarket = MMarket.begin(); iter_MMarket != MMarket.end(); iter_MMarket++)
	{
		iter_MMarket->second->MSecurity;//全市场代码map
		if (iter_MMarket->second->MSecurity.find(SecCode) != iter_MMarket->second->MSecurity.end())
		{
			MarketCode = iter_MMarket->first;
			bFind = true;
		}
		else
		{
			continue;
		}
	}
	MMarket_Lock.unlock_shared();
	return bFind;
}

bool CData::SecCode2ExchangeType(const CString &SecCode, string &ExchangeType)
{
	bool bFind = false;
	CString MarketCode;
	_Market::iterator iter_MMarket;
	MMarket_Lock.lock_shared();
	for (iter_MMarket = MMarket.begin(); iter_MMarket != MMarket.end(); iter_MMarket++)
	{
		iter_MMarket->second->MSecurity;//全市场代码map
		if (iter_MMarket->second->MSecurity.find(SecCode) != iter_MMarket->second->MSecurity.end())
		{
			MarketCode = iter_MMarket->first;
			if (MarketCode == "SSE")
			{
				ExchangeType = "1";
				bFind = true;
				break;
			}
			else if (MarketCode == "SZSE")
			{
				ExchangeType = "2";
				bFind = true;
				break;
			}
			else
			{
				ErrorLog("abnormal MarketCode");
			}
			
		}
		else
		{
			continue;
		}
	}
	MMarket_Lock.unlock_shared();
	return bFind;
}


bool CData::SecCode2SecName(const CString &SecCdoe, CString &SecName)
{
	CString MarketCode;
	if (SecCode2MarketCode(SecCdoe, MarketCode) == false)
	{
		ErrorLog("SecCode2MarketCode fail");
		return false;
	}
	CSecurity obj_CSecurity;
	if (GetSecurity(MarketCode, SecCdoe, obj_CSecurity) == false)
	{
		ErrorLog("GetSecurity fail");
		return false;
	}
	SecName = obj_CSecurity.stock_name;
	return true;

}

/*融资融券初始化它，其他不初始化，map为空则直接判定为合法*/
bool CData::initM_AlertSecCode()
{
	M_AlertSecCode.clear();
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	if (true == GetMAccount(fund_account, obj_MAccount))
	{
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			iter_MAccount->first;//stock_account
			_Security MSecurity_BiaoDi;
			if (GetMSecurity_BiaoDi(fund_account, iter_MAccount->first, MSecurity_BiaoDi) == false)
			{
				ErrorLog("GetMSecurity_BiaoDi fail");
			}
			else
			{
				for (auto iter = MSecurity_BiaoDi.begin(); iter != MSecurity_BiaoDi.end(); iter++)
				{
					M_AlertSecCode.insert(make_pair(iter->first, 0));
				}
			}
			
			_Security MSecurity_DanBao;
			if (GetMSecurity_DanBao(fund_account, iter_MAccount->first, MSecurity_DanBao) == false)
			{
				ErrorLog("GetMSecurity_DanBao fail");
			}
			else
			{
				for (auto iter = MSecurity_DanBao.begin(); iter != MSecurity_DanBao.end(); iter++)
				{
					M_AlertSecCode.insert(make_pair(iter->first, 0));
				}
			}	
		}
	}

	return true;
}

bool CData::inspect_bAlert(CString stock_code)
{
	/*非融资融券不要初始化，为空直接不过滤*/
	if (M_AlertSecCode.size() == 0)
	{
		return true;
	}
	if (M_AlertSecCode.find(stock_code) == M_AlertSecCode.end())
	{
		return false;
	}
	return true;
}

bool CData::SetRefId_T2(int RefId_HS, int MyRefId)
{
	Lock_MRefId_T2.lock();
	MRefId_T2.insert(make_pair(RefId_HS, MyRefId));
	Lock_MRefId_T2.unlock();
	return true;
}

bool CData::GetEraseRefId_T2(int RefId_HS, int &MyRefId)
{
	Lock_MRefId_T2.lock();
	auto iter = MRefId_T2.find(RefId_HS);
	if (MRefId_T2.end() == iter)
	{
		Lock_MRefId_T2.unlock();
		ErrorLog("abnormal RefId_HS");
		return false;
	}
	MyRefId = iter->second;
	MRefId_T2.erase(iter);
	Lock_MRefId_T2.unlock();
	return true;
}