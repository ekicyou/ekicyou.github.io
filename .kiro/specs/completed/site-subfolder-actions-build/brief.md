# Brief: site-subfolder-actions-build

## Problem

開発者(えちょ)は、このリポジトリを Claude が手伝いやすい形に作り替えたい。今は次の点で手伝いにくい。

- リポジトリのルートがそのままサイトの原稿なので、開発用の文書(`CLAUDE.md`、道具、ワークフロー)を置くたびに「公開物に混ざらないか」を気にする必要がある。
- ビルドは GitHub Pages の旧来方式に任せきりで、PR の上では何も確かめられない。ビルドが通るかどうかは `master` へマージして初めて分かる。

## Current State

- 公開: GitHub Pages、`build_type: legacy`、原稿は `master` のルート。`https` 強制、独自ドメイン無し。
- 旧来ビルドの実体は、GitHub が自動で走らせる「pages build and deployment」。中で `ghcr.io/actions/jekyll-build-pages:v1.0.13`(`github-pages` gem 232、Jekyll 3.10.0、Ruby 3.3、`JEKYLL_ENV=production`、safe モード)を使っている。
- `Gemfile` は `gem "github-pages", group: :jekyll_plugins` の 1 行だけ。`Gemfile.lock` は `.gitignore` で外している。
- `.github/` は無い。`.travis.yml`(Ruby 2.4 指定)は働いていない遺物。
- 環境 `github-pages` は、公開を許すブランチが `master` だけ。Actions は有効で、使えるアクションの制限は無い。
- 手元に Ruby は無い。
- 追跡している原稿は約 60 ファイル(記事 15 本、`doc/`、`img/`、`_layouts/`・`_includes/`・`_sass/`、`index.md`・`about.md`・`404.html`)。
- テンプレートは `jekyll.environment == "production"` のときだけコメント欄(giscus)を出す。`_config.yml` に `url` は無く、`absolute_url` と `{% seo %}` は `github-pages` gem が入れる値に頼っている。

## Desired Outcome

- 原稿一式が `site/` の下にあり、ルートには開発用のファイルだけが残る。
- `master` への push で GitHub Actions がビルドし、公開する。「push すれば公開される」は今と同じ。
- PR を出すとビルドだけが走り、通るかどうかがマージの前に分かる。
- 公開されるページは、切り替えの前と同じである(下の「意図した差」を除く)。
- `workflow.md` の「検証」節が「PR 上の CI が緑であること」に書き換わっている。

## Approach

公式アクション `actions/jekyll-build-pages` に `source: ./site` を渡してビルドし、`actions/upload-pages-artifact` → `actions/deploy-pages` で公開する。Ruby も `Gemfile.lock` も足さない。

選んだ理由は `.kiro/steering/roadmap.md` の「Approach Decision」。要点は 2 つ。今の旧来ビルドが既にこのアクションのコンテナ(v1.0.13)で動いているので、変わるのは原稿の置き場だけで、出力が同じになること。Jekyll の原稿は次の spec(`site-astro-redesign`)で Astro に入れ替えるので、Jekyll のビルド環境には投資しないこと。

設計で詰める材料(2026-10-11 の下調べ。出典は各アクションのリポジトリと docs.github.com):

- **アクションの中身**: `github-pages` gem 232(Jekyll 3.10.0)、Ruby 3.3。`JEKYLL_ENV=production`、`PAGES_REPO_NWO`、`JEKYLL_GITHUB_TOKEN` を自分で設定し、safe モードで `github-pages build` を走らせる。`_config.yml` は `source` に渡したフォルダから読む。`Gemfile` は使わない(中身を確かめて警告を出すだけ)。
- **`site.url` の決まり方**: ビルド中に Pages の API を読んで決める。トークンが Pages を読めないと `absolute_url` や SNS 向けの情報が変わる恐れがあるので、ビルドのジョブに `pages: read` を付ける。
- **権限**: ビルドのジョブは `contents: read` と `pages: read`。公開のジョブだけ `pages: write` と `id-token: write`。
- **PR ではビルドだけ**: 環境 `github-pages` は `master` しか受け付けない。公開のジョブは `master` への push のときだけ動かし、ビルドのジョブには環境を付けない。PR の実行が本番の公開を待たせたり取り消したりしないよう、`concurrency` を分ける。
- **`Gemfile`**: ビルドには使われない。`site/` へ一緒に移すか、消すかは設計で決める。
- **アクションの版(2026-10-11 時点)**: `actions/checkout` v7、`actions/configure-pages` v6(無くても動く)、`actions/jekyll-build-pages` v1.0.13、`actions/upload-pages-artifact` v5、`actions/deploy-pages` v5。`upload-pages-artifact` は v4 からドット始まりのファイルを含めない(このサイトには該当なし)。`jekyll-build-pages` は 2024-08 から新しい版が出ていないが、保管庫入り(archived)にはなっていない。
- **切り替えの順番(最重要)**: Pages 設定を「GitHub Actions」(`build_type: workflow`)へ切り替えるのは、**必ずマージの前**。旧来方式のまま `site/` 移動が `master` に入ると、旧来ビルドがルートを原稿として扱い、全ページが `/site/...` の下に公開される。切り替えは画面(Settings → Pages → Source)か `gh api -X PUT repos/ekicyou/ekicyou.github.io/pages -f build_type=workflow`。`gh` のトークンはリポジトリの admin 権限と `repo`・`workflow` のスコープを持つので API で足りる見込みだが、試していない(未確認)。公開中のサイトに効く操作なので、開発者の許しを得てから行う。
- **公開の成否の見方**: 今は `gh api repos/ekicyou/ekicyou.github.io/pages/builds/latest` で見ている。切り替えた後は Actions の実行結果を見ることになる見込み(未確認)。`workflow.md` の「検証」節と「完了時のアクション」を書き換えるときに確かめる。
- **切り替えてから最初の公開までの間**、今の公開物が残り続けるかは文書で確かめられていない(残る見込み)。
- **戻し方**: Actions 方式のままなら `git revert` して push。旧来方式へ戻すなら、先に原稿をルートへ戻してから `build_type=legacy` にする。
- **出力の突き合わせ**: 旧来ビルドの成果物(「pages build and deployment」の `github-pages` アーティファクト、保存は 1 日)か、公開中のサイトを基準にして、新しいビルドの成果物とファイルの一覧・中身を比べる。

## Scope

- **In**:
  - 原稿一式を `site/` へ移す(`git mv`。中身は変えない)
  - `.github/workflows/` にビルドと公開のワークフローを足す(PR ではビルドだけ)
  - Pages 設定の `build_type` を `workflow` へ切り替える手順と、その実施
  - 切り替えの前後で、公開されるパスと中身が同じであることを確かめる
  - `.travis.yml` を消す
  - `.github/dependabot.yml` を足す(アクションの版上げを PR で提案させる)
  - `.gitignore`・`.editorconfig`・`.gitattributes`・`_config.yml` の `exclude` を、新しい配置に合わせて直す(`CLAUDE.md` は原稿の外に出るので、`exclude` から外せる)
  - steering(`tech.md`・`structure.md`・`workflow.md` の「検証」節)と `CLAUDE.md`・`README.md` を、新しい配置とビルド方式に合わせて直す
- **Out**:
  - 見た目・レイアウト・スタイルの変更
  - テーマ指定(`remote_theme`)の食い違いの解消(Astro への乗り換えで Jekyll のテーマごと無くなる)
  - Jekyll の版上げ、Ruby を入れる組み方への変更
  - 生成ツールの乗り換え(Astro)
  - 手元でビルドする環境づくり
  - ブランチ保護の導入
  - 記事の内容の書き換え

## Boundary Candidates

- 原稿の移動(`site/` への `git mv` と、配置に依存する設定の直し)
- ビルドと公開のワークフロー(`.github/workflows/`、権限、PR と `master` の振り分け)
- 切り替えの段取り(Pages 設定の切り替え、順番、戻し方)
- 出力の突き合わせ(切り替えの前後で公開物が同じことの確かめ)
- 片付けと文書の更新(`.travis.yml`、Dependabot、steering、`CLAUDE.md`)

## Out of Boundary

- デザイン、テーマ、生成ツールに関わること全般。`site-astro-redesign` が持つ。
- `site/_config.yml` のテーマ指定・プラグインの中身。この spec は場所を移すだけで、中身は変えない(`exclude` の直しを除く)。
- 記事を書くときの運用(`master` へ直接 push してよい、など)。今のまま。

## Upstream / Downstream

- **Upstream**: GitHub Pages、GitHub Actions、`github-pages` gem 232、`.kiro/steering/` の 4 枚(とくに `product.md` の「守ること」、`workflow.md` の「検証」節)。
- **Downstream**: `site-astro-redesign`(`site/` という置き場、Actions 方式の公開、PR ではビルドだけという振り分けを前提にし、ビルドの段を Node.js に差し替える)。`/kiro-complete` のステップ 5(「検証」節を読む)。

## Existing Spec Touchpoints

- **Extends**: なし(このリポジトリで最初の spec)。
- **Adjacent**: `site-astro-redesign`。`site/` の中身の入れ替え、見た目の部品(`_layouts/`・`_includes/`・`_sass/`)の作り直しはあちらの持ち場。ワークフローは、ビルドの段だけを差し替えやすい形にしておく。

## Constraints

- URL `https://ekicyou.github.io/` と、公開済みのパスを変えない。確かめる段を要件に入れる(前のセッションからの申し送り)。
- **意図した差**として見込むもの: いまルートの `README.md` が `/README.md` としてそのまま公開されている。`site/` へ移さなければ公開から外れる。外してよいかは要件で開発者に確かめる。
- 同じ `ekicyou.github.io` の下で別リポジトリのページも公開されている。このリポジトリに同じ名前のトップ階層を作らない。
- Ruby 無しで完了できること。確かめは PR 上のビルドと成果物で行う。
- Pages 設定の切り替えと `master` へのマージは公開中のサイトに効く。マージの前に、開発者へ 1 度だけ確かめる。
- 改行は LF。既存のファイルを `sed -i` で書き換えない。
- origin の URL にユーザー名が含まれている(トークンは入っていない様子)。出力を報告へ写すときは伏せる。ハーネスの `sync_with_base_branch` はこの URL を理由に断るので、`master` の取り込みは `git merge` で行う。
- `core.autocrlf=true` のため、既存のファイルは作業ツリーが CRLF、索引が LF。コミット時に改行の警告が大量に出るが無害。`git mv` の後、差分が全行にならないことを確かめる。
- 移植した `kiro-start`・`kiro-design`・`kiro-complete` などは、この spec が最初の実走になる。手順におかしな所があれば、この spec の中で直すか `/kiro-discovery` で起票する。
- 「PRしてスクワッシュマージ」と言われたら、push・PR 作成・squash マージまでを 1 回で進める。
