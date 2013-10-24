/* ----------------------------------------------------------------------------
 * tunaki_string.h
 *   つなき・文字列ヒープ処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_string.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_string.h"
#include "tunaki_heap.h"


/* ----------------------------------------------------------------------------
 * 初期化
 */
BOOL tunaki_strset(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    SIZE_T size,        // 初期確保するheapサイズ
    LPCTSTR pStr        // 文字列へのポインタ
  )
{
  LPVOID p;
  SIZE_T length;
  if(pStr)  length =lstrlenA(pStr);
  else      length =0;
  if(size<(length+1)) size =length+1;

  // 一応ANSI文字列を解放する
  if(! ASTRFREE(pAnsiStr)) return FALSE;

  // 領域確保
  p =HALLOC(size);
  if(! p) return FALSE;
  pAnsiStr->s =(LPTSTR) p;

  // 転送
  pAnsiStr->size   =size;
  pAnsiStr->length =length;
  if(pStr)  lstrcpyA(pAnsiStr->s, pStr);
  else      *(pAnsiStr->s) =0;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * 解放
 */
BOOL tunaki_strfree(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr  // ANSI文字列構造体へのポインタ
  )
{
  if(pAnsiStr->s == NULL) return TRUE;
  HFREE(pAnsiStr->s);
  ZeroMemory(pAnsiStr, sizeof(*pAnsiStr));
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * 文字列構造体へ文字列を追加
 */
BOOL tunaki_strcat(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    LPCTSTR pStr        // 文字列へのポインタ
  )
{
  SIZE_T length;

  // ヒープが確保されていない場合は ASTRSET
  if(pAnsiStr->s==NULL) return ASTRSET(pAnsiStr, pStr);

  // ヒープが確保されている場合は再割り当て
  length =pAnsiStr->length +lstrlenA(pStr);
  // 結合後のサイズが現在確保しているサイズを超えるなら
  // サイズ拡大
  if(pAnsiStr->size < (length+1)){
    SIZE_T size =pAnsiStr->size;
    LPVOID p;
    while(TRUE){
      size <<=1;
      if(size>=(length+1))  break;
    }
    p =HREALLOC(pAnsiStr->s, size);
    pAnsiStr->s =(LPTSTR) p;
    pAnsiStr->size   =size;
  }
  // 文字列のコピー
  lstrcpyA((pAnsiStr->s)+pAnsiStr->length, pStr);
  pAnsiStr->length =length;
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * システムエラーを書式化する
 */
BOOL tunaki_strGetErrorMessage(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    DWORD eno           // エラー番号
  )
{
  TCHAR buf[1024];
  LPVOID lpMsgBuf =NULL;
  DWORD rc =FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      eno,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
      (LPTSTR) &lpMsgBuf,
      0,
      NULL
    );
  wsprintf(buf ,"ENO(%d) " ,eno);
  ASTRCAT(pAnsiStr, buf);
  if(rc){
    ASTRCAT(pAnsiStr, lpMsgBuf);
  }
  LocalFree(lpMsgBuf);
  return (rc!=0);
}


/* ----------------------------------------------------------------------------
 * EOF
 */
