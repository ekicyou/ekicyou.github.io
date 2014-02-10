/* ----------------------------------------------------------------------------
 * pipedprocess.c
 *   パイプで繋がったプロセスの起動.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: pipedprocess.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "pipedprocess.h"


/*
 * プロセスをパイプで繋ぐやり方はこちらを参考にしました。
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/creating_a_child_process_with_redirected_input_and_output.asp
 */

/* ----------------------------------------------------------------------------
 * 管理構造体
 */
typedef struct _PIPED_PROCESS{
    // 各関数の実行結果
    BOOL resultCreateStdPipe;
    BOOL resultCreateChild;

    // 作成されたパイプ
    HANDLE hSTDIN_R;
    HANDLE hSTDIN_W;
    HANDLE hSTDOUT_R;
    HANDLE hSTDOUT_W;
    HANDLE hSTDERR_R;
    HANDLE hSTDERR_W;

    // 作成されたプロセス
    HANDLE hProcess;

    // プロセス起動パラメータ
    LPTSTR appName;
    LPTSTR cmdLine;
    LPTSTR CurrentDir;

    HANDLE cp;              // カレントプロセスのハンドル

} PIPED_PROCESS, *PPIPED_PROCESS;



/* ----------------------------------------------------------------------------
 * 入出力パイプの作成
 */
static void createStdPipe(PPIPED_PROCESS pp)
{
  HANDLE dup;
  SECURITY_ATTRIBUTES sa;
  const HANDLE cp =GetCurrentProcess(); // カレントプロセスのハンドル
  const DWORD dupOpt =DUPLICATE_SAME_ACCESS|DUPLICATE_CLOSE_SOURCE;

  // ハンドル継承オプション（継承ＯＫ）
  ZeroMemory(&sa, sizeof(sa));
  sa.nLength               = sizeof(sa);
  sa.bInheritHandle        = TRUE;  // ハンドルの継承許可
  sa.lpSecurityDescriptor  = NULL;

  // パイプ作成
  CreatePipe(&pp->hSTDIN_R  ,&pp->hSTDIN_W  ,&sa ,0);
  CreatePipe(&pp->hSTDOUT_R ,&pp->hSTDOUT_W ,&sa ,0);
  CreatePipe(&pp->hSTDERR_R ,&pp->hSTDERR_W ,&sa ,0);

  // 親プロセスに残す側を継承不可ハンドルにする
  {
    const PHANDLE pChg =&pp->hSTDIN_W;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }
  {
    const PHANDLE pChg =&pp->hSTDOUT_R;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }
  {
    const PHANDLE pChg =&pp->hSTDERR_R;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }

  pp->resultCreateStdPipe =TRUE;
}


/* ----------------------------------------------------------------------------
 * 子プロセスの作成
 */
static void createChild(PPIPED_PROCESS pp)
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  /* スタートアップインフォ */
  ZeroMemory(&si, sizeof(si));
  si.cb =sizeof(si);  // 構造体サイズ
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput    = pp->hSTDIN_R;
	si.hStdOutput   = pp->hSTDOUT_W;
	si.hStdError    = pp->hSTDERR_W;
	si.wShowWindow  = SW_HIDE;

  // プロセス作成
  pp->resultCreateChild =CreateProcess(
          pp->appName,        // 実行可能モジュールの名前
          pp->cmdLine,        // コマンドラインの文字列
          NULL,               // セキュリティ記述子
          NULL,               // セキュリティ記述子
          TRUE,               // ハンドルの継承オプション
          CREATE_NO_WINDOW,   // 作成のフラグ
          NULL,               // 新しい環境ブロック
          NULL,               // カレントディレクトリの名前
          &si,                // スタートアップ情報
          &pi                 // プロセス情報
        );
  pp->hProcess =pi.hProcess;
  CloseHandle(pi.hThread);
  CloseHandle(pp->hSTDIN_R);
  CloseHandle(pp->hSTDOUT_W);
  CloseHandle(pp->hSTDERR_W);
}


/* ----------------------------------------------------------------------------
 * パイプ付きプロセスの作成
 */
BOOL createPipedProsess(
    LPCTSTR lpApplicationName,  // 実行可能モジュールの名前
    LPCTSTR lpCommandLine,      // コマンドラインの文字列
    LPCTSTR lpCurrentDirectory, // カレントディレクトリの名前
    PHANDLE hProcess,           // 生成されたプロセスのハンドル
    PHANDLE hWriteStdinPipe,    // 生成プロセスのSTDIN に接続されたPipe
    PHANDLE hReadStdoutPipe,    // 生成プロセスのSTDOUTに接続されたPipe
    PHANDLE hReadStderrPipe     // 生成プロセスのSTDERRに接続されたPipe
  )
{
  PIPED_PROCESS pp;
  BOOL rc;

  // 初期化転送
  ZeroMemory(&pp, sizeof(pp));
  pp.appName    =lpApplicationName;
  pp.cmdLine    =lpCommandLine;
  pp.CurrentDir =lpCurrentDirectory;
  pp.cp         =GetCurrentProcess();

  // メインルーチン
  createStdPipe(&pp); // 入出力パイプの作成
  createChild(&pp);   // 子プロセスの作成

  // 戻り値の転送
  *hProcess         =pp.hProcess;
  *hWriteStdinPipe  =pp.hSTDIN_W;
  *hReadStdoutPipe  =pp.hSTDOUT_R;
  *hReadStderrPipe  =pp.hSTDERR_R;
  return pp.resultCreateChild;
}

/* ----------------------------------------------------------------------------
 * EOF
 */
