#include "stdafx.h"
#include "BiaoDiQuanView_011.h"
#include "../tool/baseView/GridRes.h"
#include "ViewManager_credit.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"


// CBiaoDiQuanView

IMPLEMENT_DYNAMIC(CBiaoDiQuanView, CBaseView)

CBiaoDiQuanView::CBiaoDiQuanView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CBiaoDiQuanView::~CBiaoDiQuanView()
{
}

void CBiaoDiQuanView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CBiaoDiQuanView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RETURN, IDC_Edit_StockCode, &CBiaoDiQuanView::OnEditResturn)

END_MESSAGE_MAP()

// CBiaoDiQuanView 消息处理程序

void CBiaoDiQuanView::ReleaseView()
{
	delete this;
}

int CBiaoDiQuanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateControls();
	return 0;
}

void CBiaoDiQuanView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("标的券");
}

void CBiaoDiQuanView::CreateControls()
{
	m_gridBiaoDiQuan.Create(CRect(0, 0, 0, 0), this, IDC_Grid_BiaoDiQuan);
	InitGrid();
	SetData();
	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("查询证券");
	m_editStockCode.EnableNumberControl(FALSE);

}

void CBiaoDiQuanView::SetData()
{
	if (G.SPtr_CData != nullptr)
	{
		_Security MRedrawBiaoDiQuan;
		_Account obj_MAccount;
		if (false == G.SPtr_CData->GetMAccount(G.SPtr_CData->fund_account, obj_MAccount))
		{
			ErrorLog("GetAccountMap fail");
			return;
		}
		for (auto iter = obj_MAccount.begin(); iter != obj_MAccount.end(); iter++)
		{
			_Security obj_MSecurity;
			if (false == G.SPtr_CData->GetMSecurity_BiaoDi(G.SPtr_CData->fund_account, iter->first, obj_MSecurity))
			{
				ErrorLog("GetSecurity_FinancingMap fail");
				continue;
			}
			MRedrawBiaoDiQuan.insert(obj_MSecurity.begin(), obj_MSecurity.end());
		}
		m_gridBiaoDiQuan.SetRowCount(MRedrawBiaoDiQuan.size() + 1);
		int nRowCount = 1;
		for (auto iter = MRedrawBiaoDiQuan.begin(); iter != MRedrawBiaoDiQuan.end(); iter++)
		{
			m_gridBiaoDiQuan.SetItemTextEx(nRowCount, m_gridBiaoDiQuan.GetColNum(ColId_stockName), iter->second->stock_name);
			m_gridBiaoDiQuan.SetItemTextEx(nRowCount, m_gridBiaoDiQuan.GetColNum(COLID_stockCode), iter->second->stock_code);
			CString strFinRatio;
			strFinRatio.Format(_T("%0.2f"), iter->second->fin_ratio);
			m_gridBiaoDiQuan.SetItemTextEx(nRowCount, m_gridBiaoDiQuan.GetColNum(ColId_FinRatio), strFinRatio);

			m_gridBiaoDiQuan.SetItemTextEx(nRowCount, m_gridBiaoDiQuan.GetColNum(ColId_FinStatus), GetFinStatus(iter->second->fin_status));

			CString strAssureRatio;
			strAssureRatio.Format(_T("%0.2f"), iter->second->assure_ratio);
			m_gridBiaoDiQuan.SetItemTextEx(nRowCount, m_gridBiaoDiQuan.GetColNum(ColId_AssureRatio), strAssureRatio);

			nRowCount++;
		}
		m_gridBiaoDiQuan.Invalidate();
	}
}

CString CBiaoDiQuanView::GetFinStatus(char fin_status)
{
	CString strFinStatus;
	switch (fin_status)
	{
	case '0':
		strFinStatus = "正常";
		break;
	case '1':
		strFinStatus = "暂停";
		break;
	case '2':
		strFinStatus = "作废";
		break;
	default:
		break;
	}
	return strFinStatus;
}

void CBiaoDiQuanView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	m_gridBiaoDiQuan.SetCols(pTagCols, nColCnt);
	m_gridBiaoDiQuan.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridBiaoDiQuan.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridBiaoDiQuan.SetGridLines(GVL_NONE);
	m_gridBiaoDiQuan.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridBiaoDiQuan.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridBiaoDiQuan.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridBiaoDiQuan.SetRowCount(1);
	//m_gridOrder.SetEditable(TRUE);
}

void CBiaoDiQuanView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcBtnCal;
	G.GetBtnRect("123469879", CTradeDlg::s_hFontOrder, rcBtnCal);
	int nOffsetY = 4 * 2 + rcBtnCal.Height();

	CRect rcBtn;
	rcBtn.left = 120;
	rcBtn.top = 4;
	rcBtn.right = rcBtn.left + rcBtnCal.Width();
	rcBtn.bottom = rcBtn.top + rcBtnCal.Height();

	if (IsWindow(m_editStockCode.GetSafeHwnd()))
	{
		m_editStockCode.MoveWindow(&rcBtn);
	}

	if (IsWindow(m_gridBiaoDiQuan.GetSafeHwnd()))
	{
		m_gridBiaoDiQuan.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}

}

BOOL CBiaoDiQuanView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 655, 630);
	return TRUE;
}

BOOL CBiaoDiQuanView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CBiaoDiQuanView::OnEditResturn(NMHDR *pNmhdr, LRESULT *pResult)
{
	CString strCode;
	m_editStockCode.GetWindowText(strCode);
	int nRowCount = m_gridBiaoDiQuan.GetRowCount();
	int nColCount = m_gridBiaoDiQuan.GetColumnCount();
	for (int nRow = 1; nRow < nRowCount; nRow++)
	{
		CString strCodeGrid = m_gridBiaoDiQuan.GetItemText(nRow, m_gridBiaoDiQuan.GetColNum(COLID_stockCode));
		if (strCodeGrid.Trim() == strCode.Trim())
		{
			m_gridBiaoDiQuan.MySetSelectedRangeEx(nRow, 0, nRow, nColCount - 1, TRUE);
			m_gridBiaoDiQuan.Invalidate(FALSE);
			m_editStockCode.SetWindowText("");
			break;
		}
	}
}

















