@echo off
echo ..[nar]ファイル作成処理（ゴースト）

pushd %work_tmp_dir%
  zip -9 -r %narDir%\%ghost_id%_%version%.nar *.* 1>nul 2>&1
  zip -9 -r %narDir%\%ghost_id%.nar *.* 1>nul 2>&1
  popd
