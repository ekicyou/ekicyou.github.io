$(function() {
    //コンテンツの横サイズ
    var cont = $('#contents');
    var contW = $('.section').outerWidth(true) * $('div',cont ).length;
    cont.css('width', contW);
    //スクロールスピード
    var speed = -50;
    //マウスホイールで横移動
    $('html').mousewheel(function(event, mov) {
        //ie firefox
        $(this).scrollLeft($(this).scrollLeft() - mov * speed);
        //webkit
        $('body').scrollLeft($('body').scrollLeft() - mov * speed);
        return false;   //縦スクロール不可
    });
});