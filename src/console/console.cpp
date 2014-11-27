// console.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "shiori.h"
#include <string>
#include <filesystem>
#include <atlconv.h>

using namespace std;
using namespace std::tr2::sys;

// std::string → HGLOBAL
inline HGLOBAL AllocString(const string& test, long& len)
{
    HGLOBAL hText = GlobalAlloc(GMEM_FIXED, test.length());
    CopyMemory(hText, test.data(), test.length());
    len = (long)test.length();
    return hText;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // shiori load
    {
        USES_CONVERSION;
        wpath exe_path(argv[0]);
        auto dir_path = exe_path.branch_path();
        string s_dir_path(W2A(dir_path.string().c_str()));
        long len = 0;
        auto g_current = AllocString(s_dir_path, len);
        auto rc = load(g_current, len);
    }

    // 待機
    puts("何かキーを押せば終了します。");
    getchar();

    // shiori unload
    {
        auto rc = unload();
    }

    return 0;
}