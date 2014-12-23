// RANKA トーク辞書：内部表現をさくらスクリプトに展開
(function(){
//=============================================================
// 展開領域の初期化
//-------------------------------------------------------------
var initSakuraScript = function(t){
  var t = {
    waitDic: Ranka.Talk.waitDic,  // 待ち時間パターンの辞書
    waitTime: 0,  // 次に会話が入る直前に入る待ち時間(ms)。
    env: Ranka.env,
    talkedActor: {},
    pos: -1,
  };
  t.env.lastActor = null;
  return t;
}


//=============================================================
// さくらスクリプト展開
//-------------------------------------------------------------
var getSakuraScript = function(t, entry){
  var sc = "";
  var waitDic = t.waitDic;
  var env = t.env;
  var slot = Ranka.Talk.parseSlot(entry.slot);

  var changeSlot = slot.changeSlot;
  var actor = slot.actor;
  var isActorChange   = false;// アクター変更あればtrue

  // 会話するアクターの決定
  if(actor != null){
    if(env.lastActor != null && env.lastActor.pos != actor.pos){
      isActorChange = true;
    }
    var lastSlot = env.lastSlot[actor.pos];
  }
  else if(env.lastActor != null){
    actor = env.lastActor;
  }
  else{
    // 暫定
    actor = {pos: 0, surface: 0};
  }

  // ウェイト指定関数
  var addWait=function(){
    if(slot.SpeedSection){
      t.waitTime = 0;
      return;
    }
    // 50で丸め
    var remain = Math.floor(t.waitTime / 50);
    while(remain > 0){
      var w = remain;
      if(w > 9) w = 9;
      sc += "\\w" + w;
      remain -= w;
      t.waitTime -= w * 50;
    }
  }

  // ポジション指定関数
  var changePos=function(newpos){
    if(t.pos == newpos) return;
    addWait();
    switch(newpos){
      case 0: sc+="\\0"; break;
      case 1: sc+="\\1"; break;
      default:
        sc += "\\p["+ newpos +"]";
    }
    t.pos = newpos;
  }

  // サーフェス指定関数
    var addSurface=function(list){
    if(list.length < 1) return;
    addWait();
    for(var i=0; i<list.length; i++){
      var ss = list[i];
      changePos(ss.pos);
      sc += "\\s["+ss.surface+"]";
    }
  }

  // サーフェス変更の確認
  var slist1 = [];
  var slist2 = [];
  var slist3 = [];
  for(var key in slot.changeSlot){
    var ss = slot.changeSlot[key];
    if(ss.pos === undefined) continue;
    var lastSlot = env.lastSlot[ss.pos];
    if(lastSlot != null && lastSlot.surface == ss.surface) continue;
    if(actor.pos == ss.pos) slist3.push(ss);
    else if(env.lastActor != null && env.lastActor.pos == ss.pos){
      slist1.push(ss);
    }
    else slist2.push(ss);
  }

  // 開始時ウェイト
  if(isActorChange) t.waitTime += waitDic.change;

  // サーフェス変更
  addSurface(slist1);
  addSurface(slist2);
  addSurface(slist3);
  changePos(actor.pos);

  // 開始時改行 or バルーンクリア
  if(slot.Clear)  sc += "\\c";
  var CRLF = slot.CRLF * env.lineCRLF;
  if(CRLF != 0);
  else if(slot.Clear);
  else if(slot.ZeroCRLF);
  else if(t.talkedActor[actor.pos] === undefined);
  else if(isActorChange)  CRLF = env.changeCRLF;
  if(CRLF == 1)     sc += "\\n";
  else if(CRLF > 0) sc += "\\n[" + CRLF + "]";

  // script展開
  var addText = function(text){
    if(text == null) return;
    addWait();
    sc += text;
  }
  // ‥‥展開
  var addTenTen = function(text){
    if(text == null) return;
    for(var i=0; i<text.length; i++){
      addWait();
      sc += text[i];
      t.waitTime += waitDic.w1;
    }
  }

  // スクリプト展開ループ
  for(var i=0; i<entry.script.length; i++){
    var item = entry.script[i];
    addText  (item.text);
    addTenTen(item.wait);
    addText  (item.period);
    if(item.level != null)  t.waitTime += waitDic[item.level];
  }

  // クイックセクション
  if(slot.QuickSection){
    sc = "\_q" +sc +"\_q";
  }

  // 後処理
  env.lastActor            = actor;
  env.lastSlot[actor.pos]  = actor;
  t.talkedActor[actor.pos] = true;
  return sc;
}


//=============================================================
// 公開
Ranka.Talk.TalkEntryClass.prototype.initTalk  = initSakuraScript;
Ranka.Talk.TalkEntryClass.prototype.getScript = getSakuraScript;

//=============================================================
})();
