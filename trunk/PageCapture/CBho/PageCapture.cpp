// PageCapture.cpp : Implementation of CPageCapture
#include "stdafx.h"
#include "PageCapture.h"
#include "math.h"

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

//void STDMETHODCALLTYPE CPageCapture::OnDownLoadComplete()
//{
//}

//void STDMETHODCALLTYPE CPageCapture::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
//{
//
//}

void _stdcall CPageCapture::OnBeforeNavigate2(IDispatch *pDisp, VARIANT *URL, VARIANT *Flags, VARIANT *TargetFrameName, VARIANT *PostData, VARIANT *Headers, VARIANT_BOOL *Cancel)
{
	CString url(V_BSTR(URL)); 
	url = url.MakeLower();
	//AfxMessageBox(url);
	if (url.Right(5) == _T("tycap"))
	{
		*Cancel = TRUE;
		this->SelectFileName();
	}

}

STDMETHODIMP CPageCapture::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText)   
{   
	if (cCmds == 0) return E_INVALIDARG;   
	if (prgCmds == 0) return E_POINTER;   
	prgCmds[0].cmdf = OLECMDF_ENABLED;   

	return S_OK;   
}
// 执行脚本
void CPageCapture::ExecScript(CComPtr<IHTMLDocument2> m_spHtmlDoc,BSTR code)
{
	IHTMLWindow2* pWindow;   
	m_spHtmlDoc->get_parentWindow(&pWindow);
	VARIANT ret;   
	ret.vt = VT_EMPTY;   
	pWindow->execScript(code,CComBSTR(L"javascript"),&ret);
}

void CPageCapture::Refresh(CComPtr<IHTMLDocument2> m_spHtmlDoc)
{
	ExecScript(m_spHtmlDoc,CComBSTR(L"location.reload();"));
}

//卷动页面
void CPageCapture::ScrollPage(IHTMLElement * page,BSTR direction,int value)
{
	if(value<0)
		value = 0;

	page->setAttribute(direction,CComVariant(value));
}

void CPageCapture::Capture(CComPtr<IHTMLDocument2> m_spHtmlDoc,CComPtr<IDispatch> pDocDispatch,CString filename)
{
	HRESULT hr;
	CComPtr<IHTMLDocument3> pDoc3 ;

	hr = m_spHtmlDoc->QueryInterface(IID_IHTMLDocument3,(void**)&pDoc3) ; 
	
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
					hr = m_spHtmlDoc->get_body(&spBody);
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
					
					int vpages = int(ceil((double)imgsize.cy/(double)clientsize.cy)); 
					int hpages = int(ceil((double)imgsize.cx/(double)clientsize.cx)); 

					CImage img,imgtmp;
					img.CreateEx(imgsize.cx,imgsize.cy,32,BI_RGB);

					for(int y=0;y<vpages;y++)
					{
						this->ScrollPage(spPage,CComBSTR(L"scrollTop"),y*clientsize.cy);
						for(int x=0;x<hpages;x++)
						{
							this->ScrollPage(spPage,CComBSTR(L"scrollLeft"),x*clientsize.cx);
							
							pt.x = x*clientsize.cx;
							pt.y = y*clientsize.cy;
							tmpimg.cx = clientsize.cx;
							tmpimg.cy = clientsize.cy;
							srcpt.x = 2;
							srcpt.y = 2;
							
							if(y == vpages-1 && y>0)
							{
								//pt.y --;
								tmpimg.cy = imgsize.cy - y*clientsize.cy;
								srcpt.y = clientsize.cy - tmpimg.cy ;
							}

							if(x == hpages-1 && x>0)
							{
								//pt.x --;
								tmpimg.cx = imgsize.cx - x*clientsize.cx;
								srcpt.x = clientsize.cx - tmpimg.cx;
							}

							HDC hdcPage = GetDC(NULL);
							HDC hdc = CreateCompatibleDC(hdcPage);
							HBITMAP hbmp = CreateCompatibleBitmap(hdcPage,clientsize.cx+2,clientsize.cy+2);
							SelectObject(hdc, hbmp);

							PrintWindow(hwnd, hdc, PW_CLIENTONLY);

							///拼贴在一起
							BitBlt(img.GetDC(),pt.x, pt.y,tmpimg.cx,tmpimg.cy,hdc,srcpt.x,srcpt.y,SRCCOPY);

							img.ReleaseDC();
							DeleteDC(hdc);
							DeleteObject(hbmp);
							ReleaseDC(NULL, hdcPage);
						}
					}

					///恢复位置
					spPage->setAttribute(CComBSTR(L"scrollTop"),oldscrollTop);
					spPage->setAttribute(CComBSTR(L"scrollLeft"),oldscrollLeft);
					
					if(!img.IsNull())
					{
						if(SUCCEEDED(img.Save(filename)))
						{
							//打开图片
							ShellExecute(NULL,L"open",filename,NULL,NULL,SW_SHOWMAXIMIZED); 
						}

						img.Destroy();
					}
				}
			}

			pWindow->Release();
		}

		pServiceProvider->Release();
	}

}

STDMETHODIMP CPageCapture::Exec(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG *pvaIn, VARIANTARG *pvaOut)  
{   
	if(m_spUnkSite == NULL || m_spWebBrowser == NULL) return S_OK;
	this->SelectFileName();
	return S_OK;
}

void CPageCapture::SelectFileName(void)
{
	HRESULT hr;
	BSTR btitle;
	CString stitle;
	CComPtr<IHTMLDocument2> m_spHtmlDoc;
	CComPtr<IDispatch> pDocDispatch;

	hr = this->m_spWebBrowser->get_Document(&pDocDispatch);

	if(FAILED(hr) || pDocDispatch == NULL) return;

	m_spHtmlDoc = pDocDispatch;

	hr = m_spHtmlDoc->get_title(&btitle);

	stitle = OLE2CT(btitle);

	if(stitle.GetLength()<1)
		stitle = L"pagecapture.jpg";
	else
	{
		if(stitle.GetLength() > 100)
		{
			if(stitle.GetAt(99)>128)
				stitle.Format(L"%s.jpg",stitle.Left(99));
			else
			  stitle.Format(L"%s.jpg",stitle.Left(100));
		}
		
	}

	AFX_MANAGE_STATE(AfxGetStaticModuleState());   

	stitle.Replace(L"\"",L"");
	stitle.Replace(L"\\",L"");
	stitle.Replace(L"/",L"");
	stitle.Replace(L"<",L"");
	stitle.Replace(L">",L"");
	stitle.Replace(L"?",L"");
	stitle.Replace(L"*",L"");
	stitle.Replace(L":",L"");
	stitle.Replace(L"|",L"");

	TCHAR sPath[MAX_PATH];
	SHGetSpecialFolderPath(0,sPath,CSIDL_DESKTOPDIRECTORY,0);  
	this->GetOnlyFileName(sPath,stitle);

	CFileDialog filedialog(FALSE,L".jpg",stitle,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"JPG 文件(*.jpg)|*.jpg||");  
	
	filedialog.m_ofn.lpstrTitle = L"请输入文件名";


	if(filedialog.DoModal() == IDOK)
	{
		this->Capture(m_spHtmlDoc,pDocDispatch,filedialog.GetFileName());
	}
	pDocDispatch.Release();
}

// 生成唯一文件名
void CPageCapture::GetOnlyFileName(LPCWSTR InitialDir, CString & m_title)
{
	CString fn;
	fn.Format(L"%s\\%s.jpg",InitialDir,m_title);
	int index = 0;
	while(this->FileIsExist(fn))
	{
		fn.Format(L"%s\\%s[%d].jpg",InitialDir,m_title,index);
		++index;
	}

	m_title.Format(L"%s",fn);
}


// 检测文件是否存在
bool CPageCapture::FileIsExist(CString & m_file)
{
	DWORD dwAttr   =   GetFileAttributes(m_file);
	if(dwAttr != (DWORD)   -1)   
		return true;

	return false;
}
