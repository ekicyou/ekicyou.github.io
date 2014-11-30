#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlbase.h>

/////////////////////////////////////////////////////////////////////////////
// IEコンポーネントが最新バージョンで動くようにレジストリキーを設定

#define REG_ROOT    "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl"

// 指定KeyのDWORD値を返す。存在しない場合は０
static DWORD GetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName){
    CRegKey reg;
    if (reg.Open(root, key, KEY_READ) != ERROR_SUCCESS)return 0;
    DWORD rc;
    if (reg.QueryDWORDValue(key, rc) != ERROR_SUCCESS)return 0;
    return rc;
}

bool IEHostWindow::HasRegKeyWrite(){
    // exe名取得
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // レジストリルート
    CRegKey root;
    if (root.Open(HKEY_CURRENT_USER, _T(REG_ROOT), KEY_READ) != ERROR_SUCCESS)return true;

    // FEATURE_BROWSER_EMULATION >= 11000
    if (GetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName) >= 11000)return true;

    return false;
}

// EOF