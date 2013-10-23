<?php
  require_once("HTML/templateUty.pplb");

  // HEADイベント時に更新時刻を返して終了
  $CHK_FILES[] =dirname(__FILE__) ."/sakura.pplb";
  include("HTML/LastModified.pplb");

  // XMLヘッダ
  echo '<?xml version="1.0" encoding="UTF-8"?>' ."\n";

  // 各テンプレート情報取得
  include("./dengon.pplb");   // 掲示板

  include("./sakura.pplb");   // さくら表示

  include("./download.pplb"); // ダウンロード情報

  include("./link.pplb");     // リンク情報

  // ベーステンプレート
  TmplUty::out("base.tmpl" ,$buf);
  exit();
?>