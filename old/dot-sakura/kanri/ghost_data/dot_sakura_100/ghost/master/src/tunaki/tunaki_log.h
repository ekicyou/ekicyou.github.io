/* ----------------------------------------------------------------------------
 * tunaki_log.h
 *   つなき・ログ処理関数.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_log.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_LOG_H__
#define TUNAKI_LOG_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * 定数宣言
 */
#define TLOG_TYPE_FATAL   (1) // ログ：FATAL   (重大なエラー)
#define TLOG_TYPE_ERROR   (2) // ログ：ERROR   (エラー)
#define TLOG_TYPE_WARN    (3) // ログ：WARN    (警告)
#define TLOG_TYPE_SYSTEM  (4) // ログ：SYSTEM  (システムメッセージ)
#define TLOG_TYPE_TRACE   (5) // ログ：TRACE   (SAORIログ)
#define TLOG_TYPE_DEBUG   (6) // ログ：DEBUG   (デバッグログ)
#define TLOG_TYPE_STDERR  (7) // ログ：STDERR  (呼び出しモジュールのSTDERR)


/* ----------------------------------------------------------------------------
 * 関数宣言
 */
/* 初期化 */
extern BOOL tunaki_log_construct(
    PTUNAKI t       // つなき構造体
  );
/* 解放 */
extern BOOL tunaki_log_destruct(
    PTUNAKI t       // つなき構造体
  );

/* ログを整形して出力 */
extern BOOL tunaki_logAddMessage(
    PTUNAKI t,        // つなき構造体
    WORD    type,     // メッセージタイプ
    LPCTSTR subject,  // メッセージタイトル
    LPCTSTR message   // メッセージ
  );

/* ログバッファにテキストを追加 */
extern BOOL tunaki_logAddText(
    PTUNAKI t,        // つなき構造体
    INT     type,     // メッセージタイプ
    LPCTSTR text      // 登録するテキスト
  );

/* ログのフラッシュ */
extern BOOL tunaki_logFlush(
    PTUNAKI t         // つなき構造体
  );


/* ----------------------------------------------------------------------------
 * マクロ関数
 */
#define ADD_LOGMES(tp,s,m)  (tunaki_logAddMessage(t,tp,s,m))
#define ADD_LOGTEXT(tp,s)   (tunaki_logAddText(t,tp,s))
#define FLUSH_LOG           (tunaki_logFlush(t))

#define ADD_MES_FATAL(s,m)  {if(t->isLogOutputFatal)   ADD_LOGMES(TLOG_TYPE_FATAL ,s,m);}
#define ADD_MES_ERROR(s,m)  {if(t->isLogOutputError)   ADD_LOGMES(TLOG_TYPE_ERROR ,s,m);}
#define ADD_MES_WARN(s,m)   {if(t->isLogOutputWarn)    ADD_LOGMES(TLOG_TYPE_WARN  ,s,m);}
#define ADD_MES_SYSTEM(s,m) {if(t->isLogOutputSystem)  ADD_LOGMES(TLOG_TYPE_SYSTEM,s,m);}
#define ADD_MES_DEBUG(s,m)  {if(t->isLogOutputDebug)   ADD_LOGMES(TLOG_TYPE_DEBUG ,s,m);}

#define ADD_SYSMES_FATAL(ss,eno,m)            \
  if(t->isLogOutputFatal){                    \
    ANSISTR __mes =NULL_ANSISTR;              \
    ASTRCAT(&__mes, m);                       \
    ASTRCAT(&__mes, ": ");                    \
    ASTRCATSYSERR(&__mes, eno);               \
    ADD_LOGMES(TLOG_TYPE_FATAL,ss,__mes.s);   \
    ASTRFREE(&__mes);                         \
  }
#define ADD_SYSMES_ERROR(ss,eno,m)            \
  if(t->isLogOutputError){                    \
    ANSISTR __mes =NULL_ANSISTR;              \
    ASTRCAT(&__mes, m);                       \
    ASTRCAT(&__mes, ": ");                    \
    ASTRCATSYSERR(&__mes, eno);               \
    ADD_LOGMES(TLOG_TYPE_ERROR,ss,__mes.s);   \
    ASTRFREE(&__mes);                         \
  }
#define ADD_SYSMES_WARN(ss,eno,m)             \
  if(t->isLogOutputWarn){                     \
    ANSISTR __mes =NULL_ANSISTR;              \
    ASTRCAT(&__mes, m);                       \
    ASTRCAT(&__mes, ": ");                    \
    ASTRCATSYSERR(&__mes, eno);               \
    ADD_LOGMES(TLOG_TYPE_WARN,ss,__mes.s);    \
    ASTRFREE(&__mes);                         \
  }
#define ADD_SYSMES_SYSTEM(ss,eno,m)           \
  if(t->isLogOutputSystem){                   \
    ANSISTR __mes =NULL_ANSISTR;              \
    ASTRCAT(&__mes, m);                       \
    ASTRCAT(&__mes, ": ");                    \
    ASTRCATSYSERR(&__mes, eno);               \
    ADD_LOGMES(TLOG_TYPE_SYSTEM,ss,__mes.s);  \
    ASTRFREE(&__mes);                         \
  }
#define ADD_SYSMES_DEBUG(ss,eno,m)            \
  if(t->isLogOutputDebug){                    \
    ANSISTR __mes =NULL_ANSISTR;              \
    ASTRCAT(&__mes, m);                       \
    ASTRCAT(&__mes, ": ");                    \
    ASTRCATSYSERR(&__mes, eno);               \
    ADD_LOGMES(TLOG_TYPE_DEBUG,ss,__mes.s);   \
    ASTRFREE(&__mes);                         \
  }




/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
