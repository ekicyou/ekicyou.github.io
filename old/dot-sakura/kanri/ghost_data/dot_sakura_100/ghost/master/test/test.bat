@echo off
cls
echo #### TEST START ####
..\php5\php.exe commandline_test.phu >result.log 2>&1
cat result.log
