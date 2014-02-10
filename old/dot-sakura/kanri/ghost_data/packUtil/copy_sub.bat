@echo off

mkdir %dst%
xcopy /S %xcopy_opt% %src%\*.bat   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.bdp   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.c     %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.dau   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.dll   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.exe   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.h     %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.ico   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.ini   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.php   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.phu   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.plu   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.png   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.spf   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.tpl   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.txt   %dst%\ 1>nul 2>&1
xcopy /S %xcopy_opt% %src%\*.xml   %dst%\ 1>nul 2>&1
