@echo off
echo ..コピー：定義情報


set src=%ghost_id%
set dst=%work_base_dir%\tmp

xcopy %xcopy_opt% %src%\*.* %dst%\ 1>nul 2>&1
del /s %dst%\0*.bat 1>nul 2>&1
