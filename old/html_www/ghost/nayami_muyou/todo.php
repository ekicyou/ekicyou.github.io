<?php
/**
 *  TO-DO Calendar
 *
 *  @since      2005.3.19
 *  @author     Nob Funaki <nob.funaki@gmail.com>
 */
define("MAX_DAYS",      7);
define("DATA_FILE",     "tmp.csv"); //  permision => 666
define("UNCHECK_COLOR", "#000");
define("CHECKED_COLOR", "#999");

$WEEK = array("日", "月", "火", "水", "木", "金", "土");


if (array_key_exists("id", $_POST)) {
    $buf = file_get_contents(DATA_FILE);
    if (array_key_exists("check", $_POST)) {
        $ch = $_POST["check"];
    } else {
        $ch = 0;
    }
    if (array_key_exists("str", $_POST) && $_POST["str"] != "") {
        $str = $ch."\t".$_POST["id"]."\t".$_POST["str"]."\n";
    } else {
        $str = "";
    }
    if (array_key_exists("oldstr", $_POST) && $_POST["oldstr"] != "") {
        $o    = preg_quote($_POST["oldstr"]);
        $patt = "'(0|1)\t".$_POST["id"]."\t".$o."\n'";
        $buf = preg_replace($patt, $str, $buf, 1);
    } else {
        $buf .= $str;
    }
    $fp = fopen(DATA_FILE, "w");
    fwrite($fp, $buf);
    fclose($fp);
    exit;
}
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>TODO Calender: とーどー</title>

<style type="text/css">
body {
    margin: 5% 10%;
}
h1 {
    text-align: right;
    font-size: 200%;
}
h1 small {
    font-size: 40%;
}
table {
    width: 100%;
    border: 1px solid #999;
}
table td {
    border: 1px solid #CCC;
}
td.date {
    margin: 0em 3em;
    text-align: center;
}
div.addtodo {
    text-align: right;
    font-size: 90%;
}
div.addtodo a:link, div.addtodo a:visited {
    text-decoration: none;
}
div.addtodo a:hover, div.addtodo a:active {
    text-decoration: underline;
}
div.addtodoform {
    text-align: right;
}
div.addtodoform input {
    width: 15em;
}
div.todolist {
    margin: 0em 1em 1em 1em;
}
div.message {
    margin-top: 2em;
    color: #666;
    font-size: 90%;
}
</style>

<script type="text/javascript">
var UNCHECK_COLOR = "<?=UNCHECK_COLOR?>";
var CHECKED_COLOR = "<?=CHECKED_COLOR?>";

function GetXmlHttpReqObj() {
    var xmlhttp;
    try {
        xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
        try {
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        } catch (e) {
            xmlhttp = false;
        }
    }
    if (!xmlhttp && typeof XMLHttpRequest != 'undefined') {
        xmlhttp = new XMLHttpRequest();
    }
    return xmlhttp;
}
function getfilename()
{
    var filename = location.pathname.substring(location.pathname.lastIndexOf('/') + 1); // get current filename
    if (filename == "") {
        filename = "index.php";
    }
    return filename;
}
function str_replace(v)
{
    v = v.replace(/\'/g, '’');
    v = v.replace(/\"/g, '”');
    return v;
}
function senddata(id, str, oldstr, check)
{
    var req = GetXmlHttpReqObj();
    req.open("POST", getfilename(), true);
    req.setRequestHeader('Content-Type','application/x-www-form-urlencoded'); 
    req.send("id=" + id + "&str=" + str + "&oldstr=" + oldstr + "&check=" + check);
}
function addtodoform(id)
{
    if (document.getElementById(id).style.display == 'none') {
        document.getElementById(id).style.display = 'block';
        document.getElementById('input_' + id).value = "";
        document.getElementById('input_' + id).focus();
    } else {
        document.getElementById(id).style.display = 'none';
    }
}
function addtodo(e, id, i)
{
    var kc = (e.keyCode || e.which);
    var v  = document.getElementById('input_' + id).value;
    v = str_replace(v);
    if (kc == 13 && v != "") { //  enter-key, onKeyPressだとIMEの確定時のEnterを拾わない
        var str = "<div id='li_up_" + id + "_" + i + "' style='display:list;'><input type='checkbox' onClick=\"checktodo(this, '" + id + "', 'up_" + id + "_" + i + "', '" + v + "')\"><span id='up_" + id + "_" + i + "' style='color:" + UNCHECK_COLOR + ";' ondblclick=\"updatetodoform('" + id + "', 'up_" + id + "_" + i + "', '" + v + "')\">" + v + "</span></div>\n" + document.getElementById('ul_' + id).innerHTML;
        document.getElementById('ul_' + id).innerHTML = str;
        senddata(id, v, "", 0);
        document.getElementById(id).style.display = 'none';
    }
}
function back_updatetodoform(id, upid, oldstr)
{
    var v  = document.getElementById('input2_' + id).value;
    if (v != "") {
        v = str_replace(v);
        document.getElementById(upid).innerHTML = v;
    } else {
        document.getElementById('li_' + upid).style.display = 'none';
    }
    senddata(id, v, oldstr, 0);
}
function updatetodoform(id, upid, oldstr)
{
    document.getElementById(upid).innerHTML = "<input type='text' value='" + oldstr + "' id='input2_" + id + "' onKeyPress=\"updatetodo(event, '" + id + "', '" + upid + "', '" + oldstr + "')\" onBlur=\"back_updatetodoform('" + id + "', '" + upid + "', '" + oldstr + "')\">";
    document.getElementById('input2_' + id).select();
    document.getElementById('input2_' + id).focus();
}
function updatetodo(e, id, upid, oldstr)
{
    var kc = (e.keyCode || e.which);
    var v  = document.getElementById('input2_' + id).value;
    if (kc == 13) {
        if (v == "") {
            document.getElementById('li_' + upid).style.display = 'none';
        } else {
            v = str_replace(v);
            document.getElementById(upid).innerHTML = v;
        }
        senddata(id, v, oldstr, 0);
    }
}
function checktodo(element, id, upid, oldstr)
{
    var ch;
    if (element.checked == true) {
        ch = 1;
        document.getElementById(upid).style.color = CHECKED_COLOR;
    } else {
        ch = 0;
        document.getElementById(upid).style.color = UNCHECK_COLOR;
    }
    senddata(id, oldstr, oldstr, ch);
}
</script>
</head>
<body>
<h1><?=date("n")?><small>月 （<?=date("z")?>日目）</small></h1>
<table>
<?php
//  ファイルからデータ読み込み、配列に格納
//  昨日以前のデータでcheck==1のものは削除、0は今日のデータに追加
$data  = array();
$today = date("Ymd");
$res   = "";
$f     = fopen(DATA_FILE, "r+");
while (1) {
    $buf = fgets($f);
    if ($buf == "") {
        break;
    }
    if (preg_match("'(0|1)\t([0-9]*)\t(.*?)\n'", $buf, $match)) {
        if (((int)$match[2] < $today && $match[1] == "0") || (int)$match[2] >= $today) {
            $d = (int)$match[2] < $today ? $today : $match[2];
            $res .= $match[1]."\t$d\t".$match[3]."\n";
            $data[$d][] = array("str" => $match[3], "check" => $match[1]);
        }
    }
}
rewind($f);
ftruncate($f, 0);
fwrite($f, $res);
fclose($f);

//  7日分出力
for ($i = 0; $i < MAX_DAYS; $i++) {
    $d   = time() + (60*60*24*$i);
    $id  = date("Ymd", $d);
    $day = date("j", $d);
    $c   = array_key_exists($id, $data) ? count($data) : 0;
?>
<tr>
<td class="date"><?=$day?><br><small><?=$WEEK[date("w", $d)]?></small></td>
<td valign="top">
    <div class="addtodo"><a href="javascript:void(0)" onClick="addtodoform('<?=$id?>')">&gt;&gt;add todo</a></div>
    <div style="display: none;" class="addtodoform" id="<?=$id?>">
        <input type="text" value="" onKeyPress="addtodo(event, '<?=$id?>', '<?=($c+1)?>')" id="input_<?=$id?>">
    </div>
    <div class="todolist" id="ul_<?=$id?>">
    <?php
    if (array_key_exists($id, $data)) {
        $j = 0;
        foreach ($data[$id] as $tmpval) {
            $ischeck = $tmpval["check"] == 1 ? "checked" : "";
            $val     = $tmpval["str"];
            $upid    = "up_".$id."_".$j;
            echo "\t<div id='li_$upid' style='display:list;'>"
                ."<input type='checkbox' onClick=\"checktodo(this, '$id', '$upid', '$val')\" $ischeck>"
                ."<span id='$upid' style='color:".($tmpval["check"] == 1 ? CHECKED_COLOR : UNCHECK_COLOR).";' ondblclick=\"updatetodoform('$id', '$upid', '$val')\">$val</span>"
                ."</div>\n";
            $j++;
        }
    }
    ?>
    </div>
</td>
</tr>
<?php
} // end for
?>
</table>
<div class="message">
<ul>
    <li>TODO管理ツールです</li>
    <li>文字をダブルクリックで編集可能</li>
    <li>文字を消せば（0文字で）削除</li>
    <li>テキストのフォームはEnterで確定。TODO部分は他をクリックでも確定。</li>
    <li>できなかったTODOは持ち越し</li>
</ul>
</div>
</body>
</html>
