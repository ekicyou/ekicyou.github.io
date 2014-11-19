#pragma once

#include <agents.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include "interfaces.h"

using namespace concurrency;

class ATL_NO_VTABLE CShioriAgent
    : public CComObjectRoot
    , public CComCoClass<CShioriAgent, &CLSID_NULL>
    , public IShiori
    , public agent
{
public:
    CShioriAgent();
    virtual ~CShioriAgent();

private:
    CComBSTR loaddir;


protected:
    void run() override;

public: // IShiori
    HRESULT STDMETHODCALLTYPE load   (BSTR loaddir        )override;
    HRESULT STDMETHODCALLTYPE unload (                    )override;
    HRESULT STDMETHODCALLTYPE request(BSTR req, BSTR* res) override;


};

