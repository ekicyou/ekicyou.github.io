@echo off
echo ..ì‹Æ—Ìˆæíœ

if not EXIST %work_base_dir% mkdir %work_base_dir%
rm -Rf %work_base_dir%\tmp
mkdir %work_base_dir%\tmp
del /s *.bak 1>nul 2>&1
