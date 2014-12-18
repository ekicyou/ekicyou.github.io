// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// ATL
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。
#include <atlbase.h>
#include <atlstr.h>

// STL
#include <memory>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <metahost.h>
#include <mscoree.h>
#include <corerror.h>
#include <comdef.h>
#pragma comment(lib, "mscoree.lib")

#import "../lib/Loader.tlb"

