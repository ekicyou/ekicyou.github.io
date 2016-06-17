// RANKA トーク辞書：スロット解釈
(function(){

//=============================================================
// 識別定数
//-------------------------------------------------------------
var SLOT_SPACE         = Ranka.lastID++;  // スロット無視文字
var SLOT_ACTOR         = Ranka.lastID++;  // 役者
var SLOT_NUMBER        = Ranka.lastID++;  // 数字（サーフェス指定）
var SLOT_SPEED_SECTION = Ranka.lastID++;  // [s]自動ウェイトキャンセル
var SLOT_QUICK_SECTION = Ranka.lastID++;  // [q]瞬時表示
var SLOT_ZERO_CRLF     = Ranka.lastID++;  // [z]役者変更時の改行なし
var SLOT_CRLF          = Ranka.lastID++;  // [n]改行
var SLOT_CLEAR         = Ranka.lastID++;  // [c]バルーンクリア


//=============================================================
// 会話スロットの登録
//-------------------------------------------------------------
var slots ={};

// 文字を分解してキーに突っ込む関数
var setCharKey = function(obj, keys, value){
  for(var i=0; i<keys.length; i++){
    obj[keys[i]] = value;
  }
}

// スロットセットの登録関数
var setSlot = function(slot){
  var item, ss;
  for(var i=0; i<slot.length; i++){
    item = slot[i];
    if(item.type == undefined) item.type = SLOT_ACTOR;
    setCharKey(slots, item.keys, item);
  }
}

// 特殊スロット指定
setSlot([
  {type: SLOT_SPACE        , keys:"\t 　"},
  {type: SLOT_NUMBER       , keys:"0０", value: "0"},
  {type: SLOT_NUMBER       , keys:"1１", value: "1"},
  {type: SLOT_NUMBER       , keys:"2２", value: "2"},
  {type: SLOT_NUMBER       , keys:"3３", value: "3"},
  {type: SLOT_NUMBER       , keys:"4４", value: "4"},
  {type: SLOT_NUMBER       , keys:"5５", value: "5"},
  {type: SLOT_NUMBER       , keys:"6６", value: "6"},
  {type: SLOT_NUMBER       , keys:"7７", value: "7"},
  {type: SLOT_NUMBER       , keys:"8８", value: "8"},
  {type: SLOT_NUMBER       , keys:"9９", value: "9"},
  {type: SLOT_SPEED_SECTION, keys:"sSｓＳ"},
  {type: SLOT_QUICK_SECTION, keys:"qQｑＱ"},
  {type: SLOT_ZERO_CRLF,     keys:"zZｚＺ"},
  {type: SLOT_CRLF,          keys:"nNｎＮ"},
  {type: SLOT_CLEAR,         keys:"cCｃＣ"},
]);


//=============================================================
// 会話スロット情報の解決
//   全体情報：フラグの有無
//   個別情報：サーフェス指定
//-------------------------------------------------------------
var parseSlot = function(text){
  if(typeof(text)!="string"){
    console.error(text);
    throw "文字列以外を引数にはできません";
  }
  var rc = {
    SpeedSection: false,
    QuickSection: false,
    ZeroCRLF    : false,
    Clear       : false,
    CRLF          : 0,
    changeSlot    : {},
    actor         : null,
  };
  var actor     = null;
  var actorSlot = null;
  var numText   = null;
  var numActor  = null;

  // サーフェス番号の確定
  var commitSurfaceNumber = function(){
    if(numText  == null) return;
    if(numActor == null){
      console.warn("直前がスロット指定ではない");
      return;
    }
    numText -=0;
    numActor.surface = actorSlot.surface["$"+numText];
    rc.changeSlot[numActor.pos] = numActor;
    numActor = null;
    numText  = null;
  }

  for(var i=0; i<text.length; i++){
    var slot = slots[text[i]];
    if(slot == null){
      console.warn("スロット適用外文字["+ text[i] +"]");
      commitSurfaceNumber();
      continue;
    }
    switch(slot.type){
      case SLOT_NUMBER:
        if(numActor == null){
          console.warn("直前がスロット指定ではない");
        }
        else{
          if(numText == null) numText = "";
          numText += slot.value;
        }
        continue;
      case SLOT_ACTOR:
        commitSurfaceNumber();
        actorSlot = slot;
        actor = {
          name:     actorSlot.name,
          fullname: actorSlot.fullname,
          pos:      actorSlot.pos,
        };
        numActor = actor;
        rc.actor = actor;
        continue;
      case SLOT_CRLF:
        if(rc.CRLF==0)  rc.CRLF = 1;
        else            rc.CRLF += 0.5;
        break;
      case SLOT_SPACE:                                 break;
      case SLOT_SPEED_SECTION: rc.SpeedSection = true; break;
      case SLOT_QUICK_SECTION: rc.QuickSection = true; break;
      case SLOT_ZERO_CRLF:     rc.ZeroCRLF     = true; break;
      case SLOT_CLEAR:         rc.Clear        = true; break;
    }
    commitSurfaceNumber();
  }
  commitSurfaceNumber();

  return rc;
}


//=============================================================
// 公開
Ranka.Talk.setSlot   = setSlot;
Ranka.Talk.parseSlot = parseSlot;

//=============================================================
})();
