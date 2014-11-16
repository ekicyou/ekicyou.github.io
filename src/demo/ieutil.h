#pragma once

#include <windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <atlhost.h>
#include "import.h"

namespace ie{

    HRESULT	ExecExplorer(IWebBrowser2* pIWebBrowser2, DWORD nCmdID, DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut);

}

// EOF