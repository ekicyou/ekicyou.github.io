/* ----------------------------------------------------------------------------
 * tunaki_comm.h
 *   つなき・コミュニケート処理.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_comm.h,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_COMM_H__
#define TUNAKI_COMM_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * 関数宣言
 */
/* Load */
extern BOOL tunaki_comm_load(
    PTUNAKI t       // つなき構造体
  );

/* UnLoad */
extern BOOL tunaki_comm_unload(
    PTUNAKI t       // つなき構造体
  );

/* Request */
extern HGLOBAL tunaki_comm_request(
    PTUNAKI t,                // つなき構造体
    HGLOBAL hGlobal_request,  // SHIORI_REQUEST
    long* ptr_len             // リクエスト長
  );


/* ----------------------------------------------------------------------------
 * マクロ関数
 */


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
