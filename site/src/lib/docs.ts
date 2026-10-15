// 公開中の文書を、決まった順に返す。
// ページと一覧は、どちらもここの結果だけを使う。
import { getCollection, type CollectionEntry } from 'astro:content';

export interface Doc {
  entry: CollectionEntry<'doc'>;
  path: string; // "/doc/<題材>/<名前>.html"
}

// 公開中の文書を、公開パスの昇順に返す。`published: false` の文書は含めない。
// 文書の識別子は `<題材>/<名前>` で、公開パスは `/doc/<識別子>.html`。
export async function publishedDocs(): Promise<Doc[]> {
  const entries = await getCollection('doc');
  return (
    entries
      .filter((entry) => entry.data.published !== false)
      .map((entry): Doc => ({ entry, path: `/doc/${entry.id}.html` }))
      // 符号の順で比べる(環境の言語の設定に依らず、毎回同じ順になる)。
      .sort((a, b) => (a.path < b.path ? -1 : a.path > b.path ? 1 : 0))
  );
}
