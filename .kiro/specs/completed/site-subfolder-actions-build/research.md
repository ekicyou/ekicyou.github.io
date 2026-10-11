# Research & Design Decisions

## Summary

- **Feature**: `site-subfolder-actions-build`
- **Discovery Scope**: Extension(既存サイトの配置換えと、ビルド・公開方式の切り替え。軽い調査)
- **Key Findings**:
  - 今の旧来ビルドは、既に `actions/jekyll-build-pages` v1.0.13 のコンテナで動いている。同じアクションに `source: ./site` を渡せば、変わるのは原稿の置き場だけになる。
  - 置き場に依存する設定は `_config.yml` の `exclude` の 1 行(`CLAUDE.md`)だけ。`.gitignore`・`.editorconfig`・`.gitattributes` はパスを含まず、直しは要らない。テンプレートも置き場を参照していない。
  - 出力の突き合わせには PR 上のビルドの成果物が要る。ところが `/kiro-complete` は PR を最後に作る前提で書かれている。この spec では実装の途中で下書き PR を開き、`/kiro-complete` は既にある PR を使うように直す。

## Research Log

### 現在の公開設定と Actions の状態

- **Context**: 切り替えの出発点を確かめる。
- **Sources Consulted**: `gh api repos/ekicyou/ekicyou.github.io/pages`、`…/actions/permissions`、`…/environments`、`gh run list`(2026-10-11 実行)
- **Findings**:
  - Pages: `build_type: legacy`、原稿は `master` の `/`、`https_enforced: true`、状態は `built`。
  - Actions: 有効。使えるアクションの制限なし(`allowed_actions: all`)。ワークフローの既定の権限は `write`。
  - 環境 `github-pages`: 公開を許すブランチは `master` だけ(独自のブランチ方針)。
  - 直近の旧来ビルド: 実行 `38144045597`(コミット `3b9477d`、2026-10-11T13:39Z、成功)。成果物 `github-pages` は 1.4 MB、**2026-10-12T13:39:50Z に失効**する。
- **Implications**:
  - ワークフローには権限を明示する(既定が `write` なので、書かないと広すぎる)。
  - 公開のジョブは `master` でしか環境を通らない。PR では公開のジョブそのものを動かさない。
  - 突き合わせの基準に旧来の成果物を使うなら、失効の前に取り出す。失効していたら、公開中のサイトを基準にする(下の「出力の突き合わせ」)。

### 使うアクションの版

- **Context**: 版を指定して使う(要件 6.2)。
- **Sources Consulted**: `gh api repos/actions/<name>/releases/latest`(2026-10-11 実行)
- **Findings**:

  | アクション | 最新の版 | 公開日 |
  |---|---|---|
  | `actions/checkout` | v7.0.1 | 2026-07-20 |
  | `actions/configure-pages` | v6.0.0 | 2026-03-25 |
  | `actions/jekyll-build-pages` | v1.0.13 | 2024-08-06 |
  | `actions/upload-pages-artifact` | v5.0.0 | 2026-04-10 |
  | `actions/deploy-pages` | v5.0.1 | 2026-09-01 |

  - `actions/jekyll-build-pages` は 2 年ほど新しい版が出ていないが、保管庫入り(archived)ではない(最後の push は 2026-08-17)。
- **Implications**: `jekyll-build-pages` は出力を決める部品なので `v1.0.13` まで指定する。ほかは主版(`v7`、`v5`)で指定する。`configure-pages` は使わない(無くても動く。部品を 1 つ減らす)。

### 置き場に依存する設定とテンプレート

- **Context**: 「内容を変えずに移す」(要件 1.3)で、例外にする直しを洗い出す。
- **Sources Consulted**: `_config.yml`、`.gitignore`、`.editorconfig`、`.gitattributes`、`_layouts/`・`_includes/`・`assets/` の検索(`site.github`、`page.path`、`relative_path`、`site.url`、`absolute_url`、`jekyll.environment`)
- **Findings**:
  - `_config.yml` の `exclude` は `Gemfile`・`Gemfile.lock`・`node_modules`・`vendor`・`CLAUDE.md`。Jekyll 3 系は `exclude` を書くと既定の一覧を置き換えるので、既定の分も並べてある。
  - `.gitignore`(`.bundle`・`.sass-cache`・`Gemfile.lock`・`_site`・`*.gem`)は、どれも階層を問わない書き方。`site/` の下でも効く。
  - `.editorconfig`・`.gitattributes` にパスの指定は無い。
  - テンプレートは、リポジトリ内の置き場を表す値(`page.path`、`site.github` など)を使っていない。`jekyll.environment == "production"` を見ているのはコメント欄(giscus)と、使っていない部品だけ。
  - ルートの `README.md` は front matter が無く、今はそのまま `/README.md` として公開されている。`/README.html` は無い。
  - 追跡しているファイルは 120。うち原稿は 55(`404.html`、`about.md`、`index.md`、`_config.yml`、`Gemfile`、`_drafts/` 1、`_includes/` 12、`_layouts/` 4、`_posts/` 15、`_sass/` 4、`assets/` 2、`doc/` 5、`img/` 7)。
- **Implications**: 移動の後に直すのは `site/_config.yml` の `exclude` から `CLAUDE.md` を外すことだけ。出力には影響しない。

### `/kiro-complete` との噛み合わせ

- **Context**: 突き合わせと切り替えを、完了の手順のどこに置くか。
- **Sources Consulted**: `.claude/skills/kiro-complete/SKILL.md`(ステップ 5〜7)、`.kiro/steering/workflow.md`
- **Findings**:
  - `/kiro-complete` は、`tasks.md` が全て `[x]` であることを確かめてから、push → `gh pr create` → `gh pr merge --squash` を続けて行う。PR が既にあると `gh pr create` が失敗し、中断する書き方になっている。
  - 検証を PR 上の CI に委ねる場合の一文は既にある(「`gh pr checks --watch` などで緑を確かめてからマージする」)。
  - ステップ 6-5 は「検証の後の変更が公開物の外にあること」を、`.kiro/**`・`.claude/**`・`CLAUDE.md`・`README.md` という旧来の配置で判定している。
- **Implications**:
  - 突き合わせ(マージの前)には PR 上のビルドが要るので、実装の途中で下書き PR を開く。`/kiro-complete` は「PR が既にあればそれを使う」ように直す。
  - 公開方式の設定の切り替えは `tasks.md` の最後のタスクに置く(マージの直前)。マージと、その後の確かめは `/kiro-complete` が行う。確かめ方は `workflow.md` の「検証」節に書く。

## Architecture Pattern Evaluation

| Option | Description | Strengths | Risks / Limitations | Notes |
|--------|-------------|-----------|---------------------|-------|
| 公式アクションで `source: ./site` | `actions/jekyll-build-pages` → `upload-pages-artifact` → `deploy-pages` | 旧来ビルドと同じコンテナ。出力が同じ。Ruby が要らない | アクションの更新が止まり気味。Jekyll 3.10 に固定 | 採用。Jekyll は次の spec で捨てる |
| `ruby/setup-ruby` + `bundle exec jekyll build` | `Gemfile.lock` を管理し、自前でビルド | 版を自分で決められる | `Gemfile.lock` を作るのに Ruby か Docker が要る。出力が変わり得る | 不採用(`roadmap.md`) |
| ビルドと公開を別のワークフローに分ける | PR 用と公開用の 2 ファイル | 役割が見やすい | ビルドの手順が 2 か所に重複する | 不採用。1 ファイルで公開のジョブに条件を付ける |

## Design Decisions

### Decision: ワークフローは 1 ファイル、ジョブは 2 つ

- **Context**: PR ではビルドだけ、`master` では公開まで(要件 2・3)。
- **Alternatives Considered**:
  1. PR 用と公開用でファイルを分ける
  2. 1 ファイルにし、公開のジョブを条件で止める
- **Selected Approach**: 2。`build` ジョブは常に動かし、`deploy` ジョブは `pull_request` 以外かつ `master` のときだけ動かす。
- **Rationale**: ビルドの手順が 1 か所にあれば、PR で確かめたものと公開されるものが同じ手順で作られる。次の spec がビルドの段を差し替えるときも 1 か所で済む。
- **Trade-offs**: PR の実行にも成果物の保存が入る(突き合わせに使うので、むしろ必要)。
- **Follow-up**: 下書き PR で `deploy` が「skipped」になることを確かめる。

### Decision: `Gemfile` は中身を変えずに `site/` へ移す

- **Context**: 公式アクションは `Gemfile` を使わない。
- **Alternatives Considered**:
  1. 消す
  2. `site/` へ移す
- **Selected Approach**: 2。
- **Rationale**: この spec は「中身を変えない引っ越し」。消すと、Jekyll の原稿としての体裁(どの gem で動くかの手掛かり)が先に欠ける。次の spec で Jekyll の部品ごと消える。
- **Trade-offs**: 使われないファイルが 1 つ残る。`site/_config.yml` の `exclude` に `Gemfile` があるので公開はされない。

### Decision: 切り替えは最後のタスク、マージは `/kiro-complete`

- **Context**: 切り替えはマージの前でなければならない(要件 5.1)。一方、切り替えてからマージまでの間は、`master` へ push しても公開されない。
- **Alternatives Considered**:
  1. `/kiro-complete` の中で、マージの直前に切り替える
  2. `tasks.md` の最後のタスクで切り替え、続けて `/kiro-complete` を呼ぶ
- **Selected Approach**: 2。
- **Rationale**: `/kiro-complete` は汎用の手順で、1 回きりの操作を埋め込むと後に残る。タスクにすれば、許しを得る段と確かめの段が `tasks.md` に見える。
- **Trade-offs**: 切り替えからマージまでに間が空くと、その間の記事の push は公開されない。最後のタスクを「開発者がすぐ承認できるとき」に行うことで縮める。
- **Follow-up**: 切り替えの直後に公開中のサイトが応答し続けるか(文書では未確認)を、その場で確かめる。

### Decision: 出力の突き合わせの基準

- **Context**: 要件 4.6。
- **Alternatives Considered**:
  1. 旧来ビルドの成果物(`github-pages`、保存 1 日)
  2. 公開中のサイトを 1 パスずつ取得
- **Selected Approach**: 1 を優先し、失効していたら 2。
- **Rationale**: 成果物どうしなら、パスの一覧も中身も漏れなく比べられる。2 は「旧来だけにあったパス」を列挙できないので、追跡ファイルの一覧から候補を作って補う。
- **Trade-offs**: 2 は手数が増える。旧来ビルドを作り直す手(`POST …/pages/builds`)もあるが、公開中のサイトへの再公開になるので、開発者の許しが要る。
- **Follow-up**: 既知の揺れは `feed.xml` の更新時刻。ほかに差が出たら、意図しない差として扱う。

### Synthesis(一般化・採用か自作か・単純化)

- **一般化**: 「PR ではビルドだけ、`master` では公開まで」は、生成ツールが何であっても同じ形。ビルドの段だけを差し替えられる区切り(成果物 `github-pages` を渡す)にしておく。次の spec の Astro 用の手順は書かない。
- **採用か自作か**: ビルド・成果物の保存・公開・版上げの提案は、すべて GitHub の公式の部品で足りる。自作するものは無い。突き合わせは 1 回きりなので、道具をリポジトリに残さない(scratchpad で行う)。
- **単純化**: `configure-pages` を省く。push のパス絞り込み(`paths:`)をしない(旧来方式も毎回ビルドしていた)。PR の成果物の保存日数を変えない。

## Risks & Mitigations

- 切り替えの順番を誤ると全ページが `/site/...` の下に出る — 切り替えを `tasks.md` の最後のタスクとし、マージ(`/kiro-complete`)はその後にしか動かない並びにする。
- 切り替えの直後に公開が途切れる恐れ(未確認) — 直後に代表的なパスを確かめる。途切れたら、すぐ旧来方式へ戻して作り直しを求める。
- 切り替えからマージまでの間、記事の push が公開されない — 最後のタスクと `/kiro-complete` を続けて行う。開発者に前もって伝える。
- 旧来の成果物が失効する(2026-10-12T13:39Z) — 公開中のサイトを基準にする手順を用意しておく。
- `git mv` の後に改行で差分が全行になる(`core.autocrlf=true`) — 移動だけのコミットを作り、`git diff --cached -M --stat` で変更行が 0 であることを確かめる。
- `actions/jekyll-build-pages` が将来動かなくなる — 版を固定している。次の spec でこのアクションごと無くなる。

## References

- [actions/jekyll-build-pages](https://github.com/actions/jekyll-build-pages) — ビルドのコンテナ。`source`・`destination` の入力
- [actions/upload-pages-artifact](https://github.com/actions/upload-pages-artifact) — 成果物 `github-pages` の作り方
- [actions/deploy-pages](https://github.com/actions/deploy-pages) — 公開。`pages: write` と `id-token: write` が要る
- [GitHub Pages の公開元の設定](https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site) — 「GitHub Actions」を公開元にする
- [Dependabot の設定](https://docs.github.com/en/code-security/dependabot/working-with-dependabot/dependabot-options-reference) — `github-actions` の版上げ
- `.kiro/specs/site-subfolder-actions-build/brief.md` — 起票時の下調べ
- `.kiro/steering/roadmap.md` — 組み方を選んだ理由
