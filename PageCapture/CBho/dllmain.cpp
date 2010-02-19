// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "CBho_i.h"
#include "dllmain.h"

CCBhoModule _AtlModule;

class CCBhoApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCBhoApp, CWinApp)
END_MESSAGE_MAP()

CCBhoApp theApp;

BOOL CCBhoApp::InitInstance()
{
	CString Loader;
	int i1,i2;
	TCHAR loader[MAX_PATH]; 

	GetModuleFileName(NULL,loader, MAX_PATH); 
	Loader = loader;

	Loader = Loader.Mid(Loader.ReverseFind(_T('\\'))+1);
	i1 = Loader.Compare(_T("regsvr32.exe")) ;
	i2 = Loader.Compare(_T("iexplore.exe"));

	Loader.Empty();

	if (i1!=0 && i2!=0) 
		return FALSE; 

	return CWinApp::InitInstance();
}

int CCBhoApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
