@echo off
echo ..コピー：ゴースト

set src=%ghost_select%\ghost
set dst=%work_base_dir%\tmp\ghost


call packUtil\copy_sub.bat
