#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <agents.h>

namespace shiori{
    struct Request{
    };

    struct Response{
    };

    typedef concurrency::unbounded_buffer<Request> RequestQueue;
    typedef concurrency::unbounded_buffer<Response> ResponseQueue;
}

/////////////////////////////////////////////////////////////////////////////
// WM ID

const UINT WM_GETSHIORI = WM_APP + 1;
const UINT WM_SHIORI_REQUEST = WM_APP + 2;
