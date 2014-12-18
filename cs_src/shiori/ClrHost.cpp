#include "stdafx.h"
#include "ClrHost.h"

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
* 栞 Method / unload
*/
BOOL ClrHost::unload(void)
{
    return TRUE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / load
*/
BOOL  ClrHost::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    return TRUE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / request
*/
HGLOBAL ClrHost::request(HGLOBAL hGlobal_request, long& len)
{
    return NULL;
}