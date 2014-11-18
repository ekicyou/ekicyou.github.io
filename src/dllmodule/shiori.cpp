// ----------------------------------------------------------------------------
// SHIORI API
// ----------------------------------------------------------------------------
#include "stdafx.h"

#define SHIORI_API_IMPLEMENTS
#include "shiori.h"

#include "module.h"

/**----------------------------------------------------------------------------
* HGLOBALŠÖŒW
*/
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


/* ----------------------------------------------------------------------------
* žx Method / unload
*/
SHIORI_API BOOL __cdecl unload(void)
{
    return true;
}

/* ----------------------------------------------------------------------------
* žx Method / load
*/
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    AutoGrobal ag1(hGlobal_loaddir);
    return true;
}

/* ----------------------------------------------------------------------------
* žx Method / request
*/
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag1(hGlobal_request);
    return NULL;
}


// EOF