
// JGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JG.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "DialogSetting.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/TradeDlg.h"

typedef void(*p_SetLimiteFiles)(char* limitstr);

typedef void(*p_GetLimiteFiles)(char* outstr);

typedef bool(*p_GetBiaozhiwei)();

typedef void(*p_FreeLib)();

p_GetLimiteFiles API_GetFIle = NULL;
p_FreeLib API_FreeLib = NULL;

const int CheckUpdateCommand = 1;
const int GoUpdateCommand = 2;

const int NoUpdata = 0;
const int GoUpdata = 1;
const int ForceUpdata = 2;
const int ErrorConnectServer = 10;
const int ErrorExit = 11;

CString GetCurVersion()
{
	CString strVersion;
	try
	{
		ptree pt;
		read_xml("..\\share\\config.xml", pt);

		strVersion = pt.get<string>("content.Version").c_str();
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return "";
	}
	return strVersion;
}


CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Version, m_strVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL SortModuleData(const ModuleNameInfo data1, const ModuleNameInfo data2)
{
	return data1.nIndex < data2.nIndex;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strVersion;
	strVersion.Format("湘财证券，版本：%s", GetRealVersion(GetCurVersion()));
	m_strVersion.SetWindowText(strVersion);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

CString CAboutDlg::GetRealVersion(CString str)
{
	CString strVersion;
	int nLenth = str.GetLength();
	CString str1, str2, str3;
	if (5 == nLenth)
	{
		str1 = str.Mid(0, 1);
		str2 = str.Mid(1, 2);
		str3 = str.Mid(3, 2);
		int n2 = atoi(str2.GetBuffer(0));
		int n3 = atoi(str3.GetBuffer(0));
		strVersion.Format("%s.%d.%d", str1, n2, n3);
	}
	else if (6 == nLenth)
	{
		str1 = str.Mid(0, 2);
		str2 = str.Mid(2, 2);
		str3 = str.Mid(4, 2);
		int n1 = atoi(str1.GetBuffer(0));
		int n2 = atoi(str2.GetBuffer(0));
		int n3 = atoi(str3.GetBuffer(0));
		strVersion.Format("%d.%d.%d", n1, n2, n3);
	}

	return strVersion;
}


// CLoginDlg 对话框
CLoginDlg::CLoginDlg(MapModuleInfo* pMapModuleDLLInfo, CModuleDLLInfo* pCurModuleDLLInfo, CSharedByALLCData *pCSharedByALLCDataTmp, bool bInitial,  CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Login, pParent),
	pModuleDLLInfo(pCurModuleDLLInfo),
	m_pMapModuleDLLInfo(pMapModuleDLLInfo),
	pCSharedByALLCData(pCSharedByALLCDataTmp),
	//m_pVecModuleName(pVecName),
	m_bInitial(bInitial),
	s_ConfigPath("..\\share\\config.xml"),
	m_bFirstPaint(true)
{
	RetryFrequency = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#ifdef _DEBUG
	s_pass = "111111";
#endif
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_User, user_CComboBox);
	DDX_Text(pDX, IDC_Pass, s_pass);
	DDX_Control(pDX, IDC_LoginType, LoginType_CComboBox);
	DDX_Control(pDX, IDC_AssetProp, AssetProp_CComboBox);
	DDX_Control(pDX, IDC_Trade, T2_CComboBox);
	DDX_Control(pDX, IDC_Market_SSE, SSEMarket_CComboBox);
	DDX_Control(pDX, IDC_Market_SZSE, SZSEMarket_CComboBox);
	DDX_Control(pDX, IDC_PROGRESS_Login, obj_CProgressCtrl);
	DDX_Control(pDX, IDC_Font, Font_CComboBox);
	DDX_Control(pDX, IDC_Font_Style, m_comboFontStyle);
	DDX_Control(pDX, IDC_AlertServer, Alert_CComboBox);
	DDX_Control(pDX, IDC_Pass, m_editPsw);
	DDX_Control(pDX, IDC_EDIT_Vcode, m_editVCode);
	DDX_Control(pDX, IDC_AlertServer_SSE, m_comboAlertServer_SSE);
	DDX_Control(pDX, IDC_AlertServer_SZSE, m_comboAlertServer_SZSE);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Login, &CLoginDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_Cancel, &CLoginDlg::OnBnClickedCancel)
	/*键盘消息处理*/
	ON_MESSAGE(WM_VK_RETURN, &CLoginDlg::OnVK_RETURN)
	ON_BN_CLICKED(IDC_Help, &CLoginDlg::OnBnClickedHelp)
	ON_CBN_SELENDOK(IDC_Font_Style, &CLoginDlg::OnSelendokFont)
	ON_BN_CLICKED(IDC_Btn_Setting, &CLoginDlg::OnBnClickedBtnSetting)
	//ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Button_Setting, IDC_Btn_FreshVCode, &CLoginDlg::OnButton)
	ON_EN_SETFOCUS(IDC_Pass, &CLoginDlg::OnEnSetfocusEditPsw)
	ON_MESSAGE(WM_Click_Keypad, &CLoginDlg::ClickKeypad)
	ON_MESSAGE(WM_Click_Delete, &CLoginDlg::ClickDelete)
	ON_MESSAGE(WM_Click_Clear, &CLoginDlg::ClickClear)
	ON_BN_CLICKED(IDC_BUTTON_CheckVCode, &CLoginDlg::OnBnClickedButtonCheckvcode)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// LRESULT CLoginDlg::OnNcHitTest(CPoint point)
// {
// 	return HTCAPTION;
// }

void CLoginDlg::OnEnSetfocusEditPsw()
{
// 	CRect rect;
// 	GetDlgItem(IDC_Pass)->GetWindowRect(&rect);
// 	CKeypad keyPad(&rect, this);
// 	keyPad.DoModal();
}

bool CLoginDlg::Base64Encode(string * outPut, const string & inPut)
{
	typedef base64_from_binary<transform_width<string::const_iterator, 6, 8>> Base64EncodeIter;

	stringstream  result;
	copy(Base64EncodeIter(inPut.begin()),
		Base64EncodeIter(inPut.end()),
		ostream_iterator<char>(result));

	size_t Num = (3 - inPut.length() % 3) % 3;
	for (size_t i = 0; i < Num; i++)
	{
		result.put('=');
	}
	*outPut = result.str();
	return outPut->empty() == false;
}

bool CLoginDlg::Base64Decode(string * outPut, const string & inPut)
{
	typedef transform_width<binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIter;

	stringstream result;
	try
	{
		    copy(Base64DecodeIter(inPut.begin()),
			Base64DecodeIter(inPut.end()),
			ostream_iterator<char>(result));
	}
	catch (...)
	{
		return false;
	}
	*outPut = result.str();
	return outPut->empty() == false;
}

// CLoginDlg 消息处理程序
bool CLoginDlg::initLoginInfo()
{
	LoginType_CComboBox.SetCurSel(LoginType_CComboBox.AddString(_T("资金账号")));
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt);
		int count = 0;
		/*初始化用户名*/
		BOOST_AUTO(child_User, pt.get_child("content.Accounts"));
		for (BOOST_AUTO(pos, child_User.begin()); pos != child_User.end(); ++pos)
		{
			string s_tmp;
			string s_Encode;
			s_Encode = pos->second.get<string>("Value");
			Base64Decode(&s_tmp, s_Encode);
			int pos_begin = s_tmp.find("#@");
			s_tmp = s_tmp.substr(0, pos_begin);
			vec_User.push_back(s_tmp);

			if (0 == count)
			{
				user_CComboBox.SetCurSel(user_CComboBox.AddString(s_tmp.c_str()));
			}
			else
			{
				user_CComboBox.AddString(s_tmp.c_str());
			}
			count++;
		}
		/*asset_prop*/
		if (m_bInitial)
		{
			string strExePath;
			tool::initExePath(strExePath);
			int nIndex = 0;
			for (auto& data : m_vecModuleNameInfo)
			{
				string strDllPath = strExePath + "\\" + data.strDllName;
				if (PathFileExists(strDllPath.c_str()))
				{
					CString str;
					str.Format("%s", data.strModuleName.c_str());
					AssetProp_CComboBox.InsertString(nIndex++, str);
				}
			}
			if (AssetProp_CComboBox.GetCount() > 0)
			{
				AssetProp_CComboBox.SetCurSel(0);
			}
		} 
		else
		{
			AssetProp_CComboBox.SetCurSel(AssetProp_CComboBox.AddString(pModuleDLLInfo->strDes.c_str()));
			AssetProp_CComboBox.EnableWindow(FALSE);
		}
		
		/*把T2*/
		ReadData(vec_TradeAddress, pt, "content.Trade", "content.TradeLastLogin", T2_CComboBox);
		ReadData(vec_SSEMarketAddress, pt, "content.SSEMarket", "content.SSEMarketLastLogin", SSEMarket_CComboBox);
		ReadData(vec_SZSEMarketAddress, pt, "content.SZSEMarket", "content.SZSEMarketLastLogin", SZSEMarket_CComboBox);
//		ReadData(vec_AlertAddress, pt, "content.Alert", "content.AlertLastLogin", Alert_CComboBox);
		ReadData(vec_AlertAddress_SSE, pt, "content.AlertSSE", "content.AlertSSELastLogin", m_comboAlertServer_SSE);
		ReadData(vec_AlertAddress_SZSE, pt, "content.AlertSZSE", "content.AlertSZSELastLogin", m_comboAlertServer_SZSE);

		CString s_Font = pt.get<string>("content.Font").c_str();
		Font_CComboBox.SetCurSel(Font_CComboBox.AddString("微软雅黑"));
		m_comboFontStyle.InsertString(0, FontStandard);
		m_comboFontStyle.InsertString(1, FontMedium);
		m_comboFontStyle.InsertString(2, FontBig);

		//m_comboFontStyle.AddString(FontStandard);
		//m_comboFontStyle.AddString(FontMedium);
		//m_comboFontStyle.AddString(FontBig);
		CString s_FontSize = pt.get<string>("content.FontSize").c_str();
		int nCount = m_comboFontStyle.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			CString str;
			m_comboFontStyle.GetLBText(nIndex, str);
			if (str.Trim() == s_FontSize)
			{
				m_comboFontStyle.SetCurSel(nIndex);
				break;
			}
		}
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}

	return true;
}

void CLoginDlg::ReadData(vector<string>& vec_Address, ptree& pt, const string& item, const string& itemCur, CComboBox& comboBox)
{
	vec_Address.clear();
	int nIndex = 0;
	BOOST_AUTO(child_Trade, pt.get_child(item));
	for (BOOST_AUTO(pos, child_Trade.begin()); pos != child_Trade.end(); ++pos)
	{
		string s_tmp = pos->second.get<string>("server");
		vec_Address.push_back(s_tmp);
		comboBox.InsertString(nIndex++, s_tmp.c_str());
	}
	string curAddr = pt.get<string>(itemCur);
	CString strCurAddr;
	strCurAddr.Format("%s", curAddr.c_str());
	int nIndexEx = 0;
	bool bFind = false;
	for (auto data : vec_Address)
	{
		CString str;
		str.Format("%s", data.c_str());
		if (str.Find(strCurAddr) >= 0)
		{
			bFind = true;
			break;
		}
		nIndexEx++;
	}
	if (!bFind)
	{
		nIndexEx = 0;
	}
	if (comboBox.GetCount() > nIndexEx)
	{
		comboBox.SetCurSel(nIndexEx);
	}
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	/*初始化登陆控件*/
	InitAllModuleName(m_vecModuleNameInfo);
	if (initLoginInfo() == false)
	{
		ExceptionInterrupt;
	}

	UpdateData(FALSE);

	Layout();
	GetDlgItem(IDC_User)->SetFocus();
	CreateVCode();
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoginDlg::Layout()
{
	CRect rectClient;
	GetClientRect(&rectClient);
	if (::IsWindow(m_staCaption.GetSafeHwnd()))
	{
		CTradeDlg trade;
		CRect rectCal;
		trade.m_hMainViewWnd = GetSafeHwnd();

		trade.CalculateTextRect("锋 速 自 研 系 统 U F O", GetFont(), rectCal);

		CRect rectLayout;
		rectLayout.left = (rectClient.Width() - rectCal.Width()) / 2;
		rectLayout.top = 50;
		rectLayout.right = rectLayout.left + rectCal.Width();
		rectLayout.bottom = rectLayout.top + rectCal.Height();

		m_staCaption.MoveWindow(&rectLayout);
	}
	CRect rectSetting, rectLogin, rectCancel, rectVCode;
	GetWndRect(GetDlgItem(IDC_Btn_Setting), rectSetting);
	GetWndRect(GetDlgItem(IDC_Login), rectLogin);
	GetWndRect(GetDlgItem(ID_Cancel), rectCancel);
	GetWndRect(GetDlgItem(IDC_BUTTON_CheckVCode), rectVCode);
	if (::IsWindow(CSimpleButton_TradeSetting.GetSafeHwnd()))
	{
		CSimpleButton_TradeSetting.MoveWindow(rectSetting.left, rectSetting.top, rectSetting.Width(), rectSetting.Height() + 10);
		m_btnLogin.MoveWindow(rectLogin.left, rectLogin.top, rectLogin.Width(), rectLogin.Height() + 10);
		m_btnCancel.MoveWindow(rectCancel.left, rectCancel.top, rectCancel.Width(), rectCancel.Height() + 10);
		//m_btnFreshVCode.MoveWindow(&rectVCode);
	}

}

void CLoginDlg::GetWndRect(CWnd* pWnd, CRect& rect)
{
	if (pWnd != nullptr && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
	}	
}

void CLoginDlg::EnableLoginWindow(BOOL bEnable)
{
	m_staCaption.EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_LoinType)->EnableWindow(bEnable);
	LoginType_CComboBox.EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_User)->EnableWindow(bEnable);
	user_CComboBox.EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_Pass)->EnableWindow(bEnable);
	GetDlgItem(IDC_Pass)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_Font)->EnableWindow(bEnable);
	Font_CComboBox.EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_FontStyle)->EnableWindow(bEnable);
	m_comboFontStyle.EnableWindow(bEnable);
	if (m_bInitial)
	{
		AssetProp_CComboBox.EnableWindow(bEnable);
	}
	CSimpleButton_TradeSetting.EnableWindow(bEnable);
	m_btnLogin.EnableWindow(bEnable);
	m_btnCancel.EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_VCode)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_Vcode)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_CheckVCode)->EnableWindow(bEnable);
	//m_btnFreshVCode.EnableWindow(bEnable);

}

void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		if (m_bFirstPaint)
		{
			CreateVCode();
			if (IsVCode4TheSame())
			{
				CreateVCode();
			}
		}
		else
		{
			PaintPriviousVCode(m_VCodeInfo);
		}
		m_bFirstPaint = false;
		CDialogEx::OnPaint();
	}
}

bool CLoginDlg::IsVCode4TheSame()
{
	bool b4TheSame = false;
	int nLenth = m_strVerfCode.GetLength();
	if (4 == nLenth)
	{
		if (m_strVerfCode[0] == m_strVerfCode[1] &&
			m_strVerfCode[0] == m_strVerfCode[2] &&
			m_strVerfCode[0] == m_strVerfCode[3]
			)
		{
			b4TheSame = true;
		}
	}
	return b4TheSame;
}

bool CLoginDlg::CheckCurrModule(CString strModules, const CString& strCurModuleName)
{
	bool bExsit = false;
	CString strDllName;
	for (auto& data : m_vecModuleNameInfo)
	{
		CString str;
		str.Format("%s", data.strModuleName.c_str());
		if (str.Trim() == strCurModuleName)
		{
			strDllName.Format("%s", data.strDllName.c_str());
			break;
		}
	}
	if (strModules.Find(strDllName) >= 0)
	{
		bExsit = true;
	}
	else
	{
		bExsit = false;
	}
	return bExsit;
}

bool CLoginDlg::IsPublicVersion(CString strModuleDllNames)
{
	int nIndex = strModuleDllNames.Find(",");
	if (nIndex > 0)
	{
		int nLength = strModuleDllNames.GetLength();
		CString strLeft = strModuleDllNames.Mid(nIndex + 1, nLength - nIndex - 1);
		if (strLeft.Find(",") > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

void CLoginDlg::SetNewNoticeNO(const CString& strNewNoticeNO)
{
	try
	{
		ptree pt;
		string s_ConfigPath("..\\share\\config.xml");
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);

		pt.put("content.NewNoticeNO", strNewNoticeNO);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CLoginDlg::SetNotice(const CString& strNotice)
{
	try
	{
		ptree pt;
		read_xml(ConfigPath, pt, xml_parser::trim_whitespace);

		pt.put("content.Notice", strNotice);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CLoginDlg::ResetModuleDataFromXml(CString& strModules)
{
	strModules.Empty();
	int nIndex = 0;
	for (auto data : m_vecModuleNameInfo)
	{
		CString str;
		str.Format("%s,%s,%d@", data.strDllName.c_str(), data.strModuleName.c_str(), nIndex++);
		strModules += str;
	}
}

void CLoginDlg::SetNoticeInfoVec(const CString& strModules)
{
	CString str = strModules;

	int nIndex = 0;
	while ((nIndex = str.Find("$")) >= 0)
	{
		NoticeInfo info;
		info.strNotice = str.Mid(0, nIndex);
		str.Delete(0, nIndex + 1);

		nIndex = str.Find("$");
		if (nIndex >= 0)
		{
			info.strNoticeNo = str.Mid(0, nIndex);
			m_vecNoticeInfo.push_back(info);
			str.Delete(0, nIndex + 1);
		}
	}
}

// bool CLoginDlg::HandleUpdate()
// {
// 	EnableLoginWindow(FALSE);
// 	obj_CProgressCtrl.ShowWindow(SW_SHOW);
// 	obj_CProgressCtrl.SetPos(0);
// 
// 	CString strModules;
// 	DWORD result = GetUpdate(CheckUpdateCommand, strModules);
// 	SetNoticeInfoVec(strModules);
// 	
// 	CString strNoticeNO = GetNoticeNO();
// 	if (!strNoticeNO.IsEmpty())
// 	{	
// 		SetNewNoticeNO(strNoticeNO);
// 	}
// 
// 	CString strNotice = GetNotice();
// 	if (!strNotice.IsEmpty())
// 	{
// 		SetNotice(strNotice);
// 	}
// 
// 	if (ErrorConnectServer == result)
// 	{
// 		CString strMsg;
// 		strMsg.Format("%s\r\n是否继续登录？", strModules);
// 		int nResult = MessageBox(strMsg, "升级提醒", MB_ICONEXCLAMATION | MB_YESNO);
// 		
// 		if (IDNO == nResult)
// 		{
// 			return false;
// 		}
// 		ResetModuleDataFromXml(strModules);
// 	}
// 	else if (ErrorExit == result)
// 	{
// //#ifdef DEBUG
// 		CString strMsg;
// 		strMsg.Format("%s\r\n是否继续登录？", strModules);
// 		int nResult = MessageBox(strMsg, "提醒", MB_ICONEXCLAMATION | MB_YESNO);
// 
// 		if (IDNO == nResult)
// 		{
// 			return false;
// 		}
// 		ResetModuleDataFromXml(strModules);
// //#else
// //		MessageBox(strModules, "提醒", MB_ICONEXCLAMATION | MB_OK);
// //		return false;
// //#endif
// 	}
// 
// 	else if (GoUpdata == result)
// 	{
// 		int nResult = MessageBox("程序检测有新版本可以更新，是否更新？", "升级提醒", MB_ICONEXCLAMATION | MB_YESNO);
// 		if (IDYES == nResult)
// 		{
// 			ResetModuleData(strModules);
// 			ResetXmlModuleData();
// 			GetUpdate(GoUpdateCommand, strModules);
// 		}
// 	}
// 	else if (ForceUpdata == result)
// 	{
// 		ResetModuleData(strModules);
// 		ResetXmlModuleData();
// 		GetUpdate(GoUpdateCommand, strModules);
// 	}
// 
// 	SetConnectUpdateServerState(ErrorConnectServer != result);
// 	ResetModuleData(strModules);
// 	ResetXmlModuleData();
// 	InitModuleDLLInfo(m_pMapModuleDLLInfo, m_vecModuleNameInfo, pCSharedByALLCData);
// 	return true;
// }
bool CLoginDlg::HandleUpdate()
{
	EnableLoginWindow(FALSE);
	obj_CProgressCtrl.ShowWindow(SW_SHOW);
	obj_CProgressCtrl.SetPos(0);

	CString strModules = "CreditTrade.dll,融资融券,2@FastTrade.dll,快速交易,1@";

	SetConnectUpdateServerState(FALSE);

	ResetModuleData(strModules);
	ResetXmlModuleData();
	InitModuleDLLInfo(m_pMapModuleDLLInfo, m_vecModuleNameInfo, pCSharedByALLCData);
	return true;
}


CString CLoginDlg::GetNotice()
{
	CString strNotice;
	for (auto& data : m_vecNoticeInfo)
	{
		CString str;
		str.Format("公告%s$$$", data.strNoticeNo);
		str += (data.strNotice + "@@@");
		strNotice += str;
	}
	return strNotice;
}

CString CLoginDlg::GetNoticeNO()
{
	CString str;
	auto nSize = m_vecNoticeInfo.size();
	if (nSize > 0)
	{
		str = m_vecNoticeInfo[nSize - 1].strNoticeNo;
	}
	return str;
}

void CLoginDlg::ResetXmlModuleData()
{
	try
	{
		ptree pt;
		string s_ConfigPath("..\\share\\config.xml");
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);

		ptree& pp = pt.get_child("content.ModuleDLLInfo");
		pp.clear();

		int nIndex = 0;
		for (auto data : m_vecModuleNameInfo)
		{
			string subItem = "Module" + convert<string>(nIndex++ + 1);

			ptree treeSubItem;
			treeSubItem.put(subItem + ".DllName", data.strDllName);
			treeSubItem.put(subItem + ".Des", data.strModuleName);

			string item = "content.ModuleDLLInfo." + subItem;
			pt.put_child(item, treeSubItem.get_child(subItem));
		}

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CLoginDlg::ResetModuleData(CString strModules)
{
	//strModules = "前端资源$1002$AutoTrade.dll,自动交易,3@CreditTrade.dll,融资融券,2@FastTrade.dll,快速交易,1@";
// 	CString strModulesEx;
// 	int nIndex1 = strModules.Find("$");
// 	if (nIndex1 >= 0)
// 	{
// 		int nIndex2 = strModules.Find("$", nIndex1 + 1);
// 		if (nIndex2 >= 0)
// 		{
// 			strModulesEx = strModules.Mid(nIndex2 + 1, strModules.GetLength() - nIndex2 - 1);
// 		}
// 	}
// 	else
// 	{
// 		strModulesEx = strModules;
// 	}

	int nIndex1 = 0;
	while ((nIndex1 = strModules.Find("$")) >= 0)
	{
		strModules.Delete(0, nIndex1 + 1);
	}


	m_vecModuleNameInfo.clear();
	int nIndex = 0;
	while ((nIndex = strModules.Find("@")) >= 0)
	{
		CString str = strModules.Mid(0, nIndex);
		strModules.Delete(0, nIndex + 1);

		ModuleNameInfo info;
		int nIndex1 = str.Find(",");
		info.strDllName = str.Mid(0, nIndex1);

		str.Delete(0, nIndex1 + 1);

		nIndex1 = str.Find(",");
		info.strModuleName = str.Mid(0, nIndex1);

		str.Delete(0, nIndex1 + 1);
		info.nIndex = atoi(str.GetBuffer(0));
		m_vecModuleNameInfo.push_back(info);

	}

	sort(m_vecModuleNameInfo.begin(), m_vecModuleNameInfo.end(), SortModuleData);
}

void CLoginDlg::SetConnectUpdateServerState(bool bConnected)
{
	try
	{
		ptree pt;
		string s_ConfigPath("..\\share\\config.xml");
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);

		pt.put("content.ConnectUpdateServerState", bConnected ? 1 : 0);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

// CString CLoginDlg::GetCurVersion()
// {
// 	CString strVersion;
// 	try 
// 	{
// 		ptree pt;
// 		read_xml(s_ConfigPath, pt);
// 
// 		strVersion = pt.get<string>("content.Version").c_str();
// 	}
// 	catch (std::exception& e)
// 	{
// 		LOG(ERROR) << "Error:" << e.what();
// 		ExceptionInterrupt;
// 		return "";
// 	}
// 	return strVersion;
// }

CString CLoginDlg::GetCurNoticeNO()
{
	CString strVersion;
	try
	{
		ptree pt;
		read_xml("..\\share\\config.xml", pt);

		strVersion = pt.get<string>("content.CurNoticeNO").c_str();
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return "";
	}
	return strVersion;
}


DWORD CLoginDlg::GetUpdate(int nType, CString& strModule)
{
	char cExeName[_MAX_PATH] = { 0 };
	HANDLE hInstance = AfxGetInstanceHandle();
	::GetModuleFileName(HMODULE(hInstance), cExeName, _MAX_PATH);
	
	DWORD Result;
	SHELLEXECUTEINFO ShExeInfo = { 0 };
	ShExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExeInfo.hwnd = NULL;
	ShExeInfo.lpVerb = NULL;
	ShExeInfo.lpFile = "XcTradeUpdate.exe";

	CString strUser, strPsw;
	GetDlgItem(IDC_User)->GetWindowText(strUser);
	GetDlgItem(IDC_Pass)->GetWindowText(strPsw);

	CString str;
	str.Format("$%d,,%s,%s,%s,%s", nType, strUser, strPsw, GetCurVersion(), GetCurNoticeNO());
	ShExeInfo.lpParameters = str;
    ShExeInfo.lpDirectory = NULL;
	ShExeInfo.nShow = SW_SHOWNORMAL;
	ShExeInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExeInfo);

	if (CheckUpdateCommand == nType)
	{
		HINSTANCE hDll = LoadLibrary("shared_memory.dll");
		if (NULL == hDll)
		{
			FreeLibrary(hDll);
			return 0;
		}
		API_GetFIle = (p_GetLimiteFiles)GetProcAddress(hDll, "GetLimiteFiles");
		API_FreeLib = (p_FreeLib)GetProcAddress(hDll, "FreeLib");

		if (API_GetFIle == NULL || API_FreeLib == NULL)
		{
			FreeLibrary(hDll);
			return 0;
		}
		char fileName[4096] = {0};
		while (true)
		{
			API_GetFIle(fileName);
			strModule.Format("%s", fileName);
			if (!strModule.IsEmpty())
			{
				API_FreeLib();
				break;
			}
			Sleep(10);
		}

		WaitForSingleObject(ShExeInfo.hProcess, 3000);
		GetExitCodeProcess(ShExeInfo.hProcess, &Result);
	}
	else
	{
		exit(0);
	}

	return Result;
}

/*相应默认焦点按钮的enter*/
void CLoginDlg::OnBnClickedOk()
{
	if (!CheckInfo())
	{
		return;
	}
	if (m_bInitial)
	{
		if (!HandleUpdate())
		{
			EnableLoginWindow(TRUE);
			obj_CProgressCtrl.ShowWindow(SW_HIDE);
			return;
		}/*fixing 升级服务器登陆失败也改可以登陆的*/
	}
	/*在登陆界面enter键的输入默认进行登陆*/

	RetryFrequency++;
	if (HandleLogin())
	{
		/*登陆成功后，首先把登陆成功的信息写入配置文件中*/
		ResetXml(true);
		CDialogEx::OnOK();
		return;
	}
	else
	{
		EnableLoginWindow(TRUE);
		obj_CProgressCtrl.ShowWindow(SW_HIDE);
		obj_CProgressCtrl.SetPos(0);
	}
	if (RetryFrequency >= MaXRetry)
	{
		MessageBoxA(_T("登录信息输入错误超限，登录界面将自动退出"), _T("登录异常"), MB_OK);
		LOG(ERROR) << "登录信息输入错误超限，登录界面将自动退出";
		CDialogEx::OnOK();
		return;
	}
}

bool CLoginDlg::CheckInfo()
{
	user_CComboBox;
	m_editPsw;
	m_editVCode;

	CString strUser;
	user_CComboBox.GetWindowText(strUser);
	if (strUser.Trim().IsEmpty())
	{
		MessageBox("用户名不能为空，请修改！", "提醒", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	CString strPsw;
	m_editPsw.GetWindowText(strPsw);
	if (strPsw.Trim().IsEmpty())
	{
		MessageBox("密码不能为空，请修改！", "提醒", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
#ifndef DEBUG
	CString strVCode;
	m_editVCode.GetWindowText(strVCode);
	if (strVCode.Trim().IsEmpty())
	{
		MessageBox("验证码不能为空，请修改！", "提醒", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	if (strVCode.Trim().MakeUpper() != m_strVerfCode.MakeUpper())
	{
		MessageBox("验证码不正确，请修改！", "提醒", MB_ICONEXCLAMATION | MB_OK);
		//CreateVCode();
		return false;
	}
#endif

	return true;
}

bool CLoginDlg::HandleLogin()
{
	UpdateData(TRUE);//为登陆类取数据做准备
	bool bRet = false;
	if (pModuleDLLInfo != nullptr)
	{
		EnableLoginWindow(FALSE);
		obj_CProgressCtrl.ShowWindow(SW_SHOW);
		bRet = pModuleDLLInfo->pLogin(obj_CProgressCtrl, pModuleDLLInfo->SPtr_CData, &pModuleDLLInfo->apiFun, GetSafeHwnd());
		if (bRet)
		{
			LoginedDllName = pModuleDLLInfo->strDes;
			pModuleDLLInfo->bLogined = true;
		}
		else
		{
			while (1)
			{
				if (true == pModuleDLLInfo->SPtr_CData->bRuning_ErrorInfo)
				{
					Sleep(100);
				}
				else
				{
					break;
				}
			}
			pModuleDLLInfo->SPtr_CData.reset();
			pModuleDLLInfo->SPtr_CData = make_shared<CData>();
			pModuleDLLInfo->SPtr_CData->bRun_MainPageDlg = pCSharedByALLCData->bRun_MainPageDlg;
			pModuleDLLInfo->SPtr_CData->hwnd_MainPageDlg = pCSharedByALLCData->hwnd_MainPageDlg;
			pModuleDLLInfo->SPtr_CData->TestSpeed = pCSharedByALLCData->TestSpeed;
			pModuleDLLInfo->SPtr_CData->bFullLog = pCSharedByALLCData->bFullLog;
		}
	}
	return bRet;
}

bool CLoginDlg::SaveLoginedInfo()
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		SaveUser(pt);
	
		SavaLoginedTradeSever(pt);

		SavaSSEMarketAddress(pt);

		SavaSZSEMarketAddress(pt);

		SavaAlertAddress(pt);

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}

	return true;
}

void CLoginDlg::SaveUser(ptree &pt)
{
	CString s_tmp;
	string s_str;
	string s_encode;
	GetDlgItem(IDC_User)->GetWindowText(s_tmp);
	
	/*人为的添加"#@"作为密文的结束符*/
	s_str = s_tmp;

	auto iter = find(vec_User.begin(), vec_User.end(), s_str);
	/*由于可修改，所以存在找不到的可能*/
	if (iter != vec_User.end())
	{
		vec_User.erase(iter);
	}

	s_str += "#@";
	Base64Encode(&s_encode, s_str);

	ptree AccountId1;
	AccountId1.put("Account1.Value", s_encode);
	pt.put_child("content.Accounts.Account1", AccountId1.get_child("Account1"));

	int count = 1;
	for (auto iter : vec_User)
	{
		count++;
		ptree AccountId;
		string s_key = "Account";
		s_key += convert<string>(count);
		string RelativePath = s_key;
		RelativePath += ".Value";
		string s_encode;
		string s_str = iter += "#@";
		Base64Encode(&s_encode, s_str);
		AccountId.put(RelativePath, s_encode);
		string AbsolutePath = "content.Accounts.";
		AbsolutePath += s_key;
		pt.put_child(AbsolutePath, AccountId.get_child(s_key));
	}
}

void CLoginDlg::SavaLoginedTradeSever(ptree &pt)
{
	CString s_tmp;
	GetDlgItem(IDC_Trade)->GetWindowText(s_tmp);
	string s_str = s_tmp;
	auto iter = find(vec_TradeAddress.begin(), vec_TradeAddress.end(), s_str);
	if (iter != vec_TradeAddress.end())
	{
		vec_TradeAddress.erase(iter);
	}
	else
	{
		LOG(ERROR) << "abnormal TradeServer";
		ExceptionInterrupt;
	}
	

	ptree TradeId1;
	TradeId1.put("Trade1.server", s_str);
	pt.put_child("content.Trades.Trade1", TradeId1.get_child("Trade1"));

	int count = 1;
	for (auto iter: vec_TradeAddress)
	{
		count++;
		ptree TradeId;
		string s_key = "Trade";
		s_key += convert<string>(count);
		string RelativePath = s_key;
		RelativePath += ".server";
		TradeId.put(RelativePath, iter);

		string AbsolutePath = "content.Trades.";
		AbsolutePath += s_key;
		pt.put_child(AbsolutePath, TradeId.get_child(s_key));
	}
	return;

}

void CLoginDlg::SavaSSEMarketAddress(ptree &pt)
{
	CString s_tmp;
	GetDlgItem(IDC_Market_SSE)->GetWindowText(s_tmp);
	string s_str = s_tmp;
	auto iter = find(vec_SSEMarketAddress.begin(), vec_SSEMarketAddress.end(), s_str);
	if (iter != vec_SSEMarketAddress.end())
	{
		vec_SSEMarketAddress.erase(iter);
	}
	else
	{
		LOG(ERROR) << "abnormal MarketServer";
		ExceptionInterrupt;
	}

	ptree MarketId1;
	MarketId1.put("SSEMarket1.server", s_str);
	pt.put_child("content.SSEMarkets.SSEMarket1", MarketId1.get_child("SSEMarket1"));

	int count = 1;
	for (auto iter : vec_SSEMarketAddress)
	{
		count++;
		ptree MarketId;
		string s_key = "SSEMarket";
		s_key += convert<string>(count);
		string RelativePath = s_key;
		RelativePath += ".server";
		MarketId.put(RelativePath, iter);

		string AbsolutePath = "content.SSEMarkets.";
		AbsolutePath += s_key;
		pt.put_child(AbsolutePath, MarketId.get_child(s_key));
	}
	return;
}

void CLoginDlg::SavaSZSEMarketAddress(ptree &pt)
{
	CString s_tmp;
	GetDlgItem(IDC_Market_SZSE)->GetWindowText(s_tmp);
	string s_str = s_tmp;
	auto iter = find(vec_SZSEMarketAddress.begin(), vec_SZSEMarketAddress.end(), s_str);
	if (iter != vec_SZSEMarketAddress.end())
	{
		vec_SZSEMarketAddress.erase(iter);
	}
	else
	{
		LOG(ERROR) << "abnormal MarketServer";
		ExceptionInterrupt;
	}

	ptree MarketId1;
	MarketId1.put("SZSEMarket1.server", s_str);
	pt.put_child("content.SZSEMarkets.SZSEMarket1", MarketId1.get_child("SZSEMarket1"));

	int count = 1;
	for (auto iter : vec_SZSEMarketAddress)
	{
		count++;
		ptree MarketId;
		string s_key = "SZSEMarket";
		s_key += convert<string>(count);
		string RelativePath = s_key;
		RelativePath += ".server";
		MarketId.put(RelativePath, iter);

		string AbsolutePath = "content.SZSEMarkets.";
		AbsolutePath += s_key;
		pt.put_child(AbsolutePath, MarketId.get_child(s_key));
	}
	return;
}

bool CLoginDlg::SaveZoomInfo()
{
	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		CString s_Font;
		CString s_FontSize;
		GetDlgItem(IDC_Font)->GetWindowText(s_Font);
		//GetDlgItem(IDC_FontSize)->GetWindowText(s_FontSize);
		int nCurSel = m_comboFontStyle.GetCurSel();
		m_comboFontStyle.GetLBText(nCurSel, s_FontSize);
		/*if ("" == s_FontSize || convert<unsigned int>(s_FontSize) > 100)
		{
			s_FontSize = "9";
		}*/
		if ("" == s_FontSize)
		{
			s_FontSize = "标准";
		}
		pt.put("content.Font", s_Font);
		pt.put("content.FontSize", s_FontSize);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	return true;
}

/*响应esc*/
void CLoginDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CDialogEx::OnCancel();
	int i = 0;
}

/*处理WM_CLOSE相应x按钮*/
void CLoginDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SaveZoomInfo();
	EndDialog(0);    //关闭窗口 
	CDialogEx::OnClose();
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveZoomInfo();
	EndDialog(0);    //关闭窗口
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		{
			if (::GetKeyState(VK_RETURN) < 0)
			{
				/*在窗口里发任意窗口用这个，如果不是在窗口里，则不需要::*/
				int focusID = GetFocus()->GetDlgCtrlID();
				switch (focusID)
				{
				case IDC_User:
				{
#ifndef DEBUG
					CPoint   point;
					GetCursorPos(&point);

					CRect rect;
					GetDlgItem(IDC_Pass)->GetWindowRect(&rect);
					if (rect.PtInRect(point))
					{
						SetFocus();
						CKeypad keyPad(&rect, this);
						keyPad.DoModal();
						return TRUE;
					}
#else
					GetDlgItem(IDC_Pass)->SetFocus();
#endif			
				}
					
				case IDC_Pass:
					OnBnClickedOk();
					break;
				default:
					OnBnClickedOk();
					break;
				}
			}
			break;
		}
		}

	}

// #ifndef DEBUG
// 	else if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
// 	{
// 		CPoint   point;
// 		GetCursorPos(&point);
// 
// 		CRect rect;
// 		GetDlgItem(IDC_Pass)->GetWindowRect(&rect);
// 		m_keyPad.SetRect(rect);
// 
// 		if (rect.PtInRect(point))
// 		{
// 			SetFocus();
// 			if (!m_keyPad.IsWindowVisible())
// 			{
// 				CRect reCli;
// 				m_keyPad.SetFocus();
// 				m_keyPad.GetWindowRect(&reCli);
// 				m_keyPad.SetWindowPos(&wndTopMost, rect.left, rect.bottom, reCli.Width(), reCli.Height(), SWP_NOSIZE);
// 				m_keyPad.RandomNum();
// 				m_keyPad.ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				m_keyPad.ShowWindow(SW_HIDE);
// 				m_keyPad.RandomNum();
// 			}
// 			
// 			return TRUE;
// 		}
// 	}
// 
// #endif // DEBUG
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*默认对default button的处理是忽略*/
	//CDialogEx::OnOK();
	cout << endl;
}

LRESULT CLoginDlg::OnVK_RETURN(WPARAM wParam, LPARAM lParam)
{
	int focusID = GetFocus()->GetDlgCtrlID();
	switch (focusID)
	{
	case IDC_User:
		GetDlgItem(IDC_Pass)->SetFocus();
		break;
	case IDC_Pass:
		OnBnClickedOk();
		break;
	default:
		break;
	}
	return 0;
}

void CLoginDlg::OnBnClickedHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s_str = "为了您最好的体验，字体默认使用Ms Shell Dlg，推荐字号范围[8,72]，";
	s_str += "请勿使用0 -1之类的非法字号，如果您更改了字体，请务必点击取消并退出整个程序，您的更改将在下次启动时生效";
	MessageBox(s_str,_T("帮助"), MB_OK);
}

void CLoginDlg::OnSelendokFont()
{
	SaveZoomInfo();
}

void CLoginDlg::SavaAlertAddress(ptree &pt)
{
	CString s_tmp;
	GetDlgItem(IDC_AlertServer)->GetWindowText(s_tmp);
	string s_str = s_tmp;
	auto iter = find(vec_AlertAddress.begin(), vec_AlertAddress.end(), s_str);
	if (iter != vec_AlertAddress.end())
	{
		vec_AlertAddress.erase(iter);
	}
	else
	{
		LOG(ERROR) << "abnormal AlertServer";
		ExceptionInterrupt;
	}

	ptree AlertId1;
	AlertId1.put("Alert1.server", s_str);
	pt.put_child("content.Alerts.Alert1", AlertId1.get_child("Alert1"));

	int count = 1;
	for (auto iter : vec_AlertAddress)
	{
		count++;
		ptree AlertId;
		string s_key = "Alert";
		s_key += convert<string>(count);
		string RelativePath = s_key;
		RelativePath += ".server";
		AlertId.put(RelativePath, iter);

		string AbsolutePath = "content.Alerts.";
		AbsolutePath += s_key;
		pt.put_child(AbsolutePath, AlertId.get_child(s_key));
	}
	return;
}

void CLoginDlg::OnBnClickedBtnSetting()
{
	MapStation station;
	GetStationData(station);
	CDialogSetting  setting(&station);
	if (IDOK == setting.DoModal())
	{
		SetStationData(station);
	}
}

void CLoginDlg::SetStationData(const MapStation& station)
{
	SetComboData(StationTrade, T2_CComboBox, station);
	SetComboData(StationSSEMarket, SSEMarket_CComboBox, station);
	SetComboData(StationSZSEMarket, SZSEMarket_CComboBox, station);
	//SetComboData(StationAlert, Alert_CComboBox, station);
	SetComboData(StationAlertSSE, m_comboAlertServer_SSE, station);
	SetComboData(StationAlertSZSE, m_comboAlertServer_SZSE, station);
	ResetXml(false);
}

void CLoginDlg::GetStationData(MapStation& station)
{
	GetComboData(StationTrade, T2_CComboBox, station);
	GetComboData(StationSSEMarket, SSEMarket_CComboBox, station);
	GetComboData(StationSZSEMarket, SZSEMarket_CComboBox, station);
//	GetComboData(StationAlert, Alert_CComboBox, station);
	GetComboData(StationAlertSSE, m_comboAlertServer_SSE, station);
	GetComboData(StationAlertSZSE, m_comboAlertServer_SZSE, station);

}

void CLoginDlg::ResetXml(bool bSaveLastLogin)
{
	try 
	{
		ptree pt;
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		SaveUser(pt);
		SaveItem(T2_CComboBox, pt, "Trade", bSaveLastLogin);
		SaveItem(SSEMarket_CComboBox, pt, "SSEMarket", bSaveLastLogin);
		SaveItem(SZSEMarket_CComboBox, pt, "SZSEMarket", bSaveLastLogin);
	//	SaveItem(Alert_CComboBox, pt, "Alert", bSaveLastLogin);
		SaveItem(m_comboAlertServer_SSE, pt, "AlertSSE", bSaveLastLogin);
		SaveItem(m_comboAlertServer_SZSE, pt, "AlertSZSE", bSaveLastLogin);

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}

}

void CLoginDlg::SaveItem(CComboBox& combo, ptree &pt, string itemName, bool bSaveLastLogin)
{
	if (bSaveLastLogin)
	{
		CString strText;
		combo.GetWindowText(strText);
		CString strCurAddr;
		int nIndex = strText.Find(":");
		if (nIndex > 0)
		{
			strText.Delete(0, nIndex + 1);
		}

		string curAddr = strText.GetBuffer();
		string cur = "content." + itemName + "LastLogin";
		pt.put(cur, curAddr);
	}

	ptree& pp = pt.get_child("content." + itemName);
	pp.clear();

	int nCount = combo.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString strText;
		combo.GetLBText(nIndex, strText);

		ptree treeSubItem;

		string subItem = itemName + convert<string>(nIndex + 1);
		treeSubItem.put(subItem + ".server", strText);

		string item = "content." + itemName + "." + subItem;
		pt.put_child(item, treeSubItem.get_child(subItem));
	}

}

void CLoginDlg::SetComboData(StationName name, CComboBox& combo, const MapStation& station)
{
	combo.ResetContent();
	auto iterTrade = station.find(name);
	if (iterTrade != station.end())
	{
		int nIndex = 0;
		for (auto data : iterTrade->second.vecData)
		{
			combo.InsertString(nIndex++, data);
		}
		if (combo.GetCount() > iterTrade->second.nCurSel)
		{
			combo.SetCurSel(iterTrade->second.nCurSel);
		}
	}
}

void CLoginDlg::GetComboData(StationName name, CComboBox& combo, MapStation& station)
{
	CStationInfo stationInfo;
	stationInfo.nCurSel = combo.GetCurSel();
	int nCount = combo.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString strText;
		combo.GetLBText(nIndex, strText);
		stationInfo.vecData.push_back(strText);
	}
	station.insert(make_pair(name, stationInfo));
}

int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_staCaption.Create("锋 速 自 研 系 统 U F O", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_Caption);
	m_staCaption.SetBkColor(RGB(240, 240, 240));
	m_staCaption.SetTextColor(RGB(132, 45, 43));
	m_staCaption.SetFont(GetFont());
	m_staCaption.SetTextDisabledColor(RGB(100, 34, 32));

	CSimpleButton_TradeSetting.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_Setting, "设  置", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_TradeSetting.SetRgnDefColor(RGB(132, 45, 43));
	CSimpleButton_TradeSetting.SetRgnHoverColor(RGB(170, 58, 55));
	CSimpleButton_TradeSetting.SetDisabledColor(RGB(100, 34, 32));

	m_btnLogin.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_Login, "登  录", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	m_btnLogin.SetRgnDefColor(RGB(132, 45, 43));
	m_btnLogin.SetRgnHoverColor(RGB(170, 58, 55));
	m_btnLogin.SetDisabledColor(RGB(100, 34, 32));

	m_btnCancel.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_Cancel, "取  消", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	m_btnCancel.SetRgnDefColor(RGB(132, 45, 43));
	m_btnCancel.SetRgnHoverColor(RGB(170, 58, 55));
	m_btnCancel.SetDisabledColor(RGB(100, 34, 32));

// 	m_btnFreshVCode.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Btn_FreshVCode, "刷新验证码", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
// 	m_btnFreshVCode.SetRgnDefColor(RGB(132, 45, 43));
// 	m_btnFreshVCode.SetRgnHoverColor(RGB(170, 58, 55));
// 	m_btnFreshVCode.SetDisabledColor(RGB(100, 34, 32));

	m_keyPad.Create(IDD_DIALOG_Keypad, this);
	return 0;
}

HFONT CLoginDlg::GetFont()
{
	HFONT hFont = NULL;
	CTradeDlg trade;
	CString strStyle = trade.GetFontStyle();
	if (FontStandard == strStyle.Trim())
	{
		hFont = SingleFontSet.GetDefaultFont(DEFAULT_FONT_24B);
	}
	else if (FontMedium == strStyle.Trim())
	{
		hFont = SingleFontSet.GetDefaultFont(DEFAULT_FONT_28B);
	}
	else if (FontBig == strStyle.Trim())
	{
		hFont = SingleFontSet.GetDefaultFont(DEFAULT_FONT_32B);
	}
	return hFont;
}

void CLoginDlg::OnButton(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Button_Setting && nID <= IDC_Btn_FreshVCode)
	{
		switch (nID)
		{
		case IDC_Button_Setting:
			OnBnClickedBtnSetting();
			break;
		case IDC_Button_Login:
			OnBnClickedOk();
			break;
		case IDC_Button_Cancel:
			OnBnClickedCancel();
			break;
		case IDC_Btn_FreshVCode:
			CreateVCode();
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}

void CLoginDlg::InitAllModuleName(VecModuleNameInfo& vecModuleNameInfo)
{
	try
	{
		string strExePath;
		tool::initExePath(strExePath);

		ptree pt;
		read_xml(ConfigPath, pt);
		BOOST_AUTO(child_User, pt.get_child("content.ModuleDLLInfo"));
		for (BOOST_AUTO(pos, child_User.begin()); pos != child_User.end(); ++pos)
		{
			ModuleNameInfo info;
			info.strDllName = pos->second.get<string>("DllName");
			info.strModuleName = pos->second.get<string>("Des");
			vecModuleNameInfo.push_back(info);
		}
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CLoginDlg::InitModuleDLLInfo(MapModuleInfo* pMapModule, VecModuleNameInfo& moduleNameInfo, CSharedByALLCData* pSharedByALLCData)
{
	if (!pMapModule)
	{
		return;
	}
	try
	{
		string strExePath;
		tool::initExePath(strExePath);

		for (auto& data : moduleNameInfo)
		{
			CModuleDLLInfo dllInfo;
			dllInfo.strDllName = data.strDllName;

			string strDllPath = strExePath + "\\" + data.strDllName;
			HINSTANCE hInstance = LoadLibrary(strDllPath.c_str());
			if (hInstance != NULL)
			{
				dllInfo.pGetDllName = (GetDllName*)GetProcAddress(hInstance, "dll_GetDllName");
				string dllNameFromDll = dllInfo.pGetDllName();
				if (dllNameFromDll == data.strDllName)
				{
					dllInfo.hInstance = hInstance;
					dllInfo.pShowDlg = (ShowDlg*)GetProcAddress(hInstance, "dll_ShowDlg");
					dllInfo.pHideDlg = (HideDlg*)GetProcAddress(hInstance, "dll_HideDlg");
					dllInfo.pLogin = (Login*)GetProcAddress(hInstance, "dll_Login");
					dllInfo.strDes = data.strModuleName;
					dllInfo.SPtr_CData = make_shared<CData>();
					if (pSharedByALLCData)
					{
						dllInfo.SPtr_CData->bRun_MainPageDlg = pSharedByALLCData->bRun_MainPageDlg;
						dllInfo.SPtr_CData->hwnd_MainPageDlg = pSharedByALLCData->hwnd_MainPageDlg;
						dllInfo.SPtr_CData->TestSpeed = pSharedByALLCData->TestSpeed;
					}
					pMapModule->insert(make_pair(dllInfo.strDes, dllInfo));
				}	
			}
			else
			{
				FreeLibrary(hInstance);
			}
		}
		CString strCur;
		AssetProp_CComboBox.GetWindowText(strCur);
		MapModuleInfo& mapModule = *pMapModule;
		for (auto& data : mapModule)
		{
			CString strModule;
			strModule.Format("%s", data.second.strDes.c_str());
			if (strModule.Trim() == strCur.Trim())
			{
				pModuleDLLInfo = &(data.second);
				break;
			}
		}

	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

void CLoginDlg::InitModuleDLLInfo(MapModuleInfo* pMapModule, CString strModules, CSharedByALLCData* pSharedByALLCData)
{
	if (!pMapModule)
	{
		return;
	}
	try
	{
		string strExePath;
		tool::initExePath(strExePath);

		ptree pt;
		read_xml(ConfigPath, pt);
		BOOST_AUTO(child_User, pt.get_child("content.ModuleDLLInfo"));
		for (BOOST_AUTO(pos, child_User.begin()); pos != child_User.end(); ++pos)
		{
			CModuleDLLInfo dllInfo;
			dllInfo.strDllName = pos->second.get<string>("DllName");
			CString strDllName;
			strDllName.Format("%s", dllInfo.strDllName.c_str());
			if (strModules.Find(strDllName) >= 0)
			{
				string strDllPath = strExePath + "\\" + dllInfo.strDllName;
				HINSTANCE hInstance = LoadLibrary(strDllPath.c_str());
				if (hInstance != NULL)
				{
					dllInfo.hInstance = hInstance;
					dllInfo.pShowDlg = (ShowDlg*)GetProcAddress(hInstance, "dll_ShowDlg");
					dllInfo.pHideDlg = (HideDlg*)GetProcAddress(hInstance, "dll_HideDlg");
					dllInfo.pLogin = (Login*)GetProcAddress(hInstance, "dll_Login");
					dllInfo.strDes = pos->second.get<string>("Des");
					dllInfo.SPtr_CData = make_shared<CData>();
					if (pSharedByALLCData)
					{
						dllInfo.SPtr_CData->bRun_MainPageDlg = pSharedByALLCData->bRun_MainPageDlg;
						dllInfo.SPtr_CData->hwnd_MainPageDlg = pSharedByALLCData->hwnd_MainPageDlg;
						dllInfo.SPtr_CData->TestSpeed = pSharedByALLCData->TestSpeed;
					}
					pMapModule->insert(make_pair(dllInfo.strDes, dllInfo));
				}
				else
				{
					FreeLibrary(hInstance);
				}
			}
		}
		CString strCur;
		AssetProp_CComboBox.GetWindowText(strCur);
		MapModuleInfo& mapModule = *pMapModule;
		for (auto& data : mapModule)
		{
			CString strModule;
			strModule.Format("%s", data.second.strDes.c_str());
			if (strModule.Trim() == strCur.Trim())
			{
				pModuleDLLInfo = &(data.second);
				break;
			}
		}
	
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

LRESULT CLoginDlg::ClickKeypad(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = reinterpret_cast<CString*>(wParam);
	CString strGet = *pStr;

	CString strText;
	GetDlgItem(IDC_Pass)->GetWindowText(strText);
	strText += strGet;
	GetDlgItem(IDC_Pass)->SetWindowText(strText);
	return 0;
}

LRESULT CLoginDlg::ClickDelete(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	GetDlgItem(IDC_Pass)->GetWindowText(strText);
	CString strTextNew = strText.Left(strText.GetLength() - 1);
	GetDlgItem(IDC_Pass)->SetWindowText(strTextNew);
	return 0;
}

LRESULT CLoginDlg::ClickClear(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	GetDlgItem(IDC_Pass)->SetWindowText("");
	return 0;
}

void CLoginDlg::PaintPriviousVCode(VCodeInfo& vCodeInfo)
{
	CDC *pDC = GetDC();

	CRect Button_Rect;
	GetDlgItem(IDC_EDIT_Vcode)->GetWindowRect(Button_Rect);
	ScreenToClient(Button_Rect);

	for (int i = 0; i < 4; i++)
	{
		if (vCodeInfo.vecColorv.size() > i)
		{
			vector<int> colorv = vCodeInfo.vecColorv[i];
			CFont font;
			bool shanxian = true;
			if (vCodeInfo.randnum > 5)
				shanxian = false;
			CreateVCodeFont(font, Button_Rect.Height() - 3, Button_Rect.Height() - 3, 60 - vCodeInfo.escapenum, shanxian, _T("宋体"));

			vCodeInfo.escapenum += vCodeInfo.randnum;
			while (vCodeInfo.escapenum > 120)
			{
				vCodeInfo.escapenum -= 120;
			}
			pDC->SelectObject(&font);
			pDC->SetTextColor(RGB(colorv[0], colorv[1], colorv[2]));
			pDC->GetBkColor();
			vCodeInfo.randnum = vCodeInfo.randnum + vCodeInfo.randnum;
			while (vCodeInfo.randnum > 9)
			{
				vCodeInfo.randnum = vCodeInfo.randnum - 9;
			}
			if (vCodeInfo.VCodeStr.size() > vCodeInfo.randnum && m_strVerfCode.GetLength() > i)
			{
				pDC->TextOut(Button_Rect.right + 15 + i * Button_Rect.Height() - 3, Button_Rect.top, (CString)m_strVerfCode[i]);
			}
		}
	}

	ReleaseDC(pDC);
}

void CLoginDlg::CreateVCode()
{
	m_strVerfCode = "";
	CDC *pDC = GetDC();

	CRect Button_Rect;
	GetDlgItem(IDC_EDIT_Vcode)->GetWindowRect(Button_Rect);
	ScreenToClient(Button_Rect);

	vector<char> VCodeStr;
	char pchar;
// 	pchar = 'a';
// 	for (int i = 0; i < 26; i++)
// 	{
// 		VCodeStr.push_back(pchar);
// 		pchar++;
// 	}
// 	pchar = 'A';
// 	for (int i = 0; i < 26; i++)
// 	{
// 		VCodeStr.push_back(pchar);
// 		pchar++;
// 	}
	pchar = '0';
	for (int i = 0; i < 10; i++)
	{
		if ('4' != pchar)
		{
			VCodeStr.push_back(pchar);
		}
		pchar++;
	}
	random_shuffle(VCodeStr.begin(), VCodeStr.end());
	m_VCodeInfo.VCodeStr = VCodeStr;
	srand((int)time(0));
	int randnum = rand() % 9;
	int escapenum = rand() % 120;
	int colornum = rand() % 256;
	vector<int > colorv;
	for (int i = 0; i < 3; i++)
	{
		colorv.push_back(colornum);
		colornum = colornum + 100;
		if (colornum > 255)
		{
			colornum = colornum - 255;
		}
	}
	m_VCodeInfo.randnum = randnum;
	m_VCodeInfo.escapenum = escapenum;
	m_VCodeInfo.colornum = colornum;
	m_VCodeInfo.vecColorv.clear();
	for (int i = 0; i < 4; i++)
	{
		random_shuffle(colorv.begin(), colorv.end());
		m_VCodeInfo.vecColorv.push_back(colorv);
		CFont font;
		bool shanxian = true;
		if (randnum > 5)
			shanxian = false;
		//font.CREATE_FONT(60 - escapenum, shanxian, _T("宋体"));
		CreateVCodeFont(font, Button_Rect.Height() - 3, Button_Rect.Height() - 3, 60 - escapenum, shanxian, _T("宋体"));
		//CreateVCodeFont(font, 20, 20, 60 - escapenum, shanxian, _T("宋体"));

		escapenum += randnum;
		while (escapenum > 120)
		{
			escapenum -= 120;
		}
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(colorv[0], colorv[1], colorv[2]));
		pDC->GetBkColor();
		/*pDC->SetBkColor(RGB((randnum+1) * 3, (randnum+1) * 2, randnum + 1));*/
		randnum = randnum + randnum;
		while (randnum > 8)
		{
			randnum = randnum - 8;
		}
		if (VCodeStr.size() > randnum)
		{
			pDC->TextOut(Button_Rect.right + 15 + i * Button_Rect.Height() - 3, Button_Rect.top, (CString)VCodeStr[randnum]);
			m_strVerfCode += (CString)VCodeStr[randnum];
		}
	}

	ReleaseDC(pDC);
}

void CLoginDlg::OnBnClickedButtonCheckvcode()
{
	m_strVerfCode = "";
	CDC *pDC = GetDC();

	CRect Button_Rect;
	GetDlgItem(IDC_EDIT_Vcode)->GetWindowRect(Button_Rect);
	ScreenToClient(Button_Rect);

	vector<char> VCodeStr;
	char pchar;
	pchar = 'a';
	for (int i = 0; i < 26; i++)
	{
		VCodeStr.push_back(pchar);
		pchar++;
	}
	pchar = 'A';
	for (int i = 0; i < 26; i++)
	{
		VCodeStr.push_back(pchar);
		pchar++;
	}
	pchar = '0';
	for (int i = 0; i < 10; i++)
	{
		VCodeStr.push_back(pchar);
		pchar++;
	}
	random_shuffle(VCodeStr.begin(), VCodeStr.end());
	srand((int)time(0));
	int randnum = rand() % 62;
	int escapenum = rand() % 120;
	int colornum = rand() % 256;
	vector<int > colorv;
	for (int i = 0; i < 3; i++)
	{
		colorv.push_back(colornum);
		colornum = colornum + 100;
		if (colornum > 255)
		{
			colornum = colornum - 255;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		random_shuffle(colorv.begin(), colorv.end());
		CFont font;
		bool shanxian = true;
		if (randnum > 30)
			shanxian = false;
		//font.CREATE_FONT(60 - escapenum, shanxian, _T("宋体"));
		CreateVCodeFont(font, Button_Rect.Height() - 3, Button_Rect.Height() - 3, 60 - escapenum, shanxian, _T("宋体"));
		//CreateVCodeFont(font, 20, 20, 60 - escapenum, shanxian, _T("宋体"));

		escapenum += randnum;
		while (escapenum > 120)
		{
			escapenum -= 120;
		}
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(colorv[0], colorv[1], colorv[2]));
		pDC->GetBkColor();
		/*pDC->SetBkColor(RGB((randnum+1) * 3, (randnum+1) * 2, randnum + 1));*/
		randnum = randnum + randnum;
		while (randnum > 61)
		{
			randnum = randnum - 61;
		}
		pDC->TextOut(Button_Rect.right + 15 + i * Button_Rect.Height() - 3, Button_Rect.top, (CString)VCodeStr[randnum]);
		m_strVerfCode += (CString)VCodeStr[randnum];
	}

	ReleaseDC(pDC);
}

void CLoginDlg::CreateVCodeFont(CFont& font, int cHeight, int cWidth, int cEscapement, bool bShanxian, CString strFontName)
{
	font.CreateFont(
		cHeight,
		cWidth,
		cEscapement,
		0,
		900,
		FALSE,
		!bShanxian,
		bShanxian,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		strFontName
	);
}




void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	HandleMouseClick(point);

	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CLoginDlg::HandleMouseClick(const CPoint& point)
{
	CRect rectEditVCode;
	GetDlgItem(IDC_EDIT_Vcode)->GetWindowRect(rectEditVCode);

	CRect rcVCode;
	rcVCode.left = rectEditVCode.right + 15;
	rcVCode.top = rectEditVCode.top;
	rcVCode.right = rcVCode.left + 4 * (rectEditVCode.Height() - 3);
	rcVCode.bottom = rectEditVCode.bottom;
	ScreenToClient(&rcVCode);
	if (rcVCode.PtInRect(point))
	{
		CreateVCode();
	}
}

void CLoginDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HandleMouseClick(point);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


