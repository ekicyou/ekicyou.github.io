// あちこちで拾ったユーティリティ関数を全部突っ込んでおく試み
(function() {

// RegExpのエスケープ
var __specials__ = "/.*+?|()[]{}\\".split("");
var __sre__ = new RegExp("(\\" + __specials__.join("|\\") + ")", "g");
RegExp.escape = function(text) { return text.replace(__sre__, "\\$1"); }

})();