// ProxySwitch.h : Declaration of the CProxySwitch

#pragma once
#include "resource.h"       // main symbols
#include "exdispid.h"


#include "CBho_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CProxySwitch

class ATL_NO_VTABLE CProxySwitch :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProxySwitch, &CLSID_ProxySwitch>,
	public IObjectWithSiteImpl<CProxySwitch>,
	public IDispatchImpl<IProxySwitch, &IID_IProxySwitch, &LIBID_CBhoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	//用于IE事件
	public IDispEventImpl<1, CProxySwitch, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>,
	//用于工具栏图标
	public IOleCommandTarget
{
public:
	CProxySwitch()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROXYSWITCH)


BEGIN_COM_MAP(CProxySwitch)
	COM_INTERFACE_ENTRY(IProxySwitch)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
			//用于工具栏图标
	COM_INTERFACE_ENTRY(IOleCommandTarget)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	BOOL m_fAdvised;
	STDMETHOD(SetSite)(IUnknown * pUnkSite);
	CComPtr<IWebBrowser2> m_spWebBrowser;

	//用于IE事件
	BEGIN_SINK_MAP(CProxySwitch)
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete)
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownLoadComplete)
	END_SINK_MAP()

	//用于工具栏图标
	STDMETHOD(Exec)(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG* pvaOut);   
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText);

	bool FileIsExist(CString &file);
	bool ChekProxyProcess(CString &name,bool kill);
	void Switch(void);
};

OBJECT_ENTRY_AUTO(__uuidof(ProxySwitch), CProxySwitch)
