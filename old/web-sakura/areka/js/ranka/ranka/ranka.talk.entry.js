// RANKA トーク辞書：登録処理
(function(){
//=============================================================
// 識別定数
//-------------------------------------------------------------
Ranka.id.talk.NORMAL_TALK = "＊";


//=============================================================
// トーク辞書
//-------------------------------------------------------------
var talkDic = TAFFY([]);


//=============================================================
// 会話辞書エントリ管理クラス
//-------------------------------------------------------------
var TalkEntry = function(id){
  this.id   = id;
  this[id]  = true;
  this.item = [];
}

TalkEntry.prototype.setEvalScope = function(func){
  this.evalscope = func;
  return this;
};



TalkEntry.prototype.talk = function(arg){
  if(!(arg instanceof Array)) arg = [arg];
  for(var i=0; i<arg.length; i++)
    this.item.push({cmd: "talk", value: arg[i]});
  return this;
};

TalkEntry.prototype.chain = function(arg){
  this.item.push({cmd: "chain"});
  if(arg === undefined) return this;
  return this.talk(arg);
};

TalkEntry.prototype.jump = function(arg){
  this.item.push({cmd: "jump", value: arg});
  return this;
};

TalkEntry.prototype.clean = function(arg){
  this.item.push({cmd: "clean", value: arg});
  return this;
};


//=============================================================
// トップレベル登録処理
//-------------------------------------------------------------
var createEntry = function(id, items){
  var entry = new TalkEntry(id);
  talkDic.insert(entry);
  if(items !== undefined) entry.talk(items);
  return entry;
}



//=============================================================
// 公開
Ranka.Talk.entry          = createEntry;
Ranka.Talk.dic            = talkDic;
Ranka.Talk.TalkEntryClass = TalkEntry;



//=============================================================
})();
