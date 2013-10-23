<?php
//####################################################################
//
//    [ファイル名]
//    edit.php
//
//    [内容]
//    情報の編集。IDが存在する場合は修正モード
//    また、UPDATE引数が存在する場合は、
//    その値を表示し確定ボタンを表示
//
//    [引数]
//    ID:編集するデータ
//    UPDATE[]:修正された内容
//
//    [特記事項]
//    特になし
//
//####################################################################
  require_once( "./login.pplb" );
  $err =new ErrorSSTP("スクリプト表示");

  //==================================================================
  // 引数チェック
  //==================================================================
  $ID     =$_REQUEST[ID];
  $UPDATE =$_REQUEST[UPDATE];
  $el     =$_REQUEST[el];
  $COMMIT =FALSE;
  $MODE   ="NEW";

  //==================================================================
  // 処理本体
  //==================================================================
  if(is_array($UPDATE)){
    // 確認モード
    $MODE="UPD";
    $el =array_merge($el, $UPDATE);
    $ID =$el[ID];
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
  }
  else if($ID>0){
    // 修正モード
    // 編集項目定義を取得
    $MODE ="NEW";
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
    $buf[HIDDEN] =setHiddenHTML2("MODE", "COMMIT");
  }
  else{
    // 新規モード
    $MODE ="UPD";
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
    $buf[HIDDEN] =setHiddenHTML2("MODE", "COMMIT");
  }

  //==================================================================
  // さくらスクリプト送信
  //==================================================================
  if(strlen($el[SCRIPT])>0){
    $sstp =new SSTP($_SERVER[REMOTE_ADDR],"DOT-STATION");
    $script =Dot2Sakura($el[SCRIPT], $el[GHOST]);
//   $sstp->gSend($script, $el[GHOST]);
  }


  //==================================================================
  // 表示
  //==================================================================
  switch($MODE){
    case 'NEW'    :
    case 'UPD'    :
      $buf[INPUT] =TRUE;
      break;
    case 'CONFIRM':
    case 'COMMIT' :
  }


  $TMPL_FILE ="template.tmpl";
  $buf[NAME] =$err->title;
  TmplUty::out( $TMPL_FILE, $buf );

//====================================================================
// DOTスクリプト
//====================================================================
function Dot2Sakura($text, $ghost){
  // 行ごとの配列に分解
  $text =str_replace("\r\n", "\n", $text);
  $text =str_replace("\r"  , "\n", $text);
  $lines =explode("\n", $text);

  // ■レベル１：ブロック別に分類
  $blocks =array();
  $b ="__start__";
  $cnt =0;

  foreach($lines as $line){
    switch(mb_substr($line, 0, 1)){
      // コメント行は無視
      case '％':
        continue;

      // 新しい内部ブロック
      case '＃':
        $b =$line;
        $cnt++;
        continue;

      // ブロックに登録
      default:
        unset($x);
        $x[TEXT] =$line;
        $blocks[$b][$cnt][] =$x;
    }
  }


  // ■レベル２：冒頭処理（最初に：を見つけるまで）
  foreach($blocks as $bkey=>$blockBase){
    foreach($blockBase as $bCnt=>$block){
      foreach($block as $lkey=>$line){
        if(preg_match("/^((ｑ|ｓ|ｚ|ｍ|０|１)*)：(.*)$/", $line[TEXT], $match)){
          $line[MODE]  =$match[1];
          $line[TEXT] =$match[3];
        }
        else{
          $line[MODE]  ="ｌ"; // ゴーストを切り替えずに次の行
          $line[TEXT] =$line[TEXT];
        }
        $blocks[$bkey][$bCnt][$lkey] =$line;
      }
    }
  }


  // ■レベル３：括弧を見つけてトークンに分ける
  foreach($blocks as $bkey=>$blockBase){
    foreach($blockBase as $bCnt=>$block){
      foreach($block as $lkey=>$line){
        // （＃、）を検出
        $s =$line[TEXT];
        $t1 =array();
        unset($tt);
        $len =mb_strlen($s);
        for($ii=0; $ii<$len; $ii++){
          $s1 =mb_substr($s, $ii, 1);
          if($s1 == '（'){
            if(mb_substr($s, $ii+1, 1)=='＃'){
              if(isset($tt))  $t1[] =$tt;
              unset($tt);
              $ii++;
              $t1[] ="（＃";
              continue;
            }
          }
          if($s1 == "）"){
            if(isset($tt))  $t1[] =$tt;
            unset($tt);
            $t1[] ="）";
            continue;
          }
          $tt .=$s1;
        }
        if(isset($tt))  $t1[] =$tt;

        // （＃〜）のブロックを抜き出す
        $t2 =array();
        $chk=FALSE;
        unset($x);
        foreach($t1 as $k=>$v){
          if($v=="（＃"){
            if(isset($x)) $t2[] =$x;
            unset($x);
            $x[TYPE]=CALL_MODE;
            $x[TEXT]="＃";
            $chk=TRUE;
            continue;
          }
          if($chk && ($v=="）")){
            $chk=TRUE;
            if(isset($x)) $t2[] =$x;
            unset($x);
            continue;
          }
          if(! isset($x)) $x[TYPE]=TALK_MODE;
          $x[TEXT] .=$v;
        }
        if(isset($x)) $t2[] =$x;
        $line[TOKEN] =$t2;
        $blocks[$bkey][$bCnt][$lkey] =$line;
      }
    }
  }








d_print_r($blocks);



  return "\t解析完了\e";



  return $script;
}




?>