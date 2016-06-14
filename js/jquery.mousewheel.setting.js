$(function() {
    //スクロールスピード
    var speed = -300;

    var scroll_webkit = function(mov){
        var el = $('body')
        var oldPos = el.scrollLeft();
        var newPos = oldPos - mov * speed;
        el.scrollLeft(newPos);
    }

     var scroll_ie = function(mov){
        var x = window.pageXOffset;
        var oldY = window.pageYOffset;
        var y = oldY + mov * speed;
        window.scrollTo(x,y);
    }

    //マウスホイールで横移動
    $(document).mousewheel(function(event, mov) {
        scroll_webkit(mov);
        scroll_ie    (mov);

        return false;   //縦スクロール不可
    });
});