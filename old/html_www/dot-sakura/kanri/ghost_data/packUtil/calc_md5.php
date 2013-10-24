<?php
  $src =realpath(stripslashes($_ENV['src']));
  $fList =array();
  calc($src ,'' ,$flist);

  $buf_dau ="";
  $buf_txt ="";
  foreach($flist as $x){
    $a =chr(1);
    $buf_dau .=$x['FILE'] .$a .$x['MD5'] .$a ."\r\n";
    $buf_txt .='file,' .$x['FILE'] .$a .$x['MD5'] .$a ."\r\n";
  }
  @mkdir("$src\\ghost");
  @mkdir("$src\\ghost\\master");
  file_put_contents("$src\\updates2.dau" ,$buf_dau);
  file_put_contents("$src\\ghost\\master\\updates2.dau" ,$buf_dau);
  file_put_contents("$src\\ghost\\master\\updates.txt" ,$buf_txt);


function calc($baseDir ,$prefix ,&$flist){
  $dirName ="$baseDir\\$prefix";
  $d =dir($dirName);
  while (false !== ($entry = $d->read())) {
    if($entry=='.') continue;
    if($entry=='..') continue;
    $path ="$dirName\\$entry";
    $file ="$prefix\\$entry";
    if(is_dir($path)){
      calc($baseDir ,$file ,$flist);
      continue;
    }
    if(is_file($path)){
      unset($x);
      $x['FILE'] =str_replace('\\' ,'/' ,substr($file ,1));
      $x['MD5' ] =$md5 =md5_file($path);
      $flist[] =$x;
    }
  }
}

function file_put_contents($fname, $buf){
  $fp =fopen($fname ,"wb");
  fwrite($fp ,$buf);
  fclose($fp);
}



?>