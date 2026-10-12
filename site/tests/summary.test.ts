// `src/lib/summary.ts` の単体テスト。
// ページの説明の決め方と、描いた後の本文に数式の出力があるかの見分けを確かめる。
// 先頭に並べた本文の HTML は、断りの無いかぎり、原稿をこのサイトの設定(`astro.config.mjs`)で描いた実物から取った。
// テストの中に直に書いた短い HTML は、「実物」と断ったもののほかは、確かめたい形に合わせて作ったもの。
import assert from 'node:assert/strict';
import { describe as suite, it } from 'node:test';

import { describe, firstParagraphText, hasMath } from '../src/lib/summary.ts';

// サイトの説明文。`describe()` には、呼ぶ側が渡す。
const SITE_DESCRIPTION =
  'このサイトは２０世紀後半に発生した、 えちょの嗜好についての諸々をgithubが続く限り残し続ける場所です。 世界のエントロピー上昇に貢献します。';

// `2014-10-04-pasta.md`。文だけの段落で始まる。行の途中で改行している。
const PASTA_2014 = `<p>完成系が見えると放置してしまう悪い癖‥‥。
ツイッターでSHIORI構文の話が出てたので、久々に引っ張り出す。</p>
<p>淡い日本語縦書きは、か細い少女にこそ、ふさわしいのだ。</p>`;

// `2016-02-16-code.md`。最初の段落に `<code>` がある。後ろにコードのブロックが続く。
const CODE_2016 = `<p>ブログ内に<code>コード断片</code>を埋め込むテスト</p>
<pre class="astro-code astro-code-themes vitesse-light vitesse-dark" style="--shiki-light:#393a34;--shiki-dark:#dbd7caee;--shiki-light-bg:#ffffff;--shiki-dark-bg:#121212; overflow-x: auto;" tabindex="0" data-language="csharp"><code><span class="line"><span style="--shiki-light:#1E754F;--shiki-dark:#4D9375">using</span><span style="--shiki-light:#2E8F82;--shiki-dark:#5DA994"> System</span><span style="--shiki-light:#999999;--shiki-dark:#666666">;</span></span></code></pre>`;

// `2016-06-18-sun.md`。4 行にわたる段落。
const SUN_2016 = `<p>今日を逃すとしばらくお日様がない！とのことで、布団一式カバーも洗って干しまくる。
そして洗濯機が回って暇な時間にちょっと色をいじってみる。
静的な縦書きコンテンツの検証も含んでますので、しばらくは毎日更新の癖を
付けようと思います。意味を求めるのはじっくり、と。</p>`;

// `2020-08-10-emo.md`。リンクつきの画像、改行、リンクつきの文が、1 つの段落にある。
const EMO_2020 = `<p><a href="https://raw.githubusercontent.com/ekicyou/emo-gs/stable/ghost/emo-gs.nar"><img src="/img/2020-08-10-emo.png" alt="ゴースト「えも？」" title="えも？"></a><br>
<a href="https://raw.githubusercontent.com/ekicyou/emo-gs/stable/ghost/emo-gs.nar">ゴースト「えも？」（←リンクでダウンロード）</a>を公開します。</p>`;

// `2024-01-01-nyan_pu_puu.md`。最初の段落が、リンクつきの画像だけ。次の段落は文。
const NYAN_2024 = `<p><a href="/nyan-pu-puu/ghost/main/nyan-pu-puu.nar"><img src="/nyan-pu-puu/images/surface0.png" alt="ごーすと「にゃんぷっぷー」" title="にゃんぷっぷー"></a></p>
<p>新年あけましておめでとうございます。
数年間ろくに更新しませんでしたが、本年も変わらぬご愛顧よろしくお願いします。
（ずうずうしいな）</p>`;

// `2026-01-06-syuzo-to.md`。段落の途中にリンクがある。
const SYUZO_2026 = `<p>ひょんなことから、<a href="https://meatlog.de10.moe/entry/character_creator_prompt">ぽなさんのキャラクターの芯を掘り下げるためのプロンプト</a>を読んでいて「修造っぽいな……」と感じた後数時間後に完成していた。公開はするが後悔はしない。</p>`;

// `2026-02-06-pasta.md`。原稿は HTML で書いた画像で始まる。新しい処理系は、この画像を `<p>` で包まない。
const PASTA_2026 = `<img src="/img/pasta.svg" alt="Pasta logo" width="120" align="left" style="margin-right: 1em;">
<p>構想10年というか、<a href="https://ekicyou.github.io/2014/10/04/pasta.html">12年前の記事</a>なんですが。。。</p>
<p>当時から考えていたDSLが、最近のAIの進歩によってなぜかアルファーリリースできるだけの完成度に到達してしまったので公開しちゃいます。</p>`;

// 切り替えの前のサイト(kramdown)が `2026-02-06-pasta.md` の先頭から作っていた形。画像を `<p>` で包む。
const PASTA_2026_OLD_FORM = `<p><img src="/img/pasta.svg" alt="Pasta logo" width="120" align="left" style="margin-right: 1em;" /></p>
<p>構想10年というか、<a href="https://ekicyou.github.io/2014/10/04/pasta.html">12年前の記事</a>なんですが。。。</p>`;

// `2025-12-06-mascot_requirements.md` の先頭と同じ形。囲み(NOTE)で始まる。
const ALERT_FIRST = `<div class="markdown-alert markdown-alert-note" dir="auto">
<p class="markdown-alert-title" dir="auto"><svg class="octicon" viewBox="0 0 16 16" width="16" height="16" aria-hidden="true"><path d="M0 8a8 8 0 1 1 16 0A8 8 0 0 1 0 8Zm8-6.5a6.5 6.5 0 1 0 0 13 6.5 6.5 0 0 0 0-13ZM6.5 7.75A.75.75 0 0 1 7.25 7h1a.75.75 0 0 1 .75.75v2.75h.25a.75.75 0 0 1 0 1.5h-2a.75.75 0 0 1 0-1.5h.25v-2h-.25a.75.75 0 0 1-.75-.75ZM8 6a1 1 0 1 1 0-2 1 1 0 0 1 0 2Z"></path></svg>NOTE</p>
<p>囲みの中の文。</p>
</div>
<p>地の文。</p>`;

// 文中の数式 1 つ(`$q$`)を含む段落。`entanglement_cosmogenesis_v7_ja.md` から。
const INLINE_MATH = `<p>電荷 <span class="katex"><span class="katex-mathml"><math xmlns="http://www.w3.org/1998/Math/MathML"><semantics><mrow><mi>q</mi></mrow><annotation encoding="application/x-tex">q</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="base"><span class="strut" style="height:0.625em;vertical-align:-0.1944em;"></span><span class="mord mathnormal" style="margin-right:0.0359em;">q</span></span></span></span> の粒子に対し、光子放出/吸収の遷移振幅：</p>`;

// 独立した行の数式 1 つ(`$$` の行で挟んだ `x`)。数式は `<p>` の外に出る。
const DISPLAY_MATH = `<p>式は次のとおり。</p>
<span class="katex-display"><span class="katex"><span class="katex-mathml"><math xmlns="http://www.w3.org/1998/Math/MathML" display="block"><semantics><mrow><mi>x</mi></mrow><annotation encoding="application/x-tex">x</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="base"><span class="strut" style="height:0.4306em;"></span><span class="mord mathnormal">x</span></span></span></span></span>`;

// KaTeX が読めなかった数式(`$\frac{1$`)。TeX の文字のまま、色を付けて出る。
const MATH_ERROR = String.raw`<p>読めない数式 <span class="katex-error" title="ParseError: KaTeX parse error: Unexpected end of input in a macro argument, expected &#x27;}&#x27; at end of input: \frac{1" style="color:#cc0000">\frac{1</span> だけ。</p>`;

// 数式の出力と同じ文字を、コードとして書いた本文(文中のコードと、言語名の無いコードのブロック)。
const KATEX_AS_CODE = `<p><code>&#x3C;span class="katex"></code> と書くと数式になる、という話。</p>
<pre class="astro-code astro-code-themes vitesse-light vitesse-dark" style="--shiki-light:#393a34;--shiki-dark:#dbd7caee;--shiki-light-bg:#ffffff;--shiki-dark-bg:#121212; overflow-x: auto;" tabindex="0" data-language="plaintext"><code><span class="line"><span>&#x3C;span class="katex">y&#x3C;/span></span></span></code></pre>`;

// 見えない文字は、ソースに直に書かず、番号から作る。
const NBSP = String.fromCodePoint(0xa0); // 改行しない空白(U+00A0)
const REPLACEMENT_CHARACTER = String.fromCodePoint(0xfffd); // 文字にならない番号の代わりに置く文字(U+FFFD)

// 符号位置の数で数えた長さ。
function countChars(text: string): number {
  return Array.from(text).length;
}

// 対になっていないサロゲートが無いことを確かめる。
function assertWellFormed(text: string): void {
  assert.doesNotMatch(text, /[\uD800-\uDBFF](?![\uDC00-\uDFFF])|(?<![\uD800-\uDBFF])[\uDC00-\uDFFF]/);
}

suite('describe: 説明を選ぶ順', () => {
  it('`description` があれば、それを使う', () => {
    assert.equal(
      describe({ description: 'ゴースト「えも？」をリリースした。', html: EMO_2020 }, SITE_DESCRIPTION),
      'ゴースト「えも？」をリリースした。',
    );
    // 本文が無くても、`description` で決まる。
    assert.equal(describe({ description: '説明' }, SITE_DESCRIPTION), '説明');
  });

  it('`description` が無ければ、本文の最初の段落の文字を使う', () => {
    // 期待する文字は、切り替えの前のサイトが出していた説明と同じ。
    const cases: ReadonlyArray<readonly [html: string, text: string]> = [
      [PASTA_2014, '完成系が見えると放置してしまう悪い癖‥‥。 ツイッターでSHIORI構文の話が出てたので、久々に引っ張り出す。'],
      [CODE_2016, 'ブログ内にコード断片を埋め込むテスト'],
      [
        SUN_2016,
        '今日を逃すとしばらくお日様がない！とのことで、布団一式カバーも洗って干しまくる。 そして洗濯機が回って暇な時間にちょっと色をいじってみる。 静的な縦書きコンテンツの検証も含んでますので、しばらくは毎日更新の癖を 付けようと思います。意味を求めるのはじっくり、と。',
      ],
    ];
    for (const [html, text] of cases) {
      assert.equal(describe({ html }, SITE_DESCRIPTION), text);
      assert.equal(describe({ description: undefined, html }, SITE_DESCRIPTION), text);
    }
  });

  it('`description` も最初の段落の文字も無ければ、サイトの説明文を使う', () => {
    assert.equal(describe({}, SITE_DESCRIPTION), SITE_DESCRIPTION);
    assert.equal(describe({ html: undefined }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    assert.equal(describe({ html: '' }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    // 段落が 1 つも無い本文(見出しとコードだけ)。
    assert.equal(
      describe({ html: '<h2 id="x">見出し</h2>\n<pre><code>コード</code></pre>' }, SITE_DESCRIPTION),
      SITE_DESCRIPTION,
    );
  });

  it('最初の段落が画像だけのときは、サイトの説明文になる(次の段落へは進まない)', () => {
    assert.equal(describe({ html: NYAN_2024 }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    assert.equal(describe({ html: PASTA_2026_OLD_FORM }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    // 画像だけの段落で始まっても、`description` があれば、それを使う。
    assert.equal(
      describe({ description: 'ゴースト「にゃんぷっぷー」をリリースします。', html: NYAN_2024 }, SITE_DESCRIPTION),
      'ゴースト「にゃんぷっぷー」をリリースします。',
    );
  });

  it('空の `description` は、無いものとして扱う', () => {
    assert.equal(describe({ description: '', html: CODE_2016 }, SITE_DESCRIPTION), 'ブログ内にコード断片を埋め込むテスト');
    assert.equal(describe({ description: '', html: NYAN_2024 }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    assert.equal(describe({ description: '' }, SITE_DESCRIPTION), SITE_DESCRIPTION);
    // 空白だけの `description` も、空として扱う。
    assert.equal(describe({ description: ' \n\t', html: CODE_2016 }, SITE_DESCRIPTION), 'ブログ内にコード断片を埋め込むテスト');
    assert.equal(describe({ description: '　' }, SITE_DESCRIPTION), SITE_DESCRIPTION);
  });

  it('`description` とサイトの説明文には、手を加えない', () => {
    // 切らない。タグや文字参照に見える文字も、そのまま返す。
    const long = 'あ'.repeat(300);
    assert.equal(describe({ description: long, html: CODE_2016 }, SITE_DESCRIPTION), long);
    assert.equal(describe({ description: 'A &amp; B の <b>話</b>' }, SITE_DESCRIPTION), 'A &amp; B の <b>話</b>');
    assert.equal(describe({}, long), long);
    assert.equal(describe({ html: NYAN_2024 }, 'A &amp; B'), 'A &amp; B');
  });

  it('最初の段落の文字は、200 字で切る', () => {
    const text = describe({ html: `<p>${'あ'.repeat(250)}</p>` }, SITE_DESCRIPTION);
    assert.equal(text, `${'あ'.repeat(200)}…`);
  });
});

suite('firstParagraphText: 最初の段落', () => {
  it('本文の最初の `<p>` の中身だけを読む', () => {
    assert.equal(
      firstParagraphText(PASTA_2014),
      '完成系が見えると放置してしまう悪い癖‥‥。 ツイッターでSHIORI構文の話が出てたので、久々に引っ張り出す。',
    );
    // 見出しの後の段落。
    assert.equal(
      firstParagraphText('<h1 id="えちょの残骸">えちょの残骸</h1>\n<p>新旧の開発物を、後世に残す場所。</p>\n<ul>\n<li><a href="/x/">x</a></li>\n</ul>'),
      '新旧の開発物を、後世に残す場所。',
    );
  });

  it('`<p>` が無ければ `undefined` を返す', () => {
    assert.equal(firstParagraphText(''), undefined);
    assert.equal(firstParagraphText('段落でない文字'), undefined);
    assert.equal(firstParagraphText('<h2 id="x">見出し</h2>\n<ul>\n<li>項目</li>\n</ul>'), undefined);
  });

  it('`<pre>` や `<path>` を、段落と取り違えない', () => {
    assert.equal(firstParagraphText('<pre><code>コード</code></pre>\n<p>文。</p>'), '文。');
    assert.equal(firstParagraphText('<svg viewBox="0 0 1 1"><path d="M0 0"></path></svg>\n<p>文。</p>'), '文。');
    assert.equal(firstParagraphText('<picture><img src="/a.png" alt="絵"></picture>\n<p>文。</p>'), '文。');
  });

  it('属性の付いた `<p>` も、段落として読む', () => {
    assert.equal(firstParagraphText('<p class="lead" title="a > b">生の <em>HTML</em> の段落</p>\n<p>文。</p>'), '生の HTML の段落');
    // 囲みで始まる本文では、囲みの見出しが最初の `<p>` になる。
    assert.equal(firstParagraphText(ALERT_FIRST), 'NOTE');
  });

  it('`<p>` の外に置いた画像は、段落として数えない', () => {
    // 切り替えの前のサイトは、この画像を `<p>` で包み、サイトの説明文を出していた。
    assert.equal(firstParagraphText(PASTA_2026), '構想10年というか、12年前の記事なんですが。。。');
  });

  it('コメントの中の `<p>` は、段落として数えない', () => {
    assert.equal(firstParagraphText('<!-- <p>消した段落</p> -->\n<p>本当の最初の段落。</p>'), '本当の最初の段落。');
  });

  it('属性の値の中の `<p>` は、段落として数えない', () => {
    assert.equal(firstParagraphText('<img src="/a.png" alt="<p>絵の説明</p>">\n<p>文。</p>'), '文。');
  });
});

suite('firstParagraphText: タグを除く', () => {
  it('リンク・強調・コード・改行のタグを除き、中の文字を残す', () => {
    assert.equal(
      firstParagraphText(SYUZO_2026),
      'ひょんなことから、ぽなさんのキャラクターの芯を掘り下げるためのプロンプトを読んでいて「修造っぽいな……」と感じた後数時間後に完成していた。公開はするが後悔はしない。',
    );
    assert.equal(firstParagraphText(CODE_2016), 'ブログ内にコード断片を埋め込むテスト');
    // `entanglement_cosmogenesis_v1_ja.md` の先頭と同じ形(`<strong>` と `<br>`)。
    assert.equal(
      firstParagraphText('<p><strong>著者：</strong> ekicyou¹<br>\n<strong>所属：</strong> ¹ Independent Researcher, Japan</p>'),
      '著者： ekicyou¹ 所属： ¹ Independent Researcher, Japan',
    );
    // タグは、空白に置き換えない。`<br>` の前後がつながる。
    assert.equal(firstParagraphText('<p>一行目<br>二行目</p>'), '一行目二行目');
  });

  it('画像は文字として数えない(`alt` と `title` を読まない)', () => {
    assert.equal(firstParagraphText(EMO_2020), 'ゴースト「えも？」（←リンクでダウンロード）を公開します。');
    assert.equal(firstParagraphText(NYAN_2024), undefined);
    assert.equal(firstParagraphText(PASTA_2026_OLD_FORM), undefined);
  });

  it('属性の値に `>` や引用符の文字参照があっても、タグの終わりを取り違えない', () => {
    // `![a > b "x"](/img/x.png "t > u")` を描いた実物。
    assert.equal(firstParagraphText('<p><img src="/img/x.png" alt="a > b &#x22;x&#x22;" title="t > u"></p>\n<p>文。</p>'), undefined);
    assert.equal(firstParagraphText(`<p><a href="/x" title='a > "b"'>リンク</a>の文</p>`), 'リンクの文');
  });

  it('段落の中のコメントを除く', () => {
    assert.equal(firstParagraphText('<p>文の<!-- 注 > 注 -->途中。</p>'), '文の途中。');
    assert.equal(firstParagraphText('<p><!-- 注だけ --></p>\n<p>次。</p>'), undefined);
  });

  it('文中の数式は、KaTeX の出力に含まれる文字がそのまま並ぶ', () => {
    // MathML の文字、TeX の文字、表示用の文字の 3 つ(どれも `q`)。
    assert.equal(firstParagraphText(INLINE_MATH), '電荷 qqq の粒子に対し、光子放出/吸収の遷移振幅：');
  });

  it('タグでない `<` と `>` は、文字として残す', () => {
    assert.equal(firstParagraphText('<p>1 < 2 > 0、a<1、<3</p>'), '1 < 2 > 0、a<1、<3');
  });
});

suite('firstParagraphText: 文字参照を戻す', () => {
  it('処理系が出す文字参照(`&#x3C;`・`&#x26;`)を戻す', () => {
    // 原稿「コード `a < b && c > d` と `<p>` の話。」を描いた実物。
    assert.equal(
      firstParagraphText('<p>コード <code>a &#x3C; b &#x26;&#x26; c > d</code> と <code>&#x3C;p></code> の話。</p>\n<p>次。</p>'),
      'コード a < b && c > d と <p> の話。',
    );
    assert.equal(
      firstParagraphText('<p><a href="https://example.com/a?b=1&#x26;c=2">https://example.com/a?b=1&#x26;c=2</a> を見る。</p>'),
      'https://example.com/a?b=1&c=2 を見る。',
    );
  });

  it('名前の文字参照(`amp`・`lt`・`gt`・`quot`・`apos`・`nbsp`)を戻す', () => {
    assert.equal(
      firstParagraphText('<p>Tom &amp; Jerry、a &lt; b &gt; c、&quot;二重&quot;、&apos;一重&apos;、A&nbsp;B</p>'),
      `Tom & Jerry、a < b > c、"二重"、'一重'、A${NBSP}B`,
    );
  });

  it('数字の文字参照(10 進と 16 進)を戻す', () => {
    assert.equal(firstParagraphText('<p>&#39;&#x27;&#X27;&#x22;&#34;</p>'), '\'\'\'""');
    assert.equal(firstParagraphText('<p>&#169; &#xA9; &#xa9; &#12354;&#x3042;</p>'), '© © © ああ');
    // 基本多言語面の外の文字(サロゲートの組になる)。
    assert.equal(firstParagraphText('<p>&#x1F600;&#128512;&#x20BB7;野家</p>'), '😀😀𠮷野家');
  });

  it('戻すのは 1 度だけ(戻した後の文字を、もう 1 度読まない)', () => {
    assert.equal(firstParagraphText('<p>&#x26;lt; &amp;amp; &amp;#x3C; &#38;#38;</p>'), '&lt; &amp; &#x3C; &#38;');
  });

  it('戻して現れた `<…>` を、タグとして除かない', () => {
    assert.equal(firstParagraphText('<p>&lt;b&gt;太字&lt;/b&gt; と &#x3C;p> の話</p>'), '<b>太字</b> と <p> の話');
  });

  it('知らない名前と、文字参照の形でないものは、そのまま残す', () => {
    assert.equal(firstParagraphText('<p>&unknown; &constructor; &toString; &amp &#; &#x; &#xZZ; & ; A&B</p>'), '&unknown; &constructor; &toString; &amp &#; &#x; &#xZZ; & ; A&B');
  });

  it('文字にならない番号は、U+FFFD に替える(誤りを投げない)', () => {
    assert.equal(firstParagraphText('<p>a&#0;b&#xD800;c&#xDFFF;d&#x110000;e&#99999999999999999999;f</p>'), ['a', 'b', 'c', 'd', 'e', 'f'].join(REPLACEMENT_CHARACTER));
    // 境目の番号は文字になる。
    assert.equal(firstParagraphText('<p>&#xD7FF;&#xE000;&#x10FFFF;</p>'), String.fromCodePoint(0xd7ff, 0xe000, 0x10ffff));
  });
});

suite('firstParagraphText: 空白をまとめる', () => {
  it('改行・タブ・続いた空白を、空白 1 つにまとめる', () => {
    assert.equal(firstParagraphText('<p>一行目\n二行目\r\n三行目\t四  つ   め</p>'), '一行目 二行目 三行目 四 つ め');
    assert.equal(firstParagraphText(SUN_2016)?.includes('\n'), false);
  });

  it('前後の空白を落とす', () => {
    assert.equal(firstParagraphText('<p>\n  文。 \n</p>'), '文。');
    assert.equal(firstParagraphText('<p> <strong>太字</strong> </p>'), '太字');
    // `2016-07-08-dcomp.md` の最初の段落は、全角の空白で始まる。
    assert.equal(firstParagraphText('<p>　かっこいいエフェクトをシェルにかぶせたい！</p>'), 'かっこいいエフェクトをシェルにかぶせたい！');
  });

  it('文の途中の全角の空白は、そのまま残す', () => {
    assert.equal(firstParagraphText('<p>「＠おはよう　」（最後は全角空白）</p>'), '「＠おはよう　」（最後は全角空白）');
    assert.equal(firstParagraphText('<p>一　要約　　二</p>'), '一　要約　　二');
  });

  it('文字参照から戻った空白も、まとめる', () => {
    assert.equal(firstParagraphText('<p>a&#32;&#x20;&#10;b</p>'), 'a b');
  });

  it('文字が無ければ `undefined` を返す', () => {
    assert.equal(firstParagraphText('<p></p>\n<p>次。</p>'), undefined);
    assert.equal(firstParagraphText('<p> \n\t </p>\n<p>次。</p>'), undefined);
    assert.equal(firstParagraphText('<p>&nbsp;</p>\n<p>次。</p>'), undefined);
    assert.equal(firstParagraphText('<p>　</p>\n<p>次。</p>'), undefined);
    assert.equal(firstParagraphText('<p><br></p>\n<p>次。</p>'), undefined);
  });
});

suite('firstParagraphText: 200 字で切る', () => {
  it('200 字までは、切らない', () => {
    assert.equal(firstParagraphText(`<p>${'あ'.repeat(199)}</p>`), 'あ'.repeat(199));
    assert.equal(firstParagraphText(`<p>${'あ'.repeat(200)}</p>`), 'あ'.repeat(200));
  });

  it('200 字を超えるときは、200 字で切って「…」を付ける', () => {
    const text = firstParagraphText(`<p>${'あ'.repeat(201)}</p>`);
    assert.equal(text, `${'あ'.repeat(200)}…`);
    assert.equal(countChars(text ?? ''), 201);

    // 切る位置の前後で、文字が変わる文。
    const mixed = `${'日本語の文。'.repeat(33)}あい`; // 6 字 × 33 + 2 字 = 200 字
    assert.equal(countChars(mixed), 200);
    assert.equal(firstParagraphText(`<p>${mixed}</p>`), mixed);
    assert.equal(firstParagraphText(`<p>${mixed}う</p>`), `${mixed}…`);
  });

  it('字の数は、タグを除き、文字参照を戻し、空白をまとめた後で数える', () => {
    // 見える文字は 200 字。タグと文字参照の綴りは数えない。
    const html = `<p><strong>${'あ'.repeat(99)}</strong>&#x26;\n\n  <a href="/x">${'い'.repeat(99)}</a></p>`;
    const text = `${'あ'.repeat(99)}& ${'い'.repeat(99)}`;
    assert.equal(countChars(text), 200);
    assert.equal(firstParagraphText(html), text);
    assert.equal(firstParagraphText(html.replace('</a>', '</a>う')), `${text}…`);
  });

  it('サロゲートの組になる文字は、1 字と数える', () => {
    // 200 字ちょうど(UTF-16 では 201)。切らない。
    const exact = `${'あ'.repeat(199)}😀`;
    assert.equal(exact.length, 201);
    assert.equal(countChars(exact), 200);
    assert.equal(firstParagraphText(`<p>${exact}</p>`), exact);

    // 途中に 2 つ(UTF-16 では 202)あっても、200 字ちょうどなら切らない。
    const two = `𠮷${'あ'.repeat(198)}😀`;
    assert.equal(countChars(two), 200);
    assert.equal(firstParagraphText(`<p>${two}</p>`), two);
  });

  it('サロゲートの組の途中で切らない', () => {
    // 200 字めが絵文字。その後ろで切る。
    const atCut = firstParagraphText(`<p>${'あ'.repeat(199)}😀い</p>`);
    assert.equal(atCut, `${'あ'.repeat(199)}😀…`);
    assertWellFormed(atCut ?? '');

    // 201 字めが絵文字。その前で切る。
    const afterCut = firstParagraphText(`<p>${'あ'.repeat(200)}😀</p>`);
    assert.equal(afterCut, `${'あ'.repeat(200)}…`);
    assertWellFormed(afterCut ?? '');

    // 前のほうに 1 つあり、UTF-16 の 200 個めが組の途中に当たる文。
    const shifted = firstParagraphText(`<p>𠮷${'あ'.repeat(197)}😀${'い'.repeat(10)}</p>`);
    assert.equal(shifted, `𠮷${'あ'.repeat(197)}😀い…`);
    assert.equal(countChars(shifted ?? ''), 201);
    assertWellFormed(shifted ?? '');
  });
});

suite('hasMath: 数式の有無', () => {
  it('文中の数式の出力があれば、真', () => {
    assert.equal(hasMath(INLINE_MATH), true);
    assert.equal(hasMath(`${PASTA_2014}\n${INLINE_MATH}`), true);
  });

  it('独立した行の数式の出力があれば、真', () => {
    assert.equal(hasMath(DISPLAY_MATH), true);
  });

  it('数式の出力が無ければ、偽', () => {
    for (const html of [PASTA_2014, CODE_2016, EMO_2020, NYAN_2024, PASTA_2026, ALERT_FIRST]) {
      assert.equal(hasMath(html), false);
    }
    // 原稿「値段は \$5 と \$6。数式は無い。」を描いた実物。
    assert.equal(hasMath('<p>値段は $5 と $6。数式は無い。</p>'), false);
  });

  it('本文が無ければ、偽', () => {
    assert.equal(hasMath(undefined), false);
    assert.equal(hasMath(''), false);
  });

  it('数式の出力と同じ文字をコードとして書いただけでは、偽', () => {
    assert.equal(hasMath(KATEX_AS_CODE), false);
    assert.equal(hasMath('<p>KaTeX と katex と class="katex" の話。</p>'), false);
  });

  it('`katex` で始まるだけの別の名前は、数式の出力と数えない', () => {
    // 読めなかった数式は、KaTeX のスタイルシートが無くても表示できる。
    assert.equal(hasMath(MATH_ERROR), false);
    assert.equal(hasMath('<p><span class="katex-html">x</span><span class="katexy">y</span></p>'), false);
    assert.equal(hasMath('<p><span data-class="katex">x</span><spanner class="katex">y</spanner></p>'), false);
    assert.equal(hasMath(`<p><span title='class="katex"'>x</span></p>`), false);
    // 読めなかった数式と、描けた数式の両方があれば、真。
    assert.equal(hasMath(`${MATH_ERROR}\n${INLINE_MATH}`), true);
  });

  it('`class` に `katex` という名前があれば、ほかの名前や属性が並んでいても、真', () => {
    // KaTeX の今の出力には無い形。出力の書き方が少し変わっても、見逃さないことを確かめる。
    assert.equal(hasMath('<p><span class="math katex">x</span></p>'), true);
    assert.equal(hasMath('<p><span class="katex math">x</span></p>'), true);
    assert.equal(hasMath('<p><span aria-label="a > b" class="katex">x</span></p>'), true);
    assert.equal(hasMath('<p><span\n  class="katex">x</span></p>'), true);
  });

  it('`use_math` を見ない(本文だけで決まる)', () => {
    assert.equal(hasMath.length, 1);
    // `use_math: true` と書いたが、数式が描かれていないページ。
    const flaggedWithoutMath = { data: { use_math: true }, html: PASTA_2014 };
    // `use_math` を書いていないが、数式が描かれているページ。
    const unflaggedWithMath = { data: {} as { use_math?: boolean }, html: DISPLAY_MATH };
    assert.equal(hasMath(flaggedWithoutMath.html), false);
    assert.equal(hasMath(unflaggedWithMath.html), true);
  });
});
