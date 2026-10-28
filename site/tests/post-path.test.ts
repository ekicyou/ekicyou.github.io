// `src/lib/post-path.ts` の単体テスト。
// 記事の識別子(置き場からのパス。フォルダを含んでよい)から、日付・名前・公開パスが決まることを確かめる。
import assert from 'node:assert/strict';
import { describe, it } from 'node:test';

import { parsePostId } from '../src/lib/post-path.ts';

// 今ある記事 15 本の識別子と、公開パス。
// 公開中の 12 本のパスは、切り替えの前のサイトが出していたものと同じ。
// 残りの 3 本(GODZILLA、dcomp、IMM_API)は非公開だが、識別子は同じ決まりで検査される。
const currentPosts: ReadonlyArray<readonly [id: string, path: string]> = [
  ['2014-10-04-pasta', '/2014/10/04/pasta.html'],
  ['2016-02-16-code', '/2016/02/16/code.html'],
  ['2016-06-18-sun', '/2016/06/18/sun.html'],
  ['2016-07-08-dcomp', '/2016/07/08/dcomp.html'],
  ['2016-08-17-GODZILLA', '/2016/08/17/GODZILLA.html'],
  ['2018-08-26-new_site', '/2018/08/26/new_site.html'],
  ['2019-08-31-IMM_API', '/2019/08/31/IMM_API.html'],
  ['2020-08-10-emo', '/2020/08/10/emo.html'],
  ['2024-01-01-nyan_pu_puu', '/2024/01/01/nyan_pu_puu.html'],
  ['2024-12-23-nyan_pu_puu_word', '/2024/12/23/nyan_pu_puu_word.html'],
  ['2025-12-06-mascot_requirements', '/2025/12/06/mascot_requirements.html'],
  ['2026-01-06-syuzo-to', '/2026/01/06/syuzo-to.html'],
  ['2026-02-06-pasta', '/2026/02/06/pasta.html'],
  ['2026-03-26-emo2', '/2026/03/26/emo2.html'],
  ['2026-05-06-cosmogenesis', '/2026/05/06/cosmogenesis.html'],
];

// 投げられた誤りが、識別子と理由を文に含むことを確かめる。
function assertRejects(id: string, reason: RegExp): void {
  assert.throws(
    () => parsePostId(id),
    (error: unknown) => {
      assert.ok(error instanceof Error, `${id}: Error が投げられる`);
      assert.ok(error.message.includes(id), `${id}: 誤りの文に識別子がある(${error.message})`);
      assert.match(error.message, reason, `${id}: 誤りの文に理由がある`);
      return true;
    },
  );
}

describe('parsePostId: 公開パス', () => {
  it('識別子から、日付・名前・公開パスを作る', () => {
    assert.deepEqual(parsePostId('2026-03-26-emo2'), {
      year: '2026',
      month: '03',
      day: '26',
      name: 'emo2',
      date: new Date('2026-03-25T15:00:00.000Z'),
      dateText: '2026-03-26',
      path: '/2026/03/26/emo2.html',
    });
  });

  it('今ある記事 15 本が、決まったパスになる', () => {
    assert.equal(currentPosts.length, 15);
    for (const [id, path] of currentPosts) {
      assert.equal(parsePostId(id).path, path, id);
    }
  });

  it('パスは `/YYYY/MM/DD/<名前>.html` の形で、部品の値と食い違わない', () => {
    for (const [id] of currentPosts) {
      const post = parsePostId(id);
      assert.equal(post.path, `/${post.year}/${post.month}/${post.day}/${post.name}.html`, id);
      assert.equal(post.dateText, `${post.year}-${post.month}-${post.day}`, id);
      assert.equal(`${post.dateText}-${post.name}`, id, id);
    }
  });
});

describe('parsePostId: フォルダつきの識別子', () => {
  it('フォルダを読み飛ばし、直下と同じ結果になる(公開パスと名前にフォルダは入らない)', () => {
    const bare = parsePostId('2004-05-01-foo');
    assert.equal(bare.path, '/2004/05/01/foo.html');
    for (const id of ['archive/2004-05-01-foo', 'a/b/2004-05-01-foo']) {
      assert.deepEqual(parsePostId(id), bare, id);
    }
  });

  it('今ある記事 15 本に `archive/` を付けても、同じパスになる', () => {
    for (const [id, path] of currentPosts) {
      assert.equal(parsePostId(`archive/${id}`).path, path, id);
    }
  });
});

describe('parsePostId: 名前の綴り', () => {
  it('大文字・小文字・記号を変えない', () => {
    const cases: ReadonlyArray<readonly [id: string, name: string, path: string]> = [
      ['2019-08-31-IMM_API', 'IMM_API', '/2019/08/31/IMM_API.html'],
      ['2016-08-17-GODZILLA', 'GODZILLA', '/2016/08/17/GODZILLA.html'],
      ['2026-01-06-syuzo-to', 'syuzo-to', '/2026/01/06/syuzo-to.html'],
      ['2024-12-23-nyan_pu_puu_word', 'nyan_pu_puu_word', '/2024/12/23/nyan_pu_puu_word.html'],
      ['2099-01-01-Dot.Name', 'Dot.Name', '/2099/01/01/Dot.Name.html'],
    ];
    for (const [id, name, path] of cases) {
      const post = parsePostId(id);
      assert.equal(post.name, name, id);
      assert.equal(post.path, path, id);
    }
  });

  it('名前が `index` で始まるだけ、`index` を含むだけの記事は受け入れる', () => {
    assert.equal(parsePostId('2026-01-01-index2').path, '/2026/01/01/index2.html');
    assert.equal(parsePostId('2026-01-01-my-index').path, '/2026/01/01/my-index.html');
  });
});

describe('parsePostId: 日付', () => {
  it('ファイル名の日付の、日本時間の 0 時になる', () => {
    const cases: ReadonlyArray<readonly [id: string, instant: string]> = [
      ['2026-03-26-emo2', '2026-03-25T15:00:00.000Z'],
      // 年と月をまたぐ日。日本時間の 0 時は、UTC では前の日の 15 時。
      ['2024-01-01-nyan_pu_puu', '2023-12-31T15:00:00.000Z'],
      ['2026-03-01-x', '2026-02-28T15:00:00.000Z'],
      ['2014-10-04-pasta', '2014-10-03T15:00:00.000Z'],
    ];
    for (const [id, instant] of cases) {
      const { date } = parsePostId(id);
      assert.ok(date instanceof Date, id);
      assert.equal(date.toISOString(), instant, id);
      assert.equal(date.getTime(), Date.parse(instant), id);
    }
  });

  it('日本時間で見ると、ファイル名の日付の 0 時 0 分である', () => {
    const format = new Intl.DateTimeFormat('sv-SE', {
      timeZone: 'Asia/Tokyo',
      dateStyle: 'short',
      timeStyle: 'medium',
    });
    for (const [id] of currentPosts) {
      const post = parsePostId(id);
      assert.equal(format.format(post.date), `${post.dateText} 00:00:00`, id);
    }
  });

  it('うるう年の 2 月 29 日を受け入れる', () => {
    const post = parsePostId('2024-02-29-leap');
    assert.equal(post.dateText, '2024-02-29');
    assert.equal(post.date.toISOString(), '2024-02-28T15:00:00.000Z');
    assert.equal(post.path, '/2024/02/29/leap.html');
  });

  it('識別子だけで決まる(front matter の `date` を受け取らない)', () => {
    assert.equal(parsePostId.length, 1);
    assert.deepEqual(parsePostId('2026-03-26-emo2'), parsePostId('2026-03-26-emo2'));
  });
});

describe('parsePostId: 決まりに合わない識別子', () => {
  it('日付なしは、識別子を含む Error を投げる', () => {
    for (const id of ['emo2', 'about', 'index']) {
      assertRejects(id, /YYYY-MM-DD-<名前>/);
    }
  });

  it('形が違うものは、識別子を含む Error を投げる', () => {
    const ids = [
      '2026-3-26-emo2', // 月が 1 桁
      '2026-03-6-emo2', // 日が 1 桁
      '26-03-26-emo2', // 年が 2 桁
      '20260326-emo2', // 区切りが無い
      '2026_03_26_emo2', // 区切りが違う
      '2026-03-26', // 名前が無い
      '2026-03-26-', // 名前が空
      'x-2026-03-26-emo2', // 日付が先頭に無い
      '２０２６-03-26-emo2', // 全角の数字
    ];
    for (const id of ids) {
      assertRejects(id, /YYYY-MM-DD-<名前>/);
    }
  });

  it('ありえない日付は、識別子を含む Error を投げる', () => {
    const ids = [
      '2026-13-40-x',
      '2026-02-30-x',
      '2026-13-01-x',
      '2026-00-10-x',
      '2026-01-00-x',
      '2026-01-32-x',
      '2026-04-31-x',
      '2025-02-29-x', // うるう年でない
      '2100-02-29-x', // 100 で割り切れる年は、うるう年でない
    ];
    for (const id of ids) {
      assertRejects(id, /日付として読めない/);
    }
  });

  it('名前が `index` のものは、識別子を含む Error を投げる', () => {
    assertRejects('2026-01-01-index', /名前に `index` は使えない/);
  });

  it('フォルダつきでも、フォルダつきの識別子を含む Error を投げる', () => {
    assertRejects('archive/emo2', /YYYY-MM-DD-<名前>/);
    assertRejects('archive/2026-02-30-x', /日付として読めない/);
    assertRejects('archive/2004-05-01-index', /名前に `index` は使えない/);
  });
});
