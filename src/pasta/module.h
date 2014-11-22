#pragma once
#include <windows.h>
#include <atlbase.h>
#include <memory>
#include "interfaces.h"

class Module :public CAtlDllModuleT < Module >
{
public:
    Module();
    ~Module();

public:
    HINSTANCE hinst;
    UINT cp = CP_UTF8;
    CComQIPtr<IShiori> core;

public:
    // SHIORI API
    BOOL Module::unload(void);
    BOOL Module::load(HGLOBAL hGlobal_loaddir, long loaddir_len);
    HGLOBAL Module::request(HGLOBAL hGlobal_request, long& len);
};

extern std::unique_ptr<Module> module;

// EOF