@echo off
echo ..コピー：シェル

set src=%shell_select%\shell
set dst=%work_base_dir%\tmp\shell


call packUtil\copy_sub.bat
