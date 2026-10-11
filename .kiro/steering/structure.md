---
inclusion: always
updated_at: 2026-10-11
---

# Project Structure

## Organization Philosophy

Jekyll の決まりどおりの配置。**今はリポジトリのルートがそのままサイトの原稿**であり、ルートに置いたファイルは除外しない限り公開される。開発用の文書（`CLAUDE.md`、`.kiro/`、`.claude/`）は公開物ではない。

## Directory Patterns

### 記事
**Location**: `/_posts/`
**Purpose**: 公開する記事。ファイル名は `YYYY-MM-DD-<英数字の名前>.md`。日付と名前が URL（`/YYYY/MM/DD/<名前>.html`）になるので、公開後は変えない。
**Example**: `_posts/2026-03-26-emo2.md` → `/2026/03/26/emo2.html`

`_posts/note/` は技術メモの置き場（今は非公開の記事だけ）。サブフォルダ名はカテゴリとして URL に入る。

### 下書き
**Location**: `/_drafts/`
**Purpose**: 公開しない書きかけ。日付なしのファイル名で置く。

### 記事に添える文書
**Location**: `/doc/<題材>/`
**Purpose**: 記事から参照する長文、PDF、TeX の原稿、その題材の画像。Markdown は HTML に変換されて公開される。
**Example**: `doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.md` → `/doc/cosmogenesis/entanglement_cosmogenesis_v1_ja.html`

### 画像
**Location**: `/img/`
**Purpose**: サイト全体で使う画像と、記事の SNS 用画像。記事用は `img/<記事の日付>-<名前>.png` に揃える。

### レイアウトと部品
**Location**: `/_layouts/`、`/_includes/`、`/_sass/`、`/assets/`
**Purpose**: ページの枠（`default` → `home` / `page` / `post`）、差し込む部品（ヘッダー、フッター、コメント欄、数式）、スタイル。minima テーマを手元に写して手を入れたもの。

### 単独のページ
**Location**: ルート直下（`index.md`、`about.md`、`404.html`）
**Purpose**: 記事ではない固定のページ。front matter に `title` を持つページは、ヘッダーのメニューに自動で並ぶ。

### 開発用（公開しない）
**Location**: `/.kiro/`、`/.claude/`、`/CLAUDE.md`
**Purpose**: 仕様駆動開発の文書とスキル。ドット始まりのフォルダは Jekyll が無視する。ルート直下の `CLAUDE.md` は `_config.yml` の `exclude` で外している。

## Naming Conventions

- **記事**: `YYYY-MM-DD-<小文字の英数字とアンダースコア>.md`
- **spec**: 小文字のハイフン区切り（例: `site-subfolder-actions-build`）。区分や番号は付けない
- **コミット**: `<type>(<scope>): <日本語の要約>`（例: `feat(posts): …`、`fix(core): …`）

## リンクの書き方

- サイト内のリンクと画像は、ルートからの絶対パス（`/img/…`、`/doc/…`、`/2020/08/10/emo.html`）で書く。
- 別リポジトリのページ（`/ghost_dev/…`、`/nyan-pu-puu/…`、`/zzz-pasta/…`、`/old/…`）も同じ書き方で指す。これらはこのリポジトリには無い。

## Code Organization Principles

- 公開物と開発用の文書を混ぜない。ルートに開発用のファイルを足すときは、公開されないことを確かめる。
- テーマの部品を直すときは、手元の `_layouts/`・`_includes/`・`_sass/` を直す（リモートテーマ側ではない）。

---
_Document patterns, not file trees. New files following patterns shouldn't require updates_
