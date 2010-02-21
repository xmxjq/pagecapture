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
	//public IDocHostUIHandler
{
public:
	CPageCapture()
	{
		this->m_CaptueOption = false;
		this->m_NoRefresh = true;
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
	void Refresh(void);
	void Capture(void);
	CComPtr<IHTMLDocument2> m_spHtmlDoc;
	CComPtr<IDispatch> pDocDispatch;
	void ExecScript(BSTR code);
	CString m_CaptureFileName;
	STDMETHOD(SetSite)(IUnknown * pUnkSite);
	CComPtr<IWebBrowser2> m_spWebBrowser;

	void STDMETHODCALLTYPE OnDocumentComplete(IDispatch* pDisp, VARIANT* URL); 
	void STDMETHODCALLTYPE OnDownLoadComplete(); 

	BOOL m_fAdvised;
	//用于IE事件
	BEGIN_SINK_MAP(CPageCapture)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownLoadComplete)
	END_SINK_MAP()

	//用于工具栏图标
	STDMETHOD(Exec)(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG* pvaOut);   
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText);
	
	bool m_CaptueOption;
	//
	// IDocHostUIHandler 用于取消滚条条
	//
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO FAR *pInfo)
	{

		//if(m_CaptueOption)
		//{
		//	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
		//	pInfo->dwFlags =  DOCHOSTUIFLAG_DIALOG |
		//		DOCHOSTUIFLAG_THEME  |
		//		DOCHOSTUIFLAG_NO3DBORDER |
		//		DOCHOSTUIFLAG_SCROLL_NO;
		//}

		return S_OK;
	}

	bool m_NoRefresh;
	//IDocHostUIHandler 接口其他无用的处理方法
		STDMETHOD(ShowContextMenu)(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdTarget, IDispatch FAR* pdispReserved)
	{
		return S_FALSE;
	}

	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject,
		IOleCommandTarget FAR* pCommandTarget,
		IOleInPlaceFrame  FAR* pFrame,
		IOleInPlaceUIWindow FAR* pDoc)
	{
		return S_FALSE;
	}

	STDMETHOD(HideUI)(void)
	{
		return S_OK;
	}

	STDMETHOD(UpdateUI)(void)
	{

		return S_OK;
	}

	STDMETHOD(EnableModeless)(BOOL fEnable)
	{

		return S_OK;
	}

	STDMETHOD(OnDocWindowActivate)(BOOL fActivate)
	{

		return S_OK;
	}

	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate)
	{

		return S_OK;
	}

	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow, BOOL fFrameWindow)
	{

		return S_OK;
	}

	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID FAR* pguidCmdGroup, DWORD nCmdID)
	{

		return E_NOTIMPL;
	}

	STDMETHOD(GetOptionKeyPath)(LPOLESTR FAR* pchKey, DWORD dw)
	{
		return E_FAIL;
	}

	STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
	{
		return S_OK;
	}

	STDMETHOD(GetExternal)(IDispatch** ppDispatch)
	{
		return S_FALSE;
	}

	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
	{
		return S_FALSE;
	}

	STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet)
	{
		return S_FALSE;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(PageCapture), CPageCapture)
