/* ----------------------------------------------------------------------------
 * tunaki_pipe.h
 *   つなき・プロセス間通信処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_pipe.h,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_PIPE_H__
#define TUNAKI_PIPE_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * 関数宣言
 */
/* 初期化 */
extern BOOL tunaki_pipe_construct(
    PTUNAKI t       // つなき構造体
  );
/* 解放 */
extern BOOL tunaki_pipe_destruct(
    PTUNAKI t       // つなき構造体
  );

/* ------------------------------------------------------------------------- */
/* リクエストを送信する */
extern BOOL tunaki_pipeWriteRequest(
    PTUNAKI t,    // つなき構造体
    LPCVOID buf,  // 書き込みデータへのポインタ
    long len      // 書き込みデータ長 
  );

/* レスポンスを受信する */
extern long tunaki_pipeReadResponse(
    PTUNAKI t        // つなき構造体
  );

/* ------------------------------------------------------------------------- */
/* Pingを送信する */
extern BOOL tunaki_pipeWritePing(
    PTUNAKI t // つなき構造体
  );

/* Pongを受信準備完了ヘッダを受信する */
extern int tunaki_pipeReadPong(
    PTUNAKI t        // つなき構造体
  );

/* ------------------------------------------------------------------------- */
/* 指定パイプを読めるだけ読む */
extern int tunaki_pipeRead(
    PTUNAKI t,        // つなき構造体
    DWORD stdId,      // STD_OUTPUT_HANDLE / STD_ERROR_HANDLE のいずれか
    DWORD maxReadSize // 最大読込サイズ 0の時は読めるだけ読む
  );



/* ----------------------------------------------------------------------------
 * マクロ関数
 */
#define PIPE_PING         tunaki_pipeWritePing(t)
#define PIPE_PONG         tunaki_pipeReadPong(t)
#define PIPE_REQUEST(b,l) tunaki_pipeWriteRequest(t,b,l)
#define PIPE_RESPONSE     tunaki_pipeReadResponse(t)


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
