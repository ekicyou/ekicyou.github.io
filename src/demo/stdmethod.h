#pragma once
#include <windows.h>
#include <atldef.h>

inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

// AtlExceptionをキャッチしてHRESULT をreturnするコードブロックを始めます。
#define BEGIN_STDMETHOD_CODE try{

// AtlExceptionをキャッチしてHRESULT をreturnするコードブロックを終わります。
#define END_STDMETHOD_CODE }\
    catch(CAtlException &ex){ return ex.m_hr;} \
    catch(...){return E_FAIL;}

