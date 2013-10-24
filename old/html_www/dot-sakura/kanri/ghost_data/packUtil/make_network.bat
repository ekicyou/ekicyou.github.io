@echo off
call packUtil\clean_tmp.bat
call packUtil\copy_ghost.bat
call packUtil\copy_shell.bat
call packUtil\copy_base.bat


echo ..MD5計算
set netDir=%work_base_dir%\ghost
set src=%work_base_dir%\tmp
set dst=%netDir%\%ghost_id%
del %src%\install.txt 1>nul 2>&1
php packUtil\calc_md5.php


echo ..配布ファイル作成
if EXIST %netDir% rm -Rf %netDir%
mkdir %netDir%
call packUtil\copy_sub.bat


rm -Rf %work_tmp_dir%
