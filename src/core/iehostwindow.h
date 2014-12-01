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

// TODO  IE�̃C�x���g����M����
//       http://www.usefullcode.net/2009/03/receive_ie_event.html

class IEHostWindow
    : public CWindowImpl < IEHostWindow, CAxWindow, CWinTraits<WS_OVERLAPPEDWINDOW> >
{
public:
    static HANDLE CreateThread(
        HINSTANCE hinst,
        BSTR loaddir,
        RequestQueue &qreq,
        ResponseQueue &qres,
        concurrency::single_assignment<IEHostWindow*> &lazyWin,
        DWORD &thid);

    static DWORD WINAPI ThreadProc(LPVOID data);

public:
    IEHostWindow();
    virtual ~IEHostWindow();

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
    DECLARE_WND_CLASS(_T("IEHostWindow"));

    BEGIN_MSG_MAP(LayeredWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MESSAGE_HANDLER(WM_SHIORI_REQUEST, OnShioriRequest)
    END_MSG_MAP()

private:
    LRESULT OnDestroy();
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);

    LRESULT OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};