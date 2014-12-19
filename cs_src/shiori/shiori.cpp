// ----------------------------------------------------------------------------
// SHIORI API
// ----------------------------------------------------------------------------
#include "stdafx.h"

#define SHIORI_API_IMPLEMENTS
#include "shiori.h"
#include "ClrHost.h"

/**----------------------------------------------------------------------------
* グローバルインスタンス
*/
static HINSTANCE hinst;
static CAutoPtr<ClrHost> host;

/**----------------------------------------------------------------------------
* Dllエントリーポイント
*/
extern "C" __declspec(dllexport) BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // DLL モジュールのハンドル
    DWORD fdwReason,     // 関数を呼び出す理由
    LPVOID lpvReserved   // 予約済み
    )
{
    switch (fdwReason) {
    case    DLL_PROCESS_ATTACH: // プロセス接続
        hinst = hinstDLL;
        break;

    case    DLL_PROCESS_DETACH: // プロセス切り離し
        unload();
        break;

    case    DLL_THREAD_ATTACH:  // スレッド接続
        break;

    case    DLL_THREAD_DETACH:  // スレッド切り離し
        break;
    }
    return true;
}

/* ----------------------------------------------------------------------------
* 栞 Method / unload
*/
SHIORI_API BOOL __cdecl unload(void)
{
    BOOL rc = FALSE;
    if (host) {
        rc = host->unload();
        host.Free();
    }
    return rc;
}

/* ----------------------------------------------------------------------------
* 栞 Method / load
*/
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    host.Attach(new ClrHost(hinst));
    return host->load(hGlobal_loaddir, loaddir_len);
}

/* ----------------------------------------------------------------------------
* 栞 Method / request
*/
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
    return host->request(hGlobal_request, len);
}

// EOF