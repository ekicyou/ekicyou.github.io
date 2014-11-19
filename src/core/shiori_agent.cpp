#include "stdafx.h"
#include "shiori_agent.h"

/////////////////////////////////////////////////////////////////////////////
// CShioriAgent

CShioriAgent::CShioriAgent()
{
}

CShioriAgent::~CShioriAgent()
{
}


/////////////////////////////////////////////////////////////////////////////
// IShiori

HRESULT STDMETHODCALLTYPE CShioriAgent::load(BSTR bloaddir){
    loaddir = bloaddir;
    auto rc = start();


    return S_OK;
}

HRESULT STDMETHODCALLTYPE CShioriAgent::unload(){

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CShioriAgent::request(BSTR breq, BSTR* bres){
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

void CShioriAgent::run(){
}


