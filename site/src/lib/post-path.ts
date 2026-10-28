// 記事の識別子(置き場からのパス。フォルダを含んでよい)から、日付・名前・公開パスを作る。
// フォルダの部分は読み飛ばし、ファイル名だけで決める。
// 記事の URL を決める、ただ 1 つの場所。ルート・一覧・フィード・前後の記事は、ここが返す `path` を使う。
// `astro` の機能に依らない(単体テストから直に呼ぶ)。

export interface PostPath {
  year: string; // "2026"
  month: string; // "03"
  day: string; // "26"
  name: string; // "emo2"(綴りはファイル名のまま)
  date: Date; // その日の日本時間の 0 時
  dateText: string; // "2026-03-26"
  path: string; // "/2026/03/26/emo2.html"
}

// `YYYY-MM-DD-<名前>`。数字は半角だけ。名前は 1 文字以上で、綴りに手を加えない。
const POST_ID = /^\d{4}-\d{2}-\d{2}-.+$/;

// 日本時間は、UTC より 9 時間進んでいる(夏時間は無い)。
const JST_OFFSET_MS = 9 * 60 * 60 * 1000;

// 決まりに合わない識別子の誤り。識別子と理由を文に含める。
function invalid(id: string, reason: string): Error {
  return new Error(`記事のファイル名が決まりに合わない: "${id}"。${reason}`);
}

// 決まりに合わないときは Error を投げる(ビルドが止まる)。
// front matter の `date` は見ない。日付は、ファイル名だけで決まる。
export function parsePostId(id: string): PostPath {
  // 誤りの文には、フォルダを含む識別子をそのまま入れる(どこの原稿かが分かるように)。
  const fileName = id.slice(id.lastIndexOf('/') + 1);
  if (!POST_ID.test(fileName)) {
    throw invalid(id, '`YYYY-MM-DD-<名前>` の形にする。');
  }

  const year = fileName.slice(0, 4);
  const month = fileName.slice(5, 7);
  const day = fileName.slice(8, 10);
  const name = fileName.slice(11);
  const dateText = `${year}-${month}-${day}`;

  // その日の UTC の 0 時を作り、年・月・日が元の数字のままかを見る。
  // 無い日(2 月 30 日、13 月など)は、繰り上がって別の日になるので見分けられる。
  const utcMidnight = new Date(0);
  utcMidnight.setUTCFullYear(Number(year), Number(month) - 1, Number(day));
  if (
    utcMidnight.getUTCFullYear() !== Number(year) ||
    utcMidnight.getUTCMonth() !== Number(month) - 1 ||
    utcMidnight.getUTCDate() !== Number(day)
  ) {
    throw invalid(id, `"${dateText}" は、日付として読めない。`);
  }

  // 名前が `index` だと、ファイル単位の出力が `…/index` をフォルダの入口として扱い、ビルドが失敗する。
  // そのときの Astro の誤りは原因を読み取りにくいので、ここでファイル名を挙げて止める。
  if (name === 'index') {
    throw invalid(id, '名前に `index` は使えない。');
  }

  return {
    year,
    month,
    day,
    name,
    date: new Date(utcMidnight.getTime() - JST_OFFSET_MS),
    dateText,
    path: `/${year}/${month}/${day}/${name}.html`,
  };
}

// ids: すべての記事の識別子(非公開も含める)。
// ファイル名(最後の `/` より後ろ)が同じ組があれば、すべての組の識別子を挙げて Error を投げる(ビルドが止まる)。
// 公開パスはファイル名だけで決まるので、同じファイル名は同じ URL になってしまう。
export function assertUniqueFileNames(ids: readonly string[]): void {
  // shortcut: toLowerCase() は ASCII の外で Windows のファイルシステムの大文字・小文字の扱いと完全には一致しない。今の名前は ASCII だけ。ASCII の外の名前が出たら、比べ方を直す。
  const groups = new Map<string, string[]>();
  for (const id of ids) {
    const key = id.slice(id.lastIndexOf('/') + 1).toLowerCase();
    groups.set(key, [...(groups.get(key) ?? []), id]);
  }

  // 組の中も、組同士も、文字の符号の順に並べる(誤りの文が決まった形になる)。
  const duplicates = [...groups.values()]
    .filter((group) => group.length > 1)
    .map((group) => group.sort().map((id) => `"${id}"`).join('、'))
    .sort();
  if (duplicates.length > 0) {
    throw new Error(
      `同じファイル名の記事がある(大文字・小文字の違いは区別しない)。1 本だけを残す: ${duplicates.join(' / ')}`,
    );
  }
}
