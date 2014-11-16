// d2dwindow.cpp : ウィンドウ実装

#include "stdafx.h"
#include "ieutil.h"
#include "stdmethod.h"

#include <shlguid.h>
#include <shlobj.h>
#include <shlwapi.h>


HRESULT	ExecExplorer(IWebBrowser2* pIWebBrowser2, DWORD nCmdID, DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut)
{
    BEGIN_STDMETHOD_CODE;

    CComPtr<IShellBrowser> pIShellBrowser;
    if (pIWebBrowser2 == NULL) return	E_FAIL;
    HR(IUnknown_QueryService(pIWebBrowser2, SID_STopLevelBrowser, IID_IShellBrowser, (void**)&pIShellBrowser));
    return	S_OK;
    END_STDMETHOD_CODE;
}
