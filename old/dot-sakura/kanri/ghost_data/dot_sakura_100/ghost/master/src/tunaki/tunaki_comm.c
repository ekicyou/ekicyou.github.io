/* ----------------------------------------------------------------------------
 * tunaki_comm.c
 *   つなき・コミュニケート処理.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_comm.c,v 1.3 2004/04/01 15:25:44 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_heap.h"
#include "tunaki_string.h"
#include "tunaki_pipe.h"


/* ----------------------------------------------------------------------------
 * Load
 */
extern BOOL tunaki_comm_load(
    PTUNAKI t       // つなき構造体
  )
{
  BOOL rc;
  ANSISTR req =NULL_ANSISTR;

  // リクエスト作成
  ASTRCAT(&req ,"LOAD Version TUNAKI/1.0\r\n");
  ASTRCAT(&req ,"Sender: Nobody\r\n");
  ASTRCAT(&req ,"Charset: Shift_JIS\r\n");
  ASTRCAT(&req ,"LoadDirectry: ");
  ASTRCAT(&req ,t->loaddir);
  ASTRCAT(&req ,"\r\n\r\n");

  // リクエスト送信
  rc =PIPE_REQUEST(req.s ,req.length);
  ASTRFREE(&req);
  if(! rc)  return FALSE;

  // レスポンス受信
  if(PIPE_RESPONSE < 0) return FALSE;

  // Ping送信
  if(PIPE_PING < 0) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * UnLoad
 */
extern BOOL tunaki_comm_unload(
    PTUNAKI t       // つなき構造体
  )
{
  BOOL rc;
  ANSISTR req =NULL_ANSISTR;

  // Pong待ち
  PIPE_PONG;

  // リクエスト作成
  ASTRCAT(&req ,"UNLOAD Version TUNAKI/1.0\r\n");
  ASTRCAT(&req ,"Sender: Nobody\r\n");
  ASTRCAT(&req ,"Charset: Shift_JIS\r\n\r\n");

  // リクエスト送信
  rc =PIPE_REQUEST(req.s ,req.length);
  ASTRFREE(&req);
  if(! rc)  return FALSE;

  // レスポンス受信
  if(PIPE_RESPONSE < 0) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * Request
 */
extern HGLOBAL tunaki_comm_request(
    PTUNAKI t,                // つなき構造体
    HGLOBAL hGlobal_request,  // SHIORI_REQUEST
    long* ptr_len             // リクエスト長
  )
{
  HGLOBAL hRes;
  BOOL rc;

  // Pong待ち
  PIPE_PONG;

  // リクエスト送信
  rc =PIPE_REQUEST(hGlobal_request ,*ptr_len);
  GlobalFree(hGlobal_request);
  if(! rc)  return NULL;

  // レスポンス受信
  if(PIPE_RESPONSE < 0) return NULL;

  // 戻り値を作成
  hRes =GlobalAlloc(GMEM_FIXED, t->bChiStdOut.length);
  CopyMemory(hRes ,t->bChiStdOut.s ,t->bChiStdOut.length);

  // Ping送信
  PIPE_PING;

  return hRes;
}


/* ----------------------------------------------------------------------------
 * EOF
 */
