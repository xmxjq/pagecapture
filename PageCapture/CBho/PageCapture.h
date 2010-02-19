// PageCapture.h : Declaration of the CPageCapture

#pragma once
#include "resource.h"       // main symbols
#include "exdispid.h"
#include "shlguid.h"


#include "CBho_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CPageCapture
class ATL_NO_VTABLE CPageCapture :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPageCapture, &CLSID_PageCapture>,
	public IObjectWithSiteImpl<CPageCapture>,
	public IDispatchImpl<IPageCapture, &IID_IPageCapture, &LIBID_CBhoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	//用于IE事件
	public IDispEventImpl<1, CPageCapture, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>,
	//用于工具栏图标
	public IOleCommandTarget
{
public:
	CPageCapture()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PAGECAPTURE)

DECLARE_NOT_AGGREGATABLE(CPageCapture)

BEGIN_COM_MAP(CPageCapture)
	COM_INTERFACE_ENTRY(IPageCapture)
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
	
	STDMETHOD(SetSite)(IUnknown * pUnkSite);
	CComPtr<IWebBrowser2> m_spWebBrowser;

	void STDMETHODCALLTYPE OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);  
	BOOL m_fAdvised;
	//用于IE事件
	BEGIN_SINK_MAP(CPageCapture)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
	END_SINK_MAP()

	//用于工具栏图标
	STDMETHOD(Exec)(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG* pvaOut);   
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText);  



};

OBJECT_ENTRY_AUTO(__uuidof(PageCapture), CPageCapture)
