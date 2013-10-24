@echo off
call packUtil\clean_tmp.bat
call packUtil\copy_ghost.bat
call packUtil\copy_shell.bat
call packUtil\copy_balloon.bat
call packUtil\copy_base.bat

if EXIST %work_tmp_dir%\updates2.dau              del %work_tmp_dir%\updates2.dau
if EXIST %work_tmp_dir%\ghost\master\updates2.dau del %work_tmp_dir%\ghost\master\updates2.dau
if EXIST %work_tmp_dir%\ghost\master\updates.txt  del %work_tmp_dir%\ghost\master\updates.txt
touch %work_tmp_dir%\updates2.dau
touch %work_tmp_dir%\ghost\master\updates2.dau
touch %work_tmp_dir%\ghost\master\updates.txt


set narDir=%work_base_dir%\nar
if EXIST %narDir% rm -Rf %narDir%
mkdir %narDir%

call packUtil\make_nar_ghost.bat
call packUtil\make_nar_balloon.bat
call packUtil\make_crow.bat


rm -Rf %work_tmp_dir%
