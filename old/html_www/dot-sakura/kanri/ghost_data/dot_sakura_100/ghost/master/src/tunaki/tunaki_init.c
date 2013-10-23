/* ----------------------------------------------------------------------------
 * tunaki_init.c
 *   つなき・初期化＆iniファイルアクセス関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_init.c,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki.h"


/* ----------------------------------------------------------------------------
 * ログ出力モードを取り込む
 */
static BOOL setIsLogOutput(
      PTUNAKI t,          // TUNAKI 構造体
      LPTSTR pStr         // 取り込む文字列へのポインタ
  )
{
  LPTSTR buf ="123";
  pStr =CharUpper(pStr);

  while(*pStr){
    *(buf+0) =*(buf+1);
    *(buf+1) =*(buf+2);
    *(buf+2) =*pStr;
    pStr++;
    if(! lstrcmp(buf, "-FA")) t->isLogOutputFatal =TRUE;
    if(! lstrcmp(buf, "-ER")) t->isLogOutputError =TRUE;
    if(! lstrcmp(buf, "-WA")) t->isLogOutputWarn  =TRUE;
    if(! lstrcmp(buf, "-SY")) t->isLogOutputSystem=TRUE;
    if(! lstrcmp(buf, "-TR")) t->isLogOutputTrace =TRUE;
    if(! lstrcmp(buf, "-DE")) t->isLogOutputDebug =TRUE;
    if(! lstrcmp(buf, "-ST")) t->isLogOutputStderr=TRUE;
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * %dir% =>カレントディレクトリ名に変換
 */
static BOOL swapDirName(
      PTUNAKI t,          // TUNAKI 構造体
      LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ（格納先）
      LPCTSTR pStr        // 変換する文字列へのポインタ
  )
{
  LPCTSTR ss ="%dir%";
  LPTSTR buf ="X";
  LPTSTR p =pStr;
  int chk =0;
  while(*p){
    // １文字追加
    *buf =*p;
    ASTRCAT(pAnsiStr, buf);
    p++;

    // %dir%に一致するかの判定
    if(*buf != *(ss +chk)){
      chk =0;
      continue;
    }
    chk++;
    // %dir%に完全一致の場合、５文字削ってディレクトリ名を埋め込む
    if( chk==5 ){
      chk =0;
      pAnsiStr->length -=5;
      ASTRCAT(pAnsiStr, t->loaddir);
    }
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * iniファイルパラメータの取込
 */
static BOOL loadIniFile(
      PTUNAKI t   // TUNAKI 構造体
  )
{
  LPCTSTR sec =INI_SECTION;
  LPCTSTR appName =HALLOC(1024);
  LPCTSTR cmdLine =HALLOC(1024);
  LPCTSTR logType =HALLOC( 256);
  ANSISTR ini =NULL_ANSISTR;

  // ini ファイル名を作成
  ASTRCAT(&ini,t->loaddir);
  ASTRCAT(&ini,"\\");
  ASTRCAT(&ini,INI_FILE_NAME);

  // iniファイル読込
  GetPrivateProfileString(sec ,"appName" ,""                    ,appName ,1024 ,ini.s);
  GetPrivateProfileString(sec ,"cmdLine" ,""                    ,cmdLine ,1024 ,ini.s);
  GetPrivateProfileString(sec ,"logType" ,"-fatal-error-stderr" ,logType , 256 ,ini.s);
  ASTRFREE(&ini);

  { // logType 処理
    setIsLogOutput(t, logType);
    ADD_MES_DEBUG("INI logType" ,logType);
    HFREE(logType);
  }
  { // cmdLine 処理
    ANSISTR as =NULL_ANSISTR;
    swapDirName(t, &as, appName);
    t->appName =as.s;
    HFREE(appName);
    ADD_MES_DEBUG("INI appName" ,t->appName);
  }
  { // cmdLine 処理
    ANSISTR as =NULL_ANSISTR;
    swapDirName(t, &as, cmdLine);
    t->cmdLine =as.s;
    HFREE(cmdLine);
    ADD_MES_DEBUG("INI cmdLine" ,t->cmdLine);
  }

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * LOAD_DIRパラメータの取込処理
 */
static BOOL setLoadDir(
      PTUNAKI t   // TUNAKI 構造体
  )
{
  if(! t) return FALSE;
  if(! t->hGlobalLoaddir) return TRUE;

  // DLL文字列を解放し、内部heapに格納
  {
    ANSISTR as;
    ZeroMemory(&as, sizeof(as));
    if(! tunaki_strset(t ,&as ,0 ,(LPCTSTR)t->hGlobalLoaddir)) return FALSE;
    t->loaddir =as.s;
  }
  // 解放処理
  GlobalFree(t->hGlobalLoaddir);
  t->hGlobalLoaddir =NULL;
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * コンストラクタ
 */
BOOL tunaki_construct(
      PTUNAKI t,                // TUNAKI 構造体
      HGLOBAL hGlobalLoaddir,   // DLLのベースディレクトリを格納するヒープ
      SIZE_T  loaddirLength     // ベースディレクトリの文字長
  )
{
  HANDLE hDll;
  BOOL rc =FALSE;
  // 構造体ポインタがNULLなら、無条件失敗
  if(! t)  return FALSE;

  // DLLハンドルを一時保管
  hDll =t->hDll;

  // 構造体サイズがゼロ以外なら、解放処理を行う
  if(t->wSize){
    tunaki_destruct(t);
  }

  // クリア
  ZeroMemory(t, sizeof(*t));

  // 初期化
  t->wVersion         =VERSION_VALUE;
  t->hDll             =hDll;
  t->hGlobalLoaddir   =hGlobalLoaddir;
  t->loaddirLength    =loaddirLength;

  while(TRUE){
    // 各ブロックコンストラクタ呼び出し
    if(! tunaki_heap_construct(t))  break;
    if(! setLoadDir(t))             break;
    if(! tunaki_log_construct(t))   break;
    if(! loadIniFile(t))            break;
    if(! tunaki_pipe_construct(t))  break;
    if(! tunaki_comm_load(t))       break;

    // 構造体サイズをセットして終了（初期化成功を意味する）
    t->wSize =sizeof(*t);
    ADD_MES_SYSTEM("システム起動処理","完了");
    rc =TRUE;
    break;
  }

  FLUSH_LOG;
  return rc;
}



/* ----------------------------------------------------------------------------
 * デストラクタ
 */
BOOL tunaki_destruct(
      PTUNAKI t // TUNAKI 構造体
  )
{
  // 構造体ポインタがNULLなら、既に解放されていると判断
  if(! t)  return TRUE;

  // 構造体サイズがゼロなら、既に解放されていると判断
  if(! t->wSize)  return TRUE;

  // 解放処理ログ
  ADD_MES_SYSTEM("システム終了処理","****");

  // 各ブロックデストラクタ呼び出し（コンストラクタと逆順、必ず呼び出す）
  tunaki_comm_unload(t);
  tunaki_pipe_destruct(t);
  tunaki_log_destruct(t);
  tunaki_heap_destruct(t);

  // クリアして終了
  ZeroMemory(t, sizeof(*t));
  return TRUE;
}



/* ----------------------------------------------------------------------------
 * リクエスト受付
 */





/* ----------------------------------------------------------------------------
 * EOF
 */
