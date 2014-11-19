#pragma once

#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>

/////////////////////////////////////////////////////////////////////////////
// IShiori

MIDL_INTERFACE("C097D9E2-551C-49B0-B3A8-EACF306C5173")
IShiori : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE load(
        /* [in] */ BSTR loaddir) = 0;

    virtual HRESULT STDMETHODCALLTYPE unload() = 0;

    virtual HRESULT STDMETHODCALLTYPE request(
        /* [in] */ BSTR  req,
        /* [Out]*/ BSTR* res) = 0;
};

typedef CComQIPtr<IShiori> CShioriPtr;

const UINT WM_GETSHIORI = WM_APP + 1;
