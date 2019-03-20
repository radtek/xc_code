#include "stdafx.h"
#include "Fundview_004.h"
#include "../tool/baseView/GridRes.h"
#include "ViewManager_fast.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"

// CFundView
IMPLEMENT_DYNAMIC(CFundView, CBaseView)

CFundView::CFundView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CFundView::~CFundView()
{
}

void CFundView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CFundView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, IDC_Button_Refresh, &CFundView::OnBtnRefresh)
END_MESSAGE_MAP()

// CFundView 消息处理程序

void CFundView::ReleaseView()
{
	delete this;
}

int CFundView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	return 0;
}

void CFundView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("资金");
}

void CFundView::CreateControls()
{
	//InitFund(m_listFund, this, IDC_LISTBOX_FUND);
	m_gridFund.Create(CRect(0, 0, 0, 0), this, IDC_GRID_FUNDVIEW);
	InitGrid();
	m_btnRefresh.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_Refresh, "点击刷新");
}

void CFundView::SetData()
{
	if (G.SPtr_CData != nullptr)
	{
		CFund fund;
		if (G.SPtr_CData->GetFund(G.SPtr_CData->fund_account, fund) == false)
		{
			LOG(ERROR) << "GetFund fail";
			ExceptionInterrupt;
			return;
		}
		SetGridData(fund);
	}
}

void CFundView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	m_gridFund.SetCols(pTagCols, nColCnt);
	m_gridFund.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridFund.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridFund.SetGridLines(GVL_NONE);
	m_gridFund.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridFund.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridFund.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridFund.SetRowCount(2);
	//m_gridOrder.SetEditable(TRUE);
}

void CFundView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcBtnRefreshCal;
	G.GetBtnRect("点击刷新", CTradeDlg::s_hFontOrder, rcBtnRefreshCal);
	int nOffsetY = 4 * 2 + rcBtnRefreshCal.Height();

	CRect rcBtnRefresh;
	rcBtnRefresh.left = 120;
	rcBtnRefresh.top = 4;
	rcBtnRefresh.right = rcBtnRefresh.left + rcBtnRefreshCal.Width();
	rcBtnRefresh.bottom = rcBtnRefresh.top + rcBtnRefreshCal.Height();

	if (IsWindow(m_btnRefresh.GetSafeHwnd()))
	{
		m_btnRefresh.MoveWindow(&rcBtnRefresh);
	}

	if (IsWindow(m_gridFund.GetSafeHwnd()))
	{
		m_gridFund.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}

}

BOOL CFundView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 939, 272);
	return TRUE;
}

BOOL CFundView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CFundView::SetGridData(CFund& fund)
{
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_MoneyType), fund.money_type);

	CString strAssetBalance;
	strAssetBalance.Format(_T("%0.0f"), fund.asset_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_AssetBalance), strAssetBalance);

	CString strMarketValue;
	strMarketValue.Format(_T("%0.0f"), fund.market_value);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_MarketValue), strMarketValue);

	CString strCurrentBalance;
	strCurrentBalance.Format(_T("%0.0f"), fund.current_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_CurrentBalance), strCurrentBalance);

	CString strEnableBalance;
	strEnableBalance.Format(_T("%0.0f"), fund.enable_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_EnableBalance), strEnableBalance);

	CString strFetchBalance;
	strFetchBalance.Format(_T("%0.0f"), fund.fetch_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_FetchBalance), strFetchBalance);

	CString strRealBuyBalance;
	strRealBuyBalance.Format(_T("%0.0f"), fund.real_buy_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_RealBuyBalance), strRealBuyBalance);

	CString strRealSellBalance;
	strRealSellBalance.Format(_T("%0.0f"), fund.real_sell_balance);
	m_gridFund.SetItemTextEx(1, m_gridFund.GetColNum(ColId_RealSellBalance), strRealSellBalance);

	m_gridFund.RedrawRowEx(1, TRUE);
}

void CFundView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseView::OnShowWindow(bShow, nStatus);
	if (G.SPtr_CData->bBeginRelease == false)
	{
		UpdateQueryFund_Fast();
	}
}

bool CFundView::UpdateQueryFund_Fast()
{
	string s_Fun332255;
	tool::GainQueryFund332255(s_Fun332255, G.SPtr_CData);
	int RefId_HS = tool::SendOrder(G.pCApiFun, Api_Asy, s_Fun332255, G.SPtr_CData);
	if (RefId_HS < 0)
	{
		return false;
	}
	G.SPtr_CData->SetRefId_T2(RefId_HS, RefType_332255_NoFirstQueryFund);
	SetData();
	return true;
}

void CFundView::OnBtnRefresh(NMHDR *pNmhdr, LRESULT *pResult)
{
	UpdateQueryFund_Fast();
}
















