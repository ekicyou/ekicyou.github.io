#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <agents.h>
#include "messages.h"

using namespace shiori;

class IEHostWindow :
    public CWindowImpl < IEHostWindow, CAxWindow >
{
public:
    static HANDLE CreateThread(
        HINSTANCE hinst, 
        BSTR loaddir, 
        RequestQueue &qreq, 
        ResponseQueue &qres,
        concurrency::single_assignment<IEHostWindow*> &lazyWin,
        DWORD &thid);

public:
    IEHostWindow(HINSTANCE hinst, BSTR loaddir, RequestQueue &qreq, ResponseQueue &qres);
    ~IEHostWindow();

private:
    HINSTANCE hinst;
    CComBSTR loaddir;
    RequestQueue &qreq;
    ResponseQueue &qres;

private:
    HANDLE hthread;
    DWORD thid;
};
