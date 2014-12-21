#include "stdafx.h"
#include "ClrHost.h"

//-----------------------------------------------------------------------------
// HRESULTエラーで例外発行
inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

//-----------------------------------------------------------------------------
// TRUE以外で例外発行
inline void OK(BOOL const result)
{
    if (!result) AtlThrow(E_FAIL);
}

/* ----------------------------------------------------------------------------
* Win32エラーメッセージ取得
*/
CString GetWinErrMessage(const HRESULT hr)
{
    LPVOID string;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&string,
        0,
        NULL);
    CString rc;
    if (string != NULL) rc.Format(_T("[%x:%s]"), hr, string);
    else                rc.Format(_T("[%x:----]"), hr);
    LocalFree(string);
    return rc;
}

//-----------------------------------------------------------------------------
// ディレクトリ変更＆自動復帰
class Pushd
{
private:
    CString mOldDir;

public:
    Pushd(LPCTSTR newdir)
        :mOldDir()
    {
        TCHAR buf[_MAX_PATH + 1];
        ::GetCurrentDirectory(sizeof(buf), buf);
        mOldDir = buf;
        BOOL rc = ::SetCurrentDirectory(newdir);
        if (!rc) AtlThrow(ERROR_CURRENT_DIRECTORY);
    }

    ~Pushd()
    {
        if (mOldDir.IsEmpty()) return;
        ::SetCurrentDirectory(mOldDir);
    }
};

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
    auto rc = CComBSTR(wide);
    return rc;
}

//-----------------------------------------------------------------------------
// CP to CComBSTR 文字列変換
inline CPathW g2CPathW(HGLOBAL hg, long bytes, UINT cp){
    USES_CONVERSION;
    auto str = CAtlStringA((LPCSTR)hg, bytes);
    auto wide = A2CW_CP(str, cp);
    auto rc = CPathW(wide);
    return rc;
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
        // ディレクトリ名の取得
        AutoGrobal ag(hGlobal_loaddir);
        auto loaddir = g2CComBSTR(hGlobal_loaddir, loaddir_len, CP_ACP);
        ATLTRACE2(_T("         loaddir :[%s]\n"), (LPCTSTR)loaddir);
        //           "xxxxxxxxxxxxxxxx :[xx]\n"

        // 処理中のディレクトリを取得したディレクトリに切り替えておく
        Pushd pushd(loaddir);

        // アセンブリ名⇒"NSLoader.dll"
        // 絶対パス表記に変換しておく。
        TCHAR assemblyPath[_MAX_PATH + 1];
        TCHAR* assemblyFileName = nullptr;
        {
            CPath path(loaddir);
            OK(path.Append(_T("NSLoader.dll")));
            OK(path.FileExists());
            DWORD len = GetFullPathName(path, sizeof(assemblyPath) / sizeof(TCHAR), assemblyPath, &assemblyFileName);
            ATLTRACE2(_T("assemblyFileName :[%s]\n"), (LPCTSTR)assemblyFileName);
            ATLTRACE2(_T("    assemblyPath :[%s]\n"), (LPCTSTR)assemblyPath);
            //           "xxxxxxxxxxxxxxxx :[xx]\n"
        }

        // ICLRMetaHostの取得
        CComPtr<ICLRMetaHostPolicy> metaHostPolicy;
        HR(CLRCreateInstance(CLSID_CLRMetaHostPolicy, IID_ICLRMetaHostPolicy, (LPVOID*)&metaHostPolicy));

        // ICLRRuntimeInfoの取得：読み込もうとするアセンブリに適したCLRを検索します。
        CComPtr<ICLRRuntimeInfo> clrInfo;
        CAtlStringW clrVersion;
        DWORD clrVersionLength = 32;
        HR(metaHostPolicy->GetRequestedRuntime(
            METAHOST_POLICY_HIGHCOMPAT, assemblyPath, NULL,
            clrVersion.GetBufferSetLength(clrVersionLength), &clrVersionLength,
            NULL, NULL, NULL, IID_ICLRRuntimeInfo, (LPVOID*)&clrInfo));
        clrVersion.ReleaseBuffer();
        ATLTRACE2(_T("selection clr version = [%s]\n"), (LPCTSTR)clrVersion);

        // ICLRRuntimeHostの取得：CLRを読み込みます。
        HR(clrInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&clr));
        ICLRControl* clrCtrl;
        HR(clr->GetCLRControl(&clrCtrl));

        // Ghost通信用にNSHostControlを作成します。
        hostCtrl = new NSHostControl();
        HR(clr->SetHostControl(hostCtrl));

        // Ghost通信用のAppDomainManager実装[ShioriAppDomainManager]を登録します。
        LPCWSTR appDomainManagerTypename = L"NShiori.ShioriAppDomainManager";
        LPCWSTR assemblyName = L"NSLoader";
        HR(clrCtrl->SetAppDomainManagerType(assemblyName, appDomainManagerTypename));

        // CLRを起動し、Ghost通信インターフェースを取得します。
        HR(clr->Start());

        // Ghostを取得します。
        ghost = hostCtrl->GetGhost();
        CComBSTR bloaddir(loaddir);
        VARIANT_BOOL rc;
        HR(ghost->load(bloaddir, &rc));

        // load終了後にGhostを再取得します。
        // これが本体のGhostです。
        ghost = hostCtrl->GetGhost();

        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%s] <<<<<<<<\n"), (LPCTSTR)GetWinErrMessage(ex.m_hr)); }
    catch (...){}
    return FALSE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / unload
*/
BOOL ClrHost::unload(void)
{
    try{
        VARIANT_BOOL rc = FALSE;
        if (ghost)HR(ghost->unload(&rc));
        if (clr)HR(clr->Stop());
        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%d]\n"), ex.m_hr); }
    catch (...){}
    return FALSE;
}

/* ----------------------------------------------------------------------------
* 栞 Method / request
*/
HGLOBAL ClrHost::request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag(hGlobal_request);

    return NULL;
}