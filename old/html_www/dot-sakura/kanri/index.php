<?php
  include_once("./login.pplb");
?>
<html>
<head>
<title>DOT-STATION　管理メニュー</title>
<meta http-equiv="Content-Type" content="text/html;charset=EUC-JP">
<link rel="stylesheet" href="../common/style.css" type="text/css">
<link rel="stylesheet" href="../common/kanri.css" type="text/css">
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
          <td class="KANRI_TITLE_C">　DB編集</td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            　入力情報のメンテナンスを行います。　<a href="../" target="_blank">トップページはこちら</a></td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">

      <!-- ■メニューブロック　ここから　-->
      <table width="650" border="0" cellspacing="0" cellpadding="3">
        <tr>
          <td bgcolor="#DDDDFF"><b>データ取込・連動</b></td>
        </tr>
      </table>
      <table width="650" border="0" cellspacing="0" cellpadding="3">

        <tr bgcolor="#FFFFFF">
          <td width="10">&nbsp;</td>
          <td width="150"><font color="#000066">●</font><font color="#FF9900">&nbsp;</font><a href="log/log.php">ボトルログ取り込み</a></td>
          <td width="490"><font color="#666666" valign=top>ボトルのログを取り込みませう</font></td>
        </tr>

      </table>
      <br>
      <!-- ■メニューブロック　ここまで　-->

      <!-- ■メニューブロック　ここから　-->
      <table width="650" border="0" cellspacing="0" cellpadding="3">
        <tr>
          <td bgcolor="#DDDDFF"><b>開発中‥‥（不用意に触らないように(^^;）</b></td>
        </tr>
      </table>
      <table width="650" border="0" cellspacing="0" cellpadding="3">

        <tr bgcolor="#FFFFFF">
          <td width="10">&nbsp;</td>
          <td width="150"><font color="#000066">●</font><font color="#FF9900">&nbsp;</font><a href="sstp/send.php">SENDテスト</a></td>
          <td width="490"><font color="#666666" valign=top>送信テストを行います。</font></td>
        </tr>

        <tr bgcolor="#FFFFFF">
          <td width="10">&nbsp;</td>
          <td width="150"><font color="#000066">●</font><font color="#FF9900">&nbsp;</font><a href="read_ghost/index.php">旧「．さくら」データ取り込み</a></td>
          <td width="490"><font color="#666666" valign=top>旧「．さくら」のＡＩデータをＤＢに取り込みます。</font></td>
        </tr>

        <tr bgcolor="#FFFFFF">
          <td width="10">&nbsp;</td>
          <td width="150"><font color="#000066">●</font><font color="#FF9900">&nbsp;</font><a href="write_talk/index.php">「．さくら」Talkデータ出力</a></td>
          <td width="490"><font color="#666666" valign=top>「．さくら」のＡＩデータをゴーストフォルダに出力します。</font></td>
        </tr>

      </table>
      <br>
      <!-- ■メニューブロック　ここまで　-->

    </td>
  </tr>
</table>
</body>
</html>