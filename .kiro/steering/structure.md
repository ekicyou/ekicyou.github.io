---
inclusion: always
updated_at: 2026-10-15
---

# Project Structure

## Organization Philosophy

サイトの原稿と仕組みは `site/`（Astro のプロジェクト）にまとめ、その中は Astro の決まりどおりに配置する（`src/` に原稿と部品、`public/` に加工しないファイル）。**ルートに置くのは開発用だけ。`site/` の外は公開されない**（ワークフローが `site/` だけをビルドするため）。

公開されるのは、ビルドの成果物 `site/dist/` だけである。`site/src/content/` の原稿は HTML になって、`site/public/` のファイルは中身を変えずに公開される。`site/` の中でも、`package.json`、`scripts/`、`tests/` などは公開されない。

## Directory Patterns

### 記事
**Location**: `site/src/content/posts/`
**Purpose**: 公開する記事（コレクション `posts`）。ファイル名は `YYYY-MM-DD-<名前>.md`。日付と名前が URL（`/YYYY/MM/DD/<名前>.html`）になるので、公開後は変えない。URL に `site/` や `src/content/` は入らない。
**Example**: `site/src/content/posts/2026-03-26-emo2.md` → `/2026/03/26/emo2.html`

記事は、記事の置き場の下のどのフォルダにも置ける（深さを問わない）。URL はファイル名だけで決まり、フォルダ名は入らない。非公開の記事も同じ置き場に置く。URL を決めるのは `site/src/lib/post-path.ts` だけである。

2010 年以前の記事は `archive/` にある。これから書く記事は直下に置く。さらに古い記事を `archive/` へ移すのは、`git mv` の手作業で足りる（仕組みの変更は要らない）。フォルダ `archive/` は、ページ `/archive.html` とは関係が無い。

同じファイル名の記事が 2 本以上あると、すべての場所を挙げてビルドが止まる。フォルダが違っても、非公開の記事でも、大文字・小文字だけが違っても止まる。

### 文書
**Location**: Markdown は `site/src/content/doc/<題材>/`、PDF・TeX の原稿・その題材の画像は `site/public/doc/<題材>/`
**Purpose**: 記事から参照する長文と、その付属物（コレクション `doc`）。Markdown は HTML に変換されて、ほかのファイルはそのまま `/doc/<題材>/` の下に公開される。
**Example**: `site/src/content/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md` → `/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.html`、`site/public/doc/cosmogenesis/entanglement_cosmogenesis_v1.pdf` → `/doc/cosmogenesis/entanglement_cosmogenesis_v1.pdf`

### 画像
**Location**: `site/public/img/`
**Purpose**: サイト全体で使う画像と、記事の SNS 用画像。`/img/…` で公開される。記事用は `site/public/img/<記事の日付>-<名前>.png` に揃える。

### 単独のページ
**Location**: About の本文は `site/src/content/pages/about.md`（コレクション `pages`）。トップページと 404 は `site/src/pages/` の `index.astro`・`404.astro`
**Purpose**: 記事ではない固定のページ（`/`、`/about.html`、`/404.html`）。ヘッダーのメニューは自動では並ばず、`site/src/consts.ts` の `SITE.nav` に書く（今は About だけ）。

### ページのルート
**Location**: `site/src/pages/`
**Purpose**: 公開パスとページの対応。`index.astro`（`/`）、`about.astro`（`/about.html`）、`404.astro`（`/404.html`）、`feed.xml.ts`（`/feed.xml`）、`[year]/[month]/[day]/[name].astro`（記事）、`doc/[...id].astro`（文書）。

### レイアウトと部品とスタイル
**Location**: `site/src/layouts/`、`site/src/components/`、`site/src/styles/`
**Purpose**: ページの枠（`BaseLayout` → `ArticleLayout`）、部品（ヘッダー、フッター、一覧、目次、タグ、前後の記事、コメント欄、コードのコピー）、スタイル（`tokens.css` のデザインのトークン、`base.css` の土台、`prose.css` の本文）。見た目の正本は、ここである。

### 仕組みの部品
**Location**: `site/src/content.config.ts`、`site/src/consts.ts`、`site/src/lib/`
**Purpose**: コレクション 3 つの定義と front matter の検査（`content.config.ts`）。サイト名、説明文、作者、リンク、giscus の設定、メニュー（`consts.ts`）。記事の URL、記事と文書の一覧、説明の決め方（`lib/`）。

### サイトの設定と版
**Location**: `site/astro.config.mjs`、`site/package.json`、`site/package-lock.json`、`site/.node-version`、`site/tsconfig.json`
**Purpose**: 出力の形と Markdown の処理（`astro.config.mjs`）、依存の版と npm のスクリプト（`package.json`・`package-lock.json`）、Node.js の主版（`.node-version`）、型の検査の設定（`tsconfig.json`）。

### 検査とテスト
**Location**: `site/scripts/verify-dist.mjs`、`site/tests/`
**Purpose**: 成果物の検査と、保つパスの一覧（`tests/kept-paths.txt`）。`lib/` の単体テスト（`tests/*.test.ts`）。公開はされない。

### 生成物（管理しない）
**Location**: `site/node_modules/`、`site/dist/`、`site/.astro/`
**Purpose**: 依存、ビルドの成果物、Astro の作業フォルダ。ルートの `.gitignore` が無視する。

### 開発用（公開しない）
**Location**: ルート（`CLAUDE.md`、`README.md`、`.kiro/`、`.claude/`、`.github/`、`.vscode/`、ドット始まりの設定ファイル）
**Purpose**: 仕様駆動開発の文書とスキル（`.kiro/`、`.claude/`）、ビルドと公開のワークフローと Dependabot の設定（`.github/`）、リポジトリの案内と設定。`site/` の外にあるので公開されない。

## Naming Conventions

- **記事**: `YYYY-MM-DD-<名前>.md`。名前の綴り（大文字・小文字も）がそのまま URL になる。これから書く記事は、小文字の英数字とアンダースコアにそろえる（古い記事には大文字やハイフンの名前もあるが、URL を保つため変えない）
- **spec**: 小文字のハイフン区切り（例: `site-subfolder-actions-build`）。区分や番号は付けない
- **コミット**: `<type>(<scope>): <日本語の要約>`（例: `feat(posts): …`、`fix(core): …`）

## 記事の書き方

記事・文書・About に共通の決まり。決まりに合わない原稿は、多くがビルドを止める（理由とファイルが示される）。

### ファイル名
- 記事は `site/src/content/posts/YYYY-MM-DD-<名前>.md`。下のフォルダに置いてもよい。同じファイル名は置けない（ビルドが止まる）。
- 名前に `index` は使えない（ビルドが止まる）。
- 記事の日付は、ファイル名の日付で決まる（日本時間の 0 時）。front matter の `date` は見ない。
- 非公開の記事も、この決まりに合わないとビルドが止まる。

### front matter
書けるのは次の 9 項目だけ。ほかの項目（打ち間違いを含む）があると、ビルドが止まる。

| 項目 | 型 | 必須 | 使い道 |
|---|---|---|---|
| `title` | 文字列 | 必須 | 題名 |
| `layout` | 文字列 | 任意 | 受け入れるだけ（使わない） |
| `description` | 文字列 | 任意 | 説明（一覧、SNS 向けの情報、フィード） |
| `image` | 文字列（`/` 始まりの公開パス） | 任意 | SNS 向けの画像 |
| `tags` | 文字列の並び | 任意 | タグの表示、フィードの分類 |
| `comments` | 真偽 | 任意 | `false` のときコメント欄を出さない |
| `use_math` | 真偽 | 任意 | 受け入れるだけ（使わない。数式は書けば描かれる） |
| `published` | 真偽 | 任意 | `false` のとき、どこにも出さない |
| `date` | 日付 | 任意 | 受け入れるだけ（使わない。日付はファイル名で決まる） |

- `description` が無いと、記事と文書は本文の最初の段落から説明を作る（About はサイトの説明文になる）。囲みや数式で始まる原稿には `description` を書く（最初の段落から作る説明が崩れる）。
- `image` が無いと、SNS 向けの画像はサイトのアイコン絵になる。

```yaml
---
title: 題名
description: 一覧と SNS に出る説明
image: /img/2026-03-26-emo2.png
tags:
  - 伺か
---
```

### 数式
- 文中の数式は `$…$`。独立した行の数式は、`$$` を単独の行に置いて挟む。

  ```markdown
  $$
  E = mc^2
  $$
  ```

- 数式でない `$` は `\$` と書く。
- 見出しに数式を書かない。ギリシャ文字は `α` のように文字のまま書く（見出しの数式は、目次の文字と見出しの id に TeX の文字まで入る）。
- `use_math` は要らない。数式はビルド時に KaTeX で描かれる。KaTeX が読めない数式は、ビルドを止めず、TeX の文字のまま出る（`npm run verify` が警告を出す）。

### 囲み
- `> [!NOTE]` と `> [!IMPORTANT]` を、引用の 1 行目に書く。見た目を用意しているのは、この 2 つ。見出しの言葉は「NOTE」「IMPORTANT」のまま出る。

  ```markdown
  > [!NOTE]
  > 補足の文。
  ```

### 番号付きリスト
- 1 以外の番号で始まる番号付きリストを、文の行の直後に置くときは、リストの前に空行を 1 つ入れる。空行が無いと、リストにならず、前の行とつながった 1 つの段落になる。

### 太字
- 日本語の約物に接した太字（`**「強調」**の` など）も、そのまま太字になる。

### リンクと画像
- サイト内のリンクと画像は、公開されるパス（サイトの先頭からの絶対パス。`/img/…`、`/doc/…`、`/2020/08/10/emo.html`）で書く。リポジトリでの置き場（`site/`、`public/`、`src/content/`）は付けない。
- 別リポジトリのページ（`/ghost_dev/…`、`/nyan-pu-puu/…`、`/zzz-pasta/…`、`/old/…`）も同じ書き方で指す。これらはこのリポジトリには無く、手元の表示では出ない。
- 先が `dist/` に無いリンクと画像は、`npm run verify` が警告を出す（別リポジトリのページは除く）。ビルドは止めない。

### 画像の飾り
- 本文の画像には、既定で枠と影が付く。
- 飾りを外したい画像は、HTML で書いて `class="plain"` を付ける（`<img src="/img/…" alt="…" class="plain">`）。
- SVG の画像と、HTML で `align` か `style` を指定した画像は、自動で飾りが外れる。

### 非公開
- `published: false` を書くと、そのページを出さず、一覧とフィードにも載せない。下書きの置き場は無い。書きかけは、この印を付けて置く。
- About は非公開にできない（`/about.html` は保つパスで、メニューも指すため。`published: false` を書くとビルドが止まる）。

## Code Organization Principles

- 公開物と開発用の文書を混ぜない。公開するものは `site/src/content/` か `site/public/` へ、リポジトリの開発用のファイルは `site/` の外へ置く。
- 見た目を直すときは、`site/src/styles/`・`site/src/components/`・`site/src/layouts/` を直す。原稿は見た目を知らない。
- 記事の URL は `site/src/lib/post-path.ts` だけが決める。ルート・一覧・フィード・前後の記事は、ここが返すパスを使う。

---
_Document patterns, not file trees. New files following patterns shouldn't require updates_
