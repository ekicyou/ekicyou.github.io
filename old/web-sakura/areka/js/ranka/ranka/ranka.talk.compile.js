// RANKA トーク辞書：関数展開処理（コンパイル）
(function(){

//=============================================================
// 識別定数
//-------------------------------------------------------------


//=============================================================
// ウェイト制御(単位はms)
//-------------------------------------------------------------
var waitChars = {
  skip: "）］｝」』)]}｣”’"+'"'+"'",
  p1:   "。｡．.",
  p2:   "？！?!",
  p3:   "、､，,",
  w:    "…‥",
  wswap: "…",
};
var waitSpeed = {
  p1: 450,      // p1が現れたときの追加wait
  p2: 200,      // p2が現れたときの追加wait
  p3: 100,      // p3が現れたときの追加wait
  w1:  50,      // wが１文字現れたときの追加wait
  w2:  50,      // wが連続で現れたときに最後につける追加wait
  change: 450,  // スコープ変更の追加wait
  normal: 50, // 通常文字のwait値
};

var reWaitSplitG;
var reWaitSplit;
var reCheckP1, reCheckP2, reCheckP3, reCheckW;
var reSwapW;
var waitCharSwap = null;

// スクリプトウェイト分割用regex作成
var compileWaitRegs = function(wc){
  var skip = RegExp.escape(wc.skip);
  var p1   = RegExp.escape(wc.p1);
  var p2   = RegExp.escape(wc.p2);
  var p3   = RegExp.escape(wc.p3);
  var w    = RegExp.escape(wc.w);
  waitCharSwap = null;
  while(true){
    if(wc.wswap == null) break;
    if(typeof(wc.wswap) != "string") break;
    if(wc.wswap.length == 0) break;
    waitCharSwap = wc.wswap[0];
    break;
  }
  // キャラクタークラス作成
  var cclass = function(){
    var rc = "";
    for(var i=0; i<arguments.length; i++){
      rc += arguments[i];
    }
    return "[" + rc + "]";
  }
  var notclass = function(){
    var rc = "";
    for(var i=0; i<arguments.length; i++){
      rc += arguments[i];
    }
    return "[^" + rc + "]";
  }
  var cP123  = cclass(p1,p2,p3);
  var cSP    = cclass(p1,p2,p3,skip);
  var cW     = cclass(w);
  var cN$    = notclass(p1,p2,p3,w,"$＄");
  var cNOT   = notclass(p1,p2,p3,w,"0-9０-９");

  // 正規表現
  var rT  = cN$;
  var rP  = cP123            + cSP +"*";
  var rW  = "(" + cW + "+)(" + cSP +"*)";
  var r$  = "[$＄](([0-9０-９])|("+cNOT+"?))";
  var re  = "("+rT+"+)|("+r$+")|("+rW+")|("+rP+")";
  reWaitSplitG = new RegExp(re,"g");
  reWaitSplit  = new RegExp(re);
  reCheckP1    = new RegExp(cclass(p1));
  reCheckP2    = new RegExp(cclass(p2));
  reCheckP3    = new RegExp(cclass(p3));
  reCheckW     = new RegExp(cclass(w));
  reSwapW      = new RegExp(cclass(w),"g");
};
compileWaitRegs(waitChars);

// ピリオドレベル(p1～p1)の確認。
var checkPeriodLevel = function(text){
  if(text.search(reCheckP1)>=0) return "p1";
  if(text.search(reCheckP2)>=0) return "p2";
  if(text.search(reCheckP3)>=0) return "p3";
  return "";
}

var swapID={};
swapID["0"]="$0";
swapID["1"]="$1";
swapID["2"]="$2";
swapID["3"]="$3";
swapID["4"]="$4";
swapID["5"]="$5";
swapID["6"]="$6";
swapID["7"]="$7";
swapID["8"]="$8";
swapID["9"]="$9";
swapID["０"]="$0";
swapID["１"]="$1";
swapID["２"]="$2";
swapID["３"]="$3";
swapID["４"]="$4";
swapID["５"]="$5";
swapID["６"]="$6";
swapID["７"]="$7";
swapID["８"]="$8";
swapID["９"]="$9";

//=============================================================
// スクリプト句読点分割
//-------------------------------------------------------------
var splitWait = function(text){
  var items = [];
  var lastText = null;

  // 投入関数
  var addText=function(t){
    if(lastText == null){
      items.push({text: t});
      lastText = items[items.length-1];
      return;
    }
    lastText.text += t;
  }
  var addObj=function(obj){
    lastText = null;
    items.push(obj);
  }
  var addSwap=function(sp){
    addObj({swap: swapID[sp]});
  }
  var addWait=function(w,p){
    if(waitCharSwap != null){
      w = w.replace(reSwapW, waitCharSwap);
    }
    var o = {wait: w};
    if(p.length > 0){
      o.period = p;
      o.level  = checkPeriodLevel(p);
    }
    addObj(o);
  }
  var addPeriod=function(p){
    addObj({
      period: p,
      level: checkPeriodLevel(p),
    });
  }

  // マッチ
  var sp = text.match(reWaitSplitG);

  for(var i=0; i<sp.length; i++){
    var m = sp[i].match(reWaitSplit);
    if(m.length != 10) continue;
    if(m[1] !== undefined){ addText(m[0])     ; continue;}
    if(m[4] !== undefined){ addSwap(m[4])     ; continue;}
    if(m[2] !== undefined){ addText(m[0])     ; continue;}
    if(m[6] !== undefined){ addWait(m[7],m[8]); continue;}
    if(m[9] !== undefined){ addPeriod(m[9])   ; continue;}
  }
  return items;
}


//=============================================================
// 会話スクリプト１行→会話AST変換
// スロット：本文の分解と、本文の句読点判別、
// テンプレート事前分解($0～$9)まで行う
//-------------------------------------------------------------
var compileTalkLine = function(text){
  var sp = text.search(/[:：]/i);
  var slot   = text.substr(0, sp);
  var script = text.substr(sp+1);
  return {
    slot  : slot.replace(/[\t 　]/,'') ,
    script: splitWait(script),
  };
}


//=============================================================
// トークエントリの圧縮
//-------------------------------------------------------------
var packEntry = function(item){
  var rc = [];
  var lastSeq = null;
  for(var i=0; i<item.length; i++){
    var entry = item[i];
    if(entry.seq === undefined){
      lastSeq = null;
      rc.push(entry);
      continue;
    }
    if(lastSeq == null){
      rc.push(entry);
      lastSeq = entry;
      continue;
    }
    for(var j=0; j<entry.seq.length; j++){
      var entj = entry.seq[j];
      if(entj==null) continue;
      lastSeq.seq.push(entj);
    }
  }
  return rc;
}


//=============================================================
// トップレベルトークエントリコンパイル処理
//-------------------------------------------------------------
var compileEntry = function(t){
  var item = this.item;
  var rc = {item:[],};

  for(var i=0; i<item.length; i++){
    try{
      var entry = item[i];
      if(entry.cmd === undefined) continue;
      var compiled = null;
      switch(entry.cmd){
        case "talk":
          compiled = {
            seq: [compileTalkLine(entry.value)]
          };
          break;
      }
      if(compiled != null) rc.item.push(compiled);
    }
    catch(e){
      console.error(e);
    }
  }
  rc.item = packEntry(rc.item);

  this._$ = rc;
  return rc;
}


//=============================================================
// 公開
Ranka.Talk.setWaitChars    = compileWaitRegs;
Ranka.Talk.waitDic       = waitSpeed;
Ranka.Talk.splitWait       = splitWait;
Ranka.Talk.compileTalkLine = compileTalkLine;
Ranka.Talk.TalkEntryClass.prototype.compile = compileEntry;


//=============================================================
})();
