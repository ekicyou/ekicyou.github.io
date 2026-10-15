# 決まった見た目の実物(どっと方眼)

設計ディスカッション(2026-10-12)で、開発者がブラウザで見て決めた画面の、スタイルと部品の実物。`design.md` の「画面の案」の「決まった見た目」の表と対になる。表は値の一覧、このフォルダは、その値で実際に動いた画面である。

## 使い方

- 実装は、このフォルダのファイルを `site/src/` の同じ名前の場所へ写して、下敷きにしてよい(`styles/` → `site/src/styles/`、`components/` → `site/src/components/`、`layouts/` → `site/src/layouts/`)。
- 写した後は、`site/src/` の側が正本になる。このフォルダは、決めた時点の記録として残し、後から直さない。
- 実装した画面が、開発者の見た画面とずれていないかを確かめるときの、突き合わせの相手にも使う。

## 中身

| 場所 | 中身 |
|---|---|
| `styles/tokens.css` | デザインのトークン(色、書体、角丸、影、余白)。明るい配色と暗い配色 |
| `styles/base.css` | 土台(ヘッダー、フッター、トップページ、記事ページの骨組み、目次、前後の記事、404)。末尾に「どっと方眼の飾り」 |
| `styles/prose.css` | 本文(見出し、コード、表、画像の飾り、囲み、数式)。末尾に「どっと方眼の飾り」 |
| `layouts/` | `BaseLayout.astro`、`ArticleLayout.astro` |
| `components/` | `HeadMeta`、`SiteHeader`、`SiteFooter`、`Hero`、`PostList`、`DocList`、`Toc`、`TagList`、`PostNav`、`Comments`、`CodeCopy` |

## このフォルダに無いもの(`design.md` のとおりに作る)

- `consts.ts`: 部品は `SITE` の `url`・`title`・`description`・`author`・`ogLocale`・`icon`・`favicon`・`feedPath`・`fontsHref`・`nav`・`links`・`giscus` を読む。`fontsHref` は `https://fonts.googleapis.com/css2?family=DotGothic16&family=M+PLUS+1p:wght@400;500;700&display=swap`、`favicon` は `/img/alto0044_400x400.png`。
- `lib/`(`post-path.ts`・`posts.ts`・`docs.ts`・`summary.ts`)、`pages/`(ルート)、`content.config.ts`、`astro.config.mjs`。
- Shiki の配色は `vitesse-light` と `vitesse-dark`(`astro.config.mjs` に書く)。

## 画面の案から、わざと落としたもの

- 案を切り替える仕組み(4 つの案の設定、案どうしを行き来するリンク)。リポジトリに入れない(Requirement 8 の 2)。
- 選ばれなかった形のスタイル(カードの一覧、中央寄せの顔、左の目次)。
- 別リポジトリの画像を、公開中のサイトから読む設定(手元の表示のための、その場しのぎ)。

## 確かめたこと

このフォルダの中身だけ(案 D に絞ったもの)でビルドし、案 D と同じ画面になることを、トップページと記事ページで見比べた(2026-10-12)。