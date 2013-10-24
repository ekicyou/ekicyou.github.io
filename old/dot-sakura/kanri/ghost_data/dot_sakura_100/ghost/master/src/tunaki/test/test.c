/*
 *  CUnit テストユニット
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Console.h"

int main(int argc, char* argv[])
{
  void unit_heap_AddTests();
  
	setvbuf(stdout, NULL, _IONBF, 0);
	if (initialize_registry()) {
		printf("\nInitialize of test Registry failed.");
    return 1;
	}

  // テストユニットの登録
  unit_heap_AddTests();

  // テストの実行
	console_run_tests();
	cleanup_registry();

	return 0;
}
