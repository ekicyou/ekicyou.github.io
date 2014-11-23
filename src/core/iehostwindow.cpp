#include "stdafx.h"
#include "iehostwindow.h"

struct ThreadParam{
    HINSTANCE hinst;
    BSTR loaddir;
    RequestQueue &qreq;
    ResponseQueue &qres;
    concurrency::single_assignment<IEHostWindow*> &lazyWin;

    ThreadParam(
        HINSTANCE hinst,
        BSTR loaddir,
        RequestQueue &qreq,
        ResponseQueue &qres,
        concurrency::single_assignment<IEHostWindow*> &lazyWin)
        : hinst(hinst)
        , loaddir(loaddir)
        , qreq(qreq)
        , qres(qres)
        , lazyWin(lazyWin){}
};

/////////////////////////////////////////////////////////////////////////////
// IE Thread


static DWORD WINAPI IEThread(LPVOID data){
    CAtlAutoThreadModule module;    // 別スレッドでATLを動かすために必要
    CAutoPtr<ThreadParam> args((ThreadParam*)data);

    // window作成
    IEHostWindow win;
    win.Init(args->hinst, args->loaddir, args->qreq, args->qres);
    auto hwnd = win.Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    // 作成したWindowを通知する
    concurrency::send(args->lazyWin, &win);

    // メッセージループ
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// 初期化・解放

HANDLE IEHostWindow::CreateThread(
    HINSTANCE hinst,
    BSTR loaddir,
    RequestQueue &qreq,
    ResponseQueue &qres,
    concurrency::single_assignment<IEHostWindow*> &lazyWin,
    DWORD &thid){
    auto args = new ThreadParam(hinst, loaddir, qreq, qres, lazyWin);
    return Win32ThreadTraits::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEThread, (void*)args, 0, &thid);
}

void IEHostWindow::Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres){
    this->hinst = hinst;
    this->loaddir = loaddir;
    this->qreq = &qreq;
    this->qres = &qres;
}

IEHostWindow::IEHostWindow(){
}

IEHostWindow::~IEHostWindow(){
}

/////////////////////////////////////////////////////////////////////////////
// WM_CREATE
LRESULT IEHostWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    return S_OK;
}


// WM_DESTROY
LRESULT IEHostWindow::OnDestroy(){
    ::PostQuitMessage(1);
    return S_OK;
}

// WM_SHIORI_REQUEST
LRESULT IEHostWindow::OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

    return S_OK;
}
