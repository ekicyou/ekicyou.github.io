/* ----------------------------------------------------------------------------
 * pipedprocess.h
 *   パイプで繋がったプロセスの起動・停止ライブラリの宣言.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: pipedprocess.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef PIPEDPROCESS_H__
#define PIPEDPROCESS_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include <windows.h>

/* ----------------------------------------------------------------------------
 * 型宣言
 */



/* ----------------------------------------------------------------------------
 * 定数
 */



/* ----------------------------------------------------------------------------
 * 関数宣言
 */

/* 標準入出力パイプで繋がったプロセスの作成 */
extern BOOL createPipedProsess(
    LPCTSTR lpApplicationName,  // 実行可能モジュールの名前
    LPCTSTR lpCommandLine,      // コマンドラインの文字列
    LPCTSTR lpCurrentDirectory, // カレントディレクトリの名前
    PHANDLE hProcess,           // 生成されたプロセスのハンドル
    PHANDLE hWriteStdinPipe,    // 生成プロセスのSTDIN に接続されたPipe
    PHANDLE hReadStdoutPipe,    // 生成プロセスのSTDOUTに接続されたPipe
    PHANDLE hReadStderrPipe     // 生成プロセスのSTDERRに接続されたPipe
  );

/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
