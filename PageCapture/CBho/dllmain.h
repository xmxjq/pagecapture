// dllmain.h : Declaration of module class.

class CCBhoModule : public ATL::CAtlDllModuleT< CCBhoModule >
{
public :
	DECLARE_LIBID(LIBID_CBhoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CBHO, "{517B2083-7932-4402-BD24-A493F2BBE38B}")
};

extern class CCBhoModule _AtlModule;
