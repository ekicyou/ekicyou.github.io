---
inclusion: always
updated_at: 2026-10-11
---

# Technology Stack

## Architecture

Jekyll による静的サイト。原稿は `site/` にあり、GitHub Actions のワークフロー（`.github/workflows/pages.yml`）がビルドして GitHub Pages へ公開する。サーバー側の処理やデータベースは無い。

- リポジトリ: `ekicyou/ekicyou.github.io`（ユーザーサイト。公開 URL はリポジトリ名で決まる）
- 公開方式の設定: GitHub Actions（`build_type: workflow`）。
- ワークフローは 1 ファイル・2 ジョブ。`build` が `site/` を Jekyll でビルドし、成果物 `github-pages` を保存する。`deploy` がその成果物を、環境 `github-pages` を通して公開する。
- `master` への push と、`master` での手動の実行は、`build` → `deploy` と進む。`master` 向けの PR は `build` だけで、`deploy` は動かない（「skipped」になる）。
- `build` が失敗すると `deploy` は動かず、前の公開物がそのまま残る。公開の途中で次の push が届いても、進行中の公開は打ち切らない。
- 権限はジョブごとに決めている。`build` は読み取りだけ、書き込み（`pages: write`、`id-token: write`）は `deploy` だけ。環境 `github-pages` は `master` しか受け付けない。

## Core Technologies

- **生成ツール**: Jekyll 3.10.0（`actions/jekyll-build-pages@v1.0.13` の中の `github-pages` gem 232 が版を決める）。使えるプラグインも、この gem が許すものだけ。`site/Gemfile` はビルドに使われない。
- **記法**: Markdown（kramdown）、Liquid テンプレート、Sass
- **実行環境**: ビルドも公開も GitHub のランナー（`ubuntu-latest`）の上。手元に Ruby は入っておらず、手元のビルド環境は求めない

## Key Libraries

- `jekyll-feed`: RSS フィード
- `jekyll-seo-tag`: 説明文・OGP などのメタ情報（`site/_includes/head.html` の `{% seo %}`）
- `jekyll-remote-theme`: `site/_config.yml` が `pages-themes/minimal` を指定している。ただし見た目は手元の `site/_layouts/`・`site/_includes/`・`site/_sass/minima/`（minima の写し）が決めており、指定と実態が食い違っている
- MathJax: 記事の front matter で `use_math: true` のときだけ読み込む（`site/_includes/math.html`）
- giscus: 記事のコメント欄。`comments: false` で消せる。本番ビルドのときだけ出る（`site/_includes/giscus_comments.html`）

## Development Standards

### 文字と改行
- UTF-8、改行は LF、インデントは空白 2 つ（`.editorconfig`）。`.gitattributes` で LF に固定している。
- Markdown は行末の空白を消さない（改行の意味を持つため）。

### 記事の front matter
- 必須: `layout: post`、`title`
- 任意: `description`（一覧と SNS に出る説明）、`image`（SNS 用の画像）、`tags`、`comments`、`use_math`、`published: false`（非公開）

### 検証
- 手元ではビルドしない。PR を出すと `build` ジョブが走るので、マージの前にそこでビルドの成否を確かめる。手順の正本は `.kiro/steering/workflow.md` の「検証」節。

## Development Environment

### Required Tools
- git、GitHub CLI（`gh`）
- Node.js（`npx cc-sdd` の版上げにだけ使う）

### Common Commands
```bash
# 公開方式の設定を見る（workflow と出る）
gh api repos/ekicyou/ekicyou.github.io/pages --jq .build_type
# 今のブランチの PR で、ビルドの成否を見る
gh pr checks --watch
# master の直近の実行を見る
gh run list --workflow pages.yml --branch master --limit 1
# 実行が終わるまで待つ
gh run watch <実行の ID> --exit-status
# 実行の成果物を取り出す
gh run download <実行の ID> --name github-pages --dir <取り出し先>
# 失敗した実行をやり直す
gh run rerun <実行の ID>
```

## Key Technical Decisions

- **独自ドメインは持たない**。`ekicyou.github.io` のまま運用する（開発者の意向、2026-10-11）。
- **push で公開**の手触りを保つ。別の置き場（Cloudflare Pages、Netlify など）へは移さない。
- **ビルドと公開は GitHub の公式アクション 4 つだけで組む**（`actions/checkout`、`actions/jekyll-build-pages`、`actions/upload-pages-artifact`、`actions/deploy-pages`）。`jekyll-build-pages` は、切り替え前の旧来方式が中で動かしていたのと同じコンテナなので、出力が切り替え前と同じになる。Ruby を自分で入れる組み方はしない。
- **`jekyll-build-pages` は細かい版まで指定する**（`v1.0.13`）。Jekyll とプラグインの版、つまり出力を決める部品だからである。ほかの 3 つはメジャー版の指定（`@v7`、`@v5`）。
- **版上げは Dependabot に提案させ、取り込みは開発者が決める**。`.github/dependabot.yml` が月 1 回、アクションの新しい版を PR で提案する。自動マージは設定しない。`jekyll-build-pages` の版上げは出力が変わり得るので、Astro への乗り換え（`site-astro-redesign`）が済むまでは見送ってよい。 Dependabot の PR ではトークンが読み取り専用になる。`build` は読み取りの権限だけで動く見込みだが、最初の提案 PR でビルドが通ることを確かめる（未確認）。

## 戻し方

公開方式を、切り替え前（GitHub Pages の旧来方式 `legacy`。原稿はリポジトリのルート）へ戻す手順。**どの操作も公開中のサイトに効く。開発者の許しを得てから行う。**

| 状態 | 公開方式の設定 | `master` の原稿 | 戻し方 |
|---|---|---|---|
| S0 切り替え前 | `legacy` | ルート | — |
| S1 設定だけ切り替えた | `workflow` | ルート（ワークフロー無し） | R1 |
| S2 今 | `workflow` | `site/` | R2 |

- **R2（S2 から）**: まず「前へ直す」（原因を直す PR）を考える。ビルドや公開に失敗している間は前の公開物が残るので、急いで戻す必要は薄い。どうしても S0 へ戻すなら、原稿を `site/` へ移した squash のコミットを打ち消す PR をマージし（原稿がルートへ戻り、ワークフローも消える。S1 になる）、その後で R1 を行う。移動の後に `site/` へ足した原稿は、打ち消しだけでは戻り切らない（衝突で止まるか、`site/` に残る）。同じ PR でルートへ移し、`git ls-files site` が空になったことを確かめる。
- **R1（S1 → S0）**: 設定を旧来方式へ戻し、ビルドを求める。原稿がルートにあれば、これだけで元に戻る。

  ```bash
  gh api -X PUT repos/ekicyou/ekicyou.github.io/pages -f build_type=legacy -f "source[branch]=master" -f "source[path]=/"
  gh api -X POST repos/ekicyou/ekicyou.github.io/pages/builds
  ```

  この 2 つのコマンドは試していない（未確認）。通らなければ、画面（Settings → Pages → Source → Deploy from a branch、`master` の `/`）で戻す。
- **順番を守る**: 原稿が `site/` にあるうちに設定を `legacy` にすると、旧来ビルドはリポジトリのルートを原稿として扱い、全ページが `/site/...` の下に公開される。R2 で打ち消しのマージより先に R1 を行うのも、設定が `legacy` のうちに原稿を `site/` へ移す変更を `master` へ入れるのも、同じ結果になる。

---
_Document standards and patterns, not every dependency_
