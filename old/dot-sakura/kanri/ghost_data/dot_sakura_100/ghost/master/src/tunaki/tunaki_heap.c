/* ----------------------------------------------------------------------------
 * tunaki.c
 *   つなき・ベース関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_heap.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_heap.h"


/* ----------------------------------------------------------------------------
 * コンストラクタ
 */
BOOL tunaki_heap_construct(
      PTUNAKI t // TUNAKI 構造体
  )
{
  if(! t) return FALSE;

  // Heap確保
  if(! (t->hHeap=HeapCreate(0, 64 *1024, 0))) return FALSE;
  
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * デストラクタ
 */
void tunaki_heap_destruct(
      PTUNAKI t // TUNAKI 構造体
  )
{
  if(! t) return;

  // Heap解放
  if(t->hHeap){
    HeapDestroy(t->hHeap);
    t->hHeap =NULL;
  }
}


/* ----------------------------------------------------------------------------
 * ヒープ操作 -alloc
 */
LPVOID tunaki_heapAlloc(
      PTUNAKI t,  // TUNAKI 構造体
      SIZE_T size // 確保するサイズ
  )
{
  LPVOID ptr;

  if(! t)         return NULL;
  if(! t->hHeap)  return NULL;

  // メモリ確保
  ptr = HeapAlloc(t->hHeap, 0, size);
  
  // @maz エラーの場合のログ出力
  
  return ptr;
}


/* ----------------------------------------------------------------------------
 * ヒープ操作 -free
 */
void tunaki_heapFree(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr  // 対象のローカルヒープ
  )
{
  if(! t)         return;
  if(! t->hHeap)  return;

  // メモリ解放
  HeapFree(t->hHeap, 0, ptr);

  return;
}


/* ----------------------------------------------------------------------------
 * ヒープ操作 -reAlloc
 */
LPVOID tunaki_heapReAlloc(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr, // 対象のローカルヒープ
      SIZE_T size // 確保するサイズ
  )
{
  LPVOID newPtr;
  
  if(! t)         return;
  if(! t->hHeap)  return;

  // メモリ再割り当て
  newPtr =HeapReAlloc(t->hHeap, 0, ptr, size);

  // @maz エラーの場合のログ出力

  return newPtr;
}


/* ----------------------------------------------------------------------------
 * ヒープ操作 -size
 */
SIZE_T tunaki_heapSize(
      PTUNAKI t,  // TUNAKI 構造体
      LPVOID ptr  // 対象のローカルヒープ
  )
{
  if(! t)         return 0;
  if(! t->hHeap)  return 0;

  // サイズ取得
  return HeapSize(t->hHeap, 0, ptr);
}


/* ----------------------------------------------------------------------------
 * EOF
 */
