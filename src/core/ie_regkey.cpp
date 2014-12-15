#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlbase.h>
#include <urlmon.h>

/////////////////////////////////////////////////////////////////////////////
// IEコンポーネントが最新バージョンで動くようにレジストリキーを設定

// 指定KeyのDWORD値を取得する。存在しない場合は０
static DWORD GetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName){
    CRegKey reg;
    if (reg.Open(root, key, KEY_READ) != ERROR_SUCCESS)return 0;
    DWORD rc = 0;
    if (reg.QueryDWORDValue(valueName, rc) != ERROR_SUCCESS)return 0;
    return rc;
}

// 指定KeyのDWORD値を設定する。
static void SetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName, const DWORD value){
    CRegKey reg;
    OK(reg.Create(root, key));
    OK(reg.SetDWORDValue(valueName, value));
    reg.Flush();
}

/////////////////////////////////////////////////////////////////////////////
// IEコンポーネントが最新バージョンで動くようにレジストリキーを設定

#define REG_ROOT    "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl"

// レジストリの設定が必要かどうかを確認する
bool IEHostWindow::HasRegKeyWrite(){
    // exe名取得
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // レジストリルート
    CRegKey root;
    if (root.Open(HKEY_CURRENT_USER, _T(REG_ROOT), KEY_READ) != ERROR_SUCCESS)return true;

    // FEATURE_BROWSER_EMULATION >= 11001
    if (GetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName) < 11001)return true;

    // FEATURE_GPU_RENDERING >= 1
    if (GetDWORD(root, _T("FEATURE_GPU_RENDERING"), valueName) < 1)return true;

    return false;
}

/////////////////////////////////////////////////////////////////////////////
// レジストリの設定

void IEHostWindow::InitRegKey(){
    // exe名取得
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // レジストリルート
    CRegKey root;
    OK(root.Create(HKEY_CURRENT_USER, _T(REG_ROOT)));

    // FEATURE_BROWSER_EMULATION >= 11001
    SetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName, 11001);

    // FEATURE_GPU_RENDERING >= 1
    SetDWORD(root, _T("FEATURE_GPU_RENDERING"), valueName, 1);
}

// EOF