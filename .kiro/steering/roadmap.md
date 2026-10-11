---
inclusion: always
updated_at: 2026-10-11
---

# Roadmap

## Overview

「えちょのざんがい」(`ekicyou/ekicyou.github.io`) を、Claude が手伝いやすく、PR の上でビルドを確かめられるリポジトリに作り替える。公開先は GitHub Pages のまま、URL `https://ekicyou.github.io/` と公開済みの記事・文書・画像のパスは変えない。

進め方は 2 段。まず Jekyll のまま、見た目を一切変えずに、原稿を `site/` へ 1 階層下げ、ビルドと公開を GitHub Actions に切り替える。その後で、生成ツールを Jekyll から Astro へ乗り換え、デザインを一から作り直す。公開方式の切り替えと、生成ツール・デザインの入れ替えを分けるのは、切り替えの日に変わるものを 1 つにするためである。

## Approach Decision

- **Chosen**: GitHub Pages + GitHub Actions のまま、2 段で進める。
  1. 1 本目は Jekyll のまま。公式アクション `actions/jekyll-build-pages`(切り替え前の旧来ビルドと同じコンテナ)に `source: ./site` を渡す。
  2. 2 本目で Astro に乗り換え、レイアウトとスタイルを一から作る。
- **Why**:
  - 開発者の条件(2026-10-11)は 4 つ。デザイン変更の自由度が高いこと。技術文書サイトなので Markdown を持って行ければよいこと(記事は少ない)。フィードは形式が変わってよいこと(たぶん誰も見ていない)。コメントは付けられたほうがよいこと。Astro はどれも満たす。
  - 記事と `doc/` の Markdown に Jekyll 専用の書き方(`{% %}`、`{: }`)は 1 つも無い。Jekyll に縛られているのは見た目の部品(`_layouts/`・`_includes/`・`_sass/`)だけで、そこは一から作り直すので捨てる部分である。
  - 手元に Node.js は入っているが Ruby は無い。Astro なら手元で `npm run dev` を動かし、画面を見ながら直せる。
  - 1 本目に公式アクションを使うのは、切り替え前の旧来ビルドと同じコンテナ(v1.0.13、`github-pages` gem 232)で出力が確実に同じになり、Ruby への投資が要らないため。Jekyll の原稿は 2 本目で捨てるので、1 本目は小さく済ませる。
- **Rejected alternatives**:
  - Jekyll に留まり、`ruby/setup-ruby` で Jekyll 4 へ進む案(最初の起票ではこれを選んだ)。手元で見るのに Ruby か Docker が要り、新しい作りへの道具立てが弱い。
  - Eleventy。Jekyll に最も近く堅いが、部品ごとに HTML と CSS をまとめる仕組みが無く、一から作るデザインでは Astro のほうが扱いやすい。次点。
  - Hugo。テンプレートの書き方に癖があり、凝るほど読みにくくなる。
  - Zola。`….html` 形式の URL を素直に出せず、URL を変えない方針に合わない。
  - Astro の文書サイト用テーマ Starlight。見た目の枠が決まっており、一から作る希望と合わない。
  - Jekyll に触らず、Astro 版の完成と同時に公開方式ごと 1 度に切り替える案。手数は減るが、切り替えの日に変わるものが 2 つ重なる。
  - 独自ドメイン、Cloudflare Pages / Netlify などへの移転。開発者が望んでいない。

## Scope

- **In**: 原稿の `site/` への移動、Actions によるビルドと公開、PR 上のビルド確認、Pages 設定の切り替え、遺物の片付け(`.travis.yml`)、アクションと npm パッケージの版上げの提案(Dependabot)、Astro への乗り換え、デザインの作り直し、それらに伴う steering・`CLAUDE.md` の更新。
- **Out**: 独自ドメイン、公開先の移転、ブランチ保護の導入、記事の内容の書き換え、別リポジトリのページ(`/ghost_dev/`、`/nyan-pu-puu/`、`/zzz-pasta/`、`/old/`)。

## Constraints

- URL `https://ekicyou.github.io/` と、公開済みの記事・文書・画像のパスを変えない。フィードは場所(`/feed.xml`)だけ保てばよく、形式は変わってよい。
- 「push すれば公開される」手触りを保つ。記事の追加・修正は `master` へ直接 push してよい(ブランチ保護は掛けない)。
- 個人の趣味サイト。運営の手間と費用を増やさない。長く放置しても壊れないこと(版を固定する)。
- 手元にあるのは Node.js と Docker。Ruby は無い。1 本目は Ruby 無しで完了できること。
- 1 spec = 1 ブランチ = 1 PR(`workflow.md`)。

## Boundary Strategy

- **Why this split**: 1 本目は「出力を変えない引っ越し」で、完了は旧来ビルドとの突き合わせで確かめる。2 本目は「出力を作り直す乗り換え」で、完了は公開されるパスの一覧と画面で確かめる。確かめ方が違うので分ける。1 本目が済めば、2 本目は PR 上のビルドを使って進められ、デザインの相談が長引いてもリポジトリは片付いた状態でいられる。
- **Shared seams to watch**:
  - `site/` の中身: 1 本目は Jekyll の原稿を移すだけ。2 本目で Astro のプロジェクトに入れ替える。記事の Markdown・`doc/`・`img/` は 2 本目でも引き継ぐ。
  - `.github/workflows/` のビルド手順: 1 本目が作る(Jekyll)。2 本目でビルドの段を Node.js に差し替える。公開の段と、PR ではビルドだけという振り分けは引き継ぐ。
  - `.github/dependabot.yml`: 1 本目はアクションだけ。2 本目で npm を足す。
  - `workflow.md` の「検証」節: 1 本目で「PR 上の CI が緑」に書き換える。2 本目で手元での確かめ方を足す。
  - steering の `tech.md`・`structure.md`: 2 本とも書き換える。

## Direct Implementation Candidates

spec を起こすほどでもない手直し。1 本目(`site-subfolder-actions-build`)の完了の棚卸しで見つけた。

- [ ] merged-branch-cleanup -- マージ済みの PR #2 のブランチ `claude/site-migration-prep-a289c7` がリモートに残っている。消してよいかを開発者に確かめて消す。あわせて、`kiro-complete` スキルの「リモートブランチは API で削除済み」という一文が正しいかを確かめ、違っていれば直す(リポジトリの設定は `delete_branch_on_merge: false`。1 本目の PR #3 のマージの後にブランチが残ったかどうかが手掛かりになる)。

## Specs (dependency order)

- [x] site-subfolder-actions-build -- Jekyll のまま原稿を `site/` へ 1 階層下げ、ビルドと公開を GitHub Actions に切り替える(見た目は変えない)。Dependencies: none
- [ ] site-astro-redesign -- 生成ツールを Astro へ乗り換え、デザインを一から作り直す(URL は変えない)。Dependencies: site-subfolder-actions-build
