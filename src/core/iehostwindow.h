#pragma once

#include <windows.h>
#include "import.h"
#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <filesystem>
#include <agents.h>
#include <atlapp.h>
#include <atlcrack.h>
#include "messages.h"

using namespace shiori;

// TODO  IEのイベントを受信する
//       http://www.usefullcode.net/2009/03/receive_ie_event.html




// IEをホストするウィンドウ
class ATL_NO_VTABLE CIEHostWindow
    : public CComObject<CAxHostWindow>
    , public IDispEventImpl < SINKID_EVENTS, CIEHostWindow, &DIID_DWebBrowserEvents2 >
    , public IViewObjectPresentNotifySite
{
public:
    DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName());

    BEGIN_COM_MAP(CIEHostWindow)
        COM_INTERFACE_ENTRY(IViewObjectPresentSite)
        COM_INTERFACE_ENTRY(IViewObjectPresentNotifySite)
        COM_INTERFACE_ENTRY_CHAIN(CAxHostWindow)
    END_COM_MAP()

public:
    static HANDLE CreateThread(
        HINSTANCE hinst,
        BSTR loaddir,
        RequestQueue &qreq,
        ResponseQueue &qres,
        concurrency::single_assignment<CIEHostWindow*> &lazyWin,
        DWORD &thid);

    static DWORD WINAPI ThreadProc(LPVOID data);

public:
    CIEHostWindow();
    virtual ~CIEHostWindow();

    HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
        DWORD dwStyle = 0, DWORD dwExStyle = 0,
        _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);


private:
    void Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres);
    void InitWindow();
    void InitIE();
    bool HasRegKeyWrite();
    void InitRegKey();

private:
    HINSTANCE hinst;
    std::tr2::sys::wpath loaddir;
    RequestQueue *qreq;
    ResponseQueue *qres;

private:
    HANDLE hthread;
    DWORD thid;
    CComQIPtr<IWebBrowser2> web2;
    bool hasRegKeyWrite;

public:
    // インターフェース実装：IViewObjectPresentSite

    STDMETHOD(CreateSurfacePresenter)(
        /* [in] */ __RPC__in_opt IUnknown *pDevice,
        /* [in] */ UINT width,
        /* [in] */ UINT height,
        /* [in] */ UINT backBufferCount,
        /* [in] */ DXGI_FORMAT format,
        /* [in] */ VIEW_OBJECT_ALPHA_MODE mode,
        /* [out][retval] */ __RPC__deref_out_opt ISurfacePresenter **ppQueue) override;

    STDMETHOD(IsHardwareComposition)(
        /* [out][retval] */ __RPC__out BOOL *pIsHardwareComposition)override;

    STDMETHOD(SetCompositionMode)(
        /* [in] */ VIEW_OBJECT_COMPOSITION_MODE mode)override;

public:
    // インターフェース実装：IViewObjectPresentNotifySite

    STDMETHOD(RequestFrame)(void)override;



public:
    template <class Q>
    HRESULT QueryHost(Q** ppUnk)
    {
        return QueryHost(__uuidof(Q), (void**)ppUnk);
    }
    HRESULT QueryHost(REFIID iid, void** ppUnk)
    {
        ATLASSERT(ppUnk != NULL);
        if (ppUnk == NULL)
            return E_POINTER;
        HRESULT hr;
        *ppUnk = NULL;
        CComPtr<IUnknown> spUnk;
        hr = AtlAxGetHost(m_hWnd, &spUnk);
        if (SUCCEEDED(hr))
            hr = spUnk->QueryInterface(iid, ppUnk);
        return hr;
    }

public:
    // IEコントロールからのイベント通知
    BEGIN_SINK_MAP(CIEHostWindow)
        SINK_ENTRY_EX(SINKID_EVENTS, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
    END_SINK_MAP()

private:
    //HTMLページが読み終わったときに呼ばれる処理
    STDMETHOD(OnDocumentComplete)(IDispatch* pDisp, VARIANT* vURL);


private:
    class CReflectWnd : public CWindowImpl < CReflectWnd >
    {
        CIEHostWindow &win;
    public:
        CReflectWnd(CIEHostWindow& win) :win(win){};
        BEGIN_MSG_MAP(CReflectWnd)
            CHAIN_MSG_MAP_MEMBER(win)
        END_MSG_MAP()
    };

    CReflectWnd refWin;

    BEGIN_MSG_MAP(LayeredWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MESSAGE_HANDLER(WM_SHIORI_REQUEST, OnShioriRequest)
        CHAIN_MSG_MAP(CAxHostWindow)
    END_MSG_MAP()

    LRESULT OnDestroy();
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
    LRESULT OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};
