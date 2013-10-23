rmdir /s /q  %mvdir%
mkdir %mvdir%


pushd ..\..\
  del /s *.bak
  xcopy /Q /S %xcopy_opt% *.png %mvdir%\
  xcopy /Q /S %xcopy_opt% *.txt %mvdir%\
  xcopy /Q /S %xcopy_opt% *.kis %mvdir%\
  xcopy /Q /S %xcopy_opt% *.ini %mvdir%\
  xcopy /Q /S %xcopy_opt% *.dll %mvdir%\


  popd