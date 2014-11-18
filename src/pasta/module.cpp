#include "stdafx.h"
#include "module.h"
#include <atlstr.h>

Module::Module()
{
}

Module::~Module()
{
    unload();
}

//-----------------------------------------------------------------------------
// Ž©“®ŠJ•ú
class AutoGrobal
{
public:
    HGLOBAL m_hGlobal;
    AutoGrobal(HGLOBAL hGlobal) {
        m_hGlobal = hGlobal;
    }
    ~AutoGrobal() {
        GlobalFree(m_hGlobal);
    }
};

//-----------------------------------------------------------------------------
// CP to CComBSTR •¶Žš—ñ•ÏŠ·
inline CComBSTR g2CComBSTR(HGLOBAL hg, long bytes, UINT cp){
    USES_CONVERSION;
    auto str = CAtlStringA((LPCSTR)hg, bytes);
    auto wide = A2CW_CP(str, cp);
    auto bstr = CComBSTR(wide);
    return bstr;
}


//-----------------------------------------------------------------------------
// SHIORI unload
BOOL Module::unload(void)
{
    try{
        return true;
    }
    catch (...){
        return false;
    }
}

//-----------------------------------------------------------------------------
// SHIORI load
BOOL Module::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    AutoGrobal ag1(hGlobal_loaddir);
    auto loaddir = g2CComBSTR(hGlobal_loaddir, loaddir_len, CP_ACP);
    try{
        return true;
    }
    catch (...){
        return false;
    }
}

//-----------------------------------------------------------------------------
// SHIORI request
HGLOBAL Module::request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag1(hGlobal_request);
    auto req = g2CComBSTR(hGlobal_request, len, cp);

    try{

        return NULL;
    }
    catch (...){
        return NULL;
    }
}

