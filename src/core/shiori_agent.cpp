#include "stdafx.h"
#include "shiori_agent.h"

#define HR(a) ATLENSURE_SUCCEEDED(a)

/////////////////////////////////////////////////////////////////////////////
// CShiori

shiori::CShiori::CShiori()
{
}

shiori::CShiori::~CShiori()
{
}

/////////////////////////////////////////////////////////////////////////////
// IShiori

IShiori* shiori::CreateShiori(){
    CComObject<CShiori>* impl = nullptr;
    HR(CComObject < CShiori >::CreateInstance(&impl));
    return impl;
}


HRESULT STDMETHODCALLTYPE shiori::CShiori::load(HINSTANCE hinst, BSTR loaddir){
    try{
        this->hinst = hinst;
        this->loaddir = loaddir;

        // IEThreadの立ち上げ
        ieThread.Attach(IEHostWindow::CreateThread(hinst, loaddir, qreq, qres, ieWin, ieThid));

        return S_OK;
    }
    catch (...){
    }
    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE shiori::CShiori::unload(){
    try{
        // IEWindowのクローズ
        auto win = ieWin.value();
        win->SendMessageW(WM_CLOSE);
        // クローズ（スレッド終了）を待つ
        DWORD code;
        ::GetExitCodeThread(ieThread, &code);
        return S_OK;
    }
    catch (...){
    }
    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE shiori::CShiori::request(BSTR breq, BSTR* bres){
    CComBSTR req(breq), res;
    try{
        *bres = res.Detach();
        return S_OK;
    }
    catch (...){
    }

    *bres = res.Detach();
    return E_FAIL;
}

/////////////////////////////////////////////////////////////////////////////
// agent