@echo off
echo ..コピー：バルーン

set src=%balloon_select%\%balloon_name%
set dst=%work_base_dir%\tmp\%balloon_name%


call packUtil\copy_sub.bat
