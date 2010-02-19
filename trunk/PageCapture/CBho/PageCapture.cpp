// PageCapture.cpp : Implementation of CPageCapture

#include "stdafx.h"
#include "PageCapture.h"

// CPageCapture
STDMETHODIMP CPageCapture::SetSite(IUnknown *pUnkSite)
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
	return IObjectWithSiteImpl<CPageCapture>::SetSite(pUnkSite);   
}  

void STDMETHODCALLTYPE CPageCapture::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
	//MessageBox(NULL,L"test",L"pagecapture",0);
}

STDMETHODIMP CPageCapture::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText)   
{   
    if (cCmds == 0) return E_INVALIDARG;   
    if (prgCmds == 0) return E_POINTER;   
    prgCmds[0].cmdf = OLECMDF_ENABLED;   
       
    return S_OK;   
}

STDMETHODIMP CPageCapture::Exec(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG*)   
{   
    if(m_spUnkSite == 0 || m_spWebBrowser == 0) return S_OK;   
   	

	MessageBox(NULL,L"截图吧",L"pagecapture",0);

	return S_OK;   
}