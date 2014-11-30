#include "stdafx.h"
#include "module.h"
#include <atlstr.h>

#define HR(a) ATLENSURE_SUCCEEDED(a)

//-----------------------------------------------------------------------------
// 初期化・解放
Module::Module()
{
}

Module::~Module()
{
    unload();
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
// BSTR to HGLOBAL 文字列変換
inline HGLOBAL AllocString(CComBSTR& text, UINT cp, long &len)
{
    len = WideCharToMultiByte(
        cp,             // コードページ
        0,              // 処理速度とマッピング方法を決定するフラグ
        text,           // ワイド文字列のアドレス
        text.Length(),  // ワイド文字列の文字数
        NULL,           // 新しい文字列を受け取るバッファのアドレス
        0,              // 新しい文字列を受け取るバッファのサイズ
        NULL,           // マップできない文字の既定値のアドレス
        NULL            // 既定の文字を使ったときにセットするフラグのアドレス
        );

    HGLOBAL hText = GlobalAlloc(GMEM_FIXED, len);

    auto rc = WideCharToMultiByte(
        cp,             // コードページ
        0,              // 処理速度とマッピング方法を決定するフラグ
        text,           // ワイド文字列のアドレス
        text.Length(),  // ワイド文字列の文字数
        (LPSTR)hText,   // 新しい文字列を受け取るバッファのアドレス
        len,            // 新しい文字列を受け取るバッファのサイズ
        NULL,           // マップできない文字の既定値のアドレス
        NULL            // 既定の文字を使ったときにセットするフラグのアドレス
        );

    return hText;
}

//-----------------------------------------------------------------------------
// SHIORI unload
BOOL Module::unload(void)
{
    try{
        HR(core->unload());
        core.Release();
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
        core = shiori::CreateShiori();
        HR(core->load(hinst, loaddir));
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
        CComBSTR res;
        HR(core->request(req, &res));
        auto hres = AllocString(res, cp, len);
        return hres;
    }
    catch (...){
        return NULL;
    }
}