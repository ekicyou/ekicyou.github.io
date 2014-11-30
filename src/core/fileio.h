#pragma once

#include <windows.h>
#include <string>

std::wstring readFile(LPCWSTR filename);
std::tr2::sys::wpath exePath();