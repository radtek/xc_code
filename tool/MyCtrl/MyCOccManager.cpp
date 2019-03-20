#include "stdafx.h"
#include "MyCOccManager.h"
#include "../../share/define.h"

#include <string>
#include <iostream>
using namespace std;

#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;

MyCOccManager::MyCOccManager()
{
}


MyCOccManager::~MyCOccManager()
{
}

const DLGTEMPLATE* MyCOccManager::PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo, const DLGTEMPLATE* pOrigTemplate)
{
	CString s_font;
	unsigned int fontSize;
	ptree pt;
	try {
		string s_ConfigPath = "..\\share\\config.xml";
		read_xml(s_ConfigPath, pt);
		
		s_font = pt.get<string>("content.Font").c_str();
		//fontSize = pt.get<int>("content.FontSize");
		CString strFont = pt.get<string>("content.FontSize").c_str();
		if (FontStandard == strFont || strFont.IsEmpty())
		{
			fontSize = 9;
		}
		else if (FontMedium == strFont)
		{
			fontSize = 11;
		}
		else if (FontBig == strFont)
		{
			fontSize = 13;
		}	
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ASSERT(0);
		return false;
	}
	CDialogTemplate ModifiedTemplate(pOrigTemplate);
	ModifiedTemplate.SetFont(s_font, fontSize);//Ms Shell Dlg
	return COccManager::PreCreateDialog(pOccDialogInfo, (DLGTEMPLATE*)ModifiedTemplate.Detach());
}
