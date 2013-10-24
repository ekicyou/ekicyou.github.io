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
require_once( "HTML/uty.pplb" );


//====================================================================
// 表示
//====================================================================
?>
<html>
<head>
<title>DOT-STATION　管理メニュー</title>
<meta http-equiv="Content-Type" content="text/html;charset=UTP-8">
<link rel="stylesheet" href="../../common/style.css" type="text/css">
<link rel="stylesheet" href="../../common/kanri.css" type="text/css">
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
          <td class="KANRI_TITLE_C">　ボトルログ | 取り込み処理</td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            取り込みます。
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="center">

      <form name="form1" method="post" action="log_exec.php">
      <br>
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="center">
            <input type="submit" value="取り込み">
          </td>
        </tr>
      </table>
      </form>

          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</body>

</html>
