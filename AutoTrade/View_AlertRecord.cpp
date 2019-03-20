#include "stdafx.h"
#include "View_AlertRecord.h"

Type_VCAertRecord vec_CAertRecord;
IMPLEMENT_DYNAMIC(CView_AlertRecord, CBaseView)
CView_AlertRecord::CView_AlertRecord(CDockListener &listener) : CBaseView(listener)
{

}

CView_AlertRecord::~CView_AlertRecord()
{

}

void CView_AlertRecord::ReleaseView()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CView_AlertRecord, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CView_AlertRecord::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BOOL CView_AlertRecord::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

int CView_AlertRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SPtr_CData = G.SPtr_CData;
	pCApiFun = G.pCApiFun;
	//InitialWindow();
	CreateControls();
	initPreRecord();
	return 0;
}

void CView_AlertRecord::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("预警记录");
}

void CView_AlertRecord::CreateControls()
{
	CGridCtrlEx_alertRecord.Create(CRect(0, 0, 0, 0), this, IDC_Grid_AlertRecord);
	InitGrid();
	return;
}

bool CView_AlertRecord::initPreRecord()
{
	string s_path;
	tool::getAlertRecordPath(s_path, "自动交易");
	std::string s_FileContent;
	tool::File2String(s_path, s_FileContent);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(s_FileContent, root, false))
	{
		int Sum = root["recordInfo"].size();
		for (int i = 0; i < Sum; ++i)
		{
			CAertRecord obj_CAertRecord; 
			obj_CAertRecord.s_alertTime = root["recordInfo"][i]["s_alertTime"].asString().c_str();
			obj_CAertRecord.stock_code = root["recordInfo"][i]["stock_code"].asString().c_str();
			obj_CAertRecord.stock_name = root["recordInfo"][i]["stock_name"].asString().c_str();
			obj_CAertRecord.cause = root["recordInfo"][i]["cause"].asString().c_str();
			obj_CAertRecord.entrust_no = root["recordInfo"][i]["entrust_no"].asString().c_str();

			obj_CAertRecord.entrust_price = root["recordInfo"][i]["entrust_price"].asString().c_str();
			obj_CAertRecord.entrust_amount = root["recordInfo"][i]["entrust_amount"].asString().c_str();
			obj_CAertRecord.note = root["recordInfo"][i]["note"].asString().c_str();
			SPtr_CData->SetAlertRecord(obj_CAertRecord);
		}
	}
	else
	{
		return true;
	}
	SetAlertRecord();
	return true;
}

void CView_AlertRecord::SetAlertRecord()
{
	SPtr_CData->GetVAlertRecord(vec_CAertRecord);
	CGridCtrlEx_alertRecord.SetRowCount(vec_CAertRecord.size() + 1);
	CGridCtrlEx_alertRecord.MySetEndVScroll();
	CGridCtrlEx_alertRecord.Invalidate();
}

BOOL CView_AlertRecord::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 657, 420);
	return TRUE;
}

void CView_AlertRecord::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(CGridCtrlEx_alertRecord.GetSafeHwnd()))
	{
		CGridCtrlEx_alertRecord.MoveWindow(0, 0, cx, cy, TRUE);
	}
}

void CView_AlertRecord::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	CGridCtrlEx_alertRecord.SetVirtualMode(TRUE);
	CGridCtrlEx_alertRecord.SetAutoSizeStyle();
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	CGridCtrlEx_alertRecord.SetCols(pTagCols, nColCnt);//设置列头
	CGridCtrlEx_alertRecord.SetCallbackFunc(GridCallback, (LPARAM)this);
	//CGridCtrlEx_alertRecord.SetVirtualCompare(CompareVirtual);

	CGridCtrlEx_alertRecord.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	CGridCtrlEx_alertRecord.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	CGridCtrlEx_alertRecord.SetGridLines(GVL_NONE);
	CGridCtrlEx_alertRecord.SetFont(CFont::FromHandle(SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N)));
	CGridCtrlEx_alertRecord.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	CGridCtrlEx_alertRecord.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	CGridCtrlEx_alertRecord.SetSingleRowSelection(TRUE);
	//CGridCtrlEx_alertRecord.SetRowCount(1);
}

BOOL CALLBACK CView_AlertRecord::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	CView_AlertRecord* pCView_AlertRecord = (CView_AlertRecord *)lParam;
	if (nullptr == pCView_AlertRecord)
	{
		return FALSE;
	}
	if (!IsWindow(pCView_AlertRecord->GetSafeHwnd()))
	{
		return FALSE;
	}
	//设置格式
	pDispInfo->item.nFormat = pCView_AlertRecord->CGridCtrlEx_alertRecord.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pCView_AlertRecord->CGridCtrlEx_alertRecord.GetColText(pDispInfo->item.col);
	}
	else
	{
		if (pDispInfo->item.row - 1 < vec_CAertRecord.size())
		{
			pCView_AlertRecord->SetCell(*pDispInfo,vec_CAertRecord[pDispInfo->item.row - 1]);
		}
	}
	return TRUE;
}

void CView_AlertRecord::SetCell(GV_DISPINFO & dispinfo, CAertRecord &obj_CAertRecord)
{
	DealQuit(SPtr_CData->bQuit_ErrorInfo);
	int nID = CGridCtrlEx_alertRecord.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColID_time:
	{
		dispinfo.item.strText = obj_CAertRecord.s_alertTime;
		break;
	}
	case ColID_secCode:
	{
		dispinfo.item.strText = obj_CAertRecord.stock_code;
		break;
	}
	case ColID_secName:
	{
		dispinfo.item.strText = obj_CAertRecord.stock_name;
		break;
	}
	case ColID_triggerName:
	{
		dispinfo.item.strText = obj_CAertRecord.cause;
		break;
	}
	case ColID_entrustNo:
	{
		dispinfo.item.strText = obj_CAertRecord.entrust_no;
		break;
	}
	case ColID_entrustPrice:
	{
		dispinfo.item.strText = obj_CAertRecord.entrust_price;
		break;
	}
	case ColID_entrustAmount:
	{
		dispinfo.item.strText = obj_CAertRecord.entrust_amount;
		break;
	}
	case ColID_note:
		dispinfo.item.strText = obj_CAertRecord.note;
		break;
	default:
		break;
	}

}
