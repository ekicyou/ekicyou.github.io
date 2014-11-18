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

//-------------------------------------------------------------
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

#define A2CComBSTR_CP(lpa, nChars, cp) (CComBSTR(A2W_CP_EX(lpa, nChars, cp)))

#define G2CComBSTR_CP(hg, nChars, cp) A2CComBSTR_CP((LPCSTR)hg, nChars, cp)


/* ----------------------------------------------------------------------------
* SHIORI API ŽÀ‘•
*/

/* ----------------------------------------------------------------------------
* žx Method / unload
*/
BOOL Module::unload(void)
{
    try{
        return true;
    }
    catch (...){
        return false;
    }
}

/* ----------------------------------------------------------------------------
* žx Method / load
*/
BOOL Module::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    USES_CONVERSION_EX;
    AutoGrobal ag1(hGlobal_loaddir);
    auto loaddir = G2CComBSTR_CP(hGlobal_loaddir, loaddir_len, cp);
    try{
        return true;
    }
    catch (...){
        return false;
    }
}

/* ----------------------------------------------------------------------------
* žx Method / request
*/
HGLOBAL Module::request(HGLOBAL hGlobal_request, long& len)
{
    USES_CONVERSION_EX;
    AutoGrobal ag1(hGlobal_request);
    auto req = G2CComBSTR_CP(hGlobal_request, len, cp);

    try{

        return NULL;
    }
    catch (...){
        return NULL;
    }
}

