上書きしたいイベントが存在する場合はここに置く

#イベントのサーチ順位 イベントの戻り値にTRUEが返るまで順次実行される
  1.usr_event_(イベント名で指定された名称)
  2.dot_event_(イベント名で指定された名称)
  3.usr_event_NotFound
  4.dot_event_NotFound

#イベント処理中に例外が発生した場合のサーチ順位
  e1.usr_event_ServerError
  e2.dot_event_ServerError

