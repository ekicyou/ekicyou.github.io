// こんな辞書記述はどうかという折衷案
(function(t){
//=============================================================
// 会話スロットの設定
var baseSlotSet = [
  { name: "エミリ", fullname: "メカエミリ", pos: 0, keys: "えエ", surface:{
    $0: 0,
    $1: 1,
    $2: 2,
    $3: 3,
    $4: 4,
    $5: 5,
    $6: 6,
    $7: 7,
    $8: 8,
    $9: 9,
  }},
  { name: "さくら", fullname: "メカさくら", pos: 1, keys: "さサ", surface:{
    $0: 10,
    $1: 11,
    $2: 12,
    $3: 13,
    $4: 14,
    $5: 15,
    $6: 16,
    $7: 17,
    $8: 18,
    $9: 19,
  }},
];
t.setSlot(baseSlotSet);


//-------------------------------------------------------------
// トーク登録
SugarTest()
  .context('スロット解釈')
    .it("パーサ試験１", function(){
      var rc = Ranka.Talk.parseSlot("さ０");
      this.assertEqual("メカさくら", rc.actor.fullname);
      this.assertEqual(           1, rc.actor.pos);
      this.assertEqual(          10, rc.actor.surface);
      this.assertEqual(       false, rc.SpeedSection);
      this.assertEqual(       false, rc.SpeedSection);
      this.assertEqual(       false, rc.ZeroCRLF);
    })
    .it("パーサ試験２", function(){
      var rc = Ranka.Talk.parseSlot("エ２ｑｓｚ");
      this.assertEqual("メカエミリ", rc.actor.fullname);
      this.assertEqual(           0, rc.actor.pos);
      this.assertEqual(           2, rc.actor.surface);
      this.assertEqual(        true, rc.SpeedSection ,"SpeedSection");
      this.assertEqual(        true, rc.SpeedSection ,"SpeedSection");
      this.assertEqual(        true, rc.ZeroCRLF     ,"ZeroCRLF");
    })

  .root()
  .context('トーク登録')
    .it("朝", function(){
      var rc = t.entry("朝",[
        "エ０さ０：朝だー、朝だよー、朝ごはん食べて寝るよ。",
        "さ６　：‥‥ぐう。",
        "エ２ｓ：寝るんだ！？",
        "エ７ｎ：こら、早く起きなさい！",
        "さ　　：すやすや‥‥。",
        "エ４　：低電圧だなぁ。",
      ]).jump("朝ご飯分岐");
      var log=function(){
        return JSON.stringify(rc,null,"  ");
      }

      this.assertEqual("朝", rc.id);
      this.assertEqual(true, rc["朝"]);
      this.assertEqual(7   , rc.item.length, log());
      // コンパイル
      var comp = rc.compile(t);
      this.assertEqual(1, comp.item.length, log());

      // スクリプト変換
      var exec = rc.initTalk(t);
      var sc = [];
      var check = [
        "\\0\\s[0]\\1\\s[10]朝だー、\\w2朝だよー、\\w2朝ごはん食べて寝るよ。",
        "\\w9\\s[16]…\\w1…\\w1ぐう。",
        "\\0\\s[2]寝るんだ！？",
        "\\w4\\s[7]\\nこら、\\w2早く起きなさい！",
        "\\w9\\w4\\1\\n[1.5]すやすや…\\w1…\\w1。",
        "\\w9\\w9\\0\\s[4]\\n[1.5]低電圧だなぁ。"
      ];

      for(var i=0; i<comp.item[0].seq.length; i++){
        sc.push(rc.getScript(exec, comp.item[0].seq[i]));
        this.assert(check[i] == sc[i], "\ncheck:[" +check[i] + "]\nact:[" +sc[i] +"]");
      }
    })

    .it("朝ご飯分岐", function(){
      var rc = t.entry("朝ご飯分岐",[
        "さ０　：フレンチトーストとロイヤルミルクティがあるなら起きてやっても良い。",
        "エ４　：どこのおじょーさんですか‥‥。",
      ]).chain([
        "さ０　：あ、納豆定食でもいいよ？",
        "エ２ｑ：だからどうやって消化するの！？",
      ]);
      this.assertEqual("朝ご飯分岐", rc.id);
      this.assertEqual(true, rc["朝ご飯分岐"]);
      this.assertEqual(5   , rc.item.length,JSON.stringify(rc,null,"  "));
    })

    .it("朝ご飯分岐2", function(){
      var rc = t.entry("朝ご飯分岐",[
        "さ４　：電池が暖まってこないと電圧が出ないんだよぅ。",
        "エ０　：‥‥あー。",
      ]);
    })

    .it("ジャンプ試験", function(){
      var rc = t.entry("ジャンプ試験",[
        "え０：ジャンプ試験だよ。",
      ]).jump("分岐先");
    })

    .it("分岐先", function(){
      var rc = t.entry("分岐先",[
        "さ０：分岐先１に飛んだ～。",
      ]);
    })

    .it("分岐先2", function(){
      var rc = t.entry("分岐先",[
        "さ０：分岐先２に飛んだ～。",
      ]);
    })

    .it("呼び出し試験", function(){
      var rc = t.entry("呼び出し試験",[
        "え０：呼び出し試験だよ。",
      ]).jump("分岐先").talk([
        "え５：続けて書いてたら戻ってくるよ。",
      ]);
    })

    .it("関数で作る場合", function(){
      var rc = t.entry("関数で作る場合").talk( function(t){
        t.talk("エ０：関数を作って、そこで会話を記述することも出来るよ。");
        t.talk("さ０：本当はもう１段分解されるけどね。");
        t.talk("エ９：そうなの？");
        t.talk("さ８：わけわかんないのがお望みならどーぞ。");
        t.talk("エ４：‥‥けっこーです。");
      }).chain( function(t){
        t.talk("エ０：中断されなかったよ。");
        t.talk("さ７：もし中断されてたら例外出てるから必要に応じてキャッチだ！");
      });
    })

    .it("もっとも展開", function(){
      var rc = t.entry("もっとも展開された関数").talk( function(t){
        var s = t.slot;
        t.addElement(
          {seq:[
            {slot:"エ０", script:[
              {text: "最初の引数がスロット指定、",   level: "p3"},
              {text: "２つめ以降はスクリプトだよ。", level: "p1"},
            ]},
            {slot:"さ０", script:[
              {text: "ここは展開後の記述だからウェイトも自分で何とかしないと駄目。", level: "p1"},
            ]},
          ],});
      }).chain( function(t){
        t.addElement(
          {seq:[
            {slot:"エ０", script:[
              {text: "sendの戻りがtrueだったみたい。", level: "p1"},
            ]},
            {slot:"さ０", script:[
              {text: "trueの時はチェーン中断されなかった印だから続けて会話だね。", level: "p1"},
            ]},
          ]});
      });
    })

  .root()
.run();


//=============================================================
})(Ranka.Talk);
