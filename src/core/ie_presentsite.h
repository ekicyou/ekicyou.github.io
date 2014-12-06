#pragma once

#include "import.h"
#include <atlhost.h>
#include <atlbase.h>
#include <dx.h>         // DX http://dx.codeplex.com
#include <dcomp.h>
#pragma comment(lib, "dcomp")

//////////////////////////////////////////////////////////////////////////////
// IViewObjectPresentNotifySiteImpl
//

template <class T>
class ATL_NO_VTABLE IViewObjectPresentNotifySiteImpl
    : public IViewObjectPresentNotifySite
{
public:
    virtual ~IViewObjectPresentNotifySiteImpl()
    {
    }
    VIEW_OBJECT_COMPOSITION_MODE m_compositionMode;

private:

    class ISurfacePresenterImpl
        : public CComObjectRootEx < CComSingleThreadModel >
        , public ISurfacePresenter
    {
    public:
        BEGIN_COM_MAP(ISurfacePresenterImpl)
            COM_INTERFACE_ENTRY(ISurfacePresenter)
        END_COM_MAP()

    public:
        typedef IViewObjectPresentNotifySiteImpl<T> Site;

        Site* site;
        HWND m_hWnd;
        DXGI_SWAP_CHAIN_DESC1 description = {};
        CComQIPtr<ID3D11Device> direct3dDevice;
        CComQIPtr<IDXGIDevice> dxgiDevice;
        CComPtr<IDXGISwapChain1> swapChain;
        CComPtr<IDCompositionDevice> dcompDevice;
        CComPtr<IDCompositionTarget> target;
        CComPtr<IDCompositionVisual> visual;

        virtual ~ISurfacePresenterImpl(){}

        HRESULT Init(
            Site *site,
            /* [in] */ __RPC__in_opt IUnknown *pDevice,
            /* [in] */ UINT width,
            /* [in] */ UINT height,
            /* [in] */ UINT backBufferCount,
            /* [in] */ DXGI_FORMAT format,
            /* [in] */ VIEW_OBJECT_ALPHA_MODE mode
            )
        {
            this->site = site;
            auto win = static_cast<T*>(site);
            m_hWnd = win->m_hWnd;
            HRESULT hr;

            // ＜＜ DirectCompositionによるスワップ チェーン 合成＞＞
            // この辺の処理はここ参考
            //http://msdn.microsoft.com/ja-jp/magazine/dn745861.aspx

            // D3Dデバイス取得
            direct3dDevice = pDevice;
            // デバイス無ければなければ作っていい？
            if (direct3dDevice == NULL){
                hr = D3D11CreateDevice(nullptr,    // Adapter
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,    // Module
                    D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                    nullptr, 0, // Highest available feature level
                    D3D11_SDK_VERSION,
                    &direct3dDevice,
                    nullptr,    // Actual feature level
                    nullptr);   // Device context
                if (FAILED(hr))return hr;
                if (direct3dDevice == NULL)return E_FAIL;
            }

            // D3DデバイスをDXGIデバイスに関連付ける
            // (D2Dとの相互運用性確保)
            dxgiDevice = direct3dDevice;
            if (dxgiDevice == NULL)return E_FAIL;

            // スワップチェーンファクトリの取得
            CComPtr<IDXGIFactory2> dxFactory;
            hr = CreateDXGIFactory2(
                DXGI_CREATE_FACTORY_DEBUG,
                __uuidof(dxFactory),
                reinterpret_cast<void **>(&dxFactory));
            if (FAILED(hr))return hr;
            if (dxFactory == NULL)return E_FAIL;

            // スワップチェーン情報：基本設定
            description.Width = width;
            description.Height = height;
            description.BufferCount = backBufferCount;
            description.Format = format;
            description.Width = width;

            description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            description.SampleDesc.Count = 1;

            // スワップチェーン情報：AlphaMode
            switch (mode)
            {
            case VIEW_OBJECT_ALPHA_MODE_IGNORE:
                description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
                break;
            case VIEW_OBJECT_ALPHA_MODE_PREMULTIPLIED:
                description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
                break;
            default:
                description.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
                break;
            }

            // スワップチェーンの作成
            hr = dxFactory->CreateSwapChainForComposition(
                dxgiDevice,
                &description,
                nullptr, // Don’t restrict
                &swapChain);
            if (FAILED(hr))return hr;
            if (swapChain == NULL)return E_FAIL;

            // DirectComposition デバイスの作成
            hr = DCompositionCreateDevice(
                dxgiDevice,
                __uuidof(dcompDevice),
                reinterpret_cast<void **>(&dcompDevice));
            if (FAILED(hr))return hr;
            if (dcompDevice == NULL)return E_FAIL;

            // 合成ターゲット(hwnd)
            hr = dcompDevice->CreateTargetForHwnd(
                m_hWnd,
                true, // Top most
                &target);
            if (FAILED(hr))return hr;
            if (target == NULL)return E_FAIL;

            // visualにスワップチェーンを関連付ける
            hr = dcompDevice->CreateVisual(&visual);
            if (FAILED(hr))return hr;
            if (visual == NULL)return E_FAIL;

            hr = visual->SetContent(swapChain);     // ビジュアルにスワップチェーンに関連付ける
            if (FAILED(hr))return hr;
            hr = target->SetRoot(visual);           // 合成ターゲットのルートとして設定
            if (FAILED(hr))return hr;
            hr = dcompDevice->Commit();             // 合成エンジンに完了を通知
            if (FAILED(hr))return hr;

            return S_OK;
        }

        STDMETHOD(Present)(
            /* [in] */ UINT uBuffer,
            /* [in] */ __RPC__in RECT *pDirty)
        {
            // TODO: 暫定。引数は反映しなくていい？
            auto hr = swapChain->Present(
                1,   // sync
                0); // flags
            return hr;
        }

        STDMETHOD(GetBuffer)(
            /* [in] */ UINT backBufferIndex,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out][retval] */ __RPC__deref_out_opt void **ppBuffer)
        {
            auto hr = swapChain->GetBuffer(backBufferIndex, riid, ppBuffer);
            return hr;
        }

        STDMETHOD(IsCurrent)(
            /* [out][retval] */ __RPC__out BOOL *pIsCurrent)
        {
            auto hr = direct3dDevice->GetDeviceRemovedReason();
            *pIsCurrent = SUCCEEDED(hr) ? TRUE : FALSE;
            return S_OK;
        }
    };

public:
    // インターフェース実装：IViewObjectPresentSite

    // ISurfacePresenterの作成
    STDMETHOD(CreateSurfacePresenter)(
        /* [in] */ __RPC__in_opt IUnknown *pDevice,
        /* [in] */ UINT width,
        /* [in] */ UINT height,
        /* [in] */ UINT backBufferCount,
        /* [in] */ DXGI_FORMAT format,
        /* [in] */ VIEW_OBJECT_ALPHA_MODE mode,
        /* [out][retval] */ __RPC__deref_out_opt ISurfacePresenter **ppQueue)
    {
        auto sp = new CComObject < ISurfacePresenterImpl >();
        auto hr = sp->Init(
            this, pDevice, width, height, backBufferCount, format, mode);
        if (FAILED(hr)){
            delete sp;
            return hr;
        }
        *ppQueue = sp;
        sp->AddRef();
        return S_OK;
    }

    STDMETHOD(IsHardwareComposition)(
        /* [out][retval] */ __RPC__out BOOL *pIsHardwareComposition)
    {
        *pIsHardwareComposition = TRUE;
        return S_OK;
    }

    STDMETHOD(SetCompositionMode)(
        /* [in] */ VIEW_OBJECT_COMPOSITION_MODE mode)
    {
        m_compositionMode = mode;
        return S_OK;
    }

public:
    // インターフェース実装：IViewObjectPresentNotifySite

    STDMETHOD(RequestFrame)(void)
    {
        // 暫定、何もしていない
        return S_OK;
    }
};
