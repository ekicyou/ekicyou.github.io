//ｸｯｷｰに一時記憶する汎用関数1
function tempCookie(sName,sValue){
  life =new Date();
  //次行ではｸｯｷｰの寿命を30日に設定。寿命を変えるには30を修正
  life.setTime(life.getTime() +24*60*60*1000*sDay);
  document.cookie =sName +"=" +escape(sValue);
}

//ｸｯｷｰに記憶する汎用関数1
function setCookie(sName,sValue,sDay){
  life =new Date();
  //次行ではｸｯｷｰの寿命を30日に設定。寿命を変えるには30を修正
  life.setTime(life.getTime() + 24*60*60*1000*sDay);
  document.cookie =sName +"=" +escape(sValue) +"; expires=" +life.toGMTString();
}

//ｸｯｷｰから値を得る汎用関数2
function getCookie(sName) {
  var cookieArray = document.cookie.split("; ");
  for(var i=0; i<cookieArray.length; i++)  {
    var aCrumb =cookieArray[i].split("=");
    if(sName == aCrumb[0]){
      return unescape(aCrumb[1]);
    }
  }
  return null;
}

//ｸｯｷｰを削除する汎用関数3
function delCookie(sName) {
  document.cookie = sName + "=" + escape('') + "; expires=Fri, 31 Dec 1999 23:59:59 GMT;";
}
