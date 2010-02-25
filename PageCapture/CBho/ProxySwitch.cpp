// ProxySwitch.cpp : Implementation of CProxySwitch

#include "stdafx.h"
#include "ProxySwitch.h"
#include "wininet.h"
//不会加载Wininet.lib不能静态编译
#pragma comment(lib, "Wininet.lib")
#include "tlhelp32.h" 

// CProxySwitch
STDMETHODIMP CProxySwitch::SetSite(IUnknown *pUnkSite)
{
	if(pUnkSite!=NULL)   
	{   
		HRESULT hr;   
		CComPtr<IServiceProvider> sp;   

		hr = pUnkSite->QueryInterface(&sp);   
		if(SUCCEEDED(hr) && sp)   
		{   
			//缓存指向IWebBrowser2的指针   
			hr = sp->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&m_spWebBrowser);   

			if(SUCCEEDED(hr)&&m_spWebBrowser!=0)   
			{   
				//注册DWebBrowserEvents2事件。   
				hr=DispEventAdvise(m_spWebBrowser);   

				if(SUCCEEDED(hr))   
				{   
					m_fAdvised=TRUE;   
				}   
			}   
		}   
		m_spUnkSite = pUnkSite;   

	}   
	else  
	{   
		//取消注册事件。   
		if(m_fAdvised)   
		{   
			DispEventUnadvise(m_spWebBrowser);   
			m_fAdvised=FALSE;   
		}   
		//在此释放缓存的指针和其他资源。   
		m_spWebBrowser.Release();   
		//m_spTarget.Release();   
	}   
	//调用基类实现。   
	return IObjectWithSiteImpl<CProxySwitch>::SetSite(pUnkSite);   
}  

STDMETHODIMP CProxySwitch::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText)   
{   
	if (cCmds == 0) return E_INVALIDARG;   
	if (prgCmds == 0) return E_POINTER;   
	prgCmds[0].cmdf = OLECMDF_ENABLED;   

	return S_OK;   
}

bool CProxySwitch::FileIsExist(CString &file)
{
	CString fullname;
	//WCHAR sPath[MAX_PATH] ;
	//GetModuleFileName(NULL, sPath, MAX_PATH);
	//fullname.AppendFormat(sPath);
	//int index = fullname.ReverseFind(L'\\');
	//fullname.AppendFormat(L"%s\\%s",fullname.Left(index),file);
	//AfxMessageBox(fullname);

	fullname.Format(L"c:\\%s",file);
	DWORD dwAttr   =   GetFileAttributes(fullname);
	if(dwAttr != (DWORD)   -1)   
	{
		file.Format(L"%s",fullname);
		return true;
	}

	return false;
}

///检测proxy 进程是否运行，
bool CProxySwitch::ChekProxyProcess(CString &name,bool kill)
{
	bool result = false;
	HANDLE   hSnapshotPro;     
	
	//创建遍历进程所需要的Snapshot     
	hSnapshotPro = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);     
	
	if  (!hSnapshotPro)   return   0;   
	//循环遍历进程列表中的进程     
	
	PROCESSENTRY32  ProcessEntry;     
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);   
	Process32First(hSnapshotPro,   &ProcessEntry);    

	DWORD npid = GetCurrentProcessId();

	while (Process32Next(hSnapshotPro, &ProcessEntry)!=0)     
	{ 

		if(ProcessEntry.th32ProcessID != GetCurrentProcessId() &&
				ProcessEntry.szExeFile == name)
		{
			//第是否杀死进程
			if(kill)
			{

#if ! _DEBUG
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,ProcessEntry.th32ProcessID);
				::TerminateProcess(hProcess,0);
#else
				OutputDebugString(L"发现相同进程并准备杀掉它！\n\r");  
#endif
			}

			result = true;
			break;
		}
	}

	CloseHandle(hSnapshotPro); 

	return result;
}

void CProxySwitch::Switch(void)
{
	INTERNET_PER_CONN_OPTION_LISTW    List;
	INTERNET_PER_CONN_OPTIONW         Option[5];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LISTW);

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LISTW);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;

	CString proxy = L"proxy.exe";

	if(InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
	{
		if(Option[2].Value.dwValue == PROXY_TYPE_PROXY)
		{
			 Option[2].Value.dwValue = PROXY_TYPE_DIRECT;
			 this->ChekProxyProcess(proxy,true);
		}else
		{
			Option[2].Value.dwValue = PROXY_TYPE_PROXY;
			if(!this->ChekProxyProcess(proxy,false))
				if(this->FileIsExist(proxy))
					ShellExecute(NULL,L"open",proxy,NULL,NULL,SW_SHOWNORMAL);
		}
	}
	
	//激活代理
	if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION,&List,sizeof(INTERNET_PER_CONN_OPTION_LISTW)))
		AfxMessageBox(L"InternetQueryOption failed! (%d)\n", GetLastError()); 
	else
	{
		if(Option[2].Value.dwValue == PROXY_TYPE_DIRECT)
			AfxMessageBox(L"代理 关闭。");
		else
			AfxMessageBox(L"代理 激活。");
	}
}

STDMETHODIMP CProxySwitch::Exec(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG*)   
{   
	this->Switch();
	return S_OK;
}