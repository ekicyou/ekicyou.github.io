<?php
//####################################################################
//
//    [ファイル名]
//    log.php
//
//    [内容]
//    ボトルログ取り込み
//
//    [特記事項]
//    特になし
//
//####################################################################
  require_once( "./login.pplb" );
  $err->init("データ取り込み | 「．さくら」データ取り込み");

//====================================================================
// 表示情報準備
//====================================================================
  $bb[URL] =$bb[URL2];
  $buf[HTML] = TmplUty::getText("index.tmpl", $bb);

//====================================================================
// 表示
//====================================================================
  $buf[MESSAGE] ="「．さくら未満（ぉ）」のデータをＤＢに登録します。";
  outputTmpl(&$err, $buf);
?>