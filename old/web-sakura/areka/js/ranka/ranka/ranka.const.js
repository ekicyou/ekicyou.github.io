// RANKA 基本定義
(function(){
//=============================================================
// 基本変数
Ranka = {
  version: "RANKA_0.1.0.0",
  event: {},
  save: {},
  env: {
    lastSlot: {},     // 各スロットの最終状態
    lastActor: null,  // 最後に発言したアクター
    changeCRLF: 1.5,  // スロット切り替え時改行量
    lineCRLF: 1,      // 通常改行量
  },
  Talk: {},
  lastID: 1,
  id: {
    talk: {},
  },
};


//=============================================================
// 固定値を返すイベント
// 基本定数のうち、文字列値のものを設定
var sendConst = function( mes, value ){
  var result = {
    request: mes,
    code: 200,
    items: {
      Value: value,
    },
  };
  shioriAgent.port.send(result);
}
var constSetter = function(){
  for(var key in Ranka){
    var item = Ranka[key];
    if(typeof(item) != "string") continue;
    Ranka.event[key] =
      eval("function(m){ sendConst(m, Ranka." + key +");}");
  }
}
constSetter();


//=============================================================
// イベントハンドリングエージェント
var shioriEventAgent = function(){
  var agent = {
    name: "栞：イベント分岐",
    onReceive: function(port ,mes){
      var id = mes.item["ID"];
      var func = Ranka.event[id];
      if(func != null) func(mes);
    },
  };
  return agent;
}

shioriAgent.portGet.connect(shioriEventAgent());

//=============================================================
})();
