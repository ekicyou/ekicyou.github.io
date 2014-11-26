#pragma once

#include <windows.h>
#include <agents.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include "interfaces.h"
#include "shiori_messages.h"

using namespace concurrency;

namespace shiori{

    class ATL_NO_VTABLE CShioriAgent
        : public CComObjectRoot
        , public CComCoClass<CShioriAgent, &CLSID_NULL>
        , public IShiori
    {
    public:
        CShioriAgent();
        virtual ~CShioriAgent();

    private:
        HINSTANCE hinst;
        CComBSTR loaddir;
        unbounded_buffer<Request> reqBuf;
        unbounded_buffer<Response> resBuf;

    public: // IShiori
        HRESULT STDMETHODCALLTYPE load(HINSTANCE hinst, BSTR loaddir)override;
        HRESULT STDMETHODCALLTYPE unload()override;
        HRESULT STDMETHODCALLTYPE request(BSTR req, BSTR* res)override;
    };
}
