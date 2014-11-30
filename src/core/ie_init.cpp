#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlsafe.h>

/////////////////////////////////////////////////////////////////////////////
// コントロール初期化

void IEHostWindow::Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres){
    this->hinst = hinst;
    this->loaddir = loaddir;
    this->qreq = &qreq;
    this->qres = &qres;

    this->hasRegKeyWrite = HasRegKeyWrite();
    if (this->hasRegKeyWrite)InitRegKey();
    InitWindow();
    InitIE();
}

void IEHostWindow::InitWindow(){
    // window作成
    Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    ResizeClient(320, 480);
}

#define SHOW_PASTA_SAN

void IEHostWindow::InitIE(){
    // IEコントロールの作成
    CComPtr<IUnknown> unknown;
    HR(CreateControlEx(_T("Shell.Explorer.2"), NULL, NULL, &unknown, IID_NULL, NULL));
    web2 = unknown;

    {
        CComQIPtr<IServiceProvider> sv = unknown;
    }


#ifndef SHOW_PASTA_SAN
    // 空ページの作成
    CComVariant	no_use, blank_url(_T("about:blank"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

    // ドキュメントファイルの読み込み
    CComPtr<IDispatch> disp;
    CComQIPtr<IHTMLDocument2> doc2;
    CComSafeArray<VARIANT> buf;
    auto path = loaddir;
    path /= L"index.html";

    auto htmlText = readFile(path.string().c_str());
    CComBSTR bHtml(htmlText.c_str());
    CComVariant vHtml(bHtml);
    HR(buf.Add(vHtml));
    HR(web2->get_Document(&disp));
    doc2 = disp;
    HR(doc2->clear());
    HR(doc2->writeln(buf));

#else
    // ぱすたさんの読み込み
    CComVariant	no_use, blank_url(_T("http://ekicyou.github.io/pasta/app/index.html"));
    //CComVariant	no_use, blank_url(_T("http://zakkiweb.net/tools/accessinfo/"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

#endif
}