#pragma once

class NSHostControl
    : public IHostControl
{
public:
    NSHostControl() :refCount(0){}
    virtual ~NSHostControl(){}

    HRESULT STDMETHODCALLTYPE QueryInterface(const IID &iid, void **ppv)
    {
        if (!ppv) return E_POINTER;
        *ppv = this;
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement(&refCount);
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        if (InterlockedDecrement(&refCount) == 0)
        {
            delete this;
            return 0;
        }
        return refCount;
    }

public:
    HRESULT STDMETHODCALLTYPE GetHostManager(REFIID riid, void **ppv)
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

public:
    HRESULT STDMETHODCALLTYPE SetAppDomainManager(DWORD dwAppDomainID, IUnknown *pUnkAppDomainManager)
    {
        HRESULT hr = S_OK;
        UnkAppDomainManager = pUnkAppDomainManager;
        CComQIPtr<NSLoader::IShiori1> g(UnkAppDomainManager);
        if (!g) ghost = g;
        return hr;
    }

    CComPtr<NSLoader::IShiori1> GetGhost(){ return ghost; }

private:
    long refCount;
    CComPtr<IUnknown> UnkAppDomainManager;
    CComPtr<NSLoader::IShiori1> ghost;
};
