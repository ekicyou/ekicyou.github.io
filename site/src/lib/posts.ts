// 公開中の記事を、決まった順に返す。前後の記事を決める。
// ページ・一覧・フィード・前後の記事は、どれもここの結果だけを使う。
import { getCollection, type CollectionEntry } from 'astro:content';
import { parsePostId, type PostPath } from './post-path.ts';

export interface Post extends PostPath {
  entry: CollectionEntry<'posts'>;
}

export interface Adjacent {
  older?: Post;
  newer?: Post;
}

// 文字の並びを、符号の順で比べる(環境の言語の設定に依らず、毎回同じ順になる)。
function compareText(a: string, b: string): number {
  return a < b ? -1 : a > b ? 1 : 0;
}

// 日付の新しい順。同じ日付のときは、名前の降順。
function newerFirst(a: Post, b: Post): number {
  return b.date.getTime() - a.date.getTime() || compareText(b.name, a.name);
}

// 公開中の記事を、新しい順に返す。`published: false` の記事は含めない。
export async function publishedPosts(): Promise<Post[]> {
  const entries = await getCollection('posts');
  // 非公開の記事も含めて、すべての識別子を検査に通す。
  // 決まりに合わないファイルは、非公開でもここで Error になり、ビルドが止まる。
  const all = entries.map((entry): Post => ({ ...parsePostId(entry.id), entry }));
  return all.filter((post) => post.entry.data.published !== false).sort(newerFirst);
}

// 前後の記事を、公開中の記事の並び(新しい順)から決める。端では片方だけになる。
// `older` は 1 つ古い記事、`newer` は 1 つ新しい記事。
export function adjacentPosts(posts: readonly Post[], current: Post): Adjacent {
  const index = posts.findIndex((post) => post.entry.id === current.entry.id);
  // 並びに無い記事には、前後が無い。
  if (index < 0) return {};

  const adjacent: Adjacent = {};
  const older = posts[index + 1];
  const newer = posts[index - 1];
  if (older) adjacent.older = older;
  if (newer) adjacent.newer = newer;
  return adjacent;
}
