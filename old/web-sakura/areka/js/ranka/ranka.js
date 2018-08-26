// RANKA ルートスクリプト
try{
//=============================================================
// RANKA モジュール
  include("./js/nilbug.js");
  include("./js/taffy.js");
  include("./js/js.js");
  include("./js/util.js");
  include("./ranka/util.js");
  include("./ranka/jactor.js");
  include("./ranka/shiolink.js");
  include("./ranka/ranka.const.js");
  include("./ranka/ranka.talk.entry.js");
  include("./ranka/ranka.talk.slot.js");
  include("./ranka/ranka.talk.compile.js");
  include("./ranka/ranka.talk.sakurascript.js");

//=============================================================
// ゴースト辞書 # ここを編集してください




//              # ここまで編集してください
//=============================================================
// メインルーチン


}
catch(e){
  system.stdout(e);
}
