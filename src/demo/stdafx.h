// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。

// 基本ライブラリ
#include <dx.h>         // DX http://dx.codeplex.com
#include <comdef.h>     // コンパイラCOMサポート
#include <atlbase.h>    // ATL BASE
#include <atlhost.h>    // ATL HOST
#include <atlsafe.h>	// ATL SAFEARRAY

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// stl ヘッダー ファイル
#include <memory>


// EOF