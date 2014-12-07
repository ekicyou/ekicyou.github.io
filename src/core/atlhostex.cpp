#include "stdafx.h"
#include "atlhostex.h"
#include "iehostwindow.h"
#include "stdmethod.h"

/////////////////////////////////////////////////////////////////////////////

// Axコントロールの作成（＆ホスト作成）
HRESULT IEHostWindow::CreateControlEx2(
    _In_z_ LPCOLESTR lpszName,
    _Inout_opt_ IStream* pStream,
    _Outptr_opt_ IUnknown** ppUnkContainer,
    _Outptr_opt_ IUnknown** ppUnkControl,
    _In_ REFIID iidSink,
    _Inout_opt_ IUnknown* punkSink)
{
    ATLASSERT(::IsWindow(m_hWnd));
    // We must have a valid window!

    // Get a pointer to the container object connected to this window
    CComPtr<IAxWinHostWindow> spWinHost;
    HRESULT hr = QueryHost(&spWinHost);

    // If QueryHost failed, there is no host attached to this window
    // We assume that the user wants to create a new host and subclass the current window
    if (FAILED(hr)){
        hr = CAxHostWindowEX::AxCreateControlEx(
            lpszName, m_hWnd, pStream, ppUnkContainer, ppUnkControl, iidSink, punkSink);
        if (FAILED(hr))return hr;


        return hr;
    }

    // Create the control requested by the caller
    CComPtr<IUnknown> pControl;
    if (SUCCEEDED(hr))
        hr = spWinHost->CreateControlEx(
        lpszName, m_hWnd, pStream, &pControl, iidSink, punkSink);

    // Send back the necessary interface pointers
    if (SUCCEEDED(hr))
    {
        if (ppUnkControl)
            *ppUnkControl = pControl.Detach();

        if (ppUnkContainer)
        {
            hr = spWinHost.QueryInterface(ppUnkContainer);
            ATLASSERT(SUCCEEDED(hr)); // This should not fail!
        }
    }

    return hr;
}

// EOF