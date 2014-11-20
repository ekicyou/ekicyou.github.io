// dllmain.cpp : DLL エントリポイント
#include "stdafx.h"
#include "module.h"

/**----------------------------------------------------------------------------
* グローバルインスタンス
*/

std::unique_ptr<Module> module;

/**----------------------------------------------------------------------------
* エントリポイント
*/

BOOL APIENTRY DllMain(
    HMODULE hinstDLL,
    DWORD  fdwReason,
    LPVOID lpvReserved)
{
    if (!module){
        module.reset(new Module());
        module->hinst = hinstDLL;
    }
    auto rc = module->DllMain(fdwReason, lpvReserved);
    switch (fdwReason)
    {
    case DLL_PROCESS_DETACH:
        module.release();
        break;
    }
    return rc;
}

STDAPI DllCanUnloadNow(void){
    return module->DllCanUnloadNow();
}

STDAPI DllGetClassObject(
    const CLSID & rclsid,
    const IID & riid,
    void ** ppv){
    return module->DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
    return module->DllRegisterServer();
}

STDAPI DllUnregisterServer(void)
{
    return module->DllUnregisterServer();
}