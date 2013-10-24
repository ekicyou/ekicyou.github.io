@echo off
echo ..[crow]配布ファイル作成

set srcCrowDir=%crow_select%
set srcCrowGhostDir=%work_base_dir%\tmp
set srcCrowBalloonDir=%srcCrowGhostDir%\%balloon_name%

set dstCrowDir=%work_base_dir%\crow
set dstCrowGhostDir=%dstCrowDir%\ghost
set dstCrowBalloonDir=%dstCrowDir%\balloon


if EXIST %dstCrowDir% rm -Rf %dstCrowDir%
mkdir %dstCrowDir%
mkdir %dstCrowGhostDir%
mkdir %dstCrowGhostDir%\%ghost_id%
mkdir %dstCrowBalloonDir%
mkdir %dstCrowBalloonDir%\%balloon_name%
copy %srcCrowDir%\*.* %dstCrowDir% 1>nul 2>&1
xcopy /S %srcCrowGhostDir%\*.* %dstCrowGhostDir%\%ghost_id% 1>nul 2>&1
rm -Rf %dstCrowGhostDir%\%ghost_id%\%balloon_name%
xcopy /S %srcCrowBalloonDir%\*.* %dstCrowBalloonDir%\%balloon_name% 1>nul 2>&1


pushd %work_base_dir%
  zip -9 -q -r %narDir%\crow_%ghost_id%.zip crow
  popd


rm -Rf %dstCrowDir%
