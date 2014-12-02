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

#define SINKID_EVENTS 0

using namespace shiori;

// TODO  IEのイベントを受信する
//       http://www.usefullcode.net/2009/03/receive_ie_event.html

class IEHostWindow
    : public CComObject<CAxHostWindow>
    , public IDispEventImpl < SINKID_EVENTS, IEHostWindow, &DIID_DWebBrowserEvents2 >
{
    class CReflectWnd : public CWindowImpl<CReflectWnd>
    {
        IEHostWindow* win;
    public:
        CReflectWnd(IEHostWindow* win)
        {
            this->win = win;
        }
        BEGIN_MSG_MAP(CReflectWnd)
            CHAIN_MSG_MAP_MEMBER((*win))
        END_MSG_MAP()
    };

    CReflectWnd refWin;

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

    HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
        DWORD dwStyle = 0, DWORD dwExStyle = 0,
        _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

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
        CHAIN_MSG_MAP(CAxHostWindow)
    END_MSG_MAP()

private:
    LRESULT OnDestroy();
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);

    LRESULT OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
