#include "stdafx.h"
#include "DanBaoQuanView_012.h"
#include "../tool/baseView/GridRes.h"
#include "ViewManager_credit.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"
// CDanBaoQuanView

IMPLEMENT_DYNAMIC(CDanBaoQuanView, CBaseView)

CDanBaoQuanView::CDanBaoQuanView(CDockListener &listener)
	: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CDanBaoQuanView::~CDanBaoQuanView()
{
}

void CDanBaoQuanView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CDanBaoQuanView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RETURN, IDC_Edit_StockCode, &CDanBaoQuanView::OnEditResturn)

END_MESSAGE_MAP()

// CDanBaoQuanView 消息处理程序

void CDanBaoQuanView::ReleaseView()
{
	delete this;
}

int CDanBaoQuanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	return 0;
}

void CDanBaoQuanView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("担保证券");
}

void CDanBaoQuanView::CreateControls()
{
	m_gridDanBaoQuan.Create(CRect(0, 0, 0, 0), this, IDC_Grid_BiaoDiQuan);
	InitGrid();
	SetData();
	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("查询证券");
	m_editStockCode.EnableNumberControl(FALSE);

}

void CDanBaoQuanView::SetData()
{
	if (G.SPtr_CData != nullptr)
	{
		_Security MRedrawDanBaoQuan;
		_Account obj_MAccount;
		if (false == G.SPtr_CData->GetMAccount(G.SPtr_CData->fund_account, obj_MAccount))
		{
			LOG(ERROR) << "GetAccountMap fail";
			ExceptionInterrupt;
			return;
		}
		for (auto iter = obj_MAccount.begin(); iter != obj_MAccount.end(); iter++)
		{
			_Security obj_MSecurity;
			//最简单的办法是申请查询担保证券的时候加入证券类别需求，因为查上海有八千多个，但是只有一千三百多个是股票，其次是寻找性能问题
			if (false == G.SPtr_CData->GetMSecurity_DanBao(G.SPtr_CData->fund_account, iter->first, obj_MSecurity))
			{
				LOG(ERROR) << "GetMSecurity_DanBao fail";
				ExceptionInterrupt;
				continue;
			}
			MRedrawDanBaoQuan.insert(obj_MSecurity.begin(), obj_MSecurity.end());
		}
		m_gridDanBaoQuan.SetRowCount(MRedrawDanBaoQuan.size() + 1);
		int nRowCount = 1;
		for (auto iter = MRedrawDanBaoQuan.begin(); iter != MRedrawDanBaoQuan.end(); iter++)
		{
			m_gridDanBaoQuan.SetItemTextEx(nRowCount, m_gridDanBaoQuan.GetColNum(ColId_stockName), iter->second->stock_name);
			m_gridDanBaoQuan.SetItemTextEx(nRowCount, m_gridDanBaoQuan.GetColNum(COLID_stockCode), iter->second->stock_code);

			CString strAssureRatio;
			strAssureRatio.Format(_T("%0.2f"), iter->second->assure_ratio);
			m_gridDanBaoQuan.SetItemTextEx(nRowCount, m_gridDanBaoQuan.GetColNum(ColId_AssureRatio), strAssureRatio);

			m_gridDanBaoQuan.SetItemTextEx(nRowCount, m_gridDanBaoQuan.GetColNum(ColId_AssureStatus), GetAssureStatus(iter->second->assure_status));


			nRowCount++;
		}
		m_gridDanBaoQuan.Invalidate();
	}
}

CString CDanBaoQuanView::GetAssureStatus(char fin_status)
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

void CDanBaoQuanView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	m_gridDanBaoQuan.SetCols(pTagCols, nColCnt);
	m_gridDanBaoQuan.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDanBaoQuan.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridDanBaoQuan.SetGridLines(GVL_NONE);
	m_gridDanBaoQuan.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridDanBaoQuan.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDanBaoQuan.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridDanBaoQuan.SetRowCount(1);
	//m_gridOrder.SetEditable(TRUE);
}

void CDanBaoQuanView::OnSize(UINT nType, int cx, int cy)
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

	if (IsWindow(m_gridDanBaoQuan.GetSafeHwnd()))
	{
		m_gridDanBaoQuan.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}

}

BOOL CDanBaoQuanView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 518, 630);
	return TRUE;
}

BOOL CDanBaoQuanView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CDanBaoQuanView::OnEditResturn(NMHDR *pNmhdr, LRESULT *pResult)
{
	CString strCode;
	m_editStockCode.GetWindowText(strCode);
	int nRowCount = m_gridDanBaoQuan.GetRowCount();
	int nColCount = m_gridDanBaoQuan.GetColumnCount();
	for (int nRow = 1; nRow < nRowCount; nRow++)
	{
		CString strCodeGrid = m_gridDanBaoQuan.GetItemText(nRow, m_gridDanBaoQuan.GetColNum(COLID_stockCode));
		if (strCodeGrid.Trim() == strCode.Trim())
		{
			m_gridDanBaoQuan.MySetSelectedRangeEx(nRow, 0, nRow, nColCount - 1, TRUE);
			m_gridDanBaoQuan.Invalidate(FALSE);
			m_editStockCode.SetWindowText("");
			break;
		}
	}
}

















