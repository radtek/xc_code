#include "Data_Account.h"
#include "Data_Position.h"
#include "Data_Order.h"
#include "Data_Deal.h"
#include "Data_Security.h"
#include <iostream>

CAccount::CAccount()
{
	pFund = NULL;
	stock_account = "";               // 账户ID
	market_code = "";                 // 市场代码
	account_prop = "";                // 账户属性
}

CAccount::CAccount(CFund *lpFund, CString strAccountID)
{
	pFund = lpFund;
	stock_account = strAccountID;     // 账户ID
	market_code = "";                 // 市场代码
	account_prop = "";                // 账户属性
}

CAccount::CAccount(CFund *lpFund, CAccount &Account)
{
	pFund = lpFund;
	stock_account = Account.stock_account;   // 账户ID
	market_code = Account.market_code;       // 市场代码
	account_prop = Account.account_prop;     // 账户属性
}

CAccount::~CAccount()
{
	MPosition.clear();
	MSecurity.clear();
	MSecurity_BiaoDi.clear();
	MSecurity_DanBao.clear();
	MOrder.clear();
	pFund = NULL;
}

bool CAccount::ClearMPosition()
{
	DataMutex_MPosition.lock();
	MPosition.clear();
	DataMutex_MPosition.unlock();
	return true;
}

bool CAccount::ClearMOrder()
{
	DataMutex_MOrder.lock();
	MOrder.clear();
	DataMutex_MOrder.unlock();
	return true;
}

bool CAccount::SetSecurity(CSecurity &Security)
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
		MSecurity[NewSecurity->stock_code] = NewSecurity;    // 更新CSecurity的MAP容器
		DataMutex_MSecurity.unlock_upgrade();
	}
	DataMutex_MSecurity.unlock_shared();
	return true;
}

bool CAccount::SetSecurity(CString strSecCode)
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
	DataMutex_CAccount.unlock_upgrade();
	DataMutex_CAccount.unlock_shared();
	return true;
}

bool CAccount::SetSecurity_BiaoDi(CSecurity &Security)
{
	DataMutex_MSecurity_BiaoDi.lock_shared();

	_Security::iterator ite = MSecurity_BiaoDi.find(Security.stock_code);
	if (ite != MSecurity_BiaoDi.end())
	{
		ite->second->DataMutex_CSecurity.lock();
		*ite->second = Security;
		ite->second->DataMutex_CSecurity.unlock();
	}
	else
	{
		DataMutex_MSecurity_BiaoDi.lock_upgrade();
		PtrSecurity NewSecurity(new CSecurity(this, Security));
		MSecurity_BiaoDi[NewSecurity->stock_code] = NewSecurity;
		DataMutex_MSecurity_BiaoDi.unlock_upgrade();
	}

	DataMutex_MSecurity_BiaoDi.unlock_shared();
	return true;
}

bool CAccount::SetSecurity_DanBao(CSecurity &Security)
{
	DataMutex_MSecurity_DanBao.lock_shared();

	_Security::iterator ite = MSecurity_DanBao.find(Security.stock_code);
	if (ite != MSecurity_DanBao.end())
	{
		ite->second->DataMutex_CSecurity.lock();
		*ite->second = Security;
		ite->second->DataMutex_CSecurity.unlock();
	}
	else
	{
		DataMutex_MSecurity_DanBao.lock_upgrade();
		PtrSecurity NewSecurity(new CSecurity(this, Security));
		MSecurity_DanBao[NewSecurity->stock_code] = NewSecurity;
		DataMutex_MSecurity_DanBao.unlock_upgrade();
	}

	DataMutex_MSecurity_DanBao.unlock_shared();
	return true;
}

bool CAccount::SetPosition(CPosition& Position)
{
	DataMutex_MPosition.lock_shared();
	_Position::iterator ite = MPosition.find(Position.stock_code);
	if (ite != MPosition.end())
	{
		ite->second->DataMutex_CPosition.lock();
		*ite->second = Position;      // 更新CPosition类
		ite->second->DataMutex_CPosition.unlock();
	}
	else
	{	
		DataMutex_MPosition.lock_upgrade();
		PtrPosition NewPosition(new CPosition(this, Position)); // 新建CPosition类
		MPosition[NewPosition->stock_code] = NewPosition;       // 更新CPosition的MAP容器
		DataMutex_MPosition.unlock_upgrade();
	}
	DataMutex_MPosition.unlock_shared();
	return true;
}

bool CAccount::SetOrder(COrder& Order)
{
	DataMutex_MOrder.lock_shared();
	_Order::iterator ite = MOrder.find(Order.entrust_no);
	if (ite != MOrder.end())
	{
		
		ite->second->DataMutex_COrder.lock();
		*ite->second = Order;      // 更新COrder类
		ite->second->DataMutex_COrder.unlock();
	}
	else
	{
		DataMutex_MOrder.lock_upgrade();
		PtrOrder NewOrder(new COrder(this, Order)); // 新建COrder类
		MOrder[NewOrder->entrust_no] = NewOrder;    // 更新COrder的MAP容器
		DataMutex_MOrder.unlock_upgrade();
	}
	DataMutex_MOrder.unlock_shared();
	return true;
}

bool CAccount::SetOrder(UInt_64 entrust_no)
{
	DataMutex_MOrder.lock_shared();
	_Order::iterator ite = MOrder.find(entrust_no);
	if (ite != MOrder.end())
	{
		DataMutex_MOrder.unlock_shared();
		return true;
	}
	
	DataMutex_MOrder.lock_upgrade();
	PtrOrder NewOrder(new COrder(this, entrust_no));
	MOrder[NewOrder->entrust_no] = NewOrder;
	DataMutex_MOrder.unlock_upgrade();
	DataMutex_MOrder.unlock_shared();
	return true;
}

bool CAccount::SetDeal(UInt_64 entrust_no, CDeal& Deal)
{
	DataMutex_MOrder.lock_shared();
	bool ret;
	_Order::iterator ite = MOrder.find(entrust_no);
	if (ite != MOrder.end())
	{
		ret = ite->second->SetDeal(Deal);
		DataMutex_MOrder.unlock_shared();
		return ret;
	}
	if (!SetOrder(entrust_no))
	{
		DataMutex_MOrder.unlock_shared();
		return false;
	}
	
	ite = MOrder.find(entrust_no);
	if (ite == MOrder.end())
	{
		DataMutex_MOrder.unlock_shared();
		return false;
	}
	ret = ite->second->SetDeal(Deal);
	DataMutex_MOrder.unlock_shared();
	return ret;
}

bool CAccount::GetPosition(CString strSecCode, CPosition& Position)
{
	DataMutex_MPosition.lock_shared();
	_Position::iterator ite = MPosition.find(strSecCode);
	if (ite == MPosition.end())
	{
		DataMutex_MPosition.unlock_shared();
		return false;
	}
		
	ite->second->DataMutex_CPosition.lock_shared();
	Position = *ite->second;
	ite->second->DataMutex_CPosition.unlock_shared();
	DataMutex_MPosition.unlock_shared();
	return true;
}

bool CAccount::GetOrder(UInt_64 entrust_no, COrder& Order)
{
	DataMutex_MOrder.lock_shared();
	_Order::iterator ite = MOrder.find(entrust_no);
	if (ite == MOrder.end())
	{
		DataMutex_MOrder.unlock_shared();
		return false;
	}
	
	ite->second->DataMutex_COrder.lock_shared();
	Order = *ite->second;
	ite->second->DataMutex_COrder.unlock_shared();
	DataMutex_MOrder.unlock_shared();
	return true;
}

bool CAccount::GetDeal(UInt_64 entrust_no, CString business_id, CDeal& Deal)
{
	DataMutex_MOrder.lock_shared();
	bool ret;
	_Order::iterator ite = MOrder.find(entrust_no);
	if (ite == MOrder.end())
	{
		DataMutex_MOrder.unlock_shared();
		return false;
	}

	ret = ite->second->GetDeal(business_id, Deal);
	DataMutex_MOrder.unlock_shared();
	return ret;
}

bool CAccount::GetMPosition(_Position& Position)
{
	DataMutex_MPosition.lock_shared();
	if (&MPosition == NULL)
	{
		DataMutex_MPosition.unlock_shared();
		return false;
	}	

	Position.clear();
	_Position::iterator ite = MPosition.begin();
	while (ite != MPosition.end())
	{
		ite->second->DataMutex_CPosition.lock_shared();
		PtrPosition NewPosition(new CPosition(this, *ite->second)); // 新建CPosition类
		Position[NewPosition->stock_code] = NewPosition;       // 更新CPosition的MAP容器
		ite->second->DataMutex_CPosition.unlock_shared();
		ite++;
	}
	DataMutex_MPosition.unlock_shared();
	return true;
}

bool CAccount::GetMOrder(_Order& Order)
{
	if (&MOrder == NULL)
		return false;

	Order.clear();
	DataMutex_MOrder.lock_shared();
	_Order::iterator ite = MOrder.begin();
	while (ite != MOrder.end())
	{
		ite->second->DataMutex_COrder.lock_shared();
		PtrOrder NewOrder(new COrder(this, *ite->second)); // 新建COrder类
		Order[NewOrder->entrust_no] = NewOrder;       // 更新COrder的MAP容器
		ite->second->DataMutex_COrder.unlock_shared();
		ite++;
	}
	DataMutex_MOrder.unlock_shared();
	return true;
}

bool CAccount::GetMDeal(UInt_64 entrust_no, _Deal& Deal)
{
	DataMutex_MOrder.lock_shared();
	bool ret;
	_Order::iterator ite = MOrder.find(entrust_no);
	if (ite == MOrder.end())
	{
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal entrust_no" << entrust_no;;
		DataMutex_MOrder.unlock_shared();
		return false;
	}
	ret = ite->second->GetMDeal(Deal);
	DataMutex_MOrder.unlock_shared();
	return ret;
}

bool CAccount::GetMSecurity(_Security& Security)
{
	if (&MSecurity == NULL)
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

bool CAccount::GetMSecurity_BiaoDi(_Security& Security)
{
	if (&MSecurity_BiaoDi == NULL)
		return false;

	Security.clear();

	DataMutex_MSecurity_BiaoDi.lock_shared();

	_Security::iterator ite = MSecurity_BiaoDi.begin();
	while (ite != MSecurity_BiaoDi.end())
	{
		ite->second->DataMutex_CSecurity.lock_shared();
		PtrSecurity NewSecurity(new CSecurity(this, *ite->second)); // ???¨CSecurity?à
		Security[NewSecurity->stock_code] = NewSecurity;       // ?ü??CSecurity??MAP???÷
		ite->second->DataMutex_CSecurity.unlock_shared();
		ite++;
	}

	DataMutex_MSecurity_BiaoDi.unlock_shared();
	return true;
}

bool CAccount::GetMSecurity_DanBao(_Security& Security)
{
	if (&MSecurity_DanBao == NULL)
		return false;

	Security.clear();

	DataMutex_MSecurity_DanBao.lock_shared();

	_Security::iterator ite = MSecurity_DanBao.begin();
	while (ite != MSecurity_DanBao.end())
	{
		ite->second->DataMutex_CSecurity.lock_shared();
		PtrSecurity NewSecurity(new CSecurity(this, *ite->second)); // ???¨CSecurity?à
		Security[NewSecurity->stock_code] = NewSecurity;       // ?ü??CSecurity??MAP???÷
		ite->second->DataMutex_CSecurity.unlock_shared();
		ite++;
	}

	DataMutex_MSecurity_DanBao.unlock_shared();
	return true;
}

bool CAccount::GetSecurity(CString strSecCode, CSecurity &Security)
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

bool CAccount::GetSecurity_BiaoDi(CString strSecCode, CSecurity &Security)
{
	DataMutex_MSecurity_BiaoDi.lock_shared();

	_Security::iterator ite = MSecurity_BiaoDi.find(strSecCode);
	if (ite == MSecurity_BiaoDi.end())
	{
		DataMutex_MSecurity_BiaoDi.unlock_shared();
		return false;
	}

	ite->second->DataMutex_CSecurity.lock_shared();
	Security = *ite->second;
	ite->second->DataMutex_CSecurity.unlock_shared();

	DataMutex_MSecurity_BiaoDi.unlock_shared();
	return true;
}

bool CAccount::GetSecurity_DanBao(CString strSecCode, CSecurity &Security)
{
	DataMutex_MSecurity_DanBao.lock_shared();

	_Security::iterator ite = MSecurity_DanBao.find(strSecCode);
	if (ite == MSecurity_DanBao.end())
	{
		DataMutex_MSecurity_DanBao.unlock_shared();
		return false;
	}

	ite->second->DataMutex_CSecurity.lock_shared();
	Security = *ite->second;
	ite->second->DataMutex_CSecurity.unlock_shared();

	DataMutex_MSecurity_DanBao.unlock_shared();
	return true;
}