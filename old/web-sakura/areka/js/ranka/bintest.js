// バイナリ試験
try{
include("util");

//=============================================================
// 0-255 ファイル出力
(function(){

var buf = [];
buf.length=256;
for(var i=0; i<256; i++){
  buf[i]=i
}

var f = new File('bintest1.dat');
f.open('wb')
f.write(buf); // 数字の配列を渡せばバイナリのやり取りが出来る
f.close();

})();
//=============================================================
// 0-255 ファイル読み込み
(function(){

var f = new File('bintest1.dat');
f.open('rb')
var buf = f.read(256, true);  // 数字の配列が返ってくる
f.close();

system.stdout("[READ DATA]\n");
system.stdout(JSON.stringify(buf,null,"  "));
system.stdout("\n");

})();
//=============================================================
// utf8エンコード・デコード
(function(){
var encdata = Util.utf8decode('こんにちわ');

var buf = [];
for(var i=0; i<encdata.length; i++) buf.push(encdata.charCodeAt(i));

system.stdout("[Util.utf8decode]\n");
system.stdout(JSON.stringify(buf,null,"  "));
system.stdout("\n");

})();
//=============================================================
// 終了

}
catch(e){
  system.stdout(e);
}
