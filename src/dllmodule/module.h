#pragma once
#include <windows.h>
#include <atlbase.h>
#include <memory>

class Module :public CAtlDllModuleT< Module >
{
public:
    Module();
    ~Module();

public:
    HINSTANCE hinst;

};

extern std::unique_ptr<Module> module;


// EOF