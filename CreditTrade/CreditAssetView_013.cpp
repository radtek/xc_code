#include "stdafx.h"
#include "CreditAssetView_013.h"
#include "../tool/baseView/GridRes.h"
#include "ViewManager_credit.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"


// CCreditAssetView

IMPLEMENT_DYNAMIC(CCreditAssetView, CBaseView)

CCreditAssetView::CCreditAssetView(CDockListener &listener)
	: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CCreditAssetView::~CCreditAssetView()
{
}

void CCreditAssetView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CCreditAssetView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CCreditAssetView 消息处理程序

void CCreditAssetView::ReleaseView()
{
	delete this;
}

int CCreditAssetView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	return 0;
}

void CCreditAssetView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("信用资产");
}

void CCreditAssetView::CreateControls()
{
	m_gridCreditAsset.Create(CRect(0, 0, 0, 0), this, IDC_Grid_BiaoDiQuan);
	InitGrid();
	SetData();
}

void CCreditAssetView::SetData()
{
	if (nullptr == G.SPtr_CData)
	{
		return;
	}
	VecCreditAsset VCreditAsset;
	if (G.SPtr_CData->GetVCreditAsset(G.SPtr_CData->fund_account, VCreditAsset) == false)
	{
		ErrorLog("GetVCreditAsset fail");
		return ;
	}
	
	int nRowCount = VCreditAsset.size();
	m_gridCreditAsset.SetRowCount(nRowCount + 1);
	for (int nIndex = 0; nIndex < nRowCount; nIndex++)
	{
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 0, VCreditAsset[nIndex][0].c_str());

		CString str;
		if (MaintenanceGuaranteeRatio == VCreditAsset[nIndex][0])
		{
			str.Format("%0.4lf", atof(VCreditAsset[nIndex][1].c_str()));
		}
		else
		{
			if (nIndex < 10)
			{
				str.Format("%0.2lf", atof(VCreditAsset[nIndex][1].c_str()));
			}
			else
			{
				str = "";
			}
		}
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 1, str);
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 2, VCreditAsset[nIndex][2].c_str());

		str.Format("%0.2lf", atof(VCreditAsset[nIndex][3].c_str()));
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 3, str);
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 4, VCreditAsset[nIndex][4].c_str());

		str.Format("%0.2lf", atof(VCreditAsset[nIndex][5].c_str()));
		m_gridCreditAsset.SetItemTextEx(nIndex + 1, 5, str);
	}
	m_gridCreditAsset.Invalidate();
}

CString CCreditAssetView::GetAssureStatus(char fin_status)
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

void CCreditAssetView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	m_gridCreditAsset.SetCols(pTagCols, nColCnt);
	m_gridCreditAsset.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCreditAsset.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridCreditAsset.SetGridLines(GVL_NONE);
	m_gridCreditAsset.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridCreditAsset.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCreditAsset.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridCreditAsset.SetRowCount(2);
	//m_gridOrder.SetEditable(TRUE);
}

void CCreditAssetView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(m_gridCreditAsset.GetSafeHwnd()))
	{
		m_gridCreditAsset.MoveWindow(0, 0, cx, cy, TRUE);
	}

}

BOOL CCreditAssetView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 953, 531);
	return TRUE;
}

BOOL CCreditAssetView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CCreditAssetView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseView::OnShowWindow(bShow, nStatus);
	QueryCreditAsset();
}

void CCreditAssetView::QueryCreditAsset()
{
	string s_335504;
	tool::GainQueryCreditAsset335504(s_335504, G.SPtr_CData);
	if (tool::SendOrder(G.pCApiFun, Api_Asy, s_335504, G.SPtr_CData) < 0)
	{
		return;
	}
}


















