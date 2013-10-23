/* ----------------------------------------------------------------------------
 * tunaki_pipe.c
 *   つなき・プロセス間通信処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_pipe.c,v 1.3 2004/04/01 15:25:44 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_heap.h"
#include "tunaki_string.h"
#include "pipedprocess.h"


/* ----------------------------------------------------------------------------
 * パイプに残っているデータ量を確認
 * 戻り値: -1: EOF
 *          0以上: 残量
 */
static int getPipeRemain(
    PTUNAKI t,  // つなき構造体
    HANDLE hPipe
  )
{
  LPCTSTR func ="パイプ残量確認";
  TCHAR buf[1];
  DWORD size =0 ,readSize ,eno;
  BOOL rc =PeekNamedPipe(hPipe ,buf ,sizeof(buf) ,&readSize ,&size ,NULL);
  if(size>0)  return size;
  if(rc)      return 0;
  eno =GetLastError();
  ADD_SYSMES_DEBUG(func,eno,"パイプの読み取りに失敗");
  switch(eno){
    case ERROR_BROKEN_PIPE:
      return -1;
  }

  return -1;
}


/* ----------------------------------------------------------------------------
 * 指定パイプを読めるだけ読む、エラーの場合ハンドルをクローズして終了
 * 戻り値は読み込んだデータのサイズ。クローズ終了の場合 -1 を返す
 */
int tunaki_pipeRead(
    PTUNAKI t,        // つなき構造体
    DWORD stdId,      // STD_OUTPUT_HANDLE / STD_ERROR_HANDLE のいずれか
    DWORD maxReadSize // 最大読込サイズ 0の時は読めるだけ読む
  )
{
  LPTSTR func;
  PHANDLE phPipe;
  PANSISTR pAS;
  BOOL eof =FALSE;
  int rc =-1;
  DWORD allReadSize =0;

  // 処理対象の選択
  switch(stdId){
    case STD_OUTPUT_HANDLE:
      phPipe =&(t->hChlStdOutRead);
      pAS    =&(t->bChiStdOut);
      func   ="子プロセス STDOUT 読込処理";
      break;

    case STD_ERROR_HANDLE:
      phPipe =&(t->hChlStdErrRead);
      pAS    =&(t->bChiStdErr);
      func   ="子プロセス STDERR 読込処理";
      break;

    default: return -1;
  }

  // 有効なパイプか
  if(*phPipe == NULL) return -1;

  // 初期化処理
  t->chiLastReadStdId =stdId;
  if(maxReadSize==0)  maxReadSize =MAXDWORD;

  // 読み込みできなくなるか、sizeが一杯になるまでループ
  while(maxReadSize>0){
    LPTSTR buf;
    DWORD readSize;
    BOOL ret;
    // 読み込めるサイズを取得
    INT size =getPipeRemain(t ,*phPipe);

    // EOF処理
    if(size < 0){
      ADD_MES_DEBUG(func ,"読込完了の為、ハンドルクローズし終了");
      eof =TRUE;
      CloseHandle(*phPipe);
      *phPipe =NULL;
      break;
    }

    // 読み込めるサイズが０
    if(size==0) break;

    // 読込サイズを決定
    if(size>maxReadSize)  size =maxReadSize;
    {
      TCHAR __buf[1024];
      wsprintf(__buf ,"読込処理[%d]byte" ,size);
      ADD_MES_DEBUG(func ,__buf);
    }

    // 読込処理本体
    buf =(LPTSTR)HALLOC(size+1);
    ret =ReadFile(*phPipe ,buf ,size ,&readSize ,NULL );
    if(! ret){
      DWORD eno =GetLastError();
      ADD_SYSMES_WARN(func ,eno ,"読込処理に異常が発生");
    }
    *(buf +readSize) =0;
    {
      TCHAR __buf[1024];
      wsprintf(__buf ,"読込データ[%s]" ,buf);
      ADD_MES_DEBUG(func ,__buf);
    }
    ASTRCAT(pAS, buf);
    HFREE(buf);

    // 読み込んだだけポインタを進める
    maxReadSize -=readSize;
    allReadSize +=readSize;
  }

  // 終了処理
  if((allReadSize==0) && (eof==TRUE)) return -1;
  return allReadSize;
}


/* ----------------------------------------------------------------------------
 * パイプをEOFになるまで全て読み捨てる
 */
static BOOL closeChildPipe(
    PTUNAKI t       // つなき構造体
  )
{
  BOOL rc;
  LPCTSTR func ="パイプクローズ＆読み捨て処理";
  const DWORD limitTick =10 *10000000;
  FILETIME sTime;
  int size;
  // 書込パイプのクローズ
  ADD_MES_DEBUG(func ,"STDIN出力パイプClose");
  if(! CloseHandle(t->hChlStdInWrite)){
    ADD_MES_WARN(func ,"STDIN出力のCloseに失敗");
  }

  // 開始時間の記録
  GetSystemTimeAsFileTime(&sTime);

  // 読込パイプのクローズ
  while(TRUE){
    FILETIME eTime;
    int chk=0;
    if(tunaki_pipeRead(t, STD_OUTPUT_HANDLE, 0)!=-1) chk++;
    if(tunaki_pipeRead(t, STD_ERROR_HANDLE , 0)!=-1) chk++;
    if(chk==0)  break;

    // タイムアウト判定
    GetSystemTimeAsFileTime(&eTime);
    if(eTime.dwLowDateTime -sTime.dwLowDateTime > limitTick){
      ADD_MES_WARN(func ,"10秒以内に子プロセスのSTDOUT/STDERRが解放されないため、強制Closeします");
      if(t->hChlStdOutRead){
        ADD_MES_DEBUG(func ,"STDOUT入力パイプ強制Close");
        rc =CloseHandle(t->hChlStdOutRead);
        if(! rc)  ADD_MES_WARN(func ,"STDOUTのCloseに失敗");
      }
      if(t->hChlStdErrRead){
        ADD_MES_DEBUG(func ,"STDERR入力パイプ強制Close");
        rc =CloseHandle(t->hChlStdErrRead);
        if(! rc)  ADD_MES_WARN(func ,"STDERRのCloseに失敗");
      }
      t->hChlStdOutRead =NULL;
      t->hChlStdErrRead =NULL;
      return FALSE;
    }
    Sleep(100);
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ターミネートマークを発見するか、EOFまで読み込む。
 * 戻り値は読み込んだデータのサイズ。EOF終了の場合 -1 を返す
 */
static long tunaki_pipeReadToMark(
    PTUNAKI t,    // つなき構造体
    LPCTSTR mark  // 終了マーク文字列
  )
{
  LPCTSTR func ="読込処理（終了マーク検出）";
  int size =0;
  int rc =0;
  BOOL noMatch =TRUE;
  LPTSTR chkStr;
  int markLen =lstrlen(mark);

  while(noMatch){
    /* STDOUT 読込 */
    rc =tunaki_pipeRead(t ,STD_OUTPUT_HANDLE, 0);
    if(rc<0){
      ADD_MES_ERROR(func ,"EOFが検出されました。");
      return -1;
    }
    size +=rc;

    /* 終了マークのチェック */
    if(size >= markLen){
      chkStr =t->bChiStdOut.s +(t->bChiStdOut.length -markLen);
      if(lstrcmp(chkStr,mark)==0) noMatch =FALSE;
    }

    /* STDERR 読込 */
    rc =tunaki_pipeRead(t ,STD_ERROR_HANDLE, 0);

    /* SLEEP */
    if(noMatch){
      Sleep(50);
    }
  }
  return size;
}



/* ----------------------------------------------------------------------------
 * Pingを送信する
 * 送信成功でTRUE
 */
BOOL tunaki_pipeWritePing(
    PTUNAKI t // つなき構造体
  )
{
  LPCTSTR func="Ping送信処理";
  BOOL rc;
  DWORD writeLen;

  // 送信処理
  rc =WriteFile(t->hChlStdInWrite ,".\r\n" ,3 ,&writeLen ,NULL);
  if(! rc){
    DWORD eno =GetLastError();
    ADD_SYSMES_ERROR(func ,eno ,"Pingの送信に失敗しました");
  }

  return rc;
}


/* ----------------------------------------------------------------------------
 * Pong[受信準備完了ヘッダ]を受信する。
 * 準備完了データとは[.\r\n]（ピリオド＋改行）でターミネートされたSTDOUT情報
 * 戻り値は読み込んだデータのサイズ。クローズ終了の場合 -1 を返す
 */
int tunaki_pipeReadPong(
    PTUNAKI t        // つなき構造体
  )
{
  int rc =tunaki_pipeReadToMark(t ,".\r\n");
  FLUSH_LOG;  // 送信可能状態になった段階でログフラッシュ
  return rc;
}


/* ----------------------------------------------------------------------------
 * リクエストを送信する
 * 送信成功でTRUE
 */
BOOL tunaki_pipeWriteRequest(
    PTUNAKI t,    // つなき構造体
    LPCVOID buf,  // 書き込みデータへのポインタ
    long len      // 書き込みデータ長
  )
{
  LPCTSTR func="リクエスト送信処理";
  BOOL rc;
  DWORD writeLen;

  // 送信処理
  rc =WriteFile(t->hChlStdInWrite ,buf ,len ,&writeLen ,NULL);
  if(! rc){
    DWORD eno =GetLastError();
    ADD_SYSMES_ERROR(func ,eno ,"リクエストの送信に失敗しました");
  }

  // 送信データをトレースログに登録
  if(t->isLogOutputTrace){
    LPTSTR logText =HALLOC(len+1);
    CopyMemory(logText ,buf ,len);
    *(logText+len)=0;
    ASTRCAT(&t->logStrTrace, "■Request\r\n");
    ASTRCAT(&t->logStrTrace, logText);
    HFREE(logText);
  }

  return rc;
}


/* ----------------------------------------------------------------------------
 * レスポンス１回分受信する。
 * １回のデータとは、[\r\n\r\n]（２連続改行）でターミネートされたSTDOUT情報
 * 戻り値は読み込んだデータのサイズ。クローズ終了の場合 -1 を返す
 */
long tunaki_pipeReadResponse(
    PTUNAKI t        // つなき構造体
  )
{
  return tunaki_pipeReadToMark(t ,"\r\n\r\n");
}


/* ----------------------------------------------------------------------------
 * プロセス間通信処理系の初期化
 */
BOOL tunaki_pipe_construct(
    PTUNAKI t       // つなき構造体
  )
{
  LPCTSTR func ="子プロセス起動処理";
  BOOL rc;
  ADD_MES_DEBUG(func ,"開始");

  // 子プロセス起動
  rc =createPipedProsess(
      t->appName,           // 実行可能モジュールの名前
      t->cmdLine,           // コマンドラインの文字列
      t->loaddir,           // カレントディレクトリの名前
      &(t->hChlProcess),    // 生成されたプロセスのハンドル
      &(t->hChlStdInWrite), // 生成プロセスのSTDIN に接続されたPipe
      &(t->hChlStdOutRead), // 生成プロセスのSTDOUTに接続されたPipe
      &(t->hChlStdErrRead)  // 生成プロセスのSTDERRに接続されたPipe
    );
  if(! rc){
    ANSISTR mes =NULL_ANSISTR;
    ASTRCAT(&mes, "プロセスが起動できませんでした");
    ASTRCAT(&mes, "\r\n       appName: ");
    ASTRCAT(&mes, t->appName);
    ASTRCAT(&mes, "\r\n       cmdLine: ");
    ASTRCAT(&mes, t->cmdLine);
    ADD_MES_FATAL(func, mes.s);
    ASTRFREE(&mes);
    return FALSE;
  }

  ADD_MES_DEBUG(func ,"完了");
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * プロセス間通信処理系の解放
 */
BOOL tunaki_pipe_destruct(
    PTUNAKI t       // つなき構造体
  )
{
  LPCTSTR func ="子プロセス解放処理";
  DWORD exitCode;

  ADD_MES_DEBUG(func ,"開始");

  /* @maz OnUnloadイベントを発行 */

  closeChildPipe(t);

  while(TRUE){
    ADD_MES_DEBUG(func ,"子プロセスの終了確認");
    GetExitCodeProcess(t->hChlProcess, &exitCode);
    if(exitCode!=STILL_ACTIVE)  break;
    ADD_MES_WARN(func ,"子プロセスが終了していないため、最長１秒待機");
    SleepEx(1000, TRUE);
    GetExitCodeProcess(t->hChlProcess, &exitCode);
    if(exitCode!=STILL_ACTIVE)  break;
    ADD_MES_WARN(func ,"子プロセスが終了しないため、強制終了します");
    if(! TerminateProcess(t->hChlProcess, 0)){
      ADD_MES_ERROR(func ,"子プロセスの強制終了に失敗しました");
    }
    break;
  }
  CloseHandle(t->hChlProcess);

  ADD_MES_DEBUG(func ,"完了");
  return TRUE;
}

/* ----------------------------------------------------------------------------
 * EOF
 */
