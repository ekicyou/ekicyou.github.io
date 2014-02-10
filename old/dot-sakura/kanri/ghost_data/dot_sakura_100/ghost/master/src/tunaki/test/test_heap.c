/* ----------------------------------------------------------------------------
 * test_heap.h
 *   テストユニット：ヒープ関係.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: test_heap.c,v 1.3 2004/04/01 15:25:44 cvs Exp $
 * ----------------------------------------------------------------------------
 */

#include "../tunaki.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TestDB.h>



/* ----------------------------------------------------------------------------
 * 初期化・解放
 */
static PTUNAKI t;   // つなき構造体
static LPTSTR cDir; // カレントディレクトリ


static void makeIniFile(){
  char* fileName =INI_FILE_NAME;
  FILE *fp;
  if ((fp =fopen(fileName, "w")) == NULL) return;
  fputs("[", fp);
  fputs(INI_SECTION, fp);
  fputs("]\n", fp);
  fputs("appName= %dir%\\echo.exe\n", fp);
  fputs("cmdLine= -i %dir%\\bin\\test.ini\n", fp);
  fputs("logType= -fatal -error -warn -system -trace -debug -stderr\n", fp);
//  fputs("logType= -fatal -error -stderr\n", fp);
  fclose(fp);
}


int unit_heap_init(void)
{
  HANDLE  hDll=NULL;              // DLL自身のハンドル
  HGLOBAL hGlobal_loaddir =NULL;  // DLLのベースディレクトリを格納するヒープ
  long loaddir_len=0;             // ベースディレクトリの文字長

  // カレントディレクトリの取得
  loaddir_len =GetCurrentDirectory(0, NULL);
  hGlobal_loaddir =GlobalAlloc(GMEM_FIXED, loaddir_len);
  GetCurrentDirectory(loaddir_len, hGlobal_loaddir);

  // cDirにカレントディレクトリ情報を保管
  (HGLOBAL)cDir =GlobalAlloc(GMEM_FIXED, loaddir_len);
  GetCurrentDirectory(loaddir_len, cDir);

  // Iniファイルの作成
  makeIniFile();

  // つなき構造体の作成
  t =(PTUNAKI)GlobalAlloc(GMEM_FIXED, sizeof(*t));
  ZeroMemory(t, sizeof(*t));
  tunaki_construct(t, hGlobal_loaddir, loaddir_len);

  // PONG受信
  PIPE_PONG;

  return 0;
}

int unit_heap_clean(void)
{
  // つなき構造体の解放
  tunaki_destruct(t);
  GlobalFree(t);
  GlobalFree(cDir);
  t =NULL;
  return 0;
}


/* ----------------------------------------------------------------------------
 * テストユニット
 */
void unit_test_CurrentDir(void)
{
  // つなき構造体にカレントディレクトリが設定されているか？
  ASSERT_STRING_EQUAL(t->loaddir, cDir);

  printf("\n");
  printf("t->appName =[%s]\n", t->appName);
  printf("t->cmdLine =[%s]\n", t->cmdLine);

  printf("t->isLogOutputFatal  =[%d]\n", t->isLogOutputFatal );
  printf("t->isLogOutputError  =[%d]\n", t->isLogOutputError );
  printf("t->isLogOutputWarn   =[%d]\n", t->isLogOutputWarn  );
  printf("t->isLogOutputSystem =[%d]\n", t->isLogOutputSystem);
  printf("t->isLogOutputTrace  =[%d]\n", t->isLogOutputTrace );
  printf("t->isLogOutputDebug  =[%d]\n", t->isLogOutputDebug );
  printf("t->isLogOutputStderr =[%d]\n", t->isLogOutputStderr);

}


void unit_test_string(void)
{
  // 文字列関数テスト
  ANSISTR as =NULL_ANSISTR;
  ASTRCAT(&as, "TEST1");
  ASSERT_STRING_EQUAL(as.s, "TEST1");
  ASSERT_EQUAL(as.length, 5);

  ASTRCAT(&as, "/TEST2");
  ASSERT_STRING_EQUAL(as.s, "TEST1/TEST2");
  ASSERT_EQUAL(ASTRLEN(&as), 11);

  ASTRCAT(&as, "/TEST3");
  ASSERT_STRING_EQUAL(as.s, "TEST1/TEST2/TEST3");
  ASSERT_EQUAL(ASTRLEN(&as), 17);

  ASTRCAT(&as, "/T4");
  ASSERT_STRING_EQUAL(as.s, "TEST1/TEST2/TEST3/T4");
  ASSERT_EQUAL(ASTRLEN(&as), 20);


  ASTRFREE(&as);
}


void unit_test_pipe(void){
  TCHAR testData[] ="TESTDATA";
  DWORD writeSize;
  // パイプ読み書きテスト

  // テストデータを出力し、同じデータが取得できればTRUE
  WriteFile(t->hChlStdInWrite ,testData ,8 ,&writeSize, NULL);
  Sleep(500); // データが戻ってくるのを待つ

  ASSERT_EQUAL(tunaki_pipeRead(t ,STD_OUTPUT_HANDLE, 0), 8);
  ASSERT_STRING_EQUAL(t->bChiStdOut.s, testData);

  ASSERT_EQUAL(tunaki_pipeRead(t ,STD_ERROR_HANDLE, 0), 8);
  ASSERT_STRING_EQUAL(t->bChiStdErr.s, testData);


  // Pingテスト
  ASSERT(PIPE_PING>0);
  ASSERT(PIPE_PONG>0);

  // REQUEST/RESPONSE 1
  {
    char req[] ="TEST_REQUEST 1.0\r\nOK?\r\n\r\n";
    ASSERT(PIPE_REQUEST(req ,sizeof(req)-1)>0);
    ASSERT(PIPE_RESPONSE>0);
    ASSERT(PIPE_PING>0);
  }

}


/* ----------------------------------------------------------------------------
 * テストユニットの登録
 */
void unit_heap_AddTests(void)
{
  PTestGroup pGroup = NULL;
  PTestCase pTest = NULL;

  pGroup = add_test_group("heap", unit_heap_init, unit_heap_clean);
  pTest = add_test_case(pGroup, "unit_test_CurrentDir", unit_test_CurrentDir);
  pTest = add_test_case(pGroup, "unit_test_string", unit_test_string);
  pTest = add_test_case(pGroup, "unit_test_pipe", unit_test_pipe);
}

