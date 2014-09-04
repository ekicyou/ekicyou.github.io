setlocal
pushd ..
  rmdir /Q /S _site
  rmdir /Q /S _site
  rmdir /Q /S _site
  _bin\pretzel bake --debug <NUL
  pause
  robocopy /MIR _site _out /XD .git
  popd
