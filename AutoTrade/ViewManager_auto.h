#pragma once
#include "../tool/baseView/ViewManager.h"

#include "View_EntrustEach.h"
#include "View_TradeConfig.h"
#include "View_AlertRecord.h"
#include "View_CustomPool.h"
#include "View_blackList.h"

class CViewManager_auto;
#define SingleViewManager Singleton<CViewManager_auto>::Instance()

enum
{
	ID_base_view = ID_BASE_VIEW,
	ID_View_EntrustEach,//EntrustEach
	ID_View_TradeConig,
	ID_View_AlertRecord,
	ID_View_CustomPool,
	ID_View_blackList,
	ID_MAX_VIEW,
	ID_view_nil = ID_VIEW_NIL,
};

class CViewManager_auto :public CViewManager
{
public:
	CViewManager_auto();
	~CViewManager_auto();
};

