/* あくたーもどきライブラリ */
(function(){

JActor = {
    Version: '0.1',
    MID: 1,
};

//=============================================================
// Seq
var seq = function(k,v){
  this.key   = k;
  this.value = v;
}
seq.prototype.add = function(k,v){
  var s = new seq(k,v);
  return this.chain(s);
}
seq.prototype.chain = function(s){
  if(this.key > s.key){
    s.next = this;
    return s;
  }
  var base = this;
  while(true){
    if(base.next === undefined){
      base.next = s;
      break;
    }
    if(base.next.key > s.key){
      s.next    = base.next;
      base.next = s;
      break;
    }
    base = base.next;
  }
  return this;
}

// seqheader
var seqHeader = function(){
}
seqHeader.prototype.add = function(k,v){
  if(this.top === undefined){
    this.top = new seq(k,v);
  }
  else{
    this.top = this.top.add(k,v);
  }
  return this;
}
seqHeader.prototype.remove = function(){
  if(this.top === undefined){
    return null;
  }
  var rc = this.top;
  if(rc.next === undefined) this.top = undefined;
  else                      this.top = rc.next;
  return rc;
}


//=============================================================
// ディスパッチャー
var wakeUpNow = new Date();
var sleepQueue = new seqHeader();
var jobQueue = new seqHeader();

// 起床処理
var wakeUpJob = function(){
  var item;
  while(true){
    if(sleepQueue.top ===undefined) return;
    if(sleepQueue.top.key > wakeUpNow) return;
    item = sleepQueue.remove();
    jobQueue.add(item.value.key, item.value.value);
  }
}

// ジョブ実行
var runJobQueue = function(){
  var item;
  while(true){
    item = jobQueue.remove();
    if(item==null) return;
    try{
      item.value();
    }
    catch(e){
      JActor.Dispatcher.onError(e);
    }
  }
}

// ディスパッチャー API
JActor.Dispatcher = {
  run: function(){
    wakeUpNow = new Date();
    while(true){
      wakeUpJob();
      if(jobQueue.top === undefined) return;
      runJobQueue();
    }
  },
  invoke: function(level, job){
    jobQueue.add(level, job);
  },
  sleepInvoke: function(time, level, job){
    var item = {
      key: level,
      value: job,
    };
    sleepQueue.add(time, item);
  },
  onError: function(e){
    console.error(e);
  },
}


//=============================================================
// ポート
JActor.Port = function(name) {
  this.name = name;
  this.agent = null;
  this.mQueue = new Array();
}

// toString
JActor.Port.prototype.toString = function() {
  return "port[" + this.name + "]";
};

// 接続
JActor.Port.prototype.connect = function(agent) {
  agent.toString = function(){
    return "agent[" + this.name + "]";
  };
  console.info(this.toString() + " connect << " + agent );
  this.agent = agent;
  if(agent.port === undefined){
    agent.port = new JActor.Port(agent.name);
  }
  return agent.port;
};

// 切断
JActor.Port.prototype.disconnect = function(agent) {
  console.info(this.toString() + " disconnect >< " + agent);
  this.agent = null;
  return this;
};

// 全切断
JActor.Port.prototype.disconnectAll = function() {
  console.info(this.toString() + " disconnectAll");
  this.agent = null;
  return this;
};

// 受信
JActor.Port.prototype.send = function(mes) {
  var sel = "onReceive";
  this.select(sel, mes);
};

// 選択受信
JActor.Port.prototype.select = function(sel, mes) {
  console.info(this.toString() + " << send[" + sel + "]");
  this.mQueue.push(mes);
  var agent = this.agent;
  var queue = this.mQueue;
  while(agent != null){
    try{
      var item = queue.shift();
      if(item == null) break;
      if(agent[sel] !== undefined){
        console.info(this.toString() + " >> " + agent + "." + sel);
        agent[sel](this, item);
      }
      else{
        console.info(this.toString() + " >> " + agent + "." + sel  + " [not found]");
        console.info(this.toString() + " >> agent.onReceive");
        agent.onReceive(this, item);
      }
    }
    catch(e){
      if(agent.onError !== undefined) agent.onError(e);
      else              this.onError(e);
    }
  }
};

// キュー取りだし
JActor.Port.prototype.tryReceive = function(mes) {
  return this.mQueue.shift();
};

// エラー処理
JActor.Port.prototype.onError = function(e) {
  console.error(e);
};

})();
