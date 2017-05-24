# えちょの残骸：新サイト構築
github pagesがJekyll3.0に対応したので、生成を完全にgithubに任せた静的サイトの再構築を行う。windowsにおけるrubyの構築からの記録を残す。

## [Chocolatey](https://chocolatey.org/install)のインストール
任意の場所にインストールしたい場合は、
```chocolateyInstall.ps1```をダウンロードして設定情報をスクリプトに直接記述したほうが良い。
自分が設定したのは下記の情報

```ps1
$env:chocolateyInstall = 'd:\wintools\Chocolatey'
$env:chocolateyProxyLocation = 'http://プロキシサイト:プロキシポート'
$env:chocolateyProxyUser = 'プロキシユーザ'
$env:chocolateyProxyPassword = 'プロキシパスワード'
```

2. 環境変数```ChocolateyToolsLocation```をツールインストールフォルダに変更

## windowsにruby/jekyllをインストール
[Jekyll on Windows](https://jekyllrb.com/docs/windows/#installation)を参考に、rubyのインストールを行う。

1. Chocolateyを使ってrubyをインストール：```choco install ruby -y```
2. github-pages(Jekyll一式)をインストール：```gem install github-pages```
3. インストールされたものを確認：```github-pages versions```

* コマンドシェルは毎回起動しなおす必要がある。

## 試験サイトの構築

```ps1
jekyll new demo
cd demo
rm Gemfile
```

## 起動
```cmd
jekyll s 
```

http://localhost:4000/ にアクセス。

この辺参考にする。
http://qiita.com/takuya0301/items/374b2ab5be407b138ef9
