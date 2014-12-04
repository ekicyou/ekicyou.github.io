#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"

struct ThreadParam{
    HINSTANCE hinst;
    BSTR loaddir;
    RequestQueue &qreq;
    ResponseQueue &qres;
    concurrency::single_assignment<CIEHostWindow*> &lazyWin;

    ThreadParam(
        HINSTANCE hinst,
        BSTR loaddir,
        RequestQueue &qreq,
        ResponseQueue &qres,
        concurrency::single_assignment<CIEHostWindow*> &lazyWin)
        : hinst(hinst)
        , loaddir(loaddir)
        , qreq(qreq)
        , qres(qres)
        , lazyWin(lazyWin){}
};



/////////////////////////////////////////////////////////////////////////////
// IE Thread

DWORD WINAPI CIEHostWindow::ThreadProc(LPVOID data){
    CAtlAutoThreadModule module;    // 魔法、スレッドに関するATLの初期化をしてくれる
    HR(::CoInitialize(NULL));
    OK(::AtlAxWinInit());
    {
        CIEHostWindow win;
        {
            CAutoPtr<ThreadParam> args((ThreadParam*)data);
            win.Init(args->hinst, args->loaddir, args->qreq, args->qres);
            concurrency::send(args->lazyWin, &win); // 作成したWindowを通知
        }

        // メッセージループ
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    OK(::AtlAxWinTerm());
    ::CoUninitialize();
    return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// 初期化・解放

HANDLE CIEHostWindow::CreateThread(
    HINSTANCE hinst,
    BSTR loaddir,
    RequestQueue &qreq,
    ResponseQueue &qres,
    concurrency::single_assignment<CIEHostWindow*> &lazyWin,
    DWORD &thid){
    auto args = new ThreadParam(hinst, loaddir, qreq, qres, lazyWin);
    return CRTThreadTraits::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)args, 0, &thid);
}

CIEHostWindow::CIEHostWindow()
    :refWin(*this)
{
}

CIEHostWindow::~CIEHostWindow(){
}

/////////////////////////////////////////////////////////////////////////////
// WM_CREATE
LRESULT CIEHostWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    return S_OK;
}

// WM_DESTROY
LRESULT CIEHostWindow::OnDestroy(){
    ::PostQuitMessage(1);
    return S_OK;
}

// WM_SHIORI_REQUEST
LRESULT CIEHostWindow::OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
    while (true){
        shiori::Request req;
        if (!concurrency::try_receive(qreq, req)) return S_OK;
    }
}