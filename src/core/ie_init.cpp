#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include "checkIF.h"
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
    auto rect = CWindow::rcDefault;
    auto szWindowName = _T("IEWindow");
    auto dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    auto dwExStyle = WS_EX_OVERLAPPEDWINDOW;
    //auto dwExStyle =  WS_EX_NOREDIRECTIONBITMAP;

    Create(NULL, rect, szWindowName, dwStyle, dwExStyle);
    ResizeClient(320, 480);
}

//#define SHOW_PASTA_SAN

void IEHostWindow::InitIE(){

#ifdef xxxxxxxx

    // IEコントロールの作成
    CComPtr<IUnknown> unknown, uhost;
    HR(CreateControlEx2(_T("Shell.Explorer.2"), NULL, &uhost, &unknown, IID_NULL, NULL));
    web2 = unknown;

#ifndef SHOW_PASTA_SAN
    // 空ページの作成
    CComVariant	no_use, blank_url(_T("about:blank"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));
    HR(web2->get_Document(&disp));
    doc2 = disp;

#else
    // ぱすたさんの読み込み
    CComVariant	no_use, blank_url(_T("http://ekicyou.github.io/pasta/app/index.html"));
    //CComVariant	no_use, blank_url(_T("http://zakkiweb.net/tools/accessinfo/"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

#endif


#endif
    // htmlfile_FullWindowEmbedの作成
    CComPtr<IUnknown> unknown, uhost;
#ifdef xxxxxxxx
    // htmlfile
    HR(CreateControlEx2(_T("htmlfile"), NULL, &uhost, &unknown, IID_NULL, NULL));
    // Microsoft HTML DwnBindInfo
    HR(CreateControlEx2(_T("{3050F3C2-98B5-11CF-BB82-00AA00BDCE0B}"), NULL, &uhost, &unknown, IID_NULL, NULL));
    // Microsoft Html Component
    HR(CreateControlEx2(_T("{3050f4f8-98b5-11cf-bb82-00aa00bdce0b}"), NULL, &uhost, &unknown, IID_NULL, NULL));
#endif
    // htmlfile_FullWindowEmbed
    HR(CreateControlEx2(_T("htmlfile_FullWindowEmbed"), NULL, &uhost, &unknown, IID_NULL, NULL));


    doc2 = unknown;

    // ドキュメントファイルの読み込み
    CComPtr<IDispatch> disp;
    CComSafeArray<VARIANT> buf;
    auto path = loaddir;
    path /= L"index.html";
    auto htmlText = readFile(path.string().c_str());
    CComBSTR bHtml(htmlText.c_str());
    CComVariant vHtml(bHtml);
    HR(buf.Add(vHtml));
    HR(doc2->clear());
    HR(doc2->writeln(buf));

    {
        // どうにもならない試みであるが同じことを繰り返さないために残す
        ::CheckInterface(unknown, _T("unknown"));
        ::CheckInterface(doc2, _T("doc2"));

        CComQIPtr<IOleObject> ole = unknown;
        CComPtr<IOleClientSite> site;
        ole->GetClientSite(&site);
        ::CheckInterface(site, _T("site"));

        CComQIPtr<IViewObjectEx> vex = unknown;
        DWORD status;
        HR(vex->GetViewStatus(&status));
        AtlTrace(_T("doc2->[IViewObjectEx::GetViewStatus] %x"), status);
    }
}