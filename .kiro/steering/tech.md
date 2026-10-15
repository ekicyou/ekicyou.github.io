---
inclusion: always
updated_at: 2026-10-15
---

# Technology Stack

## Architecture

Astro による静的サイト。原稿と仕組みは `site/`（Astro のプロジェクト）にあり、GitHub Actions のワークフロー（`.github/workflows/pages.yml`）がビルドして GitHub Pages へ公開する。サーバー側の処理やデータベースは無い。閲覧側のスクリプトは、コードのコピーのボタンと giscus の 2 つだけである。

- 経緯: 2026-10 に、生成ツールを Jekyll から Astro へ替えた（spec `site-astro-redesign`）。公開済みの URL は変えていない。
- リポジトリ: `ekicyou/ekicyou.github.io`（ユーザーサイト。公開 URL はリポジトリ名で決まる）
- 公開方式の設定: GitHub Actions（`build_type: workflow`）。
- ワークフローは 1 ファイル・2 ジョブ。`build` が `site/` を Astro でビルドし、成果物 `github-pages`（中身は `site/dist`）を保存する。`deploy` がその成果物を、環境 `github-pages` を通して公開する。
- `master` への push と、`master` での手動の実行は、`build` → `deploy` と進む。`master` 向けの PR と、`master` 以外での手動の実行は `build` だけで、`deploy` は動かない（「skipped」になる）。
- `build` が失敗すると `deploy` は動かず、前の公開物がそのまま残る。公開の途中で次の push が届いても、進行中の公開は打ち切らない。
- 権限はジョブごとに決めている。`build` は読み取り（`contents: read`）だけ、書き込み（`pages: write`、`id-token: write`）は `deploy` だけ。環境 `github-pages` は `master` しか受け付けない。

## Core Technologies

- **生成ツール**: Astro 7（`astro` 7.3.8）。出力はファイル単位（`build.format: 'file'`。`/about.html` の形）、末尾の `/` なし（`trailingSlash: 'never'`）、サイトの URL は `https://ekicyou.github.io`。`base` は置かない。統合（サイトマップなど）は入れない。設定は `site/astro.config.mjs`。
- **記法**: Markdown（従来の処理系 `unified()` を明示して選ぶ。GFM と見出しの id は既定のまま）、Astro の部品（`.astro`）、TypeScript、素の CSS
- **実行環境**: Node.js 26（主版だけを固定）と、Node.js に付いてくる npm。ビルドは GitHub のランナー（`ubuntu-latest`）でも、手元の Windows でも同じ手順で動く。Ruby も Docker も要らない

## Key Libraries

版は `site/package.json` に、範囲ではなくそのままの数字で書いてある。

- `@astrojs/markdown-remark`: 従来の Markdown の処理系。remark のプラグインは `remark-cjk-friendly` → `remark-math` → `remark-github-blockquote-alert` の順、rehype のプラグインは `rehypeNoMailto` → `rehype-katex` の順で渡す（順番を変えない）
  - `remark-cjk-friendly`: 日本語の約物に接した太字を、太字として読む
  - `remark-math` + `rehype-katex` + `katex`: `$…$` と `$$…$$` の数式を、ビルド時に KaTeX で描く。`\lambdabar` を記号として定義している。スタイルシートとフォントは `katex` から束ねて自分の公開物（`/_astro/`）として配り、数式のあるページだけが読む。フォントは `woff2` だけを出す（`astro.config.mjs` の小さな変換）
  - `remark-github-blockquote-alert`: `> [!NOTE]`・`> [!IMPORTANT]` を囲みにする。見た目は `site/src/styles/prose.css` が決める（付属のスタイルシートは使わない）
  - `rehypeNoMailto`（`astro.config.mjs` の中の部品）: 本文のメールアドレスが自動でリンク（`mailto:`）になるのを止める
- Shiki（`astro` に付いてくる）: コードの色付け。配色 `vitesse-light`・`vitesse-dark` を渡し、色を CSS の変数として出させる。知らない言語名は警告を出して、色なしで出す
- `@astrojs/rss`: RSS 2.0 のフィード（`/feed.xml`。`site/src/pages/feed.xml.ts`）
- `@astrojs/check` + `typescript`（開発用）: 型の検査（`astro check`）
- giscus: 記事と文書のコメント欄。設定は `site/src/consts.ts` の `SITE.giscus`、部品は `site/src/components/Comments.astro`。`comments: false` で消せる。手元の表示でも出る
- Google Fonts: 見出しの DotGothic16 と本文の M PLUS 1p を `<link>` で読む（`SITE.fontsHref`）。読めないときは OS のゴシック体になる
- 単体テストは Node.js の標準（`node --test`）、成果物の検査は Node.js の標準の機能だけで書いた `site/scripts/verify-dist.mjs`。どちらも依存を足さない

## Development Standards

### 文字と改行
- UTF-8、改行は LF、インデントは空白 2 つ（`.editorconfig`）。`.gitattributes` で LF に固定している。
- Markdown は行末の空白を消さない（改行の意味を持つため）。

### 記事の書き方
- ファイル名、front matter、数式、囲み、リンクと画像の決まりは、`.kiro/steering/structure.md` の「記事の書き方」にある。

### 検証
- マージの前に、`site/` で `npm run check`・`npm test`・`npm run build`・`npm run verify` を通す。PR を出すと、`build` ジョブが同じ段を走らせるので、そこでもビルドの成否を確かめる。手順の正本は `.kiro/steering/workflow.md` の「検証」節。
- `npm run verify` は、保つパスの一覧（`site/tests/kept-paths.txt`）がすべてあること、認めない形のパスやメールのリンクが無いこと、非公開の記事が出ていないことなどを検査し、誤りがあれば終了コード 1 で止まる。リンク切れと読めない数式は警告だけで、ビルドを止めない。
- 手元では、中身の変わらない原稿の検査をやり直さない（`site/node_modules/.astro/data-store.json` のキャッシュ）。冷えた状態で確かめたいときは、このファイルを消してからビルドする。CI は常に冷えた状態である。

## Development Environment

### Required Tools
- git、GitHub CLI（`gh`）
- Node.js 26（npm はこれに付いてくるもの。`npx cc-sdd` の版上げにも使う）。手元で確かめた版は 26.8.1

### 手元の表示
- `site/` で `npm ci` → `npm run dev`。ブラウザで `http://localhost:4321/` を開く。記事・文書・単独のページは、公開と同じパスで見える。原稿やスタイルを書き換えると、作り直しの操作なしに画面に映る。
- 環境によっては、手元の表示が `[::1]:4321` だけで待ち受けることがある（`127.0.0.1` では繋がらない）。`localhost` で開く。
- 手元の表示でもコメント欄（giscus）が出る。ここから投稿すると本番のページのコメントになるので、手元からは投稿しない。
- `npm run dev`・`npm run build`・`npm run check` を、`site/` で同時に 2 つ動かさない（作業フォルダ `.astro` と `dist` を取り合う）。
- Windows では、背景で動かした `npm run dev` を止めても、子の `node` が残ってポート 4321 を握ることがある。止めた後に、ポートが空いたことを確かめる。

### Common Commands
```bash
# --- site/ で実行する ---
# ロックファイルどおりに依存を入れる
npm ci
# 手元の表示（http://localhost:4321/）
npm run dev
# 型の検査
npm run check
# 単体テスト（node --test tests/）
npm test
# site/dist にビルドする
npm run build
# ビルドした結果を手元で見る
npm run preview
# 成果物の検査（node scripts/verify-dist.mjs）
npm run verify
# 公開されるパスの一覧を出す
node scripts/verify-dist.mjs --list
# 基準の一覧と比べ、消えるパスと増えるパスを出す
node scripts/verify-dist.mjs --baseline <一覧のファイル>

# --- どこでも ---
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

## ビルドの段

`pages.yml` の `build` ジョブは、次の順に進む。`run` の手順は `site/` で動く（`defaults.run.working-directory: site`）。どれかが落ちれば成果物を保存せず、公開もしない。

1. `actions/checkout@v7`
2. `actions/setup-node@v7`（`node-version-file: site/.node-version`、`package-manager-cache: false`）
3. `npm ci`（ロックファイルどおりに入れる）
4. `npm run check`（型の検査）
5. `npm test`（単体テスト）
6. `npm run build`（`site/dist` に出す）
7. `npm run verify`（成果物の検査）
8. `actions/upload-pages-artifact@v5`（`path: site/dist`。名前は既定の `github-pages`）

キャッシュは使わない（`npm ci` は短く済み、壊れ方を 1 つ減らせる）。ジョブには環境変数 `ASTRO_TELEMETRY_DISABLED: 1` を置いている。

## Key Technical Decisions

- **独自ドメインは持たない**。`ekicyou.github.io` のまま運用する（開発者の意向、2026-10-11）。
- **push で公開**の手触りを保つ。別の置き場（Cloudflare Pages、Netlify など）へは移さない。
- **ビルドと公開は GitHub の公式アクション 4 つだけで組む**（`actions/checkout`、`actions/setup-node`、`actions/upload-pages-artifact`、`actions/deploy-pages`）。ビルドの段は自前で並べる。サードパーティのアクション（`withastro/action` など）は使わない。
- **出力を決める部品は、細かい版まで固定する**。生成ツールとその依存の版は、`site/package.json` にそのままの数字で書き、`site/package-lock.json` を管理下に置く。CI は `npm ci` で、ロックファイルどおりに入れる。アクションはメジャー版の指定（`@v7`、`@v5`）。
- **Node.js は主版だけを固定する**（`site/.node-version` に `26`）。ワークフローはこのファイルを読み、その主版の中の版を用意する（ランナーに入っている版が先に使われる）。細かい版と安全上の直しは、何もしなくても入る。主版の版上げは提案を求めず、下の手順で見直す（開発者の判断、2026-10-12。「年に数回しか更新しないサイトなので、そのくらい緩くてよい」）。
- **版上げは Dependabot に提案させ、取り込みは開発者が決める**。`.github/dependabot.yml` が月 1 回、2 つを PR で提案する。アクションの新しい版（`github-actions`、置き場 `/`）と、npm の依存の新しい版（`npm`、置き場 `/site`）。npm の小さい版上げ（minor・patch）は組 `minor-and-patch` で 1 つの PR にまとめ、主版の版上げは 1 つずつの PR になる。自動マージは設定しない。取り込みは、PR のビルド（型の検査・単体テスト・ビルド・成果物の検査）を見て決める。
  - Dependabot が公式に扱う npm は 11 までで、ロックファイルは npm 12 で作った。最初の提案の PR で、ロックファイルが正しく直るかを見る（未確認）。
  - Dependabot の PR ではトークンが読み取り専用になる。`build` は読み取りの権限だけで動く見込みだが、最初の提案 PR でビルドが通ることを確かめる（未確認）。
- **利用状況の送信（Astro のテレメトリー）は止める**。送る理由が無い。CI はワークフローの環境変数 `ASTRO_TELEMETRY_DISABLED` で止めている。手元は PC ごとの設定なので、新しい PC で初めて `npm ci` をした後に、`site/` で 1 度だけ `npx astro telemetry disable` を実行する。

### Astro の主版を上げるとき（Node.js の主版の見直し）

Node.js の版上げは、Dependabot が提案できない（版のファイルに対応していない）。そこで、Astro の主版の版上げの PR を取り込むときに、あわせて見直す。

1. 新しい Astro が求める Node.js の版を確かめる（その PR のブランチで `npm ci` した後の `site/node_modules/astro/package.json` の `engines.node`。今の 7.3.8 は `>=22.12.0`）。
2. `site/.node-version` の主版が、その範囲に入り、Node.js の保守期間の中にあるかを見る。あわせて、単体テストと成果物の検査は、Node.js に組み込みの型の取り除きで `.ts` を読むので、22.18 以上が要る(Astro の求める版より高い)。外れていれば、同じ PR で `site/.node-version` を新しい主版に書き換える。
3. 手元の Node.js も同じ主版にそろえ、`site/` で `npm ci` → `npm run check` → `npm test` → `npm run build` → `npm run verify` を通す。
4. PR の `build` ジョブが通ることを見てから取り込む。

新しい Astro が今の Node.js では動かないときは、その PR のビルドが失敗するので気付ける。

## 戻し方

公開を前の状態へ戻す手順。**どの操作も公開中のサイトに効く。開発者の許しを得てから行う。**

| 状態 | 公開方式の設定 | `master` の原稿 | 生成ツール | 戻し方 |
|---|---|---|---|---|
| S0 公開方式の切り替え前 | `legacy` | ルート | Jekyll（旧来方式） | — |
| S1 設定だけ切り替えた | `workflow` | ルート（ワークフロー無し） | —（ビルドされない） | R1 |
| S2 生成ツールの切り替え前 | `workflow` | `site/` | Jekyll | R2 |
| S3 今 | `workflow` | `site/` | Astro | R3 |

- **R3（S3 → S2）**: 生成ツールの切り替え（spec `site-astro-redesign`）は、公開方式の設定を変えていない。戻すのはリポジトリの中身だけである。まず「前へ直す」（原因を直す PR）を考える。ビルドが失敗している間は、前の公開物が残るので、急いで戻す必要は薄い。見た目が崩れたまま公開されてしまったときは、その spec の squash のコミットを打ち消す PR をマージする（原稿が切り替え前の置き場へ戻り、ワークフローも切り替え前の Jekyll のビルドへ戻る）。切り替えの後に足した記事は、打ち消しだけでは戻らないので、同じ PR で切り替え前の記事の置き場（`site/_posts/`）へ移す。
- **R2（S2 から）**: S3 から旧来方式へ戻すときは、先に R3 を行う。まず「前へ直す」（原因を直す PR）を考える。ビルドや公開に失敗している間は前の公開物が残るので、急いで戻す必要は薄い。どうしても S0 へ戻すなら、原稿を `site/` へ移した squash のコミットを打ち消す PR をマージし（原稿がルートへ戻り、ワークフローも消える。S1 になる）、その後で R1 を行う。移動の後に `site/` へ足した原稿は、打ち消しだけでは戻り切らない（衝突で止まるか、`site/` に残る）。同じ PR でルートへ移し、`git ls-files site` が空になったことを確かめる。
- **R1（S1 → S0）**: 設定を旧来方式へ戻し、ビルドを求める。原稿がルートにあれば、これだけで元に戻る。

  ```bash
  gh api -X PUT repos/ekicyou/ekicyou.github.io/pages -f build_type=legacy -f "source[branch]=master" -f "source[path]=/"
  gh api -X POST repos/ekicyou/ekicyou.github.io/pages/builds
  ```

  この 2 つのコマンドは試していない（未確認）。通らなければ、画面（Settings → Pages → Source → Deploy from a branch、`master` の `/`）で戻す。
- **順番を守る**: 原稿が `site/` にあるうちに設定を `legacy` にすると、旧来ビルドはリポジトリのルートを原稿として扱い、全ページが `/site/...` の下に公開される。R2 で打ち消しのマージより先に R1 を行うのも、設定が `legacy` のうちに原稿を `site/` へ移す変更を `master` へ入れるのも、同じ結果になる。

---
_Document standards and patterns, not every dependency_
