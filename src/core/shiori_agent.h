#pragma once

#include <agents.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include "interfaces.h"

using namespace concurrency;

namespace shiori{
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

    public: // IShiori
        HRESULT STDMETHODCALLTYPE load(HINSTANCE hinst, BSTR loaddir)override;
        HRESULT STDMETHODCALLTYPE unload()override;
        HRESULT STDMETHODCALLTYPE request(BSTR req, BSTR* res) override;
    };
}
