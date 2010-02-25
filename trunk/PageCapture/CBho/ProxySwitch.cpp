// ProxySwitch.cpp : Implementation of CProxySwitch

#include "stdafx.h"
#include "ProxySwitch.h"
#include "wininet.h"
//�������Wininet.lib���ܾ�̬����
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
			//����ָ��IWebBrowser2��ָ��   
			hr = sp->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&m_spWebBrowser);   

			if(SUCCEEDED(hr)&&m_spWebBrowser!=0)   
			{   
				//ע��DWebBrowserEvents2�¼���   
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
		//ȡ��ע���¼���   
		if(m_fAdvised)   
		{   
			DispEventUnadvise(m_spWebBrowser);   
			m_fAdvised=FALSE;   
		}   
		//�ڴ��ͷŻ����ָ���������Դ��   
		m_spWebBrowser.Release();   
		//m_spTarget.Release();   
	}   
	//���û���ʵ�֡�   
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

///���proxy �����Ƿ����У�
bool CProxySwitch::ChekProxyProcess(CString &name,bool kill)
{
	bool result = false;
	HANDLE   hSnapshotPro;     
	
	//����������������Ҫ��Snapshot     
	hSnapshotPro = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);     
	
	if  (!hSnapshotPro)   return   0;   
	//ѭ�����������б��еĽ���     
	
	PROCESSENTRY32  ProcessEntry;     
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);   
	Process32First(hSnapshotPro,   &ProcessEntry);    

	DWORD npid = GetCurrentProcessId();

	while (Process32Next(hSnapshotPro, &ProcessEntry)!=0)     
	{ 

		if(ProcessEntry.th32ProcessID != GetCurrentProcessId() &&
				ProcessEntry.szExeFile == name)
		{
			//���Ƿ�ɱ������
			if(kill)
			{

#if ! _DEBUG
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,ProcessEntry.th32ProcessID);
				::TerminateProcess(hProcess,0);
#else
				OutputDebugString(L"������ͬ���̲�׼��ɱ������\n\r");  
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
	
	//�������
	if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION,&List,sizeof(INTERNET_PER_CONN_OPTION_LISTW)))
		AfxMessageBox(L"InternetQueryOption failed! (%d)\n", GetLastError()); 
	else
	{
		if(Option[2].Value.dwValue == PROXY_TYPE_DIRECT)
			AfxMessageBox(L"���� �رա�");
		else
			AfxMessageBox(L"���� ���");
	}
}

STDMETHODIMP CProxySwitch::Exec(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG*)   
{   
	this->Switch();
	return S_OK;
}