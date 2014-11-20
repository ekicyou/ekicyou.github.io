#include "stdafx.h"
#include "shiori_agent.h"

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

HRESULT STDMETHODCALLTYPE shiori::CShiori::load(HINSTANCE hinst, BSTR loaddir){
    try{
        this->hinst = hinst;
        this->loaddir = loaddir;
        return S_OK;
    }
    catch (...){
    }
    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE shiori::CShiori::unload(){
    try{
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