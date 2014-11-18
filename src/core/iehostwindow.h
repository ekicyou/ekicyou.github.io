#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>


class IEHostWindow :
    public CWindowImpl< IEHostWindow, CAxWindow >
{
public:
    IEHostWindow();
    ~IEHostWindow();
};

