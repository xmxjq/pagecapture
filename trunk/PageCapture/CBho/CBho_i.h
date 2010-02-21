

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Feb 21 20:48:28 2010
 */
/* Compiler settings for CBho.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CBho_i_h__
#define __CBho_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPageCapture_FWD_DEFINED__
#define __IPageCapture_FWD_DEFINED__
typedef interface IPageCapture IPageCapture;
#endif 	/* __IPageCapture_FWD_DEFINED__ */


#ifndef __PageCapture_FWD_DEFINED__
#define __PageCapture_FWD_DEFINED__

#ifdef __cplusplus
typedef class PageCapture PageCapture;
#else
typedef struct PageCapture PageCapture;
#endif /* __cplusplus */

#endif 	/* __PageCapture_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPageCapture_INTERFACE_DEFINED__
#define __IPageCapture_INTERFACE_DEFINED__

/* interface IPageCapture */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPageCapture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0B9CA036-554B-474E-B920-74035F37B369")
    IPageCapture : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPageCaptureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPageCapture * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPageCapture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPageCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPageCapture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPageCapture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPageCapture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPageCapture * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IPageCaptureVtbl;

    interface IPageCapture
    {
        CONST_VTBL struct IPageCaptureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPageCapture_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPageCapture_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPageCapture_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPageCapture_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPageCapture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPageCapture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPageCapture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPageCapture_INTERFACE_DEFINED__ */



#ifndef __CBhoLib_LIBRARY_DEFINED__
#define __CBhoLib_LIBRARY_DEFINED__

/* library CBhoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CBhoLib;

EXTERN_C const CLSID CLSID_PageCapture;

#ifdef __cplusplus

class DECLSPEC_UUID("7EEF1B3C-280A-4C17-A1F0-45AD0CFDAECE")
PageCapture;
#endif
#endif /* __CBhoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


