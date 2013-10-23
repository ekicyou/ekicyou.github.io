<?php
//####################################################################
//
//    [ファイル名]
//    log_exec.php
//
//    [内容]
//    ボトルログ取り込み
//
//    [特記事項]
//    なし
//
//####################################################################
//====================================================================
// ヘッダＨＴＭＬ
//====================================================================
?>
<html>
<head>
<title>DOT-STATION　管理メニュー</title>
<meta http-equiv="Content-Type" content="text/html;charset=UTP-8">
<link rel="stylesheet" href="/common/style.css" type="text/css">
<link rel="stylesheet" href="/common/kanri.css" type="text/css">
<link rel="stylesheet" href="/common/item.css" type="text/css" />
</head>

<body bgcolor="#FFFFFF">
<table width="700" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center" height="49">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td>
            <table border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="600" class="KANRI_TITLE_A">　管理画面</td>
                <td width="100" align="right"><a href="/kanri/index.php">ＴＯＰに戻る</a></td>
              </tr>
            </table>
          </td>
        </tr>
        <tr>
          <td class="KANRI_TITLE_B">DOT-STATION　管理メニュー</td>
        </tr>
        <tr>
          <td class="KANRI_TITLE_C">　ボトルログ | 取り込み処理 | 実行</td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            取り込み処理中です。<br>しばらくお待ちください。</td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<?php
//====================================================================
// メインルーチン
//====================================================================
  require_once("local/dot/db.pplb");
  require_once("HTML/ErrorSSTP.pplb");
  require_once("HTML/string.pplb");
  require_once("HTML/uty.pplb");
  require_once("log_exec_main.pplb");
  $err =new ErrorSSTP("ボトルログ | 取り込み処理 | 実行");

  while (@ob_end_clean());
  $conn =logonDB();
  $jobList[] =array(JOB_ID=>2, NAME=>"ログ削除");
  $jobList[] =array(JOB_ID=>3, NAME=>"ゴーストチャンネルエリアスデータ読込");
  $jobList[] =array(JOB_ID=>1, NAME=>"ログ取り込み");

  DButil::begin($conn);

  foreach($jobList as $job){
    set_time_limit(120);
    $JOB_ID =$job[JOB_ID];
    echo "■{$job[NAME]}....<br>\n";
    flush();
    $eval_str="job{$JOB_ID}" .'($conn, $err);';
    echo "COMMAND: $eval_str<br>\n";
    eval($eval_str);
    if($err->isError()) break;
  }
  if($err->isError()) DButil::rollback($conn);
  else                DButil::commit($conn);
  logoffDB($conn);
  $err->isErrorExit();


//####################################################################
// 更新処理本体　　＊＊ここまで＊＊
//####################################################################
?>
<table width="700" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            ログ取り込み終了しました。</td>
        </tr>
      </table>
    </td>
  </tr>
</table>


</body>

</html>