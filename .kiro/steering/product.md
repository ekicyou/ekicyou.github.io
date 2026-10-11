---
inclusion: always
updated_at: 2026-10-11
---

# Product Overview

「えちょのざんがい」は、えちょ（ekicyou）の個人ブログ兼、開発物の置き場である。`https://ekicyou.github.io/` で公開している。サイトの説明は「えちょの嗜好についての諸々を github が続く限り残し続ける場所」。

読者は主に伺か（デスクトップマスコット）界隈の人と、作者本人の未来の自分である。

## Core Capabilities

- **記事の公開**: Markdown で書いた記事を日付付きで残す。数式（MathJax）、コード、画像、コメント欄（giscus）を使える。
- **文書の保管**: 記事に添える長文や PDF、TeX の原稿を `site/doc/` に置き、タイムスタンプ付きの記録として残す。
- **開発物への入口**: About ページから、別リポジトリで公開している配布物（ゴースト、過去作）へ案内する。
- **フィードと SNS 向けの情報**: RSS フィードと、記事ごとの説明文・画像（OGP）を出す。

## Target Use Cases

- 新しい配布物を出したときに、紹介記事を 1 本書いて push する。
- 考えたことを、後から「いつ書いたか」を示せる形で置いておく。
- 古い記事を、URL を変えずに残し続ける。

## Value Proposition

- **残ること**が最優先。記事の URL（パーマリンク）と `https://ekicyou.github.io/` というアドレスは変えない。
- **手間をかけない**。push すれば公開される。独自ドメインや有料の仕組みは持たない。
- 個人の趣味サイトであり、凝った機能より壊れにくさを取る。

## 守ること

- 公開済みの URL を壊さない。構成を変えるときは、変える前と後で公開されるパスが同じであることを確かめる。
- 同じ `ekicyou.github.io` の下で、別リポジトリのページも公開されている（`/ghost_dev/`、`/nyan-pu-puu/`、`/zzz-pasta/`、`/old/` など）。このリポジトリに同じ名前のトップ階層を作らない。
- 既定ブランチへのマージはそのまま公開になる。下書きは `site/_drafts/` か `published: false` で隠す。

---
_Focus on patterns and purpose, not exhaustive feature lists_
