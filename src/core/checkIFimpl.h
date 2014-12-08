#pragma once

#include "checkIF.h"

template <class T>
inline void Check(IUnknown *src)
{
    CComQIPtr<T> check(src);

    if (check == NULL)return;
    auto result = check != NULL ? _T("Åõ") : _T("Å~");

    CComBSTR tpname(typeid(T).name());
    LPCTSTR name = tpname;
    AtlTrace(_T("  Å®%s [%s]\n"), result, name);
}


void CheckInterface1(IUnknown *src);
void CheckInterface2(IUnknown *src);
