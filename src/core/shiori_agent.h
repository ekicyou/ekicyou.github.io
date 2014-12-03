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
        : public CComObjectRootEx<CComSingleThreadModel>
        , public CComCoClass < CShiori, &CLSID_NULL >
        , public IShiori
    {
    public:
        BEGIN_COM_MAP(CShiori)
            COM_INTERFACE_ENTRY(IShiori)
        END_COM_MAP()

    public:
        CShiori();
        virtual ~CShiori();

    private:
        HINSTANCE hinst;
        CComBSTR loaddir;
        RequestQueue qreq;
        ResponseQueue qres;

        // IEWindowŠÖŒW
        single_assignment<::CIEHostWindow*> ieWin;
        CHandle ieThread;
        DWORD ieThid;

    public: // IShiori
        HRESULT STDMETHODCALLTYPE load(HINSTANCE hinst, BSTR loaddir)override;
        HRESULT STDMETHODCALLTYPE unload()override;
        HRESULT STDMETHODCALLTYPE request(BSTR req, BSTR* res) override;
    };
}
