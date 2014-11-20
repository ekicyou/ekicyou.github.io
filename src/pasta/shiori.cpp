// ----------------------------------------------------------------------------
// SHIORI API
// ----------------------------------------------------------------------------
#include "stdafx.h"

#define SHIORI_API_IMPLEMENTS
#include "shiori.h"
#include "module.h"

/* ----------------------------------------------------------------------------
* žx Method / unload
*/
SHIORI_API BOOL __cdecl unload(void)
{
    return module->unload();
}

/* ----------------------------------------------------------------------------
* žx Method / load
*/
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    return module->load(hGlobal_loaddir, loaddir_len);
}

/* ----------------------------------------------------------------------------
* žx Method / request
*/
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
    return module->request(hGlobal_request, len);
}

// EOF