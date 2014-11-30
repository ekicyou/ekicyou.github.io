#include "stdafx.h"
#include "fileio.h"
#include <windows.h>
#include <sstream>
#include <fstream>
#include <codecvt>

std::wstring readFile(LPCWSTR filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

std::tr2::sys::wpath exePath(){
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::tr2::sys::wpath fullpath(buffer);
    return fullpath;
}