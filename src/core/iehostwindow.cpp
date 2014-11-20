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
    CAutoPtr<ThreadParam> args((ThreadParam*)data);

    // window作成
    auto win = new IEHostWindow(
        args->hinst,
        args->loaddir,
        args->qreq,
        args->qres);

    // 作成したWindowを登録する
    concurrency::send(args->lazyWin, win);

    // メッセージループ

    return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// 初期化

HANDLE IEHostWindow::CreateThread(
    HINSTANCE hinst,
    BSTR loaddir,
    RequestQueue &qreq,
    ResponseQueue &qres,
    concurrency::single_assignment<IEHostWindow*> &lazyWin,
    DWORD &thid){
    auto args = new ThreadParam(hinst, loaddir, qreq, qres, lazyWin);
    return CRTThreadTraits::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEThread, (void*)args, 0, &thid);
}

IEHostWindow::IEHostWindow(HINSTANCE hinst, BSTR loaddir, RequestQueue &qreq, ResponseQueue &qres)
    : hinst(hinst)
    , loaddir(loaddir)
    , qreq(qreq)
    , qres(qres)
{
}

/////////////////////////////////////////////////////////////////////////////
// 解放

IEHostWindow::~IEHostWindow()
{
}