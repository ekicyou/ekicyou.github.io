// RANKA 単語辞書：登録処理
(function(){
//=============================================================
// トーク辞書
//-------------------------------------------------------------
var wordDic = TAFFY([]);


//=============================================================
// 単語辞書エントリ管理クラス
//-------------------------------------------------------------
var WordEntry = function(keys, values){
  // キー要素の作成
  var kobj = {};
  if(!(keys instanceof Array))  keys = [keys];
  for(var i=0; i<keys.length; i++){
    var item = keys[i];
    if(item instanceof Object) addKeyValue(item);
    else my[item] = true;
  }
  var addKeyValue = function(obj){
    for(var k in obj) my[k] = obj[k];
  }
  this.key = kobj;

  // 要素の追加
  this.$(values);
}


// 要素の追加
WordEntry.prototype.entry = function(values){
  var my = this;

  // 要素の追加
  var add = function(v){
    var obj = {};
    for(var k in this.key)  obj[k] = my.key[k];
    if(v instanceof Function) obj._$get = v;
    else obj._$get = function(){return v;}
    wordDic.insert(obj);
  }

  if(!(values instanceof Array))  values = [values];
  for(var i=0; i<values.length; i++){
    add(values[i]);
  }
  return this;
};

WordEntry.prototype.$ = WordEntry.prototype.entry;


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
Ranka.Word.entry          = createEntry;
Ranka.Word.dic            = wordDic;



//=============================================================
})();
