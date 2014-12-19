#include "stdafx.h"
#include "ClrHost.h"

//-----------------------------------------------------------------------------
// HRESULTエラーで例外発行
inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

//-----------------------------------------------------------------------------
// 自動開放
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
// CP to CComBSTR 文字列変換
inline CComBSTR g2CComBSTR(HGLOBAL hg, long bytes, UINT cp){
    USES_CONVERSION;
    auto str = CAtlStringA((LPCSTR)hg, bytes);
    auto wide = A2CW_CP(str, cp);
    auto bstr = CComBSTR(wide);
    return bstr;
}

//-----------------------------------------------------------------------------
// デストラクタ
ClrHost::~ClrHost()
{
}

//-----------------------------------------------------------------------------
// コンストラクタ
ClrHost::ClrHost(const HINSTANCE hinst)
    :hinst(hinst)
{
}

/* ----------------------------------------------------------------------------
* 栞 Method / load
*/
BOOL  ClrHost::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    try{
        AutoGrobal ag(hGlobal_loaddir);
        auto loaddir = g2CComBSTR(hGlobal_loaddir, loaddir_len, CP_ACP);

        // 

        return TRUE;
    }
    catch (CAtlException &ex){
        ATLTRACE2(_T("CAtlException hresult:[%d]"), ex.m_hr);
    }
    catch (...){
    }
    return FALSE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / unload
*/
BOOL ClrHost::unload(void)
{
    return TRUE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / request
*/
HGLOBAL ClrHost::request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag(hGlobal_request);

    return NULL;
}