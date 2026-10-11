---
inclusion: always
updated_at: 2026-10-11
---

# Technology Stack

## Architecture

Jekyll による静的サイト。GitHub Pages が既定ブランチ `master` への push を受けてビルドし、公開する。サーバー側の処理やデータベースは無い。

- リポジトリ: `ekicyou/ekicyou.github.io`（ユーザーサイト。公開 URL はリポジトリ名で決まる）
- ビルド方式: GitHub Pages の旧来方式（`build_type: legacy`、原稿の置き場はリポジトリのルート）。GitHub 側が決めた Jekyll の版と、許可済みのプラグインだけが使える。

## Core Technologies

- **生成ツール**: Jekyll（`github-pages` gem が版を決める）
- **記法**: Markdown（kramdown）、Liquid テンプレート、Sass
- **実行環境**: ビルドは GitHub 側。手元に Ruby は入っていない

## Key Libraries

- `jekyll-feed`: RSS フィード
- `jekyll-seo-tag`: 説明文・OGP などのメタ情報（`_includes/head.html` の `{% seo %}`）
- `jekyll-remote-theme`: `_config.yml` が `pages-themes/minimal` を指定している。ただし見た目は手元の `_layouts/`・`_includes/`・`_sass/minima/`（minima の写し）が決めており、指定と実態が食い違っている
- MathJax: 記事の front matter で `use_math: true` のときだけ読み込む（`_includes/math.html`）
- giscus: 記事のコメント欄。`comments: false` で消せる。本番ビルドのときだけ出る（`_includes/giscus_comments.html`）

## Development Standards

### 文字と改行
- UTF-8、改行は LF、インデントは空白 2 つ（`.editorconfig`）。`.gitattributes` で LF に固定している。
- Markdown は行末の空白を消さない（改行の意味を持つため）。

### 記事の front matter
- 必須: `layout: post`、`title`
- 任意: `description`（一覧と SNS に出る説明）、`image`（SNS 用の画像）、`tags`、`comments`、`use_math`、`published: false`（非公開）

### 検証
- 手元ではビルドできない（Ruby が無い）。確認の正本は `.kiro/steering/workflow.md` の「検証」節。

## Development Environment

### Required Tools
- git、GitHub CLI（`gh`）
- Node.js（`npx cc-sdd` の版上げにだけ使う）

### Common Commands
```bash
# 公開の状態を見る
gh api repos/ekicyou/ekicyou.github.io/pages
# 直近のビルドを見る
gh api repos/ekicyou/ekicyou.github.io/pages/builds/latest
```

## Key Technical Decisions

- **独自ドメインは持たない**。`ekicyou.github.io` のまま運用する（開発者の意向、2026-10-11）。
- **push で公開**の手触りを保つ。別の置き場（Cloudflare Pages、Netlify など）へは移さない。
- `.travis.yml` は Ruby 2.4 指定の古い遺物で、今は働いていない。

---
_Document standards and patterns, not every dependency_
