#include "stdafx.h"
#include "shiori_agent.h"

/////////////////////////////////////////////////////////////////////////////
// CShioriAgent

shiori::CShioriAgent::CShioriAgent()
{
}

shiori::CShioriAgent::~CShioriAgent()
{
}


/////////////////////////////////////////////////////////////////////////////
// IShiori

HRESULT STDMETHODCALLTYPE shiori::CShioriAgent::load(HINSTANCE hinst, BSTR loaddir){
    this->hinst = hinst;
    this->loaddir = loaddir;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE shiori::CShioriAgent::unload(){

    return S_OK;
}

HRESULT STDMETHODCALLTYPE shiori::CShioriAgent::request(BSTR breq, BSTR* bres){
    CComBSTR req(breq), res;
    try{



    }
    catch (...){

    }

    *bres = res.Detach();
    return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// agent

