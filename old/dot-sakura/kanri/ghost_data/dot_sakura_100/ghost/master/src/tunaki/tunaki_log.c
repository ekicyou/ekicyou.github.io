/* ----------------------------------------------------------------------------
 * tunaki_log.c
 *   つなき・ログ処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_log.c,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_string.h"


/* ----------------------------------------------------------------------------
 * ログ処理系の初期化
 */
BOOL tunaki_log_construct(
    PTUNAKI t       // つなき構造体
  )
{
  ANSISTR fName =NULL_ANSISTR;

  if(t->hLog) return TRUE;

  // ファイル名作成
  ASTRCAT(&fName,t->loaddir);
  ASTRCAT(&fName,"\\");
  ASTRCAT(&fName,LOG_FILE_NAME);
  t->logFilename =fName.s;

  // ファイルオープン
  t->hLog =CreateFile(
        t->logFilename,             // ファイル名
        GENERIC_WRITE,              // アクセスモード
        FILE_SHARE_READ,            // 共有モード
        NULL,                       // セキュリティ記述子
        OPEN_ALWAYS,                // 作成方法
        FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_SEQUENTIAL_SCAN,  // ファイル属性
        NULL                        // テンプレートファイルのハンドル
      );
  if(! t->hLog) return FALSE;

  // ファイルポインタを最後尾に
  if(INVALID_SET_FILE_POINTER==SetFilePointer(t->hLog ,0 ,0 ,FILE_END)) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ログ処理系の解放
 */
BOOL tunaki_log_destruct(
    PTUNAKI t       // つなき構造体
  )
{
  if(! t->hLog) return TRUE;
  ADD_MES_DEBUG("ログファイル" ,"クローズ");
  FLUSH_LOG;
  CloseHandle(t->hLog);

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * 現在日時文字列の作成
 */
static LPSTR getTimeString(LPSTR buf)
{
  FILETIME fileTime, localFileTime;
  SYSTEMTIME systemTime;
  DWORD tinysec;
  BOOL res;


  /* 時間の取得 */
  GetSystemTimeAsFileTime(&fileTime);
  res = FileTimeToLocalFileTime(&fileTime, &localFileTime) && FileTimeToSystemTime(&localFileTime, &systemTime);
  if (!res) {
    GetLocalTime(&systemTime);
  }
  /* 100 nanosec == 1/10 usec */
  tinysec = (fileTime.dwLowDateTime) % 10000000;

  /* 出力 */
  wsprintf(
      buf,
      "%04d/%02d/%02d %02d:%02d:%02d.%06d",
      systemTime.wYear,
      systemTime.wMonth,
      systemTime.wDay,
      systemTime.wHour,
      systemTime.wMinute,
      systemTime.wSecond,
      tinysec / 10);
  return buf;
}

/* ----------------------------------------------------------------------------
 * ログの出力タイプをチェック、保管しないならFALSE
 */
static LPTSTR checkLogType(
    PTUNAKI t,        // つなき構造体
    WORD    type      // メッセージタイプ
  )
{
  switch(type){
    case TLOG_TYPE_FATAL : if(t->isLogOutputFatal ) return "FATAL" ; break;
    case TLOG_TYPE_ERROR : if(t->isLogOutputError ) return "ERROR" ; break;
    case TLOG_TYPE_WARN  : if(t->isLogOutputWarn  ) return "WARN"  ; break;
    case TLOG_TYPE_SYSTEM: if(t->isLogOutputSystem) return "SYSTEM"; break;
    case TLOG_TYPE_TRACE : if(t->isLogOutputTrace ) return "TRACE" ; break;
    case TLOG_TYPE_DEBUG : if(t->isLogOutputDebug ) return "DEBUG" ; break;
    case TLOG_TYPE_STDERR: if(t->isLogOutputStderr) return "STDERR"; break;
  }
  return NULL;
}

/* ----------------------------------------------------------------------------
 * ログの記録（タイトル：メッセージ形式に整形）
 */
BOOL tunaki_logAddMessage(
    PTUNAKI t,        // つなき構造体
    WORD    type,     // メッセージタイプ
    LPCTSTR subject,  // タイトル
    LPCTSTR message   // メッセージ
  )
{
  TCHAR buf[1024];
  LPTSTR sLogType =checkLogType(t, type);
  if(sLogType == NULL) return TRUE;
  wsprintf(buf ,"(%s)%s: %s\r\n" ,sLogType ,subject ,message);
  return ADD_LOGTEXT(type ,buf);
}


/* ----------------------------------------------------------------------------
 * ログの記録（バッファに保管）
 */
BOOL tunaki_logAddText(
    PTUNAKI t,        // つなき構造体
    INT     type,     // メッセージタイプ
    LPCTSTR text      // 登録するテキスト
  )
{
  if(checkLogType(t, type)==NULL) return TRUE;

  // ログバッファに初めて登録されたときの処理
  if(! t->setLogBufferd){
    t->setLogBufferd =TRUE;
    getTimeString(t->logTime);  // 現在日時を保持
  }

  { // バッファに追加
    PANSISTR pAS;
    switch(type){
      case TLOG_TYPE_TRACE : pAS =&(t->logStrTrace ); break;
      case TLOG_TYPE_STDERR: pAS =&(t->logStrStderr); break;
      default              : pAS =&(t->logStrEtc   ); break;
    }
    ASTRCAT(pAS,text);
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ログの吐き出し
 */
static BOOL _writeText(
    HANDLE  hFile,    // ファイルのハンドル
    LPCTSTR lpText    // 書き込むテキスト
  )
{
  DWORD length =lstrlenA(lpText);
  DWORD wLen;
  BOOL rc =WriteFile(
      hFile,  // ファイルのハンドル
      lpText, // データバッファ
      length, // 書き込み対象のバイト数
      &wLen,  // 書き込んだバイト数
      NULL    // オーバーラップ構造体のバッファ
    );
  if(! rc)            return FALSE;
  if(length != wLen)  return FALSE;
  return TRUE;
}
#define WW(t) if(! _writeText(fp,t)) break;


static BOOL tunaki_logFlush_impl(
    PTUNAKI t         // つなき構造体
  )
{
  BOOL rc =FALSE;

  // 書き込む内容が存在するかどうか
  if(t->isLogOutputTrace  && t->bChiStdOut.length) t->setLogBufferd=TRUE;
  if(t->isLogOutputStderr && t->bChiStdErr.length) t->setLogBufferd=TRUE;
  if(! t->setLogBufferd)  return TRUE;
  t->setLogBufferd =FALSE;

  // 出力本体
  if(t->hLog) while(TRUE){
    HANDLE fp =t->hLog;
    TCHAR buf[1024];

    // ヘッダ
    WW("-----------------------------------------------------------------------\r\n");
    WW("DATE: ");
    WW(t->logTime);
    WW("\r\n\r\n");

    // STDOUT LOGを作成
    if(t->isLogOutputTrace){
      if(t->bChiStdOut.length){
        ASTRCAT(&t->logStrTrace, "■Response\r\n");
        ASTRCAT(&t->logStrTrace, t->bChiStdOut.s);
      }
    }

    // STDERR LOGを作成
    if(t->isLogOutputStderr){
      if(t->bChiStdErr.length){
        ASTRCAT(&t->logStrStderr, t->bChiStdErr.s);
      }
    }

    // TRACE LOG
    if(t->logStrTrace.length){
      WW(t->logStrTrace.s);
      WW("\r\n");
    }

    // STDERR LOG
    if(t->logStrStderr.length){
      WW("■STDERR\r\n");
      WW(t->logStrStderr.s);
      WW("\r\n");
    }

    // REPORT LOG
    if(t->logStrEtc.length){
      WW("■REPORT\r\n");
      WW(t->logStrEtc.s);
    }

    WW("\r\n");
    rc =TRUE;
    break;
  }
  return rc;
}


BOOL tunaki_logFlush(
    PTUNAKI t         // つなき構造体
  )
{
  // 書込処理本体
  BOOL rc =tunaki_logFlush_impl(t);

  // ログバッファの解放
  ASTRFREE(&(t->logStrTrace));
  ASTRFREE(&(t->logStrStderr));
  ASTRFREE(&(t->logStrEtc));
  ASTRFREE(&(t->bChiStdOut));
  ASTRFREE(&(t->bChiStdErr));

  return rc;
}






/* ----------------------------------------------------------------------------
 * EOF
 */
