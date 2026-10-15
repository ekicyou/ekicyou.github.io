# 移行の記録: site-astro-redesign

Jekyll から Astro へ乗り換える作業の、1 回きりの記録である。原稿のどこを直したか、何をどう確かめたかを、ここ 1 か所に残す(`design.md` の「Migration Strategy」の「移行の記録に残すもの」)。

- 欄は、タスクが進むたびに書き足す。まだ行っていない確かめの欄は、見出しと、どのタスクが書くかだけを置いてある。
- 行の番号は、断りが無ければ、その直しを入れた時点のファイルのものである。

## 直した箇所の一覧

`requirements.md` の「原稿を直す範囲」の 4 種類を直す(Requirement 2 の 12、14)。どれも、記号と空行の置き方だけの直しで、言葉と式は変えていない。4 種類のほかに見つけた崩れは、開発者の判断を仰いでから直した(Requirement 2 の 15。タスク 6.2 の 5 と 6)。

### タスク 2.2 で直したもの(2026-10-12)

画面を見なくても決まる 3 種類を直した。直したファイルは、文書の 2 本だけである。

- v1: `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md`
- v7: `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md`

「直す前の行」は、移動のコミット(`1181eeb`)の時点の行である。中身は移す前と同じなので、`research.md` の §2.3 が挙げる行の番号と一致する。「直した後の行」は、このタスクの直しをすべて入れた後の行である。行が増えるので、後ろの箇所ほど番号がずれる。

#### 1. 1 行に書いた `$$…$$` を、`$$` を単独の行に置く形にした(24 か所)

1 行を 3 行(`$$`、式、`$$`)にした。式の文字は 1 字も変えていない。「直した後の行」は、開きの `$$` から閉じの `$$` までである。

| 文書 | 直す前の行 | 直した後の行 | 直前の行 |
|---|---|---|---|
| v1 | 100 | 100〜102 | 文の行 |
| v1 | 103 | 105〜107 | 文の行 |
| v1 | 142 | 146〜148 | 文の行 |
| v1 | 189 | 195〜197 | 空行 |
| v1 | 190 | 198〜200 | 数式(189 行の続き) |
| v1 | 305 | 315〜317 | 文の行 |
| v7 | 155 | 155〜157 | 文の行 |
| v7 | 158 | 160〜162 | 文の行 |
| v7 | 264 | 268〜270 | 文の行 |
| v7 | 265 | 271〜273 | 数式(264 行の続き) |
| v7 | 283 | 291〜293 | 文の行 |
| v7 | 286 | 296〜298 | 文の行 |
| v7 | 304 | 316〜318 | 空行 |
| v7 | 411 | 425〜427 | 空行 |
| v7 | 465 | 481〜483 | 空行 |
| v7 | 469 | 487〜489 | 空行 |
| v7 | 539 | 559〜561 | 文の行 |
| v7 | 556 | 578〜580 | 文の行 |
| v7 | 572 | 596〜598 | 空行 |
| v7 | 601 | 627〜629 | 空行 |
| v7 | 603 | 631〜633 | 空行 |
| v7 | 607 | 637〜639 | 空行 |
| v7 | 619 | 651〜653 | 空行 |
| v7 | 651 | 685〜687 | 空行 |

- 24 か所とも、行の全体が `$$…$$` だけの行だった。リストや引用の中にあるもの、同じ行に文があるものは無い。
- 空行は足していない。文の行の直後にある数式は、文の行の直後のままである。2 行続きの数式(v1 の 189・190 行、v7 の 264・265 行)は、間に空行を入れず、3 行の塊を 2 つ続けた。

#### 2. 番号付きリストの前に、空行を 1 つ足した(8 か所)

文の行(太字の小見出し)の直後に、1 以外の番号で始まるリストが置かれていた箇所である。どれも v7。「直す前の行」は、リストの最初の項目の行である。

| 文書 | 直す前の行 | 足した空行(直した後の行) | リストの最初の項目(直した後の行) | 番号 | 項目の数 |
|---|---|---|---|---|---|
| v7 | 983 | 1019 | 1020 | 8〜10 | 3 |
| v7 | 988 | 1025 | 1026 | 11〜20 | 10 |
| v7 | 1000 | 1038 | 1039 | 21〜27 | 7 |
| v7 | 1009 | 1048 | 1049 | 28〜33 | 6 |
| v7 | 1017 | 1057 | 1058 | 34〜43 | 10 |
| v7 | 1029 | 1070 | 1071 | 44〜49 | 6 |
| v7 | 1037 | 1079 | 1080 | 50〜56 | 7 |
| v7 | 1046 | 1089 | 1090 | 57〜60 | 4 |

項目は、合わせて 53 個である。

#### 3. 表のセルの `---` を「—」と書いた(2 か所)

v1 の「4.1 qubit 階段」の表の、「BH段階」の列である。「—」は U+2014(今の公開サイトが出している長い横線と同じ字)。

| 文書 | 直す前の行 | 直した後の行 | 直し |
|---|---|---|---|
| v1 | 227 | 237 | 行の末尾のセル `---` → `—` |
| v1 | 230 | 240 | 行の末尾のセル `---` → `—` |

#### 直しの確かめ(タスク 2.2)

- 差分は、文書 2 本だけに出る。消えた行は 26(数式 24、表 2)、足した行は 82(数式 72、空行 8、表 2)。
- 差分の塊は 32 個、直しは 34 か所である(2 行続きの数式が 2 組あり、その 2 個の塊が直しを 2 つずつ含む)。直しを 1 つずつ分けると、数式が 24、空行が 8、表が 2 で、ほかは無い。
- 直した後のファイルから 3 種類の直しを外すと、2 本とも、直す前のファイルとバイトが一致する。改行は LF のままである。
- `site/astro.config.mjs` の処理系で 2 本を描いて数えた結果:

  | | 直す前 | 直した後 |
  |---|---|---|
  | 数式の数(v1 / v7) | 23 / 125 | 23 / 125 |
  | 独立した行の数式(v1 / v7) | 0 / 0 | 6 / 18 |
  | 文中の数式(v1 / v7) | 23 / 125 | 17 / 107 |
  | 読めなかった数式 | 0 | 0 |
  | 1 以外の番号で始まるリスト(v7) | 0 | 8(8・11・21・28・34・44・50・57 から) |
  | 段落の中に残った「番号. 」の行(v7) | 53 | 0 |
  | `---` のままのセル(v1) | 2 | 0 |

  これは数を数えただけである。画面で 1 本ずつ見る確かめは、下の「数式の確かめ」で行う。

### タスク 6.2 で直したもの(2026-10-15)

#### 4. 見出し行の無い表を、表にした(1 か所)

表にするか、リンクの並びにするかを、開発者が画面で 2 つの見え方を見比べて決めた(A の「表」を選んだ、2026-10-15)。

| ファイル | 直す前の行 | 直した後の行 | 直し |
|---|---|---|---|
| `site/src/content/posts/2026-02-06-pasta.md` | 13 | 13〜14 | 13 行の直後に、区切りの行 `\|---\|---\|` を 1 行足した |

- 13 行(リンク 2 つの行)は 1 字も変えていない。この直しで、ほかの行は変えていない(同じファイルの 3 行は、下の 6 の直し)。改行は LF のままである。
- 直す前は、表にならず、`<p>| <a …>リポジトリ</a> | <a …>リリース</a> |</p>` と、`|` が文字のまま出ていた。
- 直した後は、`<table>` が 1 つ出る。13 行は表の見出しの行になり、リンク 2 つは見出しのセル(`<th>`)に入る。本文の行は無い。`|` の文字は残っていない。

#### 5. 見出しの中の数式を、文字にした(1 か所)

「原稿を直す範囲」の 4 種類のほかに見つけた崩れである。開発者の判断(Requirement 2 の 15)で直した(2026-10-15)。

| ファイル | 行 | 直す前 | 直した後 |
|---|---|---|---|
| `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md` | 647 | `## 6.7 Holographic Throttling と $\alpha$（v7 新規）` | `## 6.7 Holographic Throttling と α（v7 新規）` |

- 崩れ: 本文の見出しの数式は正しく描かれていた。しかし、目次(広い幅の `toc-wide` と、狭い幅の `toc-narrow` の 2 つ)のリンクの文字が `6.7 Holographic Throttling と α\alphaα（v7 新規）` になり、見出しの id も `67-holographic-throttling-と-αalphaαv7-新規` になっていた。
- 理由: Astro の見出しの集め方(`@astrojs/markdown-remark` の見出しの id を付ける部品)は、見出しの中の文字をすべてつなげる。KaTeX の出力には、見える数式のほかに、MathML の文字と元の TeX(`annotation`)が入っているので、3 つが並んだ。見出しに数式を書いているのは、全原稿でここだけだった。
- 直し方: `$\alpha$` を、文字の `α`(U+03B1)にした。ほかの行は変えていない。改行は LF のままである。これで数式は 1 つ減った(v7 は 125 → 124。文中の数式が 107 → 106)。
- 直した後: 目次の 2 つのリンクの文字は `6.7 Holographic Throttling と α（v7 新規）`、見出しの id は `67-holographic-throttling-と-αv7-新規` で、目次の `href` は見出しの id と一致する。
- これからの原稿のために、`.kiro/steering/structure.md` の「記事の書き方」の「数式」に、「見出しに数式を書かない。ギリシャ文字は文字のまま書く」を足した。
- 公開中のサイトのこの見出しの id は `67-holographic-throttling-と-alphav7-新規` で(2026-10-15 に取って見た)、新しい id とは違う。この見出しを指すリンクは、原稿には無い。

#### 6. 記事の題名のバッククォートを外した(1 か所)

開発者の判断(Requirement 2 の 15)で直した(2026-10-15)。

| ファイル | 行 | 直す前 | 直した後 |
|---|---|---|---|
| `site/src/content/posts/2026-02-06-pasta.md` | 3 | ``title:  SHIORI `pasta.dll`とサンプルゴーストをでっちあげる`` | `title:  SHIORI pasta.dllとサンプルゴーストをでっちあげる` |

- 直したのは、バッククォート 2 字だけである。`title:` の後の空白 2 つは、そのままにした。
- 理由: 切り替えの前のサイトでは、`<title>` と `og:title` はバッククォートの無い形だった(Jekyll の SEO の部品が落としていた)。一方、記事の見出し・一覧は文字のままのバッククォートを出していた。新しい仕組みは、どこでも front matter の文字をそのまま出すので、`<title>` と `og:title` にもバッククォートが付いてしまう。原稿から外せば、記事の `<h1>`・`<title>`・`og:title`・トップページの一覧・フィードの題名が、どれも `SHIORI pasta.dllとサンプルゴーストをでっちあげる` にそろう。
- URL(`/2026/02/06/pasta.html`)と、コメント欄の対応(`pathname`)は、題名に依らないので変わらない。

### 原稿を直さないもの

- 約物に接した太字(5 か所)と `\lambdabar`(1 か所)は、原稿を直さず、仕組みの側で受け止める(Requirement 2 の 13)。どちらも、下の「数式の確かめ」で、意図どおりに出ることを見た。

### 原稿を直さずに受け入れた変化

- **`/2026/02/06/pasta.html` の説明**(SNS 向けの説明と `<meta name="description">`): 切り替えの前は、サイトの説明文(「このサイトは２０世紀後半に発生した、 えちょの嗜好についての諸々を…」)だった。切り替えの後は、本文の最初の段落の文字「構想10年というか、12年前の記事なんですが。。。」になる。
  - 理由: 原稿の先頭の `<img>` は生の HTML のかたまりで、新しい処理系では段落に包まれない。そのため、最初の段落は次の文になる(Requirement 4 の 14 の文面どおり)。
  - 開発者が、この新しい説明を受け入れた(2026-10-15)。原稿にも仕組みにも手を入れていない。`design.md` の「Testing Strategy」の単体テストの括弧書きと、`research.md` の §11.2 の「最初の段落」の行が、Jekyll の出力を前提にしていたので、`design.md` は書き直し、`research.md` には訂正を書き足した。
  - 説明の無いほかの記事の説明は、切り替えの前と同じである(タスク 3.3 で確かめた)。

## コメント欄の設定の突き合わせ

タスク 4.3 で確かめた(2026-10-12)。giscus の埋め込みが、切り替えの前と同じ値で出ることを、属性を 1 つずつ突き合わせた(Requirement 4 の 1、3)。

出どころのファイル(`site/_includes/giscus_comments.html`)は、タスク 7.1 で消える。消えた後は、この表が、値が同じだったことの記録になる。

突き合わせたのは、次の 3 つである。

- **前(部品)**: `site/_includes/giscus_comments.html`(コミット `acd4563` の時点)。
- **前(公開中)**: `https://ekicyou.github.io/2026/03/26/emo2.html` を、2026-10-12 に取ったもの。
- **後**: `npm run build` が出した `site/dist/2026/03/26/emo2.html`。値は `site/src/consts.ts` の `SITE.giscus`、埋め込みは `site/src/components/Comments.astro`。

| 属性 | 値 | 前(部品) | 前(公開中) | 後 |
|---|---|---|---|---|
| `src` | `https://giscus.app/client.js` | 同じ | 同じ | 同じ |
| `data-repo` | `ekicyou/ekicyou.github.io` | 同じ | 同じ | 同じ |
| `data-repo-id` | `MDEwOlJlcG9zaXRvcnk2OTMzMzcz` | 同じ | 同じ | 同じ |
| `data-category` | `Announcements` | 同じ | 同じ | 同じ |
| `data-category-id` | `DIC_kwDOAGnLfc4CzVb5` | 同じ | 同じ | 同じ |
| `data-mapping` | `pathname` | 同じ | 同じ | 同じ |
| `data-strict` | `0` | 同じ | 同じ | 同じ |
| `data-reactions-enabled` | `1` | 同じ | 同じ | 同じ |
| `data-emit-metadata` | `0` | 同じ | 同じ | 同じ |
| `data-input-position` | `bottom` | 同じ | 同じ | 同じ |
| `data-theme` | `preferred_color_scheme` | 同じ | 同じ | 同じ |
| `data-lang` | `ja` | 同じ | 同じ | 同じ |
| `data-loading` | `lazy` | 同じ | 同じ | 同じ |
| `crossorigin` | `anonymous` | 同じ | 同じ | 同じ |
| `async` | (値なし) | 同じ | 同じ | 同じ |

- 属性は 15 個(`src`、`data-*` が 12 個、`crossorigin`、`async`)。3 つとも同じ 15 個で、過不足も、値の違いも無い。
- **コメントとの対応**: 対応の取り方は `pathname`(ページのパスから拡張子を除いた文字)のままである。手元の表示で `/2026/03/26/emo2.html` を開くと、giscus の `<iframe>` は `term=2026/03/26/emo2` で作られた。記事 12 本のパスは、公開中のサイトと成果物とで同じである(12 本とも、公開中は 200 を返して埋め込みを 1 つ持ち、成果物も同じパスに埋め込みを 1 つ持つ)。リポジトリ、カテゴリ、対応の取り方、パスの 4 つが変わらないので、ページとディスカッションの対応は切れない。なお、ページに結び付いたディスカッションは、まだ 1 つも無い(`research.md` の §2.3 の i)。
- **埋め込みの周りの違い**(対応には響かない): 前は、スクリプトの前に空の `<div id="giscus_thread">` を置き、本番のビルドのときだけ出していた。後は、`<section class="comments" aria-label="コメント">` で包み、手元の表示でも出す(`design.md` の「設計で決めた点」)。コメント欄は、後でも、スクリプトの直後(`<section>` の中)に作られる。
- **出るページ**: 成果物の HTML 13 個のうち、埋め込みを持つのは記事の 12 個。`/404.html` には無い。公開中のサイトも、`/`、`/about.html`、`/404.html` には無い。文書の 2 ページと About・トップは、まだルートが無い(タスク 5.1、5.2)。文書も同じ部品(`Comments.astro`)を通る。その後、タスク 8.4 のビルド(HTML 17 本、2026-10-15)で数え直すと、埋め込みを持つのは記事 12 本と文書 2 本の 14 個で、About・トップ・404 には無い。
- **`comments: false`**(Requirement 4 の 2): 記事 1 本(`2016-02-16-code.md`)を仮に `comments: false` にしてビルドすると、そのページだけ埋め込みが消えた(12 個 → 11 個)。確かめた後、原稿はバイト一致で戻した。
- **配色**(Requirement 3 の 15): 手元の表示(`/2026/01/06/syuzo-to.html`)で、ブラウザの配色を明るい・暗いに切り替えて見た。コメント欄は、明るい配色では明るく、暗い配色では暗く出た。`<iframe>` の `theme` は、どちらも `preferred_color_scheme`。ログインも投稿もしていない。

## 数式の確かめ

タスク 6.2 で確かめた(2026-10-15)。確かめの基準は、切り替えの前の表示ではなく、原稿が意図した数式である(Requirement 4 の 5、Requirement 2 の 13)。

### 確かめ方

- **手元の表示**: `npm run dev` で文書 2 本を 1 本ずつブラウザで開き(v7 は幅 1280px、v1 は幅 780px。v7 の `\lambdabar` は 780px でも見た)、ページの上で数えた。独立した行の数式は、要素が `display: block`・`text-align: center` であることを見た。v7 では、式の左右の余白が等しいこと(18 個とも差 0px)も測った。文中の数式は、要素が `inline` で文の中に置かれていることを見た。画面でも、独立した行の数式・文中の数式・表のセルの中の数式を見た。手元の表示で見たのは、見出しの直し(上の 5)の前である(そのときの v7 の数式は 125)。
- **原稿との突き合わせ**: KaTeX は、描いた数式ごとに、元の TeX を `<annotation encoding="application/x-tex">` に残す。原稿を同じプラグイン(GFM、日本語の太字、数式の読み取り)で読んで数式の TeX を順に取り出し、ページの上の数式の TeX の並びと比べた。
  - 見出しの直しの前: 手元の表示のページと原稿とで、TeX の並びの SHA-256 を比べた(148 個)。ビルドの成果物(`dist/doc/cosmogenesis/*.html`)でも、1 つずつ文字と、独立した行か文中かの別を比べた。
  - 見出しの直しの後: 冷えた状態でビルドし直し、成果物で、もう一度 1 つずつ比べた(147 個)。
- **崩れの残り**: 初めは、成果物の本文だけを探していた(数式を除いた文字に、`$`、TeX の命令、`**` が残っていないか)。目次と `<head>` を見ていなかったので、見出しの数式が目次に漏れていたのを見落とした(レビューで見つかり、上の 5 で直した)。直した後は、ページの全体を探した(下の「ページの全体の検査」)。

### 結果

見出しの直し(上の 5)の後の数である。

| | v1(`entanglement_cosmogenesis_v1_ja`) | v7(`entanglement_cosmogenesis_v7_ja`) |
|---|---|---|
| 数式の数(原稿 / ページ) | 23 / 23 | 124 / 124 |
| 独立した行の数式(原稿 / ページ) | 6 / 6 | 18 / 18 |
| 中央に置かれた独立した行の数式 | 6 | 18 |
| 文中の数式(原稿 / ページ) | 17 / 17 | 106 / 106 |
| 読めなかった数式(`katex-error`) | 0 | 0 |
| 原稿と TeX が違う数式 | 0 | 0 |
| ページの全体で、数式の外に残った TeX・Markdown の文字 | 0 | 0 |
| 独立した行の数式が横にはみ出す数(v1 は幅 780px、v7 は幅 1280px) | 0 | 0 |

- 合わせて 147 個(独立した行 24、文中 123)。見出しの直しの前は 148 個(文中 124)だった。
- 独立した行の数式の原稿の行(開きの `$$` の行)は、v1 が 100・105・146・195・198・315、v7 が 155・160・268・271・291・296・316・425・481・487・559・578・596・627・631・637・651・685 である。タスク 2.2 の「直した後の行」と一致する(見出しの直しは行の数を変えない)。
- TeX の並びの SHA-256: v1 は `ac9170f7eebd500c…`(直しの前後で同じ)。v7 は、直しの前が `eaff527b97075660…`(手元の表示のページと原稿とで同じ)、直しの後が `917f6035bf0cfe62…`(原稿)で、成果物とも 1 つずつ一致した。
- 数式のフォントは、ページの上で KaTeX のフォント(v1 は 3 種、v7 は 4 種)が読み込まれていた。数式のスタイルシートは、文書のページが指している。
- 切り替えの前に表に化けていた v1 の 2 行(`|\bar{0}\rangle = …`、`|\bar{1}\rangle = …`。原稿の 195・198 行)は、中央に置く 2 つの数式として、`⊗` の上下に添字の付いた形で出た。

### `\lambdabar`

- 場所は v7 の 685〜687 行の独立した行の数式(移す前の 651 行)の、分母の `\lambdabar_C^{(e)}` である。式は `\frac{1}{\alpha} = \frac{a_0}{\lambdabar_C^{(e)}} = \frac{\text{Pauli 排除距離}}{\text{固有 qubit スケール}} \approx 137`。
- `astro.config.mjs` のマクロ(`{\lambda\mkern-9mu\raisebox{0.35ex}{-}}`)で描かれ、赤い文字(読めない数式)にはならない。
- 画面を拡大して見た字形: 斜体の λ の上の方に、短い横棒が左から重なった形で、λ に横棒を引いた記号(ƛ)として読める。右下に添字 C、右上に (e) が付き、分数の分母として組まれている。字形の出来を最後に決めるのは開発者が画面で見たときである(`design.md` の AstroConfig の Risks)。

### 約物に接した太字(5 か所)

原稿を直さずに、5 か所とも `<strong>` で出た。成果物の文書 2 本と `mascot_requirements` の本文に、`**` の文字は 1 つも残っていない。原稿を `remark-cjk-friendly` なしで読むと、この 5 か所だけが太字にならないことも見た(仕組みの側で受け止めていることの裏付け)。

| ファイル | 行 | 太字になった文字(直後の文字) |
|---|---|---|
| `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md` | 42 | 「完成された理論ではなく、プログラム的予想（programmatic conjecture）」(である) |
| `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md` | 322 | 「「外部 1 logical qubit」」(を持つ) |
| `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md` | 673 | 「蜂の巣格子（hexagonal lattice）」(でタイル張り) |
| `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md` | 677 | 「HCP（六方最密充填）」(で配列) |
| `site/src/content/posts/2025-12-06-mascot_requirements.md` | 38 | 「「利用者に愛されるゴーストをデスクトップ上に再現する」」(こと) |

- v7 の行は、タスク 2.2 の直しの後の番号である(`research.md` の §2.3 の f の 308・639・643 行)。

### 成果物の検査の警告

- タスク 6.1 から渡された警告(サイト内のリンクと画像、読めない数式)は、0 件だった。タスク 6.2 の直し(上の 4〜6)の後も、`npm run verify` は誤り 0 件・警告 0 件である。

### ほかに見つけた崩れ

- **見出しの数式が目次に漏れていた**(v7 の 647 行): 「原稿を直す範囲」のほかに見つかった崩れである。初めの確かめは本文だけを見ていて見落とし、レビューで見つかった。開発者の判断で原稿を直した(上の 5)。
- 題名のバッククォート(`2026-02-06-pasta.md`)も、開発者の判断で原稿を直した(上の 6)。
- ほかには見つからなかった(下の「ページの全体の検査」)。

### ページの全体の検査

見出しと題名の直しの後、冷えた状態でビルドした成果物の HTML 17 本すべて(記事 12、文書 2、About、トップ、404)を、ページの全体で探した。

- **見た場所**: `<head>`(`<title>`、`<meta>` の `content`)と `<body>` の文字のすべて(目次の `toc-wide`・`toc-narrow`、ヘッダー、フッター、一覧、前後の記事を含む)。属性は `content`・`alt`・`title`・`aria-label`・`id`。
- **外した場所**: KaTeX の出力の中(元の TeX を持つため)、`<pre>`・`<code>` の中(コードの例として書いた文字)、`<script>`・`<style>`。
- **探したもの**: `$`、バッククォート、TeX の命令(`\` で始まる語)、`**`、`[!NOTE]` などの囲みの目印、Markdown のリンクの書き方 `](`、`|` で始まる文字、二重に書かれた文字参照(`&amp;` など)。見出しの id に TeX の命令の名前(`alpha`、`frac`、`lambda` など)が入っていないか。目次などのページ内リンク(`#…`)が、ページの中の id を指しているか。
- **結果**: 17 本とも 0 件だった。ページ内リンクは 372 個で、指す先の無いものは 0。この探し方は、直す前の目次の文字(`α\alphaα`)と、バッククォートつきの題名を見つけることを、文字の並びで確かめてある。
- **フィード**: `dist/feed.xml` の題名・説明・分類(43 個)にも、`$`・バッククォート・TeX の命令・`**`・`](` は無かった。
- **pasta の題名**: 記事の `<h1>`・`<title>`(「 | えちょのざんがい」つき)・`og:title`、トップページの一覧、フィードの `<title>` が、どれも `SHIORI pasta.dllとサンプルゴーストをでっちあげる` である。

## 検査の確かめ

タスク 6.3 で確かめた(2026-10-15)。検査と、記事を足す流れが働くことを、1 回きりの仕掛けで見た(`design.md` の「Testing Strategy」の「1 回きりの確かめ」)。

- 仕掛けは、どれも 1 つずつ置き、確かめたらすぐに消した。コマンドは、どれも `site/` で打った。
- 始める前と終えた後に、冷えた状態(`site/node_modules/.astro/data-store.json` を消した状態)で `npm run build` を行った。どちらも 17 ページで、`npm run verify` は誤り 0 件・警告 0 件(ファイル 49 個、HTML 17 本、非公開の記事 3 本)だった。
- 書き換えた管理下のファイルは `site/tests/kept-paths.txt` の 1 つだけで、戻した後の SHA-256 は書き換える前と同じ(`48f7d980…`)。足した原稿は、どれも消した。

### 1. 成果物の検査の仕掛け(Requirement 1 の 1、4、Requirement 3 の 25、Requirement 2 の 11)

4 つの仕掛けとも、`npm run verify` が終了コード 1 で止まった。

| 仕掛け | 置いたもの | 終了コード | 出た誤り |
|---|---|---|---|
| 保つパスを、無いパスに替える | `tests/kept-paths.txt` の `/about.html` を `/about-missing.html` に替えた | 1 | `/about-missing.html: 保つパスが無い(tests/kept-paths.txt)`(誤り 1 件) |
| 別リポジトリと同じ名前のフォルダ | `dist/old/x.txt` | 1 | `/old/x.txt: 別リポジトリのページと同じ名前のトップ階層(/old/)に出ている`(誤り 1 件) |
| メールのリンク | `dist/about.html` の末尾に `<a href="mailto:someone@example.invalid">x</a>` を書き足した | 1 | `/about.html: メールのリンク(href="mailto:…")がある`(誤り 1 件) |
| 非公開の記事のパス | `dist/2016/08/17/GODZILLA.html`(中身は `dist/2016/02/16/code.html` の写し) | 1 | `/2016/08/17/GODZILLA.html: 非公開の記事(src/content/posts/2016-08-17-GODZILLA.md)のページが出ている`(誤り 1 件) |

- 別リポジトリの名前は、ほかに `dist/old/index.html` と `dist/nyan-pu-puu/x.html` も置いた。どちらも終了コード 1 で、トップ階層の誤りが出た。中身が `<p>x</p>` だけの HTML なので、`lang="ja"`・正規の URL・フィードの場所・favicon が無い誤りも 4 件ずつ並んだ(誤り 5 件)。
- 非公開の記事のパスも、初めは中身が `<p>x</p>` だけの HTML で置いた。このときも終了コード 1 で、非公開の記事の誤りに、上の 4 件が並んだ(誤り 5 件)。表は、整ったページの写しで、非公開の記事の誤りだけが出ることを見たときのものである。
- 戻すたびに `npm run verify` を行い、誤り 0 件・警告 0 件に戻ることを見た。`dist/about.html` は写しから戻し、SHA-256 が書き足す前と同じ(`d659be74…`)であることを見た。

### 2. front matter と記事の名前の検査(Requirement 2 の 4)

仮の記事を 1 本ずつ置き、冷えた状態で `npm run build` を行った。3 つとも終了コード 1 で止まった。2 つはファイルのパスを、名前が `index` のものは識別子(ファイル名から `.md` を除いたもの)を示した。

| 置いた記事 | 中身 | 終了コード | 出た誤り |
|---|---|---|---|
| `src/content/posts/2099-01-01-notitle.md` | `title` が無い(`description` と `tags` だけ) | 1 | `[InvalidContentEntryDataError] posts → 2099-01-01-notitle data does not match collection schema.`、`title: Required`。`Location:` に、このファイルの絶対パス |
| `src/content/posts/2099-01-02-unknown.md` | 知らない項目 `publised: false`(`published` の打ち間違い) | 1 | `[InvalidContentEntryDataError] posts → 2099-01-02-unknown data does not match collection schema.`、`Unrecognized key: "publised"`。`Location:` に、このファイルの絶対パス |
| `src/content/posts/2099-01-03-index.md` | front matter は決まりどおり。名前が `index` | 1 | `` 記事のファイル名が決まりに合わない: "2099-01-03-index"。名前に `index` は使えない。 ``(静的なルートを作る段で止まる) |

- 前の 2 本は、原稿を読む段(`[content] Syncing content`)で止まる。名前が `index` の記事は、front matter の検査を通り、記事のルートを作る段で止まる。誤りの文は識別子を示し、`Location:` は Astro の内部のファイルを指す(識別子がそのままファイル名なので、原稿は 1 本に決まる)。

### 3. 記事を足す流れ(Requirement 2 の 9、11)

`src/content/posts/2099-12-31-trial.md` を、決まりどおりの front matter(`title`・`description`・`tags`)で仮に置いた。

- **公開のとき**: `npm run build` は終了コード 0、18 ページ。`dist/2099/12/31/trial.html` が出て、正規の URL は `https://ekicyou.github.io/2099/12/31/trial.html` だった。トップページの記事の一覧では、13 本の先頭に出た。フィードは 13 項目になり、先頭の項目が `https://ekicyou.github.io/2099/12/31/trial.html`(題名、説明、`Wed, 30 Dec 2099 15:00:00 GMT`、カテゴリ「確かめ」)だった。1 つ古い記事 `/2026/05/06/cosmogenesis.html` は、この記事を指すリンクを持った。`npm run verify` は誤り 0 件・警告 0 件(ファイル 50 個、HTML 18 本)。保つパスの一覧は替えずに通る(記事の形のパスなので、増えても誤りにならない)。
- **`published: false` のとき**: 同じ記事に `published: false` を足し、冷えた状態でビルドした。終了コード 0、17 ページ。`dist/2099/` は無く、`dist/` のどのファイルにも、この記事の名前と題名は出なかった(トップページの一覧にも、フィードにも無い。フィードは 12 項目)。`npm run verify` は誤り 0 件・警告 0 件で、非公開の記事を 4 本と数えた。
- 確かめた後、記事を消し、冷えた状態でビルドし直した(17 ページ、誤り 0 件・警告 0 件、非公開の記事 3 本)。

### 4. そのまま置くファイルのバイト一致(Requirement 1 の 3)

成果物の PDF・TeX・画像の 10 個を、移動のコミット(`1181eeb`)の 1 つ前のコミットの中身(`git cat-file -p 1181eeb^:site/<パス>`)と、SHA-256 で比べた。10 個とも一致した。`site/public/` の同じファイルも、10 個とも同じ値である。

| 成果物のパス | 大きさ(バイト) | SHA-256(先頭 16 字) | 結果 |
|---|---|---|---|
| `/doc/cosmogenesis/entanglement_cosmogenesis_v1.pdf` | 329413 | `03625478fedeecd6` | 一致 |
| `/doc/cosmogenesis/entanglement_cosmogenesis_v1.tex` | 43679 | `e3611174fcaabf90` | 一致 |
| `/doc/cosmogenesis/space_cat.jpg` | 28445 | `df7481b8aacc3bee` | 一致 |
| `/img/2020-08-10-emo.png` | 253117 | `5a73c9b9597080dc` | 一致 |
| `/img/2026-02-06-pasta.png` | 288393 | `e04e50d652271283` | 一致 |
| `/img/EmbeddedImage.png` | 4989 | `84b3666dd61257bb` | 一致 |
| `/img/alto0044_400x400.png` | 137883 | `ef9f67c5933d7879` | 一致 |
| `/img/favicon.ico` | 9662 | `47e7899437ab6a32` | 一致 |
| `/img/favicon.png` | 184 | `1861eca1d57dc3e8` | 一致 |
| `/img/pasta.svg` | 4124 | `63c85578878205d5` | 一致 |

`dist/doc/cosmogenesis/` と `dist/img/` にあるファイルは、この 10 個と、文書の HTML 2 本だけである。

### 5. フィードと `katex` の版(Requirement 4 の 10)

**フィード**: `dist/feed.xml`(RSS 2.0、5,889 バイト)を読んで確かめた。

- 項目は 12 個で、公開中の記事 12 本と同じ並び(新しい順。`/2026/05/06/cosmogenesis.html` から `/2014/10/04/pasta.html` まで)だった。非公開の 3 本は無い。
- 12 項目とも、題名、絶対 URL の `<link>`(`https://ekicyou.github.io/YYYY/MM/DD/<名前>.html`)、同じ URL の `<guid>`、日付の `<pubDate>`、説明の `<description>` を持つ。`<pubDate>` は、どれもファイル名の日付の日本時間 0 時(GMT の表記で前の日の 15:00)で、前の項目より新しいものは無い。
- 全文の要素(`content:encoded`)は無い。説明は最も長いもので 131 字である。`lastBuildDate` も無い。

**切り替えの前後の違い**(タスク 5.3 で見た事実。公開中のフィードは、2026-10-15 に取り直して同じであることを見た):

| | 切り替えの前(公開中) | 切り替えの後 |
|---|---|---|
| 形式 | Atom(Jekyll の部品が出す) | RSS 2.0 |
| 項目の数 | 10 | 12(公開中の記事すべて) |
| 大きさ | 約 153 KB(取り直すたびに数バイト違う) | 約 6 KB(5,889 バイト) |
| 全文 | あり(`<content>` が 10 個) | なし |
| 項目の識別子 | `.html` なし(例 `https://ekicyou.github.io/2026/05/06/cosmogenesis`) | `.html` つき(`<guid>` が記事の URL) |

- 増えるのは古い 2 本(`/2016/02/16/code.html`、`/2014/10/04/pasta.html`)である。
- 形式と識別子が変わることは、要件の「Adjacent expectations」が受け入れている(開発者の判断、2026-10-12)。

**`katex` の版**: `npm ls katex` は 0.16.47 の 1 つだけを返した。直に入れた `katex@0.16.47` に、`rehype-katex@7.0.1` と `remark-math@6.0.0`(`micromark-extension-math@3.1.0` を通る)の使う `katex` が `deduped` で重なる。

## `look/` との差分

タスク 8.1 で確かめた(2026-10-15)。`diff -ru .kiro/specs/completed/site-astro-redesign/look/ site/src/` で、スタイル・レイアウト・部品のすべてを突き合わせた。理由の無い差は無かったので、`site/src/` は直していない。画面の見比べの結果は、下の「画面の確かめ」にある。

### 差のあるファイル

| ファイル | 差 | 理由 |
|---|---|---|
| `site/src/components/HeadMeta.astro` | 書体の `<link>` 3 行(`preconnect` 2 行と、書体のスタイルシート)を、`{SITE.fontsHref && (<>…</>)}` で包んだ(1 か所。3 行を消し、7 行を足した) | 設計の HeadMeta の「書体のスタイルシート(`fontsHref` があるとき。`preconnect` を添える)」に合わせた(タスク 4.1)。`look/` は無条件に出していた。`SITE.fontsHref` の型は `string \| undefined` である。今は値があるので、出る HTML は `look/` と同じ 3 行になる |

差は、この 1 か所だけである。

### バイト一致のファイル(15 個)

- スタイル: `styles/tokens.css`、`styles/base.css`、`styles/prose.css`
- レイアウト: `layouts/BaseLayout.astro`、`layouts/ArticleLayout.astro`
- 部品: `components/` の `CodeCopy`、`Comments`、`DocList`、`Hero`、`PostList`、`PostNav`、`SiteFooter`、`SiteHeader`、`TagList`、`Toc`(どれも `.astro`)

### 片方にだけあるもの

| もの | ある側 | 理由 |
|---|---|---|
| `consts.ts`、`content.config.ts`、`lib/`(`post-path.ts`・`posts.ts`・`docs.ts`・`summary.ts`)、`pages/`(`index.astro`、`404.astro`、`about.astro`、`[year]/[month]/[day]/[name].astro`、`doc/[...id].astro`、`feed.xml.ts`) | `site/src/` だけ | `look/` に無い(`look/README.md` の「このフォルダに無いもの」)。`design.md` のとおりに起こした |
| `content/`(記事・文書・About の原稿) | `site/src/` だけ | 原稿の置き場(タスク 2.1 で移した)。見た目のファイルではない |
| `README.md` | `look/` だけ | `look/` の説明書きで、写すものではない |

### 差ではないが、記録に残す点

どれも `look/` のままの動きである。開発者が設計の段で見た画面と同じなので、直していない。

- **フッターの境目 `48rem`**: `base.css` には、設計の「決まった見た目」の表(「境目は 3 つ: 40rem、64rem、72rem」)に無い、4 つ目の境目がある。幅 48rem 以上で、フッターを 3 列(サイト名と作者、リンク、説明文)にし、リンクを縦に並べる。測ると、767px で 1 列、768px で 3 列だった。表との食い違いとして、ここに書いておく。
- **幅がちょうど 640px(40rem)のとき**: `base.css` の `(max-width: 40rem)` と `(min-width: 40rem)` の両方が当たる。顔は縦並び(アイコン絵の下に吹き出し)、記事の一覧の日付は題名の横(7rem の列)になる。639px ではどちらも縦、641px ではどちらも横だった。
- **目次の境目**: 狭い側は `(max-width: 63.99rem)`、広い側は `(min-width: 64rem)` と書いてある。1023px で畳んだ目次、1024px で右の目次になった。
- **`tags: []` の記事**: 空の `tag-list` が出る(`TagList.astro` は `look/` のまま。タスク 5.2 で見た)。今の原稿に、`tags: []` の記事は無い。
- **`prose.css` の置かれ方**: ビルドすると、`prose.css` は 4,096 バイト未満なので、記事・文書・About の HTML の `<style>` に埋め込まれる(Astro の既定)。`tokens.css` と `base.css` は、`/_astro/BaseLayout.<名前>.css` の別ファイルになる。中身は `look/` と同じで、置かれ方が違うだけである。
- **ビルドした CSS の幅の条件**: ビルドすると、幅の条件は `(width>=40rem)`・`(width<=40rem)` の書き方に縮められる(Vite の既定。6 種、`48rem` も含む)。元のファイルは `min-width`・`max-width` の書き方のままである。古いブラウザでの影響は、下の「開発者に見せる点」の 7 にある。

## 画面の確かめ(タスク 8.1)

タスク 8.1 で確かめた(2026-10-15)。`design.md` の「Testing Strategy」の「画面の確かめ」のうち、ページの種類 × 明暗 × 広い幅・狭い幅、決まった見た目の値、コメント欄の出る場所、書き換えの反映、公開と同じパスを見た(Requirement 3 の 1、6、7、8、10、13、14、15、22、23、Requirement 4 の 2、Requirement 5 の 1、2、4)。目次・前後の記事・コピーのボタンの動きは、タスク 4.2・4.3 で見てある。

### 手元の表示の始め方(Requirement 5 の 1)

`.kiro/steering/tech.md` の「手元の表示」のとおりに、`site/` で `npm ci` → `npm run dev` と打った。

- `npm ci` は終了コード 0 で終わった(終わりに、下の 8 の low 4 件の知らせが出る)。`npm ls katex` は 0.16.47 の 1 つだけだった。
- `npm run dev` は、約 14 秒で `http://localhost:4321/` を示して待ち受けた。ブラウザで `http://localhost:4321/` を開くと、トップページが出た。この PC では `[::1]:4321` だけで待ち受けた(`tech.md` の注のとおり。`localhost` で開けば繋がる)。
- 書いたとおりに動いた。直すところは無かった。

### 確かめ方

- **道具は 2 つ**:
  - Claude のアプリのブラウザ。幅と配色を切り替えてページを開き、計測のスクリプトを流した。書き換えの反映も、これで見た。
  - 写し(PNG)と、全部の組の計測は、この PC の Microsoft Edge をヘッドレスで動かし、開発者ツールの仕組み(CDP)で幅と配色(`prefers-color-scheme`)を切り替えて取った。スクリプトはセッションの一時の置き場に置き、リポジトリには入れていない。スクロールバーは隠した(幅 1280px で、ページの幅が 1280px になる)。幅 375px は、携帯の端末として扱わせた。
  - 同じ組(トップと v7 の、暗い配色・幅 375px)を両方で測り、値が一致することを見た。
- **計測**: ページの幅(`scrollWidth`)と画面の幅(`clientWidth`)、横に送れる箱の外で画面の端からはみ出す要素、トークンの値と、要素ごとの計算された色・書体・枠・影、書体が読み込まれたか(`document.fonts.check`)、コメント欄の要素の有無。
- **写し**: 手元の表示だけに出る Astro の道具の帯(画面の下の黒い帯)は、写す前に取り除いた。公開するページには出ない。

### 見たページと条件

次の 11 ページを、明るい配色と暗い配色、幅 1280px と 375px の 4 つの組で見た(44 組)。境目の確かめでは、トップと `mascot_requirements` を、幅 639・640・641・767・768・1023・1024・1151・1152・1920px でも見た。

| ページ | パス | 375px での横送り(明暗とも) | コメント欄 |
|---|---|---|---|
| トップ | `/` | 無い(375 = 375) | 無い |
| 記事(見出し・囲み・表) | `/2025/12/06/mascot_requirements.html` | 無い(375 = 375) | ある |
| 記事(長いコード) | `/2026/01/06/syuzo-to.html` | 無い(375 = 375) | ある |
| 記事(画像) | `/2020/08/10/emo.html` | 無い(375 = 375) | ある |
| 記事(別リポジトリの画像) | `/2026/03/26/emo2.html` | 無い(375 = 375) | ある |
| 記事(別リポジトリの画像) | `/2024/01/01/nyan_pu_puu.html` | 無い(375 = 375) | ある |
| 記事(表・コード・ロゴの SVG) | `/2026/02/06/pasta.html` | 無い(375 = 375) | ある |
| 文書(数式と表) | `/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.html` | 無い(375 = 375) | ある |
| 文書(数式と表) | `/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.html` | 無い(375 = 375) | ある |
| About | `/about.html` | 無い(375 = 375) | 無い |
| 404 | `/404.html` | 無い(375 = 375) | 無い |

- 「375 = 375」は、ページの幅と画面の幅である。44 組のすべてで、ページの幅は画面の幅と同じだった。横に送れる箱の外で画面の端からはみ出す要素は、44 組とも 0 個だった。
- 幅 375px では、表・コード・独立した行の数式が、その箱の中だけで横に送れる(`overflow-x: auto`)。中身が箱より広いものは、v7 で 13 個、v1 で 7 個、`mascot_requirements`(アドベントカレンダーの表)・`syuzo-to`(コード)・`pasta`(コード)で 1 個ずつあった。どれもページ全体は送らない(Requirement 3 の 22、23)。画像は幅に収まった(`emo` の画像は、1280px で 692px、375px で 343px)。
- 囲み `IMPORTANT` は `/2024/12/23/nyan_pu_puu_word.html` にだけある。このページも 4 つの組で見て、横送りは無く、コメント欄はあった。
- **コメント欄**(Requirement 4 の 1、2): giscus の埋め込み(`section.comments` とスクリプト)は、記事と文書のすべてにあり、トップ・About・404 には無かった。About は `comments: false` である。`syuzo-to` で、giscus の `<iframe>` が `theme=preferred_color_scheme`・`term=2026/01/06/syuzo-to` で作られた。暗い配色では暗く、明るい配色では明るく出た(写しの 20、20b)。ログインも投稿もしていない。
- About には、日付の欄(`<time>`)も目次も無い。`mascot_requirements` と About は `<h1>` が 2 つある(原稿の本文に `#` の見出しがあるため。原稿由来で、直さない)。

### 暗い配色でのそろい方(Requirement 3 の 15)

暗い配色の 22 組すべてで、地は `rgb(26, 32, 48)`(`#1a2030`)、文字は `rgb(240, 237, 228)`(`#f0ede4`)だった。

| 部分 | 暗い配色で測った値 |
|---|---|
| コード(`syuzo-to`、`pasta`) | 地 `#141926`(`--color-code-bg`)、枠 2px `#f0ede4`、影は黒。色付けは `--shiki-dark` の系統(`rgba(219, 215, 202, 0.93)`、`#666666`、`rgb(77, 147, 117)`、`rgb(212, 151, 108)`)。コピーのボタンは地 `#232b3f`、文字 `#f0ede4` |
| 数式(v7) | `.katex` の色が `#f0ede4`(周りの文字の色を引き継ぐ)。読めない数式は 0、数式は 124 個 |
| 囲み NOTE(`mascot_requirements`) | 地は NOTE の色を面に 1 割混ぜた色(約 `#2b3952`)、左の線 5px `#74b3ff`、枠 2px `#f0ede4`、見出し `#74b3ff` |
| 囲み IMPORTANT(`nyan_pu_puu_word`) | 地 約 `#39344d`、左の線 5px `#ff86c8`、見出し `#ff86c8` |
| 表(v7 ほか) | 見出しのセルの地 `#232b3f`、文字 `#f0ede4`、線 `#3f4a66`。本文のセルは面の色を 55% |
| コメント欄 | giscus が閲覧者の配色に合わせ、暗く出た(写しの 20) |
| 画像の飾り(`emo`) | 枠 3px の点線 `#f0ede4`、影は差し色 3 割と細い線の色の混ぜ色(約 `#795463`)、地は白 |

明るい配色の 22 組も同じく、地 `#f6f0e1`、文字 `#22242c` で、コードの地 `#fffdf8`、囲みの地(NOTE 約 `#e8ebeb`、IMPORTANT 約 `#f9e4e3`)、表の線 `#cdbf9f` にそろっていた。

### 決まった見た目の表との突き合わせ

`design.md` の「決まった見た目」の表の値を、トップ・記事・文書の画面で測った。どの項目も表のとおりだった。

| 項目 | 測った値 |
|---|---|
| 配色(明るい) | トークン 12 個が表と同じ(地 `#f6f0e1`、面 `#fffaf0`、文字 `#22242c`、控えめな文字 `#5f5d66`、差し色 `#e0322d`、リンク `#b3231f`、触れたとき `#e0322d`、枠 `#22242c`、細い線 `#cdbf9f`、コードの地 `#fffdf8`、NOTE `#1a66c2`、IMPORTANT `#c21f70`)。計算された色でも、地・文字・面(ヘッダー)・控えめな文字(一覧の説明)・差し色(`h2` の「■」)・リンク・コードの地・囲みの線と見出しが、この値だった |
| 配色(暗い) | トークン 12 個が表と同じ(地 `#1a2030`、面 `#232b3f`、文字 `#f0ede4`、控えめな文字 `#aaa69c`、差し色 `#ff6a5c`、リンク `#ff9d92`、触れたとき `#ffd0ca`、枠 `#f0ede4`、細い線 `#3f4a66`、コードの地 `#141926`、NOTE `#74b3ff`、IMPORTANT `#ff86c8`)。計算された色も同じ |
| 背景 | `body` に `linear-gradient` が 2 本(横と縦)、大きさ `24px 24px`。線の色は明るい配色 `rgba(70, 90, 140, 0.1)`、暗い配色 `rgba(190, 210, 255, 0.06)` |
| 書体 | サイト名・題名(`h1`)・本文の見出し・日付・タグ・コピーのボタン・表の見出し・窓の帯・囲みの見出しが `DotGothic16`。本文が `M PLUS 1p`。2 つとも、どのページでも読み込まれていた。コード(`pre code` と文中の `code`)は `ui-monospace, "Cascadia Mono", Consolas, "BIZ UDGothic", "MS Gothic", monospace` |
| 面と枠 | 目次・窓・吹き出しが(前後の記事は、目次と同じ CSS の規則)、枠 2px の実線、角丸 12px(吹き出しは 18px)、影 `4px 4px 0`(明るい配色は `#22242c`、暗い配色は黒)。ヘッダーの下とフッターの上は 3px の点線 |
| リンク | 下線が `dotted`・太さ 2px |
| 画像の飾り | 角丸 18px、3px の点線の枠、影 `6px 6px 0`、地は白、内側の余白 8px。影の色は明るい配色で約 `#d3957d`(表の「くすんだ朱 `#d3957d` ほど」) |
| 飾らない画像 | ロゴの `pasta.svg`(`pasta` の記事)は飾りが外れていた(枠なし)。About のバナー `EmbeddedImage.png` と `emo` の絵は飾りつき(一覧は空のとおり) |
| コードの配色 | `vitesse-light`・`vitesse-dark` の色が出て、地はトークンの色。知らない言語 `pasta` のブロックは、色付けなし(1 色) |
| 囲み | 地は囲みの色を面に 1 割混ぜた色、左に 5px の色の線、まわりに 2px の枠。見出しの言葉は「NOTE」「IMPORTANT」のまま、囲みの色 |
| 見出し(本文) | `h2` の前に差し色の「■」、下に 3px の点線。`h3` の前に差し色の「□」 |
| トップページ | アイコン絵の傾きは `matrix(0.997564, -0.0697565, 0.0697565, 0.997564, 0, 0)`(左へ 4 度)、角丸 22%、余白 8px、地は白、やわらかい影。幅 375px では、アイコン絵の下に吹き出し。記事と文書は窓の形(明るい配色では黒い帯 `#22242c` に面の色の文字。暗い配色では帯が `#f0ede4` になる。帯の色は枠の色のトークンなので、明暗で入れ替わる)。記事の 1 件は日付・題名・説明(2 行まで)・タグで、画像は無い |
| 目次 | 幅 1024px 以上で本文の右(幅 272px = 17rem、`position: sticky`)。それより狭いとき、本文の前に畳んで置く |
| 本文の幅 | 上限が無い。本文の幅は、1280px の画面で 858px、1920px の画面で 1456px(目次の 272px を除いた残り) |
| 幅の切り替え | 40rem: 639px で顔は縦・日付は題名の上、641px で顔は横・日付は題名の横(640px は上の「`look/` との差分」の点)。64rem: 1023px で畳んだ目次、1024px で右の目次。72rem: 1151px で 1 列、1152px で 2 列(記事 714px と文書 298px)。ほかに、フッターの 48rem(上の「`look/` との差分」の点) |
| メニュー | About だけ。狭い幅でも折り返すだけ(`flex-wrap: wrap`) |

- **読みやすさの物差し**(Requirement 3 の 7): 測った色で計算したコントラスト比は、文字と地が 13.61:1(明るい)・13.87:1(暗い)、控えめな文字が 5.69:1(明るい配色の地の上)・5.81:1(暗い配色の面の上)、リンクと地が 5.81:1・8.11:1 で、設計の値と同じだった。
- favicon と、`<head>` の決まった項目は、成果物の検査(`npm run verify`)がビルドのたびに見る。

### 別リポジトリの画像

本文の画像のうち 3 か所は、別リポジトリの Pages にある。手元の表示では 404 で、出ないのが正しい。

| 記事 | 画像 |
|---|---|
| `/2024/01/01/nyan_pu_puu.html` | `/nyan-pu-puu/images/surface0.png` |
| `/2026/03/26/emo2.html` | `/ghost_dev/emo2/images/5.png` |
| `/2026/03/26/emo2.html` | `/ghost_dev/emo2/images/code.png` |

- 3 つとも、手元の表示は 404 を返し、ページの上で読み込まれていなかった。`<img>` には飾りのスタイル(点線の枠など)が当たっている。
- 飾りの見え方は、公開した後に、公開中のサイトで確かめる(タスク 8.4 が作る「公開後の確かめ」の欄)。
- About の `/ghost_dev/`・`/nyan-pu-puu/`・`/zzz-pasta/` は、画像ではなくリンクである。

### 書き換えの反映(Requirement 5 の 2)

`npm run dev` を動かしたまま、2 つのファイルを仮に書き換え、ブラウザで開いたままのページを見た。

- **記事**: `site/src/content/posts/2026-03-26-emo2.md` の本文の 1 行の末尾に、目印の文字を足した。開いていた `/2026/03/26/emo2.html` に、操作なしで目印が出た(手元の表示がページを読み込み直した。記録に `Reloaded data from 2026-03-26-emo2.md`)。
- **スタイル**: `site/src/styles/tokens.css` の明るい配色の `--color-bg` を、仮に `#00ff00` にした。ページを読み込み直さずに、地の色が `rgb(0, 255, 0)` に替わった(スタイルだけの差し替え)。
- どちらも、書き換える前の写しから戻した。SHA-256 が書き換える前と同じであること(`emo2.md` は `b97befce…`、`tokens.css` は `8e89042f…`)と、`git status` に差が無いことを見た。戻した後、ページも元の姿に戻った。

### 公開と同じパス(Requirement 5 の 4)

手元の表示で、公開と同じパスを開いた。

- 応答 200: `/`、`/2025/12/06/mascot_requirements.html`、`/2026/01/06/syuzo-to.html`、`/2026/03/26/emo2.html`、`/2024/01/01/nyan_pu_puu.html`、`/2020/08/10/emo.html`、`/doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.html`、`/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.html`、`/about.html`、`/feed.xml`、`/img/alto0044_400x400.png`、`/doc/cosmogenesis/entanglement_cosmogenesis_v1.pdf`。
- 非公開の 3 本(`/2016/08/17/GODZILLA.html`、`/2016/07/08/dcomp.html`、`/2019/08/31/IMM_API.html`)は 404 で、新しい 404 ページが出た。
- `/404.html` は、新しい 404 ページ(`lang="ja"`、正規の URL `https://ekicyou.github.io/404.html`)を出した。手元の表示は、このページを状態 404 で返す。`/index.md` は 404 である(Jekyll の原稿は、タスク 7.1 で消えた)。

### 写し

開発者に見てもらうための写し(PNG)を、セッションの一時の置き場の `screens-8.1/` に置いた(リポジトリには入れない)。タスク 8.5 で、手元の表示と合わせて見せる。

| ファイル | 中身 |
|---|---|
| `01-top-light-1280.png`、`21-top-dark-1280.png`、`22-top-light-375.png`、`02-top-dark-375.png` | トップ |
| `03-article-mascot-light-1280.png`、`04-article-mascot-dark-375.png` | 記事(`mascot_requirements`) |
| `05-doc-v7-light-1280-math.png`、`06-doc-v7-dark-375-math.png` | 文書 v7 の、数式の多いところ |
| `18-doc-v7-table-math-dark-1280.png`、`25-doc-v7-table-dark-375.png` | 文書 v7 の、数式の入った表(暗い配色) |
| `07-about-light-1280.png`、`24-about-dark-375.png` | About |
| `23-404-light-1280.png`、`08-404-dark-375.png` | 404 |
| `09-code-vitesse-light-1280.png`、`10-code-vitesse-dark-1280.png` | コードの配色(`syuzo-to`) |
| `11-copy-button-overlap-light-375.png` | 幅 375px のコピーのボタン(`syuzo-to`) |
| `12-copied-label-light-1280.png` | 「コピーしました」(明るい配色) |
| `13-v7-toc-open-light-375.png` | v7 の長い目次を、幅 375px で開いたところ |
| `14-lambdabar-light-1280.png`、`14b-lambdabar-dark-1280.png` | `\lambdabar` の数式 |
| `15-top-doc-descriptions-light-1280.png` | トップの文書の欄 |
| `16-pasta-title-table-light-1280.png` | `pasta` の記事の題名と表 |
| `17-alert-dark-1280.png`、`26-alert-important-light-1280.png`、`27-alert-important-dark-375.png` | 囲み(NOTE、IMPORTANT) |
| `19-image-decoration-light-1280.png`、`19b-image-decoration-dark-375.png` | 画像の飾り(`emo`) |
| `20-comments-dark-1280.png`、`20b-comments-light-375.png` | コメント欄 |

- `12` は、開発者のクリップボードを書き換えないよう、ページのクリップボードへの書き込みを「成功を返すだけ」に替えてから、ボタンを押して写した。ボタンの表示が「コピーしました」に替わることだけを見た。本物のクリップボードへの写しは、下の 3 のとおり、開発者に 1 度押してもらう。
- `26`・`27` は、ビルドした成果物(`npm run preview`)で写した。ほかは手元の表示で写した。

### 開発者に見せる点(タスク 8.5)

これまでのタスクで「開発者に画面で見せる」とした点を、ここに集めた。どれも、設計の指定か `look/` のままの動き、または原稿由来で、実装では変えていない。

1. **コードの配色の、コメントと記号の薄さ**(タスク 4.2): `vitesse` のコメントと記号の色は、コードの地とのコントラスト比が、明るい配色で 2.80:1(`#999999` と `#fffdf8`)、暗い配色で 3.06:1(`#666666` と `#141926`)である。地の文字は 11.30:1・12.19:1、ほかの色は 4.80〜7.05:1。直すなら、`prose.css` で薄い色だけを上書きするか、配色の組を替える。写し `09`、`10`。
2. **幅 375px で、コピーのボタンがコードの 1 行目に重なる**(タスク 4.3): `/2026/01/06/syuzo-to.html` で、横に送る前、ボタンが 1 行目の末尾を隠す。横に送れば読める。写し `11`。
3. **コピーのボタンを、開発者が 1 度押す**(タスク 4.3): 貼り付けた文字がコードそのままであることを、開発者のクリップボードで見てもらう(確かめでは、クリップボードに渡る文字だけを見た)。
4. **明るい配色の「コピーしました」は 4.33:1**(タスク 4.3): 差し色 `#e0322d` と面 `#fffaf0`。12px の文字で、2 秒だけ出る。設計の 4.5:1 の物差しの対象は、本文の文字とリンクである。写し `12`。
5. **文書の目次が長い**(タスク 5.1): v7 は 91 項目、v1 は 61 項目。広い幅では目次の中で送れるが、幅 375px で開くと高さが約 2,670px になり、本文がその分だけ下がる。写し `13`。
6. **文書 2 本の説明が、トップページで同じ文になる**(タスク 5.2): 「プログラム的予想と検証可能予言」。2 本の front matter の `description` が同じため(原稿由来)。写し `15`。
7. **ビルドした CSS の幅の条件の書き方と、対象のブラウザ**(タスク 4.1): ビルドした CSS は `(width>=40rem)` の書き方になる。Safari 16.4 より古いブラウザでは幅の切り替えが効かず、狭い幅の組み方のままになる(読めなくはならない)。`tokens.css` の `color-mix()` も、ほぼ同じ下限を持つ。設計に、対象のブラウザの決めは無い。救うなら、`astro.config.mjs` の `vite.build.cssTarget` で決められる。
8. **`npm audit` の low 4 件**(タスク 1.1): `katex` 0.11.0〜0.18.1 への勧告 GHSA-238p-pmpm-9mq7 と、その波及。直る版は `katex` 0.19.0 だが、`rehype-katex` 7.0.1 が `katex ^0.16.0` を求めるので、上げると版が 2 つになる。設計の固定(0.16.47)のまま据え置いている。数式はビルド時に自分の原稿だけを描く。2026-10-15 の `npm ci` でも、同じ 4 件が出た。受け入れるかは開発者が決める。
9. **`\lambdabar` の字形**(タスク 6.2): v7 の 685〜687 行の数式の分母。斜体の λ に短い横棒が重なり、ƛ と読める。やや粗い。写し `14`、`14b`。
10. **`pasta` の記事の表と題名**(タスク 6.2 の直しの結果): 見出しの無い表は、開発者が選んだ「表」の形で、リンク 2 つが見出しのセルに入る。題名はバッククォートの無い `SHIORI pasta.dllとサンプルゴーストをでっちあげる`。写し `16`。
11. **暗い配色の、トップページの欄の見出しの帯**(タスク 8.1): 帯は枠の色を地に使うので、暗い配色では明るい帯(`#f0ede4` の地に `#232b3f` の文字)になる。設計の「決まった見た目」は「黒い帯の見出し」と書くが、暗い配色の値は別に決めていない。`look/` とバイト一致の、開発者が画面で選んだ見た目のままである。写し `02`、`21`。

## Linux の確かめ

2026-10-15 に、開発者に確かめてから、ブランチ `claude/kiro-site-astro-redesign-2ed957` を push した(先頭は `39c7b03`)。push の前に `git fetch origin` で `origin/master` の先頭を見て、基準を取ったコミット `da322dd` のままであることを確かめた。そのうえで `pages.yml` を手動で実行した(`gh workflow run pages.yml --ref claude/kiro-site-astro-redesign-2ed957`)。

- 実行: 38645104903(`workflow_dispatch`、ランナーは `ubuntu-latest`)。結果は成功。
- `build` ジョブの段は、すべて成功した。
  - Node.js の用意: 主版だけを書いた `site/.node-version`(`26`)を読み、v26.11.1 を取ってきた。ワークフローに主版を直に書く手当ては要らなかった。
  - `npm ci`: 410 個を入れた。Windows で作ったロックファイルのまま、Linux で通った。
  - `npm run check`: 誤り 0 件。
  - `npm test`: 59 件すべて成功。
  - `npm run build`: 17 ページ。
  - `npm run verify`: 誤り 0 件、警告 0 件(ファイル 49 個、HTML 17 本、非公開の記事 3 本)。
  - 成果物の保存: `github-pages`(1,367,393 バイト)。
- `deploy` ジョブは動いていない(skipped)。`master` 以外の手動の実行なので、公開はしていない。

## 要件と設計の段で済んだ確かめの在り処

次の 2 つは、実装の前(要件と設計の段)に済ませた。実装では、その結果を使った。

- **画面の案を作って見比べること**(Requirement 3 の 12): 案の作り方、用意した案(A〜D の 4 つ)、見たページと条件(明暗、広い幅と狭い幅)、決まった値は、`design.md` の「画面の案」の節にある(決まった値は「決まった見た目(2026-10-12)」の表)。開発者が選んだ案 D「どっと方眼」の実物は `.kiro/specs/completed/site-astro-redesign/look/`(スタイル 3 つ、レイアウト 2 つ、部品 11 個、`README.md`)にある。案そのものは、リポジトリの外に作り、コミットしていない(Requirement 9 の 8)。
- **試作**(Requirement 9 の 6、7): 置き場と作り・使った版・確かめた結果(a〜i)・要件と設計への含みは、`research.md` の §2(§2.1〜§2.4)にある。Requirement 9 の 6 の 3 つは、§2.3 の a(URL)、c と d(数式とコード)、g(Windows と手元の Node.js での手元の表示)。結果を要件ディスカッションへ持ち込んで決めたことは、`requirements.md` の「要件ディスカッションの結果」にある。試作も、リポジトリの外に作り、コミットしていない(Requirement 9 の 8)。

## パスの突き合わせ

タスク 8.4 で確かめた(2026-10-15)。切り替えの前の公開物と、新しいビルドの成果物とを、公開されるパスの一覧で突き合わせた(Requirement 9 の 1、2)。

**基準が今も正しいこと**(`design.md` の「Migration Strategy」の B):

- 2026-10-15 07:59(日本時間)に `git fetch origin` を行い、`git rev-parse origin/master` を見た。結果は `da322dd7da2eb1d0ed878de55813eb837350f1d8` で、基準を取ったコミットと同じである。`master` は動いていない。足された記事も、作り直す一覧も無い。
- このブランチと `origin/master` の分かれ目(`git merge-base HEAD origin/master`)も `da322dd` である。

**突き合わせ方**: ブランチの先頭 `ad9aca2` の作業ツリーで、`site/` から `npm run build`(17 ページ、終了コード 0)を行い、続けて `node scripts/verify-dist.mjs --baseline ../.kiro/specs/completed/site-astro-redesign/baseline-paths.txt`(終了コード 0)を行った。

- 基準: 53 個。成果物: 49 個。両方にあるパス: 28 個(記事 12、文書 2、About、トップ、404、フィード、文書に添えるファイル 3、画像 7)。
- 検査の仕掛けの出力の最後の行は「参考: /assets/ の外で消えるパス 0 個、/_astro/ の外で増えるパス 0 個」。
- **自分の手での照合**: `node scripts/verify-dist.mjs --list`(49 行)と基準の一覧を、それぞれ並べ替えて `comm` で比べた。消えるパス 25 個(うち `/assets/` の外は 0 個)、増えるパス 21 個(うち `/_astro/` の外は 0 個)、両方にあるパス 28 個で、仕掛けの出力と一致した。`dist/` のファイルの数(`find dist -type f`)も 49 個である。

**消えるパス(25 個。どれも `/assets/` の下)**: Jekyll の見た目(minima)の出力で、設計の「消えてよいパス」と同じである。

- `/assets/css/style.css`
- `/assets/fonts/Noto-Sans-700/Noto-Sans-700.{eot,svg,ttf,woff,woff2}`(5 個)
- `/assets/fonts/Noto-Sans-700italic/Noto-Sans-700italic.{eot,svg,ttf,woff,woff2}`(5 個)
- `/assets/fonts/Noto-Sans-italic/Noto-Sans-italic.{eot,svg,ttf,woff,woff2}`(5 個)
- `/assets/fonts/Noto-Sans-regular/Noto-Sans-regular.{eot,svg,ttf,woff,woff2}`(5 個)
- `/assets/img/logo.png`
- `/assets/js/scale.fix.js`
- `/assets/main.css`
- `/assets/minima-social-icons.svg`

**増えるパス(21 個。どれも `/_astro/` の下)**: スタイルシート 2 個と、数式のフォント(`woff2`)19 個である。名前の中の印はビルドの中身で変わるので、数と種類だけを見る。

- スタイルシート: `/_astro/BaseLayout.BMO6HV9o.css`、`/_astro/katex.min.BYkPozyE.css`
- 数式のフォント: `/_astro/KaTeX_AMS-Regular.BQhdFMY1.woff2`、`/_astro/KaTeX_Caligraphic-Bold.Dq_IR9rO.woff2`、`/_astro/KaTeX_Caligraphic-Regular.Di6jR-x-.woff2`、`/_astro/KaTeX_Fraktur-Bold.CL6g_b3V.woff2`、`/_astro/KaTeX_Fraktur-Regular.CTYiF6lA.woff2`、`/_astro/KaTeX_Main-Bold.Cx986IdX.woff2`、`/_astro/KaTeX_Main-BoldItalic.DxDJ3AOS.woff2`、`/_astro/KaTeX_Main-Italic.NWA7e6Wa.woff2`、`/_astro/KaTeX_Main-Regular.B22Nviop.woff2`、`/_astro/KaTeX_Math-BoldItalic.CZnvNsCZ.woff2`、`/_astro/KaTeX_Math-Italic.t53AETM-.woff2`、`/_astro/KaTeX_SansSerif-Bold.D1sUS0GD.woff2`、`/_astro/KaTeX_SansSerif-Italic.C3H0VqGB.woff2`、`/_astro/KaTeX_SansSerif-Regular.DDBCnlJ7.woff2`、`/_astro/KaTeX_Script-Regular.D3wIWfF6.woff2`、`/_astro/KaTeX_Size1-Regular.mCD8mA8B.woff2`、`/_astro/KaTeX_Size2-Regular.Dy4dx90m.woff2`、`/_astro/KaTeX_Size4-Regular.Dl5lxZxV.woff2`、`/_astro/KaTeX_Typewriter-Regular.CO6r4hn1.woff2`

**判定**: 差は、消える `/assets/` の 25 個と、増える `/_astro/` の 21 個だけである。意図しない差は無い。マージを止める差は無い(Requirement 9 の 2)。この結果は、タスク 8.5 で開発者に示す(Requirement 9 の 1)。

## 履歴

タスク 8.4 で確かめた(2026-10-15)。移した原稿 18 本(記事 15、文書 2、About 1)の履歴が、移す前まで辿れることを見た(Requirement 2 の 16)。

**このブランチの上で**: 移動のコミットは `1181eeb`(タスク 2.1。28 個すべてが `R100`、中身は同じ)。18 本それぞれに `git log --follow -- <新しいパス>` を打ち、`1181eeb` より古いコミットまで出ることを見た。18 本とも、移動の前の履歴まで辿れた。

| 新しいパス(`site/src/content/` の下) | 出たコミット | うち移動より古いもの | 一番古いコミット(そのときのパス) |
|---|---|---|---|
| `doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md` | 6 | 4 | `1a31013` 2026-05-06(`doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md`) |
| `doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md` | 5 | 2 | `31a85d4` 2026-05-10(`doc/cosmogenesis/entanglement_cosmogenesis_v7_ja.md`) |
| `pages/about.md` | 12 | 11 | `e116822` 2018-08-26(`about.md`) |
| `posts/2014-10-04-pasta.md` | 4 | 3 | `1be61f1` 2018-08-26(`_posts/2014-10-04-pasta.md`) |
| `posts/2016-02-16-code.md` | 3 | 2 | `1be61f1` 2018-08-26(`_posts/2016-02-16-code.md`) |
| `posts/2016-06-18-sun.md` | 3 | 2 | `1be61f1` 2018-08-26(`_posts/2016-06-18-sun.md`) |
| `posts/2016-07-08-dcomp.md` | 4 | 3 | `1be61f1` 2018-08-26(`_posts/2016-07-08-dcomp.md`) |
| `posts/2016-08-17-GODZILLA.md` | 3 | 2 | `1be61f1` 2018-08-26(`_posts/2016-08-17-GODZILLA.md`) |
| `posts/2018-08-26-new_site.md` | 6 | 5 | `8a1db3d` 2018-08-26(`_posts/2018-08-26-new_site.md`) |
| `posts/2019-08-31-IMM_API.md` | 4 | 3 | `c9c5203` 2019-08-27(`_posts/2019-08-31-IMM_API.md`) |
| `posts/2020-08-10-emo.md` | 17 | 16 | `c552f91` 2020-08-10(`_posts/2020-08-10.md`) |
| `posts/2024-01-01-nyan_pu_puu.md` | 10 | 9 | `c639de4` 2024-01-01(`_posts/2024-01-01-nyan-pu-puu.md`) |
| `posts/2024-12-23-nyan_pu_puu_word.md` | 35 | 34 | `c639de4` 2024-01-01(`_posts/2024-01-01-nyan-pu-puu.md`) |
| `posts/2025-12-06-mascot_requirements.md` | 43 | 42 | `c639de4` 2024-01-01(`_posts/2024-01-01-nyan-pu-puu.md`) |
| `posts/2026-01-06-syuzo-to.md` | 12 | 11 | `d9d57cb` 2026-01-06(`_posts/2026-01-06-syuzo.md`) |
| `posts/2026-02-06-pasta.md` | 9 | 7 | `63c8574` 2026-02-06(`2026-02-06-pasta.md`) |
| `posts/2026-03-26-emo2.md` | 4 | 3 | `f99dd63` 2026-03-26(`_posts/2026-03-26-emo2.md`) |
| `posts/2026-05-06-cosmogenesis.md` | 7 | 6 | `1a31013` 2026-05-06(`_posts/2026-05-06-cosmogenesis.md`) |

- 移動の 1 つ前のコミットは、どれも `da322dd`(1 本目の spec。ルートから `site/` へ移した)で、そこから先も名前の変更として辿れた。例: `2026-02-06-pasta.md` は `fa03dbd`(直し) → `1181eeb`(`R100`) → `da322dd`(`R100`) → `4151d6c`・`7056d6d`(`_posts/` での書き換え) → `049a3ce`(`R100`、ルートから `_posts/` へ)。
- `2024-12-23-nyan_pu_puu_word.md` と `2025-12-06-mascot_requirements.md` の一番古いコミットが `2024-01-01-nyan-pu-puu.md` になるのは、`--follow` が昔の似た中身の記事を元と見たためで、この移行とは関わらない(移行の前の `master` でも同じに出る)。

**squash の後も辿れるか**: この spec は 1 つの PR の squash で `master` に入るので、移動と直しのコミットは 1 つにまとまる(`design.md` の「移行の記録に残すもの」)。支えになるのは、git の名前変更の検出(中身の似かたが 50% 以上)である。

- `git diff -M --name-status origin/master HEAD -- site/` で、squash のコミットが持つ差を見た。移した 28 個は、どれも `R`(名前の変更)と出た。直した 3 本の似かたは、v1 が 97%(`R097`)、v7 が 96%(`R096`)、`2026-02-06-pasta.md` が 93%(`R093`)で、ほかの 25 個は 100%。
- squash の姿を、ref を動かさずに確かめた。`git commit-tree 'HEAD^{tree}' -p origin/master` で、`origin/master` の上に今のツリーを載せたコミットの物(`5c26560`)を作り、どのブランチにも付けずに、18 本それぞれに `git log --follow 5c26560 -- <新しいパス>` を打った。18 本とも、そのコミットが `R093`〜`R100` の名前の変更として出て、その先に、上の表の「移動より古いもの」と同じ数のコミットが続いた(例: About は 1 + 11 = 12、pasta は 1 + 7 = 8、v7 は 1 + 2 = 3)。squash でマージしても、移す前の履歴まで辿れる見込みである。作った物はどの ref からも指されず、git の掃除で消える。
- マージの後に、`master` の上で 1 本だけ `git log --follow` を打って、同じことを確かめるとよい(下の「公開後の確かめ」の 6)。

## 公開後の確かめ

欄はタスク 8.4 で作った(2026-10-15)。結果は、マージと公開の後に `/kiro-complete` が書く(Requirement 9 の 4、5)。崩れていたら、その場で開発者に報告し、`tech.md` の「戻し方」(まず前へ直す PR、見た目が崩れたまま公開されたなら R3)を示す。戻す操作は、開発者の許しを得るまで行わない。

- 確かめた日時: 公開後に書く
- 公開した `master` のコミットと、`pages.yml` の実行の番号と成否: 公開後に書く
- 判定は状態コードで行う(無いパスにも 404 ページの中身が返る)。公開の直後は、CDN が最長 10 分ほど前の応答を返すことがあるので、期待と違ったら少し待って確かめ直す。コマンドは `workflow.md` の「代表パス」にある。

**1. 応答するはず(200)**: `workflow.md` の「代表パス」と同じ 8 つ。

| パス | 期待 | 結果 |
|---|---|---|
| `/` | 200 | 公開後に書く |
| `/about.html` | 200 | 公開後に書く |
| `/feed.xml` | 200 | 公開後に書く |
| `/2026/03/26/emo2.html` | 200 | 公開後に書く |
| `/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.html` | 200 | 公開後に書く |
| `/doc/cosmogenesis/entanglement_cosmogenesis_v1.pdf` | 200 | 公開後に書く |
| `/img/alto0044_400x400.png` | 200 | 公開後に書く |
| トップページの HTML が指す `/_astro/…css`(名前は HTML から読み取り、ここに書く。空なら崩れ) | 200 | 公開後に書く |

**2. 応答しないはず(404)**: `workflow.md` の「代表パス」と同じ 7 つ。

| パス | 期待 | 結果 |
|---|---|---|
| `/site/` | 404 | 公開後に書く |
| `/CLAUDE.md` | 404 | 公開後に書く |
| `/README.md` | 404 | 公開後に書く |
| `/package.json` | 404 | 公開後に書く |
| `/src/content/posts/2026-03-26-emo2.md` | 404 | 公開後に書く |
| `/sitemap.xml` | 404 | 公開後に書く |
| `/assets/main.css`(切り替えで消えたパス) | 404 | 公開後に書く |

**3. 別リポジトリの画像の飾り**: 手元の表示では 404 で見られなかった 3 つを、公開中のサイトのブラウザで見る(「画面の確かめ」の「別リポジトリの画像」)。画像が出ること、本文の画像の飾り(角丸、3px の点線の枠、影など。「画面の確かめ」の「決まった見た目の表との突き合わせ」の「画像の飾り」と同じ)が当たって崩れていないことを、明るい配色と暗い配色で見る。

| 記事 | 画像 | 画像が出るか | 飾りの見え方 |
|---|---|---|---|
| `/2024/01/01/nyan_pu_puu.html` | `/nyan-pu-puu/images/surface0.png` | 公開後に書く | 公開後に書く |
| `/2026/03/26/emo2.html` | `/ghost_dev/emo2/images/5.png` | 公開後に書く | 公開後に書く |
| `/2026/03/26/emo2.html` | `/ghost_dev/emo2/images/code.png` | 公開後に書く | 公開後に書く |

- 透過の絵などで飾りが合わないものは、スタイルシートの「飾らない画像」の一覧に挙げるかを、開発者が決める(`design.md` の「画像の飾りから外す決まり」)。

**4. コメント欄が付いたままか**: 公開中の `/2026/03/26/emo2.html` を開き、giscus の `<iframe>` が `term=2026/03/26/emo2`(対応の取り方 `pathname`)で作られることを見る(「コメント欄の設定の突き合わせ」)。投稿はしない。

- 結果: 公開後に書く

**5. フィード**: `/feed.xml` が 200 で、RSS 2.0 として読め、12 項目のリンクが `https://ekicyou.github.io` + 公開パス(`.html` つき)であることを見る(「検査の確かめ」の 5)。

- 結果: 公開後に書く

**6. `master` の上の履歴**: マージの後の `master` で、1 本(例: `site/src/content/posts/2026-02-06-pasta.md`)に `git log --follow` を打ち、squash のコミットの先に、移す前のコミット(`049a3ce` など)が出ることを見る(上の「履歴」)。

- 結果: 公開後に書く

**判定**: 公開後に書く(崩れが無い、または、崩れの中身と開発者への報告)

## 記録の欄のそろい

タスク 8.4 で、`tasks.md` の 8.4 が挙げる欄がそろっていることを見た(2026-10-15)。

| 欄 | 在り処 | 状態 |
|---|---|---|
| 直した箇所の一覧 | 「直した箇所の一覧」(タスク 2.2、6.2) | 済み |
| 数式の確かめ | 「数式の確かめ」(タスク 6.2) | 済み |
| 検査の確かめ: 成果物の検査の仕掛け | 「検査の確かめ」の 1(タスク 6.3) | 済み |
| 検査の確かめ: front matter | 「検査の確かめ」の 2 | 済み |
| 検査の確かめ: 記事を足す流れ | 「検査の確かめ」の 3 | 済み |
| 検査の確かめ: バイト一致 | 「検査の確かめ」の 4 | 済み |
| 検査の確かめ: フィード、`katex` の版 | 「検査の確かめ」の 5 | 済み |
| `look/` との差分 | 「`look/` との差分」(タスク 8.1) | 済み |
| Linux の確かめ | 「Linux の確かめ」(タスク 7.4) | 済み |
| パスの突き合わせ | 「パスの突き合わせ」(タスク 8.4) | 済み |
| 履歴 | 「履歴」(タスク 8.4) | 済み |
| 要件と設計の段で済んだ確かめの在り処 | 「要件と設計の段で済んだ確かめの在り処」(タスク 8.4) | 済み |
| 公開後の確かめ | 「公開後の確かめ」(欄はタスク 8.4) | 結果は公開後に `/kiro-complete` が書く |
| 開発者の許し | 「開発者の許し」 | ある(タスク 8.5、2026-10-15) |

## 開発者の許し

2026-10-15 に、開発者が切り替えを明示して許した(チャットで「切り替えてください」)。

- 示したもの: 新しいデザインの画面の写し 12 枚(トップ・記事・文書 v7 の明暗と広い幅・狭い幅、「開発者に見せる点」の写し)、手元の表示(`npm run dev`、`http://localhost:4321/`)、パスの突き合わせの結果(消えるのは `/assets/` の 25 個、増えるのは `/_astro/` の 21 個だけ)、`master` が基準のコミット `da322dd` のままであること、Linux の確かめ(手動の実行 38645104903)。
- 「開発者に見せる点(タスク 8.5)」の 1〜11 は、切り替えの前に直すものとして番号を挙げられなかったので、どれも今の形のまま受け入れた。
- 3(コピーのボタンを開発者が 1 度押し、貼り付けた文字を見る)の結果は、許しの時点で報告を受けていない。
- マージと公開後の確かめは、`/kiro-complete` が行う。
