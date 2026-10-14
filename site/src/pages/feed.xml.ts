// フィード。`/feed.xml` に、RSS 2.0 で、公開中のすべての記事を新しい順に載せる。
// 載せる記事と順は、`publishedPosts()` の結果そのまま。`published: false` の記事は含まれない。
// 項目は、題名・リンク・日付・説明・タグだけ。本文の全文は載せない。
import type { APIRoute } from 'astro';
import rss from '@astrojs/rss';
import { SITE } from '../consts.ts';
import { publishedPosts } from '../lib/posts.ts';
import { describe } from '../lib/summary.ts';

export const GET: APIRoute = async () => {
  const posts = await publishedPosts();

  return rss({
    title: SITE.title,
    description: SITE.description,
    // チャンネルのリンクは、トップページの正規の URL と同じ `https://ekicyou.github.io/` になる。
    // `trailingSlash` は渡さない。`false` にすると、このリンクの末尾の `/` まで落ちる。
    site: SITE.url,
    customData: `<language>${SITE.lang}</language>`,
    items: posts.map((post) => {
      const { data, rendered } = post.entry;
      return {
        title: data.title,
        // 絶対 URL で渡す。公開パスは `post.path` をそのまま使い、ここでは組み立てない。
        // 絶対 URL は、`@astrojs/rss` が手を加えずに `<link>` と `<guid>` に出す。
        link: `${SITE.url}${post.path}`,
        // ファイル名の日付の、日本時間の 0 時。front matter の `date` は見ない。
        pubDate: post.date,
        // 記事のページの説明と同じ決め方。
        description: describe({ description: data.description, html: rendered?.html }, SITE.description),
        // タグは `<category>` になる。タグの無い記事には出ない。
        categories: data.tags,
      };
    }),
  });
};
