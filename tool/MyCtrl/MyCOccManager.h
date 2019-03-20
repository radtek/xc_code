#pragma once
#include "afxocc.h"

class MyCOccManager :
	public COccManager
{
public:
	MyCOccManager();
	~MyCOccManager();
	virtual const DLGTEMPLATE* PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo, const DLGTEMPLATE* pOrigTemplate);
};

