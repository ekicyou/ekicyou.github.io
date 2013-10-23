/* ----------------------------------------------------------------------------
 * tunak_defi.h
 *   つなき・構造体定義.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_def.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_DEF_H__
#define TUNAKI_DEF_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include <windows.h>


/* ----------------------------------------------------------------------------
 * 定数宣言
 */
#define VERSION_VALUE (0x0101)
#define INI_FILE_NAME "tunaki.ini"
#define LOG_FILE_NAME "tunaki.log"
#define INI_SECTION   "TUNAKI"
#define NULL_ANSISTR {0,0,0}


/* ----------------------------------------------------------------------------
 * 型宣言
 */

/* ANSI文字列管理構造体 */
typedef struct _ANSISTR{
  SIZE_T size;    // heapのサイズ
  SIZE_T length;  // 文字列長(NULLは含まない)
  LPTSTR s;       // 文字列へのポインタ
} ANSISTR, *PANSISTR, *LPANSISTR;


/* WIDE文字列管理構造体 */
typedef struct _WIDESTR{
  SIZE_T size;    // heapのサイズ
  SIZE_T length;  // 文字列長(NULLは含まない)
  LPWSTR s;       // 文字列へのポインタ
} WIDESTR, *PWIDESTR, *LPWIDESTR;

/* つなき構造体 */
typedef struct _TUNAKI{
    WORD wSize;     // この構造体のサイズ（０の時は初期化されていない）
    WORD wVersion;  // バージョン表記

    /* DLL情報 */
    HANDLE hDll;    // DLL Handle

    /* ヒープ情報 */
    HANDLE hHeap;           // LocalHeapハンドル

    /* ディレクトリ情報 */
    HGLOBAL hGlobalLoaddir;
    SIZE_T  loaddirLength;
    LPTSTR  loaddir;


    /* ログ操作情報 */
    LPTSTR  logFilename;
    HANDLE  hLog;               // ログファイルハンドル
    BOOL    setLogBufferd;      // ログバッファに何かあるならTRUE
    TCHAR   logTime[32];        // ログバッファ確保日時
    BOOL    isLogOutputFatal;   // ログ出力：Fatal
    BOOL    isLogOutputError;   // ログ出力：Error
    BOOL    isLogOutputWarn;    // ログ出力：Warn
    BOOL    isLogOutputSystem;  // ログ出力：System
    BOOL    isLogOutputTrace;   // ログ出力：Trace
    BOOL    isLogOutputDebug;   // ログ出力：Debug
    BOOL    isLogOutputStderr;  // ログ出力：Stderr
    ANSISTR logStrTrace;        // ログバッファ：Trace
    ANSISTR logStrStderr;       // ログバッファ：Stderr
    ANSISTR logStrEtc;          // ログバッファ：その他


    /* 子プロセスの情報 */
    LPTSTR appName;         // アプリケーション名
    LPTSTR cmdLine;         // コマンドライン
    HANDLE hChlProcess;     // 子プロセスハンドル
    HANDLE hChlStdInWrite;  // 子プロセスSTDIN 書込用パイプ
    HANDLE hChlStdOutRead;  // 子プロセスSTDOUT読込用パイプ
    HANDLE hChlStdErrRead;  // 子プロセスSTDERR読込用パイプ
    ANSISTR bChiStdOut;     // 子プロセスのSTDOUT受信バッファ
    ANSISTR bChiStdErr;     // 子プロセスのSTDERR受信バッファ
    DWORD chiLastReadStdId; // 最後に読み込んだパイプ(STD_OUTPUT_HANDLE/STD_ERROR_HANDLE)
    LPTSTR bChilastRead;    // 最後に読込処理を行ったときの内容


} TUNAKI, *PTUNAKI, *LPTUNAKI;


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
