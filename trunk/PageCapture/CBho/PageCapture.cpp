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
void STDMETHODCALLTYPE CPageCapture::OnDownLoadComplete()
{
	if(this->m_CaptueOption)
		this->Capture();
}

void STDMETHODCALLTYPE CPageCapture::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{

}

STDMETHODIMP CPageCapture::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText)   
{   
	if (cCmds == 0) return E_INVALIDARG;   
	if (prgCmds == 0) return E_POINTER;   
	prgCmds[0].cmdf = OLECMDF_ENABLED;   

	return S_OK;   
}
// 执行脚本
void CPageCapture::ExecScript(BSTR code)
{
	IHTMLWindow2* pWindow;   
	this->m_spHtmlDoc->get_parentWindow(&pWindow);
	VARIANT ret;   
	ret.vt = VT_EMPTY;   
	pWindow->execScript(code,CComBSTR(L"javascript"),&ret);
}

void CPageCapture::Refresh(void)
{
	if(this->m_NoRefresh) return;

	ExecScript(CComBSTR(L"location.reload();"));
}

//卷动页面
void CPageCapture::ScrollPage(IHTMLElement * page,BSTR direction,int value)
{
	if(value<0)
		value = 0;

	page->setAttribute(direction,CComVariant(value));
}

void CPageCapture::Capture(void)
{
	//MessageBox(NULL,this->m_CaptureFileName,L"截图吧",0);

	HRESULT hr;
	CComPtr<IHTMLDocument3> pDoc3 ;
	hr = this->m_spHtmlDoc->QueryInterface(IID_IHTMLDocument3,(void**)&pDoc3) ; 
	
	if(FAILED(hr) || pDoc3 == NULL) return;

	IServiceProvider* pServiceProvider = NULL;

	if (SUCCEEDED(m_spWebBrowser->QueryInterface(
		IID_IServiceProvider, 
		(void**)&pServiceProvider)))
	{
		IOleWindow* pWindow = NULL;
		if (SUCCEEDED(pServiceProvider->QueryService(
			SID_SShellBrowser, 
			IID_IOleWindow,
			(void**)&pWindow)))
		{
			HWND hwndBrowser = NULL;
			if (SUCCEEDED(pWindow->GetWindow(&hwndBrowser)))
			{
				HWND hwnd = FindWindowEx(hwndBrowser, NULL, L"Shell DocObject View", NULL);
				hwnd = FindWindowEx(hwnd, NULL, L"Internet Explorer_Server", NULL);
				if(hwnd!=NULL)
				{	
					//MessageBox(NULL,L"句柄有了",L"截图",0);
					
					//获取documentElement
					IHTMLElement *spDocEle;
					hr = pDoc3->get_documentElement(&spDocEle);
					if(FAILED(hr) || spDocEle == NULL) return;
					
					CComVariant ieHeight,ieWidth,pageHeight,pageWidth,bodyIeHeight,docIeHeight,oldscrollTop,oldscrollLeft; 
					spDocEle->getAttribute(CComBSTR(L"clientHeight"),0,&docIeHeight);

					///获取Body
					IHTMLElement *spBody;
					hr = this->m_spHtmlDoc->get_body(&spBody);
					if(FAILED(hr) || spBody == NULL) return;

					spBody->getAttribute(CComBSTR(L"clientHeight"),0,&bodyIeHeight);
					
					bool bodyEnable = false;
					//实际操作的对象
					IHTMLElement *spPage;

					if (docIeHeight.intVal == 0 && bodyIeHeight.intVal > 0)
					{
						bodyEnable = true;
						spPage = spBody;
					}else
						spPage = spDocEle;

					//获取IE显示尺寸
					spPage->getAttribute(CComBSTR(L"clientHeight"),0,&ieHeight);
					spPage->getAttribute(CComBSTR(L"clientWidth"),0,&ieWidth);
					//获取页面尺寸
					spPage->getAttribute(CComBSTR(L"scrollHeight"),0,&pageHeight);
					spPage->getAttribute(CComBSTR(L"scrollWidth"),0,&pageWidth);

					if (ieHeight.intVal == 0 || ieWidth.intVal == 0)
						return;

					spPage->getAttribute(CComBSTR(L"scrollTop"),0,&oldscrollTop);
					spPage->getAttribute(CComBSTR(L"scrollLeft"),0,&oldscrollLeft);

					POINT pt,srcpt;
					SIZE tmpimg,clientsize,imgsize; 
					//客户区尺寸
					clientsize.cx = ieWidth.intVal;
					clientsize.cy = ieHeight.intVal;
					imgsize.cx = pageWidth.intVal;
					imgsize.cy = pageHeight.intVal;
					
					CString tmp;
					tmp.Format(L"ieHeight:%d",ieHeight.intVal);
					MessageBox(NULL,tmp,L"截图",0);
					tmp.Format(L"ieWidth:%d",ieWidth.intVal);
					MessageBox(NULL,tmp,L"截图",0);

					int vpages = int((double)imgsize.cy/(double)clientsize.cy+0.5); 
					int hpages = int((double)imgsize.cx/(double)imgsize.cx+0.5); 

					CImage img,imgtmp;
					img.CreateEx(imgsize.cx,imgsize.cy,32,BI_RGB);
					

					for(int i=0;i<vpages;i++)
					{
						this->ScrollPage(spPage,CComBSTR(L"scrollTop"),i*clientsize.cy);
						for(int x=0;x<hpages;x++)
						{
							this->ScrollPage(spPage,CComBSTR(L"scrollLeft"),i*clientsize.cx);

							HDC hdcPage = GetDC(NULL);
							HDC hdc = CreateCompatibleDC(hdcPage);
							HBITMAP hbmp = CreateCompatibleBitmap(hdcPage,clientsize.cx+2,clientsize.cy+2);
							SelectObject(hdc, hbmp);
							//截图
							PrintWindow(hwnd, hdc, PW_CLIENTONLY);


							//CImage test;
							//test.Attach(hbmp);
							//test.Save(this->m_CaptureFileName);
							//return;

							pt.x = x*clientsize.cx;
							pt.y = i*clientsize.cy;
							tmpimg.cx = clientsize.cx;
							tmpimg.cy = clientsize.cy;
							srcpt.x = 2;
							srcpt.y = 2;
							
							if(i == vpages-1 && i>0)
							{
								//pt.y --;
								tmpimg.cy = imgsize.cy - i*clientsize.cy;
								srcpt.y = clientsize.cy - tmpimg.cy ;
							}

							if(x == hpages-1 && x>0)
							{
								//pt.x --;
								tmpimg.cx = imgsize.cx - x*clientsize.cx;
								srcpt.x = clientsize.cx - tmpimg.cx;
							}

							//拼贴在一起
							BitBlt(img.GetDC(),pt.x, pt.y,tmpimg.cx,tmpimg.cy,hdc,srcpt.x,srcpt.y,SRCCOPY);

							DeleteDC(hdc);
							DeleteObject(hbmp);
							ReleaseDC(NULL, hdcPage);
						}
					}
					img.Save(this->m_CaptureFileName);
					///恢复位置
					spPage->setAttribute(CComBSTR(L"scrollTop"),oldscrollTop);
					spPage->setAttribute(CComBSTR(L"scrollLeft"),oldscrollLeft);
				}
			}

			pWindow->Release();
		}

		pServiceProvider->Release();
	}

	this->m_CaptueOption = false;
	this->m_CaptureFileName.Empty();

	if(!this->m_NoRefresh)
	{
		/*CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(this->pDocDispatch);
		if (spCustomDoc)
			spCustomDoc->SetUIHandler(this);*/
	}
	
	this->pDocDispatch.Release();
	this->Refresh();
}

STDMETHODIMP CPageCapture::Exec(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG*)   
{   

	if(m_spUnkSite == NULL || m_spWebBrowser == NULL) return S_OK;

	HRESULT hr;
	BSTR btitle;
	CString stitle;

	hr = this->m_spWebBrowser->get_Document(&this->pDocDispatch);

	if(FAILED(hr) || this->pDocDispatch == NULL) return S_OK;

	this->m_spHtmlDoc = pDocDispatch;

	if(this->m_CaptureFileName.IsEmpty())
	{
		hr = this->m_spHtmlDoc->get_title(&btitle);

		stitle = OLE2CT(btitle);

		if(stitle.GetLength()<1)
			stitle = L"pagecapture.jpg";
		else
			stitle.Format(L"%s.jpg",stitle.Left(10));

		CFileDialog filedialog(FALSE,L".jpg",stitle,OFN_HIDEREADONLY,L"JPG 文件(*.jpg)|*.jpg||");  
		filedialog.m_ofn.lpstrTitle = L"请输入文件名";

		if(filedialog.DoModal() != IDOK)
		{
			this->pDocDispatch.Release();
			return S_OK; 
		}

		this->m_CaptureFileName =  filedialog.GetPathName();
	}

	this->m_CaptueOption = true;

	if(this->m_NoRefresh)
	{
		this->Capture();
	}else
	{
	/*	CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(pDocDispatch);
		if (spCustomDoc)
			spCustomDoc->SetUIHandler(this);*/
	}
	
	this->Refresh();

	return S_OK;
}