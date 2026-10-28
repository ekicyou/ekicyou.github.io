# Brief: posts-archive-folder

## Problem

記事の置き場 `site/src/content/posts/` に、424 本の記事が平らに並んでいる。そのうち約 400 本は 2003〜2010 年の、もう増えない古い記事である。新しい記事を書くたびに、エディタのファイル一覧で古い記事の山をかき分けることになる。

## Current State

- 記事はサブフォルダに置けない。`site/src/lib/post-path.ts` の `parsePostId()` が、識別子に `/` があればビルドを止める。
- 識別子は、置き場からのパスから拡張子を除いたもの(`site/src/content.config.ts` の `keepName`)。コレクションの読み込みは、すでに `**/*.md` でサブフォルダまで読む。
- 記事の URL `/YYYY/MM/DD/<名前>.html` は、`parsePostId()` がファイル名から決める。ルート・一覧・フィード・前後の記事は、どれもこの結果を使う。
- `site/scripts/verify-dist.mjs` は、非公開の記事を置き場のファイルから探し、そのパスを `parsePostId()` に渡す。サブフォルダの記事があると、公開パスを決められずに誤りになる。
- `site/tests/post-path.test.ts` に「サブフォルダつきは Error を投げる」テストがある。`site/tests/kept-paths.txt` は、保つべき公開パスの一覧である。
- steering の `structure.md` に「サブフォルダに置かない(ビルドが止まる)」とある。

## Desired Outcome

- `posts/` の下なら、どのフォルダに記事を置いてもよい。URL はファイル名だけで決まり、フォルダ名は URL に入らない。
- 同じファイル名の記事が 2 つあれば(フォルダが違っても)、両方のファイルを挙げてビルドを止める。
- 2010 年までの記事を `posts/archive/` へ移してある。移す前と後で、ビルドが出す公開パスの一覧が 1 つも変わらない。
- 直下に残るのは 2011 年以降の記事だけになる。

## Approach

フォルダは自由に置けるようにし、識別子の最後の部分(ファイル名)だけから URL を決める。

- `parsePostId()` は、識別子の最後の `/` より後ろを読む。サブフォルダを拒む規則は外す。
- 同じファイル名の記事の重複を見つけて止める。置き場所は、記事の一覧を作る `publishedPosts()` の近く(全記事が揃う場所)。非公開の記事も重複に数える(URL がぶつかるため)。
- `verify-dist.mjs` は、サブフォルダの記事でも公開パスを決められるように合わせる。
- 古い記事は `git mv` で移し、履歴を保つ。

選ばなかった案:
- `archive/` だけを許す。決まりが固くなるが、手間に見合う得が無い。
- 年のフォルダ(`posts/2004/`)。フォルダ名と日付の食い違いを検査する手間が増える。
- 古い年だけ別のコレクションにする。部品が増える。

## Scope

- **In**: サブフォルダに置いた記事を読めるようにすること、同じファイル名の重複を止めること、`verify-dist.mjs` の追従、テストの更新、2010 年までの記事の `posts/archive/` への移動、steering(`structure.md` ほか、記事の置き場に触れる箇所)の更新。
- **Out**: URL の変更、記事の内容の書き換え、フォルダ名を一覧やページに出すこと、文書(`doc/`)の置き場の変更、2011 年以降の記事の整理。

## Boundary Candidates

- 識別子 → 公開パスの規則(`parsePostId()`)
- 記事の一覧に対する検査(重複の検出)
- 公開後の確かめ(`verify-dist.mjs`)
- 原稿の移動(`git mv` だけ。仕組みの変更と同じ PR に入れるが、コミットは分ける)

## Out of Boundary

- 記事ページ・一覧・フィードの見た目や中身
- `site/public/` の画像の置き場
- 文書コレクション `doc` の識別子(`/doc/<題材>/<名前>.html` で、フォルダが URL に入る。記事とは規則が違うまま)

## Upstream / Downstream

- **Upstream**: Astro のコンテンツコレクション(`glob` ローダー)、`site-astro-redesign` で作った `parsePostId()` と `verify-dist.mjs`。
- **Downstream**: これから書く記事の置き方。将来、年が進んだら、さらに古い記事を `archive/` へ移すのは手作業でよい(仕組みの変更は要らない)。

## Existing Spec Touchpoints

- **Extends**: なし(`site-astro-redesign` は完了済み。その決まりのうち「記事は平らに置く」を、この spec で改める)
- **Adjacent**: `site-astro-redesign`(記事の URL の決め方の正本)

## Constraints

- 公開済みの記事の URL を 1 つも変えない(`master` へのマージがそのまま公開になる)。確かめは、移動の前と後のビルドで公開パスの一覧を突き合わせること、`tests/kept-paths.txt`、`npm run verify`。
- 非公開(`published: false`)の記事が、移動の後も隠れたままであること。
- 1 spec = 1 ブランチ = 1 PR(`workflow.md`)。
