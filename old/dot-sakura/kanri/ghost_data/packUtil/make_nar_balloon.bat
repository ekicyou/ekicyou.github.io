@echo off
echo ..[nar]ファイル作成処理（バルーン）

pushd %work_tmp_dir%\%balloon_name%
  zip -9 -q -r %narDir%\balloon_%balloon_name%.nar *.*
  popd
