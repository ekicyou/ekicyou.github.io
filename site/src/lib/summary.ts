// ページの説明を決める。描いた後の本文に、数式の出力があるかを見分ける。
// 説明の決まりは、SNS 向けの情報・記事の一覧・フィードの 3 か所で、同じ `describe()` を使う。
// `astro` の機能に依らない(単体テストから直に呼ぶ)。

export interface DescribeInput {
  description?: string; // front matter の値
  html?: string; // 描いた後の本文(entry.rendered?.html)
}

// 最初の段落の文字は、この字数で切る。字は、符号位置で数える。
const SUMMARY_LENGTH = 200;

// コメントか、タグ。タグは、名前(1)と、名前の後ろの属性を読む。
// 属性の値は `>` を含むことがあるので、引用符の組をひとまとめに読む。
// 本文の文字としての `<` は、処理系が文字参照(`&#x3C;`)にして出すので、タグと取り違えない。
const MARKUP = /<!--[\s\S]*?-->|<\/?([A-Za-z][^\s/>]*)(?:"[^"]*"|'[^']*'|[^"'>])*>/g;

// 文字参照。10 進の番号(1)、16 進の番号(2)、名前(3)。
const CHARACTER_REFERENCE = /&(?:#(\d+)|#[xX]([0-9A-Fa-f]+)|([A-Za-z][A-Za-z0-9]*));/g;

// 名前の文字参照のうち、戻すもの。処理系は番号の文字参照しか出さないので、手で書かれやすいものに絞る。
// 見えない文字は、ソースに直に書かず、番号から作る。
const NAMED_REFERENCES: ReadonlyMap<string, string> = new Map([
  ['amp', '&'],
  ['lt', '<'],
  ['gt', '>'],
  ['quot', '"'],
  ['apos', "'"],
  ['nbsp', String.fromCodePoint(0xa0)], // 改行しない空白(U+00A0)
]);

// 文字にならない番号の代わりに置く文字(U+FFFD)。
const REPLACEMENT_CHARACTER = String.fromCodePoint(0xfffd);

// HTML が 1 つにまとめて表示する空白(空白、タブ、改行)。全角の空白は含まない。
const HTML_SPACES = /[ \t\n\f\r]+/g;

// KaTeX が描いた数式の、いちばん外の要素(`class` に `katex` という名前を持つ `<span>`)。
// 独立した行の数式(`katex-display`)も、中にこの要素を持つ。
// 読めなかった数式(`katex-error`)は含まない。色を要素に直に持ち、スタイルシートが要らない。
const KATEX_OUTPUT = /<span(?=\s)(?:"[^"]*"|'[^']*'|[^"'>])*?\sclass="(?:[^"]*\s)?katex(?:\s[^"]*)?"/;

// 番号の文字参照を文字に戻す。文字にならない番号(0、サロゲート、範囲の外)は U+FFFD にする。
function fromCodePoint(code: number): string {
  const valid = code > 0 && code <= 0x10ffff && !(code >= 0xd800 && code <= 0xdfff);
  return valid ? String.fromCodePoint(code) : REPLACEMENT_CHARACTER;
}

// 文字参照を戻す。知らない名前は、そのまま残す。戻すのは 1 度だけ。
function decodeCharacterReferences(text: string): string {
  return text.replace(
    CHARACTER_REFERENCE,
    (reference: string, decimal: string | undefined, hex: string | undefined, name: string | undefined) => {
      if (decimal !== undefined) return fromCodePoint(Number.parseInt(decimal, 10));
      if (hex !== undefined) return fromCodePoint(Number.parseInt(hex, 16));
      return (name !== undefined ? NAMED_REFERENCES.get(name) : undefined) ?? reference;
    },
  );
}

// 本文の最初の `<p>` の中身(HTML のまま)を返す。`<p>` が無ければ `undefined`。
// コメントの中や、属性の値の中に書かれた `<p>` は、数えない。
function firstParagraphHtml(html: string): string | undefined {
  let start: number | undefined;
  for (const markup of html.matchAll(MARKUP)) {
    if (markup[1]?.toLowerCase() !== 'p') continue;
    // 段落は入れ子にならない。次に現れる `<p>` か `</p>` で、最初の段落は終わる。
    if (start !== undefined) return html.slice(start, markup.index);
    if (!markup[0].startsWith('</')) start = markup.index + markup[0].length;
  }
  return start === undefined ? undefined : html.slice(start);
}

// 本文の最初の `<p>` から、タグを除き、文字参照を戻し、空白をまとめた文字を返す。
// 文字が無ければ(画像だけの段落など)`undefined` を返す。次の段落へは進まない。
// 200 字を超えるときは、200 字で切って「…」を付ける。
export function firstParagraphText(html: string): string | undefined {
  const paragraph = firstParagraphHtml(html);
  if (paragraph === undefined) return undefined;

  // タグを除いてから、文字参照を戻す。逆にすると、戻して現れた `<…>` まで除いてしまう。
  const text = decodeCharacterReferences(paragraph.replace(MARKUP, '')).replace(HTML_SPACES, ' ').trim();
  if (text === '') return undefined;

  // 符号位置で数える。サロゲートの組の途中では切れない。
  const chars = Array.from(text);
  if (chars.length <= SUMMARY_LENGTH) return text;
  return `${chars.slice(0, SUMMARY_LENGTH).join('')}…`;
}

// ページの説明を決める。
// `description`(空でなければ)→ 本文の最初の段落の文字 → `fallback`(サイトの説明文)の順に選ぶ。
export function describe(input: DescribeInput, fallback: string): string {
  if (input.description !== undefined && input.description.trim() !== '') return input.description;
  if (input.html !== undefined) {
    const text = firstParagraphText(input.html);
    if (text !== undefined) return text;
  }
  return fallback;
}

// 描いた後の本文に、KaTeX の出力があるかを返す。front matter の `use_math` は見ない。
export function hasMath(html: string | undefined): boolean {
  return html !== undefined && KATEX_OUTPUT.test(html);
}
