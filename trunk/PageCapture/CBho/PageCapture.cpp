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
   	

	MessageBox(NULL,L"��ͼ��",L"pagecapture",0);

	return S_OK;   
}