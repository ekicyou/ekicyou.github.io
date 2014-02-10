/* ----------------------------------------------------------------------------
 * tunaki_heap.h
 *   つなき・メインヘッダー.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_heap.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_HEAP_H__
#define TUNAKI_HEAP_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * 関数宣言
 */

/* コンストラクタ */
extern BOOL tunaki_heap_construct(
      PTUNAKI t // TUNAKI 構造体
  );

/* デストラクタ */
extern void tunaki_heap_destruct(
      PTUNAKI t // TUNAKI 構造体
  );

/* ヒープ操作 -alloc */
extern LPVOID tunaki_heapAlloc(
      PTUNAKI t,  // TUNAKI 構造体
      SIZE_T size // 確保するサイズ
  );

/* ヒープ操作 -free */
extern void tunaki_heapFree(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr  // 対象のローカルヒープ
  );

/* ヒープ操作 -reAlloc */
extern LPVOID tunaki_heapReAlloc(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr, // 対象のローカルヒープ
      SIZE_T size // 確保するサイズ
  );

/* ヒープ操作 -size */
extern SIZE_T tunaki_heapSize(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr  // 対象のローカルヒープ
  );


/* ----------------------------------------------------------------------------
 * マクロ関数
 */
#define HALLOC(s)     (tunaki_heapAlloc(t,s))
#define HFREE(p)      (tunaki_heapFree(t,p))
#define HREALLOC(p,s) (tunaki_heapReAlloc(t,p,s))
#define HSIZE(p)      (tunaki_heapSize(t,p))


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
