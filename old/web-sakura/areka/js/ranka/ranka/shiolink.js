// SHIOLINK 接続API( &raw shiori api )
(function(){
//=============================================================
// 基本変数
loaddir = null; // ルートディレクトリ
portRawShiori = new JActor.Port("栞：RawRequest");
shiolink = {
  readLine: null,   // 読み込み関数を設定すること
  writeLine: null,  // 書き込み関数を設定すること
};
M_SHIORI_GET       =JActor.MID++;
M_SHIORI_NOTIFY    =JActor.MID++;


//=============================================================
// イベントハンドリング



//=============================================================
// SHIORIイベント切りだし(イベントを１つ切り出して返す)
readRequest = function(){
  var request, line, ss;
  mode = 1;
  request        = new Object();
  request.method = null;
  request.uuid   = null;
  request.date   = null;
  request.item   = new Object();

  while (true){
    line = shiolink.readLine();
    if( line === null ) return null;
    line = line.replace(/[ 　\t\r\n]*$/gim, "");

    switch(mode){
    case 1: // *------- コマンドモード
      var cmd = line.substr(0,3);
      switch(cmd){
      case "*L:":
        loaddir = line.substr(3);
        continue;
      case "*U:":
        return null;
      case "*S:":
        // seq応答
        shiolink.writeLine(line);
        request.uuid = line;
        mode = 2;
      }
      continue;

    case 2: // *------- request method受信モード
      switch(line){
      case "GET SHIORI/3.0":    request.method = M_SHIORI_GET   ; break;
      case "NOTIFY SHIORI/3.0": request.method = M_SHIORI_NOTIFY; break;
      default:
        request.date   = new Date();
        return request;
      }
      mode = 3;
      continue;

    case 3: // *------- request item受信モード
      if(line.length == 0){
        request.date   = new Date();
        return request;
      }
      ss = line.split(": ");
      request.item[ss[0]] = ss[1];
    }
    continue;

  }
  return null;
}


//=============================================================
// SHIORIイベント処理エージェント
var genShioriAgent = function(){
  var agent = {
    name: "栞：Request処理",
    portGet:    new JActor.Port("栞：SHIORI GET"),
    portNotify: new JActor.Port("栞：SHIORI NOTIFY"),

    onReceive: function(port ,mes){
      var result = {
        request : mes,
      };
      try{
        switch(mes.method){
        case M_SHIORI_GET:
          agent.portGet.send(mes);
          break;
        case M_SHIORI_NOTIFY:
          agent.portNotify.send(mes);
          break;
        default:
          throw "NOT SHIORI REQUEST";
        }
      }
      catch(e){
        console.error(e);
        result.code  = 400;
        result.error = e;
        agent.port.send(result);
        result = null;
      }
      // 最終的に揃ったイベントを基に応答をキック
      agent.port.select("onOrgMessage", mes);
      // 何もイベントが発生しなかったときに204を出すため
      if(result != null){
        result.code  = 204;
        agent.port.send(result);
      }
    },
  };
  return agent;
}


//=============================================================
// SHIORI応答エージェント
//   以下２つのメッセージが揃ったときに起動
//   onReceive    : 応答データ
//   onOrgMessage : オリジナルメッセージ
var genShioriResultAgent = function(writeLineFunc){
  var resultCode = {
      OK            : "SHIORI/3.0 200 OK",
      NoContent     : "SHIORI/3.0 204 No Content",
      BadRequest    : "SHIORI/3.0 400 Bad Request",
      ServerError   : "SHIORI/3.0 500 Internal Server Error",
  };
  var response = null;
  var request  = null;

  var addItems = function(lines, res){
    if(res.items === undefined) return;
    for(key in res.items){
      var value = res.items[key];
      if(typeof(value) != "string") continue;
      lines.push(key + ": " + value);
    }
  };

  var complite = function(){
    if(response == null) return;
    if(request == null) return;
    if(response.request !== request){
      response = null;
      return;
    }

    var lines = [];
    lines.push("Charset: UTF-8");
    lines.push("Sender: Ranka");
    var resultMes;
    switch(response.code){
      case 200:
        resultMes = resultCode.OK;
        addItems(lines, response);
        break;
      case 400:
        resultMes = resultCode.BadRequest;
        break;
      case 500:
        resultMes = resultCode.ServerError;
        break;
      default:
        resultMes = resultCode.NoContent;
    }
    if(response.error !== undefined){
      lines.push("X-Exception: " + response.error);
    }
    for(i = 0; i < lines.length; i++){
      resultMes += "\r\n" + lines[i];
    }
    resultMes +="\r\n\r\n";
    shiolink.writeLine(resultMes);

    response = null;
    request = null;
  };

  var agent = {
    name: "栞：Request応答",
    onReceive: function(port ,mes){
      response = mes;
      complite();
    },
    onOrgMessage: function(port ,mes){
      request = mes;
      complite();
    },
  };
  return agent;
}


//=============================================================
// エージェント接続の作成
shioriAgent = genShioriAgent();

portRawShiori
  .connect(shioriAgent)
  .connect(genShioriResultAgent(shiolink.writeLine))
  ;

//=============================================================
})();
