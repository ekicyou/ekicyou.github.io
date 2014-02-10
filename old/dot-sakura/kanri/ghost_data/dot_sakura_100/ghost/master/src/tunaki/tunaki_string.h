/* ----------------------------------------------------------------------------
 * tunaki_string.h
 *   つなき・文字列ヒープ処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_string.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_STRING_H__
#define TUNAKI_STRING_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * 関数宣言
 */
/* 文字列構造体の初期化 */
extern BOOL tunaki_strset(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    SIZE_T size,        // 初期確保するheapサイズ
    LPCTSTR pStr        // 文字列へのポインタ
  );

/* 文字列構造体の解放 */
extern BOOL tunaki_strfree(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr  // ANSI文字列構造体へのポインタ
  );

/* 文字列構造体へ文字列を追加 */
extern BOOL tunaki_strcat(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    LPCTSTR pStr        // 文字列へのポインタ
  );

extern BOOL tunaki_strGetErrorMessage(
    PTUNAKI t,          // つなき構造体
    LPANSISTR pAnsiStr, // ANSI文字列構造体へのポインタ
    DWORD eno           // エラー番号
  );


/* ----------------------------------------------------------------------------
 * マクロ関数
 */
/* 文字列の取り出し */
#define ASTR2LPTSTR(pAs)  (pAs->s)

#define ASTRSET(pAs,pStr) (tunaki_strset(t,pAs,0,pStr))
#define ASTRFREE(pAs)     (tunaki_strfree(t,pAs))
#define ASTRCAT(pAs,pStr) (tunaki_strcat(t,pAs,pStr))
#define ASTRLEN(pAs)      ((pAs)->length)

#define ASTRCATSYSERR(pAs,eno) (tunaki_strGetErrorMessage(t,pAs,eno))


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
