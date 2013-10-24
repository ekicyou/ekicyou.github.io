@echo off
call packUtil\clean_tmp.bat
call packUtil\copy_ghost.bat
call packUtil\copy_shell.bat
call packUtil\copy_base.bat


echo ..デバッグゴースト作成
set src=%work_base_dir%\tmp
set dst=%ssp_dir%\ghost\%ghost_id%_d
if EXIST %dst% rm -Rf %dst%
call packUtil\copy_sub.bat


rm -Rf %work_tmp_dir%
