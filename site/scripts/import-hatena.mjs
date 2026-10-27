// 旧サイト(はてなブログ)の記事を、公開ページから取って Markdown の原稿にする。
// 使い方: node scripts/import-hatena.mjs [--blog https://ekicyou.hatenablog.com] [--out src/content/posts] [--limit N] [--year YYYY]
// 既にあるファイルは上書きしない。何度流しても、足りない分だけが増える。
import { mkdir, writeFile, access } from 'node:fs/promises';
import { join } from 'node:path';
import TurndownService from 'turndown';

const arg = (name, def) => {
  const i = process.argv.indexOf(`--${name}`);
  return i < 0 ? def : process.argv[i + 1];
};
const blog = arg('blog', 'https://ekicyou.hatenablog.com').replace(/\/$/, '');
const out = arg('out', 'src/content/posts');
const limit = Number(arg('limit', Infinity));
const year = arg('year', '');

const get = async (url) => {
  const res = await fetch(url);
  if (!res.ok) throw new Error(`${res.status} ${url}`);
  return res.text();
};
const locs = (xml) => [...xml.matchAll(/<loc>([^<]+)<\/loc>/g)].map((m) => m[1].replace(/&amp;/g, '&'));
const decode = (s) =>
  s.replace(/<[^>]+>/g, '').replace(/&(#x?[0-9a-f]+|amp|lt|gt|quot|apos);/gi, (_, e) =>
    ({ amp: '&', lt: '<', gt: '>', quot: '"', apos: "'" })[e] ??
    String.fromCodePoint(e[1] === 'x' ? parseInt(e.slice(2), 16) : parseInt(e.slice(1), 10)));

const td = new TurndownService({ headingStyle: 'atx', codeBlockStyle: 'fenced', bulletListMarker: '-' });
// 地の文の $ は、サイトの数式(remark-math)に読まれないよう逃がす。コードの中は逃がさない。
const baseEscape = td.escape.bind(td);
td.escape = (s) => baseEscape(s).replace(/\$/g, '\\$');
// はてなのコード欄: <pre class="code lang-cs"> の中の span を捨て、文字だけを囲む。
td.addRule('hatenaCode', {
  filter: (n) => n.nodeName === 'PRE',
  replacement: (_, n) => {
    const lang = (n.getAttribute('class') ?? '').match(/lang-(\S+)/)?.[1] ?? '';
    return `\n\n\`\`\`${lang}\n${n.textContent.replace(/\n$/, '')}\n\`\`\`\n\n`;
  },
});
// キーワードリンク(はてなの自動リンク)は文字だけに戻す。
td.addRule('keyword', { filter: (n) => n.nodeName === 'A' && /\bkeyword\b/.test(n.getAttribute('class') ?? ''), replacement: (c) => c });
// 埋め込みカードは、リンクの 1 行にする。
td.addRule('embed', {
  filter: 'iframe',
  replacement: (_, n) => (n.getAttribute('src') ? `\n\n${n.getAttribute('src')}\n\n` : ''),
});

// 記事 1 本分の HTML から、{ file, text } を作る。
export const convert = (html, url) => {
  const title = decode(html.match(/<h1 class="entry-title[^"]*">[\s\S]*?<a [^>]*>([\s\S]*?)<\/a>/)?.[1] ?? '').trim();
  const body = html.match(/<div class="entry-content hatenablog-entry">([\s\S]*?)<footer class="entry-footer">/)?.[1];
  if (!title || body === undefined) throw new Error(`記事の形が読めない: ${url}`);
  const tagsHtml = html.match(/<div class="entry-tags">([\s\S]*?)<\/div>/)?.[1] ?? '';
  const tags = [...tagsHtml.matchAll(/<a [^>]*>([\s\S]*?)<\/a>/g)].map((m) => decode(m[1]).trim()).filter(Boolean);

  // 日付は URL でなく時刻属性(UTC)を日本時間にして決める(旧サイトに年の誤記の URL がある)。
  const dt = html.match(/<time datetime="([^"]+)"/)?.[1];
  const jst = new Date(new Date(dt).getTime() + 9 * 3600 * 1000).toISOString().slice(0, 10).replace(/^1008/, '2008'); // 旧サイトの誤記
  const path = new URL(url).pathname.replace(/^\/entry\//, '').replace(/^\d{4}\/\d{2}\/\d{2}\//, '');
  const name = `${jst}-${path.replace(/[^\w.-]+/g, '_')}`;
  // 末尾の </div> が残るので、本文の閉じ div を 1 つ落としてから変換する。
  const md = td.turndown(body.replace(/<\/div>\s*$/, '')).trim();
  const fm = ['---', `title: ${JSON.stringify(title)}`, ...(tags.length ? [`tags: ${JSON.stringify(tags)}`] : []), '---'];
  return { file: `${name}.md`, text: `${fm.join('\n')}\n\n${md}\n` };
};

// 自分のフォトライフの画像(はてなの画像置き場)を imgDir へ落とし、本文の URL を公開パスに替える。
// 他所の画像(Amazon など)は触らない。
const imgDir = arg('img', 'public/img/hatena');
const imgUrl = arg('img-url', '/img/hatena');
const localizeImages = async (text) => {
  const urls = new Set(text.match(/https?:\/\/cdn-ak\.f\.st-hatena\.com\/images\/fotolife\/[^\s)"]+/g) ?? []);
  for (const u of urls) {
    const file = u.split('/').pop();
    const res = await fetch(u);
    if (!res.ok) throw new Error(`画像が取れない ${res.status} ${u}`);
    await mkdir(imgDir, { recursive: true });
    await writeFile(join(imgDir, file), Buffer.from(await res.arrayBuffer()));
    text = text.replaceAll(u, `${imgUrl}/${file}`);
  }
  return text;
};

if (import.meta.url === new URL(process.argv[1], 'file:').href || process.argv[1]?.endsWith('import-hatena.mjs')) {
  const months = locs(await get(`${blog}/sitemap.xml`)).filter((u) => u.includes('sitemap_periodical') && (!year || u.includes(`year=${year}&`)));
  await mkdir(out, { recursive: true });
  let done = 0, skipped = 0;
  const failed = [];
  outer: for (const m of months) {
    for (const url of locs(await get(m)).filter((u) => u.includes('/entry/'))) {
      if (done >= limit) break outer;
      try {
        const { file, text } = convert(await get(url), url);
        const dest = join(out, file);
        if (await access(dest).then(() => true, () => false)) { skipped++; continue; }
        await writeFile(dest, await localizeImages(text));
        done++;
        console.log(`ok   ${file}`);
      } catch (e) {
        failed.push(`${url} (${e.message})`);
        console.log(`FAIL ${url} ${e.message}`);
      }
      await new Promise((r) => setTimeout(r, 300)); // 相手に優しく
    }
  }
  console.log(`\n書いた ${done} / 既にあった ${skipped} / 失敗 ${failed.length}`);
  if (failed.length) process.exitCode = 1;
}
