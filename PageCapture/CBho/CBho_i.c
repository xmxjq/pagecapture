

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 11 12:05:56 2010
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPageCapture,0x0B9CA036,0x554B,0x474E,0xB9,0x20,0x74,0x03,0x5F,0x37,0xB3,0x69);


MIDL_DEFINE_GUID(IID, IID_IProxySwitch,0xB051BE98,0xC45B,0x46AB,0x8A,0x2A,0xCF,0xFD,0x93,0x9C,0xDA,0x74);


MIDL_DEFINE_GUID(IID, LIBID_CBhoLib,0x89711670,0xD382,0x4BFF,0x9B,0xF6,0xF8,0x44,0x07,0xE8,0x54,0x1A);


MIDL_DEFINE_GUID(CLSID, CLSID_PageCapture,0x7EEF1B3C,0x280A,0x4C17,0xA1,0xF0,0x45,0xAD,0x0C,0xFD,0xAE,0xCE);


MIDL_DEFINE_GUID(CLSID, CLSID_ProxySwitch,0xB2611A8C,0x379A,0x4AC6,0x84,0x12,0xB8,0xCA,0xF5,0xC4,0x2E,0xD0);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



