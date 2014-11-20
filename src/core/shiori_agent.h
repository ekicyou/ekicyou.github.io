#pragma once

#include <agents.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include "interfaces.h"
#include "messages.h"
#include "iehostwindow.h"

namespace shiori{
    using namespace concurrency;

    class ATL_NO_VTABLE CShiori
        : public CComObjectRoot
        , public CComCoClass < CShiori, &CLSID_NULL >
        , public IShiori
    {
    public:
        CShiori();
        virtual ~CShiori();

    private:
        HINSTANCE hinst;
        CComBSTR loaddir;
        RequestQueue qreq;
        ResponseQueue qres;
        single_assignment<::IEHostWindow*> lazyWin;

    public: // IShiori
        HRESULT STDMETHODCALLTYPE load(HINSTANCE hinst, BSTR loaddir)override;
        HRESULT STDMETHODCALLTYPE unload()override;
        HRESULT STDMETHODCALLTYPE request(BSTR req, BSTR* res) override;
    };
}
