# Research & Design Decisions

ギャップ分析(`/kiro-validate-gap`)の記録。要件(`requirements.md`)と、今のリポジトリ・公開中のサイトとの差を調べ、あわせて捨てる前提の試作を行った(Requirement 9 の 6〜8)。調べた日は 2026-10-12。

この文書は事実と選択肢を並べる。どれを選ぶかは、要件ディスカッションと設計で決める。

## Summary

- **Feature**: `site-astro-redesign`
- **Discovery Scope**: Complex Integration(生成ツールの乗り換え + デザインの作り直し)
- **Key Findings**:
  - **試作は通った。行き止まりは無い。** Astro 7.3.8 は Windows 11 + Node.js 26.8.1 でインストール・ビルド・手元の表示のどれも動いた。記事 12 本・文書 2 本・単独のページ 3 つ・フィード・そのまま置くファイル 10 個の計 28 パスが、今と同じパスに出た。そのまま置くファイルは中身も同じ(バイト一致)。
  - **消えるパスは 4 つではなく 25 個ある。** リモートテーマは、要件に挙げた 2 つのほかに、`/assets/img/logo.png` と Noto Sans のフォント 20 個も公開している(公開中のサイトで 200 を確かめた)。要件の「消えてよいパス」は 4 つだけなので、このままだと Requirement 9 の 2(意図しない差があればマージしない)に掛かる。
  - **数式は、今のサイトでほとんど表示されていない。** 文書 2 本の数式 148 か所のうち、今 MathJax が描いているのは 22 か所だけ。文中の数式 124 か所は `$…$` で書かれていて、今は TeX の文字のまま見えている。`brief.md` の「`$$…$$` を文中にも使っている」という前提は当たらない。新しい仕組みでは 148 か所すべてが数式として読まれる。直しが要るのは、1 行に書いた `$$…$$` の 24 行(中央に置く数式にならない)と、未定義のマクロ 1 か所。
  - **Markdown の方言の差で、数式のほかにも崩れる書き方がある。** 1 以外の番号で始まる番号付きリスト 8 か所(53 項目)、日本語の約物に接した太字 5 か所、見出し行の無い表 1 か所。どれも「新しい仕組みで崩れる書き方の直し」(Requirement 2 の 2)にあたる。プラグインで吸収できるものと、原稿を直すしかないものがある。
  - **Markdown の処理系は、従来の側(`unified()`)で試作した。** 数式・囲み・日本語の太字のプラグインがそろっている。Astro 7 の既定(Sätteri)は数式を読む機能を内蔵するが、描画と囲みは自作になる。方言の差(リスト・太字・表)は、どちらの処理系でも同じに出る。

## 1. 今の姿

### 1.1 リポジトリの中身(サイトにかかわるもの)

| 種類 | 場所 | 数 | 乗り換えでの扱い |
|---|---|---|---|
| 記事 | `site/_posts/*.md` | 13(うち非公開 1) | 引き継ぐ |
| 記事(サブフォルダ) | `site/_posts/note/*.md` | 2(どちらも非公開) | 引き継ぐ |
| 文書の Markdown | `site/doc/cosmogenesis/*.md` | 2 | 引き継ぐ |
| そのまま置くファイル | `site/doc/cosmogenesis/`(PDF・TeX・JPEG)、`site/img/`(7 個) | 10 | パスを変えずに公開 |
| 単独のページ | `site/index.md`・`site/about.md`・`site/404.html` | 3 | `about.md` は本文を引き継ぐ。ほかの 2 つは作り直す |
| 下書き | `site/_drafts/dummy.md`(0 バイト) | 1 | 要件ディスカッションの項目 8 |
| Jekyll の部品 | `site/_layouts/`(4)、`site/_includes/`(12)、`site/_sass/`(4)、`site/assets/`(2)、`site/_config.yml`、`site/Gemfile` | 24 | 消す(Requirement 8) |
| ビルドと公開 | `.github/workflows/pages.yml`、`.github/dependabot.yml` | 2 | ビルドの段を差し替え、npm を足す |
| 開発用の設定 | `.gitignore`(中身は Jekyll 用: `.bundle`・`.sass-cache`・`Gemfile.lock`・`_site`・`*.gem`) | 1 | Node.js 用に書き換える(`node_modules`・`dist`・`.astro`) |
| プロジェクト文書 | `tech.md`・`structure.md`・`product.md`・`workflow.md`・`roadmap.md`・`CLAUDE.md`・`README.md` | 7 | Jekyll を前提にした記述を直す(Requirement 10) |

公開中の記事は 12 本、非公開は 3 本(`2016-08-17-GODZILLA.md`、`note/2016-07-08-dcomp.md`、`note/2019-08-31-IMM_API.md`)。

### 1.2 原稿の書き方(検索で確かめたこと)

- **front matter**: 記事と文書の 17 本と `about.md` で使われている項目は、`layout`・`title`・`description`・`image`・`tags`・`comments`・`use_math`・`published`・`date` の 9 つ。`date` は `about.md` だけ(`date: 2024-01-01`)。`use_math: true` は文書 2 本だけ。
- **Jekyll 専用の書き方**: `{% %}`・`{{ }}`・`{: }` は、記事・文書・`about.md` に 1 つも無い。
- **数式**: 文書 2 本だけにある。1 行に書いた `$$…$$` が 24 行(v1 に 6、v7 に 18)、文中の `$…$` が 124 か所(v1 に 17、v7 に 107)。`$$` を単独の行に置く書き方は 0。コードのブロックの中の `$` は 0。
- **コード**: 言語名つきは `csharp`(` ``` csharp ` と空白あり)・`markdown`(4 連のバッククォート)・`pasta` の 3 種。ほかは言語名なし。
- **囲み**: `[!NOTE]` が 5 か所、`[!IMPORTANT]` が 1 か所(記事 2 本)。
- **表**: 記事 4 本と文書 2 本にある(v7 は 19 個)。
- **画像**: Markdown の画像は 5 か所。すべて `/` で始まる絶対パス。相対パスは無い。うち 3 か所は別リポジトリのページ(`/nyan-pu-puu/…`、`/ghost_dev/…`)を指す。
- **サイト内リンク**: すべて絶対パスか絶対 URL。ページ内リンク(`#…`)は無い。
- **文中の HTML**: `<img … align="left" style="…">` が 1、`<iframe>` が 1、`<br>` が 2。
- **脚注**: `[^…]` は 0 件(§3.1)。
- **縦書き**: `writing-mode` などの指定は 0 件(§3.1)。

### 1.3 公開中のサイトが出しているパス

リポジトリの原稿から割り出し、公開中のサイトで応答を確かめた(読むだけ)。全部で **53 個**。1 本目の spec が記録した数(54 個から `README.md` を除いた 53 個)と合う。

| まとまり | 数 | 由来 |
|---|---|---|
| `/index.html`・`/about.html`・`/404.html`・`/feed.xml` | 4 | 単独のページとフィード |
| 記事 `/YYYY/MM/DD/<名前>.html` | 12 | `site/_posts/` |
| `/doc/cosmogenesis/…`(HTML 2、PDF・TeX・JPEG 各 1) | 5 | `site/doc/` |
| `/img/…` | 7 | `site/img/` |
| `/assets/main.css`・`/assets/minima-social-icons.svg` | 2 | 手元の minima の写し |
| `/assets/css/style.css`・`/assets/js/scale.fix.js` | 2 | リモートテーマ |
| **`/assets/img/logo.png`** | 1 | リモートテーマ(要件に挙がっていない) |
| **`/assets/fonts/Noto-Sans-{700,700italic,italic,regular}/…{eot,svg,ttf,woff,woff2}`** | 20 | リモートテーマ(要件に挙がっていない) |

ほかに確かめたこと:

- `/sitemap.xml`・`/robots.txt`・`/favicon.ico` は 404。
- 拡張子なしの `/about`・`/2026/03/26/emo2` も 200 で応答する(GitHub Pages が `.html` を補う。ファイル単位の出力なら、乗り換えの後も同じ)。
- 非公開の 3 本(`/2016/08/17/GODZILLA.html`、`/note/2016/07/08/dcomp.html`、`/note/2019/08/31/IMM_API.html`)は 404。

## 2. 試作の記録

### 2.1 作ったもの

- **置き場**: `C:\Users\maz-o\AppData\Local\Temp\claude\C--home-maz-git-my-site--claude-worktrees-kiro-site-astro-redesign-2ed957\4c1490fc-b27a-4de8-bcb8-71aeaa28f4f7\scratchpad\astro-proto`(リポジトリの外。リポジトリには何も足していない)
- **中身**: `site/` から写した記事 15 本(`content/_posts/`)、文書の Markdown 2 本(`content/doc/`)、`about.md`(`content/pages/`)、そのまま置くファイル 10 個(`public/img/`・`public/doc/`)。写しは元とバイト一致のまま(試しの書き換えは元に戻した)。
- **作り**: 飾りの無い最小の枠(`src/layouts/Base.astro`)、記事・文書・About・404・トップ・フィードのページ、コメント欄の部品(`src/layouts/Giscus.astro`)、コンテンツ機能の定義(`src/content.config.ts`)。
- **比べる道具**: 公開中のページを scratchpad の `live/` に保存し、要素の数・本文の文字・見出しの id を比べるスクリプトを置いた(`compare-live.mjs`・`textdiff.mjs`・`compare-ids.mjs`・`scan-src.mjs` など)。

**もう一度動かすコマンド**(PowerShell):

```powershell
Set-Location "C:\Users\maz-o\AppData\Local\Temp\claude\C--home-maz-git-my-site--claude-worktrees-kiro-site-astro-redesign-2ed957\4c1490fc-b27a-4de8-bcb8-71aeaa28f4f7\scratchpad\astro-proto"
npm run dev      # http://localhost:4321/ で見る。止めるのは Ctrl+C
npm run build    # dist/ に出す
```

崩れる書き方を直した姿で見るときは、先に環境変数を置く(`astro.config.mjs` の先頭に説明がある)。

```powershell
$env:PROTO_MD = 'unified-katex-cjk'; $env:PROTO_DOCS_BASE = './content-normalized/doc'; npm run dev
```

試作のために動かした手元の表示とブラウザは、すべて止めてある。

### 2.2 使った版

| 部品 | 版 | 備考 |
|---|---|---|
| Node.js / npm | 26.8.1 / 12.1.0 | Windows 11 Pro 10.0.26300 |
| `astro` | 7.3.8 | `engines` は Node.js 22.12 以上。26 で警告は出なかった |
| `@astrojs/markdown-remark` | 7.3.2 | 従来の処理系(`unified()`)。別に入れる |
| `@astrojs/markdown-satteri` / `satteri` | 0.4.3 / 0.10.5 | 既定の処理系。`astro` に付いてくる |
| `@astrojs/rss` | 4.0.19 | フィード |
| `remark-math` / `rehype-katex` / `katex` | 6.0.0 / 7.0.1 / 0.16.47 | 数式(KaTeX) |
| `rehype-mathjax` / `mathjax-full` | 7.1.0 / 3.2.1 | 数式(MathJax をビルド時に動かす) |
| `remark-github-blockquote-alert` | 2.1.0 | `[!NOTE]` などの囲み |
| `remark-cjk-friendly` | 2.3.2 | 日本語の約物に接した太字(直しの案の確かめ用) |
| 付いてきたもの | `vite` 8.3.4、`rolldown` 1.2.13、`shiki` 4.5.0、`zod` 4.6.6、`sharp` 0.35.5、`remark-gfm` 4.0.1 | |

- 直に入れたパッケージは 8 個。`package-lock.json` に載った項目は 414(OS ごとの部品を含む)、`node_modules` は 199 MB。
- Rust 製の部品(`@astrojs/compiler-rs`、`rolldown`、`satteri`、`lightningcss`)と `sharp` は、OS ごとの実行ファイルを別パッケージで入れる。Windows 用が入り、動いた。`package-lock.json` には Linux 用(`…-linux-x64-gnu`)も載っている。
- `npm audit` は 6 件を報告した(高 1、中 1、低 4)。どれも数式の部品の系統(`katex`、`mathjax-full` が使う `@xmldom/xmldom`)で、ビルド時に自分の原稿を処理するだけの部品である。`katex` の 1 件は、`rehype-katex` が求める版の範囲では直らない。

### 2.3 確かめた結果(a〜i)

| | 確かめたこと | 結果 | 要点 |
|---|---|---|---|
| a | URL が今と同じに出るか | **確認できた**(注意 3 点) | 28 パスが一致。消えるのは `/assets/…` の 25 個、増えるのは `/_astro/…` |
| b | Markdown の処理系と front matter の検査 | **確認できた** | `unified()` で試作。18 本すべてが検査を通った |
| c | 数式 | **確認できた**(直しが要る) | 148 か所すべて読める。24 行が中央に置く数式にならない。未定義のマクロ 1 か所 |
| d | コードの色付け | **確認できた** | `pasta` は警告つきで色付けなしになる。ビルドは止まらない |
| e | `[!NOTE]`・`[!IMPORTANT]` の囲み | **確認できた** | 6 か所すべてが囲みになった |
| f | 表・画像・文中の HTML | **確認できた**(崩れ 1 か所) | 見出し行の無い表 1 か所が表にならない。ほかは同じ |
| g | 手元の表示とビルドの時間 | **確認できた** | 起動 3.3 秒、書き換えは反映される。ビルドは 1.3〜2.0 秒 |
| h | フィードと SNS 向けの情報 | **確認できた**(最小限) | `/feed.xml` は RSS 2.0 で 12 本。URL は絶対 URL |
| i | コメント欄の埋め込み | **確認できた** | 今と同じ設定のスクリプトが出る。投稿はしていない |

#### a. URL

- 出力形式は `build.format: 'file'`。記事は、ファイル名(`YYYY-MM-DD-<名前>.md`)から日付と名前を取り出して URL を作った。front matter に日付は足していない。
- **一致したパス**: §1.3 の 53 個のうち、`/assets/…` を除く 28 個すべて。
- **そのまま置くファイル**: 10 個とも、原稿とバイト一致(`public/` に置くだけ)。
- **消えるパス**: `/assets/…` の 25 個(§1.3)。要件が挙げた 4 つのほかに 21 個ある。
- **増えるパス**: `/_astro/Base.<ハッシュ>.css`(スタイル)が 1 つ。数式を KaTeX で描くなら、KaTeX のフォントが 59 個(計 1.2 MB。`woff2` だけなら 19 個・292 KB)。`/_astro/` の下のファイル名は、中身が変わるたびにハッシュが変わる。
- **名前の綴り**: `glob` ローダーに `generateId` を渡し、ファイル名(拡張子を除く)をそのまま識別子にした。試しのファイルで確かめた結果:

  | ファイル名の `<名前>` | `generateId` あり | 既定(指定なし) |
  |---|---|---|
  | `Dot.Name` | `Dot.Name.html` | `dotname.html` |
  | `UPPER_lower-Mix` | `UPPER_lower-Mix.html` | `upper_lower-mix.html` |
  | `GODZILLA`(既存・非公開) | `GODZILLA.html` | `godzilla.html` |
  | `IMM_API`(既存・非公開) | `IMM_API.html` | `imm_api.html` |
  | `日本語の名前` | `日本語の名前.html` | `日本語の名前.html` |
  | `space name` | `space name.html` | `space-name.html` |
  | `a+b(c)` | `a+b(c).html` | `abc.html` |

  公開中の 12 本は小文字と `_`・`-` だけなので、既定でも同じに出る。差が出るのは非公開の 2 本と、これから足す記事である。`generateId` を入れておけば、大文字・ドット・記号がそのまま保たれる(Requirement 1 の 2)。
- **`published: false`**: 3 本とも出力されない。一覧にもフィードにも出ない(`getCollection` の絞り込み)。
- **注意 1(名前が `index` の記事)**: `YYYY-MM-DD-index.md` を置くと、ビルドが失敗する(`NoMatchingStaticPathFound`。`'file'` 形式が `…/index` をフォルダの入口として扱うため)。既存の記事に `index` は無い。
- **注意 2(`Astro.url`)**: ビルドでは `/about.html`・`/index.html`、手元の表示では求められた URL のまま(`/about` なら `/about`、`/` なら `/`)になる。試作では、正規 URL を記事のデータから組み立てた。この方法なら、どちらでも同じ値になる。
- **注意 3(Markdown を `src/pages/` に置けない)**: 既存の front matter の `layout: post` は、Astro の Markdown ページでは「読み込むレイアウトのファイル」として扱われる。`about.md` を `src/pages/` に置くと、ビルドが失敗した(`failed to resolve import "post"`)。front matter を書き換えずに引き継ぐには、コンテンツ機能(コレクション)で読む。そのときは `layout` はただのデータになる。

#### b. Markdown の処理系と front matter

- **使った処理系**: 従来の remark / rehype(`@astrojs/markdown-remark` の `unified()`)。理由は、数式(`remark-math` + `rehype-katex` / `rehype-mathjax`)、囲み(`remark-github-blockquote-alert`)、日本語の太字(`remark-cjk-friendly`)のプラグインがそろっているから。
- **既定の処理系(Sätteri)も試した**:
  - 何も足さないと、数式は `$…$` の文字のまま出て、`[!NOTE]` も引用のままになる。
  - `features: { math: true }` を渡すと、数式を読み取って `<code class="language-math math-inline">` の形にする(148 か所)。ただし描画はしない。KaTeX などを呼ぶ部品(Sätteri の hast プラグイン)を自作する必要がある。囲みも同じく自作になる。
  - 1 行に書いた `$$…$$` は、Sätteri でも文中の数式(`math-inline`)として扱われた。`brief.md` の「Sätteri は文中でも常に独立の数式にする」とは違う結果である。この点は remark-math と同じ。
  - リスト・太字・表の崩れ(§2.4)は、Sätteri でも同じに出る。CommonMark の決まりに由来する。
- **設定の書き方**: Astro 7 では、プラグインを `markdown.processor` に渡した処理系へ渡す。`markdown.remarkPlugins`・`markdown.rehypePlugins`・`markdown.gfm` は「廃止予定」になっている(`astro` の型定義の注記)。
- **front matter の検査**: 9 項目を任意(`title` だけ必須)として定義した。記事 15 本(非公開を含む)、文書 2 本、`about.md` の 18 本すべてが通った。`date: 2024-01-01` は日付として読める。
- **壊れた front matter**(Requirement 2 の 4): 試しのファイルで確かめた。`title` が無いと、ビルドは終了コード 1 で止まり、ファイルの場所を示す(`posts → 2099-02-01-notitle data does not match collection schema. title: Required` と、ファイルのパス)。YAML として読めないときも、ファイルと行を示して止まる。

#### c. 数式

**今のサイト**(公開中のページをブラウザで開いて確かめた):

- 読み込んでいる `//cdn.mathjax.org/…/MathJax.js` は、2017 年に止まった配布元である。今は転送用の短いスクリプトだけを返し、それが cdnjs の MathJax 2.7.1 を読み込む。
- MathJax 2.7.1 のこの設定(`TeX-AMS-MML_HTMLorMML`)は、`\(…\)` と `\[…\]` だけを数式として扱う。`$…$` は扱わない。
- kramdown は、段落の先頭にある `$$…$$` を `\[…\]` に、文の行に続く `$$…$$` を `\(…\)` に変える。`$…$` は何もしない。`|` を含む行は、表として解釈することがある。
- その結果:

  | 文書 | 数式の数 | 今、数式として描かれている | 今、崩れている |
  |---|---|---|---|
  | v1(`…_v1_ja`) | 23 | 4(どれも文中の大きさ) | `$…$` の 17 か所が TeX の文字のまま。2 行(`$$\|\bar{0}\rangle…$$`)が表に化けている |
  | v7(`…_v7_ja`) | 125 | 18(中央に置く数式 10、文中の大きさ 8) | `$…$` の 107 か所が TeX の文字のまま(うち 2 か所は表に化けている) |

**新しい仕組み**(`remark-math` + `rehype-katex`、原稿は直さない):

- 148 か所すべてが数式として読まれ、描かれる。`$…$` の 124 か所は、今より良くなる。表に化けていた箇所も直る。
- **崩れ 1(24 行)**: 1 行に書いた `$$…$$` が、文中の数式として描かれる。中央に置かれず、分数などが小さい組み方になる。v1 に 6 行(100・103・142・189・190・305 行)、v7 に 18 行。
- **崩れ 2(1 か所)**: v7 の 651 行の `\lambdabar` は、KaTeX にも MathJax にも無い命令で、赤い文字の `\lambdabar` として出る。今のサイトの MathJax でも定義されていない。

**直し方の案**(試作で確かめた):

| 崩れ | 案 1: 原稿を揃え直す | 案 2: 原稿を直さず、仕組みで吸収する |
|---|---|---|
| 1 行の `$$…$$`(24 行) | `$$` を単独の行に置く(1 行を 3 行に。24 行が 72 行になる)。**確かめ済み**: 24 か所すべてが中央に置く数式になった。文の行の直後でも効く | 「段落に数式が 1 つだけ」などの条件で独立の数式へ格上げする remark プラグインを自作する(**未試行**) |
| `\lambdabar`(1 か所) | 原稿の式を、定義のある書き方に直す | KaTeX の `macros` で `\lambdabar` を定義する。**確かめ済み**(赤い文字は消えた。字形の出来は見ていない) |

案 1 は、Sätteri に移ることになっても同じ結果になる書き方である(`$$` を単独の行に置く形は、どちらの処理系でも独立の数式になる)。

**KaTeX と MathJax(ビルド時)の違い**(v7 の文書で比べた):

| | KaTeX(`rehype-katex`) | MathJax(`rehype-mathjax`、SVG) |
|---|---|---|
| 閲覧側のスクリプト | 要らない | 要らない |
| 別に要るファイル | スタイルシートとフォント(`/_astro/` に 59 個、または `woff2` だけに絞る) | 無い(ページに埋め込まれる) |
| ページの大きさ(v7) | 約 311〜333 KB | 約 825 KB |
| 参考: 今のページ(v7) | 79 KB | |
| 読めなかった数式 | `\lambdabar` の 1 か所 | `\lambdabar` の 1 か所 |
| 数式の文字 | 文字として選べる | 図形(SVG)になる |

KaTeX のスタイルシートとフォントを外部の配布元(CDN)から読む手もあるが、外部のサービスを増やすことになる(Requirement 7 の 5)。試作では、手元の `katex` パッケージから束ねた。

#### d. コードの色付け

- Astro に付いてくる Shiki が、ビルド時に色を付ける。色はタグの `style` に直に書かれ、スタイルシートは要らない。既定の配色は `github-dark`(暗い地)。
- ` ``` csharp `(空白あり)は C# として色が付いた。4 連のバッククォートの `markdown` も色が付いた。
- `pasta` は、ビルドの記録に `[Shiki] The language "pasta" doesn't exist, falling back to "plaintext".` と出て、色付けなしのコードになる。ビルドは止まらない(Requirement 4 の 7 のとおり)。警告は毎回のビルドに出る。

#### e. 囲み

- `remark-github-blockquote-alert` で、6 か所すべてが `<div class="markdown-alert markdown-alert-note">`(または `…-important`)になった。見出し(「NOTE」とアイコン)が付き、`[!NOTE]` の文字は出ない。ブラウザで箱として見えることも確かめた。
- 見た目(枠、色、見出しの言葉)はスタイルで決める。プラグインは構造だけを出す。見出しの言葉(既定は英語の「NOTE」「IMPORTANT」)を変えられるかは見ていない。

#### f. 表・画像・文中の HTML、ほかの方言の差

公開中のページと試作の出力を、要素の数と本文の文字で比べた(15 ページ)。

- **本文がまったく同じ**(数式と囲みのプラグインを外して比べたとき): 12 ページ。
- **見出しの id**: 270 個のうち 269 個が同じ。違う 1 個は、見出しに数式を含むもの(v7 の「6.7 … と `$\alpha$`」)。
- **画像**: 絶対パスのまま出る(加工されず、URL も変わらない)。`/_astro/` に画像は出ない。
- **文中の HTML**: `<img align="left" style="…">`、`<iframe>`、`<br>` はそのまま出る。
- **リンク**: サイト内のリンク先(`/img/…`、`/doc/…`、`/2020/08/10/emo.html`、`https://ekicyou.github.io/2014/10/04/pasta.html`)は、すべて試作の出力にある。

**崩れる書き方**(kramdown と CommonMark / GFM の決まりの違い):

| 崩れ | 場所 | 数 | 新しい仕組みでの見え方 | 直し方の案 |
|---|---|---|---|---|
| 文の行の直後に、1 以外の番号で始まる番号付きリストを置いている | v7 の 983・988・1000・1009・1017・1029・1037・1046 行 | 8 か所(53 項目) | リストにならず、前の行とつながった 1 つの段落になる | リストの前に空行を 1 つ足す(8 行)。**確かめ済み**(`<ol start="8">` などになった) |
| 閉じの `**` の直前が約物(`）`・`」`)で、直後が文字 | v1 の 42 行、v7 の 308・639・643 行、`2025-12-06-mascot_requirements.md` の 38 行 | 5 か所 | 太字にならず、`**` が文字のまま出る | (a) `remark-cjk-friendly` を入れる(**確かめ済み**、5 か所とも太字になった)。(b) 原稿を直す(5 行) |
| 見出し行と区切り行の無い表(1 行だけ) | `2026-02-06-pasta.md` の 13 行 | 1 か所 | 表にならず、`\| リポジトリ \| リリース \|` と文字で出る(リンクは効く) | 原稿を直す(見出し行を足す、または表をやめてリンクを並べる) |
| 表のセルの `---` | v1 の 227 行ほか | 2 か所 | 今は「—」(長い横線)、新しい仕組みでは `---` のまま | 原稿を `—` に直すか、そのままにする |

**今より良くなる点**: 文中の数式 124 か所。数式の `|` が表に化けていた 3 か所。URL だけを書いた箇所がリンクになる(2 か所)。

#### g. 手元の表示とビルド

- `npm run dev` は、Windows 11 + Node.js 26.8.1 で起動した(`astro v7.3.8 ready in 3312 ms`)。Rust 製の部品も動いた。
- **同じパスで見える**: `/`、`/about.html`、`/2026/03/26/emo2.html`、`/doc/…_v1_ja.html`、`/feed.xml`、PDF・画像が 200。拡張子なしの `/about` も 200。非公開の記事と、無いパスは 404(404 ページの中身つき)。
- **書き換えの反映**: 記事の Markdown に 1 行足すと、1.5 秒後の応答に出た(記録に `Reloaded data from 2016-06-18-sun.md`)。レイアウトの書き換えも同じ。ページには自動で読み直すためのスクリプトが入っている。ブラウザが自動で読み直すところそのものは見ていない。
- **Claude からの確認**: ブラウザのペインで試作のページを開き、数式・囲み・コードの画面を見られた。
- **ビルドの時間**: キャッシュを消した状態で 2.0 秒、2 回目は 1.3 秒(Astro の表示。`npm run build` の全体では 4〜5 秒)。入れた直後の最初の 1 回は 8.1 秒。`npm install` は 11 秒。
- 記録の先頭に、利用状況の送信(テレメトリ)の案内が出る。`astro telemetry disable` か、環境変数 `ASTRO_TELEMETRY_DISABLED=1` で止められる。

#### h. フィードと SNS 向けの情報

- **フィード**: `@astrojs/rss` で `/feed.xml` を出した。形式は RSS 2.0。公開中の 12 本を新しい順に載せ、題名・URL(絶対 URL)・日付・`description`・タグを示す。
  - 日付は GMT で出る(日本時間の 0 時は、前日の 15:00 GMT と書かれる)。
  - 項目の識別子(`guid`)は記事の URL。今の Atom の識別子(`.html` なしの URL)とは違う。フィードを読んでいる人がいれば、切り替えのときに全部が新着として見える見込み(**未確認**)。
- **SNS 向けの情報**: 各ページに、題名、説明、正規の URL(`https://ekicyou.github.io/…`)、`og:image`(`image` があるときだけ、絶対 URL)、フィードの場所(`<link rel="alternate">`)を出せた。`/ghost_dev/…` のような別リポジトリの画像も、絶対 URL に直る。

#### i. コメント欄

- リポジトリにある今の設定(`site/_includes/giscus_comments.html`):

  | 項目 | 値 |
  |---|---|
  | `data-repo` | `ekicyou/ekicyou.github.io` |
  | `data-repo-id` | `MDEwOlJlcG9zaXRvcnk2OTMzMzcz` |
  | `data-category` / `data-category-id` | `Announcements` / `DIC_kwDOAGnLfc4CzVb5` |
  | `data-mapping` / `data-strict` | `pathname` / `0` |
  | `data-reactions-enabled` / `data-emit-metadata` | `1` / `0` |
  | `data-input-position` / `data-theme` | `bottom` / `preferred_color_scheme` |
  | `data-lang` / `data-loading` | `ja` / `lazy` |

- 同じ値のスクリプトを `is:inline` で置き、ビルドが通った。記事と文書の 14 ページに出て、About・404・トップには出ない。
- 今のサイトは、本番のビルドのときだけコメント欄を出している(`jekyll.environment == "production"`)。試作は条件を付けなかったので、手元の表示でもコメント欄が読み込まれた(「0 件のコメント」と出た)。投稿はしていない。
- リポジトリのディスカッションは 1 件だけ(題名「test」、カテゴリ Announcements、コメント 0)。ページに結び付いたディスカッションは、まだ 1 つも無い(GitHub の API で確かめた)。つまり、引き継ぐべき既存のコメントは無い。

### 2.4 試作から要件・設計への含み

- **要件の直し候補**: 「消えてよいパス」の数(§7 の項目 14)。数式の要件(Requirement 4 の 4)は「今と同じ」ではなく「今より良くなる」になる。
- **原稿の直しの範囲**: 数式 24 行、リストの前の空行 8 行、見出し行の無い表 1 か所は、原稿を直すのが素直。太字 5 か所と `\lambdabar` は、原稿を直す手と、仕組みで吸収する手がある。直す行は、合わせて 40 行ほどに収まる。
- **設計で使える形**: `build.format: 'file'`、`glob` ローダー + `generateId`、ファイル名からの日付の取り出し、記事のデータから組み立てる正規 URL、`public/` に置くそのまま置くファイル。どれも試作で動いた。

## 3. 引き継がれた点の確かめ

### 3.1 脚注と縦書き

- **脚注**: `site/` の Markdown を `[^…]` で検索して 0 件。`brief.md` の「あるのは…脚注…」は、今の原稿には当たらない。
- **縦書き**: `site/` 全体を `writing-mode`・`vertical`・`tategaki` で検索して、縦書きにする指定は 0 件(`_sass/` にあるのは `vertical-align` など、別の用途だけ)。「縦書き」という言葉は、古い記事 4 本の本文に話題として出てくるだけ(2014-10-04、2016-02-16、2016-06-18、2025-12-06)。どれも横書きで表示されている。
- 参考: 脚注(`[^1]`)は、どちらの処理系でも GFM の機能として既定で使える。縦書きは CSS だけで決まり、生成ツールに左右されない。

### 3.2 SNS 向けの情報の既定(公開中のページの実物)

`jekyll-seo-tag` が出している。

- **説明**(`description`・`og:description`):
  1. front matter の `description` があれば、それ。
  2. 無ければ、記事の最初の段落(HTML を落とした文字)。例: `/2016/02/16/code.html` は「ブログ内にコード断片を埋め込むテスト」。
  3. それも空なら、サイトの説明文。例: `/2026/02/06/pasta.html`(最初の段落が `<img>` だけ)、`/about.html`、トップ、404。
- **画像**(`og:image`・`twitter:image`): front matter の `image` があるときだけ出る。既定の画像は無い。
- **カードの種類**: `image` があれば `summary_large_image`、無ければ `summary`。
- **そのほか**: `og:type` は、記事と About が `article`、文書・トップ・404 が `website`。`og:locale` は `ja_JP`。構造化データ(JSON-LD)も出ている。
- 公開中の 12 本のうち、`description` が無いのは 5 本(2014-10-04、2016-02-16、2016-06-18、2018-08-26、2026-02-06)。`image` があるのは 6 本。

### 3.3 フィード(公開中の `/feed.xml` の実物)

- 形式は Atom(`jekyll-feed`)。大きさは 153 KB。
- **載っているのは新しい順に 10 本**。12 本のうち、古い 2 本(2016-02-16、2014-10-04)は載っていない。
- 1 本ごとに、**本文の全文**(`<content type="html">`)と、要約(`<summary>`。最初の段落)の両方がある。
- タグは `<category>` に、`image` は `<media:thumbnail>` に出ている。

### 3.4 About の日付

- `site/about.md` は `layout: post` と `date: 2024-01-01` を持つ。公開中の `/about.html` は、題名の下に「2024-01-01」と表示している。`article:published_time` と構造化データにも、この日付が出ている。
- 本文の「更新」の節の一番新しい日付も 2024-01-01 である。

### 3.5 favicon

- `/img/favicon.ico`(9,662 バイト)と `/img/favicon.png`(184 バイト)は、200 で公開されている。
- 公開中のどのページの `<head>` にも、`<link rel="icon">` は無い。`site/` の中で、この 2 つを指している箇所も無い。
- `/favicon.ico` は 404。ブラウザが既定で探しに行く場所には、何も無い。

### 3.6 ほかに分かったこと

- **ヘッダーのメニューに文書 2 本が並んでいる。** 今の枠は、`title` を持つページを自動でメニューに並べる。そのため、About のほかに、文書 2 本の題名が並んでいる。
- **v7 の文書(`…_v7_ja.html`)は、どの記事からもリンクされていない。** 入口は、このメニューだけである。新しいデザインでメニューから外すと、URL は生きたまま、サイトの中から辿れなくなる。v1 の文書は、記事 `2026-05-06-cosmogenesis.md` からリンクされている。
- **文書のページの日付は空**(`<time datetime="">`)。文書に日付は無い。
- **`/img/alto0044_400x400.png`**(`brief.md` が挙げるアイコン絵)は、今はどのページからも使われていない。
- **`lang`**: 今は `<html lang="ja-JP">`。
- **改行**: `.gitattributes` の `eol=lf` が効いていて、ワークツリーの原稿は LF だった(`core.autocrlf=true` でも CRLF になっていない)。

## 4. 要件ごとの対応表

印: **有** = 今あるものをそのまま使える / **無** = 新しく作る / **制約** = 守る決まりがある / **不明** = 調べが要る

| 要件 | 要るもの | 今あるもの | 差 |
|---|---|---|---|
| 1. URL を保つ | ファイル単位の出力、名前を保つ仕組み、そのまま置く場所 | 原稿の配置と名前 | **無**: Astro の設定とルート。**制約**: 消えるパスが 25 個(要件は 4 つ)。`/_astro/` が増える。別リポジトリと同じ名前のトップ階層を作らない |
| 2. 記事と文書を引き継ぐ | コレクションの定義、front matter の検査、囲み、非公開の扱い | 記事 15、文書 2、`about.md` | **無**: 定義とプラグイン。**制約**: 崩れる書き方の直し(§2.3 の c と f)。`layout: post` のため `src/pages/` には置けない |
| 3. 新しいデザイン | レイアウト、部品、スタイル | 使えるのは `site/img/` の絵だけ | **無**: すべて。デザインの決めごと 5 点は未定 |
| 4. 記事の機能 | コメント欄、数式、色付け、SNS 向けの情報、フィード | giscus の設定値 | **無**: 部品 5 つ。**不明**: 数式の描画方式、フィードの中身、説明と画像の既定 |
| 5. 手元の表示 | `npm run dev` | Node.js 26、npm 12 | **無**: `package.json`。試作で動作を確かめた |
| 6. ビルドと公開 | ビルドの段 | `pages.yml`(2 ジョブ、振り分け、権限) | **有**: 公開の段と振り分け。**無**: Node.js の用意 → `npm ci` → ビルド。成果物の場所が `site/dist` に変わる。`build` の `pages: read` は、Jekyll のための権限 |
| 7. 版の固定 | ロックファイル、Node.js の版の指定、版上げの提案 | `dependabot.yml`(アクションだけ) | **無**: `package-lock.json`、npm の項目。**不明**: Windows で作ったロックファイルが、Linux の `npm ci` で通るか |
| 8. Jekyll を残さない | 消すファイルの一覧 | §1.1 の 24 個と `index.md`・`404.html` | **無**(消すだけ)。`.gitignore` も書き換える |
| 9. 切り替えを確かめる | パスの突き合わせ、画面の確認、代表パス | 1 本目の手順(成果物と公開中のサイトの比べ方) | **有**: 手順の型。**無**: 突き合わせの道具(試作のスクリプトが下敷きになる)。試作(6〜8)は済んだ |
| 10. 文書を合わせる | steering・`CLAUDE.md`・`README.md` の書き換え | Jekyll を前提にした記述 | **無**: 書き換え。`tech.md` はほぼ全体、`structure.md` は配置の節、`workflow.md` は「検証」節 |

## 5. 進め方の案

決めることは 4 つの軸に分かれる。軸どうしは、ほぼ独立に選べる。

### 軸 1: Markdown の処理系

| 案 | 中身 | 良い点 | 悪い点 |
|---|---|---|---|
| A. 従来の処理系 + 既存のプラグイン | `unified()` に `remark-math`・`rehype-katex`(または `rehype-mathjax`)・`remark-github-blockquote-alert`(と `remark-cjk-friendly`)を渡す | 試作で動いた。自作が少ない | Astro の既定ではない側。直に入れるパッケージが増える。版上げのたびに見直す |
| B. 既定の処理系 + 自作の部品 | Sätteri の数式を読む機能を使い、描画と囲みを自作する | Astro の既定に乗る。入れるパッケージが減る | 自作が増える(数式の描画、囲み、日本語の太字)。Sätteri は 0.x 版で、決まりが動く見込み |
| C. A で始め、原稿を「どちらでも同じに読める形」に揃える | A を採りつつ、原稿を直す(`$$` を単独の行に、リストの前に空行) | 後で B に移るときの手間が小さい。原稿が処理系に縛られない | 原稿に手を入れる行が増える(それでも 40 行ほど) |

### 軸 2: 原稿の置き場

| 案 | 中身 | 良い点 | 悪い点 |
|---|---|---|---|
| P1. 今の場所のまま読む | `site/_posts/`・`site/doc/`・`site/about.md` を動かさず、`glob` ローダーの `base` で指す。そのまま置くファイルだけ `site/public/` へ移す | 記事を動かさない。書く場所が今と同じ(Adjacent expectations) | Astro の流儀(`src/content/`)と違う。`site/doc/<題材>/` の Markdown と PDF・画像が、別のフォルダに分かれる |
| P2. Astro の流儀に合わせる | `site/src/content/posts/` などへ移す | Astro の文書どおりで、迷いが無い | 記事の置き場が変わる(`git mv`。中身は変わらない)。`structure.md` の書き方の決まりが変わる |
| P3. 文書の Markdown を `public/` の下に置く | Markdown と PDF を同じフォルダに保つ | 題材ごとに 1 フォルダのまま | Markdown そのものも公開される(`/doc/…_ja.md`。今は 404。**確かめ済み**)。Requirement 1 の 6 に掛かる |

- 試作は P1 に近い形で、`src/` の外の置き場を `base` で指して動いた。
- どの案でも、`site/img/` と、`site/doc/` の Markdown 以外のファイルは `site/public/` の下へ移る(P3 を除く)。

### 軸 3: 数式の描画

§2.3 の c の表のとおり。KaTeX は軽いが、フォントのファイルが増える。MathJax(SVG)はファイルが増えないが、ページが重い。どちらも閲覧側のスクリプトは要らない。

### 軸 4: ビルドの段

| 案 | 中身 | 良い点 | 悪い点 |
|---|---|---|---|
| W1. 自前で並べる | `actions/setup-node`(版を指定)→ `npm ci`(`site/` で)→ `npm run build` → `upload-pages-artifact`(`site/dist`) | 版を厳密に固定できる。公式アクションだけで組める(今の方針と同じ) | 手順が 3 つ増える |
| W2. `withastro/action` | Astro 公式のアクションに任せる | 手順が短い | `npm install` を使う。Node.js の既定が 24。サードパーティのアクションが 1 つ増える(`brief.md` の調べ) |

## 6. 手間と危うさ

- **手間: L(1〜2 週間)**。乗り換えそのもの(設定、ルート、プラグイン、ワークフロー)は、試作でほぼ形が見えており、S〜M に収まる。重いのは、一から作るデザイン(決めごと 5 点の話し合い、画面を見ながらの直し)と、プロジェクト文書の書き換えである。
- **危うさ: Medium**。
  - 技術の行き止まりは、試作で消えた。
  - 残るのは、(1) 公開中の URL に効く変更であること、(2) Astro の主版の上がり方が速いこと(版を固定すれば止まらないが、上げるときに手間が掛かる)、(3) 従来の処理系が既定でない側であること、(4) デザインの決めごとが未定であること。

## 7. 「要件ディスカッションで決めること」への材料

`requirements.md` の末尾の 13 項目に、今回分かった事実と選択肢を添える。決めるのは開発者である。

### 1. デザインの方向

- 今のスタイルの骨組み: 本文の幅は最大 800px、切り替えの幅は 600px と 800px。文字は OS のフォント。リモートテーマが公開している Noto Sans(20 個)は、どのページも使っていない。
- 試作の表示は、ブラウザのペインで Claude も見られた。「設計の中で画面の案をいくつか見て決める」進め方は、道具の面では成り立つ。
- コードのブロックの色は、ビルド時に決まる(既定は暗い地の `github-dark`)。デザインの方向と合わせて選ぶことになる。

### 2. 暗い配色

- コメント欄(giscus)は、今も OS の設定に合わせて明暗が変わる(`preferred_color_scheme`)。
- 数式(KaTeX も MathJax の SVG も)は、周りの文字の色を引き継ぐ。
- コードの色付けは、色がタグに直に書かれる。明暗の両方に対応するなら、Shiki の「配色を 2 つ持つ」設定を使うことになる(Astro の設定にある。**未試行**)。
- 透過の画像(`/img/pasta.svg` など)が暗い地でどう見えるかは、見ていない。

### 3. トップページの見せ方

- 今は、12 本すべてを、日付・題名・書き出し(最初の段落。画像を含む)で並べている。
- 書き出しを自動で取り出す機能は、Astro には無い。`description` は 12 本のうち 7 本にある。書き出しを出すなら、自前の処理が要る。
- 今のメニューには、About のほかに文書 2 本が並んでいる。v7 の文書は、ここが唯一の入口である(§3.6)。

### 4. 記事ページに足すもの

- **タグ**: 公開中の記事に付いているタグは 12 種(「伺か」が 6 本、ほかは 1 本ずつ)。文書 2 本にもタグがある。
- **タグごとの一覧ページ**: 新しい URL が増える。タグは日本語なので、URL も日本語になる(試作では、日本語の名前のページも出力できた。URL の上ではパーセント表記になる)。置き場は、別リポジトリのページ(`/ghost_dev/`・`/nyan-pu-puu/`・`/zzz-pasta/`・`/old/`)と重ならない名前にする。
- **目次**: 見出しの一覧は、Astro が記事ごとに渡してくれる。見出しの多いページがある(`mascot_requirements` が 81、v7 が 94)。
- **前後の記事**: 日付順に並べた一覧から決まる。新しい仕組みは要らない。

### 5. スマートフォンでの見え方

- 幅の広い要素は、表(v7 に 19 個)、中央に置く数式(24 か所)、コード、`<iframe width="100%" height="362">`(1 か所)。
- 試作の仮のスタイルでは、表・コード・数式に「その部分だけ横に送る」指定を置いた。狭い画面での実際の見え方は、確かめていない。

### 6. 文書のページのコメント欄

- 今は、文書 2 本にコメント欄が出ている。
- ページに結び付いたディスカッションは、記事も文書も 0 件。出し続けても、やめても、失われるコメントは無い。

### 7. サブフォルダの記事の URL

- Jekyll の決まりでは `/note/YYYY/MM/DD/<名前>.html`。今は 2 本とも 404 で、公開済みの URL は無い。
- 試作は、Jekyll と同じ決まり(サブフォルダ名を先頭に置く)で作り、非公開を外すと `/note/2019/08/31/IMM_API.html` に出ることを確かめた。
- 選択肢: (a) Jekyll と同じにする。(b) サブフォルダ名を URL に入れない(`/YYYY/MM/DD/<名前>.html`)。(c) サブフォルダに置いた記事は公開できない決まりにする。(d) 今は決めず、公開するときに決める(それまでは非公開の記事だけを置く)。
- (a) は、`/note/` というトップ階層を新しく作ることになる。

### 8. 下書きの置き場

- `site/_drafts/dummy.md` は 0 バイト。
- コレクションに含めない置き場は、ビルドに読まれない。`_drafts/` を残しても、読む対象から外しておけば、「公開されない書きかけの置き場」として働く。
- コレクションに含めるなら、`title` の無いファイルはビルドを止める(Requirement 2 の 4 と同じ動き)。0 バイトの `dummy.md` も止める原因になる。
- 選択肢: (a) `_drafts/` をやめ、`published: false` だけにする。(b) `_drafts/` を「読まない置き場」として残す。

### 9. フッターの中身

- 今: サイト名、作者名(`ekicyou`)、メールアドレス(`mailto:` のリンク)、GitHub(`ekicyou`)と Mastodon(`ukadon.shillest.net` の `@ekicyou`)へのリンク、フィードへのリンク、サイトの説明文。
- メールアドレスは、`site/_config.yml` に書いた値がそのまま出ている。

### 10. SNS 向けの説明と画像の既定

- 今の実物は §3.2。説明は「`description` → 最初の段落 → サイトの説明文」の順、画像は「`image` があるときだけ」。
- 試作は「`description` → サイトの説明文」だけにした。「最初の段落」を挟むなら、自前の処理が要る(項目 3 の書き出しと同じ仕組みで足りる)。
- 既定の画像を置くなら、使える絵は `site/img/` にあるもの(`alto0044_400x400.png` など)に限られる。

### 11. フィードの中身

- 今の実物は §3.3(Atom、10 本、全文と要約の両方)。
- 試作は RSS 2.0、12 本すべて、`description` だけ(全文なし)にした。
- `@astrojs/rss` で全文を載せるには、記事の HTML を自前で取り出して渡す。数式や色付けの入った HTML は大きくなる。
- 形式が変わると、項目の識別子も変わる(§2.3 の h)。

### 12. favicon

- 今の実物は §3.5(ファイルはあるが、どのページも指していない)。
- 選択肢: (a) 使わない(今と同じ)。(b) `<link rel="icon">` で今ある `/img/favicon.ico` を指す(新しいパスは増えない)。(c) `/favicon.ico` にも置く(トップ階層に新しいパスが 1 つ増える)。

### 13. 脚注と縦書き

- どちらも、今の原稿には無い(§3.1)。
- 脚注は、どちらの処理系でも既定で使える(脚注の節の見出しの言葉は、既定では英語)。縦書きは CSS だけの話で、生成ツールに左右されない。

### 新しく出た点(13 項目の外)

14. **消えてよいパスの数**: 要件は 4 つ。実際に消えるのは 25 個(`/assets/img/logo.png` と Noto Sans のフォント 20 個を含む。どれもリモートテーマ由来で、どのページからも使われていない)。Requirement 1 の「消えてよいパス」に 21 個を足すか、`/assets/` の下すべてとするか、残すかを決める。
15. **数式の描画方式**: KaTeX か MathJax(ビルド時)か。増えるパスの数(フォント 59 個、または 0)と、ページの大きさ(v7 で約 311 KB か 825 KB)に効く。
16. **崩れる書き方の直し方**: 原稿を直す範囲(数式 24 行、空行 8 行、表 1 か所、太字 5 行、`\lambdabar` 1 か所、`---` 2 か所)と、仕組みで吸収する範囲(太字、`\lambdabar`)。
17. **Markdown の処理系**: 従来の側か、既定の側か(§5 の軸 1)。
18. **原稿の置き場**: 記事を今の場所に残すか、Astro の流儀の場所へ移すか。文書の Markdown と PDF・画像が別のフォルダに分かれてよいか(§5 の軸 2)。
19. **About の日付**: 今は「2024-01-01」と出ている。新しいデザインで出し続けるか。`about.md` の `date` は、front matter の検査で受け入れる項目に足す必要がある(Requirement 2 の 3 の一覧に `date` は無い)。
20. **文書への入口**: 今はヘッダーのメニューに文書 2 本が並ぶ。v7 はここが唯一の入口。新しいデザインで、文書への入口をどこに置くか。
21. **手元の表示でのコメント欄**: 今は本番のビルドのときだけ出る。手元の表示でも出すか、出さないか。
22. **フィードの形式の変わり目**: RSS 2.0 になると、識別子が変わり、読んでいる人には全部が新着に見える見込み。受け入れるか、Atom を自前で出すか。
23. **名前が `index` の記事**: `YYYY-MM-DD-index.md` はビルドが失敗する。書き方の決まりに「使えない名前」として書くか。
24. **利用状況の送信**: Astro は既定で、匿名の利用状況を送る。止めるか。
25. **`lang` の値**: 今は `ja-JP`。Requirement 3 の 5 は「今と同じ」としている。`ja-JP` のままか、`ja` でもよいか。

## 8. 設計へ持ち越す調べもの(Research Needed)

- **Linux での `npm ci`**: ロックファイルは Windows で作る。OS ごとの部品(`rolldown`、`@astrojs/compiler-rs`、`satteri`、`lightningcss`、`sharp`)の Linux 用は、ロックファイルに載っていた。ランナー(`ubuntu-latest`)で `npm ci` とビルドが通るかは、最初の PR で確かめる。
- **Node.js の版の固定**: Astro は 22.12 以上を求める。手元は 26.8.1。ワークフローで指定する版と、指定の置き場(`.nvmrc`、`package.json` の `engines` など)。
- **Dependabot の npm**: 置き場は `/site`。Astro の主版が速く上がるので、提案の数とまとめ方。数式の部品は `npm audit` の報告が出やすい(§2.2)。
- **KaTeX のスタイルシートを、数式のあるページだけに入れる方法**: 試作では、全ページに入った。フォントを `woff2` だけに絞れるかも見る。
- **Shiki の配色**: 暗い配色に対応すると決めたときの、配色 2 つの設定。
- **囲みの見出しの言葉**: `remark-github-blockquote-alert` で、言葉とアイコンを変えられるか。
- **突き合わせの基準**: マージの前に、`master` の最新の成果物(`github-pages`)を取り出して基準にする(1 本目の手順)。今回のパスの一覧(53 個)は原稿と公開中の応答から割り出したもので、成果物そのものとは突き合わせていない。
- **ビルドの段の権限**: `build` の `pages: read` は Jekyll のための権限。Astro では要らない見込み(**未確認**)。
- **`astro check`**: 型の検査をビルドに入れるか(入れるなら `@astrojs/check` と `typescript` が増える)。

## 9. 設計フェーズへの申し送り

- 試作で動いた組み合わせ(`build.format: 'file'`、`glob` + `generateId`、ファイル名から作る URL、`unified()` + 数式と囲みのプラグイン、`public/`、`@astrojs/rss`、giscus の `is:inline`)は、そのまま設計の出発点になる。
- 設計に入る前に、要件ディスカッションで決めておくと手戻りが少ない点: 項目 14(消えてよいパス)、15(数式の描画方式)、16(原稿の直しの範囲)、18(原稿の置き場)、20(文書への入口)。
- デザインの決めごと 5 点(項目 1〜5)は、要件のとおり、決まるまで決まったものとして扱わない。
- 試作のフォルダは捨てる前提である。設計と実装は、試作のファイルを写さず、要件と設計の文書から起こす。

## References

- `astro` 7.3.8 の型定義(`node_modules/astro/dist/types/public/config.d.ts`)— `build.format`、`markdown.processor`、廃止予定の `markdown.remarkPlugins` など
- `astro` 7.3.8 の `glob` ローダーの型定義(`dist/content/loaders/glob.d.ts`)— `generateId`
- `satteri` 0.10.5 の型定義(`dist/compile.d.ts`)— `Features.math`、`singleDollarTextMath`
- [Astro: Markdown の処理系の選び方](https://docs.astro.build/en/guides/markdown-content/#choosing-a-markdown-processor) — 型定義が指している案内
- [Astro: エラー `NoMatchingStaticPathFound`](https://docs.astro.build/en/reference/errors/no-matching-static-path-found/) — 名前が `index` の記事で出た
- [MathJax: cdn.mathjax.org の終了](https://www.mathjax.org/cdn-shutting-down/) — 今のサイトが読み込んでいる配布元
- 公開中のサイト `https://ekicyou.github.io/`(2026-10-12 に読むだけで確かめた。パスの応答、`<head>`、`/feed.xml`、文書のページの数式)
- `.kiro/specs/completed/site-subfolder-actions-build/`(`research.md`・`tasks.md`)— 公開物 54 ファイルの記録と、突き合わせの手順
