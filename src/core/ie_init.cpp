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

    // window作成
    Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    // IEコントロールの作成[
    RECT					rect;
    CComPtr<IUnknown>		unknown;

    OK(GetClientRect(&rect));

    const auto wndClass = CAxWindow::GetWndClassName();
    const auto winStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE;
    //const auto exStyle = WS_EX_NOREDIRECTIONBITMAP;
    const auto exStyle = WS_EX_TRANSPARENT;

    auto hwnd = CreateWindowEx(exStyle, wndClass, _T("Shell.Explorer.2"), winStyle,
        rect.left, rect.top,
        abs(rect.right - rect.left),
        abs(rect.bottom - rect.top),
        m_hWnd, 0,
        (HINSTANCE)GetWindowLong(GWL_HINSTANCE),
        0
        );
    OK(hwnd != NULL);
    HR(AtlAxGetControl(hwnd, &unknown));
    web2 = unknown;

    // 続きはOnInit2で
    this->PostMessageW(WM_IEWIN_INIT2);
}

LRESULT IEHostWindow::OnInit2(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
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
    return S_OK;
}
