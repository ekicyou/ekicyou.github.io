# Brief: site-astro-redesign

## Problem

開発者(えちょ)は、サイト「えちょのざんがい」のデザインを変えたい。今の見た目は 2018 年に minima テーマを手元へ写して手を入れたもので、コメント欄を付けた以外に工夫は無い(開発者の言葉、2026-10-11)。

今の生成ツール Jekyll は、この作り直しに向かない。

- 手元に Ruby が無く、画面を見ながら直せない。デザインは「直す → 見る」を何度も繰り返す作業である。
- GitHub Pages の Jekyll は 3.10 と許可済みプラグインに固定されており、新しい作りへの道具立てが弱い。
- 設定と実態が食い違っている。`_config.yml` は `remote_theme: pages-themes/minimal@v0.2.0` を指定しているが、見た目を決めているのは手元の `_layouts/`・`_includes/`・`_sass/minima/` である。

開発者の条件(2026-10-11)は 4 つ。

1. デザイン変更の自由度が高いこと。
2. 技術文書サイトなので、Markdown を持って行ければ基本は問題ない(記事は少ない)。
3. フィードは形式が変わってよい(たぶん誰も見ていない)。
4. コメントは付けられたほうがよい(今のところ誰も書いていない)。

## Current State

`site-subfolder-actions-build` が済んだ時点の姿を前提にする。

- 原稿は `site/` の下にあり、`master` への push で Actions がビルドして公開する。PR ではビルドだけが走る。ビルドは Jekyll(公式アクション `actions/jekyll-build-pages`)。
- 中身:
  - 記事 15 本(`_posts/`)。うち 3 本は `published: false`(`_posts/note/` の 2 本を含む)。
  - 記事に添える文書(`doc/cosmogenesis/` の Markdown 2 本、PDF、TeX、画像)。
  - 画像(`img/`)、単独のページ(`index.md`・`about.md`・`404.html`)。
- 記事と `doc/` の Markdown に、Jekyll 専用の書き方(`{% %}`、`{: }`)は 1 つも無い(2026-10-11 に検索して確かめた)。あるのは素の Markdown と、数式、脚注、`[!NOTE]`、少しの HTML。
- 見た目の部品は minima の写し(`_layouts/` の `default` → `home` / `page` / `post`、`_includes/`、`_sass/minima/`、`assets/main.scss`)。使われていない部品も残っている(`disqus_comments.html`、`google-analytics.html`、SNS アイコンの多く)。
- 記事の機能: コメント欄(giscus、本番ビルドのときだけ)、数式(MathJax をブラウザで動かす。`use_math: true` のときだけ)、SNS 向けの説明と画像、RSS(`/feed.xml`)。
- リモートテーマは、使われていないファイルだけを公開物に足している(`/assets/css/style.css`、`/assets/js/scale.fix.js`)。
- 手元の道具: Node.js 26、npm 12、Docker。Ruby は無い。

## Desired Outcome

- サイトが Astro でビルドされ、新しいデザインで公開されている。
- 公開済みの記事・文書・画像の URL は変わっていない。
- 記事の Markdown は、ほぼ書き直さずに引き継がれている。
- コメント欄、数式、コードの色付け、SNS 向けの情報、フィード(`/feed.xml`)が働く。
- 手元で `npm run dev` を動かし、画面を見ながら直せる。Claude もブラウザで確かめながら手伝える。
- PR ではビルドだけが走り、`master` への push で公開される(1 本目と同じ)。
- Jekyll の部品とテーマ指定は残っていない。

どんなデザインにするか(雰囲気、配色、トップページの見せ方、記事ページの読みやすさ、スマートフォンでの見え方、暗い配色への対応など)は**未定**。要件の話し合いで開発者と決める。

## Approach

生成ツールを **Astro** に乗り換え、レイアウトとスタイルを**一から自前で作る**(開発者の選択、2026-10-11)。既製テーマ(Astro の Starlight を含む)は使わない。選んだ理由と退けた案(Jekyll に留まる、Eleventy、Hugo、Zola)は `.kiro/steering/roadmap.md` の「Approach Decision」。

成立性の確かめ(2026-10-11、下請けによる調査。出典は Astro の公式文書・npm・GitHub)では、行き止まりは見つからなかった。ただし、前もって決めておく点が 3 つある(Markdown の処理系、数式の書き方の揃え直し、出力形式)。下の材料に織り込んである。

- **Astro の現況**: 最新の安定版は 7.3.8(2026-10-08)。MIT ライセンス。Node.js は 22.12 以上が要る(手元の 26 は条件を満たす。26 での動作を明言した文書は見つかっていない)。主版の上がり方が速い(5 が 2024-12、6 が 2026-03、7 が 2026-06。8 の議論も始まっている)。版を固定しておけば放置してもビルドは通るが、いずれ版を上げるときは主版を複数またぐ手間が掛かる。

設計で詰める材料:

- **URL の保ち方**: 今の URL は `….html` で終わる(記事 `/YYYY/MM/DD/<名前>.html`、`/about.html`、`/doc/<題材>/<名前>.html`)。Astro の出力形式をファイル単位(`build.format: 'file'`)にし、記事はファイル名(`YYYY-MM-DD-<名前>.md`)から日付と名前を取り出して URL を作る。記事側に日付を書き足さずに済ませる。
  - `'file'` は、下の階層の `index` を `<フォルダ名>.html` として出す。今のサイトに下の階層の `index` は無いので支障は無いが、作るなら `'preserve'` を選ぶ。
  - ページの URL を表す `Astro.url` は、手元の表示とビルドとで値が違う(既知の不具合)。正規 URL や SNS 向けの URL は、記事のデータから組み立てる。
  - 英数字以外やドットを含むファイル名が URL にどう出るかは未確認。試作で確かめる。
- **Markdown の引き継ぎ**: 記事と文書は Astro のコンテンツ機能(`src/content.config.ts`、`glob` ローダー)で読み、front matter を検査する。今の front matter(`layout`・`title`・`description`・`image`・`tags`・`comments`・`use_math`・`published`)は受け入れ、`published: false` は今と同じく公開しない。
  - 既定では、ファイル名から作る識別子が小文字化・記号落としされる。URL を保つため、ファイル名をそのまま識別子にする指定(`generateId`)を入れる。
  - 検査の定義に無い項目は、黙って捨てられる(エラーにならない)。使う項目は任意の項目として定義する。
- **Markdown の処理系(前もって決める)**: Astro 7 から、既定の処理系が Rust 製の Sätteri に替わった。従来の remark / rehype のプラグインは、この上では動かない。数式と `[!NOTE]` のためのプラグインは従来の側に揃っており、Sätteri 向けは出て数週間の個人製しか無い。**従来の処理系(`@astrojs/markdown-remark` の `unified()`)を明示して選ぶ**のが堅い。既定でない側を使うことになる点は、版上げのたびに見直す。
- **数式(唯一の実害になり得る点)**: 今の Jekyll(kramdown)は `$$…$$` を、文中にも独立した行にも使える。乗り換え先はどれも解釈が違う(remark-math は、1 行に書いた `$$ … $$` を文中の数式として扱う。Sätteri は文中でも常に独立の数式にする)。**数式の書き方を 1 度だけ揃え直す**必要がある(独立の数式は `$$` を単独の行に置く、など)。これは「新しい仕組みで崩れる書き方の直し」にあたる。数式のある記事と文書(宇宙創生論など数本)は、1 本ずつ表示を確かめる。
  - 数式のプラグイン無しで、今と同じくブラウザの MathJax に任せる手は使えない。Markdown が TeX の中の `_`・`\\`・`*` を先に解釈して壊す。
  - 描画は、ビルド時に済ませる方式(rehype-mathjax。閲覧側のスクリプトが要らない)と、KaTeX(スタイルシートが要る)から選ぶ。
- **そのまま置くファイル**: PDF、TeX、画像、favicon は、`public/` に置けばパスを変えずに公開物へ写る。Markdown の中で相対パスで指した画像は、最適化されて URL が変わる。絶対パス(`/img/…`、`/doc/…`)で指していることを確かめ、相対のものは直す。
- **コメント欄**: giscus はスクリプトを置くだけ(`is:inline` を付ける)。今の設定(リポジトリ、カテゴリ、ページとの対応の取り方)を引き継ぐ。URL が同じなら、対応も保たれる見込み。ページ遷移の演出(`ClientRouter`)は、スクリプトが再実行されなくなるので使わない。
- **フィード**: `/feed.xml` の場所だけ保つ。`@astrojs/rss` で作ると、形式は Atom から RSS 2.0 に変わる(変わってよい、と開発者が確かめ済み)。
- **サイトマップ**: 今は公開していない(`/sitemap.xml` は 404)。足さない。
- **`[!NOTE]`**: 今は文字のまま表示されている。囲みとして表示するかは要件で決める(従来の処理系なら、対応するプラグインがある)。
- **ビルドと公開**: 1 本目のワークフローのうち、ビルドの段を Node.js(Astro)に差し替える。公開の段と、PR ではビルドだけという振り分けは引き継ぐ。
  - 公式の `withastro/action`(v6)は `path: ./site` に対応するが、`npm ci` ではなく `npm install` を使い、Node.js の既定が 24 である。版を厳密に固定したいので、Node.js の用意 → `npm ci` → ビルド → 成果物の保存(`site/dist`)を自前で並べる案を優先する。
  - `site` の設定に `https://ekicyou.github.io` を入れる(フィードと SNS 向けの URL に要る)。`base` は要らない。
- **版の固定**: `package-lock.json` を管理下に入れ、ワークフローの Node.js の版も固定して、長く放置しても同じ結果でビルドできるようにする。Dependabot に npm(置き場は `/site`)を足す。
- **最初の一歩は試作**: 捨てる前提の試作を、リポジトリの外(セッションの scratchpad)で作る。記事 15 本と `doc/` を読ませ、URL が今と同じに出るか、数式とコードがどう映るかを実物で確かめてから、要件と設計を固める。あわせて、Windows と Node.js 26 の組み合わせで、手元の表示(`npm run dev`)が動くことも確かめる(Astro 7 は Rust 製の部品を含む。未確認)。

## Scope

- **In**:
  - `site/` の中身を、Jekyll の原稿から Astro のプロジェクトへ入れ替える
  - 記事・文書・画像・単独のページの引き継ぎ(URL を変えない)
  - レイアウト、部品、スタイルを一から作る
  - コメント欄、数式、コードの色付け、SNS 向けの情報、フィードの作り直し
  - ワークフローのビルドの段の差し替え、Dependabot への npm の追加
  - Jekyll の部品・テーマ指定・`Gemfile` の削除
  - 切り替えの前後で、公開されるパスの一覧を突き合わせる
  - 手元で見る手順の用意と、`workflow.md` の「検証」節への追記
  - steering(`tech.md`・`structure.md`・`product.md` の該当箇所)と `CLAUDE.md` の更新(記事の書き方、ビルドの手順)
- **Out**:
  - 記事の内容の書き換え(新しい仕組みで崩れる書き方の直しは除く)
  - 記事や文書の URL の変更
  - `site/` という置き場と、Actions 方式の公開そのもの(`site-subfolder-actions-build` が済ませている)
  - 独自ドメイン、公開先の移転
  - アイコン絵の原画探し(原画 PNG は見つかっていない。別件)

## Boundary Candidates

- デザインの方針決め(雰囲気、配色、文字、ページごとの構成)
- 中身の引き継ぎ(記事・文書の読み込み、URL の組み立て、front matter の検査、そのまま置くファイル)
- 枠と部品の実装(レイアウト、ヘッダー、フッター、一覧、記事ページ)
- 記事の機能(コメント欄、数式、コードの色付け、SNS 向けの情報、フィード)
- ビルドと公開の差し替え(ワークフロー、版の固定、Dependabot)
- 確かめ方(手元での表示、公開されるパスの突き合わせ)

## Out of Boundary

- 公開方式(GitHub Pages + Actions)と `site/` という置き場。変えるのはビルドの段だけ。
- 別リポジトリのページ(`/ghost_dev/`、`/nyan-pu-puu/`、`/zzz-pasta/`、`/old/`)の見た目。
- 記事を書くときの運用(`master` へ直接 push してよい、など)。今のまま。

## Upstream / Downstream

- **Upstream**: `site-subfolder-actions-build`(`site/` の配置、Actions 方式の公開、PR 上のビルド)。`.kiro/steering/product.md` の「守ること」。
- **Downstream**: 今のところ無し。以後の記事は新しい仕組みとデザインの上で書かれる。

## Existing Spec Touchpoints

- **Extends**: `site-subfolder-actions-build` が作るワークフローと `.github/dependabot.yml`(ビルドの段と npm の追加)。同じく書き換えた steering と `CLAUDE.md`。
- **Adjacent**: なし。

## Constraints

- **変えない URL**: `/`、記事(`/YYYY/MM/DD/<名前>.html`)、`/about.html`、`/404.html`、文書(`/doc/…` の HTML・PDF・TeX・画像)、画像(`/img/…`)、`/feed.xml`。
- **消えてよい見込みの URL**(要件で確かめる): Jekyll の部品に由来する `/assets/main.css`・`/assets/minima-social-icons.svg`、リモートテーマが足していた `/assets/css/style.css`・`/assets/js/scale.fix.js`。
- 既存の記事と文書が、新しいデザインで読めること。数式、コード、画像、表、脚注、縦書きの試み、`[!NOTE]` を含む。
- 個人の趣味サイト。凝った機能より壊れにくさを取る。外部のサービスや有料の仕組みを増やさない。長く放置しても、版を固定した状態でビルドできること。
- 同じ `ekicyou.github.io` の下で別リポジトリのページも公開されている。このリポジトリに同じ名前のトップ階層を作らない。
- アイコン絵の原画 PNG は手元に無い。使える絵は `img/alto0044_400x400.png` など今あるものに限られる。
- 見た目に効く変更なので、マージの前に開発者が画面を確かめる。
- 改行は LF。`core.autocrlf=true` のため作業ツリーは CRLF になる。差分が全行にならないことを確かめる。
