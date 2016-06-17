$(function() {
    //スクロールスピード
    var speed = -300;

    var scroll_webkit = function(mov){
        var el = $('body')
        var oldL = el.scrollLeft();
        var newL = oldL - mov * speed;
        el.scrollLeft(newL);
        var check = el.scrollLeft();
        return newL === check;
    }

    var scroll_ie = function(mov){
        var x = window.pageXOffset;
        var oldY = window.pageYOffset;
        var y = oldY + mov * speed;
        window.scrollTo(x,y);
    }

    var scrollFunc = function(mov){
      var rc = scroll_webkit(mov);
      if( rc ) scrollFunc = scroll_webkit;
      else{
        scrollFunc = scroll_ie;
        scroll_ie(mov);
      }
    }

    //マウスホイールで横移動
    $(document).mousewheel(function(event, mov) {
      //scroll_webkit(mov);
      //scroll_ie    (mov);
      scrollFunc(mov);
      return false;   //縦スクロール処理を禁ずる
    });
});