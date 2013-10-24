rem ## ネットワークファイル ##
set nar_name=dot_sakura
set ssp_base_dir=c:\wintools\何か\ssp
set xcopy_opt=/D:9-1-2003
rmdir /s /q  c:\wrkdir\download
mkdir c:\wrkdir\download
mkdir c:\wrkdir\download\ghost
set mvdir=c:\wrkdir\download\ghost\%nar_name%_000
cmd /C up1.bat

del %mvdir%\ghost\master\descriptDebug.txt
del %mvdir%\ghost\master\kawariDebug.ini

del %mvdir%\install.txt
del %mvdir%\ghost\master\dict-keeps-savedata.txt
touch %mvdir%\updates2.dau
rmdir /s /q  %mvdir%\bottle
