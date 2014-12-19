#include "stdafx.h"
#include "ClrHost.h"

//-----------------------------------------------------------------------------
// HRESULTエラーで例外発行
inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

//-----------------------------------------------------------------------------
// HRESULTエラーで例外発行
inline void OK(BOOL const result)
{
    if (!result) AtlThrow(E_FAIL);
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
        AutoGrobal ag(hGlobal_loaddir);
        auto loaddir = g2CPathW(hGlobal_loaddir, loaddir_len, CP_ACP);

        // アセンブリ名⇒"NSLoader.dll"
        OK(loaddir.Append(_T("NSLoader.dll")));
        loaddir.Canonicalize();
        ATLTRACE2(_T("loaddir = [%s]\n"), (LPCTSTR)loaddir);
        OK(loaddir.FileExists());

        // ICLRMetaHostの取得
        CComPtr<ICLRMetaHostPolicy> metaHostPolicy;
        HR(CLRCreateInstance(CLSID_CLRMetaHostPolicy, IID_ICLRMetaHostPolicy, (LPVOID*)&metaHostPolicy));

        // ICLRRuntimeInfoの取得：読み込もうとするアセンブリに適したCLRを検索します。
        CComPtr<ICLRRuntimeInfo> clrInfo;
        CAtlStringW clrVersion;
        DWORD clrVersionLength = 32;
        HR(metaHostPolicy->GetRequestedRuntime(
            METAHOST_POLICY_HIGHCOMPAT, (LPCWSTR)loaddir, NULL,
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

        // Ghost通信用のAppDomainManagerクラスを登録します。
        LPCWSTR appDomainManagerTypename = L"ShioriAppDomainManager";
        LPCWSTR assemblyName = L"NSLoader";
        HR(clrCtrl->SetAppDomainManagerType(assemblyName, appDomainManagerTypename));

        // CLRを起動し、Ghost通信インターフェースを取得します。
        ATLTRACE2(_T("clr start >>>>>>>>>>>>>>>>\n"));
        HR(clr->Start());
        ATLTRACE2(_T("clr start <<<<<<<<<<<<<<<<\n"));

        // Ghostを取得します。
        ghost = hostCtrl->GetGhost();
        CComBSTR bloaddir(loaddir);
        VARIANT_BOOL rc;
        HR(ghost->load(bloaddir, &rc));
        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%d] <<<<<<<<\n"), ex.m_hr); }
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