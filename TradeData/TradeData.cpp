#include "DataBase.h"

using namespace std;

CPositionDetail::CPositionDetail()
{
	PositionID = 0;
	PositionCost = 0;
	TdVolume = 0;
	YdVolume = 0;
	Margin = 0;
	Profit = 0;
	TradeID = 0;
    DataMutex = CreateMutex(NULL,FALSE,NULL);
	pPosition = NULL;
}

CPositionDetail::CPositionDetail( CPosition *lpPosition, CPositionDetail &PositionDetail )
{
	PositionID = PositionDetail.PositionID;
	PositionCost = PositionDetail.PositionCost;
	TdVolume = PositionDetail.TdVolume;
	YdVolume = PositionDetail.YdVolume;          
	Margin = PositionDetail.Margin;          
	Profit = PositionDetail.Profit;          
	TradeID = PositionDetail.TradeID;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pPosition = lpPosition;
}

CPositionDetail::~CPositionDetail()
{
	CloseHandle( DataMutex );
}

CPosition::CPosition()
{
	VarietyID = 0;
    InstrumentID = _T("");
	LongOrShort = _Long;     // 默认多头
	HedgeFlag = _TG;         // 默认投机
	TdVolume = 0;
	YdVolume = 0;
	SumVolume = 0;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pAccount = NULL;
}

CPosition::CPosition( CAccount *lpAccount, CPosition &Position )
{
	VarietyID = Position.VarietyID;
	InstrumentID = Position.InstrumentID;
	LongOrShort = Position.LongOrShort;       
	HedgeFlag = Position.HedgeFlag;         
	TdVolume = Position.TdVolume;
	YdVolume = Position.YdVolume;
	SumVolume = Position.SumVolume;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pAccount = lpAccount;
}

CPosition::~CPosition()
{
	MPositionDetail.clear();
	CloseHandle( DataMutex );
}

bool CPosition::SetPositionDetail( CPositionDetail& PositionDetail )
{
	_PositionDetail::iterator ite = MPositionDetail.find( PositionDetail.PositionID );
	if( ite != MPositionDetail.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = PositionDetail;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrPositionDetail NewPositionDetail( new CPositionDetail( this, PositionDetail ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MPositionDetail[ NewPositionDetail->PositionID ] = NewPositionDetail;
		::ReleaseMutex( this->DataMutex );
	}
	return ResetPosition();
}

bool CPosition::ResetPosition()
{
	::WaitForSingleObject( this->DataMutex, INFINITE );

	int TdVolume = 0;
	int YdVolume = 0;
	_PositionDetail::iterator ite = MPositionDetail.begin();
	while( ite != MPositionDetail.end() )
	{
		TdVolume = TdVolume + ite->second->TdVolume;
		YdVolume = YdVolume + ite->second->YdVolume;
		ite++;
	}
	this->TdVolume = TdVolume;
	this->YdVolume = YdVolume;

	::ReleaseMutex( this->DataMutex );
	return true;
}

CAccount::CAccount()
{
	AccountID = _T("");
    PreMargin = 0;
	PreFrzCash = 0;
	PreAviCash = 0;
	Margin = 0;
	FrzCash = 0;
	AviCash = 0;
	Deposit = 0;
	Withdraw = 0;
	OtherCash = 0;       
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pProduct = NULL;
}

CAccount::CAccount( CProduct *lpProduct, CAccount &Account )
{
	AccountID = Account.AccountID;
	PreMargin = Account.PreMargin;
	PreFrzCash = Account.PreFrzCash;
	PreAviCash = Account.PreAviCash;
	Margin = Account.Margin;
	FrzCash = Account.FrzCash;
	AviCash = Account.AviCash;
	Deposit = Account.Deposit;
	Withdraw = Account.Withdraw;
	OtherCash = Account.OtherCash;       
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pProduct = lpProduct;
}

CAccount::~CAccount()
{
	MPosition.clear();
	MOriginTradePlan.clear();
	MNewTradePlan.clear();
	CloseHandle( DataMutex );
}

bool CAccount::ConvertOriginToNewPlan()
{
	CTime time = CTime::GetCurrentTime();
	int NowMinute = time.GetHour()*60 + time.GetMinute();
	_OriginTradePlan::iterator ite = MOriginTradePlan.begin();
	while( ite != MOriginTradePlan.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		if( ite->second->Status == _Checked && ite->second->Volume > 0 )
		{
			int StartMinute = (int)(ite->second->BeginTime/10000)*60 + (ite->second->BeginTime - (int)(ite->second->BeginTime/10000)*10000)/100;
			if( StartMinute < NowMinute )
				StartMinute = NowMinute;
			int EndMinute = (int)(ite->second->EndTime/10000)*60 + (ite->second->EndTime - (int)(ite->second->EndTime/10000)*10000)/100;
			int DifferMinute = EndMinute - StartMinute;
			int SpaceMinue = ceil((double)( DifferMinute / ite->second->Volume ));
			int ResultVolume = ite->second->Volume;
			while( ResultVolume > 0 )
			{
				CNewTradePlan NewTradePlan;
				NewTradePlan.PlanID = this->pProduct->pData->GetNewPlanID();
				NewTradePlan.OriginID = ite->second->PlanID;
				NewTradePlan.Type = ite->second->Type;
				NewTradePlan.Direction = ite->second->Direction;
				NewTradePlan.HedgeFlag = ite->second->HedgeFlag;
				NewTradePlan.InstrumentID = ite->second->InstrumentID;
				NewTradePlan.Volume1 = 1;
				NewTradePlan.StartTime = StartMinute;
				NewTradePlan.EndTime = EndMinute;
				if( NewTradePlan.StartTime > NewTradePlan.EndTime )
					NewTradePlan.Status = _Error;
				else
					NewTradePlan.Status = _Hold;
				PtrNewTradePlan PNewTradePlan( new CNewTradePlan( this, NewTradePlan ) );

				::WaitForSingleObject( this->DataMutex, INFINITE );
				MNewTradePlan[ PNewTradePlan->PlanID ] = PNewTradePlan;
				::ReleaseMutex( this->DataMutex );

				StartMinute = StartMinute + SpaceMinue;
				ResultVolume = ResultVolume - NewTradePlan.Volume1;
			}
			ite->second->Status = _Converted;
		}
		::ReleaseMutex( ite->second->DataMutex );
		ite++;
	}
	return true;
}

bool CAccount::CheckOriginTradePlan()
{
	_OriginTradePlan::iterator ite1 = this->MOriginTradePlan.begin();
	while( ite1 != this->MOriginTradePlan.end() )
	{
		if( ite1->second->Status == _Init )
		{
			if( ite1->second->Type == _Open )  // 开仓时有反向持仓，优先平反向持仓
			{
				struct_VarietyInfo VarietyInfo;
				if( ite1->second->Direction == _Buy )
					VarietyInfo.LongOrShort = _Short;  // 买入开仓：多头——反向持仓：空头,买入平仓
				if( ite1->second->Direction == _Sell )
					VarietyInfo.LongOrShort = _Long;   // 卖出开仓：空头——反向持仓：多头，卖出平仓
				VarietyInfo.HedgeFlag = ite1->second->HedgeFlag;
				VarietyInfo.InstrumentID = ite1->second->InstrumentID;
				DWORD VarietyID = this->pProduct->pData->GetVariety( VarietyInfo );
				_Position::iterator ite2 = this->MPosition.find( VarietyID );
				if( ite2 != this->MPosition.end() )  // 具有反向持仓
				{
					COriginTradePlan OriginTradePlan;
					OriginTradePlan.PlanID = this->pProduct->pData->GetOriginPlanID();
					OriginTradePlan.Type = _Close;
					OriginTradePlan.Direction = ite1->second->Direction;
					OriginTradePlan.HedgeFlag = ite1->second->HedgeFlag;
					OriginTradePlan.InstrumentID = ite1->second->InstrumentID;
					OriginTradePlan.BeginTime = ite1->second->BeginTime;
					OriginTradePlan.EndTime = ite1->second->EndTime;
					OriginTradePlan.Status = _Checked;
					if( ite1->second->Volume >= ite2->second->SumVolume )
					{
						OriginTradePlan.Volume = ite2->second->SumVolume;
						ite1->second->Volume = ite1->second->Volume - ite2->second->SumVolume;
					}
					else
					{
						OriginTradePlan.Volume = ite1->second->Volume;
						ite1->second->Volume = 0;
					}
					SetOriginTradePlan( OriginTradePlan );  // 反向对冲平仓计划
					CString Msg = _T("");
					Msg.Format( _T("开仓反向对冲:%s"), ite1->second->InstrumentID );
					this->pProduct->pData->SetErrorMsg( Msg );
				}
			}
			if( ite1->second->Type == _Close ) // 无对应持仓可平，反向开仓
			{
				struct_VarietyInfo VarietyInfo;
				if( ite1->second->Direction == _Buy )
					VarietyInfo.LongOrShort = _Short;  
				if( ite1->second->Direction == _Sell )
					VarietyInfo.LongOrShort = _Long;   
				VarietyInfo.HedgeFlag = ite1->second->HedgeFlag;
				VarietyInfo.InstrumentID = ite1->second->InstrumentID;
				DWORD VarietyID = this->pProduct->pData->GetVariety( VarietyInfo );
				_Position::iterator ite2 = this->MPosition.find( VarietyID );
				if( ite2 != this->MPosition.end() )  // 具有对应持仓
				{
					if( ite2->second->SumVolume < ite1->second->Volume )  // 对应持仓数量不足
					{
						COriginTradePlan OriginTradePlan;
						OriginTradePlan.PlanID = this->pProduct->pData->GetOriginPlanID();
						OriginTradePlan.Type = _Open;
						OriginTradePlan.Direction = ite1->second->Direction;
						OriginTradePlan.HedgeFlag = ite1->second->HedgeFlag;
						OriginTradePlan.InstrumentID = ite1->second->InstrumentID;
						OriginTradePlan.BeginTime = ite1->second->BeginTime;
						OriginTradePlan.EndTime = ite1->second->EndTime;
						OriginTradePlan.Volume = ite1->second->Volume - ite2->second->SumVolume;
						OriginTradePlan.Status = _Checked;
						SetOriginTradePlan( OriginTradePlan );  // 反向对冲开仓计划
						CString Msg = _T("");
						Msg.Format( _T("平仓反向对冲:%s"), ite1->second->InstrumentID );
						this->pProduct->pData->SetErrorMsg( Msg );
						ite1->second->Volume = ite2->second->SumVolume;
					}
				}
				else   // 对应无持仓
				{
					COriginTradePlan OriginTradePlan;
					OriginTradePlan.PlanID = this->pProduct->pData->GetOriginPlanID();
					OriginTradePlan.Type = _Open;
					OriginTradePlan.Direction = ite1->second->Direction;
					OriginTradePlan.HedgeFlag = ite1->second->HedgeFlag;
					OriginTradePlan.InstrumentID = ite1->second->InstrumentID;
					OriginTradePlan.BeginTime = ite1->second->BeginTime;
					OriginTradePlan.EndTime = ite1->second->EndTime;
					OriginTradePlan.Volume = ite1->second->Volume;
					OriginTradePlan.Status = _Checked;
					this->SetOriginTradePlan( OriginTradePlan );  // 反向对冲开仓计划
					CString Msg = _T("");
					Msg.Format( _T("平仓反向对冲:%s"), ite1->second->InstrumentID );
					this->pProduct->pData->SetErrorMsg( Msg );
					ite1->second->Volume = 0;
				}
			}
			// 有对冲情况，报异常
			ite1->second->Status = _Checked;
		}
		ite1++;
	}
	return true;
}

bool CAccount::StartHoldToNewPlan()
{
	CTime time = CTime::GetCurrentTime();
	int NowMinute = time.GetHour()*60 + time.GetMinute();
	_NewTradePlan::iterator ite = MNewTradePlan.begin();
	while( ite != MNewTradePlan.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		if( ite->second->EndTime < NowMinute && ( ite->second->Status != _CancelOver && ite->second->Status != _CompleteOver ) )
			ite->second->Status = _Error;  // 超时指令错误

		if( ite->second->Status == _Hold && ite->second->StartTime <= NowMinute )
			ite->second->Status = _WaitForPrice;  // 适时挂起指令查询价格
		
		::ReleaseMutex( ite->second->DataMutex );
		ite++;
	}
	return true;
}

bool CAccount::CanceltheUnTradedOrderToNewPlan()
{
	CTime time = CTime::GetCurrentTime();
	int NowMinute = time.GetHour()*60 + time.GetMinute();
	_NewTradePlan::iterator ite = MNewTradePlan.begin();
	while( ite != MNewTradePlan.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		if( ite->second->Status == _Run && ite->second->Type != _Cancel && NowMinute >= ite->second->StartTime + 5 )
		{
			CNewTradePlan NewTradePlan;
			NewTradePlan.PlanID = this->pProduct->pData->GetNewPlanID();
			NewTradePlan.LinkPlanID = ite->second->PlanID;
			NewTradePlan.OriginID = ite->second->OriginID;
			NewTradePlan.Type = _Cancel;
			NewTradePlan.Direction = ite->second->Direction;
			NewTradePlan.HedgeFlag = ite->second->HedgeFlag;
			NewTradePlan.InstrumentID = ite->second->InstrumentID;
			NewTradePlan.Volume1 = ite->second->Volume1 - ite->second->Volume2;
			NewTradePlan.StartTime = NowMinute;
			NewTradePlan.EndTime = ite->second->EndTime;
			NewTradePlan.Status = _Hold;
			PtrNewTradePlan PNewTradePlan( new CNewTradePlan( this, NewTradePlan ) );
			
			PNewTradePlan->pOrder->Type = _Cancel;
			
			::WaitForSingleObject( this->DataMutex, INFINITE );
			MNewTradePlan[ PNewTradePlan->PlanID ] = PNewTradePlan;
			::ReleaseMutex( this->DataMutex );

			ite->second->Status = _CancelForRetry;  // 撤单待重试
			ite->second->LinkPlanID = NewTradePlan.PlanID;
		}
		::ReleaseMutex( ite->second->DataMutex );
		ite++;
	}
	return true;
}

bool CAccount::RetrytheCanceledOrderToNewPlan()
{
	CTime time = CTime::GetCurrentTime();
	int NowMinute = time.GetHour()*60 + time.GetMinute();
	_NewTradePlan::iterator ite = MNewTradePlan.begin();
	while( ite != MNewTradePlan.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		if( ite->second->Status == _CancelForRetry )
		{
			_NewTradePlan::iterator findite = MNewTradePlan.find( ite->second->LinkPlanID );
			if( findite != MNewTradePlan.end() )
			{
				if( findite->second->Status == _CompleteOver )
				{
					CNewTradePlan NewTradePlan;
					NewTradePlan.PlanID = this->pProduct->pData->GetNewPlanID();
					NewTradePlan.OriginID = ite->second->OriginID;
					NewTradePlan.Type = ite->second->Type;
					NewTradePlan.Direction = ite->second->Direction;
					NewTradePlan.HedgeFlag = ite->second->HedgeFlag;
					NewTradePlan.InstrumentID = ite->second->InstrumentID;
					NewTradePlan.Volume1 = findite->second->Volume1;
					NewTradePlan.StartTime = NowMinute;
					NewTradePlan.EndTime = ite->second->EndTime;
					NewTradePlan.Status = _Hold;
					PtrNewTradePlan PNewTradePlan( new CNewTradePlan( this, NewTradePlan ) );

					::WaitForSingleObject( this->DataMutex, INFINITE );
					MNewTradePlan[ PNewTradePlan->PlanID ] = PNewTradePlan;
					::ReleaseMutex( this->DataMutex );

					ite->second->Status = _CancelOver;  // 撤单结束
				}
			}
		}
		::ReleaseMutex( ite->second->DataMutex );
		ite++;
	}
	return true;
}

bool CAccount::ReSetOriginTradePlan()
{
	_OriginTradePlan::iterator ite1 = this->MOriginTradePlan.begin();
	while( ite1 != this->MOriginTradePlan.end() )
	{
		int Volume0 = 0; // 待委托数量
		int Volume1 = 0; // 委托数量
		int Volume2 = 0; // 成交数量
		_NewTradePlan::iterator ite2 = this->MNewTradePlan.begin();
		while( ite2 != this->MNewTradePlan.end() )
		{
			if( ite2->second->OriginID == ite1->second->PlanID )
			{
				if( ite2->second->Type != _Cancel )
				{
					if( ite2->second->Status == _Hold || ite2->second->Status == _WaitForPrice || ite2->second->Status == _Ready || ite2->second->Status == _CancelForRetry )
						Volume0 += ite2->second->Volume1;
					if( ite2->second->Status == _Run )
						Volume1 += ite2->second->Volume1;
					Volume2 += ite2->second->Volume2;
				}
			}
			ite2++;
		}
		ite1->second->Volume0 = Volume0;
		ite1->second->Volume1 = Volume1;
		ite1->second->Volume2 = Volume2;
		ite1++;
	}
	return true;
}

bool CAccount::SetPosition( CPosition& Position )
{
	_Position::iterator ite = MPosition.find( Position.VarietyID );
	if( ite != MPosition.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = Position;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrPosition NewPosition( new CPosition( this, Position ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MPosition[ NewPosition->VarietyID ] = NewPosition;
		::ReleaseMutex( this->DataMutex );
	}
	return true;
}

bool CAccount::SetPositionDetail( DWORD VarietyID, CPositionDetail& PositionDetail )
{
	_Position::iterator ite = MPosition.find( VarietyID );
	if( ite != MPosition.end() )
		return ite->second->SetPositionDetail( PositionDetail );
	else
	{
		struct_VarietyInfo VarietyInfo;
		VarietyInfo.LongOrShort = _Long;
		VarietyInfo.HedgeFlag = _TG;
		VarietyInfo.InstrumentID = _T("");
		if( !this->pProduct->pData->GetVarietyInfo( VarietyID, VarietyInfo ) )
			return false;
		CPosition Position;
		Position.VarietyID = VarietyID;
		Position.LongOrShort = VarietyInfo.LongOrShort;
		Position.HedgeFlag = VarietyInfo.HedgeFlag;
		Position.InstrumentID = VarietyInfo.InstrumentID;
		if( !this->SetPosition( Position ) )
			return false;
		ite = MPosition.find( VarietyID );
		if( ite == MPosition.end() )
			return false;
		return ite->second->SetPositionDetail( PositionDetail );
	}
	return true;
}

bool CAccount::SetOrder( QWORD PlanID, COrder& Order )
{
	_NewTradePlan::iterator ite = MNewTradePlan.find( PlanID );
	if( ite != MNewTradePlan.end() )
		return ite->second->SetOrder( Order );
	else
	{
		CNewTradePlan NewTradePlan;
		NewTradePlan.PlanID = PlanID;
		NewTradePlan.Type = Order.Type;
		NewTradePlan.Direction = Order.Direction;
		NewTradePlan.HedgeFlag = Order.HedgeFlag;
		NewTradePlan.InstrumentID = Order.InstrumentID;
		NewTradePlan.Volume1 = Order.Volume1;
		NewTradePlan.Volume2 = Order.Volume2;
		NewTradePlan.StartTime = Order.Time;
		NewTradePlan.EndTime = Order.Time;
		NewTradePlan.Status = _Unknow;
		PtrNewTradePlan PNewTradePlan( new CNewTradePlan( this, NewTradePlan ) );

		::WaitForSingleObject( this->DataMutex, INFINITE );
		MNewTradePlan[ PNewTradePlan->PlanID ] = PNewTradePlan;
		::ReleaseMutex( this->DataMutex );

		ite = MNewTradePlan.find( PlanID );
		if( ite == MNewTradePlan.end() )
			return false;  // 无计划委托异常

		return ite->second->SetOrder( Order );
	}
}

bool CAccount::SetTrade( QWORD PlanID, QWORD OrderID, CTrade& Trade )
{
	_NewTradePlan::iterator ite = MNewTradePlan.find( PlanID );
	if( ite != MNewTradePlan.end() )
		return ite->second->SetTrade( OrderID, Trade );
	else
	{
		CNewTradePlan NewTradePlan;
		NewTradePlan.PlanID = PlanID;
		NewTradePlan.Type = Trade.Type;
		NewTradePlan.Direction = Trade.Direction;
		NewTradePlan.HedgeFlag = Trade.HedgeFlag;
		NewTradePlan.InstrumentID = Trade.InstrumentID;
		NewTradePlan.Volume1 = Trade.Volume;
		NewTradePlan.Volume2 = Trade.Volume;
		NewTradePlan.StartTime = Trade.Time;
		NewTradePlan.EndTime = Trade.Time;
		NewTradePlan.Status = _Unknow;
		PtrNewTradePlan PNewTradePlan( new CNewTradePlan( this, NewTradePlan ) );

		::WaitForSingleObject( this->DataMutex, INFINITE );
		MNewTradePlan[ PNewTradePlan->PlanID ] = PNewTradePlan;
		::ReleaseMutex( this->DataMutex );

		ite = MNewTradePlan.find( PlanID );
		if( ite == MNewTradePlan.end() )
			return false;  // 无计划委托异常

		return ite->second->SetTrade( OrderID, Trade );
	}
}

bool CAccount::SetOriginTradePlan( COriginTradePlan& OriginTradePlan )
{
	_OriginTradePlan::iterator ite = MOriginTradePlan.find( OriginTradePlan.PlanID );
	if( ite != MOriginTradePlan.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = OriginTradePlan;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrOriginTradePlan pOriginTradePlan( new COriginTradePlan( this, OriginTradePlan ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MOriginTradePlan[ pOriginTradePlan->PlanID ] = pOriginTradePlan;
		::ReleaseMutex( this->DataMutex );
	}
	return true;
}

bool CAccount::SetCancelTradeStatus( QWORD PlanID, _EnumPlanStatus Status )
{
	_NewTradePlan::iterator ite1 = MNewTradePlan.find( PlanID );
	if( ite1 == MNewTradePlan.end() )
		return false;
	else
	{
		if( Status == _CompleteOver && ite1->second->Status != _CancelForRetry )
			ite1->second->Status = _CancelOver;

		if( ite1->second->LinkPlanID != 0 )
		{
			_NewTradePlan::iterator ite2 = MNewTradePlan.find( ite1->second->LinkPlanID );
			if( ite1 == MNewTradePlan.end() )
				return false;

			if( ite2->second->Type == _Cancel )
				ite2->second->Status = Status;
		}
	}
	
	return true;
}

CVariety::CVariety()
{
	MaxVarietyID = 0;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
}

CVariety::~CVariety()
{
	MVariety.clear();
	MVarietyInfo.clear();
	CloseHandle( DataMutex );
}

DWORD CVariety::GetVarietyID()
{
	DWORD VarietyID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	VarietyID = this->MaxVarietyID++;
	::ReleaseMutex( this->DataMutex );
	return VarietyID;
}

CProduct::CProduct()
{
	ProductID = _T("");
	PreMargin = 0;
	PreFrzCash = 0; 
	PreAviCash = 0;
	Margin = 0;
	FrzCash = 0;
	AviCash = 0;
	Deposit = 0;
	Withdraw = 0;
	OtherCash = 0;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pData = NULL;
}

CProduct::CProduct( CData* lpData, CProduct &Product )
{
	ProductID = Product.ProductID;
	PreMargin = Product.PreMargin;
	PreFrzCash = Product.PreFrzCash; 
	PreAviCash = Product.PreAviCash;
	Margin = Product.Margin;
	FrzCash = Product.FrzCash;
	AviCash = Product.AviCash;
	Deposit = Product.Deposit;
	Withdraw = Product.Withdraw;
	OtherCash = Product.OtherCash;
	DataMutex = CreateMutex(NULL,FALSE,NULL);
	pData = lpData;
}

CProduct::~CProduct()
{
	MAccount.clear();
	CloseHandle( DataMutex );
}

bool CProduct::ConvertOriginToNewPlan()
{
	_Account::iterator ite = MAccount.begin();
	while( ite != MAccount.end() )
	{
		if( !ite->second->ConvertOriginToNewPlan() )
			return false;
		ite++;
	}
	return true;
}

bool CProduct::CheckOriginTradePlan()
{
	_Account::iterator ite = MAccount.begin();
	while( ite != MAccount.end() )
	{
		if( !ite->second->CheckOriginTradePlan() )
			return false;
		ite++;
	}
	return true;
}

bool CProduct::TradePlanRunManage()
{
	_Account::iterator ite = MAccount.begin();
	while( ite != MAccount.end() )
	{
		if( !ite->second->StartHoldToNewPlan() )
			return false;
		if( !ite->second->CanceltheUnTradedOrderToNewPlan() )
			return false;
		if( !ite->second->RetrytheCanceledOrderToNewPlan() )
			return false;
		if( !ite->second->ReSetOriginTradePlan() )
			return false;

		ite++;
	}
	return true;
}

bool CProduct::SetAccount( CAccount& Account )
{
	_Account::iterator ite = MAccount.find( Account.AccountID );
	if( ite != MAccount.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = Account;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrAccount NewAccount( new CAccount( this, Account ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MAccount[ NewAccount->AccountID ] = NewAccount;
		::ReleaseMutex( this->DataMutex );
	}
	return true;
}

bool CProduct::SetPosition( CString AccountID, CPosition& Position )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite != MAccount.end() )
		return ite->second->SetPosition( Position );
	else
	{
		CAccount Account;
		Account.AccountID = AccountID;
		if( !this->SetAccount( Account ) )
			return false;
		ite = MAccount.find( AccountID );
		if( ite == MAccount.end() )
			return false;
		return ite->second->SetPosition( Position );
	}
	return true;
}

bool CProduct::SetPositionDetail( CString AccountID, DWORD VarietyID, CPositionDetail& PositionDetail )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite != MAccount.end() )
		return ite->second->SetPositionDetail( VarietyID, PositionDetail );
	else
	{
		CAccount Account;
		Account.AccountID = AccountID;
		if( !this->SetAccount( Account ) )
			return false;
		ite = MAccount.find( AccountID );
		if( ite == MAccount.end() )
			return false;
		return ite->second->SetPositionDetail( VarietyID, PositionDetail );
	}
	return true;
}

bool CProduct::SetOrder( CString AccountID, QWORD PlanID, COrder& Order )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite != MAccount.end() )
		return ite->second->SetOrder( PlanID, Order );
	else
	{
		CAccount Account;
		Account.AccountID = AccountID;
		if( !this->SetAccount( Account ) )
			return false;
		ite = MAccount.find( AccountID );
		if( ite == MAccount.end() )
			return false;
		return ite->second->SetOrder( PlanID, Order );
	}
	return true;
}

bool CProduct::SetTrade( CString AccountID, QWORD PlanID, QWORD OrderID, CTrade& Trade )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite != MAccount.end() )
		return ite->second->SetTrade( PlanID, OrderID, Trade );
	else
	{
		CAccount Account;
		Account.AccountID = AccountID;
		if( !this->SetAccount( Account ) )
			return false;
		ite = MAccount.find( AccountID );
		if( ite == MAccount.end() )
			return false;
		return ite->second->SetTrade( PlanID, OrderID, Trade );
	}
	return true;
}

bool CProduct::SetOriginTradePlan( CString AccountID, COriginTradePlan& OriginTradePlan )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite != MAccount.end() )
		return ite->second->SetOriginTradePlan( OriginTradePlan );
	else
	{
		CAccount Account;
		Account.AccountID = AccountID;
		if( !this->SetAccount( Account ) )
			return false;
		ite = MAccount.find( AccountID );
		if( ite == MAccount.end() )
			return false;
		return ite->second->SetOriginTradePlan( OriginTradePlan );
	}
	return true;
}

bool CProduct::SetCancelTradeStatus( CString AccountID, QWORD PlanID, _EnumPlanStatus Status )
{
	_Account::iterator ite = MAccount.find( AccountID );
	if( ite == MAccount.end() )
		return false;
	else
		return ite->second->SetCancelTradeStatus( PlanID, Status );
}

CData::CData()
{
	PositionPath = _T("");
	TradePlanPath = _T("");
	MaxVarietyID = 0;
	MaxOrderID = 0;
	MaxTradeID = 0;
	MaxOriginPlanID = 0;
	MaxNewPlanID = 0;
	MaxPositionID = 0;
	MaxOrderNumber = 0;   
	pVariety = PtrVariety( new CVariety() );
	DataMutex = CreateMutex(NULL,FALSE,NULL);
}

CData::~CData()
{
	pVariety.reset();
	MProduct.clear();
	MObject.clear();
	MOrderInfo.clear();
	ErrorVT.clear();
	::CloseHandle(DataMutex);
}

bool CData::InitLocalPathData()
{
	if( !this->GetConfigInfo( _T("PositionPath"), this->PositionPath ) )
		return false;
	if( !this->GetConfigInfo( _T("TradePlanPath"), this->TradePlanPath ) )
		return false;

	return true;
}

DWORD CData::GetVariety( struct_VarietyInfo& VarietyInfo )
{
	_LongOrShort::iterator ite1 = this->pVariety->MVariety.find( VarietyInfo.LongOrShort );
	if( ite1 == this->pVariety->MVariety.end() )
	{
		DWORD VarietyID = this->pVariety->GetVarietyID();
		_Variety MV1;
		MV1[ VarietyInfo.InstrumentID ] = VarietyID;
		_Hedge MH1;
		MH1[ VarietyInfo.HedgeFlag ] = MV1;
		
		this->pVariety->MVariety[ VarietyInfo.LongOrShort ] = MH1;
		this->pVariety->MVarietyInfo[ VarietyID ] = VarietyInfo;
		return VarietyID;
	}
	else
	{
		_Hedge::iterator ite2 = ite1->second.find( VarietyInfo.HedgeFlag );
		if( ite2 == ite1->second.end() )
		{
			DWORD VarietyID = this->pVariety->GetVarietyID();
			_Variety MV1;
			MV1[ VarietyInfo.InstrumentID ] = VarietyID;

			ite1->second[ VarietyInfo.HedgeFlag ] = MV1;
			this->pVariety->MVarietyInfo[ VarietyID ] = VarietyInfo;
			return VarietyID;
		}
		else
		{
			_Variety::iterator ite3 = ite2->second.find( VarietyInfo.InstrumentID );
			if( ite3 == ite2->second.end() )
			{
				DWORD VarietyID = this->pVariety->GetVarietyID();

				ite2->second[ VarietyInfo.InstrumentID ] = VarietyID;
				this->pVariety->MVarietyInfo[ VarietyID ] = VarietyInfo;
				return VarietyID;
			}
			else
				return ite3->second;
		}
	}
}

bool CData::GetVarietyInfo( DWORD VarietyID, struct_VarietyInfo& VarietyInfo )
{
	map<DWORD, struct_VarietyInfo>::iterator ite = this->pVariety->MVarietyInfo.find( VarietyID );
	if( ite == this->pVariety->MVarietyInfo.end() )
		return false;
	VarietyInfo.LongOrShort = ite->second.LongOrShort;
	VarietyInfo.HedgeFlag = ite->second.HedgeFlag;
	VarietyInfo.InstrumentID = ite->second.InstrumentID;
	return true;
}

bool CData::OutportPositionFile()
{
	::DeleteFile( (LPCSTR)this->PositionPath );
	if( !this->PositionFile.Open( this->PositionPath, CFile::modeCreate | CFile::modeWrite ) )
		return false;

	this->PositionFile.SeekToBegin();  

	CString strTitle = _T("产品名,账号,多空方向,投机套保,合约代码,昨仓,今仓,总仓\n");
	this->PositionFile.WriteString( (LPCTSTR)strTitle );
	
	char buffer[4096];
	_Product::iterator ite1 = this->MProduct.begin();
	while( ite1 != this->MProduct.end() )
	{
		_Account::iterator ite2 = ite1->second->MAccount.begin();
		while( ite2 != ite1->second->MAccount.end() )
		{
			_Position::iterator ite3 = ite2->second->MPosition.begin();
			while( ite3 != ite2->second->MPosition.end() )
			{
				CString LongOrShort = _T("");
				CString Hedgeflag = _T("");
				if( ite3->second->LongOrShort == _Long )
					LongOrShort = _T("L");
				if( ite3->second->LongOrShort == _Short )
					LongOrShort = _T("S");
				if( ite3->second->HedgeFlag == _TG )
					Hedgeflag = _T("1");
				if( ite3->second->HedgeFlag == _TB )
					Hedgeflag = _T("2");
				if( ite3->second->HedgeFlag == _TL )
					Hedgeflag = _T("3");
				int Offset = sprintf( buffer, _T("%s,%s,%s,%s,%s,%d,%d,%d\n"), ite1->first, ite2->first, LongOrShort, Hedgeflag, ite3->second->InstrumentID, ite3->second->YdVolume, ite3->second->TdVolume, ite3->second->SumVolume );
				this->PositionFile.Write( buffer, Offset );
	
				ite3++;
			}
			ite2++;
		}
		ite1++;
	}
	this->PositionFile.Close();

	return true;
}

bool CData::ImportOriginTradePlanFile()
{
	if( !this->TradePlanFile.Open( this->TradePlanPath, CFile::modeRead ) )
		return false;

	CString strText = _T("");
	bool bfirst = true;
	this->TradePlanFile.SeekToBegin();   
	vector<CString> substrSvText;
	while( this->TradePlanFile.ReadString( strText ) )
	{
		if( bfirst )
		{
			bfirst = false;
			continue;
		}
		int index = strText.Find( _T(",") );
		while ( index != -1 )
		{
			substrSvText.push_back( strText.Left( index ) );
			strText = strText.Right( strText.GetLength() - index - 1 );
			index = strText.Find( _T(",") );
		}
		substrSvText.push_back( strText );
		// 文件格式：产品名，账号，开平标志，多空方向，投机套保标志，合约代码，数量，开始时间，结束时间
		if( substrSvText.size() != 9 )
			return false;

		COriginTradePlan OriginTradePlan;
		OriginTradePlan.PlanID = this->GetOriginPlanID();
		CString ProductID = substrSvText[0].Trim();
		CString AccountID = substrSvText[1].Trim();
		if( substrSvText[2].Trim() == _T("K") )
			OriginTradePlan.Type = _Open;
		if( substrSvText[2].Trim() == _T("P") )
			OriginTradePlan.Type = _Close;
		if( substrSvText[3].Trim() == _T("B") )
			OriginTradePlan.Direction = _Buy;
		if( substrSvText[3].Trim() == _T("S") )
			OriginTradePlan.Direction = _Sell;
		if( substrSvText[4].Trim() == _T("1") )
			OriginTradePlan.HedgeFlag = _TG;
		if( substrSvText[4].Trim() == _T("2") )
			OriginTradePlan.HedgeFlag = _TB;
		if( substrSvText[4].Trim() == _T("3") )
			OriginTradePlan.HedgeFlag = _TL;

		OriginTradePlan.InstrumentID = substrSvText[5].Trim();
		OriginTradePlan.Volume = atoi(substrSvText[6].Trim());
		OriginTradePlan.BeginTime = atoi(substrSvText[7].Trim());
		OriginTradePlan.EndTime = atoi(substrSvText[8].Trim());
		
		struct_VarietyInfo VarietyInfo;
		if( OriginTradePlan.Direction == _Buy )
		{
			if( OriginTradePlan.Type == _Open )
				VarietyInfo.LongOrShort = _Long;
			else
				VarietyInfo.LongOrShort = _Short;
		}
		else
		{
			if( OriginTradePlan.Type == _Open )
				VarietyInfo.LongOrShort = _Short;
			else
				VarietyInfo.LongOrShort = _Long;
		}
		VarietyInfo.HedgeFlag = OriginTradePlan.HedgeFlag;
		VarietyInfo.InstrumentID = OriginTradePlan.InstrumentID;
		DWORD VarietyID = this->GetVariety( VarietyInfo );
		
		if( !this->SetOriginTradePlan( ProductID, AccountID, OriginTradePlan ) )
			return false;
		substrSvText.clear();
	}
	this->TradePlanFile.Close();

	return true;
}

bool CData::ConvertOriginToNewPlan()
{
	if( !this->CheckOriginTradePlan() )
		return false;

	_Product::iterator ite = this->MProduct.begin();
	while( ite != this->MProduct.end() )
	{
		if( !ite->second->ConvertOriginToNewPlan() )
			return false;
		ite++;
	}
	return true;
}

bool CData::CheckOriginTradePlan()
{
	_Product::iterator ite = this->MProduct.begin();
	while( ite != this->MProduct.end() )
	{
		if( !ite->second->CheckOriginTradePlan() )
			return false;
		ite++;
	}
	return true;
}

bool CData::TradePlanRunManage()
{
	_Product::iterator ite = this->MProduct.begin();
	while( ite != this->MProduct.end() )
	{
		if( !ite->second->TradePlanRunManage() )
			return false;

		ite++;
	}
	return true;
}

bool CData::SetProduct( CProduct& Product )
{
	_Product::iterator ite = MProduct.find( Product.ProductID );
	if( ite != MProduct.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = Product;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrProduct NewProduct( new CProduct( this, Product ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MProduct[ NewProduct->ProductID ] = NewProduct;
		::ReleaseMutex( this->DataMutex );
	}
	return true;
}

bool CData::SetAccount( CString ProductID, CAccount& Account )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetAccount( Account );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetAccount( Account );
	}
	return true;
}

bool CData::SetPosition( CString ProductID, CString AccountID, CPosition& Position )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetPosition( AccountID, Position );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetPosition( AccountID, Position );
	}
	return true;
}

bool CData::SetPositionDetail( CString ProductID, CString AccountID, DWORD VarietyID, CPositionDetail& PositionDetail )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetPositionDetail( AccountID, VarietyID, PositionDetail );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetPositionDetail( AccountID, VarietyID, PositionDetail );
	}
	return true;
}

bool CData::SetObject( CObj& Object )
{
	_Obj::iterator ite = MObject.find( Object.InstrumentID );
	if( ite != MObject.end() )
	{
		::WaitForSingleObject( ite->second->DataMutex, INFINITE );
		*ite->second = Object;
		::ReleaseMutex( ite->second->DataMutex );
	}
	else
	{
		PtrObj NewObject( new CObj( this, Object ) );
		::WaitForSingleObject( this->DataMutex, INFINITE );
		MObject[ NewObject->InstrumentID ] = NewObject;
		::ReleaseMutex( this->DataMutex );
	}
	return true;
}

bool CData::SetDyna( CString InstrumentID, CDynaQuote& Dyna )
{
	_Obj::iterator ite = MObject.find( InstrumentID );
	if( ite != MObject.end() )
		return ite->second->SetDyna( Dyna );
	else
	{
		CObj Object;
		Object.InstrumentID = InstrumentID;
		if( !this->SetObject( Object ) )
			return false;
		ite = MObject.find( InstrumentID );
		if( ite == MObject.end() )
			return false;
		return ite->second->SetDyna( Dyna );
	}
	return true;
}

bool CData::SetOrder( CString ProductID, CString AccountID, QWORD PlanID, COrder& Order )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetOrder( AccountID, PlanID, Order );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetOrder( AccountID, PlanID, Order );
	}
	return true;
}

bool CData::SetTrade( CString ProductID, CString AccountID, QWORD PlanID, QWORD OrderID, CTrade& Trade )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetTrade( AccountID, PlanID, OrderID, Trade );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetTrade( AccountID, PlanID, OrderID, Trade );
	}
	return true;
}

bool CData::SetOriginTradePlan( CString ProductID, CString AccountID, COriginTradePlan& OriginTradePlan )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite != MProduct.end() )
		return ite->second->SetOriginTradePlan( AccountID, OriginTradePlan );
	else
	{
		CProduct Product;
		Product.ProductID = ProductID;
		if( !this->SetProduct( Product ) )
			return false;
		ite = MProduct.find( ProductID );
		if( ite == MProduct.end() )
			return false;
		return ite->second->SetOriginTradePlan( AccountID, OriginTradePlan );
	}
	return true;
}

bool CData::SetErrorMsg( CString& Msg)
{
	this->ErrorVT.push_back( Msg );
	return true;
}

bool CData::SetCancelTradeStatus( CString ProductID, CString AccountID, QWORD PlanID, _EnumPlanStatus Status )
{
	_Product::iterator ite = MProduct.find( ProductID );
	if( ite == MProduct.end() )
		return false;
	else
		ite->second->SetCancelTradeStatus( AccountID, PlanID, Status );
}

bool CData::GetConfigInfo( CString Name, CString& Text )
{
	CString strDirText = _T("");
	char szPathTemp[ 512 ];
 	GetModuleFileName( NULL, szPathTemp, 512 );  
	//取出文件路径   
 	for( int i = strlen( szPathTemp ); i >= 0; i-- )
 	{   
 		if( szPathTemp[ i ] == '\\' )
 		{
 			szPathTemp[ i ] = '\0';
 			break;   
 		}
 	}
	strDirText.Format( "%s\\config.txt", szPathTemp );

	if( !this->ConfigFile.Open( strDirText, CFile::modeRead ) )
		return false;

	CString strText = _T("");
	this->ConfigFile.SeekToBegin();   //读取数据文件标题
	vector<CString> substrSvText;
	while( this->ConfigFile.ReadString( strText ) )
	{
		int index = strText.Find( _T("=") );
		if( index == -1 )
			return false;

		substrSvText.push_back( strText.Left(index) );
		strText = strText.Right( strText.GetLength() - index -1 );
		substrSvText.push_back( strText );

		if( substrSvText.size() < 2 )
			return false;
		
		if( substrSvText[0].Trim() == Name )
			Text = substrSvText[1].Trim();

		substrSvText.clear();
	}
	this->ConfigFile.Close();

	return true;
}

QWORD CData::GetOrderID( struct_OrderInfo& OrderInfo )
{
	QWORD OrderID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	OrderID = this->MaxOrderID++;
	::ReleaseMutex( this->DataMutex );
	MOrderInfo[ OrderID ] = OrderInfo;
	return OrderID;
}

bool CData::GetOrderInfo( QWORD OrderID, struct_OrderInfo& OrderInfo )
{
	map<QWORD, struct_OrderInfo>::iterator ite = MOrderInfo.find( OrderID );
	if( ite == MOrderInfo.end() )
		return false;
	OrderInfo.ProductID = ite->second.ProductID;
	OrderInfo.AccountID = ite->second.AccountID;
	OrderInfo.NewPlanID = ite->second.NewPlanID;
	return true;
}

QWORD CData::GetTradeID()
{
	QWORD TradeID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	TradeID = this->MaxTradeID++;
	::ReleaseMutex( this->DataMutex );
	return TradeID;
}

QWORD CData::GetOriginPlanID()
{
	QWORD PlanID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	PlanID = this->MaxOriginPlanID++;
	::ReleaseMutex( this->DataMutex );
	return PlanID;
}

QWORD CData::GetNewPlanID()
{
	QWORD PlanID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	PlanID = this->MaxNewPlanID++;
	::ReleaseMutex( this->DataMutex );
	return PlanID;
}

QWORD CData::GetPositionID()
{
	QWORD PositionID = 0;
	::WaitForSingleObject( this->DataMutex, INFINITE );
	PositionID = this->MaxPositionID++;
	::ReleaseMutex( this->DataMutex );
	return PositionID;
}
