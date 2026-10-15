// 原稿を 3 つのコレクションとして読み、front matter を検査する。
// 検査を通らない原稿が 1 本でもあれば、ビルドはそのファイルを示して止まる。
import { defineCollection } from 'astro:content';
import { glob } from 'astro/loaders';
import { z } from 'astro/zod';

// front matter の定義。3 つのコレクションで同じものを使う。
// 認めるのは、この 9 項目だけ。知らない項目があれば止める(`published` の打ち間違いで、非公開の記事が出るのを防ぐ)。
const frontmatter = z.strictObject({
  title: z.string(),
  // 受け入れるだけで、使わない。レイアウトの選択には使わない。
  layout: z.string().optional(),
  description: z.string().optional(),
  // SNS 向けの画像。公開パスで書く。
  image: z.string().startsWith('/', { error: '"/" で始まる公開パスで書く' }).optional(),
  tags: z.array(z.string()).optional(),
  comments: z.boolean().optional(),
  // 受け入れるだけで、使わない。数式の有無は、描いた後の本文で見分ける。
  use_math: z.boolean().optional(),
  published: z.boolean().optional(),
  // 受け入れるだけで、使わない。記事の日付は、ファイル名から決める。
  date: z.date().optional(),
});

// 識別子は、置き場からのパスから拡張子を除いたもの。
// 既定の小文字化と記号落としを使わず、ファイル名の綴りをそのまま保つ。
const keepName = ({ entry }: { entry: string }) => entry.replace(/\.md$/, '');

export const collections = {
  // 記事。識別子は `YYYY-MM-DD-<名前>`。
  posts: defineCollection({
    loader: glob({ base: './src/content/posts', pattern: '**/*.md', generateId: keepName }),
    schema: frontmatter,
  }),
  // 文書。識別子は `<題材>/<名前>`。
  doc: defineCollection({
    loader: glob({ base: './src/content/doc', pattern: '**/*.md', generateId: keepName }),
    schema: frontmatter,
  }),
  // 単独のページ(About)。
  pages: defineCollection({
    loader: glob({ base: './src/content/pages', pattern: '*.md', generateId: keepName }),
    schema: frontmatter,
  }),
};
