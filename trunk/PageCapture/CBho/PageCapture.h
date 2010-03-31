// PageCapture.h : Declaration of the CPageCapture

#pragma once
#include "resource.h"       // main symbols
#include "exdispid.h"


#include "CBho_i.h"

// Define the event sink cookies
#define IDE_WebBrowserEvents	1
#define IDE_WebBrowserEvents2	2


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
	public IDispEventImpl<IDE_WebBrowserEvents2, CPageCapture, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>,

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
	void ScrollPage(IHTMLElement * page,BSTR direction,int value);
	
	void Refresh(CComPtr<IHTMLDocument2> m_spHtmlDoc);
	void SelectFileName(void);
	void Capture(CComPtr<IHTMLDocument2> m_spHtmlDoc,CComPtr<IDispatch> pDocDispatch,CString filename);
	void ExecScript(CComPtr<IHTMLDocument2> m_spHtmlDoc,BSTR code);

	BOOL m_fAdvised;
	STDMETHOD(SetSite)(IUnknown * pUnkSite);
	CComPtr<IWebBrowser2> m_spWebBrowser;

	void _stdcall OnBeforeNavigate2(IDispatch *pDisp, VARIANT *URL, VARIANT *Flags, VARIANT *TargetFrameName, VARIANT *PostData, VARIANT *Headers, VARIANT_BOOL *Cancel);

	//void STDMETHODCALLTYPE OnDocumentComplete(IDispatch* pDisp, VARIANT* URL); 
	//void STDMETHODCALLTYPE OnDownLoadComplete(); 

	//用于IE事件
	BEGIN_SINK_MAP(CPageCapture)
		SINK_ENTRY_EX(IDE_WebBrowserEvents2, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2 , OnBeforeNavigate2)
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete)
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownLoadComplete)
	END_SINK_MAP()

	//用于工具栏图标
	STDMETHOD(Exec)(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG *pvaIn, VARIANTARG *pvaOut);   
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText);

};

OBJECT_ENTRY_AUTO(__uuidof(PageCapture), CPageCapture)
