// 成果物の検査。ビルドの成果物(`dist/`)が、公開してよい形であることを確かめる。
// Node.js の標準の機能だけで書く。読むのは `dist/`・記事の原稿の置き場・保つパスの一覧だけで、何も書き換えない。
// 検査の決まりは、このファイルと `tests/kept-paths.txt` だけに置く。
//
// 呼び方(`site/` で):
//   node scripts/verify-dist.mjs                     dist/ を検査する。誤りがあれば終了コード 1
//   node scripts/verify-dist.mjs --list              公開されるパスの一覧(`/` 始まり、並べ替え済み)を出す
//   node scripts/verify-dist.mjs --baseline <file>   基準の一覧と比べ、消えるパスと増えるパスを出す(判断は人が行う)
//
// 誤りはビルドを止める。警告は記録に出すだけで、止めない(リンクの打ち間違いで公開が止まらないように)。

import { readFileSync, readdirSync } from 'node:fs';
import { dirname, join, relative, resolve, sep } from 'node:path';
import { fileURLToPath } from 'node:url';
import { parseArgs } from 'node:util';
// 記事の公開パスは、ビルドと同じ決まり(記事の URL を決める、ただ 1 つの場所)で求める。
// `.ts` のまま、Node.js が型を取り除いて読む(単体テストと同じ)。
import { parsePostId } from '../src/lib/post-path.ts';

const SITE_ROOT = resolve(dirname(fileURLToPath(import.meta.url)), '..');
const DIST = join(SITE_ROOT, 'dist');
const POSTS = join(SITE_ROOT, 'src', 'content', 'posts');
const KEPT = join(SITE_ROOT, 'tests', 'kept-paths.txt');

const HOST = 'ekicyou.github.io';
const ORIGIN = `https://${HOST}`;
// トップページの SNS 向けの画像。アイコン絵の絶対 URL。
const ICON_URL = `${ORIGIN}/img/alto0044_400x400.png`;
// フィードの場所。`/` 始まりと、絶対 URL のどちらでもよい。
const FEED_HREFS = new Set(['/feed.xml', `${ORIGIN}/feed.xml`]);
// 同じ github.io の下にある、別リポジトリのページのトップ階層。成果物に作らず、リンクの先も検査しない。
const OTHER_REPOS = new Set(['ghost_dev', 'nyan-pu-puu', 'zzz-pasta', 'old']);

// 認めるパスの形。どれにも合わないパスは誤り。
const ALLOWED_PATHS = [
  /^\/(?:index|about|archive|404)\.html$/,
  /^\/feed\.xml$/,
  /^\/\d{4}\/\d{2}\/\d{2}\/[^/]+\.html$/, // 記事
  /^\/doc\/.+$/, // 文書と、文書に添えるファイル
  /^\/img\/.+$/, // 画像
  /^\/_astro\/.+$/, // スタイル、数式のフォントなど。名前と数は決め打ちしない
];

// ---- 読み込み ----

// フォルダの下のファイルを、`/` 始まりのパスで返す(区切りは `/`)。
function listFiles(root) {
  const out = [];
  const walk = (dir) => {
    for (const entry of readdirSync(dir, { withFileTypes: true })) {
      const full = join(dir, entry.name);
      if (entry.isDirectory()) walk(full);
      else out.push('/' + relative(root, full).split(sep).join('/'));
    }
  };
  walk(root);
  return out;
}

// 文字の符号の順で並べる(OS と言語の設定に依らない)。
const byCodeUnit = (a, b) => (a < b ? -1 : a > b ? 1 : 0);

function listDist() {
  try {
    return listFiles(DIST).sort(byCodeUnit);
  } catch (err) {
    if (err.code === 'ENOENT') fail(`dist/ が無い。先に npm run build を行う。`);
    throw err;
  }
}

// 1 行に 1 つのパスの一覧を読む。空の行は飛ばす。
function readPathList(file) {
  return readFileSync(file, 'utf8')
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter((line) => line !== '');
}

function fail(message) {
  console.error(`誤り: ${message}`);
  process.exit(1);
}

// ---- front matter(非公開の記事を拾う) ----

// Astro と同じ切り出し方(`---` は YAML、`+++` は TOML)。
const FRONTMATTER = /(?:^﻿?|^\s*\n)(?:---|\+\+\+)([\s\S]*?\n)(?:---|\+\+\+)/;
const FRONTMATTER_TYPE = /(?:^﻿?|^\s*\n)(---|\+\+\+)/;

// 原稿が `published: false` を書いているか。
// YAML の依存を足さずに、`published` の行だけを読む。認める書き方:
//   - 一番上の段の `published: false`。`False`・`FALSE`、`:` の後の空白、行末の `# 注釈`、
//     キーを引用符で囲む形(`"published":`)、値を次の行に字下げして書く形、`!!bool` の札。
//   - TOML の front matter(`+++`)の `published = false`。
// front matter の検査は真偽しか通さないので、値が `true` の綴りでなければ、非公開として扱う。
// 読み違えても、非公開の記事を見逃す側には倒れない。
function isUnpublished(text) {
  const raw = FRONTMATTER.exec(text)?.[1];
  if (raw === undefined) return false;
  const toml = FRONTMATTER_TYPE.exec(text)?.[1] === '+++';
  const lines = raw.split(/\r?\n/);
  for (let i = 0; i < lines.length; i++) {
    if (toml) {
      if (/^\s*\[/.test(lines[i])) break; // ここから先は表の中(一番上の段ではない)
      const m = /^\s*(["']?)published\1\s*=(.*)$/.exec(lines[i]);
      if (m) return m[2].replace(/#.*$/, '').trim() !== 'true';
      continue;
    }
    const m = /^(["']?)published\1[ \t]*:(?:[ \t]+(.*))?$/.exec(lines[i]);
    if (!m) continue;
    let value = stripYamlComment(m[2] ?? '');
    if (value === '') {
      // 値を次の行に字下げして書く形。
      for (let j = i + 1; j < lines.length; j++) {
        const next = stripYamlComment(lines[j]);
        if (next === '') continue;
        if (/^[ \t]/.test(lines[j])) value = next;
        break;
      }
    }
    value = value.replace(/^!!bool[ \t]+/, '');
    return !/^(?:true|True|TRUE)$/.test(value);
  }
  return false;
}

function stripYamlComment(s) {
  return s.replace(/(?:^|[ \t])#.*$/, '').trim();
}

// 非公開の記事と、その公開パス。
function unpublishedPosts() {
  let files;
  try {
    files = listFiles(POSTS).filter((p) => p.endsWith('.md'));
  } catch (err) {
    if (err.code === 'ENOENT') return { posts: [], errors: [] };
    throw err;
  }
  const posts = [];
  const errors = [];
  for (const file of files) {
    const source = `src/content/posts${file}`;
    if (!isUnpublished(readFileSync(join(POSTS, file), 'utf8'))) continue;
    try {
      posts.push({ source, path: parsePostId(file.slice(1, -'.md'.length)).path });
    } catch (err) {
      // ビルドはこの原稿で止まるはずである。公開パスが決まらないので、隠れていることを確かめられない。
      errors.push(`${source}: 非公開の記事の公開パスを決められない(${err.message})`);
    }
  }
  return { posts, errors };
}

// ---- HTML を読む ----

const ENTITIES = { amp: '&', lt: '<', gt: '>', quot: '"', apos: "'" };

// 番号の文字参照は、ブラウザと同じく、末尾の `;` が無くても戻す(`&#109ailto:`)。
function decodeEntities(s) {
  return s.replace(/&(?:#x([0-9a-f]+);?|#(\d+);?|([a-z]+);)/gi, (whole, hex, dec, name) => {
    if (hex || dec) {
      const code = hex ? parseInt(hex, 16) : parseInt(dec, 10);
      return code > 0 && code <= 0x10ffff ? String.fromCodePoint(code) : '�';
    }
    return ENTITIES[name.toLowerCase()] ?? whole;
  });
}

// URL として読むときと同じく、前後の空白と制御文字、途中のタブと改行を落とす(`mail&#9;to:` も `mailto:` になる)。
const normalizeUrl = (s) => s.replace(/^[\x00-\x20]+|[\x00-\x20]+$/g, '').replace(/[\t\n\r]/g, '');

// 注釈と、`<script>`・`<style>` などの中身を除いてから、開きタグを拾う。
// 属性の値の中の `>` に惑わされないよう、引用符の中は丸ごと読み飛ばす。
function parseTags(html) {
  const body = html
    .replace(/<!--[\s\S]*?-->/g, '')
    .replace(/(<(script|style|textarea|title)\b(?:"[^"]*"|'[^']*'|[^'">])*>)[\s\S]*?(<\/\2\s*>)/gi, '$1$3');
  const tags = [];
  for (const m of body.matchAll(/<([a-zA-Z][a-zA-Z0-9-]*)((?:"[^"]*"|'[^']*'|[^'">])*)>/g)) {
    const attrs = {};
    for (const a of m[2].matchAll(/([^\s"'>/=]+)(?:\s*=\s*(?:"([^"]*)"|'([^']*)'|([^\s"'=<>`]+)))?/g)) {
      const name = a[1].toLowerCase();
      if (!(name in attrs)) attrs[name] = decodeEntities(a[2] ?? a[3] ?? a[4] ?? '');
    }
    tags.push({ name: m[1].toLowerCase(), attrs });
  }
  return tags;
}

const tokens = (value) => (value ?? '').toLowerCase().split(/\s+/).filter(Boolean);

// リンクの先を、成果物の中のパスにする。サイトの外と、検査しない先は null。
function internalTarget(href, pagePath) {
  const raw = normalizeUrl(href);
  if (raw === '' || raw.startsWith('#')) return null;
  let url;
  try {
    url = new URL(raw, ORIGIN + pagePath);
  } catch {
    return { error: 'URL として読めない' };
  }
  if (!/^https?:$/.test(url.protocol) || url.hostname !== HOST) return null;
  let path;
  try {
    path = decodeURIComponent(url.pathname);
  } catch {
    return { error: 'パスの % の書き方が読めない' };
  }
  if (OTHER_REPOS.has(path.split('/')[1])) return null;
  return { path };
}

// GitHub Pages と同じ見つけ方: `/` で終われば `index.html`、拡張子の無いパスは `.html` も探す。
function targetExists(path, files) {
  if (path.endsWith('/')) return files.has(path + 'index.html');
  return files.has(path) || files.has(path + '.html') || files.has(path + '/index.html');
}

// ---- 検査 ----

function verify() {
  const paths = listDist();
  const files = new Set(paths);
  const errors = [];
  const warnings = [];

  // 保つパスが、綴りも同じで、すべてある。
  for (const line of readPathList(KEPT)) {
    if (!line.startsWith('/')) errors.push(`tests/kept-paths.txt: "/" で始まらない行がある: ${line}`);
    else if (!files.has(line)) errors.push(`${line}: 保つパスが無い(tests/kept-paths.txt)`);
  }

  // すべてのパスが、認める形のどれかに合う。
  for (const path of paths) {
    if (ALLOWED_PATHS.some((re) => re.test(path))) continue;
    const top = path.split('/')[1];
    if (OTHER_REPOS.has(top) && path.split('/').length > 2) {
      errors.push(`${path}: 別リポジトリのページと同じ名前のトップ階層(/${top}/)に出ている`);
    } else {
      errors.push(`${path}: 認める形に合わないパスが出ている`);
    }
  }

  // HTML ごとの検査。
  const htmlPaths = paths.filter((p) => p.endsWith('.html'));
  for (const page of htmlPaths) {
    const html = readFileSync(join(DIST, page), 'utf8');
    const tags = parseTags(html);
    const links = tags.filter((t) => t.name === 'link');

    // メールのリンク。タグの href の値だけを見る。
    // 文字としての `mailto:`(本文のアドレス、コードの例の `&#x3C;a href="mailto:…">`)は誤りにしない。
    const mailto = tags.some((t) => /^mailto:/i.test(normalizeUrl(t.attrs.href ?? '')));
    if (mailto) errors.push(`${page}: メールのリンク(href="mailto:…")がある`);

    const htmlTag = tags.find((t) => t.name === 'html');
    if (htmlTag?.attrs.lang !== 'ja') errors.push(`${page}: <html> に lang="ja" が無い`);

    const canonical = links.find((t) => tokens(t.attrs.rel).includes('canonical'));
    if (!canonical) {
      errors.push(`${page}: 正規の URL(<link rel="canonical">)が無い`);
    } else if (!(canonical.attrs.href ?? '').startsWith(`${ORIGIN}/`)) {
      errors.push(`${page}: 正規の URL が ${ORIGIN}/ で始まる絶対 URL でない: ${canonical.attrs.href ?? ''}`);
    }

    const feed = links.some(
      (t) =>
        tokens(t.attrs.rel).includes('alternate') &&
        /^application\/(?:rss|atom)\+xml$/i.test(t.attrs.type ?? '') &&
        FEED_HREFS.has(t.attrs.href ?? ''),
    );
    if (!feed) errors.push(`${page}: フィードの場所(<link rel="alternate" … href="/feed.xml">)が無い`);

    const favicon = links.some((t) => tokens(t.attrs.rel).includes('icon') && (t.attrs.href ?? '').trim() !== '');
    if (!favicon) errors.push(`${page}: favicon(<link rel="icon">)が無い`);

    if (page === '/index.html') {
      const images = tags
        .filter((t) => t.name === 'meta' && (t.attrs.property ?? t.attrs.name) === 'og:image')
        .map((t) => t.attrs.content ?? '');
      if (images.length === 0) errors.push(`${page}: og:image が無い`);
      for (const image of images) {
        if (image !== ICON_URL) errors.push(`${page}: og:image がアイコン絵(${ICON_URL})でない: ${image}`);
      }
    }

    // サイト内のリンクと画像の先。
    const seen = new Set();
    for (const t of tags) {
      const refs = [];
      if (t.name === 'a' || t.name === 'link') refs.push(['リンク', t.attrs.href]);
      if (t.name === 'img' || t.name === 'script' || t.name === 'source' || t.name === 'iframe') {
        refs.push([t.name === 'img' ? '画像' : 'リンク', t.attrs.src]);
      }
      if (t.name === 'img' || t.name === 'source') {
        for (const candidate of (t.attrs.srcset ?? '').split(',')) {
          refs.push(['画像', candidate.trim().split(/\s+/)[0]]);
        }
      }
      for (const [kind, href] of refs) {
        if (!href) continue;
        const target = internalTarget(href, page);
        if (!target) continue;
        const key = `${kind}\0${href}`;
        if (seen.has(key)) continue;
        seen.add(key);
        if (target.error) warnings.push(`${page}: サイト内の${kind}の先が${target.error}: ${href}`);
        else if (!targetExists(target.path, files)) warnings.push(`${page}: サイト内の${kind}の先が無い: ${href}`);
      }
    }

    // KaTeX が読めなかった数式(TeX の文字のまま出る)。
    const katexErrors = tags.filter((t) => tokens(t.attrs.class).includes('katex-error'));
    if (katexErrors.length > 0) {
      const first = katexErrors[0].attrs.title ?? '';
      warnings.push(`${page}: KaTeX が読めなかった数式が ${katexErrors.length} 個ある(最初: ${first})`);
    }
  }

  // 非公開の記事が、ページ・トップページ・フィードのどこにも出ていない。
  const { posts: hidden, errors: hiddenErrors } = unpublishedPosts();
  errors.push(...hiddenErrors);
  const index = files.has('/index.html') ? readFileSync(join(DIST, 'index.html'), 'utf8') : '';
  const feed = files.has('/feed.xml') ? readFileSync(join(DIST, 'feed.xml'), 'utf8') : '';
  for (const { source, path } of hidden) {
    if (files.has(path)) errors.push(`${path}: 非公開の記事(${source})のページが出ている`);
    const mention = mentionPattern(path);
    if (mention.test(index)) errors.push(`/index.html: 非公開の記事(${source})へのリンクがある: ${path}`);
    if (mention.test(feed)) errors.push(`/feed.xml: 非公開の記事(${source})が載っている: ${path}`);
  }

  for (const e of errors) console.log(`誤り: ${e}`);
  for (const w of warnings) console.log(`警告: ${w}`);
  console.log(
    `成果物の検査: 誤り ${errors.length} 件、警告 ${warnings.length} 件` +
      `(ファイル ${paths.length} 個、HTML ${htmlPaths.length} 本、非公開の記事 ${hidden.length} 本)`,
  );
  return errors.length === 0 ? 0 : 1;
}

// 公開パスへの言及。`.html` つきと、拡張子を落とした形(GitHub Pages はどちらでも返す)、
// % で書いた形(名前に ASCII のほかの文字があるとき)を拾う。
function mentionPattern(path) {
  const stem = path.slice(0, -'.html'.length);
  const forms = new Set([stem, encodeURI(stem)]);
  const escape = (s) => s.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  const alternatives = [...forms].map(escape).join('|');
  return new RegExp(`(?:${alternatives})(?:\\.html|(?=["'<>\\s#?/&]|$))`);
}

// ---- 一覧と突き合わせ ----

function list() {
  for (const path of listDist()) console.log(path);
  return 0;
}

function compareBaseline(file) {
  let baseline;
  try {
    baseline = readPathList(file);
  } catch (err) {
    fail(`基準の一覧を読めない: ${file}(${err.code ?? err.message})`);
  }
  const current = listDist();
  const before = new Set(baseline);
  const after = new Set(current);
  const removed = [...before].filter((p) => !after.has(p)).sort(byCodeUnit);
  const added = current.filter((p) => !before.has(p));

  console.log(`基準: ${file}(${before.size} 個)`);
  console.log(`成果物: dist/(${after.size} 個)`);
  console.log('');
  console.log(`消えるパス(${removed.length} 個):`);
  for (const p of removed) console.log(`- ${p}`);
  console.log('');
  console.log(`増えるパス(${added.length} 個):`);
  for (const p of added) console.log(`+ ${p}`);
  console.log('');
  // 判断の手掛かり。判断は人が行う(終了コードは常に 0)。
  const removedElsewhere = removed.filter((p) => !p.startsWith('/assets/')).length;
  const addedElsewhere = added.filter((p) => !p.startsWith('/_astro/')).length;
  console.log(`参考: /assets/ の外で消えるパス ${removedElsewhere} 個、/_astro/ の外で増えるパス ${addedElsewhere} 個`);
  return 0;
}

// ---- 入口 ----

let args;
try {
  args = parseArgs({
    options: { list: { type: 'boolean' }, baseline: { type: 'string' } },
    allowPositionals: false,
  }).values;
} catch (err) {
  fail(`${err.message}\n使い方: node scripts/verify-dist.mjs [--list | --baseline <一覧のファイル>]`);
}
if (args.list && args.baseline !== undefined) fail('--list と --baseline は、同時に使えない。');

process.exitCode = args.list ? list() : args.baseline !== undefined ? compareBaseline(args.baseline) : verify();
