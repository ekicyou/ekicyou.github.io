---
name: kiro-complete
description: 'Kiro仕様駆動開発のSpec完了ワークフローを実行する。未解決問題の棚卸（軽微なものはその場で解決・残りは/kiro-discoveryで起票）→DoD静的ゲート→コミット→completedフォルダ移動→spec.json更新→参照パス修正→アーカイブのコミット→検証（workflow.md が定めるサイトのビルド確認）→文書更新と最終コミット→PR作成→squashマージまでを中断なく完遂する。既定ブランチへの統合はこのPRが唯一の経路（直push禁止）。Use when: 実装完了を承認する, 承認してください, 完了を承認, spec承認, approve implementation, kiro承認完了。DO NOT USE when: 実装が完了したのみ（承認の明示がない場合）、タスクが終わっただけ'
allowed-tools: Bash, PowerShell, Read, Write, Edit, Glob, Grep, Skill, AskUserQuestion
argument-hint: <feature-name>
---

# Kiro Spec 完了ワークフロー

> 由来: areka の `kiro-complete` を my-site（Jekyll の個人ブログ）向けに薄くしたもの。cargo・ライセンス検査・ロードマップ台帳・kiro-watch の調停は持ち込んでいない。

## 発動条件（必須）

> **このスキルは開発者の明示的「承認」がある場合にのみ発動する。**

### 発動する（承認の明示がある）
- 「実装完了を**承認**します」「**承認**してください」
- 「このspecを**承認**する」「**approve**」「kiro **承認**完了」

### 発動しない（承認の明示がない）
- 「実装が完了した」「タスクが全部終わった」
- 「spec完了」「アーカイブしてほしい」などの曖昧な表現のみ
- AIが自律的に「完了したと判断」した場合

### 承認が不明瞭なとき
発動を迷う場合は **発動しない**。必要なら開発者に確認する:
> 「実装完了の承認をいただけますか？承認いただいた場合、完了ワークフローを実行します。」

---

## 完了基準の権威（ベースライン内蔵 + 任意の workflow.md）

このスキルはベースラインの完了基準（DoD）とコミット規約を内蔵し、単体で機能する。**`.kiro/steering/workflow.md` が存在する場合**は、それを追加の権威として優先する。特に **検証コマンド（サイトのビルド確認）は workflow.md の「検証」節が正本**であり、このスキルはコマンドを決め打ちしない。

## 哲学
- **中断せず一連で完遂する** — 全ステップを止めずに実行する
- **spec.json の編集は移動の後** — VS Code が移動前の編集を追跡していると、移動元にファイルが復活することがある
- **既定ブランチへの統合は PR が唯一の経路** — ブランチはハーネスのワークツリーが供給する。1 feature = 1 branch = 1 PR。`{default-branch}` への直接 push は一切行わない
- **公開中のサイトに効く** — このリポジトリは既定ブランチへのマージがそのまま公開につながる。検証が赤のままマージしない
- **繰り返し仕様は移動しない** — 繰り返し実行型の仕様は `.kiro/specs/` 直下に留める

## 前提条件
- `.kiro/specs/{feature}/tasks.md` の全タスクが完了
- 設計文書と最終実装の整合確認済み

## 例外: 繰り返し仕様

spec.json や requirements.md に「繰り返し」「repeatable」「定期実行」等の記述がある仕様は `completed/` へ移動しない。ステップ3・4 を飛ばし、tasks.md のチェックボックスをリセット（全 `[x]` → `[ ]`）してコミットし、ステップ5 以降を実行する。

---

## 全体の流れ

```
冒頭 棚卸・その場の修正・起票
0  portable context・既定ブランチとの差の確認
1  DoD 静的ゲート
2  未コミットのコミット
3  completed/ へ移動・spec.json
4  参照パスの更新
5  アーカイブのコミット → 検証（workflow.md の検証コマンド）
6  文書更新（steering・CLAUDE.md・スキル文書）・PR 文面の下書き → 最終コミット
7  PR 作成 → squash マージ
```

---

## 手順

### 冒頭ステップ: 未解決問題の棚卸（軽微はその場で解決・残りは `/kiro-discovery` で起票）

完了してアーカイブした後では拾い漏れに気付けなくなるので、必ず最初に行う。

1. **未解決問題を洗い出す**。拾う元:
   - `tasks.md` の `## Implementation Notes`・`_Blocked:_` の付いたタスク
   - `design.md` の Open Questions / Risks
   - 実装・レビュー・最終検証（`/kiro-validate-impl`）の報告に残った未対応の指摘・先送り
   - 会話の中で「別 spec で」「後で」と決めたもの
2. **起票済みかを確かめる**。`.kiro/specs/`（`completed/` を含む）の brief.md を検索し、既に担当 spec があるもの・本 spec の中で解決済みのもの・「直さない」と決めたものは除く。
3. **軽微なものはその場で解決する**。次の**すべて**を満たすものだけ:
   - 開発者の裁定・設計判断・要件の追加や改訂を要しない（直し方が 1 通りに決まる）
   - 変更が小さく閉じている（目安: 数ファイル・数十行。文言・リンク切れ・設定の書き漏らしなど）
   - 進行中の他 spec の担当範囲と重ならない
   - 公開 URL・記事のパーマリンク・依存（`Gemfile`）を変えない

   修正は 1 件 1 コミットでステップ2 の前に積み、`tasks.md` の `## Implementation Notes` に「完了時にその場で解決」として 1 行ずつ記録する。条件を外れると分かったら戻して起票へ回す。
4. **残った未起票のものを `/kiro-discovery` で起票する**（Skill ツールで `kiro-discovery` を起動）。ハーネスの「別の作業として切り出す」チップ（`spawn_task`）は使わない。
5. **その場で解決した件数と起票した件数をそれぞれ記録する。0 件なら「0 件」と書く**（黙って飛ばさない）。

### ステップ0: 決定的解決（portable context）

`{remote}` と `{default-branch}` を固定優先順序で 1 回だけ解決し、以降は `origin`/`main` のハードコードの代わりに使う（`kiro-start` の Step 0 と同じ順序）。

1. **`{remote}`**: `git remote` を実行する。`origin` があれば `origin`。無くてリモートがちょうど 1 つならそれ。それ以外は none（リモート操作はすべてスキップし、一度だけ警告する）。
2. **`{default-branch}`**: `{remote}` があれば `git symbolic-ref --quiet --short refs/remotes/{remote}/HEAD` から `"{remote}/"` を除いた名前。空ならローカルの `main`、無ければ `master`、どちらも無ければ現在のブランチ。
3. **既定ブランチとの差を確かめる**: `git fetch {remote}` のあと `git log --oneline HEAD..{remote}/{default-branch}` が空でなければ既定ブランチが先へ進んでいる。`git merge-tree --write-tree HEAD {remote}/{default-branch}` が衝突を報告するなら、**検証（ステップ5）より前に**取り込む（ハーネスのワークツリーなら `sync_with_base_branch`、それ以外は `git merge`）。

> リモート URL に資格情報が含まれることがある。`git remote -v` や fetch の出力を報告へ写すときは伏せる。

### ステップ1: DoD 静的ゲート

1. `.kiro/steering/workflow.md` が存在すれば読み込み、追加の DoD ゲートと検証コマンドを取り込む。
2. **Spec Gate**: 当該 spec の `tasks.md` が全タスク `[x]` 完了であること（親の項目のチェックボックスも含む）。
3. **いずれかのゲートが失敗した場合**: ワークフローを中断し、開発者に報告する。

### ステップ2: 未コミットファイルのコミット

```bash
git status --short
git add -A
git commit -m "<type>({feature-name}): 実装完了

- 変更の要約（箇条書き）"
```

コミットタイプは `feat` / `fix` / `refactor` / `docs` / `chore`。未コミットの変更が無ければ何もしない。

### ステップ3: completed フォルダへの移動と spec.json の更新

```bash
mkdir -p .kiro/specs/completed
git mv .kiro/specs/{feature-name} .kiro/specs/completed/
```

**移動の後で** `.kiro/specs/completed/{feature-name}/spec.json` を更新する。

```json
{
  "phase": "completed",
  "updated_at": "YYYY-MM-DDTHH:MM:SSZ"
}
```

- `phase` を変えるときは必ず `updated_at` も更新する。
- `"status"` フィールドは使わない。`"phase": "completed"` で完了を表す。

### ステップ4: 参照パスの更新

移動した spec を旧パスで指している箇所を直す。

```bash
git grep -n "{feature-name}" -- . ':!.kiro/specs/completed/{feature-name}'
```

- `.kiro/specs/{feature-name}/` → `.kiro/specs/completed/{feature-name}/` に置き換える（他の spec 文書・steering・`CLAUDE.md`・スキル文書）。
- サイトの原稿（記事・レイアウト・設定）が spec 文書をパスで読むことは通常ない。ヒットしたら、公開物に効く参照かどうかを確かめてから直す。
- 検出がゼロでも「検索を実行した」ことをチェックリストに記録する。

### ステップ5: アーカイブのコミットと検証

#### 5-1. アーカイブのコミット

```bash
git add -A
git commit -m "chore({feature-name}): spec完了・アーカイブ"
git rev-parse --short HEAD   # 検証したコミットとして控える
```

#### 5-2. 検証（サイトのビルド確認）

**検証コマンドは `.kiro/steering/workflow.md` の「検証」節に従う。** このスキルはコマンドを決め打ちしない。

- 節が定めるコマンドを実行し、**終了コードで**合否を判定する（出力の一部を拾って決めない）。時間がかかるものは `run_in_background` で起動し、完了通知を待つ。
- 節が「手元では確かめられない」と定めている場合は、その節が指す代わりの確認（例: PR 上の CI の結果）を使う。その場合はステップ7 で PR を作った後、**マージの前に** CI が緑であることを確かめる。
- 節が無い、または検証手段が未整備と書かれている場合は、**黙って飛ばさない**。「検証手段が未整備のため省略」とチェックリストに記録し、PR の body にも書く。変更が公開物（記事・レイアウト・設定・画像）に触れているなら、マージの前に開発者へ 1 度だけ確認する。
- **赤のとき**: 原因を直してコミットし、検証をやり直す。直せないコードの赤なら中断して開発者に報告する。**赤のままステップ7 へ進まない。**

### ステップ6: 文書更新と最終コミット

1. **steering**: 当該変更で古くなる `.kiro/steering/*.md`（特に `structure.md`・`tech.md`・`workflow.md`）を更新する。
2. **`CLAUDE.md` とスキル文書**: 変更領域に触れる記述があれば揃える。
3. **PR 文面の下書き**: squash の subject／body と PR の body をスクラッチパッドのファイルへ書く（作り方はステップ7 の「squash メッセージ生成」）。冒頭ステップでその場で解決した件・起票した件も載せる。`gh auth status` もここで確かめる。
4. **最終コミット**:

   ```bash
   git status --short
   git add -A
   git commit -m "chore({feature-name}): 完了の文書更新"
   ```

5. **検証の後の変更が公開物の外にあることを確かめる**: `git diff --name-only <5-1 で控えたコミット> HEAD` が `.kiro/**`・`.claude/**`・`CLAUDE.md`・`README.md` だけであること。それ以外があればステップ5-2 の検証をやり直す。

### ステップ7: リモート同期（PR ベース）

> フィーチャーブランチ／ワークツリーは Claude Code（ハーネス）が供給しており、このスキルは自前でブランチ／ワークツリーを作成・削除しない。**`{default-branch}` への直接 push は一切行わない。**

#### PR 可否判定

次を**すべて満たす**ときのみ PR を作成・マージする。

1. 現在ブランチが `{default-branch}` 以外
2. `{remote}` が none でない
3. `gh auth status` が成功

いずれかが欠ける場合は警告を出して PR・push をスキップし、ローカルコミットを保持する。

#### PR 可: push → PR 作成 → squash マージ

```bash
branch=$(git rev-parse --abbrev-ref HEAD)
git push -u {remote} "$branch"
gh pr create --base {default-branch} --head "$branch" --title "<subject>" --body-file <PR body の下書き>
gh pr merge --squash --delete-branch --subject "<subject>" --body-file <squash body の下書き>
```

- 検証を PR 上の CI に委ねている場合（ステップ5-2）は、`gh pr checks --watch` などで緑を確かめてからマージする。
- **マージ成否はマージ API の結果のみで判定する。** `--delete-branch` のローカル削除の警告は非致命（カレントワークツリーでチェックアウト中のため起きる。リモートブランチは API で削除済み）。
- ローカルブランチ／ワークツリーの後始末はハーネスへ委ねる。

**squash メッセージ生成**:
1. `git log --no-merges --pretty=format:"%h %s%n%b" {default-branch}..HEAD` で分岐点以降の全コミットを取得する。
2. 対象 spec の `requirements.md` / `design.md` のタイトル・概要も参照する。
3. 次の形へ再構成する。
   - **subject**: `<type>({feature-name}): <変更全体を 1 文で表す要約>`
   - **body**: 何を・なぜ変えたかを箇条書き（3〜7 項目）。個々のコミットの羅列にしない。

#### 中断条件

PR の作成またはマージ（API）が失敗した場合（コンフリクト・必須チェック未通過・権限不足など）は、**ブランチを削除せず**中断して開発者へ報告する。

---

## 完了チェックリスト

```
- [ ] 冒頭ステップ: 未解決問題を棚卸し済み（その場で解決 n 件・起票 n 件。0 件なら「0 件」）
- [ ] ステップ0: `{remote}` / `{default-branch}` を解決済み・既定ブランチとの衝突なし
- [ ] DoD 静的ゲート通過（Spec Gate と workflow.md の追加ゲート）
- [ ] 未コミットファイルをコミット済み
- [ ] completed へ移動済み・spec.json の phase を "completed" に更新 + updated_at 更新 ※繰り返し仕様はスキップ
- [ ] 参照パスを検索・更新済み（検出ゼロでも実施を記録）※繰り返し仕様はスキップ
- [ ] アーカイブのコミット済み・検証したコミットを控えた
- [ ] 検証: workflow.md の検証コマンドが終了コード 0（未整備なら「省略」と理由を記録し、PR body にも記載）
- [ ] 文書更新（steering・CLAUDE.md・スキル文書）と最終コミット済み
- [ ] 検証の後の変更が公開物の外にあることを確認済み
- [ ] リモート同期完了（PR 可: push → PR → squash マージ／PR 不可: 警告してスキップ／失敗: ブランチを残して中断・報告）
```

---

## エラー回避

- **移動したファイルが元の場所に復活する**: spec.json を移動の前に編集した。必ず移動の後で編集する。
- **後続 spec が旧パスで参照している**: ステップ4 の検索を省いた。`git grep` で網羅的に探す。
- **push したが変更が反映されていない**: コミット漏れ。各コミットの前に `git status --short` で確かめる。
- **差分が全行になる**: 改行コードが壊れている。`sed -i` は CR を落とすので使わず、Edit ツールで編集する。
- **`gh pr create` / `gh pr merge` が失敗する**: ブランチを削除せず中断して報告する。既存 PR があればそれを確かめる。
- **`{default-branch}` 上で承認された**: PR の head に使えない。警告して PR・push をスキップし、ローカルコミットを保持する。
