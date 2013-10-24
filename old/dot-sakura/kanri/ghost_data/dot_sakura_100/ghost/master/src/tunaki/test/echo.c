/*
 *  echoプログラム
 */

#include <stdio.h>
#include <windows.h>

int main(void)
{
  HANDLE hStdin, hStdout, hStderr;
  TCHAR buf[]={0,0};
  DWORD readSize, writeSize;
  hStdin  =GetStdHandle(STD_INPUT_HANDLE);
  hStdout =GetStdHandle(STD_OUTPUT_HANDLE);
  hStderr =GetStdHandle(STD_ERROR_HANDLE);
  
  
  while(ReadFile(hStdin ,buf ,1 ,&readSize ,NULL)){
    if(readSize==0) break;
    if(! WriteFile(hStdout ,buf ,1 ,&writeSize, NULL)){
      printf("書き込みエラー\r\n");
      break;
    }
    if(! WriteFile(hStderr ,buf ,1 ,&writeSize, NULL)){
      printf("書き込みエラー\r\n");
      break;
    }
  }

  return 0;
}
